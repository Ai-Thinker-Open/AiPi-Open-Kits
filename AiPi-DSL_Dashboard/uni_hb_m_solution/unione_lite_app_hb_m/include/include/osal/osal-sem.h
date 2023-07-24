/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_SEM_H_
#define OSAL_OSAL_SEM_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sem 句柄 */
typedef void* OsalSemHandle;

/*
 * @Description: 信号量初始化
 * @Input params: value: 初始值
 * @Output params: sem: 信号量句柄
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalSemInit(OsalSemHandle* sem, int32_t value);

/*
 * @Description: 信号量 Post
 * @Input params: sem: 信号量句柄
 * @Output params:
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalSemPost(OsalSemHandle sem);

/*
 * @Description: 信号量 Wait
 * @Input params: sem: 信号量句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalSemWait(OsalSemHandle sem);

/*
 * @Description: 超时信号量 Wait
 * @Input params: sem: 信号量句柄
 *                abs_timeout: 超时时间结构
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalSemTimedWait(OsalSemHandle sem, uint32_t wait_milliseconds);

/*
 * @Description: 信号量销毁
 * @Input params: sem: 信号量句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalSemDestroy(OsalSemHandle sem);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_SEM_H_
