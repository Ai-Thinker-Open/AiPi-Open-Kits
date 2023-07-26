/**********************************************************************
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
 **********************************************************************
 *
 * Description : uni_watchdog_session.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2018.6.14
 *
 **********************************************************************/

#include "uni_watchdog_session.h"

#include "uni_fsm.h"
#include "uni_session.h"
#include "uni_session_priority.h"
#include "uni_session_manage.h"
#include "uni_event_route.h"
#include "uni_log.h"
#include "uni_hal_watchdog.h"
#include "uni_pcm_default.h"
#include "uni_recog_service.h"

#define WATCH_DOG_SESSION_TAG "watch_session"

#define WATCH_DOG_RELOAD_SEC  30

#define ENGINE_LOST_HEARTBEAT_THRESHOLD  10

typedef enum {
  STATE_IDLE = 0,
} WatchDogSessionState;

typedef struct {
  Session        *session;
  MicroFsmStruct *fsm;
  uni_u32        engine_hb_lost_count;
  uni_bool       is_engine_stop;
  uni_bool       reboot_flag;
} WatchDogSession;

static Result _idle__watch_dog_feed_event(void *event);

static MicroFsmTransition *g_watchdog_session_transition = NULL;
static WatchDogSession    *g_watchdog_session = NULL;

void _watch_dog_engine_stop_callback(VuiHandle vui, Scene *scene) {
  g_watchdog_session->is_engine_stop = true;
}

void _watch_dog_engine_start_callback(VuiHandle vui, Scene *scene) {
  g_watchdog_session->is_engine_stop = false;
  g_watchdog_session->engine_hb_lost_count = 0;
}

static int _session_transition_init() {
const MicroFsmTransition session_transition[] = {
    {STATE_IDLE, ID(WATCH_DOG_FEED_EVENT), _idle__watch_dog_feed_event},
  };
  g_watchdog_session_transition = uni_malloc(sizeof(session_transition));
  if (NULL == g_watchdog_session_transition) {
    LOGE(WATCH_DOG_SESSION_TAG, "malloc failed!");
    return -1;
  }
  uni_memcpy(g_watchdog_session_transition, session_transition,
             sizeof(session_transition));
  return (sizeof(session_transition) / sizeof(session_transition[0]));
}

static const char* _watchdog_session_state_2_str(uni_s32 state) {
  static const char *state_str[] = {
    [STATE_IDLE] = "STATE_IDLE",
  };
  if (state != STATE_IDLE) {
    return "N/A";
  }
  return state_str[state];
}

static Result _idle__watch_dog_feed_event(void *event) {
  LOGT(WATCH_DOG_SESSION_TAG, "action called.");
  FsmSetState(g_watchdog_session->fsm, STATE_IDLE);
  return E_OK;
}

static uni_bool _check_engine_heartbeat(){
  if (g_watchdog_session->is_engine_stop) {
    return true;
  }
  g_watchdog_session->engine_hb_lost_count++;
  if (g_watchdog_session->engine_hb_lost_count > ENGINE_LOST_HEARTBEAT_THRESHOLD) {
    LOGE(WATCH_DOG_SESSION_TAG, "engine heartbeat lost over count, do not feed dog now and will reboot soon!");
    return false;
  }
  return true;
}

static Result _query_handler(Event *event) {
  Result ret = IsValidEventId(g_watchdog_session->fsm, event->type);
  LOGT(WATCH_DOG_SESSION_TAG, "query handler called, event=%d ret=%d",
       event->type, ret);
  if (ret == E_OK && _check_engine_heartbeat() && !g_watchdog_session->reboot_flag) {
    uni_hal_watchdog_feed();
  }
  return E_REJECT;
}

static Result _start_handler(Event *event) {
  Result ret;
  if (NULL == event) {
    ret = ProcessNextEvent(g_watchdog_session->fsm, NULL, ID(COMMON_RESUME_EVENT));
    LOGT(WATCH_DOG_SESSION_TAG, "start handler called, event=NULL ret=%d", ret);
  } else {
    ret = ProcessNextEvent(g_watchdog_session->fsm, event, event->type);
    LOGT(WATCH_DOG_SESSION_TAG, "start handler called, event=%d ret=%d",
         event->type, ret);
  }
  return ret;
}

static Result _stop_handler(void) {
  Result ret = ProcessNextEvent(g_watchdog_session->fsm,
                                NULL, ID(COMMON_STOP_EVENT));
  LOGT(WATCH_DOG_SESSION_TAG, "stop handler called, ret=%d.", ret);
  return ret;
}

void WatchDogTriggerEngineHB(void) {
  if (g_watchdog_session != NULL) {
    g_watchdog_session->engine_hb_lost_count = 0;
  }
}

void WatchDogReboot(void) {
  if (g_watchdog_session != NULL) {
    g_watchdog_session->reboot_flag = 1;
    LOGE(WATCH_DOG_SESSION_TAG, "reboot");
  }
}

Result WatchDogSessionInit(void) {
  uni_s32 session_transition_cnt = 0;
  g_watchdog_session = (WatchDogSession *)uni_malloc(sizeof(WatchDogSession));
  if (NULL == g_watchdog_session) {
    LOGE(WATCH_DOG_SESSION_TAG, "alloc memory failed.");
    return E_FAILED;
  }
  uni_memset(g_watchdog_session, 0, sizeof(WatchDogSession));
  g_watchdog_session->session = SessionCreate(SESSION_PRIORITY_WATCHDOG, 5,
                                              _query_handler,
                                              _start_handler,
                                              _stop_handler);
  if (NULL == g_watchdog_session->session) {
    LOGE(WATCH_DOG_SESSION_TAG, "create session failed.");
    goto L_ERROR0;
  }
  g_watchdog_session->fsm = (MicroFsmStruct *)uni_malloc(sizeof(MicroFsmStruct));
  if (NULL == g_watchdog_session->fsm) {
    LOGE(WATCH_DOG_SESSION_TAG, "alloc memory failed.");
    goto L_ERROR1;
  }
  g_watchdog_session->engine_hb_lost_count = 0;
  g_watchdog_session->reboot_flag = 0;
  g_watchdog_session->is_engine_stop = false;
  session_transition_cnt = _session_transition_init();
  FsmInit(g_watchdog_session->fsm, "watchdog", g_watchdog_session_transition,
          session_transition_cnt, _watchdog_session_state_2_str);
  FsmSetState(g_watchdog_session->fsm, STATE_IDLE);
  SessionManageRegister(g_watchdog_session->session);
  RecogRegisterAfterStart(_watch_dog_engine_start_callback);
  RecogRegisterAfterStop(_watch_dog_engine_stop_callback);
  uni_hal_watchdog_enable(WDG_STEP_4S);
  return E_OK;
L_ERROR1:
  SessionDestroy(g_watchdog_session->session);
L_ERROR0:
  uni_free(g_watchdog_session);
  g_watchdog_session = NULL;
  return E_FAILED;
}

Result WatchDogSessionFinal(void) {
  if (NULL != g_watchdog_session) {
    SessionManageUnregister(g_watchdog_session->session);
    SessionDestroy(g_watchdog_session->session);
    if (NULL != g_watchdog_session->fsm) {
      uni_free(g_watchdog_session->fsm);
      g_watchdog_session->fsm = NULL;
    }
    uni_free(g_watchdog_session);
    g_watchdog_session = NULL;
  }
  if (NULL != g_watchdog_session_transition) {
    uni_free(g_watchdog_session_transition);
    g_watchdog_session_transition = NULL;
  }
  uni_hal_watchdog_disable();
  return E_OK;
}
