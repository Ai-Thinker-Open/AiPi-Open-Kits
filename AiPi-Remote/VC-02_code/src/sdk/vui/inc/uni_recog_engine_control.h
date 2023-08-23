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
 * Description : Uniapp recog_engine_control.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.12.19
 *
 **************************************************************************/
#ifndef SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_ENGINE_CONTROL_H_
#define SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_ENGINE_CONTROL_H_

#include "uni_iot.h"
#include "uni_recog_common.h"
#include "uni_vui_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

VuiHandle EngineCreate(EventHandler recog_event_handler);
void      EngineDestroy(VuiHandle vui);
Result    EngineStart(VuiHandle vui, Scene *scene);
Result    EngineCancel(VuiHandle vui);
Result    EngineStop(VuiHandle vui);
void      EngineCbRegister(EngineCallback cb, EngineCbType cb_type);
Result    EngineEnableTimeout(VuiHandle vui, uni_bool is_enable);

#ifdef __cplusplus
}
#endif
#endif  //  SDK_VUI_VUI_SERVICE_INC_UNI_RECOG_ENGINE_CONTROL_H_
