
#ifndef _TLS_H_
#define _TLS_H_

#include "aiio_tls_config.h"
#include "aiio_chip_spec.h"
#include <stdbool.h>
// #include <sys/socket.h>
/* #include <fcntl.h> */
#include "aiio_err.h"
#include "aiio_tls_errors.h"
#ifdef CONFIG_TLS_USING_MBEDTLS
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
/* #include "mbedtls/aiio_debug.h" */
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#ifdef CONFIG_TLS_SERVER_SESSION_TICKETS
#include "mbedtls/ssl_ticket.h"
#endif
#elif CONFIG_TLS_USING_WOLFSSL
#include "wolfssl/wolfcrypt/settings.h"
#include "wolfssl/ssl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief AIIO-TLS Connection State
 */
typedef enum aiio_tls_conn_state {
    AIIO_TLS_INIT = 0,
    AIIO_TLS_CONNECTING,
    AIIO_TLS_HANDSHAKE,
    AIIO_TLS_FAIL,
    AIIO_TLS_DONE,
} aiio_tls_conn_state_t;

typedef enum aiio_tls_role {
    AIIO_TLS_CLIENT = 0,
    AIIO_TLS_SERVER,
} aiio_tls_role_t;

/**
 *  @brief AIIO-TLS preshared key and hint structure
 */
typedef struct psk_key_hint {
    const uint8_t* key;                     /*!< key in PSK authentication mode in binary format */
    const size_t   key_size;                /*!< length of the key */
    const char* hint;                       /*!< hint in PSK authentication mode in string format */
} psk_hint_key_t;

/**
 * @brief aiio-tls client session ticket ctx
 */
#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
typedef struct aiio_tls_client_session {
    mbedtls_ssl_session saved_session;
} aiio_tls_client_session_t;
#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */

/**
*  @brief Keep alive parameters structure
*/
typedef struct tls_keep_alive_cfg {
    bool keep_alive_enable;               /*!< Enable keep-alive timeout */
    int keep_alive_idle;                  /*!< Keep-alive idle time (second) */
    int keep_alive_interval;              /*!< Keep-alive interval time (second) */
    int keep_alive_count;                 /*!< Keep-alive packet retry send count */
} tls_keep_alive_cfg_t;

/**
 * @brief      AIIO-TLS configuration parameters
 *
 * @note       Note about format of certificates:
 *             - This structure includes certificates of a Certificate Authority, of client or server as well
 *             as private keys, which may be of PEM or DER format. In case of PEM format, the buffer must be
 *             NULL terminated (with NULL character included in certificate size).
 *             - Certificate Authority's certificate may be a chain of certificates in case of PEM format,
 *             but could be only one certificate in case of DER format
 *             - Variables names of certificates and private key buffers and sizes are defined as unions providing
 *             backward compatibility for legacy *_pem_buf and *_pem_bytes names which suggested only PEM format
 *             was supported. It is encouraged to use generic names such as cacert_buf and cacert_bytes.
 */
