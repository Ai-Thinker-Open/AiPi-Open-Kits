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
 * Description : uni_audio_common.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef SDK_AUDIO_AUDIO_COMMON_INC_UNI_AUDIO_COMMON_H_
#define SDK_AUDIO_AUDIO_COMMON_INC_UNI_AUDIO_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uni_u32 channels;
  uni_u32 rate;
  uni_u32 bit; /*16, 32*/
} AudioParam;

#ifdef __cplusplus
}
#endif
#endif  //  SDK_AUDIO_AUDIO_COMMON_INC_UNI_AUDIO_COMMON_H_
