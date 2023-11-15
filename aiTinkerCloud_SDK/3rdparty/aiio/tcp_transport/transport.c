
#include <stdlib.h>
#include <string.h>
#include <aiio_tls.h>

#include "sys/queue.h"
#include "aiio_log.h"

#include "aiio_transport.h"
#include "aiio_transport_internal.h"
#include "aiio_transport_utils.h"

static const char *TAG = "TRANSPORT";

/**
 * Transport layer error structure including
 * * aiio-tls last error storage
 * * sock-errno
 */
struct aiio_transport_error_storage {
    struct aiio_tls_last_error aiio_tls_err_h_base;   /*!< aiio-tls last error container */
    // additional fields
    int    sock_errno;                              /*!< last socket error captured for this transport */
};

/**
 * This list will hold all transport available
 */
STAILQ_HEAD(aiio_transport_list_t, aiio_transport_item_t);

struct transport_tls;

/**
 * Internal transport structure holding list of transports and other data common to all transports
 */
typedef struct aiio_transport_internal {
    struct aiio_transport_list_t list;                      /*!< List of transports */
    struct aiio_foundation_transport *base;       /*!< Base transport pointer shared for each list item */
} aiio_transport_internal_t;

static aiio_foundation_transport_t * aiio_transport_init_foundation_transport(void)
{
    aiio_foundation_transport_t *foundation = calloc(1, sizeof(aiio_foundation_transport_t));
    AIIO_TRANSPORT_MEM_CHECK( foundation, return NULL);
    foundation->error_handle = calloc(1, sizeof(struct aiio_transport_error_storage));
    AIIO_TRANSPORT_MEM_CHECK( foundation->error_handle,
                        free(foundation);
                        return NULL);
    foundation->transport_tls = aiio_transport_tls_create();
    AIIO_TRANSPORT_MEM_CHECK( foundation->transport_tls,
                        free(foundation->error_handle);
                        free(foundation);
                        return NULL);
    return foundation;
}

static void aiio_transport_destroy_foundation_transport(aiio_foundation_transport_t *foundation)
{
    aiio_transport_tls_destroy(foundation->transport_tls);
    free(foundation->error_handle);
    free(foundation);
}

static aiio_transport_handle_t aiio_transport_get_default_parent(aiio_transport_handle_t t)
{
    /*
    * By default, the underlying transport layer handle is the handle itself
    */
    return t;
}

aiio_transport_list_handle_t aiio_transport_list_init(void)
{
    aiio_transport_list_handle_t transport = calloc(1, sizeof(aiio_transport_internal_t));
    AIIO_TRANSPORT_MEM_CHECK( transport, return NULL);
    STAILQ_INIT(&transport->list);
    transport->base = aiio_transport_init_foundation_transport();
    AIIO_TRANSPORT_MEM_CHECK( transport->base,
                            free(transport);
                            return NULL);
    return transport;
}

