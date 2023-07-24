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
 * Description : user_flash.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.14
 *
 **************************************************************************/

#ifndef USER_INC_USER_FLASH_H_
#define USER_INC_USER_FLASH_H_
  
#ifdef __cplusplus
  extern "C" {
#endif
  
#include "unione.h"

/** @addtogroup uni_flash_inf
@{*/

/**
*@brief flash初始化
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_flash_init(void);

/**
*@brief flash逆初始化
*/
void user_flash_final(void);

/**
*@brief 保存键值数据对到flash
*@param key 键名
*@param value_buf 键值
*@param buf_len 键值长度
*@return 
* -成功：返回键值长度
* -失败：-1
*/
int user_flash_set_env_blob(const char *key,
                                   const void *value_buf, int buf_len);

/**
 * Get a blob ENV value by key name.
 *
 * @brief 读取输入键的值
 * @param key ENV name
 * @param value_buf ENV blob buffer
 * @param buf_len ENV blob buffer length
 * @param save_len return the length of the value saved on the flash, 0: NOT found
 *
 * @return the actually get size on successful
 */
int user_flash_get_env_blob(const char *key,
                                   void *value_buf, int buf_len,
                                   int *save_len);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif



