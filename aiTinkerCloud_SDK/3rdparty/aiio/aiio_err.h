#ifndef __AIIO_ERR_H__
#define __AIIO_ERR_H__

#include "stdint.h"
#include "sdkconfig.h"
#include "aiio_check.h"
#include "aiio_log.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef int aiio_err_t;
/* Definitions for error constants. */
#define AIIO_OK          0       /*!< esp_err_t value indicating success (no error) */
#define AIIO_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define AIIO_ERR_NO_MEM              0x101   /*!< Out of memory */
#define AIIO_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define AIIO_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define AIIO_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define AIIO_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define AIIO_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define AIIO_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define AIIO_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define AIIO_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define AIIO_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define AIIO_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
#define AIIO_ERR_NOT_FINISHED        0x10C   /*!< There are items remained to retrieve */


#define AIIO_ERR_WIFI_BASE           0x3000  /*!< Starting number of WiFi error codes */
#define AIIO_ERR_MESH_BASE           0x4000  /*!< Starting number of MESH error codes */
#define AIIO_ERR_FLASH_BASE          0x6000  /*!< Starting number of flash error codes */
#define AIIO_ERR_HW_CRYPTO_BASE      0xc000  /*!< Starting number of HW cryptography module error codes */
#define AIIO_ERR_MEMPROT_BASE        0xd000  /*!< Starting number of Memory Protection API error codes */

extern int printf(const char *format, ...);

#if 1
#ifndef AIIO_LOGI
#define AIIO_LOGI(x, ...)
#endif
#ifndef AIIO_LOGW
#define AIIO_LOGW(x, ...)
#endif
#ifndef AIIO_LOGD
#define AIIO_LOGD(x, ...)
#endif
#ifndef AIIO_LOGE
#define AIIO_LOGE(x, ...)
#endif
#ifndef AIIO_LOGV
#define AIIO_LOGV(x, ...)
#endif
#ifndef AIIO_LOGE_IRQ
#define AIIO_LOGE_IRQ(x, ...)
#endif
#else
#ifndef AIIO_LOGI
#define AIIO_LOGI(x, ...) printf(x, ##__VA_ARGS__)
#endif
#ifndef AIIO_LOGW
#define AIIO_LOGW(x, ...) printf(x, ##__VA_ARGS__)
#endif
#ifndef AIIO_LOGD
#define AIIO_LOGD(x, ...) printf(x, ##__VA_ARGS__)
#endif
#ifndef AIIO_LOGE
#define AIIO_LOGE(x, ...) printf(x, ##__VA_ARGS__)
#endif
#ifndef AIIO_LOGV
#define AIIO_LOGV(x, ...) printf(x, ##__VA_ARGS__)
#endif
#ifndef AIIO_LOGE_IRQ
#define AIIO_LOGE_IRQ(x, ...) printf(x, ##__VA_ARGS__)
#endif
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__AIIO_ERR_H__
