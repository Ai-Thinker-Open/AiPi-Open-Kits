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
 * Description : hb_housekeeper.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.13
 *
 **************************************************************************/
#include "unione.h"
#include "user_asr.h"
#include "user_player.h"
#include "uni_timer.h"
#include "housekeeper_irc.h"

#define TAG "hb_housekeeoer"

#define AC_MATCH_TIMEOUT_MS (1000 * 20)
#define AC_MATCH_TIMER      eTIMER6

#define AC_MATCH_OK_REPLY   "108"
#define AC_MATCH_FAL1_REPLY "109"
#define AC_MATCH_FAL2_REPLY "110"
#define AC_MATCH_FAL3_REPLY "111"
#define AC_SCANING_REPLY    "500"
#define AC_POWER_OFF_REPLY  "501"
#define AC_SCAN_FAIL_REPLY  "502"
#define AC_MATCH_EXIT_REPLY "503"
#define AC_LAMP_EDGE_REPLY  "7"

typedef enum {
  AC_MATCH_FAL_0 = 0,
  AC_MATCH_FAL_1,
  AC_MATCH_FAL_2,
  AC_MATCH_FAL_3,
  AC_MATCH_FAL_MAX
}AC_MATCH_FAL_CNT;

static AC_MATCH_FAL_CNT g_ac_match_fal_cnt = AC_MATCH_FAL_0;
static TimerHandle g_ac_match_timer = -1;
static uni_bool g_is_mute_mode = false;

static int _ac_match_timeout_cb(void *arg) {
  housekeeper_irc_ac_match_stop();
  user_asr_timeout_enable();
  user_player_play(AUDIO_PLAY_REPLY, AC_MATCH_EXIT_REPLY);
  return TIMER_ERRNO_STOP_PERIODICAL;
}

static int _start_ac_match_timer(int interval_msec) {
  g_ac_match_timer = TimerStart(interval_msec, TIMER_TYPE_ONESHOT,
                                _ac_match_timeout_cb, NULL);
  if (g_ac_match_timer <= 0) {
    LOGE(TAG, "TimerStart failed.");
    return -1;
  }
  return 0;
}

static int _stop_ac_match_timer(void) {
  if (g_ac_match_timer > 0) {
    TimerStop(g_ac_match_timer);
    g_ac_match_timer = -1;
  }
  return 0;
}

void _cb_ac_match_result(uni_bool is_ok) {
  _stop_ac_match_timer();
  if (is_ok) {
    housekeeper_irc_ac_match_stop();
    user_asr_timeout_enable();
    user_player_play(AUDIO_PLAY_REPLY, AC_MATCH_OK_REPLY);
  } else {
    g_ac_match_fal_cnt++;
    if (g_ac_match_fal_cnt > AC_MATCH_FAL_3) {
      g_ac_match_fal_cnt = AC_MATCH_FAL_3;
    }
    switch (g_ac_match_fal_cnt) {
      case AC_MATCH_FAL_1:
        _start_ac_match_timer(AC_MATCH_TIMEOUT_MS + 4000);
        user_player_play(AUDIO_PLAY_REPLY, AC_MATCH_FAL1_REPLY);
        break;
      case AC_MATCH_FAL_2:
        _start_ac_match_timer(AC_MATCH_TIMEOUT_MS + 9000);
        user_player_play(AUDIO_PLAY_REPLY, AC_MATCH_FAL2_REPLY);
        break;
      case AC_MATCH_FAL_3:
        housekeeper_irc_ac_match_stop();
        user_asr_timeout_enable();
        user_player_play(AUDIO_PLAY_REPLY, AC_MATCH_FAL3_REPLY);
        break;
      default:
        break;
    }
  }
}

void _cb_ac_scan_failed(void) {
  /* don't need call housekeeper_irc_ac_scan_stop() at here */
  user_asr_timeout_enable();
  user_player_play(AUDIO_PLAY_REPLY, AC_SCAN_FAIL_REPLY);
}

static int _ac_match_replay(event_custom_setting_t *setting) {
  if (0 != housekeeper_irc_ac_match_start()) {
    LOGE(TAG, "housekeeper_irc_ac_match_start failed");
    return -1;
  }
  g_ac_match_fal_cnt = AC_MATCH_FAL_0;
  _start_ac_match_timer(AC_MATCH_TIMEOUT_MS + 1000);
  user_asr_timeout_disable();
  user_player_reply_list_num(setting->reply_files, 0);
  return 0;
}

