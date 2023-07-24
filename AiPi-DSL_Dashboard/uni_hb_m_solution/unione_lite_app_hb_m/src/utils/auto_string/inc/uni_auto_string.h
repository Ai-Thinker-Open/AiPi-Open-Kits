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
 * Description : uni_auto_string.h
 * Author      : wufangfang@unisound.com
 * Date        : 2019.05.22
 *
 **************************************************************************/
#ifndef UNI_AUTO_STRING_H_
#define UNI_AUTO_STRING_H_

#include "uni_iot.h"

typedef void * STRING_HANDLE;

/**
 * Usage:   Creat an auto string
 * Params:  ch:   origin string buffer
 * Return:  Point to auto string object
 */
STRING_HANDLE StringCreat(const char* ch);

/**
 * Usage:   Destroy a string
 * Params:  handle: point to string object
 * Return:  None
 */
void StringDestroy(STRING_HANDLE handle);

/**
 * Usage:   Assignment a string buffer to auto string
 * Params:  handle: point to string object
 *          ch:     origin string buffer
 * Return:  String Length
 */
int StringAssignment(STRING_HANDLE handle, const char* ch);

/**
 * Usage:   Copy string from auto string to a buffer
 * Params:  handle: point to string object
 * Return:  String length
 */
int SringGetLength(STRING_HANDLE handle);

/**
 * Usage:   Get string buffer of auto string
 * Params:  handle: point to string object
 * Return:  String buffer, cannot modify or free it!
 */
char* SringGetString(STRING_HANDLE handle);

/**
 * Usage:   Copy string from auto string to a buffer
 * Params:  handle: point to string object
 *          buf:    a buffer save string
 *          size:   size of this buffer
 * Return:  Copy Length
 */
int SringCopy(STRING_HANDLE handle, char* buf, int size);

/**
 * Usage:   Comparison string with another string
 * Params:  handle: point to string object
 *          ch:    a buffer save string
 * Return:  0 - means uniform, other meas diffrent
 */
int SringCmp(STRING_HANDLE handle, char* ch);

#endif  //  UNI_AUTO_STRING_H_