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
 * Description : uni_session_manage.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.6.5
 *
 **************************************************************************/

#ifndef APP_INC_UNI_SESSION_MANAGE_H_
#define APP_INC_UNI_SESSION_MANAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_session.h"

/**
 * Usage:   Used to register a session with session manager
 * Params:  session    session to be registered
 * Return:  Result of registration
 */
Result SessionManageRegister(Session *session);

/**
 * Usage:   Used to unregister a session
 * Params:  session    session to be unregistered
 * Return:  Result of unregistration
 */
Result SessionManageUnregister(Session *session);

/**
 * Usage:   put events into black list
 * Params:  event    number of events to be put in list
 * Return:  Result of setting
 */
Result SessionManageSetBlackList(int event_num, ...);

/**
 * Usage:   put events into white list
 *          to block all events by 0 size white list
 * Params:  event_num  number of events to be put in list
 * Return:  Result of setting
 */
Result SessionManageSetWhiteList(int event_num, ...);

/**
 * Usage:   Initialize session manager
 * Params:
 * Return:  Result of initialization
 */
Result SessionManageInit(void);

/**
 * Usage:   Finalize session manager
 * Params:
 * Return:  Result of finalization
 */
Result SessionManageFinal(void);

#ifdef __cplusplus
}
#endif
#endif  //  APP_INC_UNI_SESSION_MANAGE_H_