typedef struct aiio_tls_cfg {
    const char **alpn_protos;               /*!< Application protocols required for HTTP2.
                                                 If HTTP2/ALPN support is required, a list
                                                 of protocols that should be negotiated.
                                                 The format is length followed by protocol
                                                 name.
                                                 For the most common cases the following is ok:
                                                 const char **alpn_protos = { "h2", NULL };
                                                 - where 'h2' is the protocol name */

    union {
    const unsigned char *cacert_buf;        /*!< Certificate Authority's certificate in a buffer.
                                                 Format may be PEM or DER, depending on mbedtls-support
                                                 This buffer should be NULL terminated in case of PEM */
    const unsigned char *cacert_pem_buf;    /*!< CA certificate buffer legacy name */
    };

    union {
    unsigned int cacert_bytes;              /*!< Size of Certificate Authority certificate
                                                 pointed to by cacert_buf
                                                 (including NULL-terminator in case of PEM format) */
    unsigned int cacert_pem_bytes;          /*!< Size of Certificate Authority certificate legacy name */
    };

    union {
    const unsigned char *clientcert_buf;    /*!< Client certificate in a buffer
                                                 Format may be PEM or DER, depending on mbedtls-support
                                                 This buffer should be NULL terminated in case of PEM */
    const unsigned char *clientcert_pem_buf;     /*!< Client certificate legacy name */
    };

    union {
    unsigned int clientcert_bytes;          /*!< Size of client certificate pointed to by
                                                 clientcert_pem_buf
                                                 (including NULL-terminator in case of PEM format) */
    unsigned int clientcert_pem_bytes;      /*!< Size of client certificate legacy name */
    };

    union {
    const unsigned char *clientkey_buf;     /*!< Client key in a buffer
                                                 Format may be PEM or DER, depending on mbedtls-support
                                                 This buffer should be NULL terminated in case of PEM */
    const unsigned char *clientkey_pem_buf; /*!< Client key legacy name */
    };

    union {
    unsigned int clientkey_bytes;           /*!< Size of client key pointed to by
                                                 clientkey_pem_buf
                                                 (including NULL-terminator in case of PEM format) */
    unsigned int clientkey_pem_bytes;       /*!< Size of client key legacy name */
    };

    const unsigned char *clientkey_password;/*!< Client key decryption password string */

    unsigned int clientkey_password_len;    /*!< String length of the password pointed to by
                                                 clientkey_password */

    bool non_block;                         /*!< Configure non-blocking mode. If set to true the
                                                 underneath socket will be configured in non
                                                 blocking mode after tls session is established */

    bool use_secure_element;                /*!< Enable this option to use secure element ) */

    int timeout_ms;                         /*!< Network timeout in milliseconds */

    bool use_global_ca_store;               /*!< Use a global ca_store for all the connections in which
                                                 this bool is set. */

    const char *common_name;                /*!< If non-NULL, server certificate CN must match this name.
                                                 If NULL, server certificate CN must match hostname. */

    bool skip_common_name;                  /*!< Skip any validation of server certificate CN field */

    tls_keep_alive_cfg_t *keep_alive_cfg;   /*!< Enable TCP keep-alive timeout for SSL connection */

    const psk_hint_key_t* psk_hint_key;     /*!< Pointer to PSK hint and key. if not NULL (and certificates are NULL)
                                                 then PSK authentication is enabled with configured setup.
                                                 Important note: the pointer must be valid for connection */

    int (*crt_bundle_attach)(void *conf);
                                            /*!< Function pointer to aiio_crt_bundle_attach. Enables the use of certification
                                                 bundle for server verification, must be enabled in menuconfig */

    void *ds_data;                          /*!< Pointer for digital signature peripheral context */
    bool is_plain_tcp;                      /*!< Use non-TLS connection: When set to true, the aiio-tls uses
                                                 plain TCP transport rather then TLS/SSL connection.
                                                 Note, that it is possible to connect using a plain tcp transport
                                                 directly with aiio_tls_plain_tcp_connect() API */

    struct ifreq *if_name;                  /*!< The name of interface for data to go through. Use the default interface without setting */

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
    aiio_tls_client_session_t *client_session; /*! Pointer for the client session ticket context. */
#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */
} aiio_tls_cfg_t;

#ifdef CONFIG_TLS_SERVER
#if defined(CONFIG_TLS_SERVER_SESSION_TICKETS)
/**
 * @brief Data structures necessary to support TLS session tickets according to RFC5077
 */
typedef struct aiio_tls_server_session_ticket_ctx {
    mbedtls_entropy_context entropy;                                            /*!< mbedTLS entropy context structure */

    mbedtls_ctr_drbg_context ctr_drbg;                                          /*!< mbedTLS ctr drbg context structure.
                                                                                     CTR_DRBG is deterministic random
                                                                                     bit generation based on AES-256 */
    mbedtls_ssl_ticket_context ticket_ctx;                                     /*!< Session ticket generation context */
} aiio_tls_server_session_ticket_ctx_t;
#endif

