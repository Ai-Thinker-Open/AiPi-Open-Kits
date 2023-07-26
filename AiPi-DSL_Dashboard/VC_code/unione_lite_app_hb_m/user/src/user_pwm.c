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
 * Description : user_pwm.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.26
 *
 **************************************************************************/
#include "user_pwm.h"
#include "uni_hal_pwm.h"

#define TAG "user_pwm"

#define PWM_HZ_MAX    (1000 * 1000)   // max 1MHz (it's 60MHz on HB-M)

typedef enum {
  TIMER_NUM_3 = 0,    //TIMER3的PWM引脚可复用在A27, B0或者B2
  TIMER_NUM_4,        //TIMER4的PWM引脚可复用在A28, B1或者B3
  TIMER_NUM_MAX
}TIMER_NUM;

typedef struct {
  TIMER_INDEX     TimerIdx;
  uint8_t         PwmIoSel;
  PWM_StructInit  PWMParam;
}pwm_timer_context_t;

static pwm_timer_context_t g_timer_pwm[TIMER_NUM_MAX] = {{0}};

static TIMER_NUM _get_pwm_timer_num(USER_PWM_NUM num) {
  switch (num) {
    case PWM_NUM_1_A27:
      return TIMER_NUM_3;
    case PWM_NUM_1_B0:
      return TIMER_NUM_3;
    case PWM_NUM_1_B2:
      return TIMER_NUM_3;
    case PWM_NUM_2_A28:
      return TIMER_NUM_4;
    case PWM_NUM_2_B1:
      return TIMER_NUM_4;
    case PWM_NUM_2_B3:
      return TIMER_NUM_4;
    default:
      break;
  }
  return TIMER_NUM_MAX;
}

static uint8_t _get_pwm_to_sel(USER_PWM_NUM num) {
  switch (num) {
    case PWM_NUM_1_A27:
      return 1;
    case PWM_NUM_1_B0:
      return 2;
    case PWM_NUM_1_B2:
      return 3;
    case PWM_NUM_2_A28:
      return 1;
    case PWM_NUM_2_B1:
      return 2;
    case PWM_NUM_2_B3:
      return 3;
    default:
      break;
  }
  return 0;
}

int user_pwm_init(USER_PWM_NUM num, uni_u32 hz, uni_bool is_high_duty) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (timer >= TIMER_NUM_MAX || timer < TIMER_NUM_3) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  if (hz > PWM_HZ_MAX || hz <= 0) {
    LOGE(TAG, "the Maximum frequency is %d", PWM_HZ_MAX);
    return -1;
  }
  /* if use pwm already in use, cancel and resetart */
  if (g_timer_pwm[timer].PwmIoSel > 0) {
    uni_hal_pwm_uninit(g_timer_pwm[timer].TimerIdx, g_timer_pwm[timer].PwmIoSel);
  }
  g_timer_pwm[timer].PwmIoSel = _get_pwm_to_sel(num);
  g_timer_pwm[timer].TimerIdx = (timer == TIMER_NUM_3) ? TIMER3 : TIMER4;
  g_timer_pwm[timer].PWMParam.CounterMode = PWM_COUNTER_MODE_DOWN;
  g_timer_pwm[timer].PWMParam.DMAReqEnable = PWM_REQ_INTERRUPT_MODE;
  g_timer_pwm[timer].PWMParam.Duty = 0;
  g_timer_pwm[timer].PWMParam.FreqDiv = 120000000L / hz;
  if (is_high_duty) {
    g_timer_pwm[timer].PWMParam.OutputType = PWM_OUTPUT_SINGLE_1;
  } else {
    g_timer_pwm[timer].PWMParam.OutputType = PWM_OUTPUT_SINGLE_2;
  }
  ret = uni_hal_pwm_init(g_timer_pwm[timer].TimerIdx,
                         g_timer_pwm[timer].PwmIoSel,
                        &g_timer_pwm[timer].PWMParam);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_init failed : %d", ret);
    uni_memset(&g_timer_pwm[timer], 0, sizeof(pwm_timer_context_t));
    return -1;
  }
  return 0;
}

int user_pwm_final(USER_PWM_NUM num) {
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  uni_hal_pwm_disable(g_timer_pwm[timer].TimerIdx);
  uni_hal_pwm_uninit(g_timer_pwm[timer].TimerIdx, g_timer_pwm[timer].PwmIoSel);
  uni_memset(&g_timer_pwm[timer], 0, sizeof(pwm_timer_context_t));
  return 0;
}

int user_pwm_start(USER_PWM_NUM num, uni_u8 duty) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  ret = uni_hal_pwm_enable(g_timer_pwm[timer].TimerIdx);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_enable failed: %d", ret);
    return -1;
  }
  ret = uni_hal_pwm_update_duty(g_timer_pwm[timer].TimerIdx, duty);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_update_duty failed: %d", ret);
    uni_hal_pwm_disable(g_timer_pwm[timer].TimerIdx);
    return -1;
  }
  g_timer_pwm[timer].PWMParam.Duty = duty;
  return 0;
}

int user_pwm_stop(USER_PWM_NUM num) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  ret = uni_hal_pwm_disable(g_timer_pwm[timer].TimerIdx);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_disable failed: %d", ret);
    return -1;
  }
  return 0;
}

int user_pwm_pause(USER_PWM_NUM num) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  ret = uni_hal_pwm_pause(g_timer_pwm[timer].TimerIdx, 1);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_pause failed: %d", ret);
    return -1;
  }
  return 0;
}

int user_pwm_resume(USER_PWM_NUM num) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  ret = uni_hal_pwm_pause(g_timer_pwm[timer].TimerIdx, 0);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_pause failed: %d", ret);
    return -1;
  }
  return 0;
}

int user_pwm_change_duty(USER_PWM_NUM num, uni_u8 duty) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  ret = uni_hal_pwm_update_duty(g_timer_pwm[timer].TimerIdx, duty);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_update_duty failed: %d", ret);
    return -1;
  }
  g_timer_pwm[timer].PWMParam.Duty = duty;
  return 0;
}

int user_pwm_duty_inc(USER_PWM_NUM num, uni_u8 ch_duty) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  uni_u8 duty = g_timer_pwm[timer].PWMParam.Duty;
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  duty += ch_duty;
  if (duty > 100) {
    duty = 100;
  }
  ret = uni_hal_pwm_update_duty(g_timer_pwm[timer].TimerIdx, duty);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_update_duty failed: %d", ret);
    return -1;
  }
  g_timer_pwm[timer].PWMParam.Duty = duty;
  return 0;
}

int user_pwm_duty_dec(USER_PWM_NUM num, uni_u8 ch_duty) {
  PWM_ERROR_CODE ret = PWM_ERROR_OK;
  TIMER_NUM timer = _get_pwm_timer_num(num);
  uni_u8 duty = g_timer_pwm[timer].PWMParam.Duty;
  if (TIMER_NUM_MAX == timer) {
    LOGE(TAG, "unknown USER_PWM_NUM %d", num);
    return -1;
  }
  if (duty < ch_duty) {
    duty = ch_duty;
  }
  duty -= ch_duty;
  ret = uni_hal_pwm_update_duty(g_timer_pwm[timer].TimerIdx, duty);
  if (PWM_ERROR_OK != ret) {
    LOGE(TAG, "uni_hal_pwm_update_duty failed: %d", ret);
    return -1;
  }
  g_timer_pwm[timer].PWMParam.Duty = duty;
  return 0;
}

