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
 * Description : Uniapp recog_engine_control.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.12.19
 *
 **************************************************************************/
#include "uni_recog_engine_control.h"
#include "uni_log.h"
#include "uni_vui_interface.h"
#include "uni_event_list.h"
#include "uni_recog_service.h"

#define UNI_RECOG_ENG_CTRL_TAG            "recog_eng_ctrl"

typedef enum {
  ENGINE_STATE_IDLE,
  ENGINE_STATE_RUNNING
} EngineControlState;

typedef enum {
  ENGINE_EVENT_START,
  ENGINE_EVENT_STOP,
  ENGINE_EVENT_CANCEL,
  ENGINE_EVENT_TIMEOUT
} EngineControlEvent;

typedef struct {
  VuiHandle vui;
  Scene     *scene;
} EngineParam;

typedef struct EngineCallbackNode_{
  EngineCallback             cb;
  struct EngineCallbackNode_ *next;
} EngineCallbackNode;

static EngineControlState g_engine_state = ENGINE_STATE_IDLE;
static EngineCallbackNode *g_before_start_cb_list = NULL;
static EngineCallbackNode *g_after_start_cb_list = NULL;
static EngineCallbackNode *g_before_cancel_cb_list = NULL;
static EngineCallbackNode *g_after_cancel_cb_list = NULL;
static EngineCallbackNode *g_before_stop_cb_list = NULL;
static EngineCallbackNode *g_after_stop_cb_list = NULL;

static char *_event2string(EngineControlEvent event_type) {
  switch(event_type) {
    case ENGINE_EVENT_START:
      return "ENGINE_EVENT_START";
    case ENGINE_EVENT_STOP:
      return "ENGINE_EVENT_STOP";
    case ENGINE_EVENT_CANCEL:
      return "ENGINE_EVENT_CANCEL";
    case ENGINE_EVENT_TIMEOUT:
      return "ENGINE_EVENT_TIMEOUT";
  }
  return "N/A";
}

static char *_state2string(EngineControlState state) {
  switch(state) {
    case ENGINE_STATE_IDLE:
      return "ENGINE_STATE_IDLE";
    case ENGINE_STATE_RUNNING:
      return "ENGINE_STATE_RUNNING";
  }
  return "N/A";
}

static void _set_engine_state(EngineControlState state) {
  LOGD(UNI_RECOG_ENG_CTRL_TAG, "engine state is changed from %s to %s.",
       _state2string(g_engine_state), _state2string(state));
  g_engine_state = state;
}

/**
 * Usage:   Start the recognition
 * Params:  mode[in]      recognition mode
 *          (1) UNI_LP_WAKEUP:
 *          (2) UNI_LP_LASR:
 *          (3) UNI_RASR_MODE:
 * Return:  Result
 *          (1) E_OK:       start the recognition successfully
 *          (2) E_FAILED:   fail to start the recognition
 */
static Result _engine_start_internal(VuiHandle vui, uni_s32 asr_mode,  uni_u32 timeout) {
  LOGT(UNI_RECOG_ENG_CTRL_TAG, "start new asr_mode %d ,timeout %d.", asr_mode, timeout);
  return VuiRecognStart(vui, asr_mode, timeout);
}

static Result _engine_cancel_internal(VuiHandle vui) {
  return VuiRecognCancel(vui);
}

static Result _engine_stop_internal(VuiHandle vui) {
  return VuiRecognStop(vui);
}

void EngineCbRegister(EngineCallback cb, EngineCbType cb_type) {
  EngineCallbackNode **p_cb_list;
  EngineCallbackNode *node, *p;
  switch (cb_type) {
    case ENGINE_CB_TYPE_BEFORE_START:
      p_cb_list = &g_before_start_cb_list;
      break;
    case ENGINE_CB_TYPE_AFTER_START:
      p_cb_list = &g_after_start_cb_list;
      break;
    case ENGINE_CB_TYPE_BEFORE_CANCEL:
      p_cb_list = &g_before_cancel_cb_list;
      break;
    case ENGINE_CB_TYPE_AFTER_CANCEL:
      p_cb_list = &g_after_cancel_cb_list;
      break;
    case ENGINE_CB_TYPE_BEFORE_STOP:
      p_cb_list = &g_before_stop_cb_list;
      break;
    case ENGINE_CB_TYPE_AFTER_STOP:
      p_cb_list = &g_after_stop_cb_list;
      break;
    default:
      LOGE(UNI_RECOG_ENG_CTRL_TAG, "wrong engine callback type.");
      return;
  }
  p = *p_cb_list;
  node = uni_malloc(sizeof(EngineCallbackNode));
  if (NULL == node) {
    LOGE(UNI_RECOG_ENG_CTRL_TAG, "memory malloc failed.");
    return;
  }
  node->cb = cb;
  node->next = NULL;
  if (NULL == p) {
    *p_cb_list = node;
    return;
  }
  while (NULL != p->next) {
    p = p->next;
  }
  p->next = node;
}

static void _engine_cb_clear(EngineCallbackNode **p_cb_list) {
  EngineCallbackNode *node = *p_cb_list;
  while (NULL != node) {
    uni_free(node);
    node = node->next;
  }
  *p_cb_list = NULL;
}

