/**
 * @file        sdkconfig.h
 * @brief
 * @author      Jimmy (ccolacat@163.com)
 * @version     v1.0.0
 * @date        2023-03-24
 *
 * @copyright   Copyright (c) 2020-2023 Shenzhen Ai-Thinker Technology Co., Ltd. All Rights Reserved.
 *
 * Disclaimer:  This program is for informational purposes only, please keep author information and source code license.
 *
 */

#ifndef __HTTP_SDKCONFIG_H__
#define __HTTP_SDKCONFIG_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// TLS
// XXX these configs should sync with mbedtls_sample_config.h
#define CONFIG_MBEDTLS_SSL_ALPN
#define CONFIG_TLS_PSK_VERIFICATION 1
#define CONFIG_TLS_SKIP_SERVER_CERT_VERIFY 1    //允许跳过服务器证书校验

// MQTT
#define CONFIG_MQTT_TRANSPORT_SSL 1
#define CONFIG_MQTT_TRANSPORT_WEBSOCKET 1
#define CONFIG_MQTT_TRANSPORT_WEBSOCKET_SECURE 1
#define CONFIG_MQTT_PROTOCOL_311
#define CONFIG_MQTT_TASK_STACK_SIZE (2*1024)

// HTTP
#define CONFIG_HTTP_CLIENT_ENABLE_HTTPS (1)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__HTTP_SDKCONFIG_H__
/************************ Copyright (c) 2023 Shenzhen Ai-Thinker Technology Co., Ltd. ************************/
