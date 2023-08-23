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
 * Description : uni_string.h
 * Author      : chenxiaosong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_STRING_INC_UNI_STRING_H_
#define UTILS_STRING_INC_UNI_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

/* Remove space from head and tail */
void StrStrip(char *str);
/* Find substr from "str" begin of "start" and end of "end", then write it
   into "dst"*/
void StrSub(char *dst, const char *str, const char *start, const char *end);
/* Just like StrSub except for excluding "start" and "end" in "dst" */
void StrSubEx(char *dst, const char *str, const char *start, const char *end);
/* Find the location(index) of "substr" from "str" */
int StrLoc(const char *str, const char *substr);
/* Split "str" based on "sep", and wirte the result into "dst", return the
   number of substrings */
int StrSplit(char *dst[], char *str, const char *sep);
/* Find substr "from" from "str", and replace them with "to" */
void StrReplaceS(char *str, const char *from, const char *to);
char* StrReplace(char *str, const char *from, const char *to);
/* Replace charactor "from" with "to" in str */
void StrReplaceC(char *str, char from, char to);
/* Delete charactor "target" in str */
void StrDeleteC(char *str, char target);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_STRING_INC_UNI_STRING_H_
