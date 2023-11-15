
#pragma once
#include "aiio_tls.h"
#include "aiio_err.h"

/**
 * Internal Callback API for mbedtls_ssl_read
 */
ssize_t aiio_mbedtls_read(aiio_tls_t *tls, char *data, size_t datalen);

/**
 * Internal callback API for mbedtls_ssl_write
 */
ssize_t aiio_mbedtls_write(aiio_tls_t *tls, const char *data, size_t datalen);

/**
 * Internal Callback for mbedtls_handshake
 */
int aiio_mbedtls_handshake(aiio_tls_t *tls, const aiio_tls_cfg_t *cfg);

/**
 * Internal Callback for mbedtls_cleanup , frees up all the memory used by mbedtls
 */
void aiio_mbedtls_cleanup(aiio_tls_t *tls);

/**
 * Internal Callback for Certificate verification for mbedtls
 */
void aiio_mbedtls_verify_certificate(aiio_tls_t *tls);

/**
 * Internal Callback for deleting the mbedtls connection
 */
void aiio_mbedtls_conn_delete(aiio_tls_t *tls);

/**
 * Internal Callback for mbedtls_get_bytes_avail
 */
ssize_t aiio_mbedtls_get_bytes_avail(aiio_tls_t *tls);

/**
 * Internal Callback for creating ssl handle for mbedtls
 */
int aiio_create_mbedtls_handle(const char *hostname, size_t hostlen, const void *cfg, aiio_tls_t *tls);

/**
 * mbedTLS function for Initializing socket wrappers
 */
static inline void aiio_mbedtls_net_init(aiio_tls_t *tls)
{
    mbedtls_net_init(&tls->server_fd);
}

#ifdef CONFIG_TLS_SERVER
/**
 * Internal Callback for set_server_config
 *
 * /note :- can only be used with mbedtls ssl library
 */
int set_server_config(aiio_tls_cfg_server_t *cfg, aiio_tls_t *tls);

/**
 * Internal Callback for mbedtls_server_session_create
 *
 * /note :- The function can only be used with mbedtls ssl library
 */
int aiio_mbedtls_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls);

/**
 * Internal Callback for mbedtls_server_session_delete
 *
 * /note :- The function can only be used with mbedtls ssl library
 */
void aiio_mbedtls_server_session_delete(aiio_tls_t *tls);

#ifdef CONFIG_TLS_SERVER_SESSION_TICKETS
/**
 * Internal function to setup server side session ticket context
 *
 * /note :- The function can only be used with mbedtls ssl library
 */
int aiio_mbedtls_server_session_ticket_ctx_init(aiio_tls_server_session_ticket_ctx_t *cfg);

/**
 * Internal function to free server side session ticket context
 *
 * /note :- The function can only be used with mbedtls ssl library
 */
void aiio_mbedtls_server_session_ticket_ctx_free(aiio_tls_server_session_ticket_ctx_t *cfg);
#endif
#endif

/**
 * Internal Callback for set_client_config_function
 */
int set_client_config(const char *hostname, size_t hostlen, aiio_tls_cfg_t *cfg, aiio_tls_t *tls);

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
/**
 * Internal Callback for mbedtls_get_client_session
 */
aiio_tls_client_session_t *aiio_mbedtls_get_client_session(aiio_tls_t *tls);
#endif

/**
 * Internal Callback for mbedtls_init_global_ca_store
 */
int aiio_mbedtls_init_global_ca_store(void);

/**
 * Callback function for setting global CA store data for TLS/SSL using mbedtls
 */
int aiio_mbedtls_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes);

/**
 * Internal Callback for aiio_tls_global_ca_store
 */
mbedtls_x509_crt *aiio_mbedtls_get_global_ca_store(void);

/**
 * Callback function for freeing global ca store for TLS/SSL using mbedtls
 */
void aiio_mbedtls_free_global_ca_store(void);
