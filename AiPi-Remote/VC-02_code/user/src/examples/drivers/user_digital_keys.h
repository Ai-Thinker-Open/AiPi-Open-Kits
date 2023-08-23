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
 * Description : user_digital_keys.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#ifndef USER_INC_USER_DIGITAL_KEYS_H_
#define USER_INC_USER_DIGITAL_KEYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_gpio.h"

typedef void (*_keys_trigger_cb)(GPIO_NUMBER num);

int user_digital_keys_init(GPIO_INT_TYPE type);

void user_digital_keys_final(void);

int user_digital_keys_register_key(GPIO_NUMBER num, _keys_trigger_cb cb);

int user_digital_keys_cancel_key(GPIO_NUMBER num);

#ifdef __cplusplus
}
#endif

#endif
