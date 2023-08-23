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
 * Description : wakeup_session.c
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.27
 *
 **********************************************************************/

#include "uni_wakeup_session.h"

#include "uni_fsm.h"
#include "uni_session.h"
#include "uni_session_priority.h"
#include "uni_session_manage.h"
#include "uni_recog_service.h"
#include "uni_media_player.h"
#include "uni_nlu.h"
#include "uni_log.h"
#include "idle_detect.h"
#include "uni_black_board.h"
#include "uni_media_player.h"
#include "uni_event.h"
#include "uni_event_route.h"
#include "uni_config.h"
#include "uni_pcm_default.h"
#include "uni_user_meeting.h"

#define WAKEUP_SESSION_TAG  "wakeup_session"

#define WAKEUP_GREETINGS    "你好魔方"

static Result _action_notify_wakeup(void *event);
static Result _action_wakeup(void *event);
static Result _action_stop_notify_wakeup(void *event);
static Result _action_timeout_sleep(void *event);
static Result _action_sleep(void *event);
static Result _action_stop_notify_sleep(void *event);
static Result _wakeup__vui_app_wakeup(void *event);
static Result _wakeup__aduio_play_end(void *event);
static Result _hello__vui_lasr_timeout(void *event);
static Result _hello__vui_app_sleep(void *event);
static Result _hello__common_stop(void *event);
static Result _hello__vui_app_wakeup(void *event);
static Result _hello__aduio_play_end(void *event);
static Result _recong__vui_lasr_timeout(void *event);
static Result _recong__vui_app_sleep(void *event);
static Result _recong__vui_app_wakeup(void *event);
static Result _bye__common_stop(void *event);
static Result _bye__aduio_play_end(void *event);
static Result _bye__vui_app_wakeup(void *event);
static Result _action_exit_sleep(void *event);
static Result _hello__user_audio_play(void *event);
static Result _hello__user_audio_stop(void *event);
static Result _bye__user_audio_play(void *event);
static Result _bye__user_audio_stop(void *event);
static Result _wakeup_user_goto_awakend(void *event);
static Result _hello__user_goto_awakend(void *event);
static Result _recong__user_goto_awakend(void *event);
static Result _bye__user_goto_awakend(void *event);
static Result _hello__user_goto_sleeping(void *event);
static Result _recong__user_goto_sleeping(void *event);
static Result _recong__user_play(void *event);

typedef enum {
  STATE_WAKEUP = 0,
  STATE_HELLO,
  STATE_RECONG,
  STATE_BYE
} WakeupSessionState;

typedef struct {
  Session        *session;
  MicroFsmStruct *fsm;
  uni_bool enter_recong;
  uni_sem_t      sem_sleep_done;
} WakeupSession;

/*hvad threshold param*/
#define HVAD_EDGE_DET_DEF     15
#define HVAD_AUDIO_THRESHOLD  0x800
#define HVAD_TRIG_CNT         3

static Scene g_wakeup_scene = {UNI_LP_WAKEUP, 0,
                               DEFAULT_WAKEUP_SCENE_LOW_THRESHOLD,
                               DEFAULT_WAKEUP_SCENE_STD_THRESHOLD, 320, NULL};
static Scene g_asr_scene = {UNI_RASR_LASR_MODE, DEFAULT_RECOGN_SCENE_TIMEOUT,
                            DEFAULT_RECOGN_SCENE_LOW_THRESHOLD,
                            DEFAULT_RECOGN_SCENE_STD_THRESHOLD, 320, NULL};
static WakeupSession      *g_wakeup_session = NULL;
static MicroFsmTransition *g_wakeup_session_transition = NULL;

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
    g_asr_scene.std_threshold = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_asr.lp_threshold", &tmp)) {
    g_asr_scene.low_threshold = tmp;
  }
  if (0 == ConfigReadItemNumber("lasr.lasr_asr.timeout", &timeout)) {
    g_asr_scene.timeout = timeout * 1000;
  }
}

