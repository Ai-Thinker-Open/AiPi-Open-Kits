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
 * Description : user_timer_buzzer.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#ifndef USER_INC_USER_TIMER_BUZZER_H_
#define USER_INC_USER_TIMER_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_gpio.h"
#include "user_timer.h"

int user_timer_buzzer_init(GPIO_NUMBER num, eTIMER_IDX idx);

void user_timer_buzzer_final(void);

int user_timer_buzzer_set_hz(uni_u32 hz);

uni_u32 user_timer_buzzer_get_hz(void);

#ifdef __cplusplus
}
#endif

#endif /*USER_INC_USER_TIMER_BUZZER_H_*/

