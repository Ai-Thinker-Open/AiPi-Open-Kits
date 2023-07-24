/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_TYPES_H_
#define OSAL_OSAL_TYPES_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t osal_off_t;

/* Notice：64 位系统定义__WORDSIZE 为 64 */
#if __WORDSIZE == 64
typedef int64_t osal_ssize_t;
#else
typedef int32_t osal_ssize_t;
#endif

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_TYPES_H_
