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
 * Description : bkdr_hash.c
 * Author      : chenxiaosong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_hash.h"
#include "uni_log.h"

#define BKDR_HASH_TAG "bkdr_hash"

uni_u32 bkdr_hash(const char* key) {
  uni_u32 seed = 31; // 31 131 1313 13131 131313 etc..
  uni_u32 hash = 0;
  uni_u8 *cp = (uni_u8 *)key;
  if (NULL == cp) {
    LOGE(BKDR_HASH_TAG, "hash input is NULL");
    return 0;
  }
  while (*cp) {
    hash = (hash * seed + (*cp++)) & 0xFFFFFFFF;
  }
  return hash;
}
