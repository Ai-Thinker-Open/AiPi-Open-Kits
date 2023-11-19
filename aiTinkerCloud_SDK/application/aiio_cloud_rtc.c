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
#include "device.h"

static aiio_os_thread_handle_t* aiio_rtc_time_thread = NULL;


static void rtc_timer_task(void* pvParameters)
{
    aiio_rtc_time_t* rtc_time = NULL;
    uint32_t count = 0;

    while (1)
    {
        aiio_rtc_loop();

        count++;
        if (count > 30)
        {
            rtc_time = aiio_get_rtc_time();
            r2_device->date.year = rtc_time->year;
            r2_device->date.day = rtc_time->day;
            r2_device->date.month = rtc_time->mon;
            r2_device->date.hour = rtc_time->hour;
            r2_device->date.min = rtc_time->minute;
            r2_device->date.sec = rtc_time->second;
            r2_device->date.wday = rtc_time->week;
            aiio_log_i("the Date : %02d:%02d:%02d - %02d:%02d:%02d,wday %02d", r2_device->date.year, r2_device->date.month, r2_device->date.day, r2_device->date.hour, r2_device->date.min, r2_device->date.sec, r2_device->date.wday);
            yesr = r2_device->date.year;
            mon = r2_device->date.month;
            day = r2_device->date.day;
            wday = r2_device->date.wday;
            hour = r2_device->date.hour;
            min = r2_device->date.min;
            sec = r2_device->date.sec;
            count = 0;

        }

        aiio_os_tick_dealy(aiio_os_ms2tick(100));
    }
}


void aiio_rtc_time_init(void)
{
    aiio_rtc_time_t time = {
        .year = 2023,
        .mon = 6,
        .day = 1,
        .hour = 13,
        .minute = 52,
        .second = 22,
    };

    aiio_os_thread_create(&aiio_rtc_time_thread, "rtc_time_task", rtc_timer_task, 2 * 1024, NULL, 10);
    aiio_rtc_init(time, 100);
}