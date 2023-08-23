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
 * Description : uni_setting_session.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2018.07.06
 *
 **********************************************************************/

#include "uni_setting_session.h"

#include "uni_fsm.h"
#include "uni_session.h"
#include "uni_session_priority.h"
#include "uni_session_manage.h"
#include "uni_event_route.h"
#include "uni_recog_service.h"
#include "uni_media_player.h"
#include "uni_nlu.h"
#include "uni_log.h"
#include "uni_black_board.h"
#include "uni_media_player.h"
#include "uni_config.h"
#include "uni_pcm_default.h"
#include "uni_user_meeting.h"

#define SETTING_SESSION_TAG "setting_session"

#if USER_AUDIO_VOL_MIN
#define SETTING_VOLUME_MIN    USER_AUDIO_VOL_MIN
#else
#define SETTING_VOLUME_MIN    MEDIA_NORMAL_VOLUME_MIN
#endif
#if USER_AUDIO_VOL_MAX
#define SETTING_VOLUME_MAX    USER_AUDIO_VOL_MAX
#else
#define SETTING_VOLUME_MAX    MEDIA_VOLUME_MAX
#endif
#if USER_AUDIO_VOL_MID
#define SETTING_VOLUME_MID    USER_AUDIO_VOL_MID
#else
#define SETTING_VOLUME_MID    MEDIA_VOLUME_MID
#endif
#if USER_AUDIO_VOL_LEVEL
#define SETTING_VOLUME_LEVEL  USER_AUDIO_VOL_LEVEL
#else
#define SETTING_VOLUME_LEVEL  5
#endif
#if USER_AUDIO_VOL_DEF
#define SETTING_VOLUME_DEF    USER_AUDIO_VOL_DEF
#else
#define SETTING_VOLUME_DEF    MEDIA_VOLUME_DEFAULT
#endif
#define SETTING_VOLUME_STEP   ((MEDIA_VOLUME_MAX - MEDIA_VOLUME_MIN) / SETTING_VOLUME_LEVEL)

static Result _idle__vui_app_volume_setting(void *event_info);
static Result _idle__vui_app_setting(void *event_info);
static Result _broadcast__vui_app_volume_setting(void *event_info);
static Result _broadcast__vui_app_setting(void *event_info);
static Result _broadcast__audio_play_end(void *event_info);
static Result _broadcast__common_stop(void *event_info);
static Result _broadcast__user_audio_play(void *event_info);
static Result _broadcast__user_audio_stop(void *event_info);
static Result _idle__user_audio_play(void *event_info);
static Result _play__vui_app_volume_setting(void *event_info);
static Result _play__vui_app_setting(void *event_info);
static Result _play__aduio_play_end(void *event_info);
static Result _play__common_stop(void *event_info);
static Result _play__user_audio_play(void *event_info);
static Result _play__user_audio_stop(void *event_info);
static Result _idle__user_volume_setting(void *event_info);
static Result _idle__user_mic_mute_setting(void *event_info);
static Result _broadcast__user_volume_setting(void *event_info);
static Result _broadcast__user_mic_mute_setting(void *event_info);
static Result _play__user_volume_setting(void *event_info);
static Result _play__user_mic_mute_setting(void *event_info);

typedef enum {
  STATE_IDLE,
  STATE_BROADCAST,
  STATE_PLAY
} SettingSessionState;

typedef struct {
  Session        *session;
  MicroFsmStruct *fsm;
} SettingSession;

static Scene g_wakeup_scene = {UNI_LP_WAKEUP, 0,
                               DEFAULT_WAKEUP_SCENE_LOW_THRESHOLD,
                               DEFAULT_WAKEUP_SCENE_STD_THRESHOLD, 320, NULL};
static Scene g_setting_scene = {UNI_RASR_LASR_MODE, DEFAULT_RECOGN_SCENE_TIMEOUT,
                                DEFAULT_RECOGN_SCENE_LOW_THRESHOLD,
                                DEFAULT_RECOGN_SCENE_STD_THRESHOLD, 320, NULL};
