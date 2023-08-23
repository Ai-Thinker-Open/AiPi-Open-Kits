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
 * Description : uni_pcm_player.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_pcm_player.h"
#include "uni_audio_player.h"
#include "uni_pcm_resource.h"
#include "uni_log.h"
#include "pcm_bin.h"
#include "uni_hal_decoder.h"
#include "uni_pcm_default.h"

#define TAG  "adpcm_player"

#define COMMPRESSION_RATIO        (4)
#define ADPCM_BUFFER_SIZE         (512 * 2)

#define ERROR_MAX                 3
#define DECODER_BUF_SIZE          (19 * 1024)

#define ENERGY_VAL_EDGE           (10)

typedef enum {
  IDLE = 0,
  RUNNING,
} AdpcmState;

typedef struct {
  char          *files;
  int           play_num;
  uni_bool      is_need_skip;
  AdpcmState    state;
  uni_u32       offset;
  uni_u32       len;
  char          *pcm_buf;
  int           pcm_len;
  uni_mutex_t   mutex;
  uni_sem_t     sem;
  int           error_cnt;
  int           pcm_data_len;
  char          *decoder_buf;
} AdpcmPlayer;

extern char g_mp3_ais_share_buf[];

static AdpcmPlayer g_adpcm_player;

static uint32_t _fill_stream_callback(void *buffer, uint32_t length) {
  int read_len = length;
  if(g_adpcm_player.len <= 0){
    g_adpcm_player.error_cnt = ERROR_MAX;
    return 0;
  }
  if (read_len > g_adpcm_player.len) {
    read_len = g_adpcm_player.len;
  }
  uni_memcpy(buffer, &pcm_bin[g_adpcm_player.offset], read_len);
  g_adpcm_player.len -= read_len;
  g_adpcm_player.offset += read_len;
  return read_len;
}

static void _set_state(AdpcmState state) {
  LOGT(TAG, "set adpcm state:%d.", state);
  g_adpcm_player.state = state;
}

static AdpcmState _get_state(void) {
  return g_adpcm_player.state;
}

static Result _adpcm_play_internal(const char *files) {
  int ret = 0;
  uni_song_info song_info;
  if (NULL == files) {
    LOGE(TAG, "files is NULL.");
    return E_FAILED;
  }
  /* get PCM number of resource */
  ret = PcmGetResourceInfo(files, &g_adpcm_player.offset, &g_adpcm_player.len);
  if (ret < 0) {
    LOGE(TAG, "cannot find PCM file: %s", files);
    return E_FAILED;
  }
  LOGT(TAG, "fetch PCM file: %s offset: %x len: %x\n",
            files, g_adpcm_player.offset, g_adpcm_player.len);
  ret = uni_hal_decoder_init(g_adpcm_player.decoder_buf, _fill_stream_callback);
  if (0 != ret) {
    LOGE(TAG, "audio_decoder_initialize failed.");
    return E_FAILED;
  }
  uni_hal_decoder_song_info(&song_info);
  LOGT(TAG, "[SONG_INFO]: ChannelCnt : %d", (int)song_info.num_channels);
  LOGT(TAG, "[SONG_INFO]: SampleRate : %d Hz", (int)song_info.sampling_rate);
  LOGT(TAG, "[SONG_INFO]: BitRate	 : %d Kbps", (int)song_info.bitrate / 1000);
  LOGT(TAG, "[SONG_INFO]: decoder_buf addr: 0x%x", g_adpcm_player.decoder_buf);
  g_adpcm_player.error_cnt = 0;
  g_adpcm_player.pcm_data_len = 0;
  return E_OK;
}

static Result _adpcm_stop_internal(void) {
  uni_hal_decoder_deinit();
  return E_OK;
}

