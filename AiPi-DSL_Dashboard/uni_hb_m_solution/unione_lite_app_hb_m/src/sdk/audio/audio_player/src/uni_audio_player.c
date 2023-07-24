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
 * Description : uni_audio_player.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/

#include "uni_audio_player.h"
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_event.h"
#include "uni_event_route.h"
#include "uni_arpt.h"
#include "uni_hal_audio.h"

#define AUDIO_PLAYER_TAG          "audio_player"
#define PCM_FRAME_MS              16  // 16ms one frame
#define PCM_BUF_FRAMES            4   // switch source will delay PCM_BUF_FRAMES * PCM_FRAME_MS ms

static struct {
  DataBufHandle      databuf_handle;
  uni_s32            pcm_frame_size;
  uni_s32            sample_rate;
  char               *pcm_buf;
  uni_bool           audio_player_playing;
  AudioPlayerInputCb input_handler;
  uni_pthread_t      pid;
  uni_bool           running;
  uni_sem_t          wait_start;
  uni_sem_t          wait_stop;
  uni_sem_t          sem_thread_exit_sync;
  uni_mutex_t        mutex;
  uni_s32            volume;
  uni_bool           is_playing;
  uni_bool           init;
  uni_bool           is_mute;
  uni_bool           read_end;
} g_audio_player = {0};

uint32_t AudioDACBuf[DAC_BUF_LEN] = {0}; //1024 * 4 = 4K
uint32_t AudioDACBuf_size = sizeof(AudioDACBuf);

static void _report_audio_end_event(void) {
  Event *event = NULL;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT,
                      ID(AUDIO_PLAY_END_EVENT),
                      NULL, NULL);
  LOGT(AUDIO_PLAYER_TAG, "report [%s]", "AUDIO_PLAY_END_EVENT");
  EventRouteProcess(event);
  EventFree(event);
}

static void _get_enough_source_data(void) {
  uni_s32 free_len;
  uni_s32 ret = 0;

  if ((!g_audio_player.audio_player_playing)
    || (NULL == g_audio_player.input_handler)) {
    return;
  }
  uni_pthread_mutex_lock(g_audio_player.mutex);
  free_len = DataBufferGetFreeSize(g_audio_player.databuf_handle);
  if (free_len < g_audio_player.pcm_frame_size) {
    uni_pthread_mutex_unlock(g_audio_player.mutex);
    uni_msleep(5);
    return;
  }
  ret = g_audio_player.input_handler(g_audio_player.databuf_handle);
  if (AUDIO_RETRIEVE_DATA_FINISHED == ret) {
    LOGT(AUDIO_PLAYER_TAG, "read PCM AUDIO_RETRIEVE_DATA_FINISHED");
    g_audio_player.audio_player_playing = false;
    g_audio_player.read_end = true;
  } else if (ret <= 0) {
    /* wait is there is no more data */
    uni_msleep(5);
  }
  uni_pthread_mutex_unlock(g_audio_player.mutex);
}

static void _volume_process(char *buf, int len) {
  int i = 0;
  int sample_len = 0;
  uni_s16 *samples = NULL;
  sample_len = len / sizeof(uni_s16);
  samples = (uni_s16 *)buf;
  for (i = 0; i < sample_len; i++) {
    samples[i] = (uni_s16)((int)samples[i] * g_audio_player.volume \
                                           / (VOLUME_MAX - VOLUME_MIN));
  }
}

static int _feed_buffer(void) {
  char *buf = g_audio_player.pcm_buf;
  int len = DataBufferGetDataSize(g_audio_player.databuf_handle);
  if (len < g_audio_player.pcm_frame_size) {
    return 0;
  }
  len = DataBufferRead(buf, g_audio_player.pcm_frame_size,
                       g_audio_player.databuf_handle);
  _volume_process(buf, len);
  /* inject PCM to player */
  while (uni_hal_play_spacelenget(DAC1) < (len/2)) {
    uni_msleep(1);
  }
  uni_hal_play(DAC1, buf, len/2);
  return len;
}

static uni_bool _is_all_player_stopped(void) {
  uni_bool ret;
  uni_pthread_mutex_lock(g_audio_player.mutex);
  ret = !g_audio_player.audio_player_playing;
  uni_pthread_mutex_unlock(g_audio_player.mutex);
  return ret;
}

static void _flush_feed_buffer(void) {
  while (_feed_buffer() > 0);
  while (uni_hal_play_spacelenget(DAC1) < 511 * 2) {
    uni_msleep(1);
  }
}

