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
 * Description : uni_auto_string.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.05.22
 *
 **************************************************************************/
#include "uni_auto_string.h"
#include "uni_string.h"

#define UNIT_BUFFER_SIZE 8

typedef struct {
  char*   str_buf;
  int     buf_size;
  int     str_len;
}AutoString;

STRING_HANDLE StringCreat(const char* ch){
  AutoString *p_str = NULL;
  int ch_len = 0;

  p_str = (AutoString *)uni_malloc(sizeof(AutoString));
  if (p_str) {
    if (ch) {
      ch_len = uni_strlen(ch);
      p_str->buf_size = ch_len + 1;
    }
    /* Buffer size must be greater than string len */
    p_str->buf_size = ((p_str->buf_size / UNIT_BUFFER_SIZE) + 1) * UNIT_BUFFER_SIZE;
    p_str->str_len = ch_len;
    p_str->str_buf = (char *)uni_malloc(p_str->buf_size);
    if (NULL == p_str->str_buf) {
      uni_free(p_str);
      p_str = NULL;
      goto L_ERROR;
    }
    uni_memset(p_str->str_buf, 0, p_str->buf_size);
    if (ch) {
      uni_strncpy(p_str->str_buf, ch, p_str->buf_size-1);
    }
  }
  L_ERROR:
  return (STRING_HANDLE)p_str;
}

void StringDestroy(STRING_HANDLE handle){
  AutoString *p_str = (AutoString *)handle;
  if (p_str) {
    uni_free(p_str);
    p_str = NULL;
  }
}

int StringAssignment(STRING_HANDLE handle, const char* ch){
  AutoString *p_str = (AutoString *)handle;
  int ch_len = 0;

  if (p_str) {
    if (ch) {
      ch_len = uni_strlen(ch);
      if ((ch_len + 1) > p_str->str_len) {
        /* relloc buffer for more char */
        p_str->str_len = ch_len;
        p_str->buf_size = (((ch_len + 1) / UNIT_BUFFER_SIZE) + 1) * UNIT_BUFFER_SIZE;
        if (p_str->str_buf) {
          /* free buffer if already */
          uni_free(p_str->str_buf);
          p_str->str_buf = NULL;
        }
        p_str->str_buf = (char *)uni_malloc(p_str->buf_size);
        if (p_str->str_buf) {
          uni_memset(p_str->str_buf, 0, p_str->buf_size);
          uni_strncpy(p_str->str_buf, ch, p_str->buf_size-1);
        } else {
          p_str->str_len = 0;
          p_str->buf_size = 0;
        }
      } else {
        /* recover old string */
        p_str->str_len = ch_len;
        uni_memset(p_str->str_buf, 0, p_str->buf_size);
        uni_strncpy(p_str->str_buf, ch, p_str->buf_size-1);
      }
    } else {
      /* clear string of input NULL */
      uni_memset(p_str->str_buf, 0, p_str->buf_size);
      p_str->str_len = 0;
    }
    return p_str->str_len;
  }
  return 0;
}

int SringGetLength(STRING_HANDLE handle){
  AutoString *p_str = (AutoString *)handle;

  if (p_str) {
    return p_str->str_len;
  }
  return 0;
}

char* SringGetString(STRING_HANDLE handle){
  AutoString *p_str = (AutoString *)handle;

  if (p_str) {
    return p_str->str_buf;
  }
  return NULL;
}

int SringCopy(STRING_HANDLE handle, char* buf, int size){
  AutoString *p_str = (AutoString *)handle;

  if (p_str) {
    if (buf) {
      uni_memset(buf, 0, size);
      uni_strncpy(buf, p_str->str_buf, size-1);
      return (p_str->str_len < (size-1)) ? p_str->str_len : (size-1);
    }
  }
  return 0;
}

int SringCmp(STRING_HANDLE handle, char* ch) {
  AutoString *p_str = (AutoString *)handle;

  if (p_str) {
    return uni_strncmp(p_str->str_buf, ch, p_str->str_len);
  }
  return -1;
}

