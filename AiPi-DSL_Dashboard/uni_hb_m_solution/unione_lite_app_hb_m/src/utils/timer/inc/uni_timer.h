/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
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
 * Description : uni_timer.h
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_TIMER_INC_UNI_TIMER_H_
#define UTILS_TIMER_INC_UNI_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_TIMERHANDLE             (0)
#define TIMER_ERRNO_STOP_PERIODICAL     (0x09ABCDEF)

typedef enum {
  TIMER_TYPE_ONESHOT,
  TIMER_TYPE_PERIODICAL
} TimerType;

typedef int (*TimerExpireCallback)(void *arg);
typedef unsigned long long TimerHandle;

TimerHandle TimerStart(int interval_msec, TimerType type,
                       TimerExpireCallback fct, void *arg);
void        TimerStop(TimerHandle handle);
int         TimerInitialize(void);
void        TimerFinalize(void);

#ifdef __cplusplus
}
#endif
#endif