static SettingSession     *g_setting_session = NULL;
static MicroFsmTransition *g_setting_session_transition = NULL;

static void _session_scene_init(void) {
  double tmp = 0.0;
  uni_s64 timeout = 0;
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.std_threshold", &tmp)) {
    g_wakeup_scene.std_threshold = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.lp_threshold", &tmp)) {
    g_wakeup_scene.low_threshold = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_asr.std_threshold", &tmp)) {
    g_setting_scene.std_threshold = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_asr.lp_threshold", &tmp)) {
    g_setting_scene.low_threshold = tmp;
  }
  if (0 == ConfigReadItemNumber("lasr.lasr_asr.timeout", &timeout)) {
    g_setting_scene.timeout = timeout * 1000;
  }
}

static int _session_transition_init(void) {
const MicroFsmTransition session_transition[] = {
    {STATE_IDLE,         ID(VUI_APP_VOLUME_SETTING_EVENT), _idle__vui_app_volume_setting},
    {STATE_IDLE,         ID(USER_VOLUME_SETTING_EVENT),    _idle__user_volume_setting},
    {STATE_IDLE,         ID(USER_MIC_MUTE_EVENT),          _idle__user_mic_mute_setting},
    {STATE_IDLE,         ID(VUI_APP_SETTING_EVENT),        _idle__vui_app_setting},
    {STATE_IDLE,         ID(USER_AUDIO_PLAY_EVENT),        _idle__user_audio_play},
    {STATE_BROADCAST,    ID(VUI_APP_VOLUME_SETTING_EVENT), _broadcast__vui_app_volume_setting},
    {STATE_BROADCAST,    ID(USER_VOLUME_SETTING_EVENT),    _broadcast__user_volume_setting},
    {STATE_BROADCAST,    ID(USER_MIC_MUTE_EVENT),          _broadcast__user_mic_mute_setting},
    {STATE_BROADCAST,    ID(VUI_APP_SETTING_EVENT),        _broadcast__vui_app_setting},
    {STATE_BROADCAST,    ID(AUDIO_PLAY_END_EVENT),         _broadcast__audio_play_end},
    {STATE_BROADCAST,    ID(COMMON_STOP_EVENT),            _broadcast__common_stop},
    {STATE_BROADCAST,    ID(USER_AUDIO_PLAY_EVENT),        _broadcast__user_audio_play},
    {STATE_BROADCAST,    ID(USER_AUDIO_STOP_EVENT),        _broadcast__user_audio_stop},
    {STATE_PLAY,         ID(VUI_APP_VOLUME_SETTING_EVENT), _play__vui_app_volume_setting},
    {STATE_PLAY,         ID(USER_VOLUME_SETTING_EVENT),    _play__user_volume_setting},
    {STATE_PLAY,         ID(USER_MIC_MUTE_EVENT),          _play__user_mic_mute_setting},
    {STATE_PLAY,         ID(VUI_APP_SETTING_EVENT),        _play__vui_app_setting},
    {STATE_PLAY,         ID(AUDIO_PLAY_END_EVENT),         _play__aduio_play_end},
    {STATE_PLAY,         ID(COMMON_STOP_EVENT),            _play__common_stop},
    {STATE_PLAY,         ID(USER_AUDIO_PLAY_EVENT),        _play__user_audio_play},
    {STATE_PLAY,         ID(USER_AUDIO_STOP_EVENT),        _play__user_audio_stop},
  };
  g_setting_session_transition = uni_malloc(sizeof(session_transition));
  if (NULL == g_setting_session_transition) {
    LOGE(SETTING_SESSION_TAG, "malloc failed !");
    return 0;
  }
  uni_memcpy(g_setting_session_transition, session_transition,
             sizeof(session_transition));
  return (sizeof(session_transition) / sizeof(session_transition[0]));
}

static const char* _setting_session_state_2_str(uni_s32 state) {
  static const char *state_str[] = {
    [STATE_IDLE]      = "STATE_IDLE",
    [STATE_BROADCAST] = "STATE_BROADCAST",
  };
  if (state < STATE_IDLE || STATE_BROADCAST < state) {
    return "N/A";
  }
  return state_str[state];
}

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