static int _session_transition_init(void) {
const MicroFsmTransition session_transition[] = {
    {STATE_WAKEUP, ID(VUI_APP_WAKEUP_EVENT),        _wakeup__vui_app_wakeup},
    {STATE_WAKEUP, ID(AUDIO_PLAY_END_EVENT),        _wakeup__aduio_play_end},
    {STATE_WAKEUP, ID(USER_GOTO_AWAKEND_EVENT),     _wakeup_user_goto_awakend},
    {STATE_HELLO,  ID(AUDIO_PLAY_END_EVENT),        _hello__aduio_play_end},
    {STATE_HELLO,  ID(VUI_LOCAL_ASR_TIMEOUT_EVENT), _hello__vui_lasr_timeout},
    {STATE_HELLO,  ID(VUI_APP_SLEEP_EVENT),         _hello__vui_app_sleep},
    {STATE_HELLO,  ID(VUI_APP_WAKEUP_EVENT),        _hello__vui_app_wakeup},
    {STATE_HELLO,  ID(COMMON_STOP_EVENT),           _hello__common_stop},
    {STATE_HELLO,  ID(USER_AUDIO_PLAY_EVENT),       _hello__user_audio_play},
    {STATE_HELLO,  ID(USER_AUDIO_STOP_EVENT),       _hello__user_audio_stop},
    {STATE_HELLO,  ID(USER_GOTO_AWAKEND_EVENT),     _hello__user_goto_awakend},
    {STATE_HELLO,  ID(USER_GOTO_SLEEPING_EVENT),    _hello__user_goto_sleeping},
    {STATE_RECONG, ID(VUI_LOCAL_ASR_TIMEOUT_EVENT), _recong__vui_lasr_timeout},
    {STATE_RECONG, ID(VUI_APP_SLEEP_EVENT),         _recong__vui_app_sleep},
    {STATE_RECONG, ID(VUI_APP_WAKEUP_EVENT),        _recong__vui_app_wakeup},
    {STATE_RECONG, ID(USER_GOTO_AWAKEND_EVENT),     _recong__user_goto_awakend},
    {STATE_RECONG, ID(USER_GOTO_SLEEPING_EVENT),    _recong__user_goto_sleeping},
    {STATE_RECONG, ID(USER_AUDIO_PLAY_EVENT),       _recong__user_play},
    {STATE_BYE,    ID(AUDIO_PLAY_END_EVENT),        _bye__aduio_play_end},
    {STATE_BYE,    ID(VUI_APP_WAKEUP_EVENT),        _bye__vui_app_wakeup},
    {STATE_BYE,    ID(COMMON_STOP_EVENT),           _bye__common_stop},
    {STATE_BYE,    ID(USER_AUDIO_PLAY_EVENT),       _bye__user_audio_play},
    {STATE_BYE,    ID(USER_AUDIO_STOP_EVENT),       _bye__user_audio_stop},
    {STATE_BYE,    ID(USER_GOTO_AWAKEND_EVENT),     _bye__user_goto_awakend},
  };
  g_wakeup_session_transition = uni_malloc(sizeof(session_transition));
  if (NULL == g_wakeup_session_transition) {
    LOGE(WAKEUP_SESSION_TAG, "malloc failed !");
    return 0;
  }
  uni_memcpy(g_wakeup_session_transition, session_transition,
             sizeof(session_transition));
  return (sizeof(session_transition) / sizeof(session_transition[0]));
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

static void _stop_player(void) {
  MediaPlayerStop(PLAYER_PCM);
  /* User event send from app layer, because of maybe need some process */
  /*
  user_event_context_t context = {{0}};
  if (uni_user_meeting_is_subscribe(USER_AUDIO_PLAY_END)) {
    context.audio_end.type = AUDIO_PLAY_REPLY;
    context.audio_end.is_auto = false;
    uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
  }
  */
}

static Result _response_pcm(char *command, char *pcm) {
  user_event_context_t context = {{0}};
  LOGT(WAKEUP_SESSION_TAG, "playing pcm: %s", pcm);
  /* User event send from app layer, because of maybe need some process */
  if (uni_user_meeting_is_subscribe(USER_AUDIO_PLAY_START)) {
    context.audio_play.type = AUDIO_PLAY_REPLY;
    context.audio_play.file_name = _creat_buf_save_str(pcm);
    uni_user_meeting_send_event(USER_AUDIO_PLAY_START, &context);
  }
  RecogStop();
  return MediaPlayerStart(PLAYER_PCM, pcm);
}

static Result _play_hello(void *event) {
  char *pcm = NULL;
#ifdef DEFAULT_PCM_WAKEUP
  pcm = uni_get_random_pcm(DEFAULT_PCM_WAKEUP);
#else
  pcm = uni_get_random_pcm(HI);
#endif
  return _response_pcm(WAKEUP_GREETINGS, pcm);
}

static Result _play_bye(void *event) {
  user_event_context_t context = {{0}};
  Result ret = E_OK;
  if (uni_user_meeting_is_subscribe(USER_GOTO_SLEEPING)) {
    context.goto_sleeping.trigger = EVENT_TRIGGER_AUTO;
#ifdef DEFAULT_PCM_ASR_TIMEOUT
    context.goto_sleeping.reply_files = _creat_buf_save_str(DEFAULT_PCM_ASR_TIMEOUT);
#else
    context.goto_sleeping.reply_files = _creat_buf_save_str(BYE);
#endif
    uni_user_meeting_send_event(USER_GOTO_SLEEPING, &context);
    if (uni_user_meeting_is_reply()) {
#if WAKEUP_INTERRUPT_MODE
      RecogLaunch(&g_wakeup_scene);
#else
      RecogStop();
#endif
      return E_HOLD;
    } else {
      return E_OK;
    }
  }
#ifdef DEFAULT_PCM_ASR_TIMEOUT
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(DEFAULT_PCM_ASR_TIMEOUT));
#else
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(BYE));
#endif
  if (E_OK == ret) {
    return E_HOLD;
  }
  return E_FAILED;
}

static Result _play_bye_direct(Event *event) {
  user_event_context_t context = {{0}};
  uni_lasr_result_t *content = NULL;
  Result ret = E_OK;
  if (uni_user_meeting_is_subscribe(USER_GOTO_SLEEPING)) {
    context.goto_sleeping.trigger = EVENT_TRIGGER_ASR;
    content = (uni_lasr_result_t *)event->content.info;
    context.goto_sleeping.cmd = _creat_buf_save_str(content->cmd);
    context.goto_sleeping.word_str = _creat_buf_save_str(content->asr);
#ifdef DEFAULT_PCM_SLEEP
    context.goto_sleeping.reply_files = _creat_buf_save_str(DEFAULT_PCM_SLEEP);
#else
    context.goto_sleeping.reply_files = _creat_buf_save_str(BYE);
#endif
    uni_user_meeting_send_event(USER_GOTO_SLEEPING, &context);
    if (uni_user_meeting_is_reply()) {
#if WAKEUP_INTERRUPT_MODE
      RecogLaunch(&g_wakeup_scene);
#else
      RecogStop();
#endif
      return E_HOLD;
    } else {
      return E_OK;
    }
  }
#ifdef DEFAULT_PCM_SLEEP
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(DEFAULT_PCM_SLEEP));
#else
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(BYE));
#endif
  if (E_OK == ret) {
    return E_HOLD;
  }
  return E_FAILED;
}

static Result _play_bye_user(Event *event) {
  user_event_context_t context = {{0}};
  Result ret = E_OK;
  if (uni_user_meeting_is_subscribe(USER_GOTO_SLEEPING)) {
    context.goto_sleeping.trigger = EVENT_TRIGGER_USER;
#ifdef DEFAULT_PCM_SLEEP
    context.goto_sleeping.reply_files = _creat_buf_save_str(DEFAULT_PCM_SLEEP);
#else
    context.goto_sleeping.reply_files = _creat_buf_save_str(BYE);
#endif
    uni_user_meeting_send_event(USER_GOTO_SLEEPING, &context);
    if (uni_user_meeting_is_reply()) {
#if WAKEUP_INTERRUPT_MODE
      RecogLaunch(&g_wakeup_scene);
#else
      RecogStop();
#endif
      return E_HOLD;
    } else {
      return E_OK;
    }
  }
#ifdef DEFAULT_PCM_SLEEP
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(DEFAULT_PCM_SLEEP));
#else
  ret = _response_pcm(WAKEUP_GREETINGS, uni_get_random_pcm(BYE));
#endif
  if (E_OK == ret) {
    return E_HOLD;
  }
  return E_FAILED;
}

static const char* _wakeup_session_state_2_str(uni_s32 state) {
  static const char *state_str[] = {
    [STATE_WAKEUP] = "STATE_WAKEUP",
    [STATE_HELLO]  = "STATE_HELLO",
    [STATE_RECONG] = "STATE_RECONG",
    [STATE_BYE]    = "STATE_BYE",
  };
  if (state < STATE_WAKEUP || STATE_BYE < state) {
    return "N/A";
  }
  return state_str[state];
}

static Result _filter_event_query(Event *event) {
  uni_s32 type = event->type;
  if (ID(APP_GOTO_WAKEUP_EVENT) == type) {
    RecogLaunch(&g_wakeup_scene);
    LOGT(WAKEUP_SESSION_TAG, "goto wakeup scene");
    FsmSetState(g_wakeup_session->fsm, STATE_WAKEUP);
    return E_REJECT;
  }
  return E_OK;
}

