/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef INCLUDE_AIK_ERRNO_H_
#define INCLUDE_AIK_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define AIK_OK 0
#define AIK_FAILED -1
#define AIK_MALLOC_FAILED -2

#define AIK_NETWORK_OK 1
#define AIK_NETWORK_ERROR -1

#define AIK_SYSTEM_NORMAL 1
#define AIK_SYSTEM_LOWPOWER 2

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_AIK_ERRNO_H_