int aiio_transport_list_add(aiio_transport_list_handle_t h, aiio_transport_handle_t t, const char *scheme)
{
    if (h == NULL || t == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    t->scheme = calloc(1, strlen(scheme) + 1);
    AIIO_TRANSPORT_MEM_CHECK( t->scheme, return AIIO_ERR_NO_MEM);
    strcpy(t->scheme, scheme);
    STAILQ_INSERT_TAIL(&h->list, t, next);
    // Each transport in a list to share the same error tracker
    t->base = h->base;
    return AIIO_OK;
}

aiio_transport_handle_t aiio_transport_list_get_transport(aiio_transport_list_handle_t h, const char *scheme)
{
    if (!h) {
        return NULL;
    }
    if (scheme == NULL) {
        return STAILQ_FIRST(&h->list);
    }
    aiio_transport_handle_t item;
    STAILQ_FOREACH(item, &h->list, next) {
        if (strcasecmp(item->scheme, scheme) == 0) {
            return item;
        }
    }
    return NULL;
}

int aiio_transport_list_destroy(aiio_transport_list_handle_t h)
{
    aiio_transport_list_clean(h);
    aiio_transport_destroy_foundation_transport(h->base);
    free(h);
    return AIIO_OK;
}

int aiio_transport_list_clean(aiio_transport_list_handle_t h)
{
    aiio_transport_handle_t item = STAILQ_FIRST(&h->list);
    aiio_transport_handle_t tmp;
    while (item != NULL) {
        tmp = STAILQ_NEXT(item, next);
        aiio_transport_destroy(item);
        item = tmp;
    }
    STAILQ_INIT(&h->list);
    return AIIO_OK;
}

aiio_transport_handle_t aiio_transport_init(void)
{
    aiio_transport_handle_t t = calloc(1, sizeof(struct aiio_transport_item_t));
    AIIO_TRANSPORT_MEM_CHECK( t, return NULL);
    return t;
}

aiio_transport_handle_t aiio_transport_get_payload_transport_handle(aiio_transport_handle_t t)
{
    if (t && t->_read) {
        return t->_parent_transfer(t);
    }
    return NULL;
}

int aiio_transport_destroy(aiio_transport_handle_t t)
{
    if (t->_destroy) {
        t->_destroy(t);
    }
    if (t->scheme) {
        free(t->scheme);
    }
    free(t);
    return AIIO_OK;
}

int aiio_transport_connect(aiio_transport_handle_t t, const char *host, int port, int timeout_ms)
{
    int ret = -1;
    if (t && t->_connect) {
        return t->_connect(t, host, port, timeout_ms);
    }
    return ret;
}

int aiio_transport_connect_async(aiio_transport_handle_t t, const char *host, int port, int timeout_ms)
{
    int ret = -1;
    if (t && t->_connect_async) {
        return t->_connect_async(t, host, port, timeout_ms);
    }
    return ret;
}

int aiio_transport_read(aiio_transport_handle_t t, char *buffer, int len, int timeout_ms)
{
    if (t && t->_read) {
        return t->_read(t, buffer, len, timeout_ms);
    }
    return -1;
}

int aiio_transport_write(aiio_transport_handle_t t, const char *buffer, int len, int timeout_ms)
{
    if (t && t->_write) {
        return t->_write(t, buffer, len, timeout_ms);
    }
    return -1;
}

int aiio_transport_poll_read(aiio_transport_handle_t t, int timeout_ms)
{
    if (t && t->_poll_read) {
        return t->_poll_read(t, timeout_ms);
    }
    return -1;
}

int aiio_transport_poll_write(aiio_transport_handle_t t, int timeout_ms)
{
    if (t && t->_poll_write) {
        return t->_poll_write(t, timeout_ms);
    }
    return -1;
}

int aiio_transport_close(aiio_transport_handle_t t)
{
    if (t && t->_close) {
        return t->_close(t);
    }
    return 0;
}

void *aiio_transport_get_context_data(aiio_transport_handle_t t)
{
    if (t) {
        return t->data;
    }
    return NULL;
}

int aiio_transport_set_context_data(aiio_transport_handle_t t, void *data)
{
    if (t) {
        t->data = data;
        return AIIO_OK;
    }
    return AIIO_FAIL;
}

int aiio_transport_set_func(aiio_transport_handle_t t,
                             connect_func _connect,
                             io_read_func _read,
                             io_func _write,
                             trans_func _close,
                             poll_func _poll_read,
                             poll_func _poll_write,
                             trans_func _destroy)
{
    if (t == NULL) {
        return AIIO_FAIL;
    }
    t->_connect = _connect;
    t->_read = _read;
    t->_write = _write;
    t->_close = _close;
    t->_poll_read = _poll_read;
    t->_poll_write = _poll_write;
    t->_destroy = _destroy;
    t->_connect_async = NULL;
    t->_parent_transfer = aiio_transport_get_default_parent;
    return AIIO_OK;
}

int aiio_transport_get_default_port(aiio_transport_handle_t t)
{
    if (t == NULL) {
        return -1;
    }
    return t->port;
}

int aiio_transport_set_default_port(aiio_transport_handle_t t, int port)
{
    if (t == NULL) {
        return AIIO_FAIL;
    }
    t->port = port;
    return AIIO_OK;
}

int aiio_transport_set_async_connect_func(aiio_transport_handle_t t, connect_async_func _connect_async_func)
{
    if (t == NULL) {
        return AIIO_FAIL;
    }
    t->_connect_async = _connect_async_func;
    return AIIO_OK;
}

int aiio_transport_set_parent_transport_func(aiio_transport_handle_t t, payload_transfer_func _parent_transport)
{
    if (t == NULL) {
        return AIIO_FAIL;
    }
    t->_parent_transfer = _parent_transport;
    return AIIO_OK;
}

aiio_tls_error_handle_t aiio_transport_get_error_handle(aiio_transport_handle_t t)
{
    if (t && t->base && t->base->error_handle) {
        return &t->base->error_handle->aiio_tls_err_h_base;
    }
    return NULL;
}

int aiio_transport_get_errno(aiio_transport_handle_t t)
{
    if (t && t->base && t->base->error_handle) {
        int actual_errno = t->base->error_handle->sock_errno;
        t->base->error_handle->sock_errno = 0;
        return actual_errno;
    }
    return -1;
}

void capture_tcp_transport_error(aiio_transport_handle_t t, enum tcp_transport_errors error)
{
    aiio_tls_last_error_t *err_handle = aiio_transport_get_error_handle(t);
    switch (error) {
        case ERR_TCP_TRANSPORT_CONNECTION_TIMEOUT:
            err_handle->last_error = AIIO_ERR_TLS_CONNECTION_TIMEOUT;
            break;
        case ERR_TCP_TRANSPORT_CANNOT_RESOLVE_HOSTNAME:
            err_handle->last_error = AIIO_ERR_TLS_CANNOT_RESOLVE_HOSTNAME;
            break;
        case ERR_TCP_TRANSPORT_CONNECTION_CLOSED_BY_FIN:
            err_handle->last_error = AIIO_ERR_TLS_TCP_CLOSED_FIN;
            break;
        case ERR_TCP_TRANSPORT_CONNECTION_FAILED:
            err_handle->last_error = AIIO_ERR_TLS_FAILED_CONNECT_TO_HOST;
            break;
        case ERR_TCP_TRANSPORT_SETOPT_FAILED:
            err_handle->last_error = AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            break;
        case ERR_TCP_TRANSPORT_NO_MEM:
            err_handle->last_error = AIIO_ERR_NO_MEM;
            break;
    }
}

void aiio_transport_set_errors(aiio_transport_handle_t t, const aiio_tls_error_handle_t error_handle)
{
    if (t && t->base && t->base->error_handle) {
        memcpy(&t->base->error_handle->aiio_tls_err_h_base, error_handle, sizeof(aiio_tls_last_error_t));
        int sock_error;
        if (aiio_tls_get_and_clear_error_type(error_handle, AIIO_TLS_ERR_TYPE_SYSTEM, &sock_error) == AIIO_OK) {
            t->base->error_handle->sock_errno = sock_error;
        }
    }
}

void aiio_transport_capture_errno(aiio_transport_handle_t t, int sock_errno)
{
    if (t && t->base && t->base->error_handle) {
        t->base->error_handle->sock_errno = sock_errno;
    }
}

int aiio_transport_get_socket(aiio_transport_handle_t t)
{
    if (t && t->_get_socket)  {
        return  t->_get_socket(t);
    }
    return -1;
}
