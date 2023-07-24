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
 * Description : event_route.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.9.19
 *
 **********************************************************************/

#ifndef UTILS_EVENT_ROUTE_INC_UNI_EVENT_ROUTE_H_
#define UTILS_EVENT_ROUTE_INC_UNI_EVENT_ROUTE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_event.h"

#define MAX_ALLOWED_EVENT_TYPE 1023

/**
 * Usage:   Used to register a event handler with event router
 * Params:  handler    Callback routine to handle the event
 *          event_num  Specify how many event can be handled by the handler
 *          ...        <event_num> Events, variable-length param list
 * Return:  Result of registration, -1 means failure, 0 means ok
 */
int EventRouteSubscribe(EventHandler handler, int event_num, const uni_s32 events[]);

/**
 * Usage:   Used to unregister a event handler with event router
 * Params:  handler    Callback routine of the handler
 * Return:  Result of unregistration, -1 means failure, 0 means ok
 */
int EventRouteUnregister(EventHandler handler);

/**
 * Usage:   Used to process an event, event will only be handled by one event
 *          handler
 * Params:  event    Event to be handled
 * Return:  Result of the processing, -1 means failure, 0 means ok
 */
int EventRouteProcess(Event *event);

/**
 * Usage:   Used to process an event, event will be handled by all the handler
 *          registered for it one by one
 * Params:  event    Event to be handled
 * Return:  Result of the processing, return 0 only if all the handler run well
 */
int EventRouteProcessAll(Event *event);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_EVENT_ROUTE_INC_UNI_EVENT_ROUTE_H_
