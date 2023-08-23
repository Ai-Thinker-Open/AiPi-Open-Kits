/**************************************************************************
 * Copyright (C) 2018-2019  Junlon2006
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
 * Description : uni_crc16.h
 * Author      : shangjinlong@unisound.com
 * Date        : 2019.11.11
 *
 **************************************************************************/

#ifndef UTILS_CRC16_INC_UNI_CRC16_H_
#define UTILS_CRC16_INC_UNI_CRC16_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint16_t crc16(const char *buf, int len);

#ifdef __cplusplus
}
#endif
#endif   //  UTILS_CRC16_INC_UNI_CRC16_H_
