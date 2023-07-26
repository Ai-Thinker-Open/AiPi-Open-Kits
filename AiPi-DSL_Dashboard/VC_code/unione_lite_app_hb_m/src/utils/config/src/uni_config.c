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
 * Description : uni_config.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_config.h"
#include "cJSON.h"
#include "config_bin.h"

#define CONFIG_ITEM_NAME_LEN_MAX  (64)
typedef enum {
  CONFIG_VALUE_NUMBER,
  CONFIG_VALUE_DOUBLE,
  CONFIG_VALUE_STRING,
  CONFIG_VALUE_ARRAY,
} ConfigValueType;

typedef struct {
  cJSON         *config;
  uni_sem_t     sem;
}uni_config_context;

static uni_config_context g_context = {0};

static char* _load_config_data(void) {
  char *cjson_str = NULL;
  cjson_str = (char *)uni_malloc(sizeof(config_bin) + 1);
  if (NULL == cjson_str) {
    printf("malloc failed!\r\n");
    return NULL;
  }
  uni_memset(cjson_str, 0, sizeof(config_bin) + 1);
  uni_memcpy(cjson_str, config_bin, sizeof(config_bin));
  return cjson_str;
}

static cJSON *_config_load(void) {
  char *global = NULL;
  cJSON *config = NULL;

  global = _load_config_data();
  if (NULL != global) {
    config = cJSON_Parse(global);
    uni_free(global);
  }
  return config;
}

static cJSON *_get_read_item_object(const char *fmt) {
  int i = 0;
  cJSON *child = g_context.config;
  char buf[CONFIG_ITEM_NAME_LEN_MAX];
  if (NULL == g_context.config) {
    printf("config file was not parsed seccussfuly!\r\n");
    return NULL;
  }
  while((buf[i] = *fmt++) != '\0') {
    if (buf[i] == '.') {
      buf[i] = '\0';
      i = 0;
      child = cJSON_GetObjectItem(child, buf);
      if (NULL == child) {
        printf("cannot found: %s\r\n", fmt);
        return NULL;
      }
    } else {
      i++;
    }
  }
  return cJSON_GetObjectItem(child, buf);
}

static const char *_config_value_type_string(ConfigValueType type) {
  switch (type) {
    case CONFIG_VALUE_NUMBER: return "uni_s64";
    case CONFIG_VALUE_DOUBLE: return "double";
    case CONFIG_VALUE_STRING: return "string";
    case CONFIG_VALUE_ARRAY: return "array";
  }
  return "null";
}

static int _customer_input_parameter_valid(cJSON *value, ConfigValueType type) {
  if (CONFIG_VALUE_ARRAY == type) {

  } else if (value->valuestring != NULL) {
    if (CONFIG_VALUE_STRING != type) {
      printf("%s%d: In parameter type invalid. actual type string, "
             "input type %s.\r\n",
              __FUNCTION__, __LINE__, _config_value_type_string(type));
      return 0;
    }
  } else if (value->valuedouble == value->valueint) {
    if (CONFIG_VALUE_STRING == type) {
      printf("%s%d: In parameter type invalid. actual type uni_s64 or double, "
             "input type string.\r\n",
              __FUNCTION__, __LINE__);
      return 0;
    }
  } else if (value->valuedouble != value->valueint) {
    if (CONFIG_VALUE_DOUBLE != type) {
      printf("%s%d: In parameter type invalid. actual type double, "
             "input type %s.\r\n",
              __FUNCTION__, __LINE__, _config_value_type_string(type));
      return 0;
    }
  }
  return 1;
}

int ConfigReadItemNumber(const char *fmt, uni_s64 *number) {
  cJSON *value = NULL;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_NUMBER)) {
    return -1;
  }
  *number = value->valueint;
  return 0;
}

int ConfigReadItemDouble(const char *fmt, double *number) {
  cJSON *value = NULL;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_DOUBLE)) {
    return -1;
  }
  *number = value->valuedouble;
  return 0;
}

int ConfigReadItemString(const char *fmt, char *dst, int len) {
  cJSON *value = NULL;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_STRING)) {
    return -1;
  }
  if (len < (int)uni_strlen(value->valuestring)) {
    return -1;
  }
  uni_strcpy(dst, value->valuestring);
  return 0;
}

int ConfigReadItemNumberArray(const char *fmt, uni_s64 *number, int cnt) {
  cJSON *value = NULL;
  int size, i;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_ARRAY)) {
    return -1;
  }
  size = cJSON_GetArraySize(value);
  for (i = 0; i < size && i < cnt; i++) {
    number[i] = cJSON_GetArrayItem(value, i)->valueint;
  }
  return 0;
}

int ConfigReadItemDoubleArray(const char *fmt, double *number, int cnt) {
  cJSON *value = NULL;
  int size, i;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_ARRAY)) {
    return -1;
  }
  size = cJSON_GetArraySize(value);
  for (i = 0; i < size && i < cnt; i++) {
    number[i] = cJSON_GetArrayItem(value, i)->valuedouble;
  }
  return 0;
}

int ConfigReadItemstringArray(const char *fmt, char **dst, int len) {
  cJSON *value = NULL;
  int size, i;
  value = _get_read_item_object(fmt);
  if (NULL == value) {
    printf("cannot found %s\r\n", fmt);
    return -1;
  }
  if (!_customer_input_parameter_valid(value, CONFIG_VALUE_ARRAY)) {
    return -1;
  }
  size = cJSON_GetArraySize(value);
  uni_memset(dst, 0, sizeof(char *) * len);
  for (i = 0; i < uni_min(size, len); i++) {
    dst[i] = uni_malloc(uni_strlen(cJSON_GetArrayItem(value, i)->valuestring) +
                                   1);
    uni_strcpy(dst[i], cJSON_GetArrayItem(value, i)->valuestring);
  }
  return 0;
}

int ConfigInitialize(void) {
  uni_sem_init(&g_context.sem, 0);
  g_context.config = _config_load();
  if (NULL == g_context.config) {
    printf("_config_load failed, use default configrations !\r\n");
    return 0;
  }
  return 0;
}

int ConfigFinalize(void) {
  uni_sem_destroy(g_context.sem);
  if (g_context.config) {
    cJSON_Delete(g_context.config);
  }
  return 0;
}

