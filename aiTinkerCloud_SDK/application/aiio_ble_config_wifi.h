/**
 * @brief   Apply BLE distribution network protocol to define some interfaces applying BLE distribution network function
 * 
 * @file    aiio_ble_config_wifi.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly describing applying ble distribution network function
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The interface of applying ble distribution network function
 */
#ifndef __AIIO_BLE_CONFIG_WIFI_H_
#define __AIIO_BLE_CONFIG_WIFI_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"






/**
 * @brief   Launch ble distribution network function, such as starting ble protocol stack function and starting ble broadcast
 */
void aiio_ble_config_start(void);

/**
 * @brief   Stop ble protocol, and release relevant memory resource
 */
void aiio_ble_config_stop(void);



#endif
