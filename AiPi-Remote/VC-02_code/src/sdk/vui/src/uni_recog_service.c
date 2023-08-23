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
 * Description : Uniapp recog_service.c
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.20
 *
 **************************************************************************/
#include "uni_recog_service.h"

#include "uni_iot.h"
#include "uni_json.h"
#include "uni_log.h"
#include "uni_nlu.h"
#include "uni_event_route.h"
#include "uni_recog_preproc.h"
#include "uni_recog_engine_control.h"
#include "uni_recog_common.h"
#include "uni_black_board.h"
#include "idle_detect.h"
#include "uni_arpt.h"
#include "uni_config.h"

#define UNI_RECOG_SERVICE_TAG             "recog_service"

static struct {
  VuiHandle     *vui;
  Scene         *cur_scene;
  uni_s32       twice_judge_last_time;
  uni_bool      is_work;
} g_recog_service;

/**
 * Usage:   setup the scene to default value
 * Params:  scene[in]:
 * Return:
 */
static void _default_scene(Scene *scene) {
  if (NULL == scene) {
    LOGW(UNI_RECOG_SERVICE_TAG, "please initialize scene firstly.");
    return;
  }
  double tmp = 0.0;
  scene->asr_mode = UNI_LP_WAKEUP;
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.std_threshold", &tmp)) {
    scene->std_threshold = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.lp_threshold", &tmp)) {
    scene->low_threshold = tmp;
  }
  scene->timeout = 0;
  scene->offset_timems = 320;  /* 16ms * 320 = 5s */
  scene->grammar = NULL;
}

static void _set_wakeup_thresold(Scene *scene) {
  double tmp = 0.0;
  int is_hvad_wakeup = 0;
  BbRead(BB_KEY_IS_HVAD_WAKEUP, &is_hvad_wakeup);
  if (is_hvad_wakeup) {
    if (0 == ConfigReadItemDouble("lasr.lasr_wkp.sleep_std_threshold", &tmp)) {
      scene->std_threshold = tmp;
    }
    if (0 == ConfigReadItemDouble("lasr.lasr_wkp.sleep_lp_threshold", &tmp)) {
      scene->low_threshold = tmp;
    }
  } else {
    if (0 == ConfigReadItemDouble("lasr.lasr_wkp.std_threshold", &tmp)) {
      scene->std_threshold = tmp;
    }
    if (0 == ConfigReadItemDouble("lasr.lasr_wkp.lp_threshold", &tmp)) {
      scene->low_threshold = tmp;
    }
  }
}

static void _recog_before_engine_start_callback(VuiHandle vui, Scene *scene) {
  uni_s32 wakeup = 0;
  if (scene != g_recog_service.cur_scene) {
    uni_memcpy(g_recog_service.cur_scene, scene, sizeof(Scene));
  }
  LOGD(UNI_RECOG_SERVICE_TAG, "start scene called, scene = %p.", scene);
  if (g_recog_service.cur_scene->asr_mode == UNI_LP_WAKEUP) {
    ArptWrite("enter wakeup_normal\n", false);
    wakeup = 0;
  } else {
    ArptWrite("enter asr_normal\n", false);
    wakeup = 1;
  }
  BbWrite(BB_KEY_IS_WAKEUP, wakeup);
  IdleDetectServiceAccess();
}

static void _recog_after_engine_cancel_callback(VuiHandle vui, Scene *scene) {
  IdleDetectServiceAccess();
}

static uni_s32 _vui_event_handler(Event *event) {
  Result rc;
  if (!g_recog_service.is_work) {
    return 0;
  }
  rc = RecogPreproc(event, g_recog_service.cur_scene->asr_mode,
                    g_recog_service.cur_scene->std_threshold,
                    g_recog_service.cur_scene->low_threshold,
                    g_recog_service.vui);
  if (E_OK == rc) {
    LOGT(UNI_RECOG_SERVICE_TAG, "add app event to list: %d.", event->type);
    EventRouteProcess(event);
    return 0;
  }
  return -1;
}

