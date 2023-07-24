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
 * Description : uni_vui_interface.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_vui_interface.h"
#include "uni_log.h"
#include "uni_json.h"
#include "aik-config.h"
#include "asrfix.h"
#include "grammar.h"
#include "include/aik.h"
#include "include/aik-id.h"
#include "include/aik-errno.h"
#include "uni_watchdog_session.h"
#include "uni_hal_audio.h"

#define VUI_TAG          "vui"

typedef enum {
  VUI_RECOGN_INIT = 0,
  VUI_RECOGN_STOP,
  VUI_RECOGN_RUNNING,
} VuiRecognStatus;

typedef struct {
  EventHandler    event_handler;
  VuiRecognStatus status;
  uni_u32         session_id;
  AikHandle       aik;
  AikMode         mode;
  uni_bool        is_timeout;
} Vui;

extern uint32_t AudioADCBuf[];
extern uint32_t AudioADCBuf_size;

static Vui g_vui = {NULL, VUI_RECOGN_INIT, 0, NULL, AIK_MODE_NULL, true};

static const char* _status_to_str(VuiRecognStatus status) {
  switch (status) {
    case VUI_RECOGN_RUNNING: return "RECOGN_RUNNING";
    case VUI_RECOGN_STOP:    return "RECOGN_STOP";
    default: break;
  }
  return "N/A";
}

static inline void _set_recogn_status(Vui *vui, VuiRecognStatus status) {
  vui->status = status;
  LOGT(VUI_TAG, "vui set recogn status. %s", _status_to_str(status));
}

static void _create_send_event(char *command, uni_s32 event_type, uni_u32 session_id) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.info           = command;
  event_content.extend_info    = NULL;
  event_content.vui_session_id = session_id;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, event_type, &event_content,
                      DefaultEventContentFreeHandler);
  if (command) {
	  LOGW(VUI_TAG, "event_type[%d]: %s", event_type, command);
  } else {
	  LOGW(VUI_TAG, "event_type[%d]", event_type);
  }
  g_vui.event_handler(event);
  EventFree(event);
  return;
}

static void _got_lasr_result(const char* content) {
  cJSON *root = cJSON_Parse(content);
  char *command = NULL;
  if (0 != JsonReadItemString(root, "aik_event[0].value.result", &command)) {
    LOGE(VUI_TAG, "Unknown KWS result: %s, memeory may not enough, will reboot now", content);
    cJSON_Delete(root);
    WatchDogReboot();
    return;
  }
  _create_send_event(command, ID(VUI_LOCAL_ASR_SELF_RESULT_EVENT), 0);
  cJSON_Delete(root);
}

static void _lasr_timeout(void) {
  if (g_vui.is_timeout) {
    _create_send_event(NULL, ID(VUI_LOCAL_ASR_TIMEOUT_EVENT), 0);
  }
}

static void _aik_event_callback(AikEvent event, char* content) {
  switch(event) {
    case AIK_EVENT_KWS_WAKEUP:
      _got_lasr_result(content);
      break;
    case AIK_EVENT_KWS_COMMAND:
      _got_lasr_result(content);
      break;
    case AIK_EVENT_KWS_TIMEOUT:
      _lasr_timeout();
      break;
    case AIK_EVENT_ASR_RESULT:
      /* unsupport RASR on hummbird-m */
      break;
    case AIK_EVENT_ASR_TIMEOUT:
      /* unsupport RASR on hummbird-m */
      break;
    case AIK_EVENT_HEARTBEAT:
      WatchDogTriggerEngineHB();
      break;
    default:
      LOGE(VUI_TAG, "Unknown AIK event : %d", event);
      break;
  }
  return;
}

static void _aik_data_callback(AikEvent event, void* data, int size, int index) {
  return;
}

VuiHandle VuiHandleCreate(EventHandler vui_event_handler) {
  Vui *vui = &g_vui;
  if (VUI_RECOGN_INIT == g_vui.status) {
    g_vui.aik = UalAikInit((const char*)aik_config, _aik_event_callback, _aik_data_callback);
    if (g_vui.aik == NULL) {
      LOGE(VUI_TAG, "UalAikInit failed.");
      return NULL;
    }
    g_vui.mode = AIK_MODE_NULL;
    vui->event_handler = vui_event_handler;
    _set_recogn_status(vui, VUI_RECOGN_STOP);
  }
  return (VuiHandle)vui;
}