#define AUDIO_ALL_STOPPED   (1)
static uni_s32 _audio_write(void) {
  _get_enough_source_data();
  _feed_buffer();
  if (_is_all_player_stopped()) {
    _flush_feed_buffer();
    LOGT(AUDIO_PLAYER_TAG, "_is_all_player_stopped");
    return AUDIO_ALL_STOPPED;
  }
  return 0;
}

static void _player_start_internal(void) {
  /* ARPT log */
  ArptWrite("TTS START\n", false);
  /* clear databuffer */
  DataBufferClear(g_audio_player.databuf_handle);
  LOGT(AUDIO_PLAYER_TAG, "start mute: %d", g_audio_player.is_mute);
}

static void _player_stop_internal(void) {
  DataBufferClear(g_audio_player.databuf_handle);
  /* ARPT log */
  ArptWrite("TTS END\n", false);
}

static void _set_audio_playing_state(uni_bool is_playing) {
  uni_pthread_mutex_lock(g_audio_player.mutex);
  g_audio_player.is_playing = is_playing;
  uni_pthread_mutex_unlock(g_audio_player.mutex);
  LOGT(AUDIO_PLAYER_TAG, "is_playing=%d", is_playing);
}

static void _reset_sem(void) {
  uni_pthread_mutex_lock(g_audio_player.mutex);
  if (g_audio_player.audio_player_playing) {
    goto L_END;
  }
  uni_sem_destroy(g_audio_player.wait_start);
  uni_sem_init(&g_audio_player.wait_start, 0);
L_END:
  uni_pthread_mutex_unlock(g_audio_player.mutex);
}

static void _audio_player_process(void *args) {
  g_audio_player.running = true;
  /* fix issue that PA mute 150ms when system reset */
  uni_msleep(150);
  while (g_audio_player.running) {
    uni_sem_wait(g_audio_player.wait_start);
    _player_start_internal();
    LOGT(AUDIO_PLAYER_TAG, "playing started!!!");
    _set_audio_playing_state(true);
    while (true) {
      if (AUDIO_ALL_STOPPED == _audio_write()) {
        if (g_audio_player.read_end) {
          _report_audio_end_event();
          g_audio_player.read_end = false;
        }
        break;
      }
    }
    _player_stop_internal();
    _set_audio_playing_state(false);
    _reset_sem();
    LOGT(AUDIO_PLAYER_TAG, "playing finished!!!");
  }
  uni_sem_post(g_audio_player.sem_thread_exit_sync);
}

static Result _audioout_init(AudioPlayerParam *param) {
  uni_hal_play_init(DAC1, 16000, NULL, 0, (void*)AudioDACBuf, sizeof(AudioDACBuf));
  uni_hal_play_vol_set(DAC1, 100, 100);
  g_audio_player.volume = VOLUME_DEFAULT;
  g_audio_player.is_mute = false;
  return E_OK;
}

static void _audioout_final(void) {
  /* TODO: deinit player */
}

