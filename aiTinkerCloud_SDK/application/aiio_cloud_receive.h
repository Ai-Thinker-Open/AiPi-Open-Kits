/**
 * @brief   Decalre some interface about cloud, such as Lauch function of cloud or some callback function
 * 
 * @file    aiio_cloud_receive.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly desctrbing cloud interface
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The interface about cloud
 */
#ifndef __AIIO_CLOUD_RECEIVE_H_
#define __AIIO_CLOUD_RECEIVE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"



/**
 * @brief   The configuration structure of cloud information
 */
typedef struct 
{
    char *mqtt_host;                /*!< The ip address or doman of  remote service*/
    char *device_token;             /*!< Activaty information，it is given from cloud by app*/
    uint16_t mqtt_port;             /*!< The port number of remote service*/

    aiio_wifi_info_t *wifi_info;        /*!< The information of wifi*/
}aiio_cloud_receive_t;




/**
 * @brief   This function is for startting the cloud communication function
 * 
 * @note    This function have to be called for connecting cloud when device already connection the wifi
 * 
 * @param[in]   activity            The state of device activation
 * @param[in]   cloud_data          The configuration structure of cloud information
 * 
 */
void aiio_user_service_init(bool activity, aiio_cloud_receive_t *cloud_data);


/**
 * @brief   关闭mqtt连接，释放内存资源
 * 
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * 
 * 
 */

/**
 * @brief   This function is for stopping the cloud communication function
 */
void aiio_user_service_deinit(void);

#endif
