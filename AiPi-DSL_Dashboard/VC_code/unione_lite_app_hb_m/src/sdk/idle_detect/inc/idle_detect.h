/**************************************************************************
 * Copyright (C) 2019-2019  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : idle_detect.h
 * Author      : guozhenkun.unisound.com
 * Date        : 2019.01.29
 *
 **************************************************************************/
#ifndef SDK_IDLE_DETECT_INC_IDLE_DETECT_H_
#define SDK_IDLE_DETECT_INC_IDLE_DETECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef enum {
  IDLE_DETECT_TYPE_IDLE = 0,
  IDLE_DETECT_TYPE_BUSY,
  IDLE_DETECT_TYPE_UNKONWN
} IdleType;

typedef void (*IdleDetectCallback) (IdleType type);

Result IdleDetectServiceInit(void);
Result IdleDetectServiceFinal(void);
void IdleDetectServiceRegister(IdleDetectCallback callback, uni_u32 wait_idle_sec);
void IdleDetectServiceAccess(void);

#ifdef __cplusplus
}
#endif
#endif
