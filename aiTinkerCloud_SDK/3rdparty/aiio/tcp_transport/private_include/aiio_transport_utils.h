
#ifndef _TRANSPORT_UTILS_H_
#define _TRANSPORT_UTILS_H_
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Utility macro to be used for NULL ptr check after malloc
 *
 */
#define AIIO_TRANSPORT_MEM_CHECK(a, action) if (!(a)) {                                         \
        aiio_log_e("%s(%d): %s", __FUNCTION__, __LINE__, "Memory exhausted");                     \
        action;                                                                                     \
        }

/**
 * @brief      Utility macro for checking the error code of int
 */
#define AIIO_TRANSPORT_ERR_OK_CHECK(err, action) \
        {                                                     \
            int _transport_err_to_check = err;      \
            if (_transport_err_to_check != AIIO_OK) {      \
                aiio_log_e("%s(%d): Expected AIIO_OK; reported: %d", __FUNCTION__, __LINE__, _transport_err_to_check); \
                action;                                       \
            }                                                 \
        }

/**
 * @brief      Convert milliseconds to timeval struct for valid timeouts, otherwise
 *             (if "wait forever" requested by timeout_ms=-1) timeval structure is not updated and NULL returned
 *
 * @param[in]  timeout_ms  The timeout value in milliseconds or -1 to waiting forever
 * @param[out] tv          Pointer to timeval struct
 *
 * @return
 * - NULL if timeout_ms=-1 (wait forever)
 * - pointer to the updated timeval structure (provided as "tv" argument) with recalculated timeout value
 */
struct timeval* aiio_transport_utils_ms_to_timeval(int timeout_ms, struct timeval *tv);


#ifdef __cplusplus
}
#endif
#endif /* _TRANSPORT_UTILS_H_ */