static Result _create_audio_player_thread(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_HIGH;
  uni_strncpy(param.task_name, "audio_player", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&g_audio_player.pid, &param,
                              _audio_player_process, NULL)) {
    LOGE(AUDIO_PLAYER_TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(g_audio_player.pid);
  return E_OK;
}

static void _databuf_create(void) {
  uni_s32 size = g_audio_player.pcm_frame_size * PCM_BUF_FRAMES;
  g_audio_player.databuf_handle = DataBufferCreate(size);
}

static void _mutex_init(void) {
  uni_pthread_mutex_init(&g_audio_player.mutex);
}

static void _sem_init(void) {
  uni_sem_init(&g_audio_player.wait_start, 0);
  uni_sem_init(&g_audio_player.wait_stop, 0);
  uni_sem_init(&g_audio_player.sem_thread_exit_sync, 0);
}

static void _databuf_destroy(void) {
  DataBufferDestroy(g_audio_player.databuf_handle);
}

static void _mutex_destroy(void) {
  uni_pthread_mutex_destroy(g_audio_player.mutex);
}

static void _sem_destroy(void) {
  uni_sem_destroy(g_audio_player.wait_start);
  uni_sem_destroy(g_audio_player.wait_stop);
  uni_sem_destroy(g_audio_player.sem_thread_exit_sync);
}

Result AudioPlayerInit(AudioPlayerParam *param) {
  if (g_audio_player.init || NULL == param) {
    return E_FAILED;
  }
  uni_memset(&g_audio_player, 0, sizeof(g_audio_player));
  g_audio_player.sample_rate = param->rate;
  g_audio_player.pcm_frame_size = PCM_FRAME_MS * param->rate * (param->bit / 8)
                                  * param->channels / 1000;
  g_audio_player.pcm_buf = (char *)uni_malloc(g_audio_player.pcm_frame_size);
  if (NULL == g_audio_player.pcm_buf) {
    LOGE(AUDIO_PLAYER_TAG, "malloc failed !");
    return E_FAILED;
  }
  _databuf_create();
  if (NULL == g_audio_player.databuf_handle) {
    LOGE(AUDIO_PLAYER_TAG, "malloc failed !");
    uni_free(g_audio_player.pcm_buf);
    return E_FAILED;
  }
  _mutex_init();
  _sem_init();
  if (E_OK != _audioout_init(param)) {
    LOGE(AUDIO_PLAYER_TAG, "_audioout_init failed !");
    goto L_AUDIOOUT_INIT_FAILED;
  }
  if (E_OK != _create_audio_player_thread()) {
    LOGE(AUDIO_PLAYER_TAG, "_create_audio_player_thread failed !");
    goto L_PTHREAD_CREATE_FAILED;
  }
  g_audio_player.init = true;
  return E_OK;
L_PTHREAD_CREATE_FAILED:
  _databuf_destroy();
  uni_free(g_audio_player.pcm_buf);
  g_audio_player.init = false;
  _audioout_final();
L_AUDIOOUT_INIT_FAILED:
  _databuf_destroy();
  _mutex_destroy();
  _sem_destroy();
  uni_free(g_audio_player.pcm_buf);
  return E_FAILED;
}

static void _worker_thread_exit(void) {
  g_audio_player.running = false;
  uni_sem_post(g_audio_player.wait_start);
  uni_sem_wait(g_audio_player.sem_thread_exit_sync);
}

static void _destroy_all(void) {
  _databuf_destroy();
  _mutex_destroy();
  _sem_destroy();
  _audioout_final();
}

void AudioPlayerFinal(void) {
  _worker_thread_exit();
  _destroy_all();
  uni_free(g_audio_player.pcm_buf);
}

static uni_s32 _clear_audio_end_event(Event *event) {
  if (event->type == ID(AUDIO_PLAY_END_EVENT)) {
    LOGW(AUDIO_PLAYER_TAG, "event (%d) is disabled by audio_player",
         event->type);
    event->type = INVALID_EVENT_ID_EVENT_INVALID;
  }
  return 0;
}

static void _clear_history_audio_event(void) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.info = _clear_audio_end_event;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(SESSION_MGR_REVIEW_EVENT),
                      &event_content, NULL);
  EventRouteProcess(event);
  EventFree(event);
}

Result AudioPlayerStart(AudioPlayerInputCb input_handler) {
  LOGT(AUDIO_PLAYER_TAG, "AudioPlayerStart");
  _clear_history_audio_event();
  uni_pthread_mutex_lock(g_audio_player.mutex);
  g_audio_player.input_handler = input_handler;
  g_audio_player.audio_player_playing = true;
  g_audio_player.read_end = false;
  uni_sem_post(g_audio_player.wait_start);
  uni_pthread_mutex_unlock(g_audio_player.mutex);
  return E_OK;
}

Result AudioPlayerStop(void) {
  LOGT(AUDIO_PLAYER_TAG, "AudioPlayerStop");
  uni_pthread_mutex_lock(g_audio_player.mutex);
  g_audio_player.audio_player_playing = false;
  uni_pthread_mutex_unlock(g_audio_player.mutex);
  while (AudioPlayerIsActive()) {
    uni_msleep(10);
  }
  return E_OK;
}

uni_bool AudioPlayerIsActive(void) {
  uni_bool active = false;
  if (g_audio_player.init) {
    uni_pthread_mutex_lock(g_audio_player.mutex);
    active = g_audio_player.is_playing;
    uni_pthread_mutex_unlock(g_audio_player.mutex);
  }
  return active;
}

Result AudioVolumeSet(uni_s32 vol) {
  if (vol > VOLUME_MAX || vol < VOLUME_MIN) {
    LOGW(AUDIO_PLAYER_TAG, "set volume %d rejected", vol);
    return E_REJECT;
  }
  /* TODO: set audio decoder volume */
  g_audio_player.volume = vol;
  LOGT(AUDIO_PLAYER_TAG, "set volume to %d", g_audio_player.volume);
  return E_OK;
}

uni_s32 AudioVolumeGet(void) {
  return g_audio_player.volume;
}

Result AudioVolumeSetMute(uni_bool is_mute) {
  if (is_mute == g_audio_player.is_mute) {
    return E_OK;
  }
  if (is_mute) {
    /* TODO: player mute */
    uni_hal_play_mute(DAC1, true, true);
  } else {
    uni_hal_play_mute(DAC1, false, false);
  }
  g_audio_player.is_mute = is_mute;
  return E_OK;
}

uni_bool AudioVolumeGetMute(void) {
  return g_audio_player.is_mute;
}

