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

#include "uni_nlu.h"
#include "uni_hash.h"
#include "uni_string.h"
#include "uni_log.h"
#include "list_head.h"
#include "uni_nlu_content.h"
#include "uni_json.h"
#include "uni_watchdog_session.h"

#define WORD_BLOCKLIST_MAX  10
#define TAG "nlu"

static uni_u32 g_word_blacklist[WORD_BLOCKLIST_MAX] = {0};

int _find_key_word_in_blacklist(uni_u32 hashCode) {
  int i = 0;
  for (i = 0; i < WORD_BLOCKLIST_MAX; i++) {
    if (hashCode == g_word_blacklist[i]) {
      return i;
    }
  }
  return -1;
}

int NluParseBlacklist(const char *key_word, uni_bool is_append) {
  uni_u32 hashCode = uni_hash(key_word);
  int idx = _find_key_word_in_blacklist(hashCode);
  int i = 0;
  if (is_append) {
    if (idx < 0) {
      for (i = 0; i < WORD_BLOCKLIST_MAX; i++) {
        if (0 == g_word_blacklist[i]) {
          g_word_blacklist[i] = hashCode;
          break;
        }
      }
      if (WORD_BLOCKLIST_MAX == i) {
        /* blacklist was full */
        return -1;
      }
    }
  } else {
    if (idx >= 0) {
      g_word_blacklist[idx] = 0;
    }
  }
  return 0;
}

static void _replace_asr_recongize(cJSON *reslut, const char *word) {
  cJSON *rec_node = cJSON_CreateString(word);
  if (reslut) {
    if (cJSON_GetObjectItem(reslut, "asr")) {
      cJSON_ReplaceItemInObject(reslut, "asr", rec_node);
    } else {
      cJSON_AddItemToObject(reslut, "asr", rec_node);
    }
  }
}

//TODO perf sort g_nlu_content_mapping by hashcode O(logN), now version O(N)
cJSON* NluParseLasr(const char *key_word) {
  cJSON *result = NULL;
  uni_u32 hashCode = uni_hash(key_word);
  int hashTableSize = sizeof(g_nlu_content_mapping) / sizeof(g_nlu_content_mapping[0]);
  int i;
  /* blocklist filter */
  if (_find_key_word_in_blacklist(hashCode) >= 0) {
    LOGT(TAG, "%s was included in blacklist", key_word);
    return NULL;
  }
  for (i = 0; i < hashTableSize; i++) {
    /* find same hashcode as keyword's */
    if (hashCode == g_nlu_content_mapping[i].key_word_hash_code) {
      LOGT(TAG, "found map %d", i);
      /* return immediately when no hash collision */
      if (NULL == g_nlu_content_mapping[i].hash_collision_orginal_str) {
        LOGT(TAG, "found result %s", g_nlu_content_str[g_nlu_content_mapping[i].nlu_content_str_index]);
        result = cJSON_Parse(g_nlu_content_str[g_nlu_content_mapping[i].nlu_content_str_index]);
        if (result == NULL) {
          LOGE(TAG, "json parse failed, memory may not enough, reboot now");
          WatchDogReboot();
          return NULL;
        }
        _replace_asr_recongize(result, key_word);
        return result;
      }
      /* return when key_word equals hash_collision_orginal_str */
      if (0 == strcmp(key_word, g_nlu_content_mapping[i].hash_collision_orginal_str)) {
        LOGT(TAG, "found result %s", g_nlu_content_str[g_nlu_content_mapping[i].nlu_content_str_index]);
        result = cJSON_Parse(g_nlu_content_str[g_nlu_content_mapping[i].nlu_content_str_index]);
        if (result == NULL) {
          LOGE(TAG, "json parse failed, memory may not enough, reboot now");
          WatchDogReboot();
          return NULL;
        }
        _replace_asr_recongize(result, key_word);
        return result;
      }
    }
  }
  return NULL;
}

cJSON* NluParseRasr(const char *rasr_result) {
  return cJSON_Parse(rasr_result);
}

