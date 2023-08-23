/**********************************************************************
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
 **********************************************************************
 *
 * Description : Uniapp uni_arpt.h
 * Author      : wangzhicheng@unisound.com 
 * Date        : 2017.11.14
 *
 **********************************************************************/

#ifndef UTILS_ARPT_INC_UNI_ARPT_H_
#define UTILS_ARPT_INC_UNI_ARPT_H_

#include "uni_iot.h"

int  ArptInit(void);
void ArptFinal(void);
int  ArptWrite(const char *content, uni_bool is_more);

#endif  // UTILS_ARPT_INC_UNI_ARPT_H_
