/**
 * @brief   Define the interface of real-time time
 * 
 * @file    aiio_rtc_timer.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Realize real-time time application function
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The interface of real-time function
 */
#include "aiio_rtc_timer.h"
#include "aiio_protocol_code.h"
#include "aiio_protocol_comm.h"
#include "aiio_log.h"
#include "aiio_ctime.h"
#include "aiio_stdio.h"





#define     TIME_ZONE_DIRECTION_EAST_FLAG                   (0)         /*!< The flag of east zone*/
#define     TIME_ZONE_DIRECTION_WEST_FLAG                   (1)         /*!< The flag of west zone*/

#define     PLATFORM_TIMER_PERIOD                           (100)       /*!< The default period of update real-time tick*/



static aiio_rtc_time_t local_time = {1970, 1, 1, 0, 0, 0, 0};
static uint32_t month_days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
static uint32_t milliscond = 0;
static uint8_t timeZone = 8;
static uint8_t zoneDirection = 0; 
static uint16_t LoopPeriod = 0;



int aiio_rtc_init(aiio_rtc_time_t time, uint16_t loop_period)
{
    local_time.second = time.second;
    local_time.minute = time.minute;
    local_time.hour = time.hour;
    local_time.day = time.day;
    local_time.mon = time.mon;
    local_time.year = time.year;

    LoopPeriod = loop_period;

    return AIIO_CODE_OK;
}


void aiio_rtc_deinit(void)
{
    local_time.week = 0;
    milliscond = 0;
    local_time.second = 0;
    local_time.minute = 0;
    local_time.hour = 0;
    local_time.day = 1;
    local_time.mon = 1;
    local_time.year = 1970;
}



int aiio_set_rtc_time(aiio_rtc_time_t *rtc_time)
{
    if(rtc_time == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }
    local_time.year = rtc_time->year; 
    local_time.mon = rtc_time->mon; 
    local_time.day = rtc_time->day; 
    local_time.hour = rtc_time->hour; 
    local_time.minute = rtc_time->minute; 
    local_time.second = rtc_time->second; 
    local_time.week = aiio_cTimeToWeek(rtc_time->year,rtc_time->mon,rtc_time->day);

    aiio_log_i("the Date : %d:%d:%d \r\n", local_time.year, local_time.mon, local_time.day);
    aiio_log_i("the time : %d:%d:%d \r\n", local_time.hour, local_time.minute, local_time.second);

    return AIIO_CODE_OK;
}


aiio_rtc_time_t *aiio_get_rtc_time(void)
{
    return &local_time;
}



static bool aiio_verify_local_time(aiio_rtc_time_t *local_time)
{
    uint8_t temp_day = 0;

    if (NULL == local_time)
    {
        aiio_log_e("param err\n");
        return false;
    }

    /* The days of month*/
    temp_day = DAYS_OF_THE_MONTH(local_time->year, local_time->mon);
    if ((local_time->year >= BASE_YEAR) && 
        ((local_time->mon >= 1)  && (local_time->mon <= 12)) && 
        ((local_time->day >= 1) && (local_time->day <= temp_day)) && 
        (local_time->hour <= 23) && (local_time->minute <= 59) && 
        (local_time->second <= 59))
    {
        return true;
    }

    aiio_log_e("verify err\n");
    return false;
}



bool aiio_online_update_local_time(const char *time_data, uint16_t time_data_len)
{
    char temp[24] = {0};
    aiio_rtc_time_t local_time = {0};
    uint8_t tmp = 0;

    if (time_data)
    {
        if ((strlen(time_data) == 20) && (time_data[10] == 'T') && (time_data[19] == 'Z'))
        {
            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 0, 4);
            local_time.year = aiio_atoi(temp);

            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 5, 2);
            local_time.mon = aiio_atoi(temp);

            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 8, 2);
            local_time.day = aiio_atoi(temp);

            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 11, 2);
            local_time.hour = aiio_atoi(temp);

            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 14, 2);
            local_time.minute = aiio_atoi(temp);

            aiio_memset(temp, 0, sizeof(temp));
            aiio_strncpy(temp, time_data + 17, 2);
            local_time.second = aiio_atoi(temp);

            aiio_log_i("the Date : %d:%d:%d \r\n", local_time.year, local_time.mon, local_time.day);
            aiio_log_i("the time : %d:%d:%d \r\n", local_time.hour, local_time.minute, local_time.second);

            if (aiio_verify_local_time(&local_time))
            {
                if(zoneDirection == TIME_ZONE_DIRECTION_WEST_FLAG)
                {
                    aiio_log_i("west time zone \r\n");
                    if(local_time.hour - timeZone >= 0)
                    {
                        local_time.hour -= timeZone;
                        aiio_log_i("hour = %d \r\n", local_time.hour);
                    }
                    else
                    {
                        local_time.hour += 24;
                        local_time.hour -= timeZone;
                        local_time.day -= 1;
                        aiio_log_i("hour = %d \r\n", local_time.hour);
                        aiio_log_i("day = %d \r\n", local_time.day);
                    }
                }
                else
                {
                    aiio_log_i("east time zone \r\n");
                    local_time.hour += timeZone;
                    if(local_time.hour == 24)            /* The hour of the local real-time time is becoming 24 when the utc time is receiving and the local real-time time need to be update
                                                            by the time zone, the hour of the local real-time time need to be reset 0, and then the days are accumulated */
                    {
                        local_time.hour = 0;
                        local_time.day += 1;
                    }
                }

                aiio_set_rtc_time(&local_time);
                return true;
            }
            else
            {
                aiio_log_e("time data verify err \r\n");
                return false;
            }
        }
        else
        {
            aiio_log_e("time data format is err \r\n");
        }
    }
    else
    {
        aiio_log_e("time data is NULL \r\n");
    }
    return false;
}