static int _ac_restore_replay(event_custom_setting_t *setting) {
  if (0 != housekeeper_irc_ac_restore()) {
    LOGE(TAG, "housekeeper_irc_ac_restore failed");
    return -1;
  }
  user_player_reply_list_random(setting->reply_files);
  return 0;
}

static int _lamp_on_replay(event_custom_setting_t *setting) {
  if (0 != housekeeper_irc_set_lamp_state(true)) {
    LOGE(TAG, "housekeeper_irc_set_lamp_state failed");
    return -1;
  }
  user_player_reply_list_random(setting->reply_files);
  return 0;
}

static int _lamp_off_replay(event_custom_setting_t *setting) {
  if (0 != housekeeper_irc_set_lamp_state(false)) {
    LOGE(TAG, "housekeeper_irc_set_lamp_state failed");
    return -1;
  }
  user_player_reply_list_random(setting->reply_files);
  return 0;
}

static int _lamp_inc_replay(event_custom_setting_t *setting) {
  LAMP_LEVEL level = housekeeper_irc_get_lamp_level();
  if (0 != housekeeper_irc_lamp_bright_inc()) {
    LOGE(TAG, "housekeeper_irc_lamp_bright_inc failed");
    return -1;
  }
  if (LAMP_BRIGHT_HIGH == level) {
    user_player_play(AUDIO_PLAY_REPLY, AC_LAMP_EDGE_REPLY);
    return 0;
  }
  user_player_reply_list_random(setting->reply_files);
  return 0;
}

static int _lamp_dec_replay(event_custom_setting_t *setting) {
  LAMP_LEVEL level = housekeeper_irc_get_lamp_level();
  if (0 != housekeeper_irc_lamp_bright_dec()) {
    LOGE(TAG, "housekeeper_irc_lamp_bright_dec failed");
    return -1;
  }
  if (LAMP_BRIGHT_LOW == level) {
    user_player_play(AUDIO_PLAY_REPLY, AC_LAMP_EDGE_REPLY);
    return 0;
  }
  user_player_reply_list_random(setting->reply_files);
  return 0;
}

