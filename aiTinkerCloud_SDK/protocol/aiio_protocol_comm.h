/**
 * @brief   Declare some common element in this file
 * 
 * @file    aiio_protocol_comm.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Declare some common elements, including macro definitions, structures,etc. 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> some common element
 */
#ifndef __AIIO_PROTOCOL_COMM_H_
#define __AIIO_PROTOCOL_COMM_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define AIIO_DEVICE_IOT_VERSION           ("1.0.0")                     /*!< The protocol version of cloud*/


// The type of report
#define         DEVICE_REPORT_TYPE                      1               /**<  The type of device auto report*/   
#define         CLOUD_QUERY_TYPE                        2               /**<  The type of cloud request report*/
#define         CLOUD_ALL_QUERY_TYPE                    3               /**<  The type of the cloud requests all status reporting*/
#define         CLOUD_CONTROL_TYPE                      4               /**< The device status is reported after the cloud issues the control*/


// topic qos define
#define     DEVICE_TOPIC_CONTROL_QOS                    2
#define     DEVICE_TOPIC_CONTROL_ACK_QOS                1
#define     DEVICE_TOPIC_QUERY_QOS                      0
#define     DEVICE_TOPIC_QUERY_ACK_QOS                  0
#define     DEVICE_TOPIC_INFO_QOS                       0
#define     DEVICE_TOPIC_INFO_REPORT_QOS                0
#define     DEVICE_TOPIC_REPORT_QOS                     0
#define     DEVICE_TOPIC_UPGRADE_QOS                    1
#define     DEVICE_TOPIC_UPGRADE_ACK_QOS                0
#define     DEVICE_TOPIC_UPGRADE_REPORT_QOS             0
#define     DEVICE_TOPIC_ONLINE_QOS                     0
#define     DEVICE_TOPIC_CONFIG_QOS                     1
#define     DEVICE_TOPIC_CONFIG_ACK_QOS                 1
#define     DEVICE_TOPIC_UTC_QOS                        0
#define     DEVICE_TOPIC_UTC_ACK_QOS                    0

/**
 * @brief   The macro definitions of Lan protocol
 */
#define     LAN_PRO_VERSION                     1                   /**< The version of Lan protocol*/
#define     LAN_PRO_HEAD_LEN                    6                   /**< The length of Lan protocol head */
#define     LAN_PRO_CRC_LEN                     2                   /**< The crc length in Lan protocol */
#define     LAN_PRO_ENC_FLAG                    1                   /**< The LAN protocol encryption flag*/
#define     LAN_PRO_NO_ENC_FLAG                 0                   /**< The LAN protocol does not encrypt the flag*/
#define     LAN_PRO_WHOLE_DATA                  1                   /**< The LAN protocol whole package data flag*/

#define     LAN_PROTOCOL_MAX_DATA_LEN                       1000             /**< The maximum length of the Lan protocol Shard-package data*/
#define     AIIO_AES_CRC_128_ENTRYPT_KEY_LEN                (16)





/**
 * @brief   Global variable declaration in Lan protocol
 */
extern uint8_t  lan_pro_key[65];
extern uint8_t  pro_iv[65];


#define     PROTOCOL_STR_NS                             "ns"
#define     PROTOCOL_STR_NAME                           "name"
#define     PROTOCOL_STR_MID                            "mid"
#define     PROTOCOL_STR_TS                             "ts"
#define     PROTOCOL_STR_VER                            "ver"
#define     PROTOCOL_STR_GID                            "gid"
#define     PROTOCOL_STR_HEADER                         "header"
#define     PROTOCOL_STR_PAYLOAD                        "payload"
#define     PROTOCOL_STR_PARAM                          "param"
#define     PROTOCOL_STR_PROPS                          "props"
#define     PROTOCOL_STR_CONTROL                        "control"
#define     PROTOCOL_STR_DEVICE                         "device"
#define     PROTOCOL_STR_CODE                           "code"
#define     PROTOCOL_STR_REPORT_TIMER                   "reportTimer"
#define     PROTOCOL_STR_KEEPALIVE                      "keepalive"
#define     PROTOCOL_STR_TIMEOUT                        "timeout"
#define     PROTOCOL_STR_DATE                           "data"
#define     PROTOCOL_STR_CHANNEL                        "chanel"
#define     PROTOCOL_STR_POINT_VER                      "pointVer"
#define     PROTOCOL_STR_BASE_VER                       "baseVer"
#define     PROTOCOL_STR_MCU_BASE_VER                   "mcuBaseVer"
#define     PROTOCOL_STR_OTA_TYPE                       "otaType"
#define     PROTOCOL_STR_APP_URL                        "appUrl"
#define     PROTOCOL_STR_MCU_URL                        "mcuUrl"
#define     PROTOCOL_STR_PROGRESS                       "progress"
#define     PROTOCOL_STR_OTA_STATUS                     "otaState"
#define     PROTOCOL_STR_OTA_VER                        "otaVer"
#define     PROTOCOL_STR_PUBID                          "pubId"
#define     PROTOCOL_STR_FROM                           "from"
#define     PROTOCOL_STR_TOKEN                          "token"
#define     PROTOCOL_STR_DEVICE                         "device"
#define     PROTOCOL_STR_ONLINE_STATUS                  "onlineStatus"
#define     PROTOCOL_STR_OFFLINE                        "offline"
#define     PROTOCOL_STR_ONLINE                         "online"
#define     PROTOCOL_STR_MD5                      "md5"



