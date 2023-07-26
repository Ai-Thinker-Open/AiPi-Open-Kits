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
 * Description : uni_pcm_resource.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.17
 *
 **************************************************************************/

#include "uni_pcm_resource.h"
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_pcm_array.h"

#define TAG  "pcm_res"

int PcmGetResourceInfo(const char *name, uni_u32 *offset, uni_u32 *len) {
  uni_u32 val = uni_strtol(name, NULL, 10);
  int source_len = sizeof(g_pcm_arry) / sizeof(PCM_RESOURCE);
  int i = 0;
  for (i = 0; i < source_len; i++) {
    if (val == g_pcm_arry[i].number) {
      *offset = g_pcm_arry[i].offset;
      *len = g_pcm_arry[i].len;
      return 0;
    }
  }
  /* not found reply none (mute audio) */
  for (i = 0; i < source_len; i++) {
    if (0 == g_pcm_arry[i].number) {
      *offset = g_pcm_arry[i].offset;
      *len = g_pcm_arry[i].len;
      return 0;
    }
  }
  return -1;
}

