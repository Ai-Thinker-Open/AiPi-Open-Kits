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
 * Description : Uniapp recog_service.h
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.20
 *
 **************************************************************************/

#ifndef SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_SERVICE_H_
#define SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_recog_common.h"

typedef enum {
  FVM_MODE_WAKEUP = 0,
  FVM_MODE_ASR    = 1,
} FvmMode;

/**
 * Usage:   Initialize Recognization
 * Params:
 * Return:  Result of initialization
 */
Result RecogInit(void);

/**
 * Usage:   Finalize Recognization
 * Params:
 * Return:  Result of Finalization
 */
Result RecogFinal(void);

/**
 * Usage:   Stop the recognition
 * Params:
 * Return:  Result of stopping recognition
 *          (1) E_OK:       stop the recognition successfully
 *          (2) E_FAILED:   fail to stop the recognition
 */
Result RecogStop(void);

/**
 * Usage:   Cancel the recognition
 * Params:
 * Return:  Cancellation result of recognition
 *          (1) E_OK:       stop the recognition successfully
 *          (2) E_FAILED:   fail to stop the recognition
 */
Result RecogCancel(void);

/**
 * Usage:   Launch another recognition
 * Params:
 * Return:  Result of launch
 *          (1) E_OK:       launch the recognition successfully
 *          (2) E_FAILED:   fail to launch the recognition
 */
Result RecogLaunch(Scene *scene);

/**
 * Usage:   Mute the recognition
 * Params:  is_mute:  true: mute, false: unmute
 * Return:  Result of mute
 *          (1) E_OK:       launch the recognition successfully
 *          (2) E_FAILED:   fail to launch the recognition
 */
Result RecogMute(uni_bool is_mute);

/**
 * Usage:   Mute the recognition
 * Params:  word:           recognition word string
 *          is_append:      true: append to blacklist, false: remove from blacklist
 * Return:  Result of mute
 *          (1) E_OK:       launch the recognition successfully
 *          (2) E_FAILED:   fail to launch the recognition
 */
Result RecogBlacklist(const char *word, uni_bool is_append);

/**
 * Usage:   Enabel or disable the recognition timeout (got to sleep mode)
 * Params:  is_enable:  true: enable, false: disable
 * Return:  Result of enable
 *          (1) E_OK:       enable or disable the timeout successfully
 *          (2) E_FAILED:   fail to set
 */
Result RecogEnableTimeout(uni_bool is_enable);

void RecogRegisterBeforeStart(EngineCallback cb);
void RecogRegisterAfterStart(EngineCallback cb);
void RecogRegisterBeforeCancel(EngineCallback cb);
void RecogRegisterAfterCancel(EngineCallback cb);
void RecogRegisterBeforeStop(EngineCallback cb);
void RecogRegisterAfterStop(EngineCallback cb);

#ifdef __cplusplus
}
#endif
#endif  // SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_SERVICE_H_
