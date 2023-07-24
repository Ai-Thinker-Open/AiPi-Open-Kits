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
 * Description : user_pwm_led.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.26
 *
 **************************************************************************/
#ifndef USER_INC_USER_PWM_LED_H_
#define USER_INC_USER_PWM_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_pwm.h"

typedef enum {
  BRIGHT_LEVEL_0 = 0,
  BRIGHT_LEVEL_1,
  BRIGHT_LEVEL_2,
  BRIGHT_LEVEL_3,
  BRIGHT_LEVEL_4,
  BRIGHT_LEVEL_5,
  BRIGHT_LEVEL_MAX
}LED_BRIGHT_LEVEL;

int user_pwm_led_init(USER_PWM_NUM num);

void user_pwm_led_final(USER_PWM_NUM num);

int user_pwm_led_set_brightness(USER_PWM_NUM num,
                                         LED_BRIGHT_LEVEL level);

#ifdef __cplusplus
}
#endif

#endif /*USER_INC_USER_PWM_LED_H_*/

