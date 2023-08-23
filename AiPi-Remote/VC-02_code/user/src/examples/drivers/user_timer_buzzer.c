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
#include "user_timer_buzzer.h"
#include "user_timer.h"
#include "user_gpio.h"

#define TAG "buzzer"

#define BUZZER_HZ_MAX 4000

typedef struct {
  eTIMER_IDX    timer_idx;
  GPIO_NUMBER   gpio_num;
  uni_u32       hz;
  uni_u32       time_us;
}buzzer_context_t;

static buzzer_context_t g_buzzer = {0};

static void _buzzer_timer_handle(eTIMER_IDX TimerIdx) {
#if GPIO_OUT_DEF_VAL
  volatile static int cur_level = 1;
#else
  volatile static int cur_level = 0;
#endif
  if (g_buzzer.timer_idx == TimerIdx) {
    cur_level ^= 1;
    user_gpio_set_value(g_buzzer.gpio_num, cur_level);
  }
}

int user_timer_buzzer_init(GPIO_NUMBER num, eTIMER_IDX idx) {
  user_gpio_init();
  if (0 != user_gpio_set_mode(num, GPIO_MODE_OUT)) {
    LOGE(TAG, "user_gpio_set_mode failed.");
  }
  g_buzzer.timer_idx = idx;
  g_buzzer.hz = 0;
  g_buzzer.gpio_num = num;
  g_buzzer.time_us = 0;
  return 0;
}

void user_timer_buzzer_final(void) {
  user_timer_pause(g_buzzer.timer_idx);
}

int user_timer_buzzer_set_hz(uni_u32 hz) {
  if (hz > BUZZER_HZ_MAX) {
    LOGE(TAG, "The maximum frequency is 4000Hz.");
    return -1;
  }
  user_timer_pause(g_buzzer.timer_idx);
  g_buzzer.hz = hz;
  g_buzzer.time_us = 1000 * 1000 / g_buzzer.hz;
  user_timer_init(g_buzzer.timer_idx, g_buzzer.time_us, false,
                  _buzzer_timer_handle);
  user_timer_start(g_buzzer.timer_idx);
  return 0;
}

uni_u32 user_timer_buzzer_get_hz(void) {
  return g_buzzer.hz;
}


