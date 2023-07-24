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
 * Description : uni_session.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef APP_INC_UNI_SESSION_H_
#define APP_INC_UNI_SESSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "list_head.h"
#include "uni_event.h"
#include "uni_databuf.h"
#include "user_config.h"

/**
 * Usage:   Callback function type used to determine if a event can be handled
 *          by the session in its current state.
 *          Implemented by session handler and called by session manager
 * Params:  event    event to be handled, cannot be NULL
 * Return:  Query result. Should be one of the following value:
 *          (1) E_OK:     event can be handled by this session
 *          (2) E_REJECT: event cannot be handled by this session
 *          (3) E_FAILED: something is wrong
 */
typedef Result (*SessionQueryCb)(Event *event);

/**
 * Usage:   Callback function type used to start a session.
 *          start callback being called means the session is scheduled.
 *          Implemented by session handler and called by session manager
 * Params:  event    event to be handled, can be NULL
 * Return:  Result of session running. Should be one of the following value:
 *          (1) E_OK:     session is done
 *          (2) E_HOLD:   session is not done
 *          (3) E_REJECT: session has nothing to do
 *          (4) E_FAILED: something is wrong
 */
typedef Result (*SessionStartCb)(Event *event);

/**
 * Usage:   Callback function type used to stop/interrupt a session. Session
 *          handler could choose to stop session or just pause.
 *          stop callback being called means the session is preempted.
 *          Implemented by session handler and called by session manager
 * Params:
 * Return:  Result of the interruption. Should be one of the following value:
 *          (1) E_OK:     session is done
 *          (2) E_HOLD:   session is not done
 *          (3) E_FAILED: something is wrong
 */
typedef Result (*SessionStopCb)(void);

typedef struct {
  uni_list_head  link;
  /* greater number means higher priority */
  uni_s32        priority;
  uni_bool       is_suspended;
  /* for caching event for later processing */
  DataBufHandle  event_buf;
  SessionQueryCb query_cb;
  SessionStartCb start_cb;
  SessionStopCb  stop_cb;
} Session;

Session* SessionCreate(uni_s32 priority, uni_s32 max_bufed_event,
                       SessionQueryCb query_cb,
                       SessionStartCb start_cb,
                       SessionStopCb stop_cb);
void   SessionDestroy(Session *session);
Result SessionQuery(Session *session, Event *event);
Result SessionStart(Session *session);
Result SessionStop(Session *session);

#ifdef __cplusplus
}
#endif
#endif  //  APP_INC_UNI_SESSION_H_
