/**
 * @brief   Decalre parsing protocol interface of cloud communication
 * 
 * @file    aiio_protocol_packet.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is decribing about the protocol of cloud communication, mainly declare parsing protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Parsing protocol interface of cloud communication
 */
#ifndef __AIIO_PROTOCOL_PARSE_H_
#define __AIIO_PROTOCOL_PARSE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"


typedef void (*aiio_protocol_payload_cb)(aiio_head_protocol_t *head, char *payload, uint32_t payload_len);
typedef void (*aiio_utc_cb)(char *time);



/**
 * @brief   Parse the json data issued by the cloud or the LAN
 * 
 * @param[in]   data                The protocol data in json string format
 * @param[in]   data_len            The length of the protocol data in json string format
 * @param[in]   payload_cb          The callback function, it will transfers the parsed protocol header data and the payload data to the device
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_JsonDataParse(char *data, uint16_t data_len, aiio_protocol_payload_cb payload_cb);

/**
 * @brief   Parsing the utc time data in the json string format
 * 
 * @param[in]   data                The protocol data in json string format
 * @param[in]   time_buff           The buffer space that receives the time data
 * @param[in]   time_buff_len       The length of the buffer space that receives the time data
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_ParseTimeData(char *data, char *time_buff, uint32_t time_buff_len);

#endif
