
#pragma once
#include "aiio_tls.h"

/**
 * Internal Callback for creating ssl handle for wolfssl
 */
int aiio_create_wolfssl_handle(const char *hostname, size_t hostlen, const void *cfg, aiio_tls_t *tls);

/**
 * Internal Callback for wolfssl_handshake
 */
int aiio_wolfssl_handshake(aiio_tls_t *tls, const aiio_tls_cfg_t *cfg);

/**
 * Internal Callback API for wolfssl_ssl_read
 */
ssize_t aiio_wolfssl_read(aiio_tls_t *tls, char *data, size_t datalen);

/**
 * Internal callback API for wolfssl_ssl_write
 */
ssize_t aiio_wolfssl_write(aiio_tls_t *tls, const char *data, size_t datalen);

/**
 * Internal Callback for wolfssl_cleanup , frees up all the memory used by wolfssl
 */
void aiio_wolfssl_cleanup(aiio_tls_t *tls);

/**
 * Internal Callback for Certificate verification for wolfssl
 */
void aiio_wolfssl_verify_certificate(aiio_tls_t *tls);

/**
 * Internal Callback for deleting the wolfssl connection
 */
void aiio_wolfssl_conn_delete(aiio_tls_t *tls);

/**
 * Internal Callback for wolfssl_get_bytes_avail
 */
ssize_t aiio_wolfssl_get_bytes_avail(aiio_tls_t *tls);

/**
 * Callback function for setting global CA store data for TLS/SSL using wolfssl
 */
int aiio_wolfssl_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes);

/**
 * Callback function for freeing global ca store for TLS/SSL using wolfssl
 */
void aiio_wolfssl_free_global_ca_store(void);

/**
 *
 * Callback function for Initializing the global ca store for TLS?SSL using wolfssl
 */
int aiio_wolfssl_init_global_ca_store(void);

/**
 * wolfSSL function for Initializing socket wrappers (no-operation for wolfSSL)
 */
static inline void aiio_wolfssl_net_init(aiio_tls_t *tls)
{
}

#ifdef CONFIG_TLS_SERVER

/**
 * Function to Create AIIO-TLS Server session with wolfssl Stack
 */
int aiio_wolfssl_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls);

/*
 * Delete Server Session
 */
void aiio_wolfssl_server_session_delete(aiio_tls_t *tls);

#endif
