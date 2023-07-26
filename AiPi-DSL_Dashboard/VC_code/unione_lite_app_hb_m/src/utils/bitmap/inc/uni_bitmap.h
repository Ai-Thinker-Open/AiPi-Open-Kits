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
 * Description : uni_bitmap.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.9.19
 *
 **********************************************************************/

#ifndef UTILS_BITMAP_INC_UNI_BITMAP_H_
#define UTILS_BITMAP_INC_UNI_BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef struct {
  uni_u32 size;
  uni_u32 *map;
} BitMap;

/**
 * Usage:   Create a bitmap
 * Params:  size    bitmap size in bit
 * Return:  Result of creation
 */
BitMap* BitMapNew(int size);

/**
 * Usage:   Delete a bitmap
 * Params:  bitmap    bitmap to be deleted
 * Return:  Result of deletion
 */
void BitMapDel(BitMap *bitmap);

/**
 * Usage:   Set a bit of bitmap
 * Params:  bitmap    bitmap to be changed
 *          i         bit location
 * Return:  Result of setting
 */
int BitMapSet(BitMap *bitmap, uni_u32 i);

/**
 * Usage:   Unset a bit of bitmap
 * Params:  bitmap    bitmap to be changed
 *          i         bit location
 * Return:  Result of unsetting
 */
int BitMapClear(BitMap *bitmap, uni_u32 i);

/**
 * Usage:   Test a bit of bitmap
 * Params:  bitmap    bitmap to be tested
 *          i         bit location
 * Return:  Result of testing
 */
int BitMapTest(BitMap *bitmap, uni_u32 i);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_BITMAP_INC_UNI_BITMAP_H_
