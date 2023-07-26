/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_MUTEX_H_
#define OSAL_OSAL_MUTEX_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* mutex 句柄 */
typedef void* OsalMutexHandle;

/*
 * @Description: 互斥锁初始化
 * @Input params: mutex: 无
 * @Output params: 互斥锁句柄
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMutexInit(OsalMutexHandle* mutex);

/*
 * @Description: 互斥锁 Lock
 * @Input params: mutex: 互斥锁句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMutexLock(OsalMutexHandle mutex);

/*
 * @Description: 互斥锁 Unlock
 * @Input params: mutex: 互斥锁句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMutexUnlock(OsalMutexHandle mutex);

/*
 * @Description: 互斥锁销毁
 * @Input params: mutex: 互斥锁句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMutexDestroy(OsalMutexHandle mutex);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_MUTEX_H_
