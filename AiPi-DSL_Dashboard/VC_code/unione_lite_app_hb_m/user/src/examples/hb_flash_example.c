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
 * Description : user_flash_example.c
 * Author      : yuanshifeng@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_flash.h"

#define TAG "flash_example"

#define BLOB_KEY_EXAMPLE_INT   "EXAMPLE_INT"
#define BLOB_TYPE_U32  uni_u32

int hb_flash(void) {
  BLOB_TYPE_U32 example_int = 0;
  int save_len = 0;

  if (0 != user_flash_init()) {
    LOGE(TAG, "user_flash_init failed.");
    return -1;
  }
  user_flash_get_env_blob(BLOB_KEY_EXAMPLE_INT, &example_int,
                          sizeof(BLOB_TYPE_U32), &save_len);
  if (save_len != sizeof(BLOB_TYPE_U32)) {
    //仅首次开机进入，发现flash中没有"EXAMPLE_INT"字段，保存一个88进去，待下次开机查看是否能直接读取出这个值。
    BLOB_TYPE_U32 set_int = 88;
    LOGT(TAG, "cannot found %s in flash, save value %d into flash", BLOB_KEY_EXAMPLE_INT, set_int);
    user_flash_set_env_blob(BLOB_KEY_EXAMPLE_INT, &set_int,sizeof(BLOB_TYPE_U32));
  }
  if (example_int > 0) {
    //打印读取flash中"EXAMPLE_INT"字段的值，读出88表示成功保存了值到flash中
    LOGT(TAG, "read %s in flash success, value is %d", BLOB_KEY_EXAMPLE_INT, example_int);
  }
  return 0;
}
