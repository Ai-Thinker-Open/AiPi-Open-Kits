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
 * Description : uni_float2string.h
 * Author      : shangjinlong@unisound.com
 * Date        : 2019.12.16
 *
 **********************************************************************/

#ifndef UTILS_FLOAT2STRING_INC_UNI_FLOAT2STRING_H_
#define UTILS_FLOAT2STRING_INC_UNI_FLOAT2STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Thread unsafe API, donot use in multi-thread env
 * only used in asr result parse, print score as string
 */
const char* Float2String(float f);
const char* PFloat2String(float *pf);

#ifdef __cplusplus
}
#endif
#endif  // UTILS_FLOAT2STRING_INC_UNI_FLOAT2STRING_H_
