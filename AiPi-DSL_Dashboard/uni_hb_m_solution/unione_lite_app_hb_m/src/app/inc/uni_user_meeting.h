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
 * Description : uni_user_meeting.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.03
 *
 **************************************************************************/
 #ifndef APP_INC_UNI_USER_MEETING_H_
#define APP_INC_UNI_USER_MEETING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "uni_event.h"
#include "user_event.h"

typedef enum {
  USER_VOLUME_SET_MIN = 0,
  USER_VOLUME_SET_MAX,
  USER_VOLUME_SET_MID,
  USER_VOLUME_SET_UP,
  USER_VOLUME_SET_DOWN
}USER_VOLUME_SET_TYPE;

int uni_user_meeting_init(void);

void uni_user_meeting_final(void);

uni_bool uni_user_meeting_is_subscribe(USER_EVENT_TYPE event);

int uni_user_meeting_send_event(USER_EVENT_TYPE event,
                                         user_event_context_t *context);

int uni_user_meeting_will_reply(void);

uni_bool uni_user_meeting_is_reply(void);

int uni_user_meeting_send_ucp(Event *event);

int uni_user_meeting_append_asr_blacklist(const char *word);

int uni_user_meeting_remove_asr_blacklist(const char *word);

int uni_user_meeting_enable_asr_timeout(uni_bool is_enable);

int uni_user_meeting_speaker_mute(uni_bool is_mute);

int uni_user_meeting_shutup_mode(uni_bool is_shutup);

#ifdef __cplusplus
}
#endif
#endif

