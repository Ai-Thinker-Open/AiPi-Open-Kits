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
 * Description : ac_device_simulator.h
 * Author      : liuwenzheng@unisound.com
 * Date        : 2019.12.30
 *
 **************************************************************************/


#ifndef APP_INC_AC_DEVICE_SIMULATOR_H_
#define APP_INC_AC_DEVICE_SIMULATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef enum {
  eACSTATE_UNKONWN = 0,
  eACSTATE_ON,
  eACSTATE_OFF,
  eACSTATE_MAX
}eAcSate;

typedef enum {
  eAcWindSpeed_UNKONWN = 0,
  eAcWindSpeed_1,
  eAcWindSpeed_2,
  eAcWindSpeed_3,
  eAcWindSpeed_MAX
}eAcWindSpeed;

typedef enum {
  eACMODE_UNKONWN = 0,
  eACMODE_NATURE,
  eACMODE_NORMAL,
  eACMODE_SLEEP,
  eACMODE_MAX
}eAcWindMode;

typedef enum {
  eACTIMER_UNKONWN = 0,
  eACTIMER_NONE,
  eACTIMER_1HR,
  eACTIMER_2HR,
  eACTIMER_4HR,
  eACTIMER_8HR,
  eACTIMER_MAX
}eAcTimer;

typedef enum {
  eACSHAKE_UNKONWN = 0,
  eACSHAKE_ON,
  eACSHAKE_OFF,
  eACSHAKE_MAX
}eAcShake;

eAcSate AcDeviceGetState(void);
eAcSate AcDeviceSetState(eAcSate state);
eAcWindSpeed AcDeviceGetWindSpeed(void);
eAcWindSpeed AcDeviceSetWindSpeed(eAcWindSpeed speed);
eAcWindSpeed AcDeviceWindSpeedInc(void);
eAcWindSpeed AcDeviceWindSpeedDec(void);
eAcWindMode AcDeviceGetWindMode(void);
eAcWindMode AcDeviceSetWindMode(eAcWindMode mode);
eAcTimer AcDeviceGetTimer(void);
eAcTimer AcDeviceSetTimer(eAcTimer hr);
eAcTimer AcDeviceTimerInc(void);
eAcTimer AcDeviceTimerDec(void);
eAcShake AcDeviceGetShakeState(void);
eAcShake AcDeviceSetShakeState(eAcShake shake);
int AcDeviceInit(void);
void AcDeviceFinal(void);

#ifdef __cplusplus
}
#endif

#endif /*APP_INC_AC_DEVICE_SIMULATOR_H_*/
