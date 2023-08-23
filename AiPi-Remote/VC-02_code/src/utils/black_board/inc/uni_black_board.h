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
 * Description : black_board.h
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.12.28
 *
 **********************************************************************/

#ifndef UTILS_BLACK_BOARD_INC_UNI_BLACK_BOARD_H_
#define UTILS_BLACK_BOARD_INC_UNI_BLACK_BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef enum {
  BB_KEY_IS_WAKEUP = 0,
  BB_KEY_IS_IN_OTA,
  BB_KEY_IS_MIC_MUTE,
  BB_KEY_BOOT_MUTE,
  BB_KEY_EVENT_ID,
  BB_KEY_IS_FACTORY_MODE,
  BB_KEY_IS_HVAD_WAKEUP,
  BB_KEY_MAX
}BLACK_BOARD_KEY;

/**
 * write key:value into black board
 * @return return write success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbWrite(BLACK_BOARD_KEY key, int value);

/**
 * read value with of key from black board
 * @return return read success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbRead(BLACK_BOARD_KEY key, int *value);

/**
 * init black board
 * @return return init success or fail
 * @retval 0 presents success, -1 presents failed
*/
int BbInit(void);

/**
 * destroy black board
 * @return
 * @retval 0 presents success, -1 presents failed
*/
void BbFinal(void);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_BLACK_BOARD_INC_UNI_BLACK_BOARD_H_
