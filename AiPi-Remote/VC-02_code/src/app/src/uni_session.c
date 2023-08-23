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
 * Description : uni_session.c
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_session.h"
#include "uni_log.h"
#include "uni_user_meeting.h"

#define SESSION_TAG "session"

Session* SessionCreate(uni_s32 priority, uni_s32 max_bufed_event,
                       SessionQueryCb query_cb,
                       SessionStartCb start_cb,
                       SessionStopCb stop_cb) {
  Session *session = uni_malloc(sizeof(Session));
  if (NULL == session) {
    LOGE(SESSION_TAG, "memory alloc failed");
    return NULL;
  }
  session->priority = priority;
  session->is_suspended = false;
  session->event_buf = DataBufferCreate(max_bufed_event * EVENT_SIZE);
  session->query_cb = query_cb;
  session->start_cb = start_cb;
  session->stop_cb = stop_cb;
  return session;
}

void SessionDestroy(Session *session) {
  Event event;
  while (DataBufferGetDataSize(session->event_buf) >= EVENT_SIZE) {
    DataBufferRead((char *)&event, EVENT_SIZE, session->event_buf);
    if (NULL != event.free_handler) {
      event.free_handler(&event.content);
    }
  }
  DataBufferDestroy(session->event_buf);
  uni_free(session);
}

Result SessionQuery(Session *session, Event *event) {
  Result ret;
  uni_s32 remain_len;
  ret = session->query_cb(event);
  if (ret == E_OK) {
    if ((remain_len = DataBufferGetFreeSize(session->event_buf)) < EVENT_SIZE) {
      LOGE(SESSION_TAG, "remain_len=%d, session cache full, fatal error",
           remain_len);
      return E_REJECT;
    }
    DataBufferWrite(session->event_buf, (char *)event, EVENT_SIZE);
  }
  return ret;
}

Result SessionStart(Session *session) {
  Result ret = E_REJECT;
  Event event;
  if (DataBufferGetDataSize(session->event_buf) >= EVENT_SIZE) {
    DataBufferRead((char *)&event, EVENT_SIZE, session->event_buf);
    ret = session->start_cb(&event);
    uni_user_meeting_send_ucp(&event);
    if (NULL != event.free_handler) {
      /* free event immediately */
      event.free_handler(&event.content);
    }
  } else {
    if (true == session->is_suspended) {
      session->is_suspended = false;
      ret = session->start_cb(NULL);
    }
  }
  return ret;
}

Result SessionStop(Session *session) {
  Result ret = session->stop_cb();
  if (ret == E_HOLD) {
    session->is_suspended = true;
  }
  return ret;
}
