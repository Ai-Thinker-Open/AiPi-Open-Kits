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
 * Description : user_event.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.03
 *
 **************************************************************************/
#include "user_event.h"

#define TAG "user_event"

static _user_event_cb g_observers[USER_EVENT_MAX] = {NULL};

int user_event_subscribe_event(USER_EVENT_TYPE event,
                                        _user_event_cb cb) {
  if (event <= USER_INVALID_EVENT || event >= USER_EVENT_MAX) {
    LOGE(TAG, "Inviled event type: %d", event);
    return -1;
  }
  g_observers[event] = cb;
  return 0;
}

int user_event_clear_observers(void) {
  uni_memset(g_observers, 0, sizeof(g_observers));
  return 0;
}

_user_event_cb user_event_get_observer(USER_EVENT_TYPE event) {
  if (event <= USER_INVALID_EVENT || event >= USER_EVENT_MAX) {
    LOGE(TAG, "Inviled event type: %d", event);
    return NULL;
  }
  return g_observers[event];
}

