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
 * Description : uni_audio_player.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef SDK_AUDIO_AUDIO_PLAYER_INC_UNI_AUDIO_PLAYER_H_
#define SDK_AUDIO_AUDIO_PLAYER_INC_UNI_AUDIO_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_databuf.h"
#include "uni_audio_common.h"

/************************ 1. play related *************************/

#define AUDIO_RETRIEVE_DATA_FINISHED             (-1)

typedef AudioParam AudioPlayerParam;

/**
 * Usage:   Callback function used by audio player to retrieve data from
 *          the callers, such as mp3/pcm/tts player. It the callers'
 *          responsability to get the data ready in the callback
 * Params:  data_buf    data buffer for audio data input
 * Return:  size of the data retrieved, -1 means play finished
 */
typedef uni_s32 (* AudioPlayerInputCb)(DataBufHandle data_buf);

/**
 * Usage:   Audio player initializer
 * Params:  Parameter of audio out
 * Return:  Result of initialization
 */
Result AudioPlayerInit(AudioPlayerParam *param);

/**
 * Usage:   Audio player finalizer
 * Params:
 * Return:
 */
void AudioPlayerFinal(void);

/**
 * Usage:   Start audio player
 * Params:  input_handler callback defined by caller to provide audio data
 * Return:  E_OK       start succeeded
 *          E_FAILED   start failed
 */
Result AudioPlayerStart(AudioPlayerInputCb intput_handler);

/**
 * Usage:   Stop audio player
 * Params:
 * Return:  E_OK       stop succeeded
 *          E_FAILED   stop failed
 */
Result AudioPlayerStop(void);

/**
 * Usage:   Check if audio player is working
 * Params:
 * Return:  true for active, false for idle
 */
uni_bool AudioPlayerIsActive(void);


/************************ 2. volume related *************************/

#define VOLUME_MAX      100
#define VOLUME_MIN      0
#define VOLUME_MID      ((VOLUME_MAX + VOLUME_MIN) / 2)
#define VOLUME_DEFAULT  VOLUME_MID

/**
 * Usage:   Set audio out volume
 * Params:  vol    volume value to be set
 * Return:  Could be
 *          E_OK:     set secceeded
 *          E_FAILED: set failed
 *          E_REJECT: invalid parameter
 */
Result AudioVolumeSet(uni_s32 vol);

/**
 * Usage:   Get audio out volume
 * Params:
 * Return:  the value of current volume, which ranges between VOLUME_MIN to
 *          VOLUME_MAX
 */
uni_s32 AudioVolumeGet(void);

/**
 * Usage:   Set audio out mute or not
 * Params:  is_mute:    ture - mute; false - unmute
 * Return:  Could be
 *          E_OK:     set secceeded
 *          E_FAILED: set failed
 *          E_REJECT: invalid parameter
 */
Result AudioVolumeSetMute(uni_bool is_mute);

/**
 * Usage:   Get audio out mute or not
 * Params:
 * Return:  true:     mute
 *          false:    unmute
 */
uni_bool AudioVolumeGetMute(void);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_AUDIO_AUDIO_PLAYER_INC_UNI_AUDIO_PLAYER_H_
