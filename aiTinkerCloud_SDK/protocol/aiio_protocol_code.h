/**
 * @brief   Declare some common error code in IOT protocol
 * 
 * @file    aiio_protocol_code.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Those error code is defined in this file that is include ble distribution network error code, ap distribution network error code, ota error code and some common error code.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Some common error code in IOT protocol
 */
#ifndef __AIIO_PROTOCOL_CODE_H_
#define __AIIO_PROTOCOL_CODE_H_


/**
 * @brief  some common error code
 */
#define AIIO_CODE_OK                    (0)
#define AIIO_NS_ERROR                   (-1)
#define AIIO_NAME_ERROR                 (-2)
#define AIIO_PROP_ERROR                 (-3)
#define AIIO_PROTOCOL_ERROR             (-4)
#define AIIO_PARAM_ERR                  (-5)
#define AIIO_CREATE_JSON_FAIL           (-6)
#define AIIO_JSON_PARSE_FAIL            (-7)
#define AIIO_MQTT_CONFIG_FAIL           (-8)
#define AIIO_MQTT_START_FAIL            (-9)
#define AIIO_MQTT_MUL_INIT              (-10)
#define AIIO_MQTT_MUL_DEINIT            (-11)
#define AIIO_MALLOC_FAIL                (-12)
#define AIIO_CLOUD_INIT_FAIL            (-13)
#define AIIO_CLOUD_NOT_INIT             (-14)
#define AIIO_OVERFLOW_ERR               (-15)
#define AIIO_SOCKET_CREATE_FAIL         (-16)
#define AIIO_SOCKET_BIND_FAIL           (-17)
#define AIIO_TASK_CREATE_FAIL           (-18)
#define AIIO_UDP_SEND_FAIL              (-19)

/**
 * @brief   AP distribution network error code
 */
#define AIIO_AP_REV_OK                  (1)
#define AIIO_AP_FAIL                    (3)

/**
 * @brief   It needs to response these error code when device is handling these status include wifi connection, mqtt connection, activation devices,etc.  
 * @note    these error code needs to response APP by aiio_PacketBleResponseDivideData().
 */
#define AIIO_BLE_CODE_OK                      0           /**<  The BLE distribution network is successful*/
#define AIIO_BLE_CODE_DATA_REV_OK             1           /**<  Distribution network data is already received*/
#define AIIO_BLE_CODE_WIFI_CONN_OK             2           /**< The device is already connected to the WiFi */
#define AIIO_BLE_CODE_FAIL                    3           /**< The BLE distribution network has failed*/
#define AIIO_BLE_CODE_WIFI_PAWD_ERR           4           /**<  WIFI wrong password*/
#define AIIO_BLE_CODE_WIFI_NO_SSID            5           /**< The ssid for WiFi was not found*/
#define AIIO_BLE_CODE_WIFI_CONNING            6           /**<  Wifi is connecting*/
#define AIIO_BLE_CODE_WIFI_CONN_FAIL          7           /**< WIFI connection fail*/
#define AIIO_BLE_CODE_HARD_ERR                8           /**< hardware error */
#define AIIO_BLE_CODE_PRO_ERR                 9           /**< protocol error*/
#define AIIO_BLE_CODE_PRO_VER_ERR             10          /**< protocol version error*/
#define AIIO_BLE_CODE_MQTT_CONNING            11          /**< mqtt is connectting */
#define AIIO_BLE_CODE_MQTT_CONN_ERR           12          /**< mqtt connnected fail */
#define AIIO_BLE_CODE_MQTT_CONN_OK            13          /**< The device is already connected to the mqtt*/
#define AIIO_BLE_CODE_MQTT_PRO_ERR            14          /**< mqtt protocol error*/
#define AIIO_BLE_CODE_MQTT_PRO_VER_ERR        15          /**< mqtt protocol version error */
#define AIIO_BLE_CODE_LAN_PRO_ERR             16          /**< lan network protocol error*/
#define AIIO_BLE_CODE_LAN_PRO_VER_ERR         17          /**< lan network protocol version error*/
#define AIIO_BLE_CODE_LAN_PRO_CRC_ERR         18          /**< The LAN data CRC verification has faileda */
#define AIIO_BLE_CODE_UTC_REQ                 19          /**< Send a request for the utc data*/
#define AIIO_BLE_CODE_UTC_TIMEOUT             20          /**< The UTC request has timed out*/
#define AIIO_BLE_CODE_ONLINE_REQ              21          /**< Send the device online status to the cloud*/
#define AIIO_BLE_CODE_ACTIVITY_FAIL           22          /**< Device activation failed*/
#define AIIO_BLE_CODE_ACTIVITY_REQ            23          /**< Send device activation messages to the cloud*/


/**
 * @brief   It needs to response these error status when each time the data is received.
 * @note    these error code needs to response APP by aiio_PacketBleResponseDivideAckData().
 */
#define AIIO_BLE_ACK_OK                             0           /**< Received correct data */
#define AIIO_BLE_ACK_CRC_ERR                        1           /**< Data crc check error*/
#define AIIO_BLE_ACK_TYPE_ERR                       2           /**<  Protocol type error*/
#define AIIO_BLE_ACK_VER_ERR                        3           /**< Protocol version error */
#define AIIO_BLE_ACK_FRAME_ERR                      4           /**< Frame number error */
#define AIIO_BLE_ACK_HARD_ERR                       5           /**< Hardware error */

/**
 * @brief   OTA error code
 */
#define AIIO_OTA_SUCCESS                (0)
#define AIIO_OTA_DOWNLOAD_FAIL          (1)
#define AIIO_OTA_INSTALL_FAIL           (2)
#define AIIO_OTA_PROTOCOL_DATA_ERR      (3)
#define AIIO_OTA_DATA_ERR               (4)
#define AIIO_OTA_DEVICE_ERR             (5)
#define AIIO_OTA_VER_ERR                (6)
#define AIIO_OTA_LAUCHED                (7)




#endif