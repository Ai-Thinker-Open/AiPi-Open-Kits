/**************************************************************************
 * Copyright (C) 2019-2019  Unisound
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
 * Description : idle_detect.h
 * Author      : guozhenkun.unisound.com
 * Date        : 2019.01.29
 *
 **************************************************************************/
#include "idle_detect.h"
#include "uni_log.h"
#include "uni_timer.h"
#include "list_head.h"
#include "uni_media_player.h"

#define TAG     "idle_detect"

typedef struct IdleDetectInternalCallback {
  IdleType           last_status;
  IdleDetectCallback func;
  uni_u32            wait_idle_sec;
  uni_list_head      link;
} IdleDetectInternalCallback;

typedef struct IdleDetectService {
  uni_list_head cb_list;
  uni_mutex_t   cb_list_lock;
  unsigned long access_time;
  uni_mutex_t   access_time_lock;
  TimerHandle   clock;
} IdleDetectService;

static IdleDetectService g_service;

static uni_u32 _get_now_sec(void) {
  return uni_get_clock_time_sec();
}

static int _clock_callback(void *arg) {
  uni_u32 idle_time = 0;
  IdleDetectInternalCallback *cb;
  if (MediaPlayerIsActive()) {
    IdleDetectServiceAccess();
  }
  idle_time = _get_now_sec() - g_service.access_time;
  uni_pthread_mutex_lock(g_service.cb_list_lock);
  uni_list_for_each_entry(cb, &g_service.cb_list, IdleDetectInternalCallback, link) {
    if (idle_time >= cb->wait_idle_sec && IDLE_DETECT_TYPE_IDLE != cb->last_status) {
      cb->func(IDLE_DETECT_TYPE_IDLE);
      cb->last_status = IDLE_DETECT_TYPE_IDLE;
    }
    if (idle_time < cb->wait_idle_sec && IDLE_DETECT_TYPE_BUSY != cb->last_status) {
      cb->func(IDLE_DETECT_TYPE_BUSY);
      cb->last_status = IDLE_DETECT_TYPE_BUSY;
    }
  }
  uni_pthread_mutex_unlock(g_service.cb_list_lock);
  return 0;
}

static void _init_cb_list(void) {
  uni_list_init(&g_service.cb_list);
  uni_pthread_mutex_init(&g_service.cb_list_lock);
}

static void _init_access_time(void) {
  g_service.access_time = _get_now_sec();
  uni_pthread_mutex_init(&g_service.access_time_lock);
}

static void _init_clock(void) {
  g_service.clock = TimerStart(10 * 1000, TIMER_TYPE_PERIODICAL, _clock_callback, NULL);
}

static void _final_clock(void) {
  TimerStop(g_service.clock);
}

static void _final_cb_list(void) {
  IdleDetectInternalCallback *item, *tmp;
  uni_pthread_mutex_lock(g_service.cb_list_lock);
  uni_list_for_each_entry_safe(item, tmp, &g_service.cb_list, IdleDetectInternalCallback, link) {
    uni_list_del(&item->link);
    uni_free(item);
  }
  uni_pthread_mutex_unlock(g_service.cb_list_lock);
  uni_pthread_mutex_destroy(g_service.cb_list_lock);
}

static void _final_access_time(void) {
  uni_pthread_mutex_destroy(g_service.access_time_lock);
}

Result IdleDetectServiceInit(void) {
  LOGD(TAG, "IdleDetectServiceInit");
  _init_cb_list();
  LOGD(TAG, "_init_cb_list");
  _init_access_time();
  LOGD(TAG, "_init_access_time");
  _init_clock();
  LOGD(TAG, "_init_clock");
  return E_OK;
}
Result IdleDetectServiceFinal(void) {
  _final_clock();
  _final_cb_list();
  _final_access_time();
  return E_OK;
}

void IdleDetectServiceRegister(IdleDetectCallback callback, uni_u32 wait_idle_sec) {
  IdleDetectInternalCallback *cb = NULL;
  if (NULL == callback) {
    return;
  }
  if (NULL == (cb = uni_malloc(sizeof(IdleDetectInternalCallback)))) {
    LOGE(TAG, "malloc failed !");
    return;
  }
  uni_memset(cb, 0, sizeof(IdleDetectInternalCallback));
  cb->last_status = IDLE_DETECT_TYPE_UNKONWN;
  cb->func = callback;
  cb->wait_idle_sec = wait_idle_sec;
  uni_pthread_mutex_lock(g_service.cb_list_lock);
  uni_list_add_tail(&cb->link, &g_service.cb_list);
  uni_pthread_mutex_unlock(g_service.cb_list_lock);
}

void IdleDetectServiceAccess(void) {
  uni_pthread_mutex_lock(g_service.access_time_lock);
  g_service.access_time = _get_now_sec();
  uni_pthread_mutex_unlock(g_service.access_time_lock);
}
