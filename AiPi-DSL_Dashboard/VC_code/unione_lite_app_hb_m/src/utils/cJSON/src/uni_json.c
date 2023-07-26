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
 * Description : uni_json.c
 * Author      : chenxiaosong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_json.h"
#include "uni_iot.h"
#include "uni_log.h"

#define UNI_JSON_TAG "json"

typedef enum {
  JSON_VALUE_INT,
  JSON_VALUE_DOUBLE,
  JSON_VALUE_STRING
} JsonValueType;

static int _get_array_index(char *fmt) {
  char *start, *end;
  int num;
  start = uni_strstr(fmt, "[");
  if (NULL != start) {
    end = uni_strstr(fmt, "]");
    if (NULL != end) {
      *start = *end = '\0';
      start++;
      num = uni_atoi(start);
      if (num >= 0) {
        return num;
      }
    }
  }
  return -1;
}

static cJSON *_read_item_object(cJSON *root, const char *fmt) {
  cJSON *parent = root;
  cJSON *child = NULL;
  int fmt_size = uni_strlen(fmt) + 1;
  char *buf = (char *)uni_malloc(fmt_size);
  char *buf_p = buf;
  int offset = 0;
  int arr_index;
  int i = 0;
  if (NULL == buf) {
    LOGE(UNI_JSON_TAG, "malloc failed !");
    return NULL;
  }
  /* 1. split */
  uni_memcpy(buf, fmt, fmt_size);
  for (; i < fmt_size; i++) {
    if ('.' == buf[i]) {
      buf[i] = '\0';
    }
  }
  /* 2. parse */
  while (fmt_size > 0) {
    offset = uni_strlen(buf) + 1;
    arr_index = _get_array_index(buf);
    child = cJSON_GetObjectItem(parent, buf);
    if (NULL == child) {
      goto L_ERROR;
    }
    if (arr_index >= 0) {
      child = cJSON_GetArrayItem(child, arr_index);
      if (NULL == child) {
        goto L_ERROR;
      }
    }
    parent = child;
    buf = buf + offset;
    fmt_size -= offset;
  }

L_ERROR:
  uni_free(buf_p);
  return child;
}

static uni_bool _input_parameter_valid(cJSON *value, JsonValueType type) {
  if (value->valuestring != NULL) {
    if (JSON_VALUE_STRING != type) {
      LOGE(UNI_JSON_TAG, "json type invalid");
      return false;
    }
  } else if (value->valuedouble == value->valueint) {
    if (JSON_VALUE_STRING == type) {
      LOGE(UNI_JSON_TAG, "json type invalid");
      return false;
    }
  } else if (value->valuedouble != value->valueint) {
    if (JSON_VALUE_DOUBLE != type) {
      LOGE(UNI_JSON_TAG, "json type invalid");
      return false;
    }
  }
  return true;
}

static int _read_item_int(cJSON *root, const char *fmt, int *dst) {
  cJSON *obj;
  if (NULL == root || NULL == fmt) {
    LOGW(UNI_JSON_TAG, "invalid input root %p fmt %p", root, fmt);
    return -1;
  }
  obj = _read_item_object(root, fmt);
  if (NULL == obj ||
      false == _input_parameter_valid(obj, JSON_VALUE_INT)) {
    LOGD(UNI_JSON_TAG, "parse error, fmt=%s", fmt);
    return -1;
  }
  if (NULL != dst) {
    *dst = obj->valueint;
  }
  return 0;
}

static int _read_item_double(cJSON *root, const char *fmt, double *dst) {
  cJSON *obj;
  if (NULL == root || NULL == fmt) {
    LOGW(UNI_JSON_TAG, "invalid input root %p fmt %p", root, fmt);
    return -1;
  }
  obj = _read_item_object(root, fmt);
  if (NULL == obj ||
      false == _input_parameter_valid(obj, JSON_VALUE_DOUBLE)) {
    LOGD(UNI_JSON_TAG, "parse error, fmt=%s", fmt);
    return -1;
  }
  if (NULL != dst) {
    *dst = obj->valuedouble;
  }
  return 0;
}

static int _read_item_string(cJSON *root, const char *fmt, char **dst) {
  cJSON *obj;
  if (NULL == root || NULL == fmt) {
    LOGW(UNI_JSON_TAG, "invalid input root %p fmt %p", root, fmt);
    return -1;
  }
  obj = _read_item_object(root, fmt);
  if (NULL == obj ||
      false == _input_parameter_valid(obj, JSON_VALUE_STRING)) {
    LOGD(UNI_JSON_TAG, "parse error, fmt=%s", fmt);
    return -1;
  }
  if (NULL != dst) {
    if (NULL == (*dst = (char *)uni_malloc(uni_strlen(obj->valuestring) + 1))) {
      LOGE(UNI_JSON_TAG, "memory alloc failed");
      return -1;
    }
    uni_strcpy(*dst, obj->valuestring);
  }
  return 0;
}

int JsonReadItemInt(cJSON *root, const char *fmt, int *dst) {
  return _read_item_int(root, fmt, dst);
}

int JsonReadItemObject(cJSON *root, const char *fmt, cJSON **dst) {
  cJSON *obj;
  if (NULL == root || NULL == fmt) {
    LOGW(UNI_JSON_TAG, "invalid input root %p fmt %p", root, fmt);
    return -1;
  }
  obj = _read_item_object(root, fmt);
  if (NULL == obj) {
    LOGD(UNI_JSON_TAG, "parse error, fmt=%s", fmt);
    return -1;
  }
  if (NULL != dst) {
    *dst = cJSON_Duplicate(obj, 1);
    if (NULL == *dst) {
      LOGE(UNI_JSON_TAG, "memory alloc failed");
      return -1;
    }
  }
  return 0;
}

int JsonReadItemDouble(cJSON *root, const char *fmt, double *dst) {
  return _read_item_double(root, fmt, dst);
}

int JsonReadItemString(cJSON *root, const char *fmt, char **dst) {
  return _read_item_string(root, fmt, dst);
}

int JsonCheckItemInt(cJSON *root, const char *fmt) {
  return _read_item_int(root, fmt, NULL);
}

int JsonCheckItemDouble(cJSON *root, const char *fmt) {
  return _read_item_double(root, fmt, NULL);
}

int JsonCheckItemString(cJSON *root, const char *fmt) {
  return _read_item_string(root, fmt, NULL);
}
