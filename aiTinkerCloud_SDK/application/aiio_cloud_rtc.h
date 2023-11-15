/**
 * @brief   Declare the interface of real-time time application function
 * 
 * @file    aiio_cloud_rtc.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly describing real-time time function initialization and starting.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The interface of real-time time application function
 */
#ifndef __AIIO_CLOUD_RTC_H_
#define __AIIO_CLOUD_RTC_H_


/**
 * @brief   The default time for initializing real-time time, When device is online, device needs to call this function that is named aiio_online_update_local_time() to update the local real-time time.
 * @note    1. This function hanve to be called, before starting the ble distribution network function or starting cloud commnunication function.
 *          2. When this function have been called, device needs to be called aiio_update_rtc_time_with_zone() for update time zone when ble distribution network is going.
 * 
 */
void aiio_rtc_time_init(void);

#endif
