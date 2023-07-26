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
 * Description : housekeeper_irc.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.06.03
 *
 **************************************************************************/
#include "housekeeper_irc.h"
#include "user_uart.h"
#include "user_flash.h"
#include "user_pwm.h"

#define TAG "housekeeper_irc"

#define PACKED              __attribute__ ((packed))

#define BLOB_KEY_AC_STATE   "ac_state"

#define PACKAGE_START_CODE  0x00

#define DEFAULT_CODE_NUM    0xC000
#define AC_SCAN_CODE_BEGIN  0x8000
#define AC_SCAN_CODE_END    0x87D0

#define RET_MATCH_OK        0x02
#define RET_MATCH_FAL       0x03

#define MSG_CMD_NORMAL      0x00
#define MSG_CMD_MATCH       0x10
#define MSG_CMD_SCAN        0x20
#define MSG_CMD_EXIT        0x40

#define KEY_LAMP_SWITCH     0x00
#define KEY_AC_POWER        0x01
#define KEY_AC_MODE         0x02
#define KEY_AC_WIND         0x03
#define KEY_AC_TMP_INC      0x04
#define KEY_AC_TMP_DEC      0x05
#define KEY_AC_SWEPT_VERT   0x06
#define KEY_AC_SWEPT_CROSS  0x07

#define ARG1_TMP_16         0x00
#define ARG1_TMP_30         0xE0
#define ARG1_POWER_ON       0x08
#define ARG1_POWER_OFF      0x00
#define ARG1_MODE_AUTO      0x00
#define ARG1_MODE_COOL      0x01
#define ARG1_MODE_DRY       0x02
#define ARG1_MODE_FAN       0x03
#define ARG1_MODE_HEAT      0x04

#define ARG2_LAMP_ON        0x80
#define ARG2_LAMP_OFF       0x00
#define ARG2_SWEPT_CRO_ON   0x08
#define ARG2_SWEPT_CRO_OFF  0x00
#define ARG2_SWEPT_VER_ON   0x04
#define ARG2_SWEPT_VER_OFF  0x00
#define ARG2_WIND_AUTO      0x00
#define ARG2_WIND_LOW       0x01
#define ARG2_WIND_MID       0x02
#define ARG2_WIND_HIGH      0x03

#define RESERVE_CODE        0x00

#define AC_SCAN_STEP_TIME   2   // add 4 one step interval 2s
#define LAMP_BLINK_HZ       4   // blink 4st a second

typedef enum {
  UART_RECV_START_CODE = 0,
  UART_RECV_AC_CODE_L,
  UART_RECV_AC_CODE_H,
  UART_RECV_KEY_VUL,
  UART_RECV_ARG_1,
  UART_RECV_ARG_2,
  UART_RECV_RESERVE,
  UART_RECV_CHECKSUM
}UART_RECV_RUN_STATE;

typedef struct {
  uni_u8  start_code;   // 0x00
  uni_u8  code_num[2];  // AC code
  uni_u8  key_val;      // key value
  uni_u8  ac_arg[2];    // AC arguments
  uni_u8  reserve;      // reserve
  uni_u8  checksum;     // checksum
}PACKED irc_com_pack_t;

typedef struct {
  uni_u16       ac_code_num;
  uni_bool      ac_power_state;
  AC_MODE_TYPE  ac_mode;
  AC_WIND_SPEED ac_wind;
  AC_TMP_NUMBER ac_tmp;
  AC_SWEPT_CMD  ac_swept;
}ac_state_t;

typedef struct {
  uni_bool      lamp_state;
  LAMP_LEVEL    lamp_level;
  ac_state_t    ac_state;
  uni_bool      is_sacnning;
  uni_bool      is_matching;
  uni_pthread_t pid;
  uni_bool      running;
  uni_sem_t     wait_start;
  uni_bool      is_stop;
  uni_bool      is_pause;
  uni_sem_t     sem_thread_exit_sync;
  uni_u16       ac_scan_last_code;
  uni_mutex_t   blink_mutex;
}uart_irc_context_t;