void aiio_update_rtc_time_with_zone(char *time_zone, uint16_t time_zone_len)
{
    char str_temp[5] = {0};
    
    aiio_rtc_time_t *local_time = NULL;

    if(time_zone == NULL)
    {
        aiio_log_e("param err \r\n");
        return;
    }

    local_time = aiio_get_rtc_time();

    aiio_strncpy(str_temp, &time_zone[3], 1);
    aiio_log_i("str_temp = %s \r\n", str_temp);
    if(aiio_strcmp(str_temp, "+") == 0)
    {
        aiio_memset(str_temp, 0, sizeof(str_temp));
        aiio_strncpy(str_temp, &time_zone[4], 2);
        aiio_log_i("str_temp = %s \r\n", str_temp);
        timeZone = aiio_atoi(str_temp);
        aiio_log_i("timeZone = %d \r\n", timeZone);
        zoneDirection = TIME_ZONE_DIRECTION_EAST_FLAG;
    }
    else
    {
        aiio_memset(str_temp, 0, sizeof(str_temp));
        aiio_strncpy(str_temp, &time_zone[4], 2);
        aiio_log_i("str_temp = %s \r\n", str_temp);
        timeZone = aiio_atoi(str_temp);
        aiio_log_i("timeZone = %d \r\n", timeZone);
        zoneDirection = TIME_ZONE_DIRECTION_WEST_FLAG;
    }

    aiio_set_rtc_time(local_time);
}


uint32_t aiio_get_current_time_stamp(void)
{
    TimeType timeData = {0};
    uint32_t  timestamp = 0;
    aiio_rtc_time_t *timer = NULL;

    timer = aiio_get_rtc_time();
    if(timer == NULL)
    {
        aiio_log_e("timer is NULL \r\n");
        return 0;
    }

    // aiio_log_i("the Date : %d:%d:%d \r\n", timer->year, timer->mon, timer->day);
    // aiio_log_i("the time : %d:%d:%d \r\n", timer->hour, timer->minute, timer->second);

    timeData.year = timer->year;
    timeData.month = timer->mon;
    timeData.day = timer->day;
    timeData.minute = timer->minute;
    timeData.second = timer->second;
    timeData.hour = timer->hour;
    timeData.cUTC = 8;

    timestamp = aiio_lTimeToStamp(timeData);
    aiio_log_i("timestamp = %ld \r\n", timestamp);


    return timestamp;
}


void aiio_rtc_loop(void)
{
    if (YEAR_LEAP(local_time.year) == 1)
    {
        month_days[2] = 29;
    }
    else
    {
        month_days[2] = 28;
    }

    if(LoopPeriod)
    {
        milliscond += LoopPeriod;
    }
    else
    {
        milliscond += PLATFORM_TIMER_PERIOD;
    }
    if(milliscond >= 1000)
    {
        milliscond = 0;
        local_time.second++;
        if (local_time.second >= 60)
        {
            local_time.second = 0;
            local_time.minute++; 
            if (local_time.minute >= 60)
            {
                local_time.minute = 0;
                local_time.hour++;
                if (local_time.hour >= 24)
                {
                    local_time.hour = 0;
                    local_time.day++;
                    if (local_time.day >= month_days[local_time.mon]) 
                    {
                        local_time.mon++;
                        if (local_time.mon >= 13)
                        {
                            local_time.mon = 0;
                            local_time.year++;
                        }
                    }
                }
            }
        }
    }

    if (local_time.second % 10 == 0 && milliscond == 0) 
    {
        local_time.week = aiio_cTimeToWeek(local_time.year,local_time.mon,local_time.day);
    }
}



