/**
 * @brief   Declare the application interface of ble distribution network
 * 
 * @file    aiio_ble_config.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Achieve logic function of ble distribution network by the protocol interface of it
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> The application interface of ble distribute network
 */
#ifndef __AIIO_BLE_CONFIG_H_
#define __AIIO_BLE_CONFIG_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"
#include "aiio_ble_protocol.h"
#include "aiio_protocol_code.h"


#define     AIIO_IOT_BLE_CONFIG_TASK_PRIORITY                           (5)                     /*!< The default task priority in ble distribution network*/
#define     AIIO_IOT_BLE_CONFIG_TASK_STACK                              (4 * 1024)              /*!< The default task stack in ble distribution network*/



/**
 * @brief   The event of ble distribute network, it will inform ble distribution network state by callbacked those event in aiio_ble_config_iot_event_t struction
 */
typedef enum
{
    AIIO_BLE_CONFIG_FAIL,
    AIIO_BLE_DISCONNECED,
    AIIO_BLE_CONFIG_OK
}aiio_ble_config_iot_event_t;


/**
 * @brief   The information of the ble distribution network, it will transfer the ble distribution network data through the callback interface.
 */
typedef struct 
{
    char *entrypt_key;
    aiio_ble_data_t  *ble_data;
    aiio_ble_config_iot_event_t event;
}aiio_ble_iot_data_t;


/**
 * @brief   The configuration information of ble distribution network, it must be configured before ble distribution network starting 
 */
typedef struct 
{
    char *deviceid;
    char *productid;
    char *wifi_flag;    
    int  task_prio;                                                       /*!< The task priority in ble distribution network, it will use default the task priority when the element is not set*/
    int  task_stack;                                                      /*!< The task stack in ble distribution network, it will use default the task stack when the element is not set*/
    void (*aiio_ble_iot_config_cb)(aiio_ble_iot_data_t *ble_data);
}aiio_ble_iot_config_t;




/**
 * @brief   Initialize the ble protocol stack and start the ble broadcast
 * 
 * @param[in]   ble_congfig                 The configuration information of ble distribution network
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_ble_config_init(aiio_ble_iot_config_t *ble_congfig);



/**
 * @brief   Deinitialize ble protocol stack, disconnect ble connection, turn off ble broadcast and release memory resources
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_ble_config_deinit(void);

/**
 * @brief   Report the status of ble distribution network to app by this function when device is in ble distribution network
 * @note    The device needs to report those status that wifi connection, mqtt connection, report activaty to cloud and report online to cloud， which by this function.
 *          And device also must to report some error status when  the ble distribution network stops by abnormity error.
 * 
 * @param[in]   error_code  The error code that needs to response to app. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_ble_config_response_status(int8_t error_code);



#endif
