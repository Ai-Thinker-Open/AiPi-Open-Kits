/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_TIME_H_
#define OSAL_OSAL_TIME_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 1 s = 1000 ms */
#define OSAL_TIME_S2MS (1000)

/* 1 s = 1000 * 1000 us */
#define OSAL_TIME_S2US (1000000)

/* 1 s = 1000 * 1000 * 1000 ns */
#define OSAL_TIME_S2NS (1000000000)

/* 1 ms = 1000 us */
#define OSAL_TIME_MS2US (1000)

/* 1 ms = 1000 * 1000 ns*/
#define OSAL_TIME_MS2NS (1000000)

/* 1 us = 1000 ns*/
#define OSAL_TIME_US2NS (1000)

#define STAMP_STRING_LENGTH 20

/*
 * 系统时间戳结构
 * 32bit 秒最多到 2038.01.18 19:14:07
 * 64bit 秒最多到 3001.01.01 00:00:00
 */
typedef union OsalTimeStamp {
  struct {
    int32_t month;       /* 时间戳（月）*/
    int32_t day;         /* 时间戳（日）*/
    int32_t hour;        /* 时间戳（时）*/
    int32_t minute;      /* 时间戳（分）*/
    int32_t second;      /* 时间戳（秒）*/
    int32_t microsecond; /* 时间戳（毫秒）*/
  } date;
  int64_t tm_msec; /* 1970年到现在的毫秒数 */
  int64_t ticks;   /* 时钟 Ticks*/
} OsalTimeStamp;

/*
 * @Description: 获取当前时间戳
 * @Input params: 无
 * @Output params: stamp：当前系统时间戳
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalTimeGetStamp(OsalTimeStamp* stamp);

/*
 * @Description: 获取毫秒数,多用于实施率统计
 * @Input params:
 * @Output params:
 * @Return:
 */
int32_t OsalTimeGetMs(OsalTimeStamp* stamp);

/*
 * @Description: 时间戳差值
 * @Input params:
 * @Output params:
 * @Return: stamp1 - stamp2, 单位：ms
 */
int64_t OsalTimeDiff(OsalTimeStamp* stamp1, OsalTimeStamp* stamp2);

/*
 * @Description: 时间戳转换成 string
 * @Input params: stamp: 需要转换的时间戳
 * @Input params: stamp_string: 时间戳转换为字符串
 * @Output params:
 * @Return:
 */
int32_t OsalTimeGetStampString(const OsalTimeStamp* stamp, char* stamp_string);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_TIME_H_
