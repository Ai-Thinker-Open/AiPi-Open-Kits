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
 * Description : uni_media_player.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.11.28
 *
 **************************************************************************/
#include "uni_media_player.h"

#include "uni_log.h"
#include "uni_audio_player.h"
#include "uni_pcm_player.h"
#include "uni_black_board.h"
#include "uni_timer.h"
#include "idle_detect.h"

#define MEDIA_PLAYER_TAG           "mediaplayer"
#define MEDIA_VOLUME_STEP          (25)

/*5 25 50 75 100*/
#define MEDIA_VOLUME_MID_LOW (25)

typedef struct {
  MediaPlayerparam param;
  uni_s32          volume;
  uni_bool         is_shutup;
  uni_mutex_t      volume_mutex;
} MediaPlayer;

static MediaPlayer g_mediaplayer;

static Result _init_player_audio(void) {
  AudioPlayerParam param;
  param.channels = 1;
  param.rate = 16000;
  param.bit = 16;
  if (E_OK != AudioPlayerInit(&param)) {
    LOGE(MEDIA_PLAYER_TAG, "audio play init failed");
    return E_FAILED;
  }
  return E_OK;
}

static void _final_player_audio(void) {
  AudioPlayerFinal();
}

static Result _register_player_pcm(void) {
  if (E_OK != PcmInit()) {
    LOGE(MEDIA_PLAYER_TAG, "pcm init failed");
    return E_FAILED;
  }
  return E_OK;
}

static void _unregister_player_pcm(void) {
  PcmFinal();
}

static void _unregister_all_player(void) {
  _unregister_player_pcm();
}

static Result _register_all_player(void) {
  if (E_OK != _register_player_pcm()) {
    return E_FAILED;
  }
  LOGT(MEDIA_PLAYER_TAG, "register all player success");
  return E_OK;
}

static void _set_volume(uni_s32 volume) {
  uni_s32 vol = 0;
  g_mediaplayer.volume = volume;
  vol = g_mediaplayer.volume * (VOLUME_MAX - VOLUME_MIN) / MEDIA_VOLUME_MAX;
  AudioVolumeSet(vol);
}

static void _mutex_init(void) {
  uni_pthread_mutex_init(&g_mediaplayer.volume_mutex);
}

static void _mutex_destroy(void) {
  uni_pthread_mutex_destroy(g_mediaplayer.volume_mutex);
}

Result MediaPlayerInit(MediaPlayerparam *param) {
  if (NULL == param) {
    LOGE(MEDIA_PLAYER_TAG, "param null");
    return E_FAILED;
  }
  uni_memset(&g_mediaplayer, 0, sizeof(MediaPlayer));
  uni_memcpy(&g_mediaplayer.param, param, sizeof(MediaPlayerparam));
  g_mediaplayer.is_shutup = false;
  _mutex_init();
  if (E_OK != _init_player_audio()) {
    LOGE(MEDIA_PLAYER_TAG, "_init_player_audio failed !");
    _mutex_destroy();
    return E_FAILED;
  }
  _set_volume(MEDIA_VOLUME_DEFAULT);
  if (E_OK != _register_all_player()) {
    LOGE(MEDIA_PLAYER_TAG, "_register_all_player failed !");
    _final_player_audio();
    _mutex_destroy();
    return E_FAILED;
  }
  LOGT(MEDIA_PLAYER_TAG, "mediaplayer init success");
  return E_OK;
}

Result MediaPlayerFinal(void) {
  _mutex_destroy();
  _unregister_all_player();
  _final_player_audio();
  return E_OK;
}

Result MediaPlayerStart(MediaPlayerType type, const char *filename) {
  LOGD(MEDIA_PLAYER_TAG, "MediaPlayerStart");
  IdleDetectServiceAccess();
  LOGD(MEDIA_PLAYER_TAG, "IdleDetectServiceAccess");
  if (g_mediaplayer.is_shutup) {
    LOGD(MEDIA_PLAYER_TAG, "Media player is shut up now.");
    return E_FAILED;
  }
  switch (type) {
    case PLAYER_PCM:
      PcmStop();
      return PcmPlay(filename);
    default:
      break;
  }
  return E_FAILED;
}