#define     STR_NS_IOT_DEVICE_CONTROL                   "iot.device.control"
#define     STR_NS_IOT_DEVICE_QUERY                     "iot.device.query"
#define     STR_NS_IOT_DEVICE_REPORT                    "iot.device.report"
#define     STR_NS_IOT_DEVICE_CONFIG                    "iot.device.config"
#define     STR_NS_IOT_DEVICE_UPGRADE                   "iot.device.upgrade"
#define     STR_NS_IOT_DEVICE_INFO                      "iot.device.info"
#define     STR_NS_IOT_DEVICE_UTC                       "iot.device.utc"


#define     STR_NAME_RESTORE                            "restore"
#define     STR_NAME_REBOOT                             "reboot"
#define     STR_NAME_INFO                               "info"
#define     STR_NAME_OTA_INFO                           "otaInfo"
#define     STR_NAME_OTA_PROGRESS                       "otaProgress"
#define     STR_NAME_ONLINE                             "online"
#define     STR_NAME_CONFIG                             "config"
#define     STR_NAME_QUERY                              "query"
#define     STR_NAME_QUERY_ALL                          "queryAll"
#define     STR_NAME_REPORT                             "report"
#define     STR_NAME_CONTROL                            "control"
#define     STR_NAME_UTC                                "utc"



#define     DEVICE_INFO_STR_DEVICE_ID                   "deviceId"
#define     DEVICE_INFO_STR_SECRTKEY                    "secrtKey"
#define     DEVICE_INFO_STR_PRODUCT_KEY                 "productKey"
#define     DEVICE_INFO_STR_FW_VER                      "fwVer"
#define     DEVICE_INFO_STR_MCU_VER                     "mcuVer"
#define     DEVICE_INFO_STR_HW_VER                     "hwVer"
#define     DEVICE_INFO_STR_MEMFREE                    "memFree"
#define     DEVICE_INFO_STR_MAC                        "mac"

#define     WIFI_STR_SSID                               "ssid"
#define     WIFI_STR_BSSID                               "bssid"
#define     WIFI_STR_RSSI                                "rssi"
#define     WIFI_STR_LOCAL_IP                            "localIp"
#define     WIFI_STR_MASK                                "mask"
#define     WIFI_STR_GW                                  "gw"
#define     WIFI_STR_AP                                  "ap"
#define     WIFI_STR_NETIF                               "netif"

#define     TOPIC_STR_UTC                                   "utc"
#define     TOPIC_STR_ACK                                   "ack"
#define     TOPIC_STR_INFO                                  "info"
#define     TOPIC_STR_CONTROL                               "control"
#define     TOPIC_STR_QUERY                                 "query"
#define     TOPIC_STR_UPGRADE                               "upgrade"
#define     TOPIC_STR_CONFIG                                "config"
#define     TOPIC_STR_REPORT                                "report"
#define     TOPIC_STR_LOG                                   "log"
#define     TOPIC_STR_ONLINE                                "online"


#define   OTA_PROGRESS_DOWNLOADING              "Downloading"
#define   OTA_PROGRESS_INSTALLING               "Installing"


#define     DEIVCE_INFO_STR_DATA                            "data"

#define     BLE_DATA_STR_SSID                "ssid"
#define     BLE_DATA_STR_PASSWD              "password"
#define     BLE_DATA_STR_WIFICC              "wificc"
#define     BLE_DATA_STR_MQTTIP              "mqttip"
#define     BLE_DATA_STR_MQTTPORT            "mqttport"
#define     BLE_DATA_STR_TZ                  "tz"
#define     BLE_DATA_STR_TS                  "ts"
#define     BLE_DATA_STR_TOKEN               "token"
#define     BLE_DATA_STR_CODE               "code"


#define     AIIO_SERVICE_MAX_TOPIC_LEN                      (100)
#define     AIIO_SERVICE_MAX_TMP_LEN                        (100)
#define     AIIO_SERVICE_MAX_MSG_LEN                        (512)

/**
 * @brief   The IOT protocol header data structure
 */
typedef struct 
{
    char ns[33];
    char name[33];
    char mid[65];
    char gid[65];
    char ver[10];
    char from[10];
    uint32_t ts;
}aiio_head_protocol_t;

/**
 * @brief   Device information structure
 */
typedef struct 
{
    char *deviceid;
    char *productid;
    char *secrtkey;
    char *fwVer;
    char *mcuVer;
    char *devType;
    char *mac;
    char *token;
    uint32_t memfree;
}aiio_device_info_t;

/**
 * @brief   The WiFi information data structure
 */
typedef struct 
{
    char *ssid;
    char *bssid;
    char *ip;
    char *mask;
    char *gw;
    int32_t rssi;
}aiio_wifi_info_t;

/**
 * @brief   ota structure to report progress during OTA
 */
typedef struct 
{
    char *otaStatus;       /**< The OTA progress status*/
    char *OtaVer;          /**< OTA version to be updated*/
    char *PubId;            /**< The ID of the OTA information release*/
    int32_t code;           /**< The error code that needs to response to cloud during OTA progressing. all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    uint32_t rate;          /**< OTA download progress*/
}aiio_ota_info_t;




#define ARGV_CMDEQUAL(token, cmd)           (memcmp(token, cmd, strlen(cmd)) == 0)


#endif
