/**
 * @brief   声明一些公共的变量、结构体
 *
 * @file    aiio_common.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *
 *
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Authorhor           <th>Notes
 * <tr><td>2023-06-12          <td>1.0.0            <td>Ai-Thinker             <td>
 */

#ifndef __AIIO_COMMON_H_
#define __AIIO_COMMON_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_os_port.h"
#include "aiio_errcode.h"
#include "aiio_error.h"
#include "aiio_protocol_comm.h"
#include "aiio_rtc_timer.h"
#include "aiio_ble_config_wifi.h"
#include "aiio_cloud_rtc.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>
#include "queue.h"
#include "aiio_wifi.h"
#include "aiio_ble_config.h"
#include "aiio_ble_protocol.h"


#define         DEVICE_FW_VERSION           ("1.2.0")                   /*!< The version of firmware，it will report to cloud，and show on APP*/


#define FLASH_RW_START_ADDR                 (0x3F3000)
#define FLASH_RW_WIFI_DATA_ADDR              (FLASH_RW_START_ADDR)
#define FLASH_RW_DEVICE_INFO_ADDR              (0x3F3000 + 0x1000)
#define FLASH_RW_OTA_INFO_ADDR              (0x3F3000 + 0x1000 * 2)


#if __has_include("aiio_dev_config.h")
#include "aiio_dev_config.h" 
#else
#define         DEVICE_CLIENT_ID            "XIYWg4Kuuaa1mz"
#define         DEVICE_USER_NAME            "1nljlu"
#define         DEVICE_PASSWD               "UyqoxMou0w0Zxq"     
#endif

typedef enum
{
    REV_CONFIG_DATA_EVENT,                          /*!< Get  config data event in  distribution network, it will callback when device got  config data*/
    REV_CONFIG_START_EVENT,                         /*!< Start event in  distribution network,it will callback when key is pressed*/
    REV_CONFIG_STOP_EVENT,                          /*!< Stop event in  distribution network,it will callback when key is pressed*/
    REV_BLE_DISCONNECTED_EVENT,                         /*!< BLE disconnect event, it will callback when device disconnected with APP*/
    REV_CONFIG_OK_EVENT,                            /*!< Success event in  distribution network，it will callback when device is online*/
    REV_CONFIG_FAIL_EVENT,                          /*!< FAIL event in  distribution network，it will callback when device is fail in  distribution network*/
    REV_WIFI_CONNECTED_EVENT,                           /*!< Wifi connected event，it will callback when device connected wifi*/
    REV_CLOUD_CONTRL_DATA_EVENT,                        /*!< Received to be control attribute event，it will callback when device received data from cloud*/
    REV_CLOUD_ONLINE_EVENT,                              /*!< Online event，it will callback when device report online status to cloud */
    REV_CLOUD_CONNECTED_EVENT,                          /*!< Cloud connectedc event, it will callback when device connected cloud by mqtt*/
    REV_CLOUD_DISCONNECTED,                              /*!< Cloud disconnected event, it will callback when device disconnected cloud by mqtt*/
    REV_CLOUD_OTA_INFO_EVENT,                           /*!< recevice ota information, ota information need to be stored in flash, the ota version needs to be used to determine whether the firmware upgrade is successful*/
    REV_CLOUD_OTA_FAIL_EVENT,                                 /*!< ota data download fail*/
    REV_CLOUD_OTA_SUCCESS_EVENT,                              /*!< ota data download success*/

}aiio_common_event_t;


typedef struct
{
    char ssid[33];                              /*!< The ssid of rout*/
    char passwd[65];                             /*!< The password of rout */
    char wificc[10];                             /*!< Country code */
    char mqttip[200];                             /*!< The ip address or doman of  remote service  */
    char token[65];                             /*!< Activaty information，it is given from cloud by app */
    char tz[20];                                /*!< time zone */
    uint32_t ts;                                /*!< time stamp */
    uint16_t  port;                             /*!< The port number of remote service*/
}wifi_config_data_t;


typedef struct
{
    uint8_t channel;                        /*!< ota upgrade channel, 1: cloud, 2: APP*/
    char    otaVer[10];                        /*!< The ota firmware version*/
    char    otaType[30];                       /*!< The ota firmware type, such as module_ota_all or module_mcu_all*/
    char    download_url[200];                  /*!< The ota firmware download link */
    char    PubId[65];                         /*!< The publish ID from cloud*/
    char    md5[65];                           /*!< The ota firmware md5 data*/
}aiio_ota_msg_t;


typedef struct
{
    char userName[65];
    char userPasswd[65];
    char deviceId[65];
}aiio_device_info;

typedef struct
{
    char* queue_data;
    uint16_t queue_data_len;
    aiio_common_event_t common_event;
}aiio_rev_queue_t;


extern QueueHandle_t    cloud_rev_queue_handle;        /*!< The receive queue data handle*/
extern bool ble_config_start;                       /*!< The launch status in ble distribution network*/
extern bool wifi_config_start;
extern bool wifi_connect;
extern aiio_device_info DeviceInfo;
extern bool device_init;





int aiio_flash_get_device_info(aiio_device_info* device_info);
int aiio_flash_clear_device_info(aiio_device_info* device_info);
int aiio_flash_save_device_info(aiio_device_info* device_info);

int aiio_flash_get_wifi_config_data(wifi_config_data_t* config_data);
void aiio_flash_save_wifi_config_data(wifi_config_data_t* config_data);
void aiio_flash_clear_config_data(void);

int aiio_flash_get_ota_info(aiio_ota_msg_t* ota_info);
int aiio_flash_clear_ota_info(aiio_ota_msg_t* ota_info);
int aiio_flash_save_ota_info(aiio_ota_msg_t* ota_info);

#endif