typedef struct aiio_tls_cfg_server {
    const char **alpn_protos;                   /*!< Application protocols required for HTTP2.
                                                     If HTTP2/ALPN support is required, a list
                                                     of protocols that should be negotiated.
                                                     The format is length followed by protocol
                                                     name.
                                                     For the most common cases the following is ok:
                                                     const char **alpn_protos = { "h2", NULL };
                                                     - where 'h2' is the protocol name */

    union {
    const unsigned char *cacert_buf;        /*!< Client CA certificate in a buffer.
                                                     This buffer should be NULL terminated */
    const unsigned char *cacert_pem_buf;    /*!< Client CA certificate legacy name */
    };

    union {
    unsigned int cacert_bytes;              /*!< Size of client CA certificate
                                                     pointed to by cacert_pem_buf */
    unsigned int cacert_pem_bytes;          /*!< Size of client CA certificate legacy name */
    };

    union {
    const unsigned char *servercert_buf;        /*!< Server certificate in a buffer
                                                     This buffer should be NULL terminated */
    const unsigned char *servercert_pem_buf;    /*!< Server certificate legacy name */
    };

    union {
    unsigned int servercert_bytes;             /*!< Size of server certificate pointed to by
                                                     servercert_pem_buf */
    unsigned int servercert_pem_bytes;          /*!< Size of server certificate legacy name */
    };

    union {
    const unsigned char *serverkey_buf;         /*!< Server key in a buffer
                                                     This buffer should be NULL terminated */
    const unsigned char *serverkey_pem_buf;     /*!< Server key legacy name */
    };

    union {
    unsigned int serverkey_bytes;               /*!< Size of server key pointed to by
                                                     serverkey_pem_buf */
    unsigned int serverkey_pem_bytes;           /*!< Size of server key legacy name */
    };

    const unsigned char *serverkey_password;    /*!< Server key decryption password string */

    unsigned int serverkey_password_len;        /*!< String length of the password pointed to by
                                                     serverkey_password */

    bool use_secure_element;                    /*!< Enable this option to use secure element ) */


#if defined(CONFIG_TLS_SERVER_SESSION_TICKETS)
    aiio_tls_server_session_ticket_ctx_t * ticket_ctx; /*!< Session ticket generation context.
                                                    You have to call aiio_tls_cfg_server_session_tickets_init
                                                    to use it.
                                                    Call aiio_tls_cfg_server_session_tickets_free
                                                    to free the data associated with this context. */
#endif
} aiio_tls_cfg_server_t;

/**
 * @brief Initialize the server side TLS session ticket context
 *
 * This function initializes the server side tls session ticket context
 * which holds all necessary data structures to enable tls session tickets
 * according to RFC5077.
 * Use aiio_tls_cfg_server_session_tickets_free to free the data.
 *
 * @param[in]  cfg server configuration as aiio_tls_cfg_server_t
 * @return
 *             AIIO_OK if setup succeeded
 *             AIIO_ERR_INVALID_ARG if context is already initialized
 *             AIIO_ERR_NO_MEM if memory allocation failed
 *             AIIO_ERR_NOT_SUPPORTED if session tickets are not available due to build configuration
 *             AIIO_FAIL if setup failed
 */
int aiio_tls_cfg_server_session_tickets_init(aiio_tls_cfg_server_t *cfg);

/**
 * @brief Free the server side TLS session ticket context
 *
 * @param cfg server configuration as aiio_tls_cfg_server_t
 */
void aiio_tls_cfg_server_session_tickets_free(aiio_tls_cfg_server_t *cfg);
#endif /* ! CONFIG_TLS_SERVER */

/**
 * @brief      AIIO-TLS Connection Handle
 */
