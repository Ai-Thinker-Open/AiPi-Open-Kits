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
 * Description : uni_types.h
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.10
 *
 **************************************************************************/
#ifndef HAL_INC_UNI_TYPES_H_
#define HAL_INC_UNI_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef char                uni_s8;
typedef unsigned char       uni_u8;
typedef short               uni_s16;
typedef unsigned short      uni_u16;
typedef int                 uni_s32;
typedef unsigned int        uni_u32;
typedef long long           uni_s64;
typedef unsigned long long  uni_u64;
typedef int                 uni_bool;

#ifndef true
#define true                  (1)
#endif

#ifndef false
#define false                 (0)
#endif

#ifndef NULL
  #define NULL                ((void *)0)
#endif

/* for event tree */
#define ID(EVENT_NAME)    (EVENT_NAME)

typedef enum {
  E_OK,
  E_FAILED,
  E_REJECT,
  E_HOLD
} Result;

typedef enum {
  COMMON_STOP_EVENT = 0,
  COMMON_RESUME_EVENT,
  AUDIO_PLAY_END_EVENT,
  INVALID_EVENT_ID_EVENT_INVALID,
  SESSION_MGR_REVIEW_EVENT,
  VUI_APP_SETTING_EVENT,
  VUI_APP_SLEEP_EVENT,
  VUI_APP_VOLUME_SETTING_EVENT,
  VUI_APP_WAKEUP_EVENT,
  VUI_APP_MIC_MUTE_EVENT,
  VUI_APP_MIC_UNMUTE_EVENT,
  VUI_LOCAL_ASR_SELF_RESULT_EVENT,
  VUI_LOCAL_ASR_TIMEOUT_EVENT,
  VUI_LOCAL_ASR_FAIL_EVENT,
  WATCH_DOG_FEED_EVENT,
  APP_GOTO_WAKEUP_EVENT,
  APP_SLEEP_MODE_EVENT,
  USER_AUDIO_PLAY_EVENT,
  USER_AUDIO_STOP_EVENT,
  USER_GOTO_SLEEPING_EVENT,
  USER_GOTO_AWAKEND_EVENT,
  USER_MIC_MUTE_EVENT,
  USER_VOLUME_SETTING_EVENT,
} UNI_EVENT_TYPE;

typedef struct {
  char *asr;
  char *cmd;
  char *pcm;
}uni_lasr_result_t;

#ifdef __cplusplus
}
#endif
#endif  //  HAL_INC_UNI_TYPES_H_