static uart_irc_context_t g_uart_irc_context = {0};
static irc_com_pack_t g_recv_package = {0};
static UART_RECV_RUN_STATE g_recv_state = UART_RECV_START_CODE;
static _ac_match_ret_cb g_ac_match_ret_cb = NULL;
static _ac_scan_auto_stop_cb g_ac_scan_auto_stop_cb = NULL;

static uni_u8 _get_checksum_code(irc_com_pack_t *pack) {
  uni_u8 *p = (uni_u8 *)pack;
  uni_u8 code = 0x00;
  uni_u8 i = 0;
  /* not include checksum */
  for (i = 0; i < (sizeof(irc_com_pack_t) - 1); i++) {
    code += *p++;
  }
  return code & 0xFF;
}

static uni_bool _check_checksum_code(irc_com_pack_t *pack) {
  uni_u8 tmp_code = _get_checksum_code(pack);
  if (pack->checksum != tmp_code) {
    LOGE(TAG, "Error chacksum: 0x%02x -- 0x%02x", tmp_code, pack->checksum);
    return false;
  }
  return true;
}

static int _uart_send_pack(irc_com_pack_t *pack) {
  return user_uart_send((char *)pack, sizeof(irc_com_pack_t));
}

static void _parser_package(irc_com_pack_t *pack) {
  uni_u8 ret_code = pack->key_val & 0xF0;
  uni_u16 code_num = 0x00;
  switch (ret_code) {
    case 0x20:  //match success
      if (g_ac_match_ret_cb) {
        g_ac_match_ret_cb(true);
      }
      code_num = pack->code_num[1];
      code_num = (code_num << 8) | pack->code_num[0];
      g_uart_irc_context.ac_state.ac_code_num = code_num;
      g_uart_irc_context.ac_state.ac_mode = AC_MODE_COOL;
      g_uart_irc_context.ac_state.ac_tmp = AC_TMP_25;
      g_uart_irc_context.ac_state.ac_swept = AC_SWEPT_ALL_ON;
      g_uart_irc_context.ac_state.ac_wind = AC_WIND_AUTO;
      g_uart_irc_context.ac_state.ac_power_state = false;
      user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                              sizeof(ac_state_t));
      g_uart_irc_context.is_matching = false;
      break;
    case 0x30:  //match failed
      if (g_ac_match_ret_cb) {
        g_ac_match_ret_cb(false);
      }
      break;
    case 0x50:  //
      if (g_uart_irc_context.is_matching) {
        if (g_ac_match_ret_cb) {
          g_ac_match_ret_cb(true);
        }
        g_uart_irc_context.is_matching = false;
      }
      if (g_uart_irc_context.is_sacnning) {
        housekeeper_irc_ac_scan_stop();
      }
      code_num = pack->code_num[1];
      code_num = (code_num << 8) | pack->code_num[0];
      g_uart_irc_context.ac_state.ac_code_num = code_num;
      g_uart_irc_context.ac_state.ac_mode = AC_MODE_COOL;
      g_uart_irc_context.ac_state.ac_tmp = AC_TMP_25;
      g_uart_irc_context.ac_state.ac_swept = AC_SWEPT_ALL_ON;
      g_uart_irc_context.ac_state.ac_wind = AC_WIND_AUTO;
      g_uart_irc_context.ac_state.ac_power_state = false;
      user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                              sizeof(ac_state_t));
    default:
      break;
  }
}

