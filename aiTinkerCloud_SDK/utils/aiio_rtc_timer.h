/**
 * @brief   Declare the interface of real-time time
 * 
 * @file    aiio_rtc_timer.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Realize real-time time application function
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The interface of real-time function
 */
#ifndef __AIIO_RTC_TIMER_H_
#define __AIIO_RTC_TIMER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



/**
 * @brief   The struction of real-time time
 */
typedef struct
{
    uint16_t year;              
    uint8_t mon;               
    uint8_t day;                
    uint8_t hour;              
    uint8_t minute;            
    uint8_t second;          
    uint8_t week;               /**< From sunday to saturday, the value is 0，1，2，3，4，5，6 */
}aiio_rtc_time_t;



/**
 * @brief   Initialize real-time time
 * 
 * @param[in]   time                Initialize default time
 * @param[in]   loop_period         The paramter is the period of setting the loop call aiio_rtc_loop. The time will update the tick time for the loop_period
 * 
 * @return  int                     Reture valus is AIIO_CODE_OK
 */
int aiio_rtc_init(aiio_rtc_time_t time, uint16_t loop_period);


/**
 * @brief   Deinitialize time
 * 
 * 
 */
void aiio_rtc_deinit(void);

/**
 * @brief   Update real-time time
 * 
 * @param[in]   rtc_time     real-time time to update
 * 
 * @return  int                     Reture valus is AIIO_CODE_OK
 */
int aiio_set_rtc_time(aiio_rtc_time_t *rtc_time);

/**
 * @brief   Get real-time time
 * 
 * 
 * @return  aiio_rtc_time_t*  Reture the pointer of real-time time data
 */
aiio_rtc_time_t *aiio_get_rtc_time(void);



/**
 * @brief   Update local real-time time from online time
 * 
 * @param[in]   time_data       The real-time data from online time
 * @param[in]   time_data_len    The length of the real-time data from online time
 * 
 * @return  true            Update local real-time time is success
 * @return  false           Update local real-time time is fail
 */
bool aiio_online_update_local_time(const char *time_data, uint16_t time_data_len);


/**
 * @brief   Update real-time time by time zone
 * 
 * @param[in]   time_zone           time zone
 * @param[in]   time_zone_len       the length of time zone
 * 
 */
void aiio_update_rtc_time_with_zone(char *time_zone, uint16_t time_zone_len);


/**
 * @brief   Get current time stamp
 * 
 * 
 * @return  uint32_t   Reture current time stamp data
 */
uint32_t aiio_get_current_time_stamp(void);



/**
 * @brief   Update tick time by looping call this function, and the real-time time update on time
 * @note    recommend the thread task or hard timer to loop call this function. and recommend setting period is 100 that loop call this function.
 * 
 */
void aiio_rtc_loop(void);


#endif
