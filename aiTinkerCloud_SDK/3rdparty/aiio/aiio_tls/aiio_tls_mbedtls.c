
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "aiio_chip_spec.h"
#include <sys/types.h>
// #include <sys/socket.h>
#include <netdb.h>

#include <http_parser.h>
#include "aiio_tls_mbedtls.h"
#include "aiio_tls_error_capture_internal.h"
#include "aiio_tls_errors.h"

/* #include <errno.h> */
#include "aiio_log.h"
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "aiio_crt_bundle.h"
#endif

#define aiio_err_to_name(x) "err"

#ifdef CONFIG_TLS_USE_SECURE_ELEMENT

#define ATECC608A_TNG_SLAVE_ADDR        0x6A
#define ATECC608A_TFLEX_SLAVE_ADDR      0x6C
#define ATECC608A_TCUSTOM_SLAVE_ADDR    0xC0
/* cryptoauthlib includes */
#include "mbedtls/atca_mbedtls_wrap.h"
#include "tng_atca.h"
#include "cryptoauthlib.h"
static const atcacert_def_t *cert_def = NULL;
/* Prototypes for functions */
static int aiio_set_atecc608a_pki_context(aiio_tls_t *tls, const void *pki);
#endif /* CONFIG_TLS_USE_SECURE_ELEMENT */

#if defined(CONFIG_TLS_USE_DS_PERIPHERAL)
#include "rsa_sign_alt.h"
static int aiio_mbedtls_init_pk_ctx_for_ds(const void *pki);
#endif /* CONFIG_TLS_USE_DS_PERIPHERAL */

static const char *TAG = "aiio-tls-mbedtls";
static mbedtls_x509_crt *global_cacert = NULL;

/* This function shall return the error message when appropriate log level has been set, otherwise this function shall do nothing */
static void mbedtls_print_error_msg(int error)
{
#if (CONFIG_LOG_DEFAULT_LEVEL_DEBUG || CONFIG_LOG_DEFAULT_LEVEL_VERBOSE)
    static char error_buf[100];
    mbedtls_strerror(error, error_buf, sizeof(error_buf));
    aiio_log_i( "(%04X): %s", error, error_buf);
#endif
}

typedef struct aiio_tls_pki_t {
    mbedtls_x509_crt *public_cert;
    mbedtls_pk_context *pk_key;
    const unsigned char *publiccert_pem_buf;
    unsigned int publiccert_pem_bytes;
    const unsigned char *privkey_pem_buf;
    unsigned int privkey_pem_bytes;
    const unsigned char *privkey_password;
    unsigned int privkey_password_len;
#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
    void *aiio_ds_data;
#endif
} aiio_tls_pki_t;