static int _lamp_cmd_replay(event_custom_setting_t *setting) {
  int ret = -1;
  if (0 == uni_strcmp(setting->cmd, "NightLampOn")) {
    ret = _lamp_on_replay(setting);
  } else if (0 == uni_strcmp(setting->cmd, "NightLampOff")) {
    ret = _lamp_off_replay(setting);
  } else if (0 == uni_strcmp(setting->cmd, "NightLampInc")) {
    ret = _lamp_inc_replay(setting);
  } else if (0 == uni_strcmp(setting->cmd, "NightLampDec")) {
    ret = _lamp_dec_replay(setting);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  return ret;
}

static int _ac_power_replay(event_custom_setting_t *setting) {
  int ret = -1;
  if (0 == uni_strcmp(setting->cmd, "ACTurnOn")) {
    ret = housekeeper_irc_set_ac_state(true);
  } else if (0 == uni_strcmp(setting->cmd, "ACTurnOff")) {
    ret = housekeeper_irc_set_ac_state(false);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_set_ac_state failed");
    return -1;
  }
  return user_player_reply_list_random(setting->reply_files);
}

static int _ac_mode_replay(event_custom_setting_t *setting) {
  int ret = -1;
  if (!housekeeper_irc_get_ac_state()) {
    return user_player_play(AUDIO_PLAY_REPLY, AC_POWER_OFF_REPLY);
  }
  if (0 == uni_strcmp(setting->cmd, "ACModeAuto")) {
    ret = housekeeper_irc_set_ac_mode(AC_MODE_AUTO);
  } else if (0 == uni_strcmp(setting->cmd, "ACModeCool")) {
    ret = housekeeper_irc_set_ac_mode(AC_MODE_COOL);
  } else if (0 == uni_strcmp(setting->cmd, "ACModeHeat")) {
    ret = housekeeper_irc_set_ac_mode(AC_MODE_HEAT);
  } else if (0 == uni_strcmp(setting->cmd, "ACModeDry")) {
    ret = housekeeper_irc_set_ac_mode(AC_MODE_DRY);
  } else if (0 == uni_strcmp(setting->cmd, "ACModeFan")) {
    ret = housekeeper_irc_set_ac_mode(AC_MODE_FAN);
  } else if (0 == uni_strcmp(setting->cmd, "ACModeSleep")) {
    ret = housekeeper_irc_ac_sleep_mode();
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_set_ac_mode failed");
    return -1;
  }
  return user_player_reply_list_random(setting->reply_files);
}

static int _ac_wind_replay(event_custom_setting_t *setting) {
  int ret = -1;
  AC_WIND_SPEED speed = AC_WIND_INVALID;
  if (!housekeeper_irc_get_ac_state()) {
    return user_player_play(AUDIO_PLAY_REPLY, AC_POWER_OFF_REPLY);
  }
  if (0 == uni_strcmp(setting->cmd, "ACWindHigh")) {
    ret = housekeeper_irc_set_ac_wind(AC_WIND_HIGH);
  } else if (0 == uni_strcmp(setting->cmd, "ACWindMid")) {
    ret = housekeeper_irc_set_ac_wind(AC_WIND_MID);
  } else if (0 == uni_strcmp(setting->cmd, "ACWindLow")) {
    ret = housekeeper_irc_set_ac_wind(AC_WIND_LOW);
  } else if (0 == uni_strcmp(setting->cmd, "ACWindAuto")) {
    ret = housekeeper_irc_set_ac_wind(AC_WIND_AUTO);
  } else if (0 == uni_strcmp(setting->cmd, "ACWindInc")) {
    speed = housekeeper_irc_get_ac_wind();
    if (AC_WIND_HIGH == speed) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    ret = housekeeper_irc_ac_wind_inc();
  } else if (0 == uni_strcmp(setting->cmd, "ACWindDec")) {
    speed = housekeeper_irc_get_ac_wind();
    if (AC_WIND_LOW == speed) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    ret = housekeeper_irc_ac_wind_dec();
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_set_ac_wind failed");
    return -1;
  }
  return user_player_reply_list_num(setting->reply_files, 0);
}

static int _ac_temp_replay(event_custom_setting_t *setting) {
  int ret = -1;
  AC_TMP_NUMBER tmp = AC_TMP_INVALID;
  if (!housekeeper_irc_get_ac_state()) {
    return user_player_play(AUDIO_PLAY_REPLY, AC_POWER_OFF_REPLY);
  }
  if (0 == uni_strcmp(setting->cmd, "ACTmp16")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_16);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp17")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_17);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp18")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_18);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp19")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_19);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp20")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_20);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp21")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_21);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp22")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_22);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp23")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_23);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp24")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_24);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp25")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_25);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp26")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_26);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp27")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_27);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp28")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_28);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp29")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_29);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmp30")) {
    ret = housekeeper_irc_set_ac_tmp(AC_TMP_30);
  } else if (0 == uni_strcmp(setting->cmd, "ACTmpInc")) {
    tmp = housekeeper_irc_get_ac_tmp();
    if (AC_TMP_30 == tmp) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    ret = housekeeper_irc_ac_tmp_inc();
  } else if (0 == uni_strcmp(setting->cmd, "ACTmpDec")) {
    tmp = housekeeper_irc_get_ac_tmp();
    if (AC_TMP_16 == tmp) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    ret = housekeeper_irc_ac_tmp_dec();
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_set_ac_tmp failed");
    return -1;
  }
  return user_player_reply_list_num(setting->reply_files, 0);
}

static int _ac_swept_replay(event_custom_setting_t *setting) {
  int ret = -1;
  if (!housekeeper_irc_get_ac_state()) {
    return user_player_play(AUDIO_PLAY_REPLY, AC_POWER_OFF_REPLY);
  }
  if (0 == uni_strcmp(setting->cmd, "ACSweptOn")) {
    ret = housekeeper_irc_set_ac_swept(AC_SWEPT_ALL_ON);
  } else if (0 == uni_strcmp(setting->cmd, "ACSweptOff")) {
    ret = housekeeper_irc_set_ac_swept(AC_SWEPT_OFF);
  } else if (0 == uni_strcmp(setting->cmd, "ACSweptVert")) {
    ret = housekeeper_irc_set_ac_swept(AC_SWEPT_VERT_ON);
  } else if (0 == uni_strcmp(setting->cmd, "ACSweptCross")) {
    ret = housekeeper_irc_set_ac_swept(AC_SWEPT_CROSS_ON);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_set_ac_swept failed");
    return -1;
  }
  return user_player_reply_list_random(setting->reply_files);
}

