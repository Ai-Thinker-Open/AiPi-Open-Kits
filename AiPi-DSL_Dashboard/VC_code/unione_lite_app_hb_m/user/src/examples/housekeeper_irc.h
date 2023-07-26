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
 * Description : housekeeper_irc.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.06.03
 *
 **************************************************************************/
#ifndef USER_INC_HOUSEKEEPER_IRC_H_
#define USER_INC_HOUSEKEEPER_IRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"

typedef enum {
  AC_MODE_INVALID = -1,
  AC_MODE_AUTO,
  AC_MODE_COOL,
  AC_MODE_HEAT,
  AC_MODE_DRY,
  AC_MODE_FAN,
  AC_MODE_MAX
}AC_MODE_TYPE;

typedef enum {
  AC_WIND_INVALID = -1,
  AC_WIND_AUTO,
  AC_WIND_LOW,
  AC_WIND_MID,
  AC_WIND_HIGH,
  AC_WIND_MAX
}AC_WIND_SPEED;

typedef enum {
  AC_TMP_INVALID = -1,
  AC_TMP_16,
  AC_TMP_17,
  AC_TMP_18,
  AC_TMP_19,
  AC_TMP_20,
  AC_TMP_21,
  AC_TMP_22,
  AC_TMP_23,
  AC_TMP_24,
  AC_TMP_25,
  AC_TMP_26,
  AC_TMP_27,
  AC_TMP_28,
  AC_TMP_29,
  AC_TMP_30,
  AC_TMP_MAX
}AC_TMP_NUMBER;

typedef enum {
  AC_SWEPT_INVALID = -1,
  AC_SWEPT_OFF,
  AC_SWEPT_ALL_ON,
  AC_SWEPT_CROSS_ON,
  AC_SWEPT_VERT_ON,
  AC_SWEPT_MAX
}AC_SWEPT_CMD;

typedef enum {
  LAMP_BRIGHT_INVALID = -1,
  LAMP_BRIGHT_LOW,
  LAMP_BRIGHT_LM,
  LAMP_BRIGHT_MID,
  LAMP_BRIGHT_MH,
  LAMP_BRIGHT_HIGH,
  LAMP_BRIGHT_MAX
}LAMP_LEVEL;

typedef void (*_ac_match_ret_cb)(uni_bool is_ok);

typedef void (*_ac_scan_auto_stop_cb)(void);

int housekeeper_irc_init(_ac_match_ret_cb match_cb,
                               _ac_scan_auto_stop_cb scan_cb);

void housekeeper_irc_final(void);

int housekeeper_irc_ac_match_start(void);

int housekeeper_irc_ac_match_stop(void);

uni_u16 housekeeper_irc_get_ac_match_code(void);

uni_bool housekeeper_irc_get_lamp_state(void);

int housekeeper_irc_set_lamp_state(uni_bool is_on);

LAMP_LEVEL housekeeper_irc_get_lamp_level(void);

int housekeeper_irc_lamp_blink(void);

int housekeeper_irc_lamp_bright_inc(void);

int housekeeper_irc_lamp_bright_dec(void);

uni_bool housekeeper_irc_get_ac_state(void);

int housekeeper_irc_set_ac_state(uni_bool is_on);

AC_MODE_TYPE housekeeper_irc_get_ac_mode(void);

int housekeeper_irc_set_ac_mode(AC_MODE_TYPE mode);

AC_WIND_SPEED housekeeper_irc_get_ac_wind(void);

int housekeeper_irc_set_ac_wind(AC_WIND_SPEED speed);

int housekeeper_irc_ac_wind_inc(void);

int housekeeper_irc_ac_wind_dec(void);

AC_TMP_NUMBER housekeeper_irc_get_ac_tmp(void);

int housekeeper_irc_set_ac_tmp(AC_TMP_NUMBER tmp);

int housekeeper_irc_ac_tmp_inc(void);

int housekeeper_irc_ac_tmp_dec(void);

AC_SWEPT_CMD housekeeper_irc_get_ac_swept(void);

int housekeeper_irc_set_ac_swept(AC_SWEPT_CMD swept);

int housekeeper_irc_ac_scan_start(void);

int housekeeper_irc_ac_scan_stop(void);

int housekeeper_irc_ac_scan_pause(void);

int housekeeper_irc_ac_scan_resume(void);

int housekeeper_irc_ac_restore(void);

uni_bool housekeeper_irc_is_ac_match(void);

uni_bool housekeeper_irc_is_ac_scan(void);

int housekeeper_irc_ac_sleep_mode(void);

#ifdef __cplusplus
}
#endif
#endif

