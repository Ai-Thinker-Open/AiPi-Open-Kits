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
 * Description : uni_vui_interface.h
 * Author      : yzs.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef SDK_VUI_VUI_BASIC_INC_UNI_VUI_INTERFACE_H_
#define SDK_VUI_VUI_BASIC_INC_UNI_VUI_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "uni_event.h"

typedef enum {
  UNI_LP_WAKEUP,
  UNI_LP_LASR,
  UNI_RASR_MODE,
  UNI_RASR_LASR_MODE
} VuiRecognModel;

typedef void* VuiHandle;

VuiHandle VuiHandleCreate(EventHandler vui_event_handler);
Result    VuiHandleDestroy(VuiHandle handle);
Result    VuiRecognStart(VuiHandle handle, VuiRecognModel model, uni_u32 timeout);
Result    VuiRecognStop(VuiHandle handle);
Result    VuiRecognCancel(VuiHandle handle);
uni_u32   VuiRecognGetSessionId(VuiHandle handle);
Result    VuiRecognEnableTimeout(VuiHandle handle, uni_bool is_enable);

#ifdef __cplusplus
}
#endif
#endif
