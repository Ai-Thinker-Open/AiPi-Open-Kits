
#ifndef __AIIO_TLS_ERROR_CAPTURE_INTERNAL_H__
#define __AIIO_TLS_ERROR_CAPTURE_INTERNAL_H__
/**
* Note: this is an implementation placeholder for error logger.
* This version is internal to aiio-tls component and only saves single aiio_err of last occurred error
*/
#include "aiio_tls_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error tracker logging macro to enable mapping tracking errors internally
 * or using an external/global implementation
 */
#define AIIO_INT_EVENT_TRACKER_CAPTURE(h, type, code)    aiio_tls_internal_event_tracker_capture(h, type, code)

/**
 * @brief Internal tracker capture error
 *
 * This implementation saves latest errors of available types
 *
 * @param[in]  h        aiio-tls error handle
 * @param[in]  err_type Specific error type
 * @param[int] code     Error code to capture
 *
 */
void aiio_tls_internal_event_tracker_capture(aiio_tls_error_handle_t h, uint32_t type, int code);

/**
 * @brief Create internal tracker storage
 *
 * @return Error tracker handle if success or NULL if allocation error
 */
aiio_tls_error_handle_t aiio_tls_internal_event_tracker_create(void);

/**
 * @brief Destroy internal tracker storage
 *
 * @param[in]  h        aiio-tls error handle
 */
 void aiio_tls_internal_event_tracker_destroy(aiio_tls_error_handle_t h);

#ifdef __cplusplus
}
#endif

#endif //__AIIO_TLS_ERROR_CAPTURE_INTERNAL_H__
