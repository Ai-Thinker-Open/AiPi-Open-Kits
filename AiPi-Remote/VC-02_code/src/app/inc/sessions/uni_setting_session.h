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
 * Description : uni_setting_session.h
 * Author      : shangjinlong@unisound.com
 * Date        : 2018.07.06
 *
 **************************************************************************/

#ifndef APP_INC_SESSIONS_UNI_SETTING_SESSION_H_
#define APP_INC_SESSIONS_UNI_SETTING_SESSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

/**
 * Usage:   Initialize setting session and register it to session manager
 * Params:
 * Return:  Result of initialization
 */
Result SettingSessionInit(void);

/**
 * Usage:   Finalize setting session and unregister it from session manager
 * Params:
 * Return:  Result of finalization
 */
Result SettingSessionFinal(void);

#ifdef __cplusplus
}
#endif
#endif  //  APP_INC_SESSIONS_UNI_SETTING_SESSION_H_