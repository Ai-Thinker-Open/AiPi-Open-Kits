
#ifndef _TRANSPORT_TCP_H_
#define _TRANSPORT_TCP_H_

#include "aiio_transport.h"
// #include <sys/socket.h>
#include "aiio_chip_spec.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Set TCP keep-alive configuration
 *
 * @param[in]  t               The transport handle
 * @param[in]  keep_alive_cfg  The keep-alive config
 *
 */
void aiio_transport_tcp_set_keep_alive(aiio_transport_handle_t t, aiio_transport_keep_alive_t *keep_alive_cfg);

/**
 * @brief      Set name of interface that socket can be binded on
 *             So the data can transport on this interface
 *
 * @param[in]  t        The transport handle
 * @param[in]  if_name  The interface name
 */
void aiio_transport_tcp_set_interface_name(aiio_transport_handle_t t, struct ifreq *if_name);

/**
 * @brief      Create TCP transport, the transport handle must be release aiio_transport_destroy callback
 *
 * @return  the allocated aiio_transport_handle_t, or NULL if the handle can not be allocated
 */
aiio_transport_handle_t aiio_transport_tcp_init(void);


#ifdef __cplusplus
}
#endif

#endif /* _TRANSPORT_TCP_H_ */
