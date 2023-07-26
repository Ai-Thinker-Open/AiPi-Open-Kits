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
 * Description : hb_pwm_led.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.26
 *
 **************************************************************************/
#include "user_gpio.h"
#include "user_pwm_led.h"

#define TAG "hb_pwm_led"

#define PWM_LED_GPIO_NUM  PWM_NUM_1_A27   // "MOSI" on HB-M demo board

static void _pwm_led_process(void *args) {
  LED_BRIGHT_LEVEL level = BRIGHT_LEVEL_0;
  while (1) {
    user_pwm_led_set_brightness(PWM_LED_GPIO_NUM, level);
    level += 1;
    if (level >= BRIGHT_LEVEL_MAX) {
      level = BRIGHT_LEVEL_0;
    }
    //uni_sleep(1);
  }
}

static Result _create_pwm_led_thread(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "pwm_led", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _pwm_led_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

int hb_pwm_led(void) {
  if (0 != user_pwm_led_init(PWM_LED_GPIO_NUM)) {
    LOGE(TAG, "user_pwm_led_init failed.");
    return -1;
  }
  if (E_OK != _create_pwm_led_thread()) {
    LOGE(TAG, "_create_pwm_led_thread failed.");
    user_pwm_led_final(PWM_LED_GPIO_NUM);
    return -1;
  }
  return 0;
}

