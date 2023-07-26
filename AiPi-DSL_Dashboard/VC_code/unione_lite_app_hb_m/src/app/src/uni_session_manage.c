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
 * Description : uni_session_manage.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.6.5
 *
 **************************************************************************/

#include "uni_session_manage.h"

#include "uni_log.h"
#include "uni_event_list.h"
#include "list_head.h"
#include "uni_event_route.h"
#include "uni_black_board.h"
#include "uni_recog_service.h"
//TODO
//#include "uni_black_list.h"

#define SESSION_MGR_TAG   "session_mgr"

static struct SessionManager {
  uni_list_head   session_list;
  Session         *active_session;
  EventListHandle event_list;
  //BlackList *black_list;
} g_session_manager;

static Result _insert_session(Session *session) {
  uni_list_head *p;
  Session *s;
  uni_list_for_each(p, &g_session_manager.session_list) {
    s = uni_list_entry(p, Session, link);
    if (s == session) {
      LOGW(SESSION_MGR_TAG, "session already registered");
      return E_FAILED;
    }
    if (s->priority < session->priority) {
      break;
    }
  }
  uni_list_add_before(&session->link, p);
  return E_OK;
}

static Result _remove_session(Session *session) {
  if (NULL != session) {
    uni_list_del(&session->link);
  }
  return E_OK;
}

Result SessionManageRegister(Session *session) {
  return _insert_session(session);
}

Result SessionManageUnregister(Session *session) {
  return _remove_session(session);
}

/**
 * Usage:   check if the event is disabled
 * Params:  event  event to be checked
 * Return:
 */
static uni_bool _check_disabled_event(Event *event) {
  return (INVALID_EVENT_ID_EVENT_INVALID == event->type);
}

/**
 * Usage:   To check if a event can be handled currently
 * Params:  event         event to be handled
 *          from_queue    specify if the event is newly comming or a low
 *                        priority one buffered in event queue before
 * Return:  Handler that will handle the event, NULL means no proper handler
 *          found
 */
static Session* _session_query(Event *event) {
  Result rc = E_REJECT;
  char event_type = event->type;
  Session *p, *session = NULL;
  uni_list_for_each_entry(p, &g_session_manager.session_list, Session, link) {
    if (E_OK == (rc = SessionQuery(p, event))) {
      break;
    }
  }
  /* session will be returned to caller only if the session found can be
     scheduled immediately, which means the current active session is NULL
     or with lower priority than the session founld */
  if (E_OK == rc) {
    if (NULL == g_session_manager.active_session) {
      session = p;
    } else {
      if (p->priority >= g_session_manager.active_session->priority) {
        if (p != g_session_manager.active_session) {
          BbWrite(BB_KEY_EVENT_ID, event_type);
          SessionStop(g_session_manager.active_session);
          g_session_manager.active_session = NULL;
        }
        session = p;
      }
    }
  } else {
    /* no one can handle this event */
    if (NULL != event->free_handler) {
      event->free_handler(&event->content);
    }
  }
  return session;
}

/**
 * Usage:   To determine the next session to run or next event to handle.
 *          Either resume an interrupted session or pick an event from event
 *          queue. This occurs only when a session is ended or blocked to wait
 *          for another event.
 * Params:  last_handler   last session handler that was running
 *          last_result    the return value of the last session handler. can be
 *                         E_HOLD means session is blocked
 *                         E_OK/E_FAILED means session is ended
 * Return:
 */
static void _session_schedule(Session *last_session, Result last_result) {
  Session *p, *session = NULL;
  Result rc = E_OK;
  /* return is no session got scheduled last time */
  if (NULL == last_session) {
    return;
  }
  /* return if current active session (with max priority) is running */
  if (E_HOLD == last_result) {
    g_session_manager.active_session = last_session;
    return;
  }
  /* next scheduling */
  g_session_manager.active_session = NULL;
  uni_list_for_each_entry(p, &g_session_manager.session_list, Session, link) {
    rc = SessionStart(p);
    if (E_REJECT != rc) {
      session = p;
      break;
    }
  }
  _session_schedule(session, rc);
}

static uni_s32 _session_manage_schedule(Event *event) {
  LOGT(SESSION_MGR_TAG, "event %d received", event->type);
  uni_s32 ret;
  if (event->type == ID(SESSION_MGR_REVIEW_EVENT)) {
    ret = EventListTraverse(g_session_manager.event_list,
                            (EventHandler)event->content.info);
  } else {
    ret = EventListAdd(g_session_manager.event_list, event,
                       EVENT_LIST_PRIORITY_MEDIUM);
  }
  return ret;
}

/*static int _check_in_black_list(Event *event_info) {
  return BlackListIsBlack(g_session_manager.black_list, event_info->event);
}*/

static uni_s32 _event_callback(Event *event) {
  if (true == _check_disabled_event(event)) {
    if (NULL != event->free_handler) {
      event->free_handler(&event->content);
    }
    LOGW(SESSION_MGR_TAG, "invalid event is skipped for event_list.");
    return -1;
  }
  /*if (_check_in_black_list(event)) {
    if (NULL != event.free_handler) {
      event.free_handler(&event.content);
    }
    uni_log_err("event in black list is skipped for event_list\n");
    return;
  }*/
  Session *session = _session_query(event);
  if (NULL != session) {
    _session_schedule(session, SessionStart(session));
  }
  return 0;
}

Result SessionManageInit(void) {
const uni_s32 events[] = {ID(AUDIO_PLAY_END_EVENT),
                      ID(SESSION_MGR_REVIEW_EVENT),
                      ID(VUI_APP_SETTING_EVENT),
                      ID(VUI_APP_SLEEP_EVENT),
                      ID(VUI_APP_VOLUME_SETTING_EVENT),
                      ID(VUI_APP_WAKEUP_EVENT),
                      ID(VUI_APP_MIC_MUTE_EVENT),
                      ID(VUI_APP_MIC_UNMUTE_EVENT),
                      ID(WATCH_DOG_FEED_EVENT),
                      ID(APP_GOTO_WAKEUP_EVENT),
                      ID(VUI_LOCAL_ASR_TIMEOUT_EVENT),
                      ID(COMMON_STOP_EVENT),
                      ID(APP_SLEEP_MODE_EVENT),
                      ID(USER_AUDIO_PLAY_EVENT),
                      ID(USER_AUDIO_STOP_EVENT),
                      ID(USER_GOTO_SLEEPING_EVENT),
                      ID(USER_GOTO_AWAKEND_EVENT),
                      ID(USER_MIC_MUTE_EVENT),
                      ID(USER_VOLUME_SETTING_EVENT)};
  uni_list_init(&g_session_manager.session_list);
  g_session_manager.active_session = NULL;
  BbWrite(BB_KEY_EVENT_ID, INVALID_EVENT_ID_EVENT_INVALID);
  g_session_manager.event_list = EventListCreate(_event_callback, 2048);
  EventRouteSubscribe(_session_manage_schedule,
                      sizeof(events) / sizeof(events[0]), events);
  //g_session_manager.black_list = BlackListCreate(64);
  return E_OK;
}

Result SessionManageFinal(void) {
  EventListDestroy(g_session_manager.event_list);
  //BlackListDestroy(g_session_manager.black_list);
  EventRouteUnregister(_session_manage_schedule);
  return E_OK;
}
