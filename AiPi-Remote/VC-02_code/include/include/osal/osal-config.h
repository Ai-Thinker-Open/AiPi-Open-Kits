/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Yu
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_H_
#define OSAL_OSAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define STATIC_MEMORY_POOL_SIZE ( * 1024)

#define LOCK(lkp)                                      \
  do {                                                 \
    while (!__sync_bool_compare_and_swap(lkp, 0, 1)) { \
      OsalThreadDelayMs(1);                            \
    }                                                  \
  } while (0)

#define UNLOCK(lkp) \
  do {              \
    *(lkp) = 0;     \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_H_
