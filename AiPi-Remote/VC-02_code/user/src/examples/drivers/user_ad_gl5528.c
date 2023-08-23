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
 * Description : user_ad_gl5528.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#include "user_ad_gl5528.h"
#include "uni_hal_adc.h"

#define TAG "gl5528"

static GPIO_NUMBER g_adc_gpio_num = GPIO_NUM_MAX;

int user_ad_gl5528_init(GPIO_NUMBER num) {
  user_gpio_init();
  g_adc_gpio_num = num;
  return user_gpio_set_mode(num, GPIO_MODE_ADC);
}

void user_ad_gl5528_final(void) {
  g_adc_gpio_num = GPIO_NUM_MAX;
}

int user_ad_gl5528_get_val(void) {
  return user_gpio_get_value(g_adc_gpio_num);
}