static void _uart_recv_cb(char *buf, int len) {
  int i = 0;
_GOTO_PARSE_RESET:
  switch (g_recv_state) {
    case UART_RECV_START_CODE:
      /* find start code */
      for (; i < len; i++) {
        if (PACKAGE_START_CODE == buf[i]) {
          break;
        }
      }
      if (i >= len) {
        break;
      }
      g_recv_package.start_code = buf[i];
      g_recv_state = UART_RECV_AC_CODE_L;
      i++;
    case UART_RECV_AC_CODE_L:
      if (i >= len) {
        break;
      }
      g_recv_package.code_num[0] = buf[i];
      g_recv_state = UART_RECV_AC_CODE_H;
      i++;
    case UART_RECV_AC_CODE_H:
      if (i >= len) {
        break;
      }
      g_recv_package.code_num[1] = buf[i];
      g_recv_state = UART_RECV_KEY_VUL;
      i++;
    case UART_RECV_KEY_VUL:
      if (i >= len) {
        break;
      }
      g_recv_package.key_val = buf[i];
      g_recv_state = UART_RECV_ARG_1;
      i++;
    case UART_RECV_ARG_1:
      if (i >= len) {
        break;
      }
      g_recv_package.ac_arg[0] = buf[i];
      g_recv_state = UART_RECV_ARG_2;
      i++;
    case UART_RECV_ARG_2:
      if (i >= len) {
        break;
      }
      g_recv_package.ac_arg[1] = buf[i];
      g_recv_state = UART_RECV_ARG_2;
      i++;
    case UART_RECV_RESERVE:
      if (i >= len) {
        break;
      }
      g_recv_package.reserve = buf[i];
      g_recv_state = UART_RECV_CHECKSUM;
      i++;
    case UART_RECV_CHECKSUM:
      if (i >= len) {
        break;
      }
      g_recv_package.checksum = buf[i];
      if (!_check_checksum_code(&g_recv_package)) {
        g_recv_state = UART_RECV_START_CODE;
        goto _GOTO_PARSE_RESET;
      }
      _parser_package(&g_recv_package);
      g_recv_state = UART_RECV_START_CODE;
      i++;
      goto _GOTO_PARSE_RESET;
    default:
      break;
  }
}

static uni_u8 _get_ac_tmp_code(AC_TMP_NUMBER tmp) {
  uni_u8 tmp_num = 0x00;
  if (tmp > AC_TMP_INVALID && tmp_num < AC_TMP_MAX) {
    tmp_num = tmp - AC_TMP_16;
    return (tmp_num << 4);
  }
  return ARG1_TMP_16;
}

static uni_u8 _get_ac_power_code(uni_bool is_on) {
  if (is_on) {
    return ARG1_POWER_ON;
  } else {
    return ARG1_POWER_OFF;
  }
}

static uni_u8 _get_ac_mode_code(AC_MODE_TYPE mode) {
  uni_u8 mode_num = 0x00;
  switch (mode) {
    case AC_MODE_AUTO:
      mode_num = ARG1_MODE_AUTO;
      break;
    case AC_MODE_COOL:
      mode_num = ARG1_MODE_COOL;
      break;
    case AC_MODE_DRY:
      mode_num = ARG1_MODE_DRY;
      break;
    case AC_MODE_FAN:
      mode_num = ARG1_MODE_FAN;
      break;
    case AC_MODE_HEAT:
      mode_num = ARG1_MODE_HEAT;
      break;
    default:
      mode_num = ARG1_MODE_AUTO;
      break;
  }
  return mode_num;
}

static uni_u8 _get_lamp_power_code(uni_bool is_on) {
  if (is_on) {
    return ARG2_LAMP_ON;
  } else {
    return ARG2_LAMP_OFF;
  }
}

static uni_u8 _get_ac_swept_code(AC_SWEPT_CMD swept) {
  uni_u8 swept_num = 0x00;
  switch (swept) {
    case AC_SWEPT_OFF:
      swept_num = ARG2_SWEPT_CRO_OFF;
      break;
    case AC_SWEPT_ALL_ON:
      swept_num = ARG2_SWEPT_VER_ON | ARG2_SWEPT_CRO_ON;
      break;
    case AC_SWEPT_CROSS_ON:
      swept_num = ARG2_SWEPT_CRO_ON;
      break;
    case AC_SWEPT_VERT_ON:
      swept_num = ARG2_SWEPT_VER_ON;
      break;
    default:
      swept_num = ARG2_SWEPT_CRO_OFF;
      break;
  }
  return swept_num;
}

static uni_u8 _get_ac_wind_code(AC_WIND_SPEED wind) {
  uni_u8 wind_num = 0x00;
  switch (wind) {
    case AC_WIND_AUTO:
      wind_num = ARG2_WIND_AUTO;
      break;
    case AC_WIND_HIGH:
      wind_num = ARG2_WIND_HIGH;
      break;
    case AC_WIND_LOW:
      wind_num = ARG2_WIND_LOW;
      break;
    case AC_WIND_MID:
      wind_num = ARG2_WIND_MID;
      break;
    default:
      wind_num = ARG2_WIND_AUTO;
      break;
  }
  return wind_num;
}

