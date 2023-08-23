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
 * Description : uni_pcm_resource.h
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.17
 *
 **************************************************************************/
#ifndef SDK_PLAYER_INC_UNI_PCM_RESOURCE_H_
#define SDK_PLAYER_INC_UNI_PCM_RESOURCE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_types.h"

int PcmGetResourceInfo(const char *name, uni_u32 *offset, uni_u32 *len) ;

#ifdef __cplusplus
}
#endif
#endif  //  SDK_PLAYER_INC_UNI_PCM_RESOURCE_H_

