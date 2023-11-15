
#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <aiio_error.h>
#include <stdbool.h>
#include "aiio_err.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
*  @brief Keep alive parameters structure
*/
typedef struct aiio_transport_keepalive {
    bool            keep_alive_enable;      /*!< Enable keep-alive timeout */
    int             keep_alive_idle;        /*!< Keep-alive idle time (second) */
    int             keep_alive_interval;    /*!< Keep-alive interval time (second) */
    int             keep_alive_count;       /*!< Keep-alive packet retry send count */
} aiio_transport_keep_alive_t;

typedef struct aiio_transport_internal* aiio_transport_list_handle_t;
typedef struct aiio_transport_item_t* aiio_transport_handle_t;

typedef int (*connect_func)(aiio_transport_handle_t t, const char *host, int port, int timeout_ms);
typedef int (*io_func)(aiio_transport_handle_t t, const char *buffer, int len, int timeout_ms);
typedef int (*io_read_func)(aiio_transport_handle_t t, char *buffer, int len, int timeout_ms);
typedef int (*trans_func)(aiio_transport_handle_t t);
typedef int (*poll_func)(aiio_transport_handle_t t, int timeout_ms);
typedef int (*connect_async_func)(aiio_transport_handle_t t, const char *host, int port, int timeout_ms);
typedef aiio_transport_handle_t (*payload_transfer_func)(aiio_transport_handle_t);

typedef struct aiio_tls_last_error* aiio_tls_error_handle_t;

/**
 * @brief      Create transport list
 *
 * @return     A handle can hold all transports
 */
aiio_transport_list_handle_t aiio_transport_list_init(void);

/**
 * @brief      Cleanup and free all transports, include itself,
 *             this function will invoke aiio_transport_destroy of every transport have added this the list
 *
 * @param[in]  list  The list
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_FAIL
 */
int aiio_transport_list_destroy(aiio_transport_list_handle_t list);

/**
 * @brief      Add a transport to the list, and define a scheme to indentify this transport in the list
 *
 * @param[in]  list    The list
 * @param[in]  t       The Transport
 * @param[in]  scheme  The scheme
 *
 * @return
 *     - AIIO_OK
 */
int aiio_transport_list_add(aiio_transport_list_handle_t list, aiio_transport_handle_t t, const char *scheme);

/**
 * @brief      This function will remove all transport from the list,
 *             invoke aiio_transport_destroy of every transport have added this the list
 *
 * @param[in]  list  The list
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_ERR_INVALID_ARG
 */
int aiio_transport_list_clean(aiio_transport_list_handle_t list);

/**
 * @brief      Get the transport by scheme, which has been defined when calling function `aiio_transport_list_add`
 *
 * @param[in]  list  The list
 * @param[in]  tag   The tag
 *
 * @return     The transport handle
 */
aiio_transport_handle_t aiio_transport_list_get_transport(aiio_transport_list_handle_t list, const char *scheme);

/**
 * @brief      Initialize a transport handle object
 *
 * @return     The transport handle
 */
aiio_transport_handle_t aiio_transport_init(void);

/**
 * @brief      Cleanup and free memory the transport
 *
 * @param[in]  t     The transport handle
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_FAIL
 */
int aiio_transport_destroy(aiio_transport_handle_t t);

/**
 * @brief      Get default port number used by this transport
 *
 * @param[in]  t     The transport handle
 *
 * @return     the port number
 */
int aiio_transport_get_default_port(aiio_transport_handle_t t);

/**
 * @brief      Set default port number that can be used by this transport
 *
 * @param[in]  t     The transport handle
 * @param[in]  port  The port number
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_FAIL
 */
int aiio_transport_set_default_port(aiio_transport_handle_t t, int port);

/**
 * @brief      Transport connection function, to make a connection to server
 *
 * @param      t           The transport handle
 * @param[in]  host        Hostname
 * @param[in]  port        Port
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 * - socket for will use by this transport
 * - (-1) if there are any errors, should check errno
 */
int aiio_transport_connect(aiio_transport_handle_t t, const char *host, int port, int timeout_ms);

/**
 * @brief      Non-blocking transport connection function, to make a connection to server
 *
 * @param      t           The transport handle
 * @param[in]  host        Hostname
 * @param[in]  port        Port
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 * - socket for will use by this transport
 * - (-1) if there are any errors, should check errno
 */
int aiio_transport_connect_async(aiio_transport_handle_t t, const char *host, int port, int timeout_ms);

/**
 * @brief      Transport read function
 *
 * @param      t           The transport handle
 * @param      buffer      The buffer
 * @param[in]  len         The length
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 *  - Number of bytes was read
 *  - (-1) if there are any errors, should check errno
 */
int aiio_transport_read(aiio_transport_handle_t t, char *buffer, int len, int timeout_ms);

