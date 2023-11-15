#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "aiio_chip_spec.h"
#include <sys/types.h>
// #include <sys/socket.h>
#include <netdb.h>
#define aiio_err_to_name(x) "err"

#include <http_parser.h>
#include "aiio_tls_wolfssl.h"
#include "aiio_tls_error_capture_internal.h"
#include <errno.h>
#include "aiio_log.h"
#if CONFIG_TLS_USING_WOLFSSL
enum { /* ssl Constants */
    WOLFSSL_ERROR_NONE      =  0,   /* for most functions */
    WOLFSSL_FAILURE         =  0,   /* for some functions */
    WOLFSSL_SUCCESS         =  1,
    WOLFSSL_SHUTDOWN_NOT_DONE =  2,  /* call wolfSSL_shutdown again to complete */

    WOLFSSL_ALPN_NOT_FOUND  = -9,
    WOLFSSL_BAD_CERTTYPE    = -8,
    WOLFSSL_BAD_STAT        = -7,
    WOLFSSL_BAD_PATH        = -6,
    WOLFSSL_BAD_FILETYPE    = -5,
    WOLFSSL_BAD_FILE        = -4,
    WOLFSSL_NOT_IMPLEMENTED = -3,
    WOLFSSL_UNKNOWN         = -2,
    WOLFSSL_FATAL_ERROR     = -1,

    WOLFSSL_FILETYPE_ASN1    = 2,
    WOLFSSL_FILETYPE_PEM     = 1,
    WOLFSSL_FILETYPE_DEFAULT = 2, /* ASN1 */
    WOLFSSL_FILETYPE_RAW     = 3, /* NTRU raw key blob */

    WOLFSSL_VERIFY_NONE                 = 0,
    WOLFSSL_VERIFY_PEER                 = 1,
    WOLFSSL_VERIFY_FAIL_IF_NO_PEER_CERT = 2,
    WOLFSSL_VERIFY_CLIENT_ONCE          = 4,
    WOLFSSL_VERIFY_FAIL_EXCEPT_PSK      = 8,

    WOLFSSL_SESS_CACHE_OFF                = 0x0000,
    WOLFSSL_SESS_CACHE_CLIENT             = 0x0001,
    WOLFSSL_SESS_CACHE_SERVER             = 0x0002,
    WOLFSSL_SESS_CACHE_BOTH               = 0x0003,
    WOLFSSL_SESS_CACHE_NO_AUTO_CLEAR      = 0x0008,
    WOLFSSL_SESS_CACHE_NO_INTERNAL_LOOKUP = 0x0100,
    WOLFSSL_SESS_CACHE_NO_INTERNAL_STORE  = 0x0200,
    WOLFSSL_SESS_CACHE_NO_INTERNAL        = 0x0300,

    WOLFSSL_ERROR_WANT_READ        =  2,
    WOLFSSL_ERROR_WANT_WRITE       =  3,
    WOLFSSL_ERROR_WANT_CONNECT     =  7,
    WOLFSSL_ERROR_WANT_ACCEPT      =  8,
    WOLFSSL_ERROR_SYSCALL          =  5,
    WOLFSSL_ERROR_WANT_X509_LOOKUP = 83,
    WOLFSSL_ERROR_ZERO_RETURN      =  6,
    WOLFSSL_ERROR_SSL              = 85,

    WOLFSSL_SENT_SHUTDOWN     = 1,
    WOLFSSL_RECEIVED_SHUTDOWN = 2,
    WOLFSSL_MODE_ACCEPT_MOVING_WRITE_BUFFER = 4,
    WOLFSSL_OP_NO_SSLv2       = 8,

    WOLFSSL_R_SSL_HANDSHAKE_FAILURE           = 101,
    WOLFSSL_R_TLSV1_ALERT_UNKNOWN_CA          = 102,
    WOLFSSL_R_SSLV3_ALERT_CERTIFICATE_UNKNOWN = 103,
    WOLFSSL_R_SSLV3_ALERT_BAD_CERTIFICATE     = 104,

    WOLF_PEM_BUFSIZE = 1024
};
static unsigned char *global_cacert = NULL;
static unsigned int global_cacert_pem_bytes = 0;
static const char *TAG = "aiio-tls-wolfssl";

