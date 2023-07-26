/**************************************************************************
 * Copyright (C) 2020-2020  Unisound
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
 * Description : user_timer.c
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#include "user_timer.h"
#include "uni_hal_timer.h"

#define TAG "user_timer"

static usr_timer_callback g_timer_cb[eTIMER_Max] = {NULL};

static TIMER_INDEX _get_timer_index(eTIMER_IDX idx) {
  TIMER_INDEX TimerIdx = 0xFF;
  switch (idx) {
    case eTIMER2:
      TimerIdx = TIMER2;
      break;
    case eTIMER5:
      TimerIdx = TIMER5;
      break;
    case eTIMER6:
      TimerIdx = TIMER6;
      break;
    default:
      break;
  }
  return TimerIdx;
}

static void _cb_timer_handler(TIMER_INDEX TimerIdx) {
  switch (TimerIdx) {
    case TIMER2:
      if (g_timer_cb[eTIMER2]) {
        g_timer_cb[eTIMER2](eTIMER2);
      }
      break;
    case TIMER5:
      if (g_timer_cb[eTIMER5]) {
        g_timer_cb[eTIMER5](eTIMER5);
      }
      break;
    case TIMER6:
      if (g_timer_cb[eTIMER6]) {
        g_timer_cb[eTIMER6](eTIMER6);
      }
      break;
    default:
      break;
  }
}

int user_timer_init(eTIMER_IDX idx, uint32_t usec, bool single_shot,
                    usr_timer_callback cb) {
  TIMER_ERROR_CODE ret;
  ret = uni_hal_timer_init(_get_timer_index(idx), usec, single_shot, _cb_timer_handler);
  if (TIMER_ERROR_OK == ret) {
    g_timer_cb[idx] = cb;
    return 0;
  }
  return -1;
}

void user_timer_start(eTIMER_IDX idx) {
  uni_hal_timer_start(_get_timer_index(idx));
}

void user_timer_pause(eTIMER_IDX idx) {
  uni_hal_timer_pause(_get_timer_index(idx), true);
}

void user_timer_resume(eTIMER_IDX idx) {
  uni_hal_timer_pause(_get_timer_index(idx), false);
}

