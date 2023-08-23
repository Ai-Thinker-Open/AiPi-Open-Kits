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
 * Description : Uniapp recog_common.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.12.28
 *
 **************************************************************************/

#ifndef SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_COMMON_H_
#define SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_vui_interface.h"

#define DEFAULT_WAKEUP_SCENE_STD_THRESHOLD    (-30)
#define DEFAULT_WAKEUP_SCENE_LOW_THRESHOLD    (-30)

#define DEEP_SLEEP_WAKEUP_SCENE_STD_THRESHOLD (-30)
#define DEEP_SLEEP_WAKEUP_SCENE_LOW_THRESHOLD (-30)

#define DEFAULT_RECOGN_SCENE_STD_THRESHOLD    (-30)
#define DEFAULT_RECOGN_SCENE_LOW_THRESHOLD    (-30)

#define DEFAULT_RECOGN_SCENE_TIMEOUT          (10000)

typedef struct {
  uni_s32  asr_mode;       /* UNI_STD_WAKEUP, UNI_STD_LASR_RASR... */
  uni_s32  timeout;        /* capture timeout */
  float    low_threshold;  /* secondary wakeup/recogn threshold */
  float    std_threshold;  /* standard wakeup/recogn threshold */
  uni_s32  offset_timems;  /* judgement for secondary wakeup/recogn */
  void     *grammar;       /* for further usage */
} Scene;

typedef enum {
  ENGINE_CB_TYPE_BEFORE_START,
  ENGINE_CB_TYPE_AFTER_START,
  ENGINE_CB_TYPE_BEFORE_CANCEL,
  ENGINE_CB_TYPE_AFTER_CANCEL,
  ENGINE_CB_TYPE_BEFORE_STOP,
  ENGINE_CB_TYPE_AFTER_STOP
} EngineCbType;

typedef void (* EngineCallback)(VuiHandle vui, Scene *scene);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_COMMON_H_
