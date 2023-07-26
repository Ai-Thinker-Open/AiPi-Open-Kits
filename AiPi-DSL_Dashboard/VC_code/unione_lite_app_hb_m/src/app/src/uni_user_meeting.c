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
#include "uni_user_meeting.h"
#include "uni_media_player.h"
#include "user_uni_ucp.h"
#include "uni_recog_service.h"
#include "uni_json.h"
#include "user_uni_sucp.h"
#include "uni_cmd_code.h"

#define TAG "user_meeting"

#define CHECK_FREE(str) if(str) {uni_free(str);}

#define UNI_CMD_NUM 1001

typedef struct {
  uni_bool      is_reply;
}user_meeting_context_t;

static user_meeting_context_t g_meeting_context = {0};

int uni_user_meeting_init(void) {
#if OPEN_ASR_EVENT_UCP
#if USE_UNIONE_PROTOCOL
  if (0 != user_uni_ucp_init(NULL)) {
    LOGE(TAG, "user_uni_ucp_init faild.");
    return -1;
  }
#elif USE_SAMPLE_PROTOCOL
  if (0 != user_uni_sucp_init(NULL)) {
    LOGE(TAG, "user_uni_sucp_init faild.");
    return -1;
  }
#endif
#endif
  return 0;
}

void uni_user_meeting_final(void) {
#if OPEN_ASR_EVENT_UCP
#if USE_UNIONE_PROTOCOL
  user_uni_ucp_final();
#elif USE_SAMPLE_PROTOCOL
  user_uni_sucp_final();
#endif
#endif
}

uni_bool uni_user_meeting_is_subscribe(USER_EVENT_TYPE event) {
  if (user_event_get_observer(event)) {
    return true;
  }
  return false;
}

int uni_user_meeting_will_reply(void) {
  g_meeting_context.is_reply = true;
  return 0;
}

uni_bool uni_user_meeting_is_reply(void) {
  return g_meeting_context.is_reply;
}

int uni_user_meeting_send_event(USER_EVENT_TYPE event,
                                         user_event_context_t *context) {
  int ret = -1;
  _user_event_cb cb_function = user_event_get_observer(event);
  if (NULL != cb_function) {
    LOGT(TAG, "Callback function of event: %d", event);
    g_meeting_context.is_reply = false;
    cb_function(event, context);
    ret = 0;
  }
  switch (event) {
    case USER_AUDIO_PLAY_START:
      CHECK_FREE(context->audio_play.file_name);
      break;
    case USER_AUDIO_PLAY_END:
      break;
    case USER_CUSTOM_SETTING:
      CHECK_FREE(context->custom_setting.cmd);
      CHECK_FREE(context->custom_setting.word_str);
      CHECK_FREE(context->custom_setting.reply_files);
      break;
    case USER_VOLUME_SETTING:
      CHECK_FREE(context->voluem_setting.cmd);
      CHECK_FREE(context->voluem_setting.word_str);
      CHECK_FREE(context->voluem_setting.reply_files);
      break;
    case USER_GOTO_SLEEPING:
      CHECK_FREE(context->goto_sleeping.cmd);
      CHECK_FREE(context->goto_sleeping.word_str);
      CHECK_FREE(context->goto_sleeping.reply_files);
      break;
    case USER_GOTO_AWAKENED:
      CHECK_FREE(context->goto_awakend.cmd);
      CHECK_FREE(context->goto_awakend.word_str);
      CHECK_FREE(context->goto_awakend.reply_files);
      break;
    default:
      break;
  }
  return ret;
}

int uni_user_meeting_send_ucp(Event *event) {
#if OPEN_ASR_EVENT_UCP
  uni_lasr_result_t *content = (uni_lasr_result_t *)(event->content.info);
  char cmd_str[16] = {0};   // the max action string length is 15
  if (NULL == event) {
    LOGE(TAG, "Event is NULL.");
    return -1;
  }
  switch (event->type) {
    case VUI_APP_SETTING_EVENT:
      if (content->cmd) {
        uni_strncpy(cmd_str, content->cmd, sizeof(cmd_str)-1);
      }
      break;
    case VUI_APP_VOLUME_SETTING_EVENT:
      if (content->cmd) {
        uni_strncpy(cmd_str, content->cmd, sizeof(cmd_str)-1);
      }
      break;
    case VUI_APP_WAKEUP_EVENT:
      uni_strncpy(cmd_str, "wakeup_uni", sizeof(cmd_str)-1);
      break;
    case APP_GOTO_WAKEUP_EVENT:
      uni_strncpy(cmd_str, "wakeup_uni", sizeof(cmd_str)-1);
      break;
    case VUI_LOCAL_ASR_TIMEOUT_EVENT:
      uni_strncpy(cmd_str, "exitUni", sizeof(cmd_str)-1);
      break;
    case VUI_APP_SLEEP_EVENT:
      uni_strncpy(cmd_str, "exitUni", sizeof(cmd_str)-1);
      break;
    default:
      break;
  }
#if USE_UNIONE_PROTOCOL
  user_uni_ucp_send(UNI_CMD_NUM, cmd_str, uni_strlen(cmd_str), false);
#elif USE_SAMPLE_PROTOCOL
  int i = 0;
  for (i = 0; i < (sizeof(g_cmd_code_arry) / sizeof(cmd_code_map_t)); i++) {
    if (0 == uni_strcmp(g_cmd_code_arry[i].cmd_str, cmd_str)) {
      user_uni_sucp_send(g_cmd_code_arry[i].cmd_code, NULL);
    }
  }
#endif
#endif
  return 0;
}

int uni_user_meeting_append_asr_blacklist(const char *word) {
  if (E_OK != RecogBlacklist(word, true)) {
    LOGE(TAG, "RecogBlacklist failed.");
    return -1;
  }
  return 0;
}

int uni_user_meeting_remove_asr_blacklist(const char *word) {
  if (E_OK != RecogBlacklist(word, false)) {
    LOGE(TAG, "RecogBlacklist failed.");
    return -1;
  }
  return 0;
}

int uni_user_meeting_enable_asr_timeout(uni_bool is_enable) {
  if (E_OK != RecogEnableTimeout(is_enable)) {
    LOGE(TAG, "RecogEnableTimeout failed.");
    return -1;
  }
  return 0;
}

int uni_user_meeting_speaker_mute(uni_bool is_mute) {
  Result result = E_FAILED;
  result = MediaPlayerSetMute(is_mute);
  if (E_FAILED == result) {
    LOGT(TAG, "MediaPlayerSetMute failed.");
    return -1;
  }
  return 0;
}

int uni_user_meeting_shutup_mode(uni_bool is_shutup) {
  Result result = E_FAILED;
  result = MediaPlayerShutUp(is_shutup);
  if (E_FAILED == result) {
    LOGT(TAG, "MediaPlayerShutUp failed.");
    return -1;
  }
  return 0;
}

