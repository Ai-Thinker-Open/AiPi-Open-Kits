
#include <stdlib.h>
#include "aiio_tls.h"
#include "aiio_tls_error_capture_internal.h"

typedef struct aiio_tls_error_storage {
    struct aiio_tls_last_error parent;   /*!< standard aiio-tls last error container */
    int    sock_errno;                  /*!< last socket error captured in aiio-tls */
} aiio_tls_error_storage_t;

void aiio_tls_internal_event_tracker_capture(aiio_tls_error_handle_t h, uint32_t type, int code)
{
    if (h) {
        aiio_tls_error_storage_t * storage = __containerof(h, aiio_tls_error_storage_t, parent);

        if (type == AIIO_TLS_ERR_TYPE_AIIO) {
            storage->parent.last_error = code;
        } else if (type == AIIO_TLS_ERR_TYPE_MBEDTLS ||
                   type == AIIO_TLS_ERR_TYPE_WOLFSSL) {
            storage->parent.aiio_tls_error_code = code;
        } else if (type == AIIO_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS ||
                   type == AIIO_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS) {
            storage->parent.aiio_tls_flags = code;
        } else if (type == AIIO_TLS_ERR_TYPE_SYSTEM) {
            storage->sock_errno = code;
        }
    }
}

aiio_tls_error_handle_t aiio_tls_internal_event_tracker_create(void)
{
    // Allocating internal error storage which extends the parent type
    // `aiio_tls_last_error` defined at interface level
    struct aiio_tls_error_storage* storage =
            calloc(1, sizeof(struct aiio_tls_error_storage));
    return &storage->parent;
}

void aiio_tls_internal_event_tracker_destroy(aiio_tls_error_handle_t h)
{
    aiio_tls_error_storage_t * storage = __containerof(h, aiio_tls_error_storage_t, parent);
    free(storage);
}

int aiio_tls_get_and_clear_error_type(aiio_tls_error_handle_t h, aiio_tls_error_type_t type, int *code)
{
    if (h && type < AIIO_TLS_ERR_TYPE_MAX && code) {
        aiio_tls_error_storage_t * storage = __containerof(h, aiio_tls_error_storage_t, parent);
        if (type == AIIO_TLS_ERR_TYPE_AIIO) {
            *code = storage->parent.last_error;
            storage->parent.last_error = 0;
        } else if (type == AIIO_TLS_ERR_TYPE_MBEDTLS ||
                   type == AIIO_TLS_ERR_TYPE_WOLFSSL) {
            *code = storage->parent.aiio_tls_error_code;
            storage->parent.aiio_tls_error_code = 0;
        } else if (type == AIIO_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS ||
                   type == AIIO_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS) {
            *code = storage->parent.aiio_tls_flags;
            storage->parent.aiio_tls_flags = 0;
        } else if (type == AIIO_TLS_ERR_TYPE_SYSTEM) {
            *code = storage->sock_errno;
            storage->sock_errno = 0;
        } else {
            return AIIO_ERR_INVALID_ARG;
        }
        return AIIO_OK;
    }
    return AIIO_ERR_INVALID_ARG;
}
