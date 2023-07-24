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
 * Description : Uniapp uni_nlu.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2018.6.8
 *
 **************************************************************************/

#include "uni_lasr_result_parser.h"
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_float2string.h"

#define LASR_PARSE_TAG     "lasr_parse"
#define IS_NUMBER(c)       (c >= '0' && c <= '9')
#define IS_ENGLISH_CHAR(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IS_SPACE(c)        (c == 0x20)
#define IS_CHINESE(c)      (!IS_NUMBER(c) && !IS_ENGLISH_CHAR(c) && !IS_SPACE(c))

/* 适用于UTF-8编码格式 */
static void _filter(unsigned char *command) {
  int idx = 0;
  unsigned char c;
  unsigned char *p = command;
  while ((c = *p) != '\0') {
    /* 去除中文与中文之间的空格 */
    if (!(IS_SPACE(c) && (IS_CHINESE(*(p - 1)) && IS_CHINESE(*(p + 1))))) {
      command[idx++] = c;
    }
    p++;
  }
  command[idx] = '\0';
}

Result LasrResultParse(char *content, char *command, float *score) {
  int i = 0;
  char *p = content;
  char ss[16] = {0};
  char c = 0;
  if (NULL == content || NULL == command || NULL == score) {
    return E_FAILED;
  }
  /* lasr result string format: "%d <s> %s </s> \n%f\n" */
  /* skip <s> */
  p = uni_strstr(content, "<s>");
  if (NULL == p) {
    LOGE(LASR_PARSE_TAG, "content: %s", command);
    return E_FAILED;
  }
  p += 3;   // skip <s>
  /* get context with not speace */
  while ((c = *p++) != '<') {       // p point to /
    if (c != '\t') {
      assert(i < CMD_LENGTH);
      command[i++] = c;
    }
  }

  /* remove space in command[0] and command[i - 1] */
  assert(i < CMD_LENGTH);
  command[i - 1] = '\0';
  command[i] = '\0';
  memmove(&command[0], &command[1], i);

  /* support english, must remove space between chinese word, en not */
  _filter((unsigned char*)command);

  /* get score */
  p += 3;   // skip </s>
  while ((' ' == *p) || ('\n' == *p) || ('\t' == *p)) {
    p++;
  }
  if ('\0' == *p) {
    LOGE(LASR_PARSE_TAG, "content: %s", command);
    return E_FAILED;
  }
  /* skip timestamp if there it is */
  i = 0;
  while ('\0' != *p) {
    if ((' ' == *p) || ('\n' == *p) || ('\t' == *p)) {
      break;
    }
    ss[i++] = *p++;
    if (i >= (sizeof(ss) - 1)) {
      break;
    }
  }
  ss[i] = '\0';
  LOGT(LASR_PARSE_TAG, "score: %s", ss);
  *score = uni_strtof(ss, NULL);
  LOGT(LASR_PARSE_TAG, "command=%s, score=%s", command, Float2String(*score));
  return E_OK;
}
