/**
 * @brief   Define the interface of real-time time application function
 *
 * @file    aiio_cloud_rtc.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly describing real-time time function initialization and starting.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The interface of real-time time application function
 */
#include "aiio_cloud_rtc.h"
#include "aiio_common.h"
 // #include "device.h"

static aiio_os_thread_handle_t* aiio_rtc_time_thread = NULL;

void aiio_rtc_time_init(rtc_task rtc_cb)
{
    aiio_rtc_time_t time = {
        .year = 2023,
        .mon = 6,
        .day = 1,
        .hour = 13,
        .minute = 52,
        .second = 22,
    };

    aiio_os_thread_create(&aiio_rtc_time_thread, "rtc_time_task", rtc_cb, 2 * 1024, NULL, 10);
    aiio_rtc_init(time, 100);
}