static Result _response_pcm(const char *command, const char *pcm) {
  user_event_context_t context = {{0}};
  LOGT(SETTING_SESSION_TAG, "playing pcm: %s", pcm);
  /* User event send from app layer, because of maybe need some process */
  if (uni_user_meeting_is_subscribe(USER_AUDIO_PLAY_START)) {
    context.audio_play.type = AUDIO_PLAY_REPLY;
    context.audio_play.file_name = _creat_buf_save_str(pcm);
    uni_user_meeting_send_event(USER_AUDIO_PLAY_START, &context);
  }
  RecogStop();
  return MediaPlayerStart(PLAYER_PCM, pcm);
}

#if !MULTI_DIALOGUE_MODE
static void _goto_wakeup(void) {
  Event *event = NULL;
  event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(APP_GOTO_WAKEUP_EVENT),
                      NULL, NULL);
  EventRouteProcess(event);
  EventFree(event);
}
#endif

static Result _action_idle(void) {
  FsmSetState(g_setting_session->fsm, STATE_IDLE);
  return E_OK;
}

static Result _reply_volume_set(const char *reply, uni_bool silent) {
  if (silent) {
    FsmSetState(g_setting_session->fsm, STATE_IDLE);
    return E_OK;
  }
  if (E_OK == _response_pcm("volumeSet", reply)) {
    FsmSetState(g_setting_session->fsm, STATE_BROADCAST);
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(SETTING_SESSION_TAG, "reply failed: %s", reply);
#if MULTI_DIALOGUE_MODE
    RecogLaunch(&g_setting_scene);
#else
    _goto_wakeup();
#endif
    return _action_idle();
  }
}

static Result _set_volume_min(uni_lasr_result_t *content, uni_bool silent) {
  char reply[16] = {0};
  Result rc = E_OK;
  MediaPlayerVolumeSet(SETTING_VOLUME_MIN);
  if (content->pcm) {
    uni_snprintf(reply, sizeof(reply), "%s", uni_get_random_pcm(content->pcm));
    rc = _reply_volume_set(reply, silent);
  }
  return rc;
}

static Result _set_volume_max(uni_lasr_result_t *content, uni_bool silent) {
  char reply[16] = {0};
  Result rc = E_OK;
  MediaPlayerVolumeSet(SETTING_VOLUME_MAX);
  if (content->pcm) {
    uni_snprintf(reply, sizeof(reply), "%s", uni_get_random_pcm(content->pcm));
    rc = _reply_volume_set(reply, silent);
  }
  return rc;
}

static Result _set_volume_mid(uni_lasr_result_t *content, uni_bool silent) {
  char reply[16] = {0};
  Result rc = E_OK;
  MediaPlayerVolumeSet(SETTING_VOLUME_MID);
  if (content->pcm) {
    uni_snprintf(reply, sizeof(reply), "%s", uni_get_random_pcm(content->pcm));
    rc = _reply_volume_set(reply, silent);
  }
  return rc;
}

static Result _set_volume_decrease(uni_lasr_result_t *content, uni_bool silent) {
  char reply[16] = {0};
  Result rc = E_OK;
  uni_s32 volume = MediaPlayerVolumeGet();
  if (SETTING_VOLUME_MIN >= volume) {
    if ((NULL == content->pcm) || (0 == uni_strcmp(content->pcm, "[]"))) {
      uni_strcpy(reply, "-1");
    } else {
#if defined(DEFAULT_PCM_VOL_MIN)
      uni_snprintf(reply, sizeof(reply), "%s",
                   uni_get_random_pcm(DEFAULT_PCM_VOL_MIN));
#elif defined(LOCAL_TONE_MIN_VOLUME)
      uni_snprintf(reply, sizeof(reply), "%s",
                  uni_get_random_pcm(LOCAL_TONE_MIN_VOLUME));
#else
      uni_snprintf(reply, sizeof(reply), "%s",
                   uni_get_random_pcm(VOLUME_MIN_NTF));
#endif
    }
  } else {
    volume = uni_max(SETTING_VOLUME_MIN, volume - SETTING_VOLUME_STEP);
    MediaPlayerVolumeSet(volume);
    if (content->pcm) {
      uni_snprintf(reply, sizeof(reply), "%s", uni_get_random_pcm(content->pcm));
    } else {
      uni_strcpy(reply, "-1");
    }
  }
  rc = _reply_volume_set(reply, silent);
  return rc;
}

static Result _set_volume_increase(uni_lasr_result_t *content, uni_bool silent) {
  char reply[16] = {0};
  Result rc = E_OK;
  uni_s32 volume = MediaPlayerVolumeGet();
  if (MEDIA_VOLUME_MAX <= volume) {
    if ((NULL == content->pcm) || (0 == uni_strcmp(content->pcm, "[]"))) {
      uni_strcpy(reply, "-1");
    } else {
#if defined(DEFAULT_PCM_VOL_MAX)
      uni_snprintf(reply, sizeof(reply), "%s",
                   uni_get_random_pcm(DEFAULT_PCM_VOL_MAX));
#elif defined(LOCAL_TONE_MAX_VOLUME)
      uni_snprintf(reply, sizeof(reply), "%s",
                   uni_get_random_pcm(LOCAL_TONE_MAX_VOLUME));
#else
      uni_snprintf(reply, sizeof(reply), "%s",
                   uni_get_random_pcm(VOLUME_MAX_NTF));
#endif
    }
  } else {
    if (SETTING_VOLUME_MIN >= volume) {
      volume = SETTING_VOLUME_MIN;
    } 
    volume = uni_min(SETTING_VOLUME_MAX, volume + SETTING_VOLUME_STEP);
    MediaPlayerVolumeSet(volume);
    if (content->pcm) {
      uni_snprintf(reply, sizeof(reply), "%s", uni_get_random_pcm(content->pcm));
    } else {
      uni_strcpy(reply, "-1");
    }
  }
  rc = _reply_volume_set(reply, silent);
  return rc;
}

static Result _volume_setting_process(uni_lasr_result_t *content, uni_bool silent) {
  Result rc = E_OK;
  user_event_context_t context = {{0}};
  if (NULL == content) {
    LOGW(SETTING_SESSION_TAG, "content not exist");
    goto L_END;
  }
  if (NULL == content->cmd) {
    LOGW(SETTING_SESSION_TAG, "read cmd failed");
    goto L_END;
  }
  if (uni_user_meeting_is_subscribe(USER_VOLUME_SETTING)) {
    context.voluem_setting.value = MediaPlayerVolumeGet();
    context.voluem_setting.min_val = SETTING_VOLUME_MIN;
    context.voluem_setting.max_val = SETTING_VOLUME_MAX;
    context.voluem_setting.trigger = EVENT_TRIGGER_ASR;
    context.voluem_setting.cmd = _creat_buf_save_str(content->cmd);
    context.voluem_setting.word_str = _creat_buf_save_str(content->asr);
    context.voluem_setting.reply_files = _creat_buf_save_str(content->pcm);
    uni_user_meeting_send_event(USER_VOLUME_SETTING, &context);
    if (uni_user_meeting_is_reply()) {
      FsmSetState(g_setting_session->fsm, STATE_BROADCAST);
#if WAKEUP_INTERRUPT_MODE
      RecogLaunch(&g_wakeup_scene);
#else
      RecogStop();
#endif
      return E_HOLD;
    } else {
#if MULTI_DIALOGUE_MODE
      RecogLaunch(&g_setting_scene);
#else
      _goto_wakeup();
#endif
      return _action_idle();
    }
  }
  if (0 == uni_strcmp(content->cmd, "volumeMinUni")) {
    rc = _set_volume_min(content, silent);
  } else if (0 == uni_strcmp(content->cmd, "volumeMaxUni")) {
    rc = _set_volume_max(content, silent);
  } else if (0 == uni_strcmp(content->cmd, "volumeMidUni")) {
    rc = _set_volume_mid(content, silent);
  } else if (0 == uni_strcmp(content->cmd, "volumeDownUni")) {
    rc = _set_volume_decrease(content, silent);
  } else if (0 == uni_strcmp(content->cmd, "volumeUpUni")) {
    rc = _set_volume_increase(content, silent);
  } else {
    LOGE(SETTING_SESSION_TAG, "unsupport cmd=%s", content->cmd);
    rc = E_FAILED;
  }
L_END:
  return rc;
}

static Result _central_control(char *command, char *pcm) {
  if (E_OK == _response_pcm(command, pcm)) {
    FsmSetState(g_setting_session->fsm, STATE_BROADCAST);
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(SETTING_SESSION_TAG, "response pcm failed");
#if MULTI_DIALOGUE_MODE
    RecogLaunch(&g_setting_scene);
#else
    _goto_wakeup();
#endif
    return _action_idle();
  }
}

static Result _setting_central_control_process(uni_lasr_result_t *content) {
  Result ret = E_FAILED;
  user_event_context_t context = {{0}};
  if (NULL == content) {
    LOGW(SETTING_SESSION_TAG, "content not exist");
    return E_FAILED;
  }
  if (NULL == content->cmd) {
    LOGW(SETTING_SESSION_TAG, "read deviceType or operands failed");
    goto L_END;
  }
  if (NULL == content->pcm) {
    LOGW(SETTING_SESSION_TAG, "read pcm failed");
    goto L_END;
  }
  if (uni_user_meeting_is_subscribe(USER_CUSTOM_SETTING)) {
    LOGT(SETTING_SESSION_TAG, "user reply process");
    context.custom_setting.cmd = _creat_buf_save_str(content->cmd);
    context.custom_setting.reply_files = _creat_buf_save_str(content->pcm);
    context.custom_setting.word_str = _creat_buf_save_str(content->asr);
    uni_user_meeting_send_event(USER_CUSTOM_SETTING, &context);
    if (uni_user_meeting_is_reply()) {
      FsmSetState(g_setting_session->fsm, STATE_BROADCAST);
#if WAKEUP_INTERRUPT_MODE
      RecogLaunch(&g_wakeup_scene);
#else
      RecogStop();
#endif
      return E_HOLD;
    } else {
#if MULTI_DIALOGUE_MODE
      RecogLaunch(&g_setting_scene);
#else
      _goto_wakeup();
#endif
      return _action_idle();
    }
  }
  ret = _central_control(content->cmd, uni_get_random_pcm(content->pcm));
L_END:
  return ret;
}

static Result _setting_process(uni_lasr_result_t *content) {
  Result ret;
  if (E_FAILED != (ret = _setting_central_control_process(content))) {
    return ret;
  }
  LOGW(SETTING_SESSION_TAG, "unsupport operands");
  return E_FAILED;
}

static Result _filter_event(Event *event) {
  Result rc = E_OK;
  return rc;
}

static Result _idle__vui_app_volume_setting(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  return _volume_setting_process((uni_lasr_result_t *)(((Event *)event)->content.info), false);
}

static Result _idle__vui_app_setting(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  return _setting_process((uni_lasr_result_t *)(((Event *)event)->content.info));
}

static void _stop_player(void) {
  MediaPlayerStop(PLAYER_PCM);
  /* User event send from app layer, because of maybe need some process */
  /*
  user_event_context_t context = {{0}};
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = false;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
  */
}

static Result _broadcast__vui_app_volume_setting(void *event) {
  return _idle__vui_app_volume_setting(event);
}

static Result _broadcast__vui_app_setting(void *event_info) {
  return _idle__vui_app_setting(event_info);
}

static Result _broadcast__audio_play_end(void *event) {
  user_event_context_t context = {{0}};
  LOGT(SETTING_SESSION_TAG, "action called");
  /* User event send from app layer, because of maybe need some process */
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = true;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
#if MULTI_DIALOGUE_MODE
  RecogLaunch(&g_setting_scene);
#else
  _goto_wakeup();
#endif
  return _action_idle();
}