static Result _play_wakeup_notify(Event *event, char is_reentry) {
  uni_lasr_result_t *content = NULL;
  user_event_context_t context = {{0}};
  if (uni_user_meeting_is_subscribe(USER_GOTO_AWAKENED)) {
    LOGT(WAKEUP_SESSION_TAG, "user reply process");
    content = (uni_lasr_result_t *)(((Event *)event)->content.info);
    context.goto_awakend.trigger = EVENT_TRIGGER_ASR;
    context.goto_awakend.cmd = _creat_buf_save_str(content->cmd);
    context.goto_awakend.word_str = _creat_buf_save_str(content->asr);
#ifdef DEFAULT_PCM_WAKEUP
    context.goto_awakend.reply_files = _creat_buf_save_str(DEFAULT_PCM_WAKEUP);
#else
    context.goto_awakend.reply_files = _creat_buf_save_str(HI);
#endif
    uni_user_meeting_send_event(USER_GOTO_AWAKENED, &context);
    if (uni_user_meeting_is_reply()) {
      return E_HOLD;
    } else {
      return E_OK;
    }
  }
  if (E_OK == _play_hello(event)) {
    return E_HOLD;
  }
  return E_FAILED;
}

static Result _play_user_wakeup_notify(Event *event, char is_reentry) {
  user_event_context_t context = {{0}};
  if (uni_user_meeting_is_subscribe(USER_GOTO_AWAKENED)) {
    LOGT(WAKEUP_SESSION_TAG, "user reply process");
    context.goto_awakend.trigger = EVENT_TRIGGER_USER;
#ifdef DEFAULT_PCM_WAKEUP
    context.goto_awakend.reply_files = _creat_buf_save_str(DEFAULT_PCM_WAKEUP);
#else
    context.goto_awakend.reply_files = _creat_buf_save_str(HI);
#endif
    uni_user_meeting_send_event(USER_GOTO_AWAKENED, &context);
    if (uni_user_meeting_is_reply()) {
      return E_HOLD;
    } else {
      return E_OK;
    }
  }
  if (E_OK == _play_hello(event)) {
    return E_HOLD;
  }
  return E_FAILED;
}

static Result _hello__aduio_play_end(void *event) {
  user_event_context_t context = {{0}};
  /* User event send from app layer, because of maybe need some process */
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = true;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
  return _action_wakeup(event);
}

static Result _bye__aduio_play_end(void *event) {
  user_event_context_t context = {{0}};
  /* User event send from app layer, because of maybe need some process */
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = true;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
  return _action_sleep(event);
}

static Result _wakeup__vui_app_wakeup(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_wakeup(event);
}

static Result _wakeup__aduio_play_end(void *event) {
  user_event_context_t context = {{0}};
  /* User event send from app layer, because of maybe need some process */
  context.audio_end.type = AUDIO_PLAY_REPLY;
  context.audio_end.is_auto = true;
  uni_user_meeting_send_event(USER_AUDIO_PLAY_END, &context);
  RecogLaunch(&g_wakeup_scene);
  return E_OK;
}

static Result _hello__vui_lasr_timeout(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_timeout_sleep(event);
}

static Result _hello__vui_app_sleep(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_exit_sleep(event);
}

static Result _hello__common_stop(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_stop_notify_wakeup(event);
}

static Result _hello__vui_app_wakeup(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_wakeup(event);
}

static Result _recong__vui_lasr_timeout(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_timeout_sleep(event);
}

static Result _recong__vui_app_sleep(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_exit_sleep(event);
}

static Result _recong__vui_app_wakeup(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_wakeup(event);
}

static Result _bye__common_stop(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_stop_notify_sleep(event);
}

static Result _bye__vui_app_wakeup(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_wakeup(event);
}

static Result _action_notify_wakeup(void *event) {
  Result ret = E_OK;
  ret = _play_wakeup_notify((Event *)event, 0);
  if (E_HOLD != ret) {
    return _action_wakeup(event);
  } 
  if (E_HOLD == ret) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_HELLO);
  }
  return E_FAILED;
}

static Result _action_notify_user_wakeup(void *event) {
  Result ret = E_OK;
  ret = _play_user_wakeup_notify((Event *)event, 0);
  if (E_HOLD != ret) {
    return _action_wakeup(event);
  } 
  if (E_HOLD == ret) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_HELLO);
  }
  return E_FAILED;
}

