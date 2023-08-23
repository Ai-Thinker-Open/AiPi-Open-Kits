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
 * Description : uni_media_player.h
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.11.28
 *
 **************************************************************************/
#ifndef SDK_PLAYER_INC_UNI_MEDIA_PLAYER_H_
#define SDK_PLAYER_INC_UNI_MEDIA_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_pcm_resource.h"

#define MEDIA_VOLUME_MIN        (0)
#define MEDIA_NORMAL_VOLUME_MIN (5)
#define MEDIA_VOLUME_MAX        (100)
#define MEDIA_VOLUME_MID        ((MEDIA_VOLUME_MIN + MEDIA_VOLUME_MAX) / 2)
#define MEDIA_VOLUME_DEFAULT    MEDIA_VOLUME_MID

typedef enum {
  PLAYER_PCM = 0,
  PLAYER_CNT,
} MediaPlayerType;

typedef struct {
  uni_bool enable_volume_change_fade;
} MediaPlayerparam;

Result MediaPlayerInit(MediaPlayerparam *param);
Result MediaPlayerFinal(void);
Result MediaPlayerStart(MediaPlayerType type, const char *filename);
Result MediaPlayerStop(MediaPlayerType type);
Result MediaPlayerPause(MediaPlayerType type);
Result MediaPlayerResume(MediaPlayerType type);
Result MediaPlayerPlayPrevious(MediaPlayerType type);
Result MediaPlayerPlayNext(MediaPlayerType type);
Result MediaPlayerVolumeSet(uni_s32 volume);
Result MediaPlayerVolumeInc(void);
Result MediaPlayerVolumeDec(void);
uni_s32 MediaPlayerVolumeGet(void);
uni_bool MediaPlayerIsPause(MediaPlayerType type);
uni_bool MediaPlayerIsPlaying(MediaPlayerType type);
uni_bool MediaPlayerIsActive(void);
Result MediaPlayerSetMute(uni_bool is_mute);
uni_bool MediaPlayerIsMute(void);
Result MediaPlayerShutUp(uni_bool is_shutup);
uni_bool MediaPlayerIsShutUp(void);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_PLAYER_INC_UNI_MEDIA_PLAYER_H_
