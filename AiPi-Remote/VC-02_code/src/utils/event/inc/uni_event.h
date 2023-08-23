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
 * Description : uni_event.h
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_EVENT_INC_UNI_EVENT_H_
#define UTILS_EVENT_INC_UNI_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

#define EVENT_SIZE                     (int)sizeof(Event)
#define EVENT_SEQUENCE_ID_DEFAULT      (uni_u32)(-1)
#define EVENT_VUI_SESSION_ID_PRIVILEGE (uni_u64)(-1)

struct EventContent;
typedef void (*EventContentFreeHandler)(struct EventContent *event_content);

typedef struct EventContent {
  void    *info;
  void    *extend_info;
  uni_s32 value_int;
  double  value_double;
  uni_s32 timems;
  uni_u32 vui_session_id;
  uni_s32 err_code;
} EventContent;

typedef struct {
  uni_u32                 seq_id;
  uni_s32                 type;
  EventContent            content;
  EventContentFreeHandler free_handler;
} Event;

typedef int (*EventHandler)(Event *event);

/**
 * DefaultEventContentFreeHandler can only be used when info and extend_info
 * allocated by uni_malloc.
 */
static inline void DefaultEventContentFreeHandler(EventContent *event_content) {
  if (NULL == event_content) {
    return;
  }
  if (NULL != event_content->info) {
    uni_free(event_content->info);
    event_content->info = NULL;
  }
  if (NULL != event_content->extend_info) {
    uni_free(event_content->extend_info);
    event_content->extend_info = NULL;
  }
}

Event* EventCreate(unsigned int seq_id, int type, EventContent *content,
                   EventContentFreeHandler free_handler);
void   EventFree(Event *event);

#ifdef __cplusplus
}
#endif
#endif