static void _fill_ac_args(irc_com_pack_t *pack) {
  pack->ac_arg[0] = _get_ac_tmp_code(g_uart_irc_context.ac_state.ac_tmp)
                  | _get_ac_power_code(g_uart_irc_context.ac_state.ac_power_state)
                  | _get_ac_mode_code(g_uart_irc_context.ac_state.ac_mode);
  pack->ac_arg[1] = _get_lamp_power_code(g_uart_irc_context.lamp_state)
                  | _get_ac_swept_code(g_uart_irc_context.ac_state.ac_swept)
                  | _get_ac_wind_code(g_uart_irc_context.ac_state.ac_wind);
}

static int _ac_match_scan(uni_u16 code_num) {
  irc_com_pack_t pack = {0};
  pack.start_code = 0x00;
  pack.code_num[0] = code_num & 0xFF;
  pack.code_num[1] = code_num >> 8;
  pack.key_val = MSG_CMD_NORMAL | KEY_AC_POWER;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) <= 0) {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

static void _ac_scan_process(void *args) {
  int count = 0;
  int scan_count = 0;
  g_uart_irc_context.running = true;
  while (g_uart_irc_context.running) {
    uni_sem_wait(g_uart_irc_context.wait_start);
    count = 0;
    scan_count = 0;
    g_uart_irc_context.is_stop = false;
    while (g_uart_irc_context.is_sacnning) {
      if (g_uart_irc_context.is_pause) {
        uni_msleep(100);
        continue;
      }
      /* sleep 3s will block when stop, lump blink use 1s, seg to 20 * 100ms */
      count++;
      if (count >= 20) {
        /* scan filed if not match run a round */
        if (scan_count >= ((AC_SCAN_CODE_END - AC_SCAN_CODE_BEGIN) / 4 + 1)) {
          g_uart_irc_context.ac_state.ac_code_num = g_uart_irc_context.ac_scan_last_code;
          if (g_ac_scan_auto_stop_cb) {
            g_ac_scan_auto_stop_cb();
          }
          /* auto stop scan */
          g_uart_irc_context.is_pause = false;
          g_uart_irc_context.is_sacnning = false;
          break;
        }
        g_uart_irc_context.ac_state.ac_code_num += 4;
        if (g_uart_irc_context.ac_state.ac_code_num > AC_SCAN_CODE_END) {
          g_uart_irc_context.ac_state.ac_code_num = AC_SCAN_CODE_BEGIN;
        }
        _ac_match_scan(g_uart_irc_context.ac_state.ac_code_num);
        count = 0;
        scan_count++;
        housekeeper_irc_lamp_blink();
      }
      uni_msleep(100);
    }
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
    g_uart_irc_context.is_stop = true;
  }
  uni_sem_post(g_uart_irc_context.sem_thread_exit_sync);
}

static Result _create_ac_scan_thread(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "ac_scan", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&g_uart_irc_context.pid, &param,
                              _ac_scan_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(g_uart_irc_context.pid);
  return E_OK;
}

static void _sem_init(void) {
  uni_sem_init(&g_uart_irc_context.wait_start, 0);
  uni_sem_init(&g_uart_irc_context.sem_thread_exit_sync, 0);
  uni_pthread_mutex_init(&g_uart_irc_context.blink_mutex);
}

static void _sem_destroy(void) {
  uni_sem_destroy(g_uart_irc_context.wait_start);
  uni_sem_destroy(g_uart_irc_context.sem_thread_exit_sync);
  uni_pthread_mutex_destroy(g_uart_irc_context.blink_mutex);
}

static void _init_ac_match_state(void) {
  ac_state_t state = {0};
  int save_len = 0;
  user_flash_get_env_blob(BLOB_KEY_AC_STATE, &state,
                          sizeof(ac_state_t), &save_len);
  if (save_len != sizeof(ac_state_t)) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    uni_memcpy(&g_uart_irc_context.ac_state, &state, sizeof(ac_state_t));
  }
}