int aiio_create_mbedtls_handle(const char *hostname, size_t hostlen, const void *cfg, aiio_tls_t *tls)
{
    assert(cfg != NULL);
    assert(tls != NULL);
    int ret;
    int aiio_ret = AIIO_FAIL;
    tls->server_fd.fd = tls->sockfd;
    mbedtls_ssl_init(&tls->ssl);
    mbedtls_ctr_drbg_init(&tls->ctr_drbg);
    mbedtls_ssl_config_init(&tls->conf);
    mbedtls_entropy_init(&tls->entropy);

    if (tls->role == AIIO_TLS_CLIENT) {
        aiio_ret = set_client_config(hostname, hostlen, (aiio_tls_cfg_t *)cfg, tls);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set client configurations, returned [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
            goto exit;
        }
    } else if (tls->role == AIIO_TLS_SERVER) {
#ifdef CONFIG_TLS_SERVER
        aiio_ret = set_server_config((aiio_tls_cfg_server_t *) cfg, tls);
        if (aiio_ret != 0) {
            aiio_log_e( "Failed to set server configurations, returned [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
            goto exit;
        }
#else
            aiio_log_e( "AIIO_TLS_SERVER Not enabled in Kconfig");
            goto exit;
#endif
    }

    if ((ret = mbedtls_ctr_drbg_seed(&tls->ctr_drbg,
                                     mbedtls_entropy_func, &tls->entropy, NULL, 0)) != 0) {
        aiio_log_e( "mbedtls_ctr_drbg_seed returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        aiio_ret = AIIO_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED;
        goto exit;
    }

    mbedtls_ssl_conf_rng(&tls->conf, mbedtls_ctr_drbg_random, &tls->ctr_drbg);

#ifdef CONFIG_MBEDTLS_DEBUG
    mbedtls_enable_debug_log(&tls->conf, CONFIG_MBEDTLS_DEBUG_LEVEL);
#endif

#ifdef CONFIG_MBEDTLS_SSL_PROTO_TLS1_3
    // NOTE: Mbed TLS currently supports only client-side config with TLS 1.3
    if (tls->role != AIIO_TLS_SERVER) {
        mbedtls_ssl_conf_min_version(&tls->conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_4);
        mbedtls_ssl_conf_max_version(&tls->conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_4);
    }
#endif

    if ((ret = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
        aiio_log_e( "mbedtls_ssl_setup returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        aiio_ret = AIIO_ERR_MBEDTLS_SSL_SETUP_FAILED;
        goto exit;
    }
    mbedtls_ssl_set_bio(&tls->ssl, &tls->server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    return AIIO_OK;

exit:
    aiio_mbedtls_cleanup(tls);
    return aiio_ret;

}

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
aiio_tls_client_session_t *aiio_mbedtls_get_client_session(aiio_tls_t *tls)
{
    if (tls == NULL) {
        aiio_log_e( "aiio_tls session context cannot be NULL");
        return NULL;
    }

    aiio_tls_client_session_t *client_session = (aiio_tls_client_session_t*)calloc(1, sizeof(aiio_tls_client_session_t));
    if (client_session == NULL) {
        aiio_log_e( "Failed to allocate memory for client session ctx");
        return NULL;
    }

    int ret = mbedtls_ssl_get_session(&tls->ssl, &(client_session->saved_session));
    if (ret != 0) {
        aiio_log_e( "Error in obtaining the client ssl session");
        mbedtls_print_error_msg(ret);
        free(client_session);
        return NULL;
    }

    return client_session;
}
#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */

int aiio_mbedtls_handshake(aiio_tls_t *tls, const aiio_tls_cfg_t *cfg)
{
    int ret;
#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
    if (cfg->client_session != NULL) {
        aiio_log_d( "Reusing the already saved client session context");
        if ((ret = mbedtls_ssl_set_session(&tls->ssl, &(cfg->client_session->saved_session))) != 0 ) {
            aiio_log_e( " mbedtls_ssl_conf_session returned -0x%04X", -ret);
            return -1;
        }
    }
#endif
    ret = mbedtls_ssl_handshake(&tls->ssl);
    if (ret == 0) {
        tls->conn_state = AIIO_TLS_DONE;

#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
        aiio_ds_release_ds_lock();
#endif
        return 1;
    } else {
        if (ret != AIIO_TLS_ERR_SSL_WANT_READ && ret != AIIO_TLS_ERR_SSL_WANT_WRITE) {
            aiio_log_e( "mbedtls_ssl_handshake returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED);
            if (cfg->cacert_buf != NULL || cfg->use_global_ca_store == true) {
                /* This is to check whether handshake failed due to invalid certificate*/
                aiio_mbedtls_verify_certificate(tls);
            }
            tls->conn_state = AIIO_TLS_FAIL;
            return -1;
        }
        /* Irrespective of blocking or non-blocking I/O, we return on getting AIIO_TLS_ERR_SSL_WANT_READ
        or AIIO_TLS_ERR_SSL_WANT_WRITE during handshake */
        return 0;
    }
}

ssize_t aiio_mbedtls_read(aiio_tls_t *tls, char *data, size_t datalen)
{

    ssize_t ret = mbedtls_ssl_read(&tls->ssl, (unsigned char *)data, datalen);
    if (ret < 0) {
        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
            return 0;
        }
        if (ret != AIIO_TLS_ERR_SSL_WANT_READ  && ret != AIIO_TLS_ERR_SSL_WANT_WRITE) {
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            aiio_log_e( "read error :-0x%04X:", -ret);
            mbedtls_print_error_msg(ret);
        }
    }
    return ret;
}

ssize_t aiio_mbedtls_write(aiio_tls_t *tls, const char *data, size_t datalen)
{
    size_t written = 0;
    size_t write_len = datalen;
    while (written < datalen) {
        if (write_len > MBEDTLS_SSL_OUT_CONTENT_LEN) {
            write_len = MBEDTLS_SSL_OUT_CONTENT_LEN;
        }
        if (datalen > MBEDTLS_SSL_OUT_CONTENT_LEN) {
            aiio_log_d( "Fragmenting data of excessive size :%d, offset: %d, size %d", datalen, written, write_len);
        }
        ssize_t ret = mbedtls_ssl_write(&tls->ssl, (unsigned char*) data + written, write_len);
        if (ret <= 0) {
            if (ret != AIIO_TLS_ERR_SSL_WANT_READ  && ret != AIIO_TLS_ERR_SSL_WANT_WRITE && ret != 0) {
                AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
                AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_MBEDTLS_SSL_WRITE_FAILED);
                aiio_log_e( "write error :-0x%04X:", -ret);
                mbedtls_print_error_msg(ret);
                return ret;
            } else {
                // Exiting the tls-write process as less than desired datalen are writable
                aiio_log_d( "mbedtls_ssl_write() returned -0x%04X, already written %d, exitting...", -ret, written);
                mbedtls_print_error_msg(ret);
                return (written > 0) ? written : ret;
            }
        }
        written += ret;
        write_len = datalen - written;
    }
    return written;
}

void aiio_mbedtls_conn_delete(aiio_tls_t *tls)
{
    if (tls != NULL) {
        aiio_mbedtls_cleanup(tls);
        if (tls->is_tls) {
            mbedtls_net_free(&tls->server_fd);
            tls->sockfd = tls->server_fd.fd;
        }
    }
}

void aiio_mbedtls_verify_certificate(aiio_tls_t *tls)
{
    int flags;

    if ((flags = mbedtls_ssl_get_verify_result(&tls->ssl)) != 0) {
        aiio_log_i( "Failed to verify peer certificate!");
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS, flags);
#if (CONFIG_LOG_DEFAULT_LEVEL > CONFIG_LOG_DEFAULT_LEVEL_DEBUG)
        char buf[100];
        bzero(buf, sizeof(buf));
        mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
        aiio_log_d( "verification info: %s", buf);
#endif
    } else {
        aiio_log_i( "Certificate verified.");
    }
}

ssize_t aiio_mbedtls_get_bytes_avail(aiio_tls_t *tls)
{
    if (!tls) {
        aiio_log_e( "empty arg passed to aiio_tls_get_bytes_avail()");
        return AIIO_FAIL;
    }
    return mbedtls_ssl_get_bytes_avail(&tls->ssl);
}

void aiio_mbedtls_cleanup(aiio_tls_t *tls)
{
    if (!tls) {
        return;
    }
    if (tls->cacert_ptr != global_cacert) {
        mbedtls_x509_crt_free(tls->cacert_ptr);
    }
    tls->cacert_ptr = NULL;
#ifdef CONFIG_TLS_SERVER
    mbedtls_x509_crt_free(&tls->servercert);
    mbedtls_pk_free(&tls->serverkey);
#endif
    mbedtls_x509_crt_free(&tls->cacert);
    mbedtls_x509_crt_free(&tls->clientcert);
    mbedtls_pk_free(&tls->clientkey);
    mbedtls_entropy_free(&tls->entropy);
    mbedtls_ssl_config_free(&tls->conf);
    mbedtls_ctr_drbg_free(&tls->ctr_drbg);
    mbedtls_ssl_free(&tls->ssl);
#ifdef CONFIG_TLS_USE_SECURE_ELEMENT
    atcab_release();
#endif
#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
    aiio_ds_release_ds_lock();
#endif
}

static int set_ca_cert(aiio_tls_t *tls, const unsigned char *cacert, size_t cacert_len)
{
    assert(tls);
    tls->cacert_ptr = &tls->cacert;
    mbedtls_x509_crt_init(tls->cacert_ptr);
    int ret = mbedtls_x509_crt_parse(tls->cacert_ptr, cacert, cacert_len);
    if (ret < 0) {
        aiio_log_e( "mbedtls_x509_crt_parse returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        return AIIO_ERR_MBEDTLS_X509_CRT_PARSE_FAILED;
    }
    if (ret > 0) {
        /* This will happen if the CA chain contains one or more invalid certs, going ahead as the hadshake
         * may still succeed if the other certificates in the CA chain are enough for the authentication */
        aiio_log_w( "mbedtls_x509_crt_parse was partly successful. No. of failed certificates: %d", ret);
    }
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
    return AIIO_OK;
}

static int set_pki_context(aiio_tls_t *tls, const aiio_tls_pki_t *pki)
{
    assert(tls);
    assert(pki);
    int ret;

    if (pki->publiccert_pem_buf != NULL &&
        pki->public_cert != NULL &&
        pki->pk_key != NULL) {

        mbedtls_x509_crt_init(pki->public_cert);
        mbedtls_pk_init(pki->pk_key);

        ret = mbedtls_x509_crt_parse(pki->public_cert, pki->publiccert_pem_buf, pki->publiccert_pem_bytes);
        if (ret < 0) {
            aiio_log_e( "mbedtls_x509_crt_parse returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            return AIIO_ERR_MBEDTLS_X509_CRT_PARSE_FAILED;
        }

#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
        if (pki->aiio_ds_data != NULL) {
            ret = aiio_mbedtls_init_pk_ctx_for_ds(pki);
            if (ret != AIIO_OK) {
                aiio_log_e( "Failed to initialize pk context for aiio_ds");
                return ret;
            }
        } else
#endif
        if (pki->privkey_pem_buf != NULL) {
            ret = mbedtls_pk_parse_key(pki->pk_key, pki->privkey_pem_buf, pki->privkey_pem_bytes,
                                       pki->privkey_password, pki->privkey_password_len);
        } else {
            return AIIO_ERR_INVALID_ARG;
        }

        if (ret < 0) {
            aiio_log_e( "mbedtls_pk_parse_keyfile returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            return AIIO_ERR_MBEDTLS_PK_PARSE_KEY_FAILED;
        }

        ret = mbedtls_ssl_conf_own_cert(&tls->conf, pki->public_cert, pki->pk_key);
        if (ret < 0) {
            aiio_log_e( "mbedtls_ssl_conf_own_cert returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            return AIIO_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED;
        }
    } else {
        return AIIO_ERR_INVALID_ARG;
    }
    return AIIO_OK;
}

static int set_global_ca_store(aiio_tls_t *tls)
{
    assert(tls);
    if (global_cacert == NULL) {
        aiio_log_e( "global_cacert is NULL");
        return AIIO_ERR_INVALID_STATE;
    }
    tls->cacert_ptr = global_cacert;
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
    return AIIO_OK;
}

#ifdef CONFIG_TLS_SERVER
#ifdef CONFIG_TLS_SERVER_SESSION_TICKETS
int aiio_mbedtls_server_session_ticket_write(void *p_ticket, const mbedtls_ssl_session *session, unsigned char *start, const unsigned char *end, size_t *tlen, uint32_t *lifetime)
{
    int ret = mbedtls_ssl_ticket_write(p_ticket, session, start, end, tlen, lifetime);
#ifndef NDEBUG
    if (ret != 0) {
        aiio_log_e( "Writing session ticket resulted in error code -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
    }
#endif
    return ret;
}

int aiio_mbedtls_server_session_ticket_parse(void *p_ticket, mbedtls_ssl_session *session, unsigned char *buf, size_t len)
{
    int ret = mbedtls_ssl_ticket_parse(p_ticket, session, buf, len);
#ifndef NDEBUG
    if (ret != 0) {
        aiio_log_d( "Parsing session ticket resulted in error code -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
    }
#endif
    return ret;
}

int aiio_mbedtls_server_session_ticket_ctx_init(aiio_tls_server_session_ticket_ctx_t *ctx)
{
    if (!ctx) {
        return AIIO_ERR_INVALID_ARG;
    }
    mbedtls_ctr_drbg_init(&ctx->ctr_drbg);
    mbedtls_entropy_init(&ctx->entropy);
    mbedtls_ssl_ticket_init(&ctx->ticket_ctx);
    int ret;
    int aiio_ret;
    if ((ret = mbedtls_ctr_drbg_seed(&ctx->ctr_drbg,
                    mbedtls_entropy_func, &ctx->entropy, NULL, 0)) != 0) {
        aiio_log_e( "mbedtls_ctr_drbg_seed returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        aiio_ret = AIIO_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED;
        goto exit;
    }

    if((ret = mbedtls_ssl_ticket_setup(&ctx->ticket_ctx,
                    mbedtls_ctr_drbg_random, &ctx->ctr_drbg,
                    MBEDTLS_CIPHER_AES_256_GCM,
                    CONFIG_TLS_SERVER_SESSION_TICKET_TIMEOUT)) != 0) {
        aiio_log_e( "mbedtls_ssl_ticket_setup returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        aiio_ret = AIIO_ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED;
        goto exit;
    }
    return AIIO_OK;
exit:
    aiio_mbedtls_server_session_ticket_ctx_free(ctx);
    return aiio_ret;
}

void aiio_mbedtls_server_session_ticket_ctx_free(aiio_tls_server_session_ticket_ctx_t *ctx)
{
    if (ctx) {
        mbedtls_ssl_ticket_free(&ctx->ticket_ctx);
        mbedtls_ctr_drbg_init(&ctx->ctr_drbg);
        mbedtls_entropy_free(&ctx->entropy);
    }
}
#endif

int set_server_config(aiio_tls_cfg_server_t *cfg, aiio_tls_t *tls)
{
    assert(cfg != NULL);
    assert(tls != NULL);
    int ret;
    int aiio_ret;
    if ((ret = mbedtls_ssl_config_defaults(&tls->conf,
                    MBEDTLS_SSL_IS_SERVER,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        aiio_log_e( "mbedtls_ssl_config_defaults returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        return AIIO_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED;
    }

#ifdef CONFIG_MBEDTLS_SSL_ALPN
    if (cfg->alpn_protos) {
        mbedtls_ssl_conf_alpn_protocols(&tls->conf, cfg->alpn_protos);
    }
#endif

    if (cfg->cacert_buf != NULL) {
        aiio_ret = set_ca_cert(tls, cfg->cacert_buf, cfg->cacert_bytes);
        if (aiio_ret != AIIO_OK) {
            return aiio_ret;
        }
    } else {
#ifdef CONFIG_TLS_SERVER_MIN_AUTH_MODE_OPTIONAL
        mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
#else
        mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
#endif
    }

    if (cfg->use_secure_element) {
#ifdef CONFIG_TLS_USE_SECURE_ELEMENT
        aiio_tls_pki_t pki = {
            .public_cert = &tls->servercert,
            .pk_key = &tls->serverkey,
            .publiccert_pem_buf = cfg->servercert_buf,
            .publiccert_pem_bytes = cfg->servercert_bytes,
            .privkey_pem_buf = NULL,
            .privkey_pem_bytes = 0,
            .privkey_password = NULL,
            .privkey_password_len = 0,
        };

        ret = aiio_set_atecc608a_pki_context(tls, (void*) &pki);
        if (ret != AIIO_OK) {
            return ret;
        }
#else
        aiio_log_e( "Please enable secure element support for AIIO-TLS in menuconfig");
        return AIIO_FAIL;
#endif /* CONFIG_TLS_USE_SECURE_ELEMENT */
    } else if (cfg->servercert_buf != NULL && cfg->serverkey_buf != NULL) {
        aiio_tls_pki_t pki = {
            .public_cert = &tls->servercert,
            .pk_key = &tls->serverkey,
            .publiccert_pem_buf = cfg->servercert_buf,
            .publiccert_pem_bytes = cfg->servercert_bytes,
            .privkey_pem_buf = cfg->serverkey_buf,
            .privkey_pem_bytes = cfg->serverkey_bytes,
            .privkey_password = cfg->serverkey_password,
            .privkey_password_len = cfg->serverkey_password_len,
        };
        aiio_ret = set_pki_context(tls, &pki);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set server pki context");
            return aiio_ret;
        }
    } else {
        aiio_log_e( "Missing server certificate and/or key");
        return AIIO_ERR_INVALID_STATE;
    }

#ifdef CONFIG_TLS_SERVER_SESSION_TICKETS
    if (cfg->ticket_ctx) {
        aiio_log_d( "Enabling server-side tls session ticket support");

        mbedtls_ssl_conf_session_tickets_cb( &tls->conf,
                aiio_mbedtls_server_session_ticket_write,
                aiio_mbedtls_server_session_ticket_parse,
                &cfg->ticket_ctx->ticket_ctx );
    }
#endif

    return AIIO_OK;
}
#endif /* ! CONFIG_TLS_SERVER */

int set_client_config(const char *hostname, size_t hostlen, aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    assert(cfg != NULL);
    assert(tls != NULL);
    int ret;
    if (!cfg->skip_common_name) {
        char *use_host = NULL;
        if (cfg->common_name != NULL) {
            use_host = strndup(cfg->common_name, strlen(cfg->common_name));
        } else {
            use_host = strndup(hostname, hostlen);
        }

        if (use_host == NULL) {
            return AIIO_ERR_NO_MEM;
        }
        /* Hostname set here should match CN in server certificate */
        if ((ret = mbedtls_ssl_set_hostname(&tls->ssl, use_host)) != 0) {
            aiio_log_e( "mbedtls_ssl_set_hostname returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            free(use_host);
            return AIIO_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED;
        }
        free(use_host);
    }

    if ((ret = mbedtls_ssl_config_defaults(&tls->conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        aiio_log_e( "mbedtls_ssl_config_defaults returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        return AIIO_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED;
    }

#ifdef CONFIG_MBEDTLS_SSL_RENEGOTIATION
    mbedtls_ssl_conf_renegotiation(&tls->conf, MBEDTLS_SSL_RENEGOTIATION_ENABLED);
#endif

    if (cfg->alpn_protos) {
#ifdef CONFIG_MBEDTLS_SSL_ALPN
        if ((ret = mbedtls_ssl_conf_alpn_protocols(&tls->conf, cfg->alpn_protos)) != 0) {
            aiio_log_e( "mbedtls_ssl_conf_alpn_protocols returned -0x%04X", -ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            mbedtls_print_error_msg(ret);
            return AIIO_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED;
        }
#else
        aiio_log_e( "alpn_protos configured but not enabled in menuconfig: Please enable MBEDTLS_SSL_ALPN option");
        return AIIO_ERR_INVALID_STATE;
#endif
    }

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
    aiio_log_d( "Enabling client-side tls session ticket support");
    mbedtls_ssl_conf_session_tickets(&tls->conf, MBEDTLS_SSL_SESSION_TICKETS_ENABLED);
    mbedtls_ssl_conf_renegotiation(&tls->conf, MBEDTLS_SSL_RENEGOTIATION_ENABLED);

#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */

    if (cfg->crt_bundle_attach != NULL) {
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
        aiio_log_d( "Use certificate bundle");
        mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        cfg->crt_bundle_attach(&tls->conf);
#else //CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
        aiio_log_e( "use_crt_bundle configured but not enabled in menuconfig: Please enable MBEDTLS_CERTIFICATE_BUNDLE option");
        return AIIO_ERR_INVALID_STATE;
#endif
    } else if (cfg->use_global_ca_store == true) {
        int aiio_ret = set_global_ca_store(tls);
        if (aiio_ret != AIIO_OK) {
            return aiio_ret;
        }
    } else if (cfg->cacert_buf != NULL) {
        int aiio_ret = set_ca_cert(tls, cfg->cacert_buf, cfg->cacert_bytes);
        if (aiio_ret != AIIO_OK) {
            return aiio_ret;
        }
        mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
    } else if (cfg->psk_hint_key) {
#if defined(CONFIG_TLS_PSK_VERIFICATION)
        //
        // PSK encryption mode is configured only if no certificate supplied and psk pointer not null
        aiio_log_d( "ssl psk authentication");
        ret = mbedtls_ssl_conf_psk(&tls->conf, cfg->psk_hint_key->key, cfg->psk_hint_key->key_size,
                                   (const unsigned char *)cfg->psk_hint_key->hint, strlen(cfg->psk_hint_key->hint));
        if (ret != 0) {
            aiio_log_e( "mbedtls_ssl_conf_psk returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            return AIIO_ERR_MBEDTLS_SSL_CONF_PSK_FAILED;
        }
#else
        aiio_log_e( "psk_hint_key configured but not enabled in menuconfig: Please enable AIIO_TLS_PSK_VERIFICATION option");
        return AIIO_ERR_INVALID_STATE;
#endif
#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
    } else if (cfg->client_session != NULL) {
        aiio_log_d( "Resuing the saved client session");
#endif
    } else {
#ifdef CONFIG_TLS_SKIP_SERVER_CERT_VERIFY
        mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
#else
        aiio_log_e( "No server verification option set in aiio_tls_cfg_t structure. Check aiio_tls API reference");
        return AIIO_ERR_MBEDTLS_SSL_SETUP_FAILED;
#endif
    }

    if (cfg->use_secure_element) {
#ifdef CONFIG_TLS_USE_SECURE_ELEMENT
        aiio_tls_pki_t pki = {
            .public_cert = &tls->clientcert,
            .pk_key = &tls->clientkey,
            .publiccert_pem_buf = cfg->clientcert_buf,
            .publiccert_pem_bytes = cfg->clientcert_bytes,
            .privkey_pem_buf = NULL,
            .privkey_pem_bytes = 0,
            .privkey_password = NULL,
            .privkey_password_len = 0,
        };
        ret = aiio_set_atecc608a_pki_context(tls, (void*) &pki);
        if (ret != AIIO_OK) {
            return ret;
        }
#else
        aiio_log_e( "Please enable secure element support for AIIO-TLS in menuconfig");
        return AIIO_FAIL;
#endif /* CONFIG_TLS_USE_SECURE_ELEMENT */
    } else if (cfg->ds_data != NULL) {
#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
        if (cfg->clientcert_pem_buf == NULL) {
            aiio_log_e( "Client certificate is also required with the DS parameters");
            return AIIO_ERR_INVALID_STATE;
        }
        aiio_ds_set_session_timeout(cfg->timeout_ms);
        /* set private key pointer to NULL since the DS peripheral with its own configuration data is used */
        aiio_tls_pki_t pki = {
            .public_cert = &tls->clientcert,
            .pk_key = &tls->clientkey,
            .publiccert_pem_buf = cfg->clientcert_buf,
            .publiccert_pem_bytes = cfg->clientcert_bytes,
            .privkey_pem_buf = NULL,
            .privkey_pem_bytes = 0,
            .privkey_password = NULL,
            .privkey_password_len = 0,
            .aiio_ds_data = cfg->ds_data,
        };

        int aiio_ret = set_pki_context(tls, &pki);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set client pki context for the DS peripheral, returned [0x%04X] (%s)", ret, aiio_err_to_name(aiio_ret));
            return aiio_ret;
        }
#else
        aiio_log_e( "Please enable the DS peripheral support for the AIIO-TLS in menuconfig. (only supported for the 32bit chip)");
        return AIIO_FAIL;
#endif
    } else if (cfg->clientcert_pem_buf != NULL && cfg->clientkey_pem_buf != NULL) {
        aiio_tls_pki_t pki = {
            .public_cert = &tls->clientcert,
            .pk_key = &tls->clientkey,
            .publiccert_pem_buf = cfg->clientcert_buf,
            .publiccert_pem_bytes = cfg->clientcert_bytes,
            .privkey_pem_buf = cfg->clientkey_buf,
            .privkey_pem_bytes = cfg->clientkey_bytes,
            .privkey_password = cfg->clientkey_password,
            .privkey_password_len = cfg->clientkey_password_len,
        };
        int aiio_ret = set_pki_context(tls, &pki);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "Failed to set client pki context");
            return aiio_ret;
        }
    } else if (cfg->clientcert_buf != NULL || cfg->clientkey_buf != NULL) {
        aiio_log_e( "You have to provide both clientcert_buf and clientkey_buf for mutual authentication");
        return AIIO_ERR_INVALID_STATE;
    }
    return AIIO_OK;
}

#ifdef CONFIG_TLS_SERVER
/**
 * @brief      Create TLS/SSL server session
 */
int aiio_mbedtls_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls)
{
    if (tls == NULL || cfg == NULL) {
        return -1;
    }
    tls->role = AIIO_TLS_SERVER;
    tls->sockfd = sockfd;
    int aiio_ret = aiio_create_mbedtls_handle(NULL, 0, cfg, tls);
    if (aiio_ret != AIIO_OK) {
        aiio_log_e( "create_ssl_handle failed, returned [0x%04X] (%s)", aiio_ret, aiio_err_to_name(aiio_ret));
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, aiio_ret);
        tls->conn_state = AIIO_TLS_FAIL;
        return -1;
    }
    tls->read = aiio_mbedtls_read;
    tls->write = aiio_mbedtls_write;
    int ret;
    while ((ret = mbedtls_ssl_handshake(&tls->ssl)) != 0) {
        if (ret != AIIO_TLS_ERR_SSL_WANT_READ && ret != AIIO_TLS_ERR_SSL_WANT_WRITE) {
            aiio_log_e( "mbedtls_ssl_handshake returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            tls->conn_state = AIIO_TLS_FAIL;
            return ret;
        }
    }
    return 0;
}
/**
 * @brief      Close the server side TLS/SSL connection and free any allocated resources.
 */
void aiio_mbedtls_server_session_delete(aiio_tls_t *tls)
{
    if (tls != NULL) {
        aiio_mbedtls_cleanup(tls);
        aiio_tls_internal_event_tracker_destroy(tls->error_handle);
        free(tls);
    }
};
#endif /* ! CONFIG_TLS_SERVER */

int aiio_mbedtls_init_global_ca_store(void)
{
    if (global_cacert == NULL) {
        global_cacert = (mbedtls_x509_crt *)calloc(1, sizeof(mbedtls_x509_crt));
        if (global_cacert == NULL) {
            aiio_log_e( "global_cacert not allocated");
            return AIIO_ERR_NO_MEM;
        }
        mbedtls_x509_crt_init(global_cacert);
    }
    return AIIO_OK;
}

int aiio_mbedtls_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes)
{
#ifdef CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT
    aiio_log_e( "Please disable dynamic freeing of ca cert in mbedtls (CONFIG_MBEDTLS_DYNAMIC_FREE_CA_CERT)\n in order to use the global ca_store");
    return AIIO_FAIL;
#endif
    if (cacert_pem_buf == NULL) {
        aiio_log_e( "cacert_pem_buf is null");
        return AIIO_ERR_INVALID_ARG;
    }
    int ret;
    if (global_cacert == NULL) {
        ret = aiio_mbedtls_init_global_ca_store();
        if (ret != AIIO_OK) {
            return ret;
        }
    }
    ret = mbedtls_x509_crt_parse(global_cacert, cacert_pem_buf, cacert_pem_bytes);
    if (ret < 0) {
        aiio_log_e( "mbedtls_x509_crt_parse returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        mbedtls_x509_crt_free(global_cacert);
        free(global_cacert);
        global_cacert = NULL;
        return AIIO_FAIL;
    } else if (ret > 0) {
        aiio_log_e( "mbedtls_x509_crt_parse was partly successful. No. of failed certificates: %d", ret);
        return AIIO_ERR_MBEDTLS_CERT_PARTLY_OK;
    }
    return AIIO_OK;
}

mbedtls_x509_crt *aiio_mbedtls_get_global_ca_store(void)
{
    return global_cacert;
}

void aiio_mbedtls_free_global_ca_store(void)
{
    if (global_cacert) {
        mbedtls_x509_crt_free(global_cacert);
        free(global_cacert);
        global_cacert = NULL;
    }
}

#ifdef CONFIG_TLS_USE_SECURE_ELEMENT
static int aiio_init_atecc608a(uint8_t i2c_addr)
{
    cfg_ateccx08a_i2c_default.atcai2c.address = i2c_addr;
    int ret = atcab_init(&cfg_ateccx08a_i2c_default);
    if(ret != 0) {
        aiio_log_e( "Failed to initialize atca device, returned -0x%04X", -ret);
        return AIIO_FAIL;
    }
    return AIIO_OK;
}

static int aiio_set_atecc608a_pki_context(aiio_tls_t *tls, const void *pki)
{
    int ret = 0;
    int aiio_ret = AIIO_FAIL;
    aiio_log_i( "Initialize the ATECC interface...");
    (void)aiio_ret;
    (void)cert_def;
#if defined(CONFIG_ATECC608A_TNG) || defined(CONFIG_ATECC608A_TFLEX)
#ifdef CONFIG_ATECC608A_TNG
    aiio_ret = aiio_init_atecc608a(ATECC608A_TNG_SLAVE_ADDR);
    if (ret != AIIO_OK) {
        return AIIO_ERR_TLS_SE_FAILED;
    }
#elif CONFIG_ATECC608A_TFLEX /* CONFIG_ATECC608A_TNG */
    aiio_ret = aiio_init_atecc608a(ATECC608A_TFLEX_SLAVE_ADDR);
    if (ret != AIIO_OK) {
        return AIIO_ERR_TLS_SE_FAILED;
    }
#endif /* CONFIG_ATECC608A_TFLEX */
    mbedtls_x509_crt_init(&tls->clientcert);
    ret = tng_get_device_cert_def(&cert_def);
    if (ret != 0) {
        aiio_log_e( "Failed to get device cert def");
        return AIIO_ERR_TLS_SE_FAILED;
    }

    /* Extract the device certificate and convert to mbedtls cert */
    ret = atca_mbedtls_cert_add(&tls->clientcert, cert_def);
    if (ret != 0) {
        aiio_log_e( "Failed to parse cert from device, return 0x%04X", ret);
        mbedtls_print_error_msg(ret);
        return AIIO_ERR_TLS_SE_FAILED;
    }
#elif CONFIG_ATECC608A_TCUSTOM
    aiio_ret = aiio_init_atecc608a(ATECC608A_TCUSTOM_SLAVE_ADDR);
    if (ret != AIIO_OK) {
        return AIIO_ERR_TLS_SE_FAILED;
    }
    mbedtls_x509_crt_init(&tls->clientcert);

    if(cfg->clientcert_buf != NULL) {
        ret = mbedtls_x509_crt_parse(&tls->clientcert, (const unsigned char*)((aiio_tls_pki_t *)pki->publiccert_pem_buf), (aiio_tls_pki_t *)pki->publiccert_pem_bytes);
        if (ret < 0) {
            aiio_log_e( "mbedtls_x509_crt_parse returned -0x%04X", -ret);
            mbedtls_print_error_msg(ret);
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
            return AIIO_ERR_MBEDTLS_X509_CRT_PARSE_FAILED;
        }
    } else {
        aiio_log_e( "Device certificate must be provided for TrustCustom Certs");
        return AIIO_FAIL;
    }
#endif /* CONFIG_ATECC608A_TCUSTOM */
    ret = atca_mbedtls_pk_init(&tls->clientkey, 0);
    if (ret != 0) {
        aiio_log_e( "Failed to parse key from device");
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        mbedtls_print_error_msg(ret);
        return AIIO_ERR_TLS_SE_FAILED;
    }

    ret = mbedtls_ssl_conf_own_cert(&tls->conf, &tls->clientcert, &tls->clientkey);
    if (ret != 0) {
        aiio_log_e( "Failed to configure client cert, returned -0x%04X", ret);
        mbedtls_print_error_msg(ret);
        AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_MBEDTLS, -ret);
        return AIIO_ERR_TLS_SE_FAILED;
    }

    return AIIO_OK;
}
#endif /* CONFIG_TLS_USE_SECURE_ELEMENT */

#ifdef CONFIG_TLS_USE_DS_PERIPHERAL
static int aiio_mbedtls_init_pk_ctx_for_ds(const void *pki)
{
    int ret = -1;
    /* initialize the mbedtls pk context with rsa context */
    mbedtls_rsa_context rsakey;
    mbedtls_rsa_init(&rsakey);
    if ((ret = mbedtls_pk_setup_rsa_alt(((const aiio_tls_pki_t*)pki)->pk_key, &rsakey, NULL, aiio_ds_rsa_sign,
                                        aiio_ds_get_keylen )) != 0) {
        aiio_log_e( "Error in mbedtls_pk_setup_rsa_alt, returned -0x%04X", -ret);
        mbedtls_print_error_msg(ret);
        return AIIO_FAIL;
    }
    ret = aiio_ds_init_data_ctx(((const aiio_tls_pki_t*)pki)->aiio_ds_data);
    if (ret != AIIO_OK) {
        aiio_log_e( "Failed to initialize DS parameters from nvs");
        return AIIO_FAIL;
    }
    aiio_log_d( "DS peripheral params initialized.");
    return AIIO_OK;
}
#endif /* CONFIG_TLS_USE_DS_PERIPHERAL */