/* Prototypes for the static functions */
static int set_client_config(const char *hostname, size_t hostlen, aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

#if defined(CONFIG_TLS_PSK_VERIFICATION)
#include "aiio_chip_spec.h"
static SemaphoreHandle_t tls_conn_lock;
static inline unsigned int aiio_wolfssl_psk_client_cb(WOLFSSL* ssl, const char* hint, char* identity,
        unsigned int id_max_len, unsigned char* key,unsigned int key_max_len);
static int aiio_wolfssl_set_cipher_list(WOLFSSL_CTX *ctx);
#ifdef WOLFSSL_TLS13
#define PSK_MAX_ID_LEN 128
#else
#define PSK_MAX_ID_LEN 64
#endif
#define PSK_MAX_KEY_LEN 64

static char psk_id_str[PSK_MAX_ID_LEN];
static uint8_t psk_key_array[PSK_MAX_KEY_LEN];
static uint8_t psk_key_max_len = 0;
#endif /* CONFIG_TLS_PSK_VERIFICATION */

#ifdef CONFIG_TLS_SERVER
static int set_server_config(aiio_tls_cfg_server_t *cfg, aiio_tls_t *tls);
#endif /* CONFIG_TLS_SERVER */


/* This function shall return the error message when appropriate log level has been set otherwise this function shall do nothing */
static void wolfssl_print_error_msg(int error)
{
#if (CONFIG_LOG_DEFAULT_LEVEL_DEBUG || CONFIG_LOG_DEFAULT_LEVEL_VERBOSE)
    static char error_buf[100];
    aiio_log_e( "(%d) : %s", error, ERR_error_string(error, error_buf));
#endif
}

typedef enum x509_file_type {
    FILE_TYPE_CA_CERT = 0, /* CA certificate to authenticate entity at other end */
    FILE_TYPE_SELF_CERT, /* Self certificate of the entity */
    FILE_TYPE_SELF_KEY, /* Private key in the self cert-key pair */
} x509_file_type_t;

__weak int wolfSSL_CTX_use_PrivateKey_buffer(WOLFSSL_CTX *,
                                      const unsigned char *, long, int)
{
    return 0;
}
/* Error type conversion utility so that aiio-tls read/write API to return negative number on error */
static inline ssize_t aiio_tls_convert_wolfssl_err_to_ssize(int wolfssl_error)
{
    switch (wolfssl_error) {
        case WOLFSSL_ERROR_WANT_READ:
            return AIIO_TLS_ERR_SSL_WANT_READ;
        case WOLFSSL_ERROR_WANT_WRITE:
            return AIIO_TLS_ERR_SSL_WANT_WRITE;
        default:
            // Make sure we return a negative number
            return wolfssl_error>0 ? -wolfssl_error: wolfssl_error;
    }
}

/* Checks whether the certificate provided is in pem format or not */
static int aiio_load_wolfssl_verify_buffer(aiio_tls_t *tls, const unsigned char *cert_buf, unsigned int cert_len, x509_file_type_t type, int *err_ret)
{
    int wolf_fileformat = WOLFSSL_FILETYPE_DEFAULT;
    if (type == FILE_TYPE_SELF_KEY) {
        if (cert_buf[cert_len - 1] == '\0' && strstr( (const char *) cert_buf, "-----BEGIN " )) {
            wolf_fileformat = WOLFSSL_FILETYPE_PEM;
        } else {
            wolf_fileformat = WOLFSSL_FILETYPE_ASN1;
        }
        if ((*err_ret = wolfSSL_CTX_use_PrivateKey_buffer( (WOLFSSL_CTX *)tls->priv_ctx, cert_buf, cert_len, wolf_fileformat)) == WOLFSSL_SUCCESS) {
            return AIIO_OK;
        }
        return AIIO_FAIL;
    } else {
        if (cert_buf[cert_len - 1] == '\0' && strstr( (const char *) cert_buf, "-----BEGIN CERTIFICATE-----" )) {
            wolf_fileformat = WOLFSSL_FILETYPE_PEM;
        } else {
            wolf_fileformat = WOLFSSL_FILETYPE_ASN1;
        }
        if (type == FILE_TYPE_SELF_CERT) {
            if ((*err_ret = wolfSSL_CTX_use_certificate_buffer( (WOLFSSL_CTX *)tls->priv_ctx, cert_buf, cert_len, wolf_fileformat)) == WOLFSSL_SUCCESS) {
                return AIIO_OK;
            }
            return AIIO_FAIL;
        } else if (type == FILE_TYPE_CA_CERT) {
            if ((*err_ret = wolfSSL_CTX_load_verify_buffer( (WOLFSSL_CTX *)tls->priv_ctx, cert_buf, cert_len, wolf_fileformat)) == WOLFSSL_SUCCESS) {
                return AIIO_OK;
            }
            return AIIO_FAIL;
        } else {
            /* Wrong file type provided */
            return AIIO_FAIL;
        }
    }
}

int aiio_create_wolfssl_handle(const char *hostname, size_t hostlen, const void *cfg, aiio_tls_t *tls)
{
#ifdef CONFIG_DEBUG_WOLFSSL
    wolfSSL_Debugging_ON();
#endif

    assert(cfg != NULL);
    assert(tls != NULL);

    int aiio_ret = AIIO_FAIL;
    int ret;

    ret = wolfSSL_Init();
    if (ret != WOLFSSL_SUCCESS) {
        aiio_log_e( "Init wolfSSL failed: 0x%04X", ret);
        wolfssl_print_error_msg(ret);
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
        goto exit;
    }

    if (tls->role == AIIO_TLS_CLIENT) {
        aiio_ret = set_client_config(hostname, hostlen, (aiio_tls_cfg_t *)cfg, tls);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set client configurations, [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
            goto exit;
        }
    } else if (tls->role == AIIO_TLS_SERVER) {
#ifdef CONFIG_TLS_SERVER
        aiio_ret = set_server_config((aiio_tls_cfg_server_t *) cfg, tls);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set server configurations, [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
            goto exit;
        }
#else
        aiio_log_e( "AIIO_TLS_SERVER Not enabled in menuconfig");
        goto exit;
#endif
    }
    else {
        aiio_log_e( "tls->role is not valid");
        goto exit;
    }

    return AIIO_OK;
exit:
    aiio_wolfssl_cleanup(tls);
    return aiio_ret;
}

static int set_client_config(const char *hostname, size_t hostlen, aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    int ret = WOLFSSL_FAILURE;

#ifdef WOLFSSL_TLS13
    tls->priv_ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_3_client_method());
#else
    tls->priv_ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_2_client_method());
