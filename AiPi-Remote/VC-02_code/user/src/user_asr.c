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
 * Description : user_asr.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.07
 *
 **************************************************************************/
#include "user_asr.h"
#include "uni_event.h"
#include "uni_event_route.h"
#include "uni_user_meeting.h"

#define TAG "user_asr"

static void _creat_and_send_asr_event(UNI_EVENT_TYPE type) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  if (USER_GOTO_AWAKEND_EVENT == type) {
    event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_GOTO_AWAKEND_EVENT),
                        &event_content, DefaultEventContentFreeHandler);
    LOGT(TAG, "send USER_GOTO_AWAKEND_EVENT event");
  } else if (USER_GOTO_SLEEPING_EVENT == type) {
    event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_GOTO_SLEEPING_EVENT),
                        &event_content, DefaultEventContentFreeHandler);
    LOGT(TAG, "send USER_GOTO_SLEEPING_EVENT event");
  }
  EventRouteProcess(event);
  EventFree(event);
}

int user_asr_goto_sleep(void) {
  _creat_and_send_asr_event(USER_GOTO_SLEEPING_EVENT);
  return 0;
}

int user_asr_goto_awakend(void) {
  _creat_and_send_asr_event(USER_GOTO_AWAKEND_EVENT);
  return 0;
}

static void _creat_and_send_mic_event(uni_bool is_mute) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.value_int = is_mute;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_MIC_MUTE_EVENT),
                      &event_content, DefaultEventContentFreeHandler);
  LOGT(TAG, "send USER_MIC_MUTE_EVENT event");
  EventRouteProcess(event);
  EventFree(event);
}

int user_asr_recognize_disable(void) {
  _creat_and_send_mic_event(true);
  return 0;
}

int user_asr_recognize_enable(void) {
  _creat_and_send_mic_event(false);
  return 0;
}

int user_asr_word_enable(const char *word) {
  return uni_user_meeting_remove_asr_blacklist(word);
}

int user_asr_word_disable(const char *word) {
  return uni_user_meeting_append_asr_blacklist(word);
}

int user_asr_timeout_disable(void) {
  return uni_user_meeting_enable_asr_timeout(false);
}

int user_asr_timeout_enable(void) {
  return uni_user_meeting_enable_asr_timeout(true);
}

