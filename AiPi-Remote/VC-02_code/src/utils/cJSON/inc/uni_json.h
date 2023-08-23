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
 * Description : uni_json.h
 * Author      : chenxiaosong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_CJSON_INC_UNI_JSON_H_
#define UTILS_CJSON_INC_UNI_JSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
#include "cJSON.h"

#define JSON_KEY_LEN_MAX      (64)
#define CJSON_OBJECT_NAME(a)  #a

int JsonReadItemInt(cJSON *root, const char *fmt, int *dst);
int JsonReadItemDouble(cJSON *root, const char *fmt, double *dst);
int JsonReadItemString(cJSON *root, const char *fmt, char **dst);
int JsonReadItemObject(cJSON *root, const char *fmt, cJSON **dst);

int JsonCheckItemInt(cJSON *root, const char *fmt);
int JsonCheckItemDouble(cJSON *root, const char *fmt);
int JsonCheckItemString(cJSON *root, const char *fmt);

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_CJSON_INC_UNI_JSON_H_
