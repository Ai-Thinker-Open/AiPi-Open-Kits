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
 * Description : main.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.12
 *
 **************************************************************************/

#include "uni_iot.h"
#include "uni_log.h"
#include "uni_timer.h"
#include "uni_event_route.h"
#include "uni_event_list.h"
#include "uni_recog_service.h"
#include "uni_media_player.h"
#include "uni_session_manage.h"
#include "uni_wakeup_session.h"
#include "uni_setting_session.h"
#include "uni_watchdog_session.h"
#include "uni_user_meeting.h"
#include "uni_black_board.h"
#include "idle_detect.h"
#include "uni_version.h"
#include "uni_record_save.h"
#include "uni_config.h"
#include "uni_hal_reset.h"
#include "uni_hal_uartcmd.h"
#include "uni_pcm_default.h"
#include "uni_cust_config.h"
#include "user_config.h"
#include "user_main.h"
#include "user_player.h"
#include "uni_hal_audio.h"
#include "uni_hal_uart_upgrade.h"
#include "uni_hal_factory.h"

#define MAIN_TAG                      "main"

static int _session_init(void);
static int _session_final(void);

static void _send_watchdog_feed_event() {
  Event *event = EventCreate(EVENT_SEQUENCE_ID_DEFAULT,
                             ID(WATCH_DOG_FEED_EVENT), NULL, NULL);
  EventRouteProcess(event);
  EventFree(event);
  LOGT(MAIN_TAG, "send watchdog event[%d]", ID(WATCH_DOG_FEED_EVENT));
}

static void _system_startup_status(void) {
  wakeup_type type;
  type = uni_hal_get_boot_status();
  uni_hal_clear_boot_status();
  switch (type) {
    case WAKEUP_BY_POWERON:
      LOGT(MAIN_TAG, "WAKEUP_BY_POWERON");
      BbWrite(BB_KEY_BOOT_MUTE, false);
      break;
    case WAKEUP_BY_WATCHDOG:
      LOGT(MAIN_TAG, "WAKEUP_BY_WATCHDOG");
      BbWrite(BB_KEY_BOOT_MUTE, true);
      break;
    case WAKEUP_BY_SYSTEM_RESET:
      LOGT(MAIN_TAG, "WAKEUP_BY_SYSTEM_RESET");
      BbWrite(BB_KEY_BOOT_MUTE, true);
      break;
    case WAKEUP_BY_RESET_PIN:
      LOGT(MAIN_TAG, "WAKEUP_BY_RESET_PIN");
      BbWrite(BB_KEY_BOOT_MUTE, false);
      break;
    case WAKEUP_BY_LVD:
      LOGT(MAIN_TAG, "WAKEUP_BY_LVD");
      BbWrite(BB_KEY_BOOT_MUTE, true);
      break;
    case WAKEUP_BY_UNKNOWN:
      LOGT(MAIN_TAG, "WAKEUP_BY_UNKNOWN");
      BbWrite(BB_KEY_BOOT_MUTE, true);
      break;
    default:
      LOGT(MAIN_TAG, "wakeup status error !!!!");
      BbWrite(BB_KEY_BOOT_MUTE, true);
      break;
  }
}

static int _init_log_print(void) {
  uni_s64 tmp = 0;
  if (0 != ConfigReadItemNumber("log.enable", &tmp)) {
    printf("Cannot found log.enable, disable log print\r\n");
    return 0;
  }
  if (tmp) {
    if (0 != LogInitialize()) {
      printf("LogInitialize failed !\r\n");
      return -1;
    }
    LogLevelSet(N_LOG_NONE);
    if (0 == ConfigReadItemNumber("log.set_level", &tmp)) {
      LogLevelSet((LogLevel)tmp);
    }
  }
  return 0;
}

static void _play_boot_tone(void) {
  int mute = false;
  BbRead(BB_KEY_BOOT_MUTE, &mute);
  LOGD(MAIN_TAG, "mute = %d", mute);
  if (!mute) {
    uni_sleep(1);
#ifdef DEFAULT_PCM_WELCOME
    user_player_reply_list_random(DEFAULT_PCM_WELCOME);
#else
    user_player_reply_list_random(NICE_TO_MEET_YOU);
#endif
  }
  return;
}

#define STACK_STATUS_DEBUG 0
#if STACK_STATUS_DEBUG
static void _task_list() {
  static char pWriteBuffer[1024];
  vTaskList((char *)&pWriteBuffer);
  LOGW(MAIN_TAG, "free:%d", xPortGetFreeHeapSize());
  LOGW(MAIN_TAG, "task_name  task_state  priority  stack  tasK_num");
  LOGW(MAIN_TAG, "%s", pWriteBuffer);
  return;
}
#endif

#if AUTO_RESET_TIME
static void _auto_reset_cb(IdleType idle_status) {
  if (IDLE_DETECT_TYPE_IDLE == idle_status) {
    WatchDogReboot();
  } else {
    LOGT(MAIN_TAG, "idle cb:%d", idle_status);
  }
}
#endif

