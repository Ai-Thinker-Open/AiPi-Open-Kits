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
 * Description : uni_event.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_iot.h"
#include "uni_log.h"

#include "uni_event.h"

Event* EventCreate(unsigned int seq_id, int type, EventContent *content,
                   EventContentFreeHandler free_handler) {
  Event *event = (Event *)uni_malloc(sizeof(Event));
  if (NULL == event) {
    LOGE("event", "memory alloc failed");
    return NULL;
  }
  event->seq_id = seq_id;
  event->type = type;
  if (NULL != content) {
    uni_memcpy(&event->content, content, sizeof(EventContent));
  } else {
    uni_memset(&event->content, 0, sizeof(EventContent));
  }
  event->free_handler = free_handler;
  return event;
}

void EventFree(Event *event) {
  if (NULL == event) {
    return;
  }
  uni_free(event);
}