/**
 * Usage:   Initialize Recognization
 * Params:
 * Return:  Result of initialization
 */
Result RecogInit(void) {
  uni_memset(&g_recog_service, 0, sizeof(g_recog_service));
  g_recog_service.vui = EngineCreate(_vui_event_handler);
  if (NULL == g_recog_service.vui) {
    LOGE(UNI_RECOG_SERVICE_TAG, "create vui failed.");
    return E_FAILED;
  }
  g_recog_service.cur_scene = uni_malloc(sizeof(Scene));
  if (NULL == g_recog_service.cur_scene) {
    LOGE(UNI_RECOG_SERVICE_TAG, "alloc memory failed.");
    goto L_ERROR0;
  }
  _default_scene(g_recog_service.cur_scene);
  _set_wakeup_thresold(g_recog_service.cur_scene);
  g_recog_service.is_work = true;
  BbWrite(BB_KEY_IS_WAKEUP, 0);
  LOGT(UNI_RECOG_SERVICE_TAG, "scene create done.");
  RecogRegisterBeforeStart(_recog_before_engine_start_callback);
  RecogRegisterAfterCancel(_recog_after_engine_cancel_callback);
  ArptInit();
  RecogPreprocInit();
  LOGT(UNI_RECOG_SERVICE_TAG, "pre recogn start done.");
  return E_OK;
L_ERROR0:
  EngineDestroy(g_recog_service.vui);
  return E_FAILED;
}

/**
 * Usage:   Finalize Recognization
 * Params:
 * Return:  Result of Finalization
 */
Result RecogFinal(void) {
  if (NULL != g_recog_service.vui) {
    if (NULL != g_recog_service.cur_scene->grammar) {
      uni_free(g_recog_service.cur_scene->grammar);
    }
    uni_free(g_recog_service.cur_scene);
    EngineDestroy(g_recog_service.vui);
    ArptFinal();
  }
  return E_OK;
}

Result RecogLaunch(Scene *scene) {
  Result rc;
  rc = EngineCancel(g_recog_service.vui);
  if (E_OK != rc) {
    LOGE(UNI_RECOG_SERVICE_TAG, "cancel vui failed.");
    /* do not return even if "cancel" failed */
  }
  if (NULL != scene) {
    rc = EngineStart(g_recog_service.vui, scene);
  } else {
    rc = EngineStart(g_recog_service.vui, g_recog_service.cur_scene);
  }
  LOGT(UNI_RECOG_SERVICE_TAG, "finished relaunch engine %d", rc);
  return rc;
}

Result RecogStop(void) {
  EngineStop(g_recog_service.vui);
  return E_OK;
}

Result RecogCancel(void) {
  EngineCancel(g_recog_service.vui);
  return E_OK;
}

Result RecogMute(uni_bool is_mute) {
  if (is_mute) {
    g_recog_service.is_work = false;
  } else {
    g_recog_service.is_work = true;
  }
  return E_OK;
}

Result RecogBlacklist(const char *word, uni_bool is_append) {
  if (0 != NluParseBlacklist(word, is_append)) {
    LOGE(UNI_RECOG_SERVICE_TAG, "NluParseBlacklist failed.");
    return E_FAILED;
  }
  return E_OK;
}

Result RecogEnableTimeout(uni_bool is_enable) {
  return EngineEnableTimeout(g_recog_service.vui, is_enable);
}

void RecogRegisterBeforeStart(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_BEFORE_START);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

void RecogRegisterAfterStart(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_AFTER_START);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

void RecogRegisterBeforeCancel(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_BEFORE_CANCEL);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

void RecogRegisterAfterCancel(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_AFTER_CANCEL);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

void RecogRegisterBeforeStop(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_BEFORE_STOP);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

void RecogRegisterAfterStop(EngineCallback cb) {
  EngineCbRegister(cb, ENGINE_CB_TYPE_AFTER_STOP);
  LOGT(UNI_RECOG_SERVICE_TAG, "engine cb %p is registered.", cb);
}

