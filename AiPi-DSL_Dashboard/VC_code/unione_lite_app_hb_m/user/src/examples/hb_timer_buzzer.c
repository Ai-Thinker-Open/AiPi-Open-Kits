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
 * Description : hb_timer_buzzer.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_gpio.h"
#include "user_timer_buzzer.h"

#define TAG "hb_timer_buzzer"

#define BUZZER_GPIO_NUM GPIO_NUM_A25  // "MISO" on demo board
#define USR_TIMER_NUM   eTIMER2       // user timer2

static uni_u32 g_cur_hz = 0;

static void _buzzer_set_process(void *args) {
  while (1) {
    g_cur_hz += 500;
    if (g_cur_hz > 4000) {
      g_cur_hz = 500;
    }
    user_timer_buzzer_set_hz(g_cur_hz);
    uni_sleep(1);
  }
}

static Result _create_buzzer_set_thread(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "buzzer_set", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _buzzer_set_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

int hb_timer_buzzer(void) {
  if (0 != user_timer_buzzer_init(BUZZER_GPIO_NUM, USR_TIMER_NUM)) {
    LOGE(TAG, "user_timer_buzzer_init failed.");
    return -1;
  }
  if (E_OK != _create_buzzer_set_thread()) {
    LOGE(TAG, "_create_buzzer_set_thread failed.");
    return -1;
  }
  return 0;
}

