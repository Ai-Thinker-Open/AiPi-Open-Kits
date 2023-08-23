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
 * Description : user_ad_gl5528.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#ifndef USER_INC_USER_AD_GL5528_H_
#define USER_INC_USER_AD_GL5528_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_gpio.h"

int user_ad_gl5528_init(GPIO_NUMBER num);

void user_ad_gl5528_final(void);

int user_ad_gl5528_get_val(void);

#ifdef __cplusplus
}
#endif

#endif /*USER_INC_USER_AD_GL5528_H_*/