static Result _broadcast__common_stop(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  _stop_player();
#if MULTI_DIALOGUE_MODE
  RecogLaunch(&g_setting_scene);
#else
  _goto_wakeup();
#endif
  return _action_idle();
}

static Result _broadcast__user_audio_play(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(SETTING_SESSION_TAG, "response pcm failed");
#if MULTI_DIALOGUE_MODE
    RecogLaunch(&g_setting_scene);
#else
    _goto_wakeup();
#endif
    return _action_idle();
  }
}

static Result _broadcast__user_audio_stop(void *event_info) {
  LOGT(SETTING_SESSION_TAG, "action called");
  _stop_player();
#if MULTI_DIALOGUE_MODE
  RecogLaunch(&g_setting_scene);
#else
  _goto_wakeup();
#endif
  return _action_idle();
}

static Result _idle__user_audio_play(void *event_info) {
  LOGT(SETTING_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event_info)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_setting_session->fsm, STATE_PLAY);
    return E_HOLD;
  } else {
    LOGE(SETTING_SESSION_TAG, "response pcm failed");
    return E_FAILED;
  }
}

static Result _play__vui_app_volume_setting(void *event_info) {
  return _idle__vui_app_volume_setting(event_info);
}

static Result _play__vui_app_setting(void *event_info) {
  return _idle__vui_app_setting(event_info);
}

static Result _play__aduio_play_end(void *event) {
  user_event_context_t context = {{0}};
  LOGT(SETTING_SESSION_TAG, "action called");
  /* User event send from app layer, because of maybe need some process */
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = true;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
#if MULTI_DIALOGUE_MODE
    RecogLaunch(&g_setting_scene);
#else
    _goto_wakeup();
#endif
  return _action_idle();
}

static Result _play__common_stop(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  _stop_player();
#if MULTI_DIALOGUE_MODE
  RecogLaunch(&g_setting_scene);
#else
  _goto_wakeup();
#endif
  return _action_idle();
}

static Result _play__user_audio_play(void *event) {
  LOGT(SETTING_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(SETTING_SESSION_TAG, "response pcm failed");
#if MULTI_DIALOGUE_MODE
    RecogLaunch(&g_setting_scene);
#else
    _goto_wakeup();
#endif
    return _action_idle();
  }
}

static Result _play__user_audio_stop(void *event_info) {
  LOGT(SETTING_SESSION_TAG, "action called");
  _stop_player();
#if MULTI_DIALOGUE_MODE
  RecogLaunch(&g_setting_scene);
#else
  _goto_wakeup();
#endif
  return _action_idle();
}

static void _user_set_volume_process(void *event_info) {
  Event *event = (Event *)event_info;
  uni_s32 volume = MediaPlayerVolumeGet();
  switch (event->content.value_int) {
    case USER_VOLUME_SET_MIN:
      MediaPlayerVolumeSet(SETTING_VOLUME_MIN);
      break;
    case USER_VOLUME_SET_MAX:
      MediaPlayerVolumeSet(SETTING_VOLUME_MAX);
      break;
    case USER_VOLUME_SET_MID:
      MediaPlayerVolumeSet(SETTING_VOLUME_MID);
      break;
    case USER_VOLUME_SET_UP:
      if (SETTING_VOLUME_MIN >= volume) {
        volume = SETTING_VOLUME_MIN;
      } 
      volume = uni_min(SETTING_VOLUME_MAX, volume + SETTING_VOLUME_STEP);
      MediaPlayerVolumeSet(volume);
      break;
    case USER_VOLUME_SET_DOWN:
      volume = uni_max(SETTING_VOLUME_MIN, volume - SETTING_VOLUME_STEP);
      MediaPlayerVolumeSet(volume);
      break;
    default:
      break;
  }
}

static Result _idle__user_volume_setting(void *event_info) {
  _user_set_volume_process(event_info);
  return E_OK;
}

static Result _idle__user_mic_mute_setting(void *event_info) {
  Event *event = (Event *)event_info;
  if (event->content.value_int) {
    RecogMute(true);
  } else {
    RecogMute(false);
  }
  return E_OK;
}

