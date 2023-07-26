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
 * Description : uni_event_list.h
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_EVENT_LIST_INC_UNI_EVENT_LIST_H_
#define UTILS_EVENT_LIST_INC_UNI_EVENT_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_event.h"

typedef void* EventListHandle;

#define EVENT_LIST_PRIORITY_HIGHEST  (1 << 0)
#define EVENT_LIST_PRIORITY_MEDIUM   (1 << 1)
#define EVENT_LIST_PRIORITY_LOWEST   (1 << 2)

EventListHandle EventListCreate(EventHandler event_handler, unsigned int stack_size);
int             EventListDestroy(EventListHandle handle);
int             EventListAdd(EventListHandle handle, Event *event,
                             int priority);
int             EventListClear(EventListHandle handle);
int             EventListTraverse(EventListHandle handle,
                                  EventHandler traverse_handler);

#ifdef __cplusplus
}
#endif
#endif
