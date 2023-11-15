/**
 * @brief   The interface of real time, inclue week, time stamp etc
 * 
 * @file    aiio_ctime.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note       Declare real time, week, month and time stamp interface
 * 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-13          <td>1.0.0            <td>zhuolm             <td> define the interface about the time and day.
 */
#ifndef __AIIO_CTIME_H_
#define __AIIO_CTIME_H_
#include <stdint.h>



/* leap year declare */
#ifndef AIIO_LEAP_YEAR
#define AIIO_LEAP_YEAR   1
#endif // LEAP_YEAR

/* common year declare */
#ifndef AIIO_COMMON_YEAR
#define AIIO_COMMON_YEAR 0
#endif // COMMON_YEAR


#define   BASE_YEAR             (2006)


/*Determine whether it is a leap year */
#define YEAR_LEAP(year) (((((year) % 4 == 0) && ((year) % 100 != 0)) || (((year) % 400 == 0) && ((year) % 3200 != 0)) || ((year) % 172800 == 0)) ? AIIO_LEAP_YEAR : AIIO_COMMON_YEAR)
/* Get the number of the year */
#define DAYS_OF_THE_YEAR(year) (YEAR_LEAP(year) == AIIO_LEAP_YEAR ? 366 : 365)
/* Get the number of the month */
#define DAYS_OF_THE_MONTH(year, month) ((((month) == 2) && (YEAR_LEAP(year) == AIIO_LEAP_YEAR)) ? 29 : aiio_GetMonthDays(month))


/**
 * @brief   Declare Time structure
 */
typedef struct
{
    int32_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
	uint8_t week;
    int8_t cUTC;
}TimeType;



/** @brief Get the number of weeks by year, month and date
 *
 *  @param[in]      iYear            The parameter of year
 *  @param[in]      ucMonth          The parameter of month
 *  @param[in]      ucDay            The parameter of date
 *  @return                          Return the number of the weeks. When the return value is more than 0, it is getting ok, otherwise is fail.
 */
uint8_t aiio_cTimeToWeek(int32_t iYear, uint8_t ucMonth, uint8_t ucDay);


/** @brief Get the time by time stamp
 *
 *  @param[in]      lStamp             The parameter of time stamp
 *  @param[in]      ptypeTime          The parameter of time structure
 *  @param[in]      cUTC               The parameter of time zone
 */
void aiio_vStampToTime(uint32_t lStamp, TimeType *ptypeTime, int8_t cUTC);

/** @brief Get time stamp by time
 *
 *  @param[in]      typeTime            The parameter of time structure
 *  @return                          Return the time stamp. When the return value is more than 0, it is getting ok, otherwise is fail.
 */
uint32_t aiio_lTimeToStamp(TimeType typeTime);



/** @brief Get the time data in the string format by time stamp
 *
 *  @param[in]      lStamp             The parameter of time stamp
 *  @param[in]      cUTC               The parameter of time zone
 *  @return                          Return the time data in string format or NULL.
 */
char *aiio_pcStampToTimeStrings(uint32_t lStamp, int8_t cUTC);


/** @brief Get the date data in the string format by time stamp
 *
 *  @param[in]      lStamp             The parameter of time stamp
 *  @param[in]      cUTC               The parameter of time zone
 *  @return                          Return the date data in string format or NULL.
 */
char *aiio_pcStampToDateStrings(uint32_t lStamp , int8_t cUTC);


/** @brief Get the days by the mouth
 *
 *  @param[in]      month             The parameter of the mouth
 *  
 *  @return                          Return the days or 0.
 */
uint8_t aiio_GetMonthDays(uint8_t month);


#endif
