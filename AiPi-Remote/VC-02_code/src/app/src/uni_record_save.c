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
 * Description : uni_record_save.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.12.23
 *
 **************************************************************************/
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_databuf.h"
#include "uni_config.h"

#define TAG "record"

#define RECORD_FRAMS_NUM  (5 * 60 * 60 * 1000 / 16)
#define RECORD_BUF_SIZE       (6 * 1024)
#define RECORD_RINGBUF_SIZE   (RECORD_BUF_SIZE * 10)

#if UNI_RECORD_SAVE_ENABLE

static DataBufHandle g_record_df = NULL;
static char *g_record_buf = NULL;

static int g_file_num = 1;
static int g_record_num = 0;
static int g_frame_size = 0;
static uni_pthread_t g_save_pid = NULL;

static void _get_file_num(void) {
  char file_name[32] = {0};
  while (1) {
    uni_snprintf(file_name, sizeof(file_name), "0:/%d.pcm", g_file_num);
    /* TODO: check file is exists or not */
    if (1) {
      break;
    } else {
      g_file_num++;
    }
  }
}

static int _open_new_file(void) {
  char file_name[32] = {0};
  int res = 1;
  
  uni_snprintf(file_name, sizeof(file_name), "0:/%d.pcm", g_file_num);
  /* TODO: open fila */
  if(res != 0) {
    LOGE(TAG, "WRITE_TEST open file error : %d\r\n",res);
    return -1;
  }
  return 0;
}

static int _sd_file_init(void)
{
  _get_file_num();
  return _open_new_file();
}

static void _sd_save_process(void *args) {
  int size = 0;
  while (1) {
    if (g_record_num > RECORD_FRAMS_NUM) {
      uni_msleep(5);
      continue;
    }
    size = DataBufferGetDataSize(g_record_df);
    if (size < RECORD_BUF_SIZE) {
      uni_msleep(5);
      continue;
    }
    if (size > RECORD_BUF_SIZE) {
      size = RECORD_BUF_SIZE;
    }
    DataBufferRead(g_record_buf, size, g_record_df);
    /* TODO: write to file */
    g_record_num += (size / g_frame_size);
    if (g_record_num > RECORD_FRAMS_NUM) {
      /* TODO: close file */
      g_file_num++;
      if (0 == _open_new_file()) {
        g_record_num = 0;
      }
    }
  }
  /* will destroy self, cannot do it in RecordSaveFinal */
  g_save_pid = NULL;
}

static Result _create_sd_save_thread(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_NORMAL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "sd_save", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&g_save_pid, &param, _sd_save_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(g_save_pid);
  return E_OK;
}

Result RecordSaveInit(void) {
  uni_s64 tmp = 0;
  if (0 != ConfigReadItemNumber("record.enable", &tmp)) {
    LOGT(TAG, "cannot found record.enable, donnot save record.");
    return E_OK;
  }
  if (tmp) {
    if (0 != _sd_file_init()) {
      return E_FAILED;
    }
    LOGT(TAG, "_sd_file_init OK!");
    g_record_df = DataBufferCreate(RECORD_RINGBUF_SIZE);
    if (NULL == g_record_df) {
      LOGE(TAG, "malloc failed\n");
      return E_FAILED;
    }
    g_record_buf = (char *)uni_malloc(RECORD_BUF_SIZE);
    if (NULL == g_record_buf) {
      LOGE(TAG, "malloc failed\n");
      DataBufferDestroy(g_record_df);
      return E_FAILED;
    }
    g_frame_size = 16 * 16 * 2 * 4;
    if (0 == ConfigReadItemNumber("record.save_asr_only", &tmp)) {
      if (tmp) {
        g_frame_size = 16 * 16 * 2;
      }
    }
    if (E_OK != _create_sd_save_thread()) {
      LOGE(TAG, "_create_sd_save_thread failed !");
      DataBufferDestroy(g_record_df);
      uni_free(g_record_buf);
      return E_FAILED;
    }
  }
  return E_OK;
}

Result RecordSaveFinal(void) {
  if (g_save_pid) {
    uni_pthread_destroy(g_save_pid);
  }
  if (g_record_df) {
    DataBufferDestroy(g_record_df);
  }
  if (g_record_buf) {
    uni_free(g_record_buf);
  }
  return E_OK;
}

#endif