Result VuiHandleDestroy(VuiHandle handle) {
  Vui *vui = (Vui *)handle;
  if (NULL == vui) {
    LOGE(VUI_TAG, "handle=%p", handle);
    return E_FAILED;
  }
  UalAikRelease(vui->aik);
  return E_OK;
}

Result VuiRecognStart(VuiHandle handle, VuiRecognModel mode_id, uni_u32 timeout) {
  Vui *vui = (Vui *)handle;
  int ret = AIK_OK;
  if (NULL == vui) {
    LOGE(VUI_TAG, "recogn start failed. handle=%p", handle);
    return E_FAILED;
  }
  if (VUI_RECOGN_RUNNING == vui->status) {
    LOGW(VUI_TAG, "recogn status[%s], cannot start again.",
                  _status_to_str(VUI_RECOGN_RUNNING));
    return E_FAILED;
  }
  /* no interface to get session ID from AIK */
  //vui->session_id ++;
  switch (mode_id) {
    case UNI_LP_WAKEUP:
      UalAikSet(vui->aik, AIK_ID_KWS_LP_SET_WAKEUP, NULL);
      vui->mode = AIK_MODE_KWS_LP;
      break;
    case UNI_LP_LASR:
      UalAikSet(vui->aik, AIK_ID_KWS_LP_SET_COMMAND, NULL);
      vui->mode = AIK_MODE_KWS_LP;
      break;
    case UNI_RASR_MODE:
      vui->mode = AIK_MODE_ASR;
      break;
    case UNI_RASR_LASR_MODE:
      UalAikSet(vui->aik, AIK_ID_KWS_LP_SET_COMMAND, NULL);
      /* TODO: support KWS LP only now */
      //vui->mode = AIK_MODE_KWS_LP_ASR;
      vui->mode = AIK_MODE_KWS_LP;
      break;
    default:
      LOGW(VUI_TAG, "unknow VUI mode: %d", mode_id);
      return E_FAILED;
  }
  /* clear ADC ringbuffer before AIK start to disacard mute record */
  uni_hal_record_dmarestart(ADC0_MODULE, AudioADCBuf, AudioADCBuf_size);
  ret = UalAikStart(vui->aik, vui->mode);
  if (AIK_OK != ret) {
    LOGE(VUI_TAG, "recogn start faild. mode_id=%d", mode_id);
    return E_FAILED;
  }
  _set_recogn_status(vui, VUI_RECOGN_RUNNING);
  LOGT(VUI_TAG, "recogn start success. mode_id=%d", mode_id);
  return E_OK;
}

Result VuiRecognStop(VuiHandle handle) {
  Vui *vui = (Vui *)handle;
  if (NULL == vui) {
    LOGE(VUI_TAG, "handle=%p.", handle);
    return E_FAILED;
  }
  if (VUI_RECOGN_STOP == vui->status) {
    LOGW(VUI_TAG, "recogn status[%s], cannot stop again.",
                  _status_to_str(VUI_RECOGN_STOP));
    return E_FAILED;
  }
  UalAikStop(vui->aik, vui->mode);
  _set_recogn_status(vui, VUI_RECOGN_STOP);
  LOGD(VUI_TAG, "recogn stop success.");
  return E_OK;
}

Result VuiRecognCancel(VuiHandle handle) {
  Vui *vui = (Vui *)handle;
  if (NULL == vui) {
    LOGE(VUI_TAG, "handle=%p", handle);
    return E_FAILED;
  }
  if (VUI_RECOGN_STOP == vui->status) {
    LOGW(VUI_TAG, "recogn status[%s], cannot cancel again.",
         _status_to_str(VUI_RECOGN_STOP));
    return E_FAILED;
  }
  UalAikStop(vui->aik, vui->mode);
  _set_recogn_status(vui, VUI_RECOGN_STOP);
  LOGD(VUI_TAG, "recogn cancel success.");
  return E_OK;
}

uni_u32 VuiRecognGetSessionId(VuiHandle handle) {
  Vui *vui = (Vui *)handle;
  if (vui) {
    return vui->session_id;
  } else {
    return (uni_u32)-1;
  }
}

Result VuiRecognEnableTimeout(VuiHandle handle, uni_bool is_enable) {
  Vui *vui = (Vui *)handle;
  if (vui) {
    vui->is_timeout = is_enable;
    return E_OK;
  } else {
    return E_FAILED;
  }
}

