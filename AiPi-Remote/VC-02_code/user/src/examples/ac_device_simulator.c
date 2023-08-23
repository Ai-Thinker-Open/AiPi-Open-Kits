/**************************************************************************
 * Copyright (C) 2019-2019  Unisound
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
 * Description : ac_device_simulator.c
 * Author      : liuwenzheng@unisound.com
 * Date        : 2019.12.30
 *
 **************************************************************************/

#include "ac_device_simulator.h"
#include "uni_log.h"

#define TAG  "ac_device"

typedef struct {
  eAcSate       state;
  eAcWindSpeed  speed;
  eAcWindMode   mode;
  eAcTimer      timer;
  eAcShake      shake;
}eAcContext_t;

static eAcContext_t g_ac_context = {0};

eAcSate AcDeviceGetState(void) {
  return g_ac_context.state;
}

eAcSate AcDeviceSetState(eAcSate state) {
  if ((state > eACSTATE_UNKONWN) && (state < eACSTATE_MAX)) {
    g_ac_context.state = state;
  }
  return g_ac_context.state;
}

eAcWindSpeed AcDeviceGetWindSpeed(void) {
  return g_ac_context.speed;
}

eAcWindSpeed AcDeviceSetWindSpeed(eAcWindSpeed speed) {
  if ((speed > eAcWindSpeed_UNKONWN) && (speed < eAcWindSpeed_MAX)) {
    g_ac_context.speed = speed;
  }
  return g_ac_context.speed;
}

eAcWindSpeed AcDeviceWindSpeedInc(void) {
  if (eAcWindSpeed_UNKONWN == g_ac_context.speed) {
    g_ac_context.speed = eAcWindSpeed_1;
  } else if (g_ac_context.speed < eAcWindSpeed_3) {
    g_ac_context.speed += 1;
  }
  return g_ac_context.speed;
}

eAcWindSpeed AcDeviceWindSpeedDec(void) {
  if (eAcWindSpeed_UNKONWN == g_ac_context.speed) {
    g_ac_context.speed = eAcWindSpeed_3;
  } else if (g_ac_context.speed > eAcWindSpeed_1) {
    g_ac_context.speed -= 1;
  }
  return g_ac_context.speed;
}

eAcWindMode AcDeviceGetWindMode(void) {
  return g_ac_context.mode;
}

eAcWindMode AcDeviceSetWindMode(eAcWindMode mode) {
  if ((mode > eACMODE_UNKONWN) && (mode < eACMODE_MAX)) {
    g_ac_context.mode = mode;
  }
  return g_ac_context.mode;
}

eAcTimer AcDeviceGetTimer(void) {
  return g_ac_context.timer;
}

eAcTimer AcDeviceSetTimer(eAcTimer hr) {
  if ((hr > eACTIMER_UNKONWN) && (hr < eACTIMER_MAX)) {
    g_ac_context.timer = hr;
  }
  return g_ac_context.timer;
}

eAcTimer AcDeviceTimerInc(void) {
  if (eACTIMER_UNKONWN == g_ac_context.timer) {
    g_ac_context.timer = eACTIMER_1HR;
  } else if (g_ac_context.timer < eACTIMER_8HR) {
    g_ac_context.timer += 1;
  }
  return g_ac_context.timer;
}

eAcTimer AcDeviceTimerDec(void) {
  if ((eACTIMER_UNKONWN == g_ac_context.timer)
    || (eACTIMER_NONE == g_ac_context.timer)) {
    g_ac_context.timer = eACTIMER_8HR;
  } else if (g_ac_context.timer > eACTIMER_1HR) {
    g_ac_context.timer -= 1;
  }
  return g_ac_context.timer;
}

eAcShake AcDeviceGetShakeState(void) {
  return g_ac_context.shake;
}

eAcShake AcDeviceSetShakeState(eAcShake shake) {
  if ((shake > eACSHAKE_UNKONWN) && (shake < eACSHAKE_MAX)) {
    g_ac_context.shake = shake;
  }
  return g_ac_context.shake;
}

int AcDeviceInit(void) {
  g_ac_context.mode = eACMODE_NATURE;
  g_ac_context.shake = eACSHAKE_OFF;
  g_ac_context.speed = eAcWindSpeed_1;
  g_ac_context.state = eACSTATE_OFF;
  g_ac_context.timer = eACTIMER_NONE;
  return 0;
}

void AcDeviceFinal(void) {
  uni_memset(&g_ac_context, 0, sizeof(g_ac_context));
}

