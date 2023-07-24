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
 * Description : hb_smart_ac.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.03
 *
 **************************************************************************/
#include "unione.h"
#include "ac_device_simulator.h"
#include "user_asr.h"
#include "user_player.h"

#define TAG "smart_ac"

static char g_power_off_reply[32] = {0};
static int _ac_power_replay(event_custom_setting_t *setting) {
  eAcSate state = eACSTATE_UNKONWN;
  eAcWindMode mode = eACMODE_UNKONWN;
  if (0 == uni_strcmp(setting->cmd, "ac_power_on")) {
    state = AcDeviceSetState(eACSTATE_ON);
  } else if (0 == uni_strcmp(setting->cmd, "ac_power_off")) {
    state = AcDeviceSetState(eACSTATE_OFF);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  LOGT(TAG, "AC power state: %d", state);
  if (eACSTATE_OFF == state) {
    uni_strncpy(g_power_off_reply, setting->reply_files, sizeof(g_power_off_reply) - 1);
    return user_asr_goto_sleep();
  } else if (eACSTATE_ON == state) {
    mode = AcDeviceGetWindMode();
    switch (mode) {
      case eACMODE_NATURE:
        return user_player_reply_list_num(setting->reply_files, 0);
      case eACMODE_NORMAL:
        return user_player_reply_list_num(setting->reply_files, 1);
      case eACMODE_SLEEP:
        return user_player_reply_list_num(setting->reply_files, 2);
      default:
        LOGE(TAG, "Unknown mode: %d", mode);
        break;
    }
  } else {
    LOGE(TAG, "invalid AC state: %d", state);
  }
  return -1;
}

static int _ac_off_replay(void) {
#ifdef DEFAULT_PCM_WAKEUP
  return user_player_reply_list_num(DEFAULT_PCM_WAKEUP, 0);
#else
  return user_player_reply_list_random(HI_AC_OFF);
#endif
}

static int _ac_speed_replay(event_custom_setting_t *setting) {
  eAcSate state = AcDeviceGetState();
  eAcWindSpeed speed = eAcWindSpeed_UNKONWN;
  if (eACSTATE_ON != state) {
    return _ac_off_replay();
  }
  if (0 == uni_strcmp(setting->cmd, "ac_speed_1")) {
    speed = AcDeviceSetWindSpeed(eAcWindSpeed_1);
  } else if (0 == uni_strcmp(setting->cmd, "ac_speed_2")) {
    speed = AcDeviceSetWindSpeed(eAcWindSpeed_2);
  } else if (0 == uni_strcmp(setting->cmd, "ac_speed_3")) {
    speed = AcDeviceSetWindSpeed(eAcWindSpeed_3);
  } else if (0 == uni_strcmp(setting->cmd, "ac_speed_inc")) {
    speed = AcDeviceGetWindSpeed();
    if (eAcWindSpeed_3 == speed) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    speed = AcDeviceWindSpeedInc();
    return user_player_reply_list_num(setting->reply_files, 0);
  } else if (0 == uni_strcmp(setting->cmd, "ac_speed_dec")) {
    speed = AcDeviceGetWindSpeed();
    if (eAcWindSpeed_1 == speed) {
      return user_player_reply_list_num(setting->reply_files, 1);
    }
    speed = AcDeviceWindSpeedDec();
    return user_player_reply_list_num(setting->reply_files, 0);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  LOGT(TAG, "AC speed state: %d", speed);
  return user_player_reply_list_random(setting->reply_files);
}

static int _ac_mode_replay(event_custom_setting_t *setting) {
  eAcSate state = AcDeviceGetState();
  eAcWindMode mode = eACMODE_UNKONWN;
  if (eACSTATE_ON != state) {
    return _ac_off_replay();
  }
  if (0 == uni_strcmp(setting->cmd, "ac_mode_nat")) {
    mode = AcDeviceSetWindMode(eACMODE_NATURE);
  } else if (0 == uni_strcmp(setting->cmd, "ac_mode_nor")) {
    mode = AcDeviceSetWindMode(eACMODE_NORMAL);
  } else if (0 == uni_strcmp(setting->cmd, "ac_mode_sleep")) {
    mode = AcDeviceSetWindMode(eACMODE_SLEEP);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  LOGT(TAG, "AC mode state: %d", mode);
  return user_player_reply_list_random(setting->reply_files);
}

static int _timer_change_replay(eAcTimer timer, const char *file_list) {
  switch (timer) {
    case eACTIMER_1HR:
      return user_player_reply_list_num(file_list, 0);
    case eACTIMER_2HR:
      return user_player_reply_list_num(file_list, 1);
    case eACTIMER_4HR:
      return user_player_reply_list_num(file_list, 2);
    case eACTIMER_8HR:
      return user_player_reply_list_num(file_list, 3);
    default:
      break;
  }
  return -1;
}

static int _ac_timing_replay(event_custom_setting_t *setting) {
  eAcSate state = AcDeviceGetState();
  eAcTimer timer = eACTIMER_UNKONWN;
  if (eACSTATE_ON != state) {
    return _ac_off_replay();
  }
  if (0 == uni_strcmp(setting->cmd, "ac_timing_1hr")) {
    timer = AcDeviceSetTimer(eACTIMER_1HR);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_2hr")) {
    timer = AcDeviceSetTimer(eACTIMER_2HR);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_4hr")) {
    timer = AcDeviceSetTimer(eACTIMER_4HR);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_8hr")) {
    timer = AcDeviceSetTimer(eACTIMER_8HR);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_cancel")) {
    timer = AcDeviceSetTimer(eACTIMER_NONE);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_inc")) {
    timer = AcDeviceTimerInc();
    return _timer_change_replay(timer, setting->reply_files);
  } else if (0 == uni_strcmp(setting->cmd, "ac_timing_dec")) {
    timer = AcDeviceTimerDec();
    return _timer_change_replay(timer, setting->reply_files);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  LOGT(TAG, "AC timer state: %d", timer);
  return user_player_reply_list_random(setting->reply_files);
}

static int _ac_shake_replay(event_custom_setting_t *setting) {
  eAcSate state = AcDeviceGetState();
  eAcShake shake = eACSHAKE_UNKONWN;
  if (eACSTATE_ON != state) {
    return _ac_off_replay();
  }
  if (0 == uni_strcmp(setting->cmd, "ac_shake_on")) {
    shake = AcDeviceSetShakeState(eACSHAKE_ON);
  } else if (0 == uni_strcmp(setting->cmd, "ac_shake_off")) {
    shake = AcDeviceSetShakeState(eACSHAKE_OFF);
  } else {
    LOGE(TAG, "unknown cmd: %s", setting->cmd);
    return -1;
  }
  LOGT(TAG, "AC shake state: %d", shake);
  return user_player_reply_list_random(setting->reply_files);
}

static void _custom_setting_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_custom_setting_t *setting = NULL;
  if (context) {
    setting = &context->custom_setting;
    if (NULL != uni_strstr(setting->cmd, "ac_power")) {
      _ac_power_replay(setting);
    } else if (NULL != uni_strstr(setting->cmd, "ac_speed")) {
      _ac_speed_replay(setting);
    } else if (NULL != uni_strstr(setting->cmd, "ac_mode")) {
      _ac_mode_replay(setting);
    } else if (NULL != uni_strstr(setting->cmd, "ac_timing")) {
      _ac_timing_replay(setting);
    } else if (NULL != uni_strstr(setting->cmd, "ac_shake")) {
      _ac_shake_replay(setting);
    } else {
      /* don't reply if other setting command */
    }
  }
}

static void _goto_awakend_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_awakend_t *awakend = NULL;
  if (context) {
    awakend = &context->goto_awakend;
    if (EVENT_TRIGGER_ASR == awakend->trigger) {
      LOGT(TAG, "ASR command: %s -> %s -> %s", awakend->cmd, awakend->word_str, awakend->reply_files);
    } else {
      LOGT(TAG, "External command: %s", awakend->reply_files);
    }
    if (awakend->reply_files) {
      if(eACSTATE_ON == AcDeviceGetState()) {
        user_player_reply_list_num(awakend->reply_files, 1);
      } else {
        user_player_reply_list_num(awakend->reply_files, 0);
      }
    }
  }
}

static void _goto_sleeping_cb (USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_sleeping_t *sleeping = NULL;
  if (context) {
    sleeping = &context->goto_sleeping;
    if (EVENT_TRIGGER_ASR == sleeping->trigger) {
      LOGT(TAG, "ASR command: %s -> %s -> %s", sleeping->cmd, sleeping->word_str,
                                               sleeping->reply_files);
    } else {
      LOGT(TAG, "External command: %s", sleeping->reply_files);
    }
    if (sleeping->reply_files) {
      if(EVENT_TRIGGER_USER == sleeping->trigger) {
        user_player_reply_list_random(g_power_off_reply);
      } else {
        user_player_reply_list_random(sleeping->reply_files);
      }
    }
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
  user_event_subscribe_event(USER_GOTO_AWAKENED, _goto_awakend_cb);
  user_event_subscribe_event(USER_GOTO_SLEEPING, _goto_sleeping_cb);
}

int hb_smart_ac(void) {
  AcDeviceInit();
  _register_event_callback();
  return 0;
}