static Result _action_wakeup(void *event) {
  if (false == g_wakeup_session->enter_recong) {
    FsmSetState(g_wakeup_session->fsm, STATE_WAKEUP);
    RecogLaunch(&g_wakeup_scene);
    return E_OK;
  }
  RecogLaunch(&g_asr_scene);
  FsmSetState(g_wakeup_session->fsm, STATE_RECONG);
  return E_OK;
}

static Result _action_stop_notify_wakeup(void *event) {
  return _action_wakeup(event);
}

static Result _action_timeout_sleep(void *event) {
  Result ret = E_OK;
  LOGT(WAKEUP_SESSION_TAG, "goto sleep");
  ret = _play_bye(event);
  if (E_HOLD == ret) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_BYE);
    return E_HOLD;
  }
  _action_sleep(event);
  return ret;
}

static Result _action_exit_sleep(void *event) {
  Result ret = E_OK;
  LOGT(WAKEUP_SESSION_TAG, "enter sleep");
  ret = _play_bye_direct((Event *)event);
  if (E_HOLD == ret) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_BYE);
    return E_HOLD;
  }
  _action_sleep(event);
  return ret;
}

static Result _action_user_sleep(void *event) {
  Result ret = E_OK;
  LOGT(WAKEUP_SESSION_TAG, "enter sleep");
  ret = _play_bye_user((Event *)event);
  if (E_HOLD == ret) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_BYE);
    return E_HOLD;
  }
  _action_sleep(event);
  return ret;
}

static Result _action_sleep(void *event) {
  RecogLaunch(&g_wakeup_scene);
  FsmSetState(g_wakeup_session->fsm, STATE_WAKEUP);
  return E_OK;
}

static Result _action_stop_notify_sleep(void *event) {
  _stop_player();
  return _action_sleep(event);
}

static Result _hello__user_audio_play(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(WAKEUP_SESSION_TAG, "response pcm failed");
    return _action_wakeup(event);
  }
}

static Result _hello__user_audio_stop(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  _stop_player();
  return _action_wakeup(event);
}

static Result _bye__user_audio_play(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    return E_HOLD;
  } else {
    LOGE(WAKEUP_SESSION_TAG, "response pcm failed");
    return _action_sleep(event);
  }
}

static Result _bye__user_audio_stop(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  _stop_player();
  return _action_sleep(event);
}

static Result _wakeup_user_goto_awakend(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_user_wakeup(event);
}

static Result _hello__user_goto_awakend(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_user_wakeup(event);
}

static Result _recong__user_goto_awakend(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_user_wakeup(event);
}

static Result _bye__user_goto_awakend(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  g_wakeup_session->enter_recong = true;
  return _action_notify_user_wakeup(event);
}

static Result _hello__user_goto_sleeping(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_user_sleep(event);
}

static Result _recong__user_goto_sleeping(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  return _action_user_sleep(event);
}

static void _go_to_sleep_mode(void) {
  LOGW(WAKEUP_SESSION_TAG, "System go to sleep mode done!");
  /* TODO: enter sleep mode */
}

static Result _recong__user_play(void *event) {
  LOGT(WAKEUP_SESSION_TAG, "action called");
  if (E_OK == _response_pcm("", ((Event *)event)->content.info)) {
#if WAKEUP_INTERRUPT_MODE
    RecogLaunch(&g_wakeup_scene);
#else
    RecogStop();
#endif
    FsmSetState(g_wakeup_session->fsm, STATE_HELLO);
    return E_HOLD;
  } else {
    LOGE(WAKEUP_SESSION_TAG, "response pcm failed");
    return E_FAILED;
  }
}

static Result _query_handler(Event *event) {
  uni_s32 state;
  Result ret = IsValidEventId(g_wakeup_session->fsm, event->type);
  if (E_OK == ret || (ID(APP_GOTO_WAKEUP_EVENT) == event->type)) {
    ret = _filter_event_query(event);
  }
  LOGT(WAKEUP_SESSION_TAG, "query handler called, event=%d ret=%d",
       event->type, ret);
  if (ID(APP_SLEEP_MODE_EVENT) == event->type) {
    state = FsmGetState(g_wakeup_session->fsm);
    if (STATE_HELLO != state) {
      _go_to_sleep_mode();
      LOGT(WAKEUP_SESSION_TAG, "enter sleep mode");
    }
  }
  return ret;
}

