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
 * Description : user_player.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.07
 *
 **************************************************************************/
#include "user_player.h"
#include "uni_event.h"
#include "uni_user_meeting.h"
#include "uni_event_route.h"
#include "uni_pcm_default.h"

#define TAG "user_player"

static char *_creat_buf_save_str(const char *str) {
  char *buf = NULL;
  int str_len = 0;
  if (NULL == str) {
    return NULL;
  }
  str_len = uni_strlen(str);
  buf = (char *)uni_malloc(str_len + 1);
  if (buf) {
    uni_memset(buf, 0, str_len + 1);
    uni_strncpy(buf, str, str_len);
  }
  return buf;
}

static void _creat_and_send_play_event(AUDIO_PLAY_TYPE type,
                                                const char *file) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.info = _creat_buf_save_str(file);
  event_content.value_int = type;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_AUDIO_PLAY_EVENT),
                      &event_content, DefaultEventContentFreeHandler);
  EventRouteProcess(event);
  EventFree(event);
  LOGT(TAG, "send USER_AUDIO_PLAY_EVENT event");
}

int user_player_play(AUDIO_PLAY_TYPE type, const char *file) {
  if (NULL == file) {
    LOGE(TAG, "Invalid file name is NULL");
    return -1;
  }
  switch (type) {
    case AUDIO_PLAY_REPLY:
      uni_user_meeting_will_reply();
      _creat_and_send_play_event(type, file);
      break;
    case AUDIO_PLAY_MUSIC:
      _creat_and_send_play_event(type, file);
      break;
    case AUDIO_PLAY_SKILL:
      _creat_and_send_play_event(type, file);
      break;
    default:
      break;
  }
  return 0;
}

int user_player_reply_list_num(const char *file_list, int num) {
  char *pcm = NULL;
  if (NULL == file_list) {
    LOGE(TAG, "Invalid file name is NULL");
    return -1;
  }
  pcm = uni_get_number_pcm(file_list, num);
  if (0 == uni_strcmp(pcm, "-1")) {
    LOGE(TAG, "Cannot found %dst file in list %s", num, file_list);
    return -1;
  }
  uni_user_meeting_will_reply();
  _creat_and_send_play_event(AUDIO_PLAY_REPLY, pcm);
  return 0;
}

int user_player_reply_list_random(const char *file_list) {
  char *pcm = NULL;
  if (NULL == file_list) {
    LOGE(TAG, "Invalid file name is NULL");
    return -1;
  }
  pcm = uni_get_random_pcm(file_list);
  if (0 == uni_strcmp(pcm, "-1")) {
    LOGE(TAG, "Cannot found any file in list %s", file_list);
    return -1;
  }
  uni_user_meeting_will_reply();
  _creat_and_send_play_event(AUDIO_PLAY_REPLY, pcm);
  return 0;
}

int user_player_reply_list_in_order(const char *file_list) {
  if (NULL == file_list) {
    LOGE(TAG, "Invalid file name is NULL");
    return -1;
  }
  uni_user_meeting_will_reply();
  _creat_and_send_play_event(AUDIO_PLAY_REPLY, file_list);
  return 0;
}

static void _creat_and_send_stop_event(AUDIO_PLAY_TYPE type) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.value_int = type;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_AUDIO_STOP_EVENT),
                      &event_content, DefaultEventContentFreeHandler);
  EventRouteProcess(event);
  EventFree(event);
  LOGT(TAG, "send USER_AUDIO_STOP_EVENT event");
}

int user_player_stop(AUDIO_PLAY_TYPE type) {
  _creat_and_send_stop_event(type);
  return 0;
}

static void _creat_and_send_volume_event(USER_VOLUME_SET_TYPE type) {
  Event *event = NULL;
  EventContent event_content;
  uni_memset(&event_content, 0, sizeof(EventContent));
  event_content.value_int = type;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(USER_VOLUME_SETTING_EVENT),
                      &event_content, DefaultEventContentFreeHandler);
  EventRouteProcess(event);
  EventFree(event);
  LOGT(TAG, "send USER_VOLUME_SETTING_EVENT event");
}

int user_player_set_volume_min(void) {
  _creat_and_send_volume_event(USER_VOLUME_SET_MIN);
  return 0;
}

int user_player_set_volume_max(void) {
  _creat_and_send_volume_event(USER_VOLUME_SET_MAX);
  return 0;
}

int user_player_set_volume_mid(void) {
  _creat_and_send_volume_event(USER_VOLUME_SET_MID);
  return 0;
}

int user_player_set_volume_up(void) {
  _creat_and_send_volume_event(USER_VOLUME_SET_UP);
  return 0;
}

int user_player_set_volume_down(void) {
  _creat_and_send_volume_event(USER_VOLUME_SET_DOWN);
  return 0;
}

int user_player_speaker_mute(void) {
  return uni_user_meeting_speaker_mute(true);
}

int user_player_speaker_unmute(void) {
  return uni_user_meeting_speaker_mute(false);
}

int user_player_shutup_mode(void) {
  return uni_user_meeting_shutup_mode(true);
}

int user_player_shutup_exit(void) {
  return uni_user_meeting_shutup_mode(false);
}

