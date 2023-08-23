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
 * Description : event_route.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.9.19
 *
 **********************************************************************/

#include "uni_event_route.h"
#include "uni_watchdog_session.h"
#include "uni_log.h"
#include "list_head.h"
#include "uni_bitmap.h"

#define EVENT_ROUTE_TAG "event_route"

typedef struct {
  uni_list_head link;
  EventHandler  handler;
  BitMap        *event_set;
} EventReceiver;

static uni_list_head g_event_receivers = UNI_LIST_HEAD_INIT(g_event_receivers);

static EventReceiver *_find_event_receiver(EventHandler handler) {
  EventReceiver *receiver = NULL;
  uni_list_for_each_entry(receiver, &g_event_receivers, EventReceiver, link) {
    if (handler == receiver->handler) {
      return receiver;
    }
  }
  return NULL;
}

int EventRouteSubscribe(EventHandler handler, int event_num, const uni_s32 events[]) {
  int i = 0;
  EventReceiver *receiver = NULL;
  if (NULL == handler || NULL != _find_event_receiver(handler)) {
    LOGE(EVENT_ROUTE_TAG, "handler is NULL or repeatedly registered");
    return -1;
  }
  receiver = (EventReceiver *)uni_malloc(sizeof(EventReceiver));
  if (NULL == receiver) {
    LOGE(EVENT_ROUTE_TAG, "memory alloc failed");
    return -1;
  }
  receiver->event_set = BitMapNew(MAX_ALLOWED_EVENT_TYPE + 1);
  if (NULL == receiver->event_set) {
    uni_free(receiver);
    LOGE(EVENT_ROUTE_TAG, "create bitmap failed");
    return -1;
  }
  receiver->handler = handler;
  for (i = 0; i < event_num; i++) {
    if (0 > BitMapSet(receiver->event_set, events[i])) {
      LOGW(EVENT_ROUTE_TAG, "register for event %d failed, max value is %d",
           events[i], MAX_ALLOWED_EVENT_TYPE);
    }
  }
  uni_list_add_tail(&receiver->link, &g_event_receivers);
  return 0;
}

int EventRouteUnregister(EventHandler handler) {
  EventReceiver *p = NULL, *n = NULL;
  uni_list_for_each_entry_safe(p, n, &g_event_receivers, EventReceiver, link) {
    if (p->handler == handler) {
      uni_list_del(&p->link);
      BitMapDel(p->event_set);
      uni_free(p);
      break;
    }
  }
  return 0;
}

int EventRouteProcess(Event *event) {
  EventReceiver *receiver = NULL;
  int ret;
  uni_list_for_each_entry(receiver, &g_event_receivers, EventReceiver, link) {
    if (0 == BitMapTest(receiver->event_set, event->type)) {
      LOGT(EVENT_ROUTE_TAG, "receiver found for event %d", event->type);
      ret = receiver->handler(event);
      if (ret < 0) {
        WatchDogReboot();
      }
      return ret;
    }
  }
  LOGW(EVENT_ROUTE_TAG, "receiver not found for event %d", event->type);
  if (event->free_handler) {
    event->free_handler(&event->content);
  }
  return 0;
}

int EventRouteProcessAll(Event *event) {
  int rc = 0;
  EventReceiver *receiver = NULL;
  uni_list_for_each_entry(receiver, &g_event_receivers, EventReceiver, link) {
    if (0 == BitMapTest(receiver->event_set, event->type)) {
      if (0 > receiver->handler(event)) {
        rc = -1;
      }
    }
  }
  return rc;
}