#endif

    if (!tls->priv_ctx) {
        aiio_log_e( "Set wolfSSL ctx failed");
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, ret);
        return AIIO_ERR_WOLFSSL_CTX_SETUP_FAILED;
    }

    if (cfg->crt_bundle_attach != NULL) {
        aiio_log_e("use_crt_bundle not supported in wolfssl");
        return AIIO_FAIL;
    }

    if (cfg->use_global_ca_store == true) {
        if ((aiio_load_wolfssl_verify_buffer(tls, global_cacert, global_cacert_pem_bytes, FILE_TYPE_CA_CERT, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading certificate verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
        wolfSSL_CTX_set_verify( (WOLFSSL_CTX *)tls->priv_ctx, WOLFSSL_VERIFY_PEER, NULL);
    } else if (cfg->cacert_buf != NULL) {
        if ((aiio_load_wolfssl_verify_buffer(tls, cfg->cacert_buf, cfg->cacert_bytes, FILE_TYPE_CA_CERT, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading certificate verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
        wolfSSL_CTX_set_verify( (WOLFSSL_CTX *)tls->priv_ctx, WOLFSSL_VERIFY_PEER, NULL);
    } else if (cfg->psk_hint_key) {
#if defined(CONFIG_TLS_PSK_VERIFICATION)
        /*** PSK encryption mode is configured only if no certificate supplied and psk pointer not null ***/
        if(cfg->psk_hint_key->key == NULL || cfg->psk_hint_key->hint == NULL || cfg->psk_hint_key->key_size <= 0) {
            aiio_log_e( "Please provide appropriate key, keysize and hint to use PSK");
            return AIIO_FAIL;
        }
        /* mutex is given back when call back function executes or in case of failure (at cleanup) */
        if ((xSemaphoreTake(tls_conn_lock, 1000/portTICK_PERIOD_MS) != pdTRUE)) {
            aiio_log_e( "tls_conn_lock could not be obtained in specified time");
            return -1;
        }
        aiio_log_i( "setting psk configurations");
        if((cfg->psk_hint_key->key_size > PSK_MAX_KEY_LEN) || (strlen(cfg->psk_hint_key->hint) > PSK_MAX_ID_LEN)) {
            aiio_log_e( "psk key length should be <= %d and identity hint length should be <= %d", PSK_MAX_KEY_LEN, PSK_MAX_ID_LEN);
            return AIIO_ERR_INVALID_ARG;
        }
        psk_key_max_len = cfg->psk_hint_key->key_size;
        memset(psk_key_array, 0, sizeof(psk_key_array));
        memset(psk_id_str, 0, sizeof(psk_id_str));
        memcpy(psk_key_array, cfg->psk_hint_key->key, psk_key_max_len);
        memcpy(psk_id_str, cfg->psk_hint_key->hint, strlen(cfg->psk_hint_key->hint));
        wolfSSL_CTX_set_psk_client_callback( (WOLFSSL_CTX *)tls->priv_ctx, aiio_wolfssl_psk_client_cb);
        if(aiio_wolfssl_set_cipher_list( (WOLFSSL_CTX *)tls->priv_ctx) != AIIO_OK) {
            aiio_log_e( "error in setting cipher-list");
            return AIIO_FAIL;
        }
#else
        aiio_log_e( "psk_hint_key configured but not enabled in menuconfig: Please enable AIIO_TLS_PSK_VERIFICATION option");
        return AIIO_ERR_INVALID_STATE;
#endif
    } else {
#ifdef CONFIG_TLS_SKIP_SERVER_CERT_VERIFY
        wolfSSL_CTX_set_verify( (WOLFSSL_CTX *)tls->priv_ctx, WOLFSSL_VERIFY_NONE, NULL);
#else
        aiio_log_e( "No server verification option set in aiio_tls_cfg_t structure. Check aiio_tls API reference");
        return AIIO_ERR_WOLFSSL_SSL_SETUP_FAILED;
#endif
    }

    if (cfg->clientcert_buf != NULL && cfg->clientkey_buf != NULL) {
        if ((aiio_load_wolfssl_verify_buffer(tls,cfg->clientcert_buf, cfg->clientcert_bytes, FILE_TYPE_SELF_CERT, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading certificate verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
        if ((aiio_load_wolfssl_verify_buffer(tls,cfg->clientkey_buf, cfg->clientkey_bytes, FILE_TYPE_SELF_KEY, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading private key verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
    } else if (cfg->clientcert_buf != NULL || cfg->clientkey_buf != NULL) {
        aiio_log_e( "You have to provide both clientcert_buf and clientkey_buf for mutual authentication\n\n");
        return AIIO_FAIL;
    }

    tls->priv_ssl =(void *)wolfSSL_new( (WOLFSSL_CTX *)tls->priv_ctx);
    if (!tls->priv_ssl) {
        aiio_log_e( "Create wolfSSL failed");
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
        return AIIO_ERR_WOLFSSL_SSL_SETUP_FAILED;
    }

    if (!cfg->skip_common_name) {
        char *use_host = NULL;
        if (cfg->common_name != NULL) {
            use_host = strdup(cfg->common_name);
        } else {
            use_host = strndup(hostname, hostlen);
        }
        if (use_host == NULL) {
            return AIIO_ERR_NO_MEM;
        }
        /* Hostname set here should match CN in server certificate */
        if ((ret = (wolfSSL_check_domain_name( (WOLFSSL *)tls->priv_ssl, use_host))) != WOLFSSL_SUCCESS) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "wolfSSL_check_domain_name returned %d, error code: %d", ret, err);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
            free(use_host);
            return AIIO_ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED;
        }
        free(use_host);
    }

    if (cfg->alpn_protos) {
#ifdef CONFIG_WOLFSSL_HAVE_ALPN
        char **alpn_list = (char **)cfg->alpn_protos;
        for (; *alpn_list != NULL; alpn_list ++) {
            aiio_log_d( "alpn protocol is %s", *alpn_list);
            if ((ret = wolfSSL_UseALPN( (WOLFSSL *)tls->priv_ssl, *alpn_list, strlen(*alpn_list), WOLFSSL_ALPN_FAILED_ON_MISMATCH)) != WOLFSSL_SUCCESS) {
                int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
                AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
                aiio_log_e( "wolfSSL UseALPN failed, returned %d, error code: %d", ret, err);
                wolfssl_print_error_msg(err);
                return AIIO_ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED;
            }
        }
#else
    aiio_log_e( "CONFIG_WOLFSSL_HAVE_ALPN not enabled in menuconfig");
    return AIIO_FAIL;
#endif /* CONFIG_WOLFSSL_HAVE_ALPN */
    }

    wolfSSL_set_fd((WOLFSSL *)tls->priv_ssl, tls->sockfd);
    return AIIO_OK;
}

#ifdef CONFIG_TLS_SERVER
static int set_server_config(aiio_tls_cfg_server_t *cfg, aiio_tls_t *tls)
{
    int ret = WOLFSSL_FAILURE;

#ifdef WOLFSSL_TLS13
    tls->priv_ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_3_server_method());
#else
    tls->priv_ctx = (void *)wolfSSL_CTX_new(wolfTLSv1_2_server_method());
#endif

    if (!tls->priv_ctx) {
        aiio_log_e( "Set wolfSSL ctx failed");
        return AIIO_ERR_WOLFSSL_CTX_SETUP_FAILED;
    }

    if (cfg->cacert_buf != NULL) {
        if ((aiio_load_wolfssl_verify_buffer(tls,cfg->cacert_buf, cfg->cacert_bytes, FILE_TYPE_CA_CERT, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading certificate verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
        wolfSSL_CTX_set_verify( (WOLFSSL_CTX *)tls->priv_ctx, WOLFSSL_VERIFY_PEER | WOLFSSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
        aiio_log_d(" Verify Client for Mutual Auth");
    } else {
        aiio_log_d(" Not verifying Client ");
        wolfSSL_CTX_set_verify( (WOLFSSL_CTX *)tls->priv_ctx, WOLFSSL_VERIFY_NONE, NULL);
    }

    if (cfg->servercert_buf != NULL && cfg->serverkey_buf != NULL) {
        if ((aiio_load_wolfssl_verify_buffer(tls,cfg->servercert_buf, cfg->servercert_bytes, FILE_TYPE_SELF_CERT, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading certificate verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
        if ((aiio_load_wolfssl_verify_buffer(tls,cfg->serverkey_buf, cfg->serverkey_bytes, FILE_TYPE_SELF_KEY, &ret)) != AIIO_OK) {
            int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
            aiio_log_e( "Error in loading private key verify buffer, returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            return AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED;
        }
    } else {
        aiio_log_e( "You have to provide both servercert_buf and serverkey_buf for https_server");
        return AIIO_FAIL;
    }

    tls->priv_ssl =(void *)wolfSSL_new( (WOLFSSL_CTX *)tls->priv_ctx);
    if (!tls->priv_ssl) {
        aiio_log_e( "Create wolfSSL failed");
        return AIIO_ERR_WOLFSSL_SSL_SETUP_FAILED;
    }

    wolfSSL_set_fd((WOLFSSL *)tls->priv_ssl, tls->sockfd);
    return AIIO_OK;
}
#endif

int aiio_wolfssl_handshake(aiio_tls_t *tls, const aiio_tls_cfg_t *cfg)
{
    int ret;
    ret = wolfSSL_connect( (WOLFSSL *)tls->priv_ssl);
    if (ret == WOLFSSL_SUCCESS) {
        tls->conn_state = AIIO_TLS_DONE;
        return 1;
    } else {
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        if (err != WOLFSSL_ERROR_WANT_READ && err != WOLFSSL_ERROR_WANT_WRITE) {
            aiio_log_e( "wolfSSL_connect returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_WOLFSSL_SSL_HANDSHAKE_FAILED);
            if (cfg->cacert_buf != NULL || cfg->use_global_ca_store == true) {
                /* This is to check whether handshake failed due to invalid certificate*/
                aiio_wolfssl_verify_certificate(tls);
            }
            tls->conn_state = AIIO_TLS_FAIL;
            return -1;
        }
        /* Irrespective of blocking or non-blocking I/O, we return on getting wolfSSL_want_read
        or wolfSSL_want_write during handshake */
        return 0;
    }
}

ssize_t aiio_wolfssl_read(aiio_tls_t *tls, char *data, size_t datalen)
{
    ssize_t ret = wolfSSL_read( (WOLFSSL *)tls->priv_ssl, (unsigned char *)data, datalen);
    if (ret < 0) {
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        /* peer sent close notify */
        if (err == WOLFSSL_ERROR_ZERO_RETURN) {
            return 0;
        }

        if (ret != WOLFSSL_ERROR_WANT_READ && ret != WOLFSSL_ERROR_WANT_WRITE) {
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, -ret);
            aiio_log_e( "read error :%d:", ret);
            wolfssl_print_error_msg(ret);
        }
        return aiio_tls_convert_wolfssl_err_to_ssize(ret);
    }
    return ret;
}

ssize_t aiio_wolfssl_write(aiio_tls_t *tls, const char *data, size_t datalen)
{
    ssize_t ret = wolfSSL_write( (WOLFSSL *)tls->priv_ssl, (unsigned char *) data, datalen);
    if (ret <= 0) {
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        if (err != WOLFSSL_ERROR_WANT_READ  && err != WOLFSSL_ERROR_WANT_WRITE) {
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, -err);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_WOLFSSL_SSL_WRITE_FAILED);
            aiio_log_e( "write error :%d:", err);
            wolfssl_print_error_msg(err);
        }
        return aiio_tls_convert_wolfssl_err_to_ssize(ret);
    }
    return ret;
}

void aiio_wolfssl_verify_certificate(aiio_tls_t *tls)
{
    int flags;
    if ((flags = wolfSSL_get_verify_result( (WOLFSSL *)tls->priv_ssl)) != X509_V_OK) {
        aiio_log_e( "Failed to verify peer certificate , returned %d", flags);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS, flags);
    } else {
        aiio_log_i( "Certificate verified.");
    }
}

ssize_t aiio_wolfssl_get_bytes_avail(aiio_tls_t *tls)
{
    if (!tls) {
        aiio_log_e( "empty arg passed to aiio_tls_get_bytes_avail()");
        return AIIO_FAIL;
    }
    return wolfSSL_pending( (WOLFSSL *)tls->priv_ssl);
}

void aiio_wolfssl_conn_delete(aiio_tls_t *tls)
{
    if (tls != NULL) {
        aiio_wolfssl_cleanup(tls);
    }
}

void aiio_wolfssl_cleanup(aiio_tls_t *tls)
{
    if (!tls) {
        return;
    }
#ifdef CONFIG_TLS_PSK_VERIFICATION
    xSemaphoreGive(tls_conn_lock);
#endif /* CONFIG_TLS_PSK_VERIFICATION */
    wolfSSL_shutdown( (WOLFSSL *)tls->priv_ssl);
    wolfSSL_free( (WOLFSSL *)tls->priv_ssl);
    tls->priv_ssl = NULL;
    wolfSSL_CTX_free( (WOLFSSL_CTX *)tls->priv_ctx);
    tls->priv_ctx = NULL;
    wolfSSL_Cleanup();
}

#ifdef CONFIG_TLS_SERVER
/**
 * @brief       Create TLS/SSL server session
 */
int aiio_wolfssl_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls)
{
    if (tls == NULL || cfg == NULL) {
        return -1;
    }
    tls->role = AIIO_TLS_SERVER;
    tls->sockfd = sockfd;
    int aiio_ret = aiio_create_wolfssl_handle(NULL, 0, cfg, tls);
    if (aiio_ret != AIIO_OK) {
        aiio_log_e( "create_ssl_handle failed, [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, aiio_ret);
        tls->conn_state = AIIO_TLS_FAIL;
        return -1;
    }
    tls->read = aiio_wolfssl_read;
    tls->write = aiio_wolfssl_write;
    int ret;
    while ((ret = wolfSSL_accept((WOLFSSL *)tls->priv_ssl)) != WOLFSSL_SUCCESS) {
        int err = wolfSSL_get_error((WOLFSSL *)tls->priv_ssl, ret);
        if (err != WOLFSSL_ERROR_WANT_READ && ret != WOLFSSL_ERROR_WANT_WRITE) {
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_WOLFSSL, err);
            aiio_log_e( "wolfSSL_accept returned %d, error code: %d", ret, err);
            wolfssl_print_error_msg(err);
            tls->conn_state = AIIO_TLS_FAIL;
            return -1;
        }
    }
    return 0;
}

/**
 * @brief       Close the server side TLS/SSL connection and free any allocated resources.
 */
void aiio_wolfssl_server_session_delete(aiio_tls_t *tls)
{
    if (tls != NULL) {
        aiio_wolfssl_cleanup(tls);
        aiio_tls_internal_event_tracker_destroy(tls->error_handle);
        free(tls);
    }
}
#endif /* CONFIG_TLS_SERVER */

int aiio_wolfssl_init_global_ca_store(void)
{
    /* This function is just to provide consistancy between function calls of aiio_tls.h and wolfssl */
    return AIIO_OK;
}

int aiio_wolfssl_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes)
{
    if (cacert_pem_buf == NULL) {
        aiio_log_e( "cacert_pem_buf is null");
        return AIIO_ERR_INVALID_ARG;
    }
    if (global_cacert != NULL) {
        aiio_wolfssl_free_global_ca_store();
    }

    global_cacert = (unsigned char *)strndup((const char *)cacert_pem_buf, cacert_pem_bytes);
    if (!global_cacert) {
        return AIIO_FAIL;
    }

    global_cacert_pem_bytes  = cacert_pem_bytes;

    return AIIO_OK;
}

void aiio_wolfssl_free_global_ca_store(void)
{
    if (global_cacert) {
        free(global_cacert);
        global_cacert = NULL;
        global_cacert_pem_bytes = 0;
    }
}

#if defined(CONFIG_TLS_PSK_VERIFICATION)
static int aiio_wolfssl_set_cipher_list(WOLFSSL_CTX *ctx)
{
    const char *defaultCipherList;
    int ret;
#if defined(HAVE_AESGCM) && !defined(NO_DH)
#ifdef WOLFSSL_TLS13
    defaultCipherList = "DHE-PSK-AES128-GCM-SHA256:"
                                    "TLS13-AES128-GCM-SHA256";
#else
    defaultCipherList = "DHE-PSK-AES128-GCM-SHA256";
#endif
#elif defined(HAVE_NULL_CIPHER)
    defaultCipherList = "PSK-NULL-SHA256";
#else
    defaultCipherList = "PSK-AES128-CBC-SHA256";
#endif
    aiio_log_d( "cipher list is %s", defaultCipherList);
    if ((ret = wolfSSL_CTX_set_cipher_list(ctx,defaultCipherList)) != WOLFSSL_SUCCESS) {
        wolfSSL_CTX_free(ctx);
        int err = wolfSSL_get_error( (WOLFSSL *)tls->priv_ssl, ret);
        aiio_log_e( "can't set cipher list, returned %d, error code: %d", ret, err);
        wolfssl_print_error_msg(err);
        return AIIO_FAIL;
    }
    return AIIO_OK;
}

/* initialize the mutex before app_main() when using PSK */
static void __attribute__((constructor))
espt_tls_wolfssl_init_conn_lock (void)
{
    if ((tls_conn_lock = xSemaphoreCreateMutex()) == NULL) {
        AIIO_EARLY_LOGE(TAG, "mutex for tls psk connection could not be created");
    }
}

/* Some callback functions required by PSK */
static inline unsigned int aiio_wolfssl_psk_client_cb(WOLFSSL* ssl, const char* hint,
        char* identity, unsigned int id_max_len, unsigned char* key,
        unsigned int key_max_len)
{
    (void)key_max_len;

    /* see internal.h MAX_PSK_ID_LEN for PSK identity limit */
    memcpy(identity, psk_id_str, id_max_len);
    for(int count = 0; count < psk_key_max_len; count ++) {
         key[count] = psk_key_array[count];
    }
    xSemaphoreGive(tls_conn_lock);
    return psk_key_max_len;
    /* return length of key in octets or 0 or for error */
}
#endif /* CONFIG_TLS_PSK_VERIFICATION */
#endif // CONFIG_TLS_USING_WOLFSSL