static Result _start_handler(Event *event) {
  Result ret;
  if (NULL == event) {
    ret = ProcessNextEvent(g_wakeup_session->fsm, NULL, ID(COMMON_RESUME_EVENT));
    LOGT(WAKEUP_SESSION_TAG, "start handler called, event=NULL ret=%d", ret);
  } else {
    ret = ProcessNextEvent(g_wakeup_session->fsm, (void *)event, event->type);
    LOGT(WAKEUP_SESSION_TAG, "start handler called, event=%d ret=%d",
         event->type, ret);
  }
  return ret;
}

static Result _stop_handler(void) {
  Result ret = ProcessNextEvent(g_wakeup_session->fsm, NULL, ID(COMMON_STOP_EVENT));
  LOGT(WAKEUP_SESSION_TAG, "stop handler called, ret=%d", ret);
  return ret;
}

#define ENTER_SLEEP_MODE_THRESOLD  (10 * 60)

static void _idle_detect_cb(IdleType idle_status) {
  Event *event;
  if (IDLE_DETECT_TYPE_IDLE == idle_status) {
    LOGW(WAKEUP_SESSION_TAG, "Go to sleep mode ...");
    event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT, ID(APP_SLEEP_MODE_EVENT),
                        NULL, NULL);
    event->content.value_int = idle_status;
    EventRouteProcess(event);
    EventFree(event);
  }
}

static void _try_init_sleep_scene(void) {
  uni_s64 tmp = 0;
  uni_sem_init(&g_wakeup_session->sem_sleep_done, 0);
  if (0 != ConfigReadItemNumber("hvad.enable", &tmp)) {
    LOGE(WAKEUP_SESSION_TAG, "cannot found hvad.enable, diable HVAD.");
    return ;
  }
  if (tmp) {
    if (0 != ConfigReadItemNumber("hvad.time_to_sleep", &tmp)
      || (tmp <= 0)) {
      LOGE(WAKEUP_SESSION_TAG, "get hvad.time_to_sleep failed, use %d.",
                                ENTER_SLEEP_MODE_THRESOLD);
      tmp = ENTER_SLEEP_MODE_THRESOLD;
    }
    IdleDetectServiceRegister(_idle_detect_cb, tmp);
  }
}

Result WakeupSessionInit(void) {
  uni_s32 session_transition_cnt = 0;
  g_wakeup_session = (WakeupSession *)uni_malloc(sizeof(WakeupSession));
  if (NULL == g_wakeup_session) {
    LOGE(WAKEUP_SESSION_TAG, "memory alloc failed");
    return E_FAILED;
  }
  uni_memset(g_wakeup_session, 0, sizeof(WakeupSession));
  _session_scene_init();
  g_wakeup_session->session = SessionCreate(SESSION_PRIORITY_WAKEUP, 2,
                                            _query_handler,
                                            _start_handler,
                                            _stop_handler);
  if (NULL == g_wakeup_session->session) {
    LOGE(WAKEUP_SESSION_TAG, "create session failed");
    goto L_ERROR0;
  }
  g_wakeup_session->fsm = (MicroFsmStruct *)uni_malloc(sizeof(MicroFsmStruct));
  if (NULL == g_wakeup_session->fsm) {
    LOGE(WAKEUP_SESSION_TAG, "memory alloc failed");
    goto L_ERROR1;
  }
  session_transition_cnt = _session_transition_init();
  FsmInit(g_wakeup_session->fsm, "wakeup", g_wakeup_session_transition,
          session_transition_cnt, _wakeup_session_state_2_str);
  FsmSetState(g_wakeup_session->fsm, STATE_WAKEUP);
  SessionManageRegister(g_wakeup_session->session);
  _try_init_sleep_scene();
  RecogLaunch(&g_wakeup_scene);
  return E_OK;
L_ERROR1:
  SessionDestroy(g_wakeup_session->session);
L_ERROR0:
  uni_free(g_wakeup_session);
  g_wakeup_session = NULL;
  return E_FAILED;
}

Result WakeupSessionFinal(void) {
  if (NULL != g_wakeup_session) {
    SessionManageUnregister(g_wakeup_session->session);
    SessionDestroy(g_wakeup_session->session);
    if (NULL != g_wakeup_session->fsm) {
      uni_free(g_wakeup_session->fsm);
      g_wakeup_session->fsm = NULL;
    }
    uni_free(g_wakeup_session);
    g_wakeup_session = NULL;
  }
  if (NULL != g_wakeup_session_transition) {
    uni_free(g_wakeup_session_transition);
    g_wakeup_session_transition = NULL;
  }
  return E_OK;
}