static char *_get_next_play_file(void) {
  char *file = NULL;
  if (NULL != uni_strchr(g_adpcm_player.files, '[')) {
    file = uni_get_number_pcm(g_adpcm_player.files, g_adpcm_player.play_num);
    if (0 == uni_strcmp(file, "-1")) {
      LOGT(TAG, "no more file to play.");
      return NULL;
    }
    g_adpcm_player.play_num++;
    g_adpcm_player.is_need_skip = true;
    return file;
  } else {
    if (g_adpcm_player.play_num > 0) {
      LOGT(TAG, "no more file to play.");
      return NULL;
    }
    g_adpcm_player.play_num++;
    g_adpcm_player.is_need_skip = true;
    return g_adpcm_player.files;
  }
}

static uni_bool _is_skip_mp3_fram(char *fram_pcm, int len) {
  short *val_p = (short *)fram_pcm;
  uni_u64 sum_val = 0;
  int sample = len / sizeof(short);
  int i = 0;
  if (g_adpcm_player.is_need_skip) {
    for (i = 0; i < sample ; i++) {
      sum_val += uni_abs(val_p[i], 0);
    }
    sum_val = sum_val / sample;
    if (sum_val < ENERGY_VAL_EDGE) {
      return true;
    } else {
      g_adpcm_player.is_need_skip = false;
      return false;
    }
  }
  return false;
}

static int _get_decode_mp3_frame(char *buf, int size) {
  int decode_size = 0;
  int i = 0;
  int frame_bytes = 0;
  uni_frame_info frame_info;
  uni_hal_decoder_frame_info(&frame_info);
  char *pcm_tmp = (char *)frame_info.pcm_addr;
  /* if pcm data buffer have data, read pcm data from buffer first */
  if (g_adpcm_player.pcm_data_len > 0) {
    decode_size = g_adpcm_player.pcm_data_len;
    if (decode_size > size) {
      decode_size = size;
    }
    uni_memcpy(buf, pcm_tmp, decode_size);
    g_adpcm_player.pcm_data_len -= decode_size;
    /* move surplus data in pcm buffer */
    for (i = 0; i < g_adpcm_player.pcm_data_len; i++) {
      pcm_tmp[i] = pcm_tmp[i + decode_size];
    }
    return decode_size;
  }
  /* decode next frame if not more data in pcm data buffer */
  if (uni_hal_decoder_can_continue()) {
    if(uni_hal_decoder_decode() >= 0) {
      uni_hal_decoder_frame_info(&frame_info);
      frame_bytes = frame_info.data_length;  // 16bit sample
      pcm_tmp = (char *)frame_info.pcm_addr;
      if (_is_skip_mp3_fram(pcm_tmp, frame_bytes)) {
        return 0;
      }
      /* copy pcm data to recive buffer */
      if (size > frame_bytes) {
        size = frame_bytes;
      }
      uni_memcpy(buf, pcm_tmp, size);
      /* move surplus data in pcm buffer */
      g_adpcm_player.pcm_data_len = frame_bytes - size;
      for (i = 0; i < g_adpcm_player.pcm_data_len; i++) {
        pcm_tmp[i] = pcm_tmp[i + size];
      }
      return size;
    } else {
      if(frame_info.error_code == -127 || frame_info.error_code == -123) {
        if(g_adpcm_player.error_cnt++ >= 3) {
          LOGE(TAG, "[INFO]: ERROR%d\n", (int)frame_info.error_code);
          _adpcm_stop_internal();
          if (E_OK != _adpcm_play_internal(_get_next_play_file())) {
            return -1;
          }
        } else {
          LOGT(TAG, "[INFO]: ERROR%d\n", (int)frame_info.error_code);
        }
      } else {
        g_adpcm_player.error_cnt = 0;
        LOGT(TAG, "[INFO]: ERROR%d\n", (int)frame_info.error_code);
      }
    }
  }else {
    LOGT(TAG, "mp3 decode finished !");
    _adpcm_stop_internal();
    if (E_OK != _adpcm_play_internal(_get_next_play_file())) {
      return -1;
    }
  }
  return 0;
}

