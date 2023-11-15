/**
 * @brief   The interface of real time, inclue week, time stamp etc
 * 
 * @file    aiio_ctime.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note       define real time, week, month and time stamp interface
 * 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-13          <td>1.0.0            <td>zhuolm             <td> define the interface about the time and day.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aiio_ctime.h"


const static uint8_t st_ucMonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



uint8_t aiio_GetMonthDays(uint8_t month)
{
    return st_ucMonthDays[month - 1];
}

uint8_t aiio_cTimeToWeek(int32_t iYear, uint8_t ucMonth, uint8_t ucDay)
{
    int32_t iCentury = 0;
    int8_t cWeek = 0;


    /* 如果是1、2月份，则需要把月份当作是上年度的13、14月份 */
    if(ucMonth <= 2)
    {
        iYear -= 1;
        ucMonth += 12;
    }

    iCentury = iYear / 100;
    iYear = iYear % 100;

    cWeek = ((iCentury / 4) - (iCentury * 2) + iYear + (iYear / 4) + (13 * (ucMonth + 1) / 5) + ucDay - 1) % 7;
    cWeek = (cWeek < 0) ? (cWeek + 7) : cWeek;

    return (uint8_t)cWeek;
}


void aiio_vStampToTime(uint32_t lStamp, TimeType *ptypeTime, int8_t cUTC)
{
    uint32_t lDaysNumber = 0;
    int32_t lYearTemp = 1970;
    uint8_t ucMonthTemp = 1;

    /* 加入时区值 */
    lStamp += cUTC * 3600;
    ptypeTime->cUTC = cUTC;

	/* 减去全部年份的整年天数（1天有86400秒） */
    for(lDaysNumber = lStamp / 86400; lDaysNumber > 0; ++lYearTemp)
        lDaysNumber -= DAYS_OF_THE_YEAR(lYearTemp);

	/* 上面循环多计算了1年 */
    ptypeTime->year = lYearTemp - 1;

	/* 加上上面循环多减去的1年，减去当前年份下全部月份的整月天数 */
    for(lDaysNumber += DAYS_OF_THE_YEAR(ptypeTime->year); lDaysNumber > 0; ++ucMonthTemp)
        lDaysNumber -= DAYS_OF_THE_MONTH(ptypeTime->year, ucMonthTemp);

	/* 上面循环多计算了1月 */
    ptypeTime->month = ucMonthTemp - 1;

	/* 加上上面循环多减去的1月 */
    lDaysNumber += DAYS_OF_THE_MONTH(ptypeTime->year, ptypeTime->month);

	/* 天从1开始计数 */
    ptypeTime->day = lDaysNumber + 1;

    ptypeTime->hour = lStamp / 3600 % 24;

    ptypeTime->minute = lStamp / 60 % 60;

    ptypeTime->second = lStamp % 60;

	ptypeTime->week = aiio_cTimeToWeek(ptypeTime->year, ptypeTime->month, ptypeTime->day);
}


uint32_t aiio_lTimeToStamp(TimeType typeTime)
{
    uint32_t lDaysNumber = 0, lStamp = 0;
    int32_t lYearTemp = 1970;
    uint8_t ucMonthTemp = 1;

    for(lYearTemp = 1970; lYearTemp < typeTime.year; ++lYearTemp)
        lDaysNumber += DAYS_OF_THE_YEAR(lYearTemp);

    for(ucMonthTemp = 1; ucMonthTemp < typeTime.month; ++ucMonthTemp)
        lDaysNumber += DAYS_OF_THE_MONTH(typeTime.year, ucMonthTemp);

    lDaysNumber += typeTime.day - 1;

    lStamp = lDaysNumber * 86400;
    lStamp += typeTime.hour * 3600;
    lStamp += typeTime.minute * 60;
    lStamp += typeTime.second;

    /* 加入计算时区值 */
    lStamp -= typeTime.cUTC * 3600;

    return lStamp;
}


char *aiio_pcStampToTimeStrings(uint32_t lStamp, int8_t cUTC)
{
    TimeType typeTime = {0};
    static char cTimeStrings[16] = {0};

    /* 东8区 */
    aiio_vStampToTime(lStamp, &typeTime, cUTC);

    memset(cTimeStrings, 0, sizeof(cTimeStrings));
    sprintf(cTimeStrings, "%02d:%02d:%02d", typeTime.hour, typeTime.minute, typeTime.second);

    return cTimeStrings;
}


char *aiio_pcStampToDateStrings(uint32_t lStamp , int8_t cUTC)
{
    TimeType typeTime = {0};
    static char cDateStrings[16] = {0};

    /* 东8区 */
    aiio_vStampToTime(lStamp, &typeTime, cUTC);

    memset(cDateStrings, 0, sizeof(cDateStrings));
    sprintf(cDateStrings, "%d/%02d/%02d", (int)typeTime.year, typeTime.month, typeTime.day);

    return cDateStrings;
}