/**
 * @brief      Poll the transport until readable or timeout
 *
 * @param[in]  t           The transport handle
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 *     - 0      Timeout
 *     - (-1)   If there are any errors, should check errno
 *     - other  The transport can read
 */
int aiio_transport_poll_read(aiio_transport_handle_t t, int timeout_ms);

/**
 * @brief      Transport write function
 *
 * @param      t           The transport handle
 * @param      buffer      The buffer
 * @param[in]  len         The length
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 *  - Number of bytes was written
 *  - (-1) if there are any errors, should check errno
 */
int aiio_transport_write(aiio_transport_handle_t t, const char *buffer, int len, int timeout_ms);

/**
 * @brief      Poll the transport until writeable or timeout
 *
 * @param[in]  t           The transport handle
 * @param[in]  timeout_ms  The timeout milliseconds (-1 indicates wait forever)
 *
 * @return
 *     - 0      Timeout
 *     - (-1)   If there are any errors, should check errno
 *     - other  The transport can write
 */
int aiio_transport_poll_write(aiio_transport_handle_t t, int timeout_ms);

/**
 * @brief      Transport close
 *
 * @param      t     The transport handle
 *
 * @return
 * - 0 if ok
 * - (-1) if there are any errors, should check errno
 */
int aiio_transport_close(aiio_transport_handle_t t);

/**
 * @brief      Get user data context of this transport
 *
 * @param[in]  t        The transport handle
 *
 * @return     The user data context
 */
void *aiio_transport_get_context_data(aiio_transport_handle_t t);

/**
 * @brief      Get transport handle of underlying protocol
 *             which can access this protocol payload directly
 *             (used for receiving longer msg multiple times)
 *
 * @param[in]  t        The transport handle
 *
 * @return     Payload transport handle
 */
aiio_transport_handle_t aiio_transport_get_payload_transport_handle(aiio_transport_handle_t t);

/**
 * @brief      Set the user context data for this transport
 *
 * @param[in]  t        The transport handle
 * @param      data     The user data context
 *
 * @return
 *     - AIIO_OK
 */
int aiio_transport_set_context_data(aiio_transport_handle_t t, void *data);

/**
 * @brief      Set transport functions for the transport handle
 *
 * @param[in]  t            The transport handle
 * @param[in]  _connect     The connect function pointer
 * @param[in]  _read        The read function pointer
 * @param[in]  _write       The write function pointer
 * @param[in]  _close       The close function pointer
 * @param[in]  _poll_read   The poll read function pointer
 * @param[in]  _poll_write  The poll write function pointer
 * @param[in]  _destroy     The destroy function pointer
 *
 * @return
 *     - AIIO_OK
 */
int aiio_transport_set_func(aiio_transport_handle_t t,
                             connect_func _connect,
                             io_read_func _read,
                             io_func _write,
                             trans_func _close,
                             poll_func _poll_read,
                             poll_func _poll_write,
                             trans_func _destroy);


/**
 * @brief      Set transport functions for the transport handle
 *
 * @param[in]  t                    The transport handle
 * @param[in]  _connect_async_func  The connect_async function pointer
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_FAIL
 */
int aiio_transport_set_async_connect_func(aiio_transport_handle_t t, connect_async_func _connect_async_func);

/**
 * @brief      Set parent transport function to the handle
 *
 * @param[in]  t                    The transport handle
 * @param[in]  _parent_transport    The underlying transport getter pointer
 *
 * @return
 *     - AIIO_OK
 *     - AIIO_FAIL
 */
int aiio_transport_set_parent_transport_func(aiio_transport_handle_t t, payload_transfer_func _parent_transport);

/**
 * @brief      Returns aiio_tls error handle.
 *             Warning: The returned pointer is valid only as long as aiio_transport_handle_t exists. Once transport
 *             handle gets destroyed, this value (aiio_tls_error_handle_t) is freed automatically.
 *
 * @param[in]  A transport handle
 *
 * @return
 *            - valid pointer of aiio_error_handle_t
 *            - NULL if invalid transport handle
 */
aiio_tls_error_handle_t aiio_transport_get_error_handle(aiio_transport_handle_t t);

/**
 * @brief      Get and clear last captured socket errno
 *
 * Socket errno is internally stored whenever any of public facing API
 * for reading, writing, polling or connection fails returning negative return code.
 * The error code corresponds to the `SO_ERROR` value retrieved from the underlying
 * transport socket using `getsockopt()` API. After reading the captured errno,
 * the internal value is cleared to 0.
 *
 * @param[in] t The transport handle
 *
 * @return
 *   - >=0 Last captured socket errno
 *   - -1  Invalid transport handle or invalid transport's internal error storage
 */
int aiio_transport_get_errno(aiio_transport_handle_t t);

#ifdef __cplusplus
}
#endif
#endif /* _TRANSPORT_ */