typedef struct aiio_tls {
#ifdef CONFIG_TLS_USING_MBEDTLS
    mbedtls_ssl_context ssl;                                                    /*!< TLS/SSL context */

    mbedtls_entropy_context entropy;                                            /*!< mbedTLS entropy context structure */

    mbedtls_ctr_drbg_context ctr_drbg;                                          /*!< mbedTLS ctr drbg context structure.
                                                                                     CTR_DRBG is deterministic random
                                                                                     bit generation based on AES-256 */

    mbedtls_ssl_config conf;                                                    /*!< TLS/SSL configuration to be shared
                                                                                     between mbedtls_ssl_context
                                                                                     structures */

    mbedtls_net_context server_fd;                                              /*!< mbedTLS wrapper type for sockets */

    mbedtls_x509_crt cacert;                                                    /*!< Container for the X.509 CA certificate */

    mbedtls_x509_crt *cacert_ptr;                                               /*!< Pointer to the cacert being used. */

    mbedtls_x509_crt clientcert;                                                /*!< Container for the X.509 client certificate */

    mbedtls_pk_context clientkey;                                               /*!< Container for the private key of the client
                                                                                     certificate */
#ifdef CONFIG_TLS_SERVER
    mbedtls_x509_crt servercert;                                                /*!< Container for the X.509 server certificate */

    mbedtls_pk_context serverkey;                                               /*!< Container for the private key of the server
                                                                                   certificate */
#endif
#elif CONFIG_TLS_USING_WOLFSSL
    void *priv_ctx;
    void *priv_ssl;
#endif
    int sockfd;                                                                 /*!< Underlying socket file descriptor. */

    ssize_t (*read_)(struct aiio_tls  *tls, char *data, size_t datalen);          /*!< Callback function for reading data from TLS/SSL
                                                                                     connection. */

    ssize_t (*write_)(struct aiio_tls *tls, const char *data, size_t datalen);    /*!< Callback function for writing data to TLS/SSL
                                                                                     connection. */

    aiio_tls_conn_state_t  conn_state;                                           /*!< AIIO-TLS Connection state */

    fd_set rset;                                                                /*!< read file descriptors */

    fd_set wset;                                                                /*!< write file descriptors */

    bool is_tls;                                                                /*!< indicates connection type (TLS or NON-TLS) */

    aiio_tls_role_t role;                                                        /*!< aiio-tls role
                                                                                     - AIIO_TLS_CLIENT
                                                                                     - AIIO_TLS_SERVER */

    aiio_tls_error_handle_t error_handle;                                        /*!< handle to error descriptor */

} aiio_tls_t;

/**
 * @brief      Create TLS connection
 *
 * This function allocates and initializes aiio-tls structure handle.
 *
 * @return      tls     Pointer to aiio-tls as aiio-tls handle if successfully initialized,
 *                      NULL if allocation error
 */
aiio_tls_t *aiio_tls_init(void);

/**
 * @brief      Create a new blocking TLS/SSL connection with a given "HTTP" url
 *
 * Note: This API is present for backward compatibility reasons. Alternative function
 * with the same functionality is `aiio_tls_conn_http_new_sync` (and its asynchronous version
 * `aiio_tls_conn_http_new_async`)
 *
 * @param[in]  url  url of host.
 * @param[in]  cfg  TLS configuration as aiio_tls_cfg_t. If you wish to open
 *                  non-TLS connection, keep this NULL. For TLS connection,
 *                  a pass pointer to 'aiio_tls_cfg_t'. At a minimum, this
 *                  structure should be zero-initialized.
 * @return pointer to aiio_tls_t, or NULL if connection couldn't be opened.
 */
aiio_tls_t *aiio_tls_conn_http_new(const char *url, const aiio_tls_cfg_t *cfg) __attribute__((deprecated("Please use aiio_tls_conn_http_new_sync (or its asynchronous version aiio_tls_conn_http_new_async) instead")));

/**
 * @brief      Create a new blocking TLS/SSL connection
 *
 * This function establishes a TLS/SSL connection with the specified host in blocking manner.
 *
 * @param[in]  hostname  Hostname of the host.
 * @param[in]  hostlen   Length of hostname.
 * @param[in]  port      Port number of the host.
 * @param[in]  cfg       TLS configuration as aiio_tls_cfg_t. If you wish to open
 *                       non-TLS connection, keep this NULL. For TLS connection,
 *                       a pass pointer to aiio_tls_cfg_t. At a minimum, this
 *                       structure should be zero-initialized.
 * @param[in]  tls       Pointer to aiio-tls as aiio-tls handle.
 *
 * @return
 *             - -1      If connection establishment fails.
 *             -  1      If connection establishment is successful.
 *             -  0      If connection state is in progress.
 */