static void _engine_cb_execute(EngineCallbackNode *cb_list,
                               VuiHandle vui, Scene *scene) {
  EngineCallbackNode *node = cb_list;
  while (NULL != node) {
    node->cb(vui, scene);
    LOGD(UNI_RECOG_ENG_CTRL_TAG, "vui cb %p is called.", node->cb);
    node = node->next;
  }
}

static Result _engine_process(Event *event) {
  Result rc = E_FAILED;
  uni_s32 old_state = g_engine_state;
  EngineParam *param = (EngineParam *)event->content.info;
  switch (g_engine_state) {
    case ENGINE_STATE_IDLE:
      if (ENGINE_EVENT_START == event->type) {
        _engine_cb_execute(g_before_start_cb_list, param->vui, param->scene);
        rc = _engine_start_internal(param->vui, param->scene->asr_mode,
                                    param->scene->timeout);
        if (E_OK == rc) {
          _engine_cb_execute(g_after_start_cb_list, param->vui, param->scene);
          _set_engine_state(ENGINE_STATE_RUNNING);
        }
      } else if (ENGINE_EVENT_TIMEOUT == event->type) {
        VuiRecognEnableTimeout(param->vui, event->content.value_int);
      }
      break;
    case ENGINE_STATE_RUNNING:
      if (ENGINE_EVENT_STOP == event->type) {
        _engine_cb_execute(g_before_stop_cb_list, param->vui, NULL);
        rc = _engine_stop_internal(param->vui);
        if (E_OK == rc) {
          _engine_cb_execute(g_after_stop_cb_list, param->vui, NULL);
          _set_engine_state(ENGINE_STATE_IDLE);
        }
      } else if (ENGINE_EVENT_CANCEL == event->type) {
        _engine_cb_execute(g_before_cancel_cb_list, param->vui, NULL);
        rc = _engine_cancel_internal(param->vui);
        if (E_OK == rc) {
          _engine_cb_execute(g_after_cancel_cb_list, param->vui, NULL);
          _set_engine_state(ENGINE_STATE_IDLE);
        }
      } else if (ENGINE_EVENT_TIMEOUT == event->type) {
        VuiRecognEnableTimeout(param->vui, event->content.value_int);
      }
      break;
    default:
      break;
  }
  if (NULL != param) {
    uni_free(param);
  }
  LOGD(UNI_RECOG_ENG_CTRL_TAG,
       "engine operation %s, state %s => %s event = %s.",
       E_OK == rc ? "OK" : "ERROR",
       _state2string(old_state),
       _state2string(g_engine_state),
       _event2string(event->type));
  return rc;
}

VuiHandle EngineCreate(EventHandler event_handler) {
  return VuiHandleCreate(event_handler);
}

void EngineDestroy(VuiHandle vui) {
  VuiHandleDestroy(vui);
  _engine_cb_clear(&g_before_start_cb_list);
  _engine_cb_clear(&g_before_stop_cb_list);
  _engine_cb_clear(&g_before_cancel_cb_list);
  _engine_cb_clear(&g_after_start_cb_list);
  _engine_cb_clear(&g_after_stop_cb_list);
  _engine_cb_clear(&g_after_cancel_cb_list);
}

Result EngineStart(VuiHandle vui, Scene *scene) {
  Event event;
  event.type = ENGINE_EVENT_START;
  EngineParam *param = uni_malloc(sizeof(EngineParam));
  if (NULL == param) {
    LOGE(UNI_RECOG_ENG_CTRL_TAG, "malloc failed !");
    return E_FAILED;
  }
  param->vui = vui;
  param->scene = scene;
  event.content.info = (char *)param;
  return _engine_process(&event);
}

Result EngineCancel(VuiHandle vui) {
  Event event;
  event.type = ENGINE_EVENT_CANCEL;
  EngineParam *param = uni_malloc(sizeof(EngineParam));
  if (NULL == param) {
    LOGE(UNI_RECOG_ENG_CTRL_TAG, "malloc failed !");
    return E_FAILED;
  }
  param->vui = vui;
  event.content.info = (char *)param;
  return _engine_process(&event);
}

Result EngineStop(VuiHandle vui) {
  Event event;
  event.type = ENGINE_EVENT_STOP;
  EngineParam *param = uni_malloc(sizeof(EngineParam));
  if (NULL == param) {
    LOGE(UNI_RECOG_ENG_CTRL_TAG, "malloc failed !");
    return E_FAILED;
  }
  param->vui = vui;
  event.content.info = (char *)param;
  return _engine_process(&event);
}

Result EngineEnableTimeout(VuiHandle vui, uni_bool is_enable) {
  Event event;
  event.type = ENGINE_EVENT_TIMEOUT;
  EngineParam *param = uni_malloc(sizeof(EngineParam));
  if (NULL == param) {
    LOGE(UNI_RECOG_ENG_CTRL_TAG, "malloc failed !");
    return E_FAILED;
  }
  param->vui = vui;
  event.content.value_int = is_enable;
  event.content.info = (char *)param;
  return _engine_process(&event);
}