int housekeeper_irc_init(_ac_match_ret_cb match_cb,
                               _ac_scan_auto_stop_cb scan_cb) {
  if (user_pwm_init(PWM_NUM_1_A27, 2000, true) < 0) {
    LOGE(TAG, "user_pwm_init failed.");
    return -1;
  }
  if (user_uart_init(_uart_recv_cb) < 0) {
    LOGE(TAG, "user_uart_init failed.");
    user_pwm_final(PWM_NUM_1_A27);
    return -1;
  }
  if (0 != user_flash_init()) {
    LOGE(TAG, "user_flash_init failed.");
    user_uart_final();
    user_pwm_final(PWM_NUM_1_A27);
    return -1;
  }
  _sem_init();
  g_uart_irc_context.ac_state.ac_code_num = DEFAULT_CODE_NUM;
  g_uart_irc_context.lamp_state = false;
  g_uart_irc_context.ac_state.ac_power_state = true;
  g_uart_irc_context.ac_state.ac_mode = AC_MODE_COOL;
  g_uart_irc_context.ac_state.ac_tmp = AC_TMP_25;
  g_uart_irc_context.ac_state.ac_swept = AC_SWEPT_ALL_ON;
  g_uart_irc_context.ac_state.ac_wind = AC_WIND_AUTO;
  g_uart_irc_context.is_sacnning = false;
  g_uart_irc_context.is_matching = false;
  g_uart_irc_context.lamp_level = LAMP_BRIGHT_MID;
  if (E_OK != _create_ac_scan_thread()) {
    LOGE(TAG, "_create_ac_scan_thread failed.");
    _sem_destroy();
    user_uart_final();
    user_flash_final();
    user_pwm_final(PWM_NUM_1_A27);
    return -1;
  }
  user_pwm_start(PWM_NUM_1_A27, 0);
  _init_ac_match_state();
  g_ac_match_ret_cb = match_cb;
  g_ac_scan_auto_stop_cb = scan_cb;
  return 0;
}

void housekeeper_irc_final(void) {
  g_uart_irc_context.running = false;
  housekeeper_irc_ac_match_stop();
  uni_sem_wait(g_uart_irc_context.sem_thread_exit_sync);
  _sem_destroy();
  user_uart_final();
  user_pwm_stop(PWM_NUM_1_A27);
  user_pwm_final(PWM_NUM_1_A27);
}

int housekeeper_irc_ac_match_start(void) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot start again.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = 0xFF;
  pack.code_num[1] = 0xFF;
  pack.key_val = MSG_CMD_MATCH | 0x0F;
  pack.ac_arg[0] = 0xFF;
  pack.ac_arg[1] = 0xFF;
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) <= 0) {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  g_uart_irc_context.is_matching = true;
  return 0;
}

int housekeeper_irc_ac_match_stop(void) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.is_matching) {
    LOGE(TAG, "is not matching, cannot stop.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = 0xFF;
  pack.code_num[1] = 0xFF;
  pack.key_val = MSG_CMD_EXIT | 0x0F;
  pack.ac_arg[0] = 0xFF;
  pack.ac_arg[1] = 0xFF;
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) <= 0) {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  g_uart_irc_context.is_matching = false;
  return 0;
}

uni_u16 housekeeper_irc_get_ac_match_code(void) {
  return g_uart_irc_context.ac_state.ac_code_num;
}

uni_bool housekeeper_irc_get_lamp_state(void) {
  return g_uart_irc_context.lamp_state;
}

static int _set_lamp_bright_level(LAMP_LEVEL level) {
  switch (level) {
    case LAMP_BRIGHT_LOW:
      user_pwm_change_duty(PWM_NUM_1_A27, 15);
      break;
    case LAMP_BRIGHT_LM:
      user_pwm_change_duty(PWM_NUM_1_A27, 30);
      break;
    case LAMP_BRIGHT_MID:
      user_pwm_change_duty(PWM_NUM_1_A27, 50);
      break;
    case LAMP_BRIGHT_MH:
      user_pwm_change_duty(PWM_NUM_1_A27, 75);
      break;
    case LAMP_BRIGHT_HIGH:
      user_pwm_change_duty(PWM_NUM_1_A27, 100);
      break;
    default:
      return -1;
  }
  return 0;
}

