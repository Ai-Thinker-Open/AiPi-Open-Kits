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
 * Description : uni_string.c
 * Author      : chenxiaosong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_string.h"

void StrStrip(char *str) {
  char *start, *end;
  if (NULL == str) {
    return;
  }
  start = str;
  end = str + uni_strlen(str) -1;
  while(*start && isspace(*(unsigned char *)start)) {
    start++;
  }
  while(*end && isspace(*(unsigned char *)end)) {
    *end-- = 0;
  }
  while ((*str++ = *start++) != '\0');
}

static int _str_sub_internal(char *dst, const char *str, const char *start,
                      const char *end, uni_bool exclude_end) {
  int s_pos, e_pos;
  if (NULL == dst || (*dst = '\0') || NULL == str || NULL == start ||
      NULL == end || uni_strlen(start) == 0 || uni_strlen(end) == 0) {
    return -1;
  }
  /* check start point */
  if (0 == uni_strcmp(start, "^")) {
    s_pos = 0;
  } else {
    const char *temp = uni_strstr(str, start);
    if (NULL == temp) {
      return -1;
    }
    if (exclude_end) {
      s_pos = temp + uni_strlen(start) - str;
    } else {
      s_pos = temp - str;
    }
  }
  if (0 == uni_strcmp(end, "$")) {
    e_pos = uni_strlen(str);
  } else {
    const char *temp = uni_strstr(str, end);
    if (NULL == temp) {
      return -1;
    }
    if (exclude_end) {
      e_pos = temp - str;
    } else {
      e_pos = temp + uni_strlen(end) - str;
    }
  }
  if (e_pos < s_pos) {
    return -1;
  }
  uni_strncpy(dst, str + s_pos, (e_pos - s_pos));
  dst[e_pos - s_pos] = '\0';
  return 0;
}

void StrSub(char *dst, const char *str, const char *start, const char *end) {
  _str_sub_internal(dst, str, start, end, false);
}

void StrSubEx(char *dst, const char *str, const char *start, const char *end) {
  _str_sub_internal(dst, str, start, end, true);
}

int StrSplit(char *dst[], char *str, const char *sep) {
  int arr_size = 0;
  char *outptr = NULL;
  char *s = strtok_r(str, sep, &outptr);
  while (s != NULL) {
    arr_size++;
    *dst++ = s;
    s = strtok_r(NULL, sep, &outptr);
  }
  return arr_size;
}

int StrLoc(const char *str, const char *substr) {
  const char *p;
  if (str == NULL || substr == NULL) {
    return -1;
  }
  p = uni_strstr(str, substr);
  if (p == NULL) {
    return -1;
  }
  return (p - str);
}

#if 1
static int _str_replace_first(char *str, const char *from, const char *to) {
  char *str_dup;
  char *p, *q;
  int str_len, from_len, to_len;
  str_len = uni_strlen(str);
  from_len = uni_strlen(from);
  to_len = uni_strlen(to);
  if (str_len == 0 || from_len == 0) {
    return -1;
  }
  str_dup = strdup(str);
  p = uni_strstr(str_dup, from);
  if (p == NULL) {
    goto L_END;
  }
  q = str + (p - str_dup);
  uni_strcpy(q, to);
  q = q + to_len;
  uni_strcpy(q, p + from_len);
L_END:
  uni_free(str_dup);
  return 0;
}

void StrReplaceS(char *str, const char *from, const char *to) {
  char *p = str;
  int to_len;
  if (str == NULL || from == NULL || to == NULL) {
    return;
  }
  to_len = strlen(to);
  while (p && (p = uni_strstr(p, from)) != NULL) {
    if (0 != _str_replace_first(p, from, to)) {
      break;
    }
    p = p + to_len;
  }
}
#endif

char* StrReplace(char *src, const char *from, const char *to) {
  int len_new = 0, len_from = 0, len_to = 0;
  char *orig = src;
  char *new_str = NULL;
  char *tmp = NULL, *pos = NULL, *dst_pos = NULL;
  int count = 0;
  if (!src || !from || !to) {
    return NULL;
  }
  len_to = uni_strlen(to);
  len_from = uni_strlen(from);
  tmp = orig;
  for (count = 0; (pos = strstr(tmp, from)); count++){
    tmp = pos + len_from;
  }
  len_new = strlen(src) + (len_to - len_from) * count + 1;
  if (NULL == (new_str = uni_malloc(len_new))) {
    return NULL;
  }
  memset(new_str, 0, len_new);
  dst_pos = new_str;
  tmp = orig;
  while(count--) {
    pos = strstr(tmp, from);
    dst_pos = (char *)memcpy(dst_pos, tmp, pos - tmp) + (pos - tmp);
    dst_pos = strcpy(dst_pos, to) + len_to;
    tmp = pos + len_from;
  }
  strcpy(dst_pos, tmp);
  return new_str;
}


void StrReplaceC(char *str, char from, char to) {
  if (str == NULL) {
    return;
  }
  while (*str) {
    if (*str == from) {
      *str = to;
    }
    str++;
  }
}

void StrDeleteC(char *str, char target) {
  char *loop, *new;
  if (str == NULL) {
    return;
  }
  new = loop = str;
  while (*loop) {
    if (*loop >> 7) {
      memcpy(new, loop, 3);
      loop += 3;
      new += 3;
    } else if (target == *loop) {
      loop ++;
    } else {
      *new = *loop;
      loop ++;
      new ++;
    }
  }
  *new = '\0';
}
