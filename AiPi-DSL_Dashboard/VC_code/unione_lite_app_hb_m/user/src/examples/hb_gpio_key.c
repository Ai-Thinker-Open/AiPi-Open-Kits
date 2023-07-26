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
 * Description : hb_gpio_key.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_gpio.h"
#include "user_digital_keys.h"
#include "user_asr.h"

#define TAG "hb_gpio_key"

#define USE_DIGITAL_KEYS_DRIVER  1  //1: user user_digital_keys driver, 0: user user_gpio directly
                                    // if 0 will send wakeup event in interrupt fuction,
                                    // it will crush because that's a long process,
                                    // so 0 is a demo show how to use GPIO directly only
                                    // donn't use it on your code

#define KEY_GPIO_NUM  GPIO_NUM_A25  // "MISO" on demo board

#if USE_DIGITAL_KEYS_DRIVER

void _key_interrupt_cb(GPIO_NUMBER num) {
  user_asr_goto_awakend();
}

int user_key_wakeup(void) {
  user_digital_keys_init(GPIO_INT_POS_EDGE);
  return user_digital_keys_register_key(KEY_GPIO_NUM, _key_interrupt_cb);
}

#else

static void _key_interrupt_cb(GPIO_NUMBER num, uni_bool is_high) {
  user_asr_goto_awakend();
}

int hb_key_wakeup(void) {
  user_gpio_init();
  if (0 != user_gpio_set_mode(KEY_GPIO_NUM, GPIO_MODE_IN)) {
    LOGE(TAG, "user_gpio_set_mode failed.");
    return -1;
  }
  user_gpio_set_pull_mode(KEY_GPIO_NUM, GPIO_PULL_UP_DOWN);
  if (0 != user_gpio_set_interrupt(KEY_GPIO_NUM, GPIO_INT_POS_EDGE,
                                   _key_interrupt_cb)) {
    LOGE(TAG, "user_gpio_set_interrupt failed.");
    return -1;
  }
  user_gpio_interrupt_enable();
}

#endif

