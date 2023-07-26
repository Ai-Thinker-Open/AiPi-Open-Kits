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
 * Description : user_timer_buzzer.c
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#include "user_sw_timer.h"
#include "user_gpio.h"

#define TAG "sw_timer_example"

#define USR_TIMER_NUM  eTIMER2  // user timer2
#define PARTICLE_MS    20       // 20ms

typedef struct MyTimer_s {
  timer_handle_t handle;
  int cnt;
}MyTimer;

static MyTimer m_timers[4] =
{
  {INVALID_TIMER_HANDLE, 0},
  {INVALID_TIMER_HANDLE, 0},
  {INVALID_TIMER_HANDLE, 0},
  {INVALID_TIMER_HANDLE, 0}
};

static void _sw_timer_cb(timer_handle_t timer) {
  int i;
  for (i =0; i < 4; i++) {
    if (timer == m_timers[i].handle) {
      m_timers[i].cnt++;
      break;
    }
  }
}

static void _timer_print_process(void *args) {
  while (1) {
    int i = 0;
    for (i =0; i < 4; i++) {
      LOGT(TAG, "---- Timer %d count = %d ----", i, m_timers[i].cnt);
    }
    uni_sleep(1);
  }
}

static Result _create_timer_print_thread(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "buzzer_set", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _timer_print_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

int hb_sw_timer(void) {
  if (E_OK != _create_timer_print_thread()) {
    LOGE(TAG, "_create_timer_print_thread failed");
    return -1;
  }
  user_sw_timer_init(USR_TIMER_NUM, PARTICLE_MS);
  m_timers[0].handle = user_sw_timer_add(PARTICLE_MS * 5, false, _sw_timer_cb);
  m_timers[1].handle = user_sw_timer_add(PARTICLE_MS * 10, false, _sw_timer_cb);
  m_timers[2].handle = user_sw_timer_add(PARTICLE_MS * 20, false, _sw_timer_cb);
  m_timers[3].handle = user_sw_timer_add(PARTICLE_MS * 40, true, _sw_timer_cb);   //count 1 time only
  return 0;
}