static Result _broadcast__user_volume_setting(void *event_info) {
  _user_set_volume_process(event_info);
  return E_OK;
}

static Result _broadcast__user_mic_mute_setting(void *event_info) {
  Event *event = (Event *)event_info;
  if (event->content.value_int) {
    RecogMute(true);
  } else {
    RecogMute(false);
  }
  return E_OK;
}

static Result _play__user_volume_setting(void *event_info) {
  _user_set_volume_process(event_info);
  return E_OK;
}

static Result _play__user_mic_mute_setting(void *event_info) {
  Event *event = (Event *)event_info;
  if (event->content.value_int) {
    RecogMute(true);
  } else {
    RecogMute(false);
  }
  return E_OK;
}

static Result _query_handler(Event *event) {
  Result ret = IsValidEventId(g_setting_session->fsm, event->type);
  LOGT(SETTING_SESSION_TAG, "query handler called, event=%d ret=%d",
       event->type, ret);
  if (E_OK == ret) {
    ret = _filter_event(event);
  }
  return ret;
}

static Result _start_handler(Event *event) {
  Result ret;
  if (NULL == event) {
    ret = ProcessNextEvent(g_setting_session->fsm,
                           NULL, ID(COMMON_RESUME_EVENT));
    LOGT(SETTING_SESSION_TAG, "start handler called, event=NULL ret=%d",
         ret);
  } else {
    ret = ProcessNextEvent(g_setting_session->fsm,
                           (void *)event, event->type);
    LOGT(SETTING_SESSION_TAG, "start handler called, event=%d ret=%d",
         event->type, ret);
  }
  return ret;
}

static Result _stop_handler(void) {
  Result ret = ProcessNextEvent(g_setting_session->fsm,
                                NULL, ID(COMMON_STOP_EVENT));
  LOGT(SETTING_SESSION_TAG, "stop handler called, ret=%d", ret);
  return ret;
}

Result SettingSessionInit(void) {
  uni_s32 session_transition_cnt = 0;
  g_setting_session = (SettingSession *)uni_malloc(sizeof(SettingSession));
  if (NULL == g_setting_session) {
    LOGE(SETTING_SESSION_TAG, "memory alloc failed");
    return E_FAILED;
  }
  uni_memset(g_setting_session, 0, sizeof(SettingSession));
  _session_scene_init();
  g_setting_session->session = SessionCreate(SESSION_PRIORITY_SETTING, 5,
                                             _query_handler,
                                             _start_handler,
                                             _stop_handler);
  if (NULL == g_setting_session->session) {
    LOGE(SETTING_SESSION_TAG, "create session failed");
    goto L_ERROR0;
  }
  g_setting_session->fsm = (MicroFsmStruct *)uni_malloc(sizeof(MicroFsmStruct));
  if (NULL == g_setting_session->fsm) {
    LOGE(SETTING_SESSION_TAG, "memory alloc failed");
    goto L_ERROR1;
  }
  session_transition_cnt = _session_transition_init();
  FsmInit(g_setting_session->fsm, "setting", g_setting_session_transition,
          session_transition_cnt, _setting_session_state_2_str);
  FsmSetState(g_setting_session->fsm, STATE_IDLE);
  SessionManageRegister(g_setting_session->session);
  return E_OK;
L_ERROR1:
  SessionDestroy(g_setting_session->session);
L_ERROR0:
  uni_free(g_setting_session);
  g_setting_session = NULL;
  return E_FAILED;
}

Result SettingSessionFinal(void) {
  if (NULL != g_setting_session) {
    SessionManageUnregister(g_setting_session->session);
    SessionDestroy(g_setting_session->session);
    if (NULL != g_setting_session->fsm) {
      uni_free(g_setting_session->fsm);
      g_setting_session->fsm = NULL;
    }
    uni_free(g_setting_session);
    g_setting_session = NULL;
  }
  if (NULL != g_setting_session_transition) {
    uni_free(g_setting_session_transition);
    g_setting_session_transition = NULL;
  }
  return E_OK;
}