static void _system_start_process(void *args) {
  MediaPlayerparam param = {1};
  if (0 != ConfigInitialize()) {
    goto L_CONFIG_INIT_FAILED;
  }
  if (0 != _init_log_print()) {
    goto L_LOG_INIT_FAILED;
  }
  LOGT(MAIN_TAG, "System start woking !");
#if UNI_RECORD_SAVE_ENABLE
  if (E_OK != RecordSaveInit()) {
    goto L_RECORD_SAVE_INIT_FAILED;
  }
#endif
  if (0 != BbInit()) {
    LOGE(MAIN_TAG, "Black board init failed");
    goto L_CLACK_BOARD_INIT_FAILED;
  }
  /* set black board key by stratup status */
  _system_startup_status();
  if (E_OK != TimerInitialize()) {
    LOGE(MAIN_TAG, "timer init failed");
    goto L_TIMER_INIT_FAILED;
  }
  if (0 != IdleDetectServiceInit()) {
    LOGE(MAIN_TAG, "Idle detect service init failed");
    goto L_IDLE_SERVICE_INIT_FAILED;
  }
  if (E_OK != MediaPlayerInit(&param)) {
    LOGE(MAIN_TAG, "media player init failed");
    goto L_MEDIA_PLAYER_INIT_FAILED;
  }
  if (E_OK != RecogInit()) {
    LOGE(MAIN_TAG, "recog init failed");
    goto L_RECOG_INIT_FAILED;
  }
  if (E_OK != SessionManageInit()) {
    LOGE(MAIN_TAG, "session manage init failed");
    goto L_SESSION_MANAGER_INIT_FAILED;
  }
  if (0 != _session_init()) {
    LOGE(MAIN_TAG, "session init failed");
    goto L_SESSIONS_INIT_FAILED;
  }
  LOGT(MAIN_TAG, "System init successfuly !");

  /* call user main function to run user code */
  if (0 != uni_user_meeting_init()) {
    LOGE(MAIN_TAG, "user meeting init failed");
    goto L_USER_MEETING_FAILED;
  }
  if (0 != user_main()) {
    LOGE(MAIN_TAG, "user man init failed");
    goto L_USER_MAIN_FAILED;
  }
  _play_boot_tone();

#if AUTO_RESET_TIME
  IdleDetectServiceRegister(_auto_reset_cb, AUTO_RESET_TIME);
#endif

#if UNI_WATCHDOG_ENABLE
#if STACK_STATUS_DEBUG
  int loop_cnt = 0;
#endif
  while (1) {
    uni_sleep(1);
    _send_watchdog_feed_event();
#if STACK_STATUS_DEBUG
    if ((loop_cnt++ & 2) == 0) _task_list();
#endif
  }
#endif
  return;

  user_exit();
L_USER_MAIN_FAILED:
  uni_user_meeting_final();
L_USER_MEETING_FAILED:
  _session_final();
L_SESSIONS_INIT_FAILED:
  SessionManageFinal();
L_SESSION_MANAGER_INIT_FAILED:
  RecogFinal();
L_RECOG_INIT_FAILED:
  MediaPlayerFinal();
L_MEDIA_PLAYER_INIT_FAILED:
  IdleDetectServiceFinal();
L_IDLE_SERVICE_INIT_FAILED:
  TimerFinalize();
L_TIMER_INIT_FAILED:
  BbFinal();
L_CLACK_BOARD_INIT_FAILED:
#if UNI_RECORD_SAVE_ENABLE
  RecordSaveFinal();
L_RECORD_SAVE_INIT_FAILED:
#endif
  LogFinalize();
L_LOG_INIT_FAILED:
  ConfigFinalize();
L_CONFIG_INIT_FAILED:
  printf("System init failed !\r\n");
  return;
}

static int _session_init(void) {
  if (E_OK != WakeupSessionInit()) {
    goto L_SESSION_WAKEUP_FAILED;
  }
  if (E_OK != SettingSessionInit()) {
    goto L_SESSION_SETTING_FAILED;
  }
#if UNI_WATCHDOG_ENABLE
  if (E_OK != WatchDogSessionInit()) {
    goto L_SESSION_WATCHDOG_FAILED;
  }
#endif
  return 0;

#if UNI_WATCHDOG_ENABLE
  WatchDogSessionFinal();
L_SESSION_WATCHDOG_FAILED:
#endif
  SettingSessionFinal();
L_SESSION_SETTING_FAILED:
  WakeupSessionFinal();
L_SESSION_WAKEUP_FAILED:
  return -1;
}

static int _session_final(void) {
  SettingSessionFinal();
  WakeupSessionFinal();
#if UNI_WATCHDOG_ENABLE
  WatchDogSessionFinal();
#endif
  return 0;
}

static Result _create_start_process(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_NORMAL_SIZE;
  param.priority = OS_PRIORITY_REALTIME;
  uni_strncpy(param.task_name, "start_process", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param, _system_start_process, NULL)) {
    LOGE(MAIN_TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

//static int _factory_gpio_cb(void) {
//  return 0;
//}


int main(void) {
#if (ACOUSTIC_MODEL_FIELD_FAR == AACOUSTIC_MODEL_FIELD)
  uni_hal_mic_gain_set(0);
#else
  uni_hal_mic_gain_set(16);
#endif
#if FLASH_BOOT_EN
  uni_hal_enable_uart_upgrade(FLASH_BOOT_EN);
#endif
  if (0 != uni_system_init()) {
    goto L_SYSTEM_INIT_FAILED;
  }

  uni_print_version();

  // register factory gpio test cb here if needed
  //uni_hal_gpio_factory_cb_register(_factory_gpio_cb);

  if (!_is_factory_mode()) {
    _create_start_process();
    uni_system_start();
  } else {
    uni_hal_uartcmdinit(NULL);
    vTaskStartScheduler();
    while(1); 
  }

L_SYSTEM_INIT_FAILED:
  return 0;
}