int housekeeper_irc_set_lamp_state(uni_bool is_on) {
  g_uart_irc_context.lamp_state = is_on;
  if (g_uart_irc_context.lamp_state) {
    _set_lamp_bright_level(g_uart_irc_context.lamp_level);
  } else {
    user_pwm_change_duty(PWM_NUM_1_A27, 0);
  }
  return 0;
}

LAMP_LEVEL housekeeper_irc_get_lamp_level(void) {
  return g_uart_irc_context.lamp_level;
}

int housekeeper_irc_lamp_blink(void) {
  int i = 0;
  uni_pthread_mutex_lock(g_uart_irc_context.blink_mutex);
  for (i = 0; i < 3; i++) {
    user_pwm_change_duty(PWM_NUM_1_A27, 0);
    uni_msleep(1000 / LAMP_BLINK_HZ / 2);
    _set_lamp_bright_level(g_uart_irc_context.lamp_level);
    uni_msleep(1000 / LAMP_BLINK_HZ / 2);
  }
  if (!g_uart_irc_context.lamp_state) {
    user_pwm_change_duty(PWM_NUM_1_A27, 0);
  }
  uni_pthread_mutex_unlock(g_uart_irc_context.blink_mutex);
  return 0;
}

int housekeeper_irc_lamp_bright_inc(void) {
  LAMP_LEVEL level = LAMP_BRIGHT_INVALID;
  if (!g_uart_irc_context.lamp_state) {
    LOGE(TAG, "lamp was turn off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.lamp_level >= LAMP_BRIGHT_HIGH) {
    level = LAMP_BRIGHT_HIGH;
  } else if (g_uart_irc_context.lamp_level < LAMP_BRIGHT_LOW) {
    level = LAMP_BRIGHT_LOW;
  } else {
    level = g_uart_irc_context.lamp_level + 1;
  }
  if (0 == _set_lamp_bright_level(level)) {
    g_uart_irc_context.lamp_level = level;
  }
  return 0;
}

int housekeeper_irc_lamp_bright_dec(void) {
  LAMP_LEVEL level = LAMP_BRIGHT_INVALID;
  if (!g_uart_irc_context.lamp_state) {
    LOGE(TAG, "lamp was turn off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.lamp_level <= LAMP_BRIGHT_LOW) {
    level = LAMP_BRIGHT_LOW;
  } else if (g_uart_irc_context.lamp_level > LAMP_BRIGHT_HIGH) {
    level = LAMP_BRIGHT_HIGH;
  } else {
    level = g_uart_irc_context.lamp_level - 1;
  }
  if (0 == _set_lamp_bright_level(level)) {
    g_uart_irc_context.lamp_level = level;
  }
  return 0;
}

uni_bool housekeeper_irc_get_ac_state(void) {
  return g_uart_irc_context.ac_state.ac_power_state;
}

int housekeeper_irc_set_ac_state(uni_bool is_on) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = g_uart_irc_context.ac_state.ac_code_num & 0xFF;
  pack.code_num[1] = g_uart_irc_context.ac_state.ac_code_num >> 8;
  pack.key_val = MSG_CMD_NORMAL | KEY_AC_POWER;
  g_uart_irc_context.ac_state.ac_power_state = is_on;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) > 0) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

AC_MODE_TYPE housekeeper_irc_get_ac_mode(void) {
  return g_uart_irc_context.ac_state.ac_mode;
}

int housekeeper_irc_set_ac_mode(AC_MODE_TYPE mode) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = g_uart_irc_context.ac_state.ac_code_num & 0xFF;
  pack.code_num[1] = g_uart_irc_context.ac_state.ac_code_num >> 8;
  pack.key_val = MSG_CMD_NORMAL | KEY_AC_MODE;
  g_uart_irc_context.ac_state.ac_mode = mode;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) > 0) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

int housekeeper_irc_ac_sleep_mode(void) {
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  if (AC_MODE_HEAT == g_uart_irc_context.ac_state.ac_mode) {
    g_uart_irc_context.ac_state.ac_tmp = AC_TMP_22;
  } else {
    g_uart_irc_context.ac_state.ac_tmp = AC_TMP_26;
  }
  return housekeeper_irc_set_ac_wind(AC_WIND_LOW);
}

AC_WIND_SPEED housekeeper_irc_get_ac_wind(void) {
  return g_uart_irc_context.ac_state.ac_wind;
}