Result MediaPlayerStop(MediaPlayerType type) {
  IdleDetectServiceAccess();
  switch (type) {
    case PLAYER_PCM:
      PcmStop();
      break;
    default:
      break;
  }
  return E_OK;
}

Result MediaPlayerPause(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      break;
    default:
      break;
  }
  return E_OK;
}

Result MediaPlayerResume(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      break;
    default:
      break;
  }
  return E_OK;
}

uni_bool MediaPlayerIsPause(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      break;
    default:
      break;
  }
  return false;
}

uni_bool MediaPlayerIsPlaying(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      return PcmCheckIsPlaying();
      break;
    default:
      break;
  }
  return false;
}

Result MediaPlayerPlayPrevious(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      break;
    default:
      break;
  }
  return E_OK;
}

Result MediaPlayerPlayNext(MediaPlayerType type) {
  switch (type) {
    case PLAYER_PCM:
      break;
    default:
      break;
  }
  return E_OK;
}

Result MediaPlayerVolumeSet(uni_s32 volume) {
  uni_pthread_mutex_lock(g_mediaplayer.volume_mutex);
  if (volume < MEDIA_VOLUME_MIN || MEDIA_VOLUME_MAX < volume) {
    LOGE(MEDIA_PLAYER_TAG, "volume[%d] invalid", volume);
    uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
    return E_FAILED;
  }
  _set_volume(volume);
  uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
  return E_OK;
}

uni_s32 MediaPlayerVolumeGet(void) {
  uni_s32 volume;
  uni_pthread_mutex_lock(g_mediaplayer.volume_mutex);
  volume = g_mediaplayer.volume;
  uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
  return volume;
}

Result MediaPlayerVolumeInc(void) {
  uni_pthread_mutex_lock(g_mediaplayer.volume_mutex);
  uni_s32 volume;
  if (g_mediaplayer.volume >= MEDIA_VOLUME_MAX) {
    LOGE(MEDIA_PLAYER_TAG, "volume already is max, cannot increase");
    uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
    return E_REJECT;
  }
  if (MEDIA_NORMAL_VOLUME_MIN == g_mediaplayer.volume) {
    volume = MEDIA_VOLUME_MID_LOW;
  } else {
    volume = uni_min(MEDIA_VOLUME_MAX, g_mediaplayer.volume + MEDIA_VOLUME_STEP);
  }
  _set_volume(volume);
  uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
  return E_OK;
}

Result MediaPlayerVolumeDec(void) {
  uni_pthread_mutex_lock(g_mediaplayer.volume_mutex);
  uni_s32 volume;
  if (g_mediaplayer.volume <= MEDIA_VOLUME_MIN) {
    LOGE(MEDIA_PLAYER_TAG, "volume already is min, cannot decrease");
    uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
    return E_REJECT;
  }
  volume = uni_max(MEDIA_NORMAL_VOLUME_MIN,
                   g_mediaplayer.volume - MEDIA_VOLUME_STEP);
  _set_volume(volume);
  uni_pthread_mutex_unlock(g_mediaplayer.volume_mutex);
  return E_OK;
}

uni_bool MediaPlayerIsActive(void) {
  return AudioPlayerIsActive();
}

Result MediaPlayerSetMute(uni_bool is_mute) {
  return AudioVolumeSetMute(is_mute);
}

uni_bool MediaPlayerIsMute(void) {
  return AudioVolumeGetMute();
}

Result MediaPlayerShutUp(uni_bool is_shutup) {
  g_mediaplayer.is_shutup = is_shutup;
  return E_OK;
}

uni_bool MediaPlayerIsShutUp(void) {
  return g_mediaplayer.is_shutup;
}

