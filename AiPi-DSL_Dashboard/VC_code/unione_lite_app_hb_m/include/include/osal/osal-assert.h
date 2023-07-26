/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_ASSERT_H_
#define OSAL_OSAL_ASSERT_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @Description: assert
 * @Input params:
 * @Output params:
 * @Return:
 */
void OsalAssert(int32_t expression);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_ASSERT_H_