static int _ac_scan_replay(event_custom_setting_t *setting) {
  int ret = -1;
  if (0 == uni_strcmp(setting->cmd, "ACScanStart")) {
    ret = housekeeper_irc_ac_scan_start();
    if (0 == ret) {
      user_asr_timeout_disable();
    }
  } else if (0 == uni_strcmp(setting->cmd, "ACScanStop")) {
    ret = housekeeper_irc_ac_scan_stop();
    if (0 == ret) {
      user_asr_timeout_enable();
    }
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  if (0 != ret) {
    LOGE(TAG, "housekeeper_irc_ac_scan failed");
    return -1;
  }
  return user_player_reply_list_random(setting->reply_files);
}

static int _speaker_mute_reply(event_custom_setting_t *setting) {
  g_is_mute_mode = true;
  return user_player_reply_list_random(setting->reply_files);
}

static int _speaker_unmute_reply(event_custom_setting_t *setting) {
  g_is_mute_mode = false;
  user_player_shutup_exit();
  return user_player_reply_list_random(setting->reply_files);
}

static void _custom_setting_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_custom_setting_t *setting = NULL;
  uni_bool is_mute_mode = g_is_mute_mode;
  int ret = -1;
  if (context) {
    setting = &context->custom_setting;
    if (housekeeper_irc_is_ac_scan()
      && (0 != uni_strcmp(setting->cmd, "ACScanStop"))) {
      housekeeper_irc_ac_scan_pause();
      user_player_play(AUDIO_PLAY_REPLY, AC_SCANING_REPLY);
      if (is_mute_mode) {
        housekeeper_irc_lamp_blink();
      }
      return ;
    }
    if (0 == uni_strcmp(setting->cmd, "LearnMatchAC")) {
      ret = _ac_match_replay(setting);
    } else if (0 == uni_strcmp(setting->cmd, "SetRestore")) {
      ret = _ac_restore_replay(setting);
    } else if (0 == uni_strcmp(setting->cmd, "SpeakerMute")) {
      ret = _speaker_mute_reply(setting);
    } else if (0 == uni_strcmp(setting->cmd, "SpeakerUnmute")) {
      ret = _speaker_unmute_reply(setting);
      is_mute_mode = false;
    } else if (0 == uni_strncmp(setting->cmd, "ACTurn", uni_strlen("ACTurn"))) {
      ret = _ac_power_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "ACMode", uni_strlen("ACMode"))) {
      ret = _ac_mode_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "ACWind", uni_strlen("ACWind"))) {
      ret = _ac_wind_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "ACTmp", uni_strlen("ACTmp"))) {
      ret = _ac_temp_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "ACSwept", uni_strlen("ACSwept"))) {
      ret = _ac_swept_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "ACScan", uni_strlen("ACScan"))) {
      ret = _ac_scan_replay(setting);
    } else if (0 == uni_strncmp(setting->cmd, "NightLamp", uni_strlen("NightLamp"))) {
      ret = _lamp_cmd_replay(setting);
    } else {
      LOGE(TAG, "unknown command : %s", setting->cmd);
      ret = -1;
    }
    if (is_mute_mode && (0 == ret)) {
      housekeeper_irc_lamp_blink();
    }
  }
}

static void _goto_awakened_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_awakend_t *awkened = NULL;
  if (context) {
    awkened = &context->goto_awakend;
    housekeeper_irc_ac_scan_pause();
    user_player_reply_list_random(awkened->reply_files);
    if (g_is_mute_mode && EVENT_TRIGGER_ASR == awkened->trigger) {
      housekeeper_irc_lamp_blink();
    }
  }
}

static void _goto_sleeping_cb (USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_sleeping_t *sleeping = NULL;
  if (context) {
    sleeping = &context->goto_sleeping;
    housekeeper_irc_ac_scan_stop();
    housekeeper_irc_ac_match_stop();
    user_asr_timeout_enable();
    user_player_reply_list_random(sleeping->reply_files);
    if (g_is_mute_mode && EVENT_TRIGGER_ASR == sleeping->trigger) {
      housekeeper_irc_lamp_blink();
    }
  }
}

static void _audio_playend_cb (USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  housekeeper_irc_ac_scan_resume();
  if (g_is_mute_mode) {
    user_player_shutup_mode();
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
  user_event_subscribe_event(USER_GOTO_AWAKENED, _goto_awakened_cb);
  user_event_subscribe_event(USER_GOTO_SLEEPING, _goto_sleeping_cb);
  user_event_subscribe_event(USER_AUDIO_PLAY_END, _audio_playend_cb);
}

int hb_housekeeper(void) {
  if (0 != housekeeper_irc_init(_cb_ac_match_result, _cb_ac_scan_failed)) {
    LOGE(TAG, "user_uart_hb1688_init failed.");
    return -1;
  }
  _register_event_callback();
  return 0;
}

