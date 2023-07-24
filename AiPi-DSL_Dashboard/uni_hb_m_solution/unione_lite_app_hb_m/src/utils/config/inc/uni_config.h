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
 * Description : uni_config.h
 * Author      : wufangfang@unisound.com
 * Date        : 2019.01.06
 *
 **************************************************************************/
#ifndef UTILS_CONFIG_INC_UNI_CONFIG_H_
#define UTILS_CONFIG_INC_UNI_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"

typedef enum {
  CONFIG_NONE = 0,
  CONFIG_RECORD,
  CONFIG_HVAD
}config_type;

#pragma pack(push)
#pragma pack(4)           // 4 byte alignment
/* items of config, must under than 4 */
typedef struct {
  uni_u32   enable;
  uni_u32   asr_only;
}config_record_t;

typedef struct {
  uni_u32   eage_det;
  uni_u32   threshold;
  uni_u32   trig_num;
}config_hvad_t;

/* all config struct can union same buffer */
typedef union {
  config_record_t record;
  config_hvad_t   hvad;
}config_items_u;

/* mailbox message data 0 - 4 */
typedef struct {
  uni_u32         type;
  config_items_u  items;
}config_context_t;
#pragma pack(pop)

int ConfigInitialize(void);
int ConfigFinalize(void);
int ConfigReadItemNumber(const char *fmt, uni_s64 *number);
int ConfigReadItemDouble(const char *fmt, double *number);
int ConfigReadItemString(const char *fmt, char *dst, int len);
int ConfigReadItemNumberArray(const char *fmt, uni_s64 *number, int cnt);
int ConfigReadItemDoubleArray(const char *fmt, double *number, int cnt);
int ConfigReadItemstringArray(const char *fmt, char **dst, int len);

#ifdef __cplusplus
}
#endif
#endif