int housekeeper_irc_set_ac_wind(AC_WIND_SPEED speed) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = g_uart_irc_context.ac_state.ac_code_num & 0xFF;
  pack.code_num[1] = g_uart_irc_context.ac_state.ac_code_num >> 8;
  pack.key_val = MSG_CMD_NORMAL | KEY_AC_WIND;
  g_uart_irc_context.ac_state.ac_wind = speed;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) > 0) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

int housekeeper_irc_ac_wind_inc(void) {
  AC_WIND_SPEED speed = AC_WIND_INVALID;
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.ac_state.ac_wind >= AC_WIND_HIGH) {
    speed = AC_WIND_HIGH;
  } else if (g_uart_irc_context.ac_state.ac_wind <= AC_WIND_AUTO) {
    speed = AC_WIND_HIGH;
  } else {
    speed = g_uart_irc_context.ac_state.ac_wind + 1;
  }
  housekeeper_irc_set_ac_wind(speed);
  return 0;
}

int housekeeper_irc_ac_wind_dec(void) {
  AC_WIND_SPEED speed = AC_WIND_INVALID;
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.ac_state.ac_wind <= AC_WIND_LOW) {
    speed = AC_WIND_LOW;
  } else if (g_uart_irc_context.ac_state.ac_wind > AC_WIND_HIGH) {
    speed = AC_WIND_HIGH;
  } else {
    speed = g_uart_irc_context.ac_state.ac_wind - 1;
  }
  housekeeper_irc_set_ac_wind(speed);
  return 0;
}

AC_TMP_NUMBER housekeeper_irc_get_ac_tmp(void) {
  return g_uart_irc_context.ac_state.ac_tmp;
}

int housekeeper_irc_set_ac_tmp(AC_TMP_NUMBER tmp) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = g_uart_irc_context.ac_state.ac_code_num & 0xFF;
  pack.code_num[1] = g_uart_irc_context.ac_state.ac_code_num >> 8;
  if (g_uart_irc_context.ac_state.ac_tmp <= tmp
    && tmp != AC_TMP_16) {
    pack.key_val = MSG_CMD_NORMAL | KEY_AC_TMP_INC;
  } else {
    pack.key_val = MSG_CMD_NORMAL | KEY_AC_TMP_DEC;
  }
  g_uart_irc_context.ac_state.ac_tmp = tmp;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) > 0) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

int housekeeper_irc_ac_tmp_inc(void) {
  AC_TMP_NUMBER tmp = AC_TMP_INVALID;
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.ac_state.ac_tmp >= AC_TMP_30) {
    tmp = AC_TMP_30;
  } else if (g_uart_irc_context.ac_state.ac_tmp < AC_TMP_16) {
    tmp = AC_TMP_16;
  } else {
    tmp = g_uart_irc_context.ac_state.ac_tmp + 1;
  }
  housekeeper_irc_set_ac_tmp(tmp);
  return 0;
}

int housekeeper_irc_ac_tmp_dec(void) {
  AC_TMP_NUMBER tmp = AC_WIND_INVALID;
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.ac_state.ac_tmp <= AC_TMP_16) {
    tmp = AC_TMP_16;
  } else if (g_uart_irc_context.ac_state.ac_tmp > AC_TMP_30) {
    tmp = AC_TMP_30;
  } else {
    tmp = g_uart_irc_context.ac_state.ac_tmp - 1;
  }
  housekeeper_irc_set_ac_tmp(tmp);
  return 0;
}

AC_SWEPT_CMD housekeeper_irc_get_ac_swept(void) {
  return g_uart_irc_context.ac_state.ac_swept;
}

