/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_STATIC_MEMORY_POOL_H_
#define OSAL_OSAL_STATIC_MEMORY_POOL_H_

#include "osal/osal-types.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS
#endif

#ifndef _WINDOWS
#ifdef OSAL_API_HIDDEN
#define OSAL_EXPORT __attribute__((visibility("hidden")))
#else
#define OSAL_EXPORT __attribute__((visibility("default")))
#endif

#else
#ifdef DLL_EXPORT
#define OSAL_EXPORT __declspec(dllexport)
#else
#define OSAL_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @Description: 创建静态内存池
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalStaticMemoryPoolInit(void);

/*
 * @Description: 分配内存
 * @Input params: size: 大小，超出内存池可用内存大小时会分配新内存
 * @Output params:
 * @Return: 内存地址
 */
OSAL_EXPORT void* OsalStaticMemoryPoolAlloc(size_t size);

/*
 * @Description: 释放内存
 * @Input params: ptr: 内存地址
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalStaticMemoryPoolDealloc(void* ptr);

/*
 * @Description: 打印内存池使用情况
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalStatusMemoryPoolStatus(void);

/*
 * @Description: 获取内存池已经使用大小
 * @Input params:
 * @Output params:
 * @Return: used size
 */
OSAL_EXPORT size_t OsalStaticMemoryPoolUsedSize(void);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_STATIC_MEMORY_POOL_H_
