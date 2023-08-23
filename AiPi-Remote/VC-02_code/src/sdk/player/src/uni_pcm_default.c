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
 * Description : uni_pcm_default.c
 * Author      : wufangfang@unisound.com
 * Date        : 20120.02.28
 *
 **************************************************************************/
 
#include "uni_pcm_default.h"
#include "uni_iot.h"

#define LIST_COUNT_MAX  30

/*[1,2,3]*/
static char value_buf[8] = {0};
static int value_list[LIST_COUNT_MAX] = {0};

int _parse_pcm_list(const char *str_list) {
  int count = 0, sum = 0;
  char last_ch = 0;
  const char *p = str_list;
  if (!str_list || str_list[0] != '[') {
    return -1;
  }
  while (*p) {
    if (*p == '[') {
      last_ch = *p;
      p ++;
      continue;
    }
    if (*p == ',' || *p == ']') {
      if ((last_ch == '[') || (last_ch == ',')) {
        /* invalid style */
        sum = 0;
        last_ch = *p;
        if (*p == ']') {
          /* finished if ']' */
          break;
        }
        continue;
      }
      value_list[count ++] = sum;
      sum = 0;
      last_ch = *p;
      if (count >= LIST_COUNT_MAX) break;
      if (*p == ']') {
          /* finished if ']' */
          break;
      }
    } else if (*p >= '0' && *p <= '9') {
      sum = sum * 10 + *p - '0';
      last_ch = *p;
    } else if (*p != ' ' && *p != '\t') {
      /* failed style if there is other char */
      return -1;
    }
    p ++;
  }
  if (count <= 0) {
    return -1;
  }
  return count;
}

char *uni_get_number_pcm(const char *str_list, int num) {
  int count = 0;
  if (num < 0) {
    return "-1";
  }
  count = _parse_pcm_list(str_list);
  if (count <= 0 || count <= num) {
    return "-1";
  }
  uni_snprintf(value_buf, sizeof(value_buf), "%d", value_list[num]);
  return value_buf;
}

char *uni_get_random_pcm(const char *str_list) {
  int count = _parse_pcm_list(str_list);
  if (count <= 0) {
    return "-1";
  }
  uni_snprintf(value_buf, sizeof(value_buf), "%d", value_list[rand() % count]);
  return value_buf;
}

char *uni_get_random_pcm_or_default(const char *pcm_list,
                                             const char *default_pcm_list) {
  char *pcm = uni_get_random_pcm(pcm_list);
  if (!uni_strcmp(pcm, "-1")) {
    return uni_get_random_pcm(default_pcm_list);
  } else {
    return pcm;
  }
}