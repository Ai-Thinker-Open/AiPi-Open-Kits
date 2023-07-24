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
 * Description : Uniapp recog_preproc.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.12.27
 *
 **************************************************************************/

#include "uni_iot.h"
#include "uni_event.h"
#include "uni_vui_interface.h"

#ifndef SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_PREPROC_H_
#define SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_PREPROC_H_

#ifdef __cplusplus
extern "C" {
#endif

Result RecogPreprocInit();
void   RecogPreprocFinal();

Result RecogPreproc(Event *event, uni_s32 asr_mode, float std_thresh,
                    float low_thresh, VuiHandle vui);

#ifdef __cplusplus
}
#endif
#endif
