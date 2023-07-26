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
 * Description : uni_wakeup_session.h
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.19
 *
 **************************************************************************/

#ifndef APP_INC_SESSIONS_UNI_WAKEUP_SESSION_H_
#define APP_INC_SESSIONS_UNI_WAKEUP_SESSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

/**
 * Usage:   Initialize wakeup session and register it to session manager
 * Params:
 * Return:  Result of initialization
 */
Result WakeupSessionInit(void);

/**
 * Usage:   Finalize wakeup session and unregister it from session manager
 * Params:
 * Return:  Result of finalization
 */
Result WakeupSessionFinal(void);

#ifdef __cplusplus
}
#endif
#endif  //  APP_INC_SESSIONS_UNI_WAKEUP_SESSION_H_
