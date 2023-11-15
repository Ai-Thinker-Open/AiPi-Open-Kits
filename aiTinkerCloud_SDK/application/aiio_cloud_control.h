/**
 * @brief   Decalre the cloud  control the attribute state interface
 * 
 * @file    aiio_cloud_control.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note    This is mainly destribing the attribute state that is received from cloud, And the different type is come from different command interface
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The  different command interface of control attribute state.
 */
#ifndef __AIIO_CLOUD_CONTROL_H_
#define __AIIO_CLOUD_CONTROL_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cJSON.h"


/**
 * @brief   Parsing control command that is from APP or cloud
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   control_data     control command data
 * 
 */
void aiio_parse_control_data(char *msgMid, char *from, char *control_data);


/**
 * @brief   Report the status of  bool type command
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   dpid                The number of attribute
 * @param[in]   status                 The state of command
 * 
 */
void aiio_report_bool_attibute_status(char *msgMid, char *from, uint8_t dpid, bool status);

/**
 * @brief   Report the value of  string type command
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   dpid            The number of attribute
 * @param[in]   value           The value of command
 * 
 */
void aiio_report_string_attibute_status(char *msgMid, char *from, uint8_t dpid, char* value);

/**
 * @brief   Report the value of  int type command
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   dpid            The number of attribute
 * @param[in]   value           The value of command
 * 
 */
void aiio_report_int_attibute_status(char *msgMid, char *from, uint8_t dpid, int value);

/**
 * @brief   Report the value of  all type command
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * 
 */
void aiio_report_all_attibute_status(char *msgMid, char *from);


#endif

