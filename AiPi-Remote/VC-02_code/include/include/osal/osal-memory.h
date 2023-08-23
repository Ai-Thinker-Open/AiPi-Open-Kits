/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_MEMORY_H_
#define OSAL_OSAL_MEMORY_H_

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
 * @Description: 申请内存
 * @Input params: size: 申请内存大小，单位：字节
 * @Output params: 无
 * @Return: 成功：内存首地址
 *          失败：NULL
 */
OSAL_EXPORT void* OsalMalloc(size_t size);

/*
 * @Description: 释放内存
 * @Input params: ptr: 内存首地址
 * @Output params: 无
 * @Return: 空
 */
OSAL_EXPORT void OsalFree(void* ptr);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_MEMORY_H_