static uni_s32 _adpcm_retrieve(DataBufHandle data_buffer) {
  uni_u32 free_size = 0;
  int write_size = 0;
  int decode_size = 0;
  free_size = DataBufferGetFreeSize(data_buffer);
  if (free_size > g_adpcm_player.pcm_len) {
    free_size = g_adpcm_player.pcm_len;
  }
  if (0 == free_size) {
    return 0;
  }
  decode_size = _get_decode_mp3_frame(g_adpcm_player.pcm_buf, free_size);
  if (decode_size < 0) {
    return AUDIO_RETRIEVE_DATA_FINISHED;
  }
  write_size = DataBufferWrite(data_buffer, g_adpcm_player.pcm_buf, decode_size);
  return write_size;
}

static int _save_play_files(const char *files) {
  if (NULL == files) {
    LOGE(TAG, "files is NULL.");
    return -1;
  }
  g_adpcm_player.files = (char *)uni_realloc(g_adpcm_player.files,
                                            uni_strlen(files) + 1);
  if (NULL == g_adpcm_player.files) {
    LOGE(TAG, "uni_realloc failed.");
    return -1;
  }
  g_adpcm_player.play_num = 0;
  uni_memcpy(g_adpcm_player.files, files, uni_strlen(files) + 1);
  return 0;
}

Result PcmPlay(const char *files) {
  uni_pthread_mutex_lock(g_adpcm_player.mutex);
  if (RUNNING == _get_state()) {
    LOGW(TAG, "adpcm state running, wrong state");
    uni_pthread_mutex_unlock(g_adpcm_player.mutex);
    return E_FAILED;
  }
  if (0 != _save_play_files(files)) {
    uni_pthread_mutex_unlock(g_adpcm_player.mutex);
    return E_FAILED;
  }
  LOGT(TAG, "Play pcm file : %s", files);
  g_adpcm_player.decoder_buf = g_mp3_ais_share_buf;
  if (E_OK != _adpcm_play_internal(_get_next_play_file())) {
    LOGW(TAG, "pcm_play_internal failed");
    uni_pthread_mutex_unlock(g_adpcm_player.mutex);
    return E_FAILED;
  }
  AudioPlayerStart(_adpcm_retrieve);
  _set_state(RUNNING);
  uni_pthread_mutex_unlock(g_adpcm_player.mutex);
  return E_OK;
}

Result PcmStop(void) {
  LOGD(TAG, "PcmStop");
  uni_pthread_mutex_lock(g_adpcm_player.mutex);
  if (RUNNING != _get_state()) {
    uni_pthread_mutex_unlock(g_adpcm_player.mutex);
    LOGW(TAG, "wrong state");
    return E_FAILED;
  }
  _set_state(IDLE);
  uni_pthread_mutex_unlock(g_adpcm_player.mutex);
  AudioPlayerStop();
  _adpcm_stop_internal();
  return E_OK;
}

uni_bool PcmCheckIsPlaying(void) {
  return (g_adpcm_player.state == RUNNING);
}

Result PcmInit(void) {
  uni_memset(&g_adpcm_player, 0, sizeof(AdpcmPlayer));
  uni_pthread_mutex_init(&g_adpcm_player.mutex);
  uni_sem_init(&g_adpcm_player.sem, 0);
  g_adpcm_player.pcm_len = ADPCM_BUFFER_SIZE;
  g_adpcm_player.pcm_buf = (char *)uni_malloc(ADPCM_BUFFER_SIZE);
  if (NULL == g_adpcm_player.pcm_buf) {
    LOGE(TAG, "malloc failed");
    uni_pthread_mutex_destroy(g_adpcm_player.mutex);
    uni_sem_destroy(g_adpcm_player.sem);
    return E_FAILED;
  }
  g_adpcm_player.files = NULL;
  g_adpcm_player.play_num = 0;
  return E_OK;
}

void PcmFinal(void) {
  uni_pthread_mutex_destroy(g_adpcm_player.mutex);
  uni_sem_destroy(g_adpcm_player.sem);
  if (g_adpcm_player.pcm_buf) {
    uni_free(g_adpcm_player.pcm_buf);
  }
  if (g_adpcm_player.files) {
    uni_free(g_adpcm_player.files);
  }
  g_adpcm_player.play_num = 0;
}

