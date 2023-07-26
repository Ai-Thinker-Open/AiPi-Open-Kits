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
 * Description : uni_session_priority.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.6.29
 *
 **************************************************************************/

#ifndef APP_INCS_UNI_PRIORITY_H_
#define APP_INCS_UNI_PRIORITY_H_

#ifdef __cplusplus
extern "C" {
#endif

enum SessionPriority {
  /* invalid priority */
  SESSION_PRIORITY_INVALID = -1,
  /* session group with priority 0 */
  SESSION_PRIORITY_WATCHDOG = 0,
  /* session group with priority 1 */
  SESSION_PRIORITY_CHAT = 1,
  SESSION_PRIORITY_RESET = 1,
  SESSION_PRIORITY_MUSIC = 1,
  SESSION_PRIORITY_TIMERSET = 1,
  SESSION_PRIORITY_TIMERUP = 1,
  SESSION_PRIORITY_WAKEUP = 1,
  SESSION_PRIORITY_WEATHER = 1,
  SESSION_PRIORITY_SETTING = 1,
  SESSION_PRIORITY_NETWORK = 1,
  SESSION_PRIORITY_STOCK = 1,
  SESSION_PRIORITY_MEMO = 1,
  SESSION_PRIORITY_BLUETOOTH = 1,
  /* max priority */
  SESSION_PRIORITY_MAX = 100
};

#ifdef __cplusplus
}
#endif
#endif  //  APP_INCS_UNI_PRIORITY_H_