int aiio_tls_conn_new_sync(const char *hostname, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

/**
 * @brief      Create a new blocking TLS/SSL connection with a given "HTTP" url
 *
 * The behaviour is same as aiio_tls_conn_new_sync() API. However this API accepts host's url.
 *
 * @param[in]  url       url of host.
 * @param[in]  cfg       TLS configuration as aiio_tls_cfg_t. If you wish to open
 *                       non-TLS connection, keep this NULL. For TLS connection,
 *                       a pass pointer to 'aiio_tls_cfg_t'. At a minimum, this
 *                       structure should be zero-initialized.
 * @param[in]  tls       Pointer to aiio-tls as aiio-tls handle.
 *
 * @return
 *             - -1      If connection establishment fails.
 *             -  1      If connection establishment is successful.
 *             -  0      If connection state is in progress.
 */
int aiio_tls_conn_http_new_sync(const char *url, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

/**
 * @brief      Create a new non-blocking TLS/SSL connection
 *
 * This function initiates a non-blocking TLS/SSL connection with the specified host, but due to
 * its non-blocking nature, it doesn't wait for the connection to get established.
 *
 * @param[in]  hostname  Hostname of the host.
 * @param[in]  hostlen   Length of hostname.
 * @param[in]  port      Port number of the host.
 * @param[in]  cfg       TLS configuration as aiio_tls_cfg_t. `non_block` member of
 *                       this structure should be set to be true.
 * @param[in]  tls       pointer to aiio-tls as aiio-tls handle.
 *
 * @return
 *             - -1      If connection establishment fails.
 *             -  0      If connection establishment is in progress.
 *             -  1      If connection establishment is successful.
 */
int aiio_tls_conn_new_async(const char *hostname, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

/**
 * @brief      Create a new non-blocking TLS/SSL connection with a given "HTTP" url
 *
 * The behaviour is same as aiio_tls_conn_new_async() API. However this API accepts host's url.
 *
 * @param[in]  url     url of host.
 * @param[in]  cfg     TLS configuration as aiio_tls_cfg_t.
 * @param[in]  tls     pointer to aiio-tls as aiio-tls handle.
 *
 * @return
 *             - -1     If connection establishment fails.
 *             -  0     If connection establishment is in progress.
 *             -  1     If connection establishment is successful.
 */
int aiio_tls_conn_http_new_async(const char *url, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

/**
 * @brief      Write from buffer 'data' into specified tls connection.
 *
 * @param[in]  tls      pointer to aiio-tls as aiio-tls handle.
 * @param[in]  data     Buffer from which data will be written.
 * @param[in]  datalen  Length of data buffer.
 *
 * @return
 *             - >=0  if write operation was successful, the return value is the number
 *                   of bytes actually written to the TLS/SSL connection.
 *             - <0  if write operation was not successful, because either an
 *                   error occured or an action must be taken by the calling process.
 *             - AIIO_TLS_ERR_SSL_WANT_READ/
 *               AIIO_TLS_ERR_SSL_WANT_WRITE.
 *                  if the handshake is incomplete and waiting for data to be available for reading.
 *                  In this case this functions needs to be called again when the underlying transport is ready for operation.
 */
static inline ssize_t aiio_tls_conn_write(aiio_tls_t *tls, const void *data, size_t datalen)
{
    return tls->write_(tls, (char *)data, datalen);
}

/**
 * @brief      Read from specified tls connection into the buffer 'data'.
 *
 * @param[in]  tls      pointer to aiio-tls as aiio-tls handle.
 * @param[in]  data     Buffer to hold read data.
 * @param[in]  datalen  Length of data buffer.
 *
 * @return
 *             - >0  if read operation was successful, the return value is the number
 *                   of bytes actually read from the TLS/SSL connection.
 *             -  0  if read operation was not successful. The underlying
 *                   connection was closed.
 *             - <0  if read operation was not successful, because either an
 *                   error occured or an action must be taken by the calling process.
 */
static inline ssize_t aiio_tls_conn_read(aiio_tls_t *tls, void  *data, size_t datalen)
{
    return tls->read_(tls, (char *)data, datalen);
}

/**
 * @brief      Close the TLS/SSL connection and free any allocated resources.
 *
 * This function should be called to close each tls connection opened with
 * aiio_tls_conn_new_sync() (or aiio_tls_conn_http_new_sync()) and
 * aiio_tls_conn_new_async() (or aiio_tls_conn_http_new_async()) APIs.
 *
 * @param[in]  tls  pointer to aiio-tls as aiio-tls handle.
 *
 * @return    - 0 on success
 *            - -1 if socket error or an invalid argument
 */
int aiio_tls_conn_destroy(aiio_tls_t *tls);

/**
 * @brief      Return the number of application data bytes remaining to be
 *             read from the current record
 *
 * This API is a wrapper over mbedtls's mbedtls_ssl_get_bytes_avail() API.
 *
 * @param[in]  tls  pointer to aiio-tls as aiio-tls handle.
 *
 * @return
 *            - -1  in case of invalid arg
 *            - bytes available in the application data
 *              record read buffer
 */
ssize_t aiio_tls_get_bytes_avail(aiio_tls_t *tls);

/**
 * @brief       Returns the connection socket file descriptor from aiio_tls session
 *
 * @param[in]   tls          handle to aiio_tls context
 *
 * @param[out]  sockfd       int pointer to sockfd value.
 *
 * @return     - AIIO_OK on success and value of sockfd will be updated with socket file descriptor for connection
 *             - AIIO_ERR_INVALID_ARG if (tls == NULL || sockfd == NULL)
 */
int aiio_tls_get_conn_sockfd(aiio_tls_t *tls, int *sockfd);

/**
 * @brief      Create a global CA store, initially empty.
 *
 * This function should be called if the application wants to use the same CA store for multiple connections.
 * This function initialises the global CA store which can be then set by calling aiio_tls_set_global_ca_store().
 * To be effective, this function must be called before any call to aiio_tls_set_global_ca_store().
 *
 * @return
 *             - AIIO_OK             if creating global CA store was successful.
 *             - AIIO_ERR_NO_MEM     if an error occured when allocating the mbedTLS resources.
 */
int aiio_tls_init_global_ca_store(void);

/**
 * @brief      Set the global CA store with the buffer provided in pem format.
 *
 * This function should be called if the application wants to set the global CA store for
 * multiple connections i.e. to add the certificates in the provided buffer to the certificate chain.
 * This function implicitly calls aiio_tls_init_global_ca_store() if it has not already been called.
 * The application must call this function before calling aiio_tls_conn_new().
 *
 * @param[in]  cacert_pem_buf    Buffer which has certificates in pem format. This buffer
 *                               is used for creating a global CA store, which can be used
 *                               by other tls connections.
 * @param[in]  cacert_pem_bytes  Length of the buffer.
 *
 * @return
 *             - AIIO_OK  if adding certificates was successful.
 *             - Other   if an error occured or an action must be taken by the calling process.
 */
int aiio_tls_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes);

/**
 * @brief      Free the global CA store currently being used.
 *
 * The memory being used by the global CA store to store all the parsed certificates is
 * freed up. The application can call this API if it no longer needs the global CA store.
 */
void aiio_tls_free_global_ca_store(void);

/**
 * @brief      Returns last error in aiio_tls with detailed mbedtls related error codes.
 *             The error information is cleared internally upon return
 *
 * @param[in]  h              aiio-tls error handle.
 * @param[out] aiio_tls_code   last error code returned from mbedtls api (set to zero if none)
 *                            This pointer could be NULL if caller does not care about aiio_tls_code
 * @param[out] aiio_tls_flags  last certification verification flags (set to zero if none)
 *                            This pointer could be NULL if caller does not care about aiio_tls_code
 *
 * @return
 *            - AIIO_ERR_INVALID_STATE if invalid parameters
 *            - AIIO_OK (0) if no error occurred
 *            - specific error code (based on AIIO_ERR_TLS_BASE) otherwise
 */
int aiio_tls_get_and_clear_last_error(aiio_tls_error_handle_t h, int *aiio_tls_code, int *aiio_tls_flags);

/**
 * @brief      Returns the last error captured in aiio_tls of a specific type
 *             The error information is cleared internally upon return
 *
 * @param[in]  h              aiio-tls error handle.
 * @param[in]  err_type       specific error type
 * @param[out] error_code     last error code returned from mbedtls api (set to zero if none)
 *                            This pointer could be NULL if caller does not care about aiio_tls_code
 * @return
 *            - AIIO_ERR_INVALID_STATE if invalid parameters
 *            - AIIO_OK if a valid error returned and was cleared
 */
int aiio_tls_get_and_clear_error_type(aiio_tls_error_handle_t h, aiio_tls_error_type_t err_type, int *error_code);

#if CONFIG_TLS_USING_MBEDTLS
/**
 * @brief      Get the pointer to the global CA store currently being used.
 *
 * The application must first call aiio_tls_set_global_ca_store(). Then the same
 * CA store could be used by the application for APIs other than aiio_tls.
 *
 * @note       Modifying the pointer might cause a failure in verifying the certificates.
 *
 * @return
 *             - Pointer to the global CA store currently being used    if successful.
 *             - NULL                                                   if there is no global CA store set.
 */
mbedtls_x509_crt *aiio_tls_get_global_ca_store(void);

#endif /* CONFIG_TLS_USING_MBEDTLS */
#ifdef CONFIG_TLS_SERVER
/**
 * @brief      Create TLS/SSL server session
 *
 * This function creates a TLS/SSL server context for already accepted client connection
 * and performs TLS/SSL handshake with the client
 *
 * @param[in]  cfg      Pointer to aiio_tls_cfg_server_t
 * @param[in]  sockfd   FD of accepted connection
 * @param[out] tls      Pointer to allocated aiio_tls_t
 *
 * @return
 *          - 0  if successful
 *          - <0 in case of error
 *
 */
int aiio_tls_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls);

/**
 * @brief      Close the server side TLS/SSL connection and free any allocated resources.
 *
 * This function should be called to close each tls connection opened with aiio_tls_server_session_create()
 *
 * @param[in]  tls  pointer to aiio_tls_t
 */
void aiio_tls_server_session_delete(aiio_tls_t *tls);
#endif /* ! CONFIG_TLS_SERVER */

/**
 * @brief Creates a plain TCP connection, returning a valid socket fd on success or an error handle
 *
 * @param[in]  host      Hostname of the host.
 * @param[in]  hostlen   Length of hostname.
 * @param[in]  port      Port number of the host.
 * @param[in]  cfg       AIIO-TLS configuration as aiio_tls_cfg_t.
 * @param[out] error_handle AIIO-TLS error handle holding potential errors occurred during connection
 * @param[out] sockfd    Socket descriptor if successfully connected on TCP layer
 * @return     AIIO_OK   on success
 *             AIIO_ERR_INVALID_ARG if invalid output parameters
 *             AIIO-TLS based error codes on failure
 */
int aiio_tls_plain_tcp_connect(const char *host, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_error_handle_t error_handle, int *sockfd);

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
/**
 * @brief Obtain the client session ticket
 *
 * This function should be called when the TLS connection is already established.
 * This can be passed again in the aiio_tls_cfg_t structure, to appropriate tls session create (e.g. aiio_tls_conn_http_new_sync) API for session resumption.
 *
 * @param[in]  aiio_tls context as aiio_tls_t
 * @return
 *             Pointer to the saved client session.
 *             NULL     on Failure
 */
aiio_tls_client_session_t *aiio_tls_get_client_session(aiio_tls_t *tls);
#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */
#ifdef __cplusplus
}
#endif

#endif /* ! _TLS_H_ */
