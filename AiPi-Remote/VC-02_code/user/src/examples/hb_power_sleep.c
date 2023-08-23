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
 * Description : hb_power_sleep.c
 * Author      : wangzhicheng@unisound.com
 * Date        : 2020.07.23
 *
 **************************************************************************/
#include "user_gpio.h"
#include "user_digital_keys.h"
#include "uni_hal_power.h"
#include "user_asr.h"

#define TAG "hb_power_sleep"

/* 本demo使用GPIO A26作为功耗模式的检测pin，拉低时进入低功耗模式，拉高时唤醒 */
#define KEY_GPIO_POWER_WAKEUP_NUM   GPIO_NUM_A26

static void _wakeup_cb(int flag) {
  /* interrupt cb: cann't use LOGX */
  uni_printf("deepsleep wake up!!!\n");
}

static void _gpio_trigger_process(void *args) {
  int value;
  while (1) {
    value = user_gpio_get_value(KEY_GPIO_POWER_WAKEUP_NUM);
    uni_msleep(50);
    if (user_gpio_get_value(KEY_GPIO_POWER_WAKEUP_NUM) != value) {
      LOGT(TAG, "pin jittery !!!\n");
      uni_msleep(100);
      continue;
    }
    if (!value) {
      LOGT(TAG, "go to sleep !!!\n");
      uni_hal_enterdeepsleep(_wakeup_cb, WAKEUP_GPIOA26, WAKEUP_GPIOPOSE);
      LOGT(TAG, "sleep to wakeup !!!\n");
    }
  }
}

static Result _create_gpio_trigger_thread(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_NORMAL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_pthread_t pid;
  uni_strncpy(param.task_name, "gpio_trigger", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _gpio_trigger_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

static void _gpio_init(void) {
  user_gpio_set_mode(KEY_GPIO_POWER_WAKEUP_NUM, GPIO_MODE_IN);
  _create_gpio_trigger_thread();
}

int hb_power_sleep(void) {
  _gpio_init();
  return 0;
}