int housekeeper_irc_set_ac_swept(AC_SWEPT_CMD swept) {
  irc_com_pack_t pack = {0};
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (!g_uart_irc_context.ac_state.ac_power_state) {
    LOGE(TAG, "AC is power off, cannot set.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  pack.start_code = 0x00;
  pack.code_num[0] = g_uart_irc_context.ac_state.ac_code_num & 0xFF;
  pack.code_num[1] = g_uart_irc_context.ac_state.ac_code_num >> 8;
  if (AC_SWEPT_CROSS_ON == swept) {
    pack.key_val = MSG_CMD_NORMAL | KEY_AC_SWEPT_CROSS;
  } else {
    pack.key_val = MSG_CMD_NORMAL | KEY_AC_SWEPT_VERT;
  }
  g_uart_irc_context.ac_state.ac_swept = swept;
  _fill_ac_args(&pack);
  pack.reserve = RESERVE_CODE;
  pack.checksum = _get_checksum_code(&pack);
  if (_uart_send_pack(&pack) > 0) {
    user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                            sizeof(ac_state_t));
  } else {
    LOGE(TAG, "_uart_send_pack failed.");
    return -1;
  }
  return 0;
}

int housekeeper_irc_ac_scan_start(void) {
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "AC scanning, cannot start again.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  g_uart_irc_context.ac_scan_last_code = g_uart_irc_context.ac_state.ac_code_num;
  if ((DEFAULT_CODE_NUM == g_uart_irc_context.ac_state.ac_code_num)
    || ((g_uart_irc_context.ac_state.ac_code_num & 0x8000) == 0x00)) {
    g_uart_irc_context.ac_state.ac_code_num = AC_SCAN_CODE_BEGIN - 4;
  }
  g_uart_irc_context.ac_state.ac_mode = AC_MODE_COOL;
  g_uart_irc_context.ac_state.ac_tmp = AC_TMP_25;
  g_uart_irc_context.ac_state.ac_swept = AC_SWEPT_ALL_ON;
  g_uart_irc_context.ac_state.ac_wind = AC_WIND_AUTO;
  g_uart_irc_context.ac_state.ac_power_state = true;
  g_uart_irc_context.is_sacnning = true;
  g_uart_irc_context.is_pause = false;
  uni_sem_post(g_uart_irc_context.wait_start);
  return 0;
}

int housekeeper_irc_ac_scan_stop(void) {
  if (g_uart_irc_context.is_sacnning) {
    g_uart_irc_context.is_pause = false;
    g_uart_irc_context.is_sacnning = false;
    uni_sem_post(g_uart_irc_context.wait_start);
    while (!g_uart_irc_context.is_stop) {
      uni_msleep(5);
    }
  }
  return 0;
}

int housekeeper_irc_ac_scan_pause(void) {
  if (g_uart_irc_context.is_sacnning) {
    g_uart_irc_context.is_pause = true;
  }
  return 0;
}

int housekeeper_irc_ac_scan_resume(void) {
  if (g_uart_irc_context.is_sacnning) {
    g_uart_irc_context.is_pause = false;
  }
  return 0;
}

int housekeeper_irc_ac_restore(void) {
  if (g_uart_irc_context.is_sacnning) {
    LOGE(TAG, "is scanning, cannot perform other tasks.");
    return -1;
  }
  if (g_uart_irc_context.is_matching) {
    LOGE(TAG, "is matching, cannot set.");
    return -1;
  }
  g_uart_irc_context.ac_state.ac_code_num = DEFAULT_CODE_NUM;
  g_uart_irc_context.ac_state.ac_mode = AC_MODE_COOL;
  g_uart_irc_context.ac_state.ac_tmp = AC_TMP_25;
  g_uart_irc_context.ac_state.ac_swept = AC_SWEPT_ALL_ON;
  g_uart_irc_context.ac_state.ac_wind = AC_WIND_AUTO;
  g_uart_irc_context.ac_state.ac_power_state = true;
  g_uart_irc_context.lamp_state = false;
  g_uart_irc_context.lamp_level = LAMP_BRIGHT_MID;
  housekeeper_irc_set_lamp_state(g_uart_irc_context.lamp_state);
  if (0 == housekeeper_irc_set_ac_state(g_uart_irc_context.ac_state.ac_power_state)) {
   user_flash_set_env_blob(BLOB_KEY_AC_STATE, &g_uart_irc_context.ac_state,
                           sizeof(ac_state_t));
  } else {
    LOGE(TAG, "housekeeper_irc_set_ac_state failed.");
    return -1;
  }
  return 0;
}

uni_bool housekeeper_irc_is_ac_match(void) {
  return g_uart_irc_context.is_matching;
}

uni_bool housekeeper_irc_is_ac_scan(void) {
  return g_uart_irc_context.is_sacnning;
}

