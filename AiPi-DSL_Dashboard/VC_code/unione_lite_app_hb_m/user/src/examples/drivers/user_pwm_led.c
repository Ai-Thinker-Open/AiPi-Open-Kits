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
 * Description : user_pwm_led.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.26
 *
 **************************************************************************/
#include "user_pwm_led.h"

#define TAG "pwm_led"

#define LED_PWM_HZ        22000   // 22KHz
#define DUTY_CHG_STEP     1       // change 1 PWM duty a step
#define DUTY_CHG_STEP_MS  10      // delay 10ms a step

static const uni_u8 g_level_duty[BRIGHT_LEVEL_MAX] = {0, 10, 18, 32, 57, 100};
static uni_u8 g_cur_duty[PWM_NUM_MAX] = {0};

int user_pwm_led_init(USER_PWM_NUM num) {
  if (0 != user_pwm_init(num, LED_PWM_HZ, true)) {
    LOGE(TAG, "user_pwm_init failed.");
    return -1;
  }
  g_cur_duty[num] = 0;
  if (0 != user_pwm_start(num, g_cur_duty[num])) {
    LOGE(TAG, "user_pwm_start failed.");
    return -1;
  }
  return 0;
}

void user_pwm_led_final(USER_PWM_NUM num) {
  user_pwm_final(num);
}

int user_pwm_led_set_brightness(USER_PWM_NUM num,
                                         LED_BRIGHT_LEVEL level) {
  uni_u8 new_duty = g_level_duty[level];
  uni_u8 cur_duty = g_cur_duty[num];
  int ret = 0;
  while (cur_duty < new_duty) {
    cur_duty += DUTY_CHG_STEP;
    ret = user_pwm_change_duty(num, cur_duty);
    if (0 != ret) {
      break;
    }
    g_cur_duty[num] = new_duty;
    uni_msleep(DUTY_CHG_STEP_MS);
  } 
  while (cur_duty > new_duty) {
    cur_duty -= DUTY_CHG_STEP;
    ret = user_pwm_change_duty(num, cur_duty);
    if (0 != ret) {
      break;
    }
    g_cur_duty[num] = new_duty;
    uni_msleep(DUTY_CHG_STEP_MS);
  }
  if (0 != ret) {
    LOGE(TAG, "user_pwm_change_duty failed.");
    return -1;
  }
  return 0;
}


