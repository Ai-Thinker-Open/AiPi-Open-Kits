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
 * Description : hb_gpio_adc.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_gpio.h"
#include "user_ad_gl5528.h"
#include "user_asr.h"

#define TAG "hb_gpio_adc"

#define USE_AD_GL5528_DRIVER     1  //1: user user_ad_gl5528 driver, 0: user user_gpio directly

#define KEY_GPIO_NUM  GPIO_NUM_A25  // "MISO" on demo board

#define GL5528_VAL_THRE 1000

static void _adc_read_process(void *args);

static uni_bool g_state = false;   // false: disable, true: enable

static Result _create_adc_read_thread(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "gpio_adc", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _adc_read_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

#if USE_AD_GL5528_DRIVER

static void _adc_read_process(void *args) {
  int val = 0;
  while (1) {
    uni_sleep(1);
    val = user_ad_gl5528_get_val();
    if (val >= 0) {
      LOGT(TAG, "****** gl5528 adc val = %d *****", val);
      if (val < GL5528_VAL_THRE) {    // means daytime, enable
        if (!g_state) {
          user_asr_recognize_enable();
          g_state = true;
        }
      } else {
        if (g_state) {
          user_asr_recognize_disable();
          user_asr_goto_sleep();
          g_state = false;
        }
      }
    }
  }
}

int user_adc_ctrl(void) {
  user_asr_recognize_disable();
  g_state = false;
  if (0 != user_ad_gl5528_init(KEY_GPIO_NUM)) {
    LOGE(TAG, "user_ad_gl5528_init faild.");
    return -1;
  }
  if (E_OK != _create_adc_read_thread()) {
    LOGE(TAG, "_create_adc_read_thread faild.");
    return -1;
  }
  return 0;
}

#else

static void _adc_read_process(void *args) {
  int val = 0;
  while (1) {
    uni_sleep(1);
    val = user_gpio_get_value(KEY_GPIO_NUM);
    if (val >= 0) {
      LOGT(TAG, "****** gl5528 adc val = %d *****", val);
      if (val < GL5528_VAL_THRE) {    // means daytime, enable
        if (!g_state) {
          user_asr_recognize_enable();
          g_state = true;
        }
      } else {
        if (g_state) {
          user_asr_recognize_disable();
          user_asr_goto_sleep();
          g_state = false;
        }
      }
    }
  }
}

int hb_adc_ctrl(void) {
  user_asr_recognize_disable();
  g_state = false;
  user_gpio_init();
  if (0 != user_gpio_set_mode(KEY_GPIO_NUM, GPIO_MODE_ADC)) {
    LOGE(TAG, "user_ad_gl5528_init faild.");
    return -1;
  }
  if (E_OK != _create_adc_read_thread()) {
    LOGE(TAG, "_create_adc_read_thread faild.");
    return -1;
  }
  return 0;
}

#endif

