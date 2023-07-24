/**************************************************************************
 * Copyright (C) 2020-2020  Unisound
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
 * Description : user_sw_timer.c
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#include "user_sw_timer.h"
#include "user_gpio.h"

#define TAG "sw_timer"

typedef struct usr_timer_s {
  unsigned long data;
  unsigned long sec;
  unsigned long usec;
  usr_sw_timer_callback cb;
  unsigned short magic;
  bool enable;
  bool single_shot;
}usr_timer_t;

typedef struct usr_time_list_s {
  usr_timer_t timers[MAX_TIMER_SUPPORT];
  uni_mutex_t mutex;
  uint32_t period;
  eTIMER_IDX timer_idx;
  bool is_run;
}usr_timer_list_t;

volatile static unsigned long usr_time_sec = 0;
volatile static unsigned long usr_time_usec = 0;

static usr_timer_list_t g_usr_timer;

static void _user_timer_handle(eTIMER_IDX id) {
  int i;
  usr_timer_t *timer;
  usr_time_usec += g_usr_timer.period;
  if (usr_time_usec >= 1000000) {
    usr_time_sec++;
    usr_time_usec -= 1000000;
  }
  for (i=0; i < MAX_TIMER_SUPPORT; i++) {
    timer = &g_usr_timer.timers[i];
    if (timer->enable) {
      if (usr_time_sec < timer->sec) {
        continue;
      } else if ((usr_time_sec == timer->sec) && (usr_time_usec < timer->usec)) {
        continue;
      }
      if (timer->single_shot) {
        timer->enable = false;
      } else {
        timer->sec = (timer->data / 1000) + usr_time_sec;
        timer->usec = ((timer->data % 1000) * 1000) + usr_time_usec;
      }
      if (timer->cb) {
        timer->cb((timer_handle_t)((timer->magic << 16) | i));
      }
    }
  }
}

void user_sw_timer_remove(timer_handle_t handle) {
  unsigned long i;
  unsigned short magc;
  usr_timer_t *timer;
  uni_pthread_mutex_lock(g_usr_timer.mutex);
  i = handle & 0xFFFF;
  magc = handle >> 16;
  LOGT(TAG, "handle = 0X%x", handle);
  if (i < MAX_TIMER_SUPPORT) {
    timer = &g_usr_timer.timers[i];
    if (timer->enable && magc == timer->magic) {
      timer->enable = false;
    } else {
      LOGW(TAG, "already remove");
    }
  } else {
    LOGE(TAG, "invalid handle 0x%x", handle);
  }
  if (g_usr_timer.is_run) {
    for (i = 0; i < MAX_TIMER_SUPPORT; i++) {
      timer = &g_usr_timer.timers[i];
      if (timer->enable) {
        break;
      }
    }
    if (MAX_TIMER_SUPPORT == i) {
      user_timer_pause(g_usr_timer.timer_idx);
      g_usr_timer.is_run = false;
    }
  }
  uni_pthread_mutex_unlock(g_usr_timer.mutex);
}

timer_handle_t user_sw_timer_add(uint32_t msec, bool single_shot,
                                 usr_sw_timer_callback cb) {
  unsigned long i;
  usr_timer_t *timer;
  timer_handle_t handle = INVALID_TIMER_HANDLE;
  uni_pthread_mutex_lock(g_usr_timer.mutex);
  for (i = 0; i < MAX_TIMER_SUPPORT; i++) {
    timer = &g_usr_timer.timers[i];
    if(! timer->enable) {
      timer->magic++;
      handle = (timer->magic << 16) | i;
      timer->cb = cb;
      timer->single_shot = single_shot;
      timer->data = msec;
      timer->sec = (msec / 1000) + usr_time_sec;
      msec = msec % 1000;
      timer->usec = (msec * 1000) + usr_time_usec;
      timer->enable = true;
      if (! g_usr_timer.is_run) {
        user_timer_resume(g_usr_timer.timer_idx);
        g_usr_timer.is_run = true;
      }
      break;
    }
  }
  uni_pthread_mutex_unlock(g_usr_timer.mutex);
  LOGT(TAG, "handle = 0X%x", handle);
  return handle;
}

void user_sw_timer_init(eTIMER_IDX timer_idx, uint32_t particle) {
  uni_memset(&g_usr_timer, 0, sizeof(g_usr_timer));
  uni_pthread_mutex_init(&g_usr_timer.mutex);
  g_usr_timer.timer_idx = timer_idx;
  if (particle > 60) {
    particle = 60;
  }
  g_usr_timer.period = particle * 500;
  user_timer_init(timer_idx, g_usr_timer.period, false, _user_timer_handle);
  user_timer_start(timer_idx);
  //g_usr_timer.is_run = true;
  user_timer_pause(timer_idx);
}
