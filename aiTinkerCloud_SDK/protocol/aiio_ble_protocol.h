/**
 * @brief   Declare the packet protocol interface of ble distribution network, and the parse protocol of it. 
 * 
 * @file    aiio_ble_protocol.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly for describing the ble distribution network protocol, for defining packet protocol interface and parse protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Describe the protocol interface about the ble distribution network
 */
#ifndef __AIIO_BLE_PROTOCOL_H_
#define __AIIO_BLE_PROTOCOL_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"


#define AIIO_BLE_PROCOTOL_VERSION             1           /**< the version of ble distribution network protocol */

#define AIIO_BLE_PROTOCOL_PACKET_MAX_LEN                    (AIIO_SERVICE_MAX_MSG_LEN)

/**
 * @brief The encryption type of the ble distribution network communication
*/
typedef enum 
{
    PROTOCOL_ENTRYPT_NONE = 0,              /*!< The type that is not encrypted*/
    PROTOCOL_ENTRYPT_AES_CBC_128,            /*!< The type that is using the AES CBC 128*/    
    PROTOCOL_ENTRYPT_UNKNOW = 0xff          /*!< The type that is unknow */
}aiio_entrypt_type_t;


/**
 * @brief   The commnunication type of the ble distribution network. 
 */
typedef enum 
{
    PROTOCOL_DIVIDE_TYPE_NONE = 0,                  /*!< The type that is using the whole package of data communication*/
    PROTOCOL_DIVIDE_TYPE_DIVIDE_DATA                /*!< The type that is using the Shard-package of data communication */
}aiio_divide_type_t;



/**
 * @brief   The information of ble distribution network structure
 */
typedef struct 
{
    char *ssid;                              /*!< ssid of rout */
    char *passwd;                             /*!< passwd of rout*/
    char *wificc;                             /*!< country code */
    char *mqttip;                             /*!< ip address or domain name */
    char *token;                             /*!< activaty information，it will be given from cloud by app */
    char *tz;                                /*!< time zone */
    uint32_t ts;                                /*!< time stamp */
    uint16_t  port;                          /*!< port number of remote service */
}aiio_ble_data_t;



/**
 * @brief   Parse protocol the event of the ble distribution network 
 */
typedef enum
{
    AIIO_BLE_DATA_CRC_ERR_EVENT = 0,                        /*!< It will callback this event when an error during CRC verification on the data from the APP, it needs to packet protocol data that include crc error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function , and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_PROTOCOL_ERR_EVENT,                        /*!< It will callback this event when an error during the protocol format verification on the data from the APP, it needs to packet protocol data that include the protocol format error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_PROTOCOL_VER_ERR_EVENT,                    /*!< It will callback this event when an error during protocol version verification on the data from the APP, it needs to packet protocol data that include protocol version error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_HARD_ERR_EVENT,                           /*!< It will callback this event when device has a hardware type error during parse protocol on the data from the APP, it needs to packet protocol data that include hardware type error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT,                /*!< It will callback this event when an error during encryption type verification on the data from the APP, it needs to packet protocol data that include encryption type error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_FRAME_ERR_EVENT,                        /*!< It will callback this event when an error during frame number verification on the data from the APP, it needs to packet protocol data that include frame number error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_VERIFICATE_OK,                            /*!< It will callback this event when the data from APP verificate successfully. it needs to packet protocol data that include data verification success status by this function 
                                                                 that is named aiio_PacketBleResponseDivideAckData(), and then send to this protocol data for APP by call other function ,and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_PARSE_FAIL_EVENT,                        /*!< It will callback this event when the json parse fail on data from the APP, it needs to packet protocol data that include json parse fail error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideData(),and then send to this protocol data for APP by call other function , and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
    AIIO_BLE_DATA_PARSE_OK_EVENT                            /*!< It will callback this event when the json data parse successfully on data from APP, it needs to packet protocol data that include json parsing success error status by this function 
                                                                 that is named aiio_PacketBleResponseDivideData(),and then send to this protocol data for APP by call other function , and all kind of error code that is defined in this file that is called aiio_protocol_code.h*/
}aiio_ble_data_parse_event_t;


/**
 * @brief   The protocol event of the ble distribution network
 * @note     Parse protocol data in aiio_ble_protocol_event_t structure by the callback function that is named aiio_ParseBleProtocolData(), 
 */
typedef struct 
{
    aiio_ble_data_t                 *ble_data;                  /*!< The information of ble distribution network structure*/
    aiio_ble_data_parse_event_t     event;                      /*!< Parse protocol the event of the ble distribution network */
    aiio_entrypt_type_t             entrypt_type;               /*!< The encryption type of the ble distribution network communication*/
    uint8_t                         frame;                      /*!< The frame number of the ble distribution network communication*/
    char                            *entrypt_key;               /*!< The encryption key of the ble distribution network communication*/
}aiio_ble_protocol_event_t;


typedef void (*aiio_ble_data_parse_cb)(aiio_ble_protocol_event_t *event);


/**
 * @brief   Pack the error code protocol packets in response to the APP, this function is mainly response json data to APP.
 * @note    This function only pack the whole package data, and this function needs to package the data into encrypted data by the paramter entrypt_type.
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged ack data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged ack data
 * @param[in]   frame                   frame number
 * @param[in]   entrypt_type            The encrypt type of the package ack data
 * @param[in]   entrypt_key             encryption key
 * @param[in]   deviceid                device id
 * @param[in]   error_code              The error code that needs to response to app. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture the length of the packaged data or all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_PacketBleResponseDivideData(char *packet_data, uint32_t packet_data_len, uint8_t frame, aiio_entrypt_type_t entrypt_type, char *entrypt_key, char *deviceid, int8_t error_code);


/**
 * @brief   Pack the error code protocol packets in response to the APP when device is received data from app every time. this function is mainly response simply ack data by ack protocol format.
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged ack data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged ack data
 * @param[in]   frame                   frame number
 * @param[in]   error_code              The error code that needs to response to app. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture the length of the packaged data or all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_PacketBleResponseDivideAckData(char *packet_data, uint32_t packet_data_len, uint8_t frame, int8_t error_code);

/**
 * @brief   Parse package data of the ble distribution network
 * 
 * @param[in]   data                    The source data of the ble distribution network
 * @param[in]   data_len                The length of the source data of the ble distribution network
 * @param[in]   data_parse_cb           The callback function, the callback function is called by this function after the source data is parse.
 * 
 * @return  int  Reture the length of the packaged data or all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_ParseBleProtocolData(uint8_t *data, uint16_t data_len, aiio_ble_data_parse_cb data_parse_cb);



#endif
