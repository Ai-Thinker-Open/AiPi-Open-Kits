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
 * Description : user_flash.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.14
 *
 **************************************************************************/
#include "user_flash.h"
#include "uni_hal_flash_easy.h"

#define TAG "user_flash"

int user_flash_init(void) {
  EfErrCode ret = uni_hal_flash_easy_init();
  if (EF_NO_ERR != ret) {
    LOGE(TAG, "uni_hal_flash_easy_init failed: %d", ret);
    return -1;
  }
  return 0;
}

void user_flash_final(void) {
}

int user_flash_set_env_blob(const char *key,
                                   const void *value_buf, int buf_len) {
  EfErrCode ret = uni_hal_flash_easy_set_env_blob(key, value_buf, buf_len);
  if (EF_NO_ERR != ret) {
    LOGE(TAG, "uni_hal_flash_easy_set_env_blob failed: %d", ret);
    return -1;
  }
  return buf_len;
}

int user_flash_get_env_blob(const char *key,
                                   void *value_buf, int buf_len,
                                   int *save_len) {
  size_t ret = uni_hal_flash_easy_get_env_blob(key,
                                               value_buf, buf_len,
                                               (size_t *)save_len);
  return ret;
}

