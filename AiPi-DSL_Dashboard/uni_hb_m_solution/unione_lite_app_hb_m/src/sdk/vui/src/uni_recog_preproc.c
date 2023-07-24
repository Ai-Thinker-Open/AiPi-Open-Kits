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
 * Description : Uniapp recog_preproc.c
 * Author      : chenxiaosong@unisound.com
 * Date        : 2017.12.27
 *
 **************************************************************************/
#include "uni_recog_preproc.h"
#include "uni_recog_common.h"
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_nlu.h"
#include "cJSON.h"
#include "uni_black_board.h"
#include "uni_lasr_result_parser.h"
#include "uni_string.h"
#include "uni_arpt.h"
#include "uni_float2string.h"
#include "uni_config.h"

#define INVALID_TYPE                  (-1)
#define TWICE_JUDGE_TIME_INTERVAL     (5)

#define DEEP_SLEEP_TIME_THRESHOLD_SEC (600)

#define RECOG_PREPROC_TAG         "recog_preproc"

typedef struct {
  uni_u32 last_asr_result_check_valid_time_in_sec;
} RecogPreprocParam;

static RecogPreprocParam g_recog_preproc;

static void _write_arpt_log(int asr_mode, char *command, float score,
                            char *json, uni_bool is_basic, uni_bool is_rasr) {
  char buffer_log[256] = {0};
  char tag[16] = {0};
  char result[16] = {0};
  char kws[8] = {0};
  char pure_cmd[64] = {0};
  uni_s32 is_wakeup = 0;
  uni_bool is_more = !(is_basic || is_rasr);
  /* basic content */
  BbRead(BB_KEY_IS_WAKEUP, &is_wakeup);
  uni_snprintf(tag, sizeof(tag), "%s_%s", is_wakeup ? "asr" : "wakeup",
               "normal");
  uni_snprintf(kws, sizeof(kws), "%s", is_basic ? "KWS\t" : "");
  uni_snprintf(result, sizeof(result), "%s", is_rasr ? "online_json" : "offline_result");
  if (is_rasr) {
    uni_snprintf(buffer_log, sizeof(buffer_log), "%s%s:[%s]\t", kws, result, tag);
  } else {
    //StrSubEx(pure_cmd, command, ">", "</");
    uni_snprintf(pure_cmd, sizeof(pure_cmd), "%s", command);
    uni_snprintf(buffer_log, sizeof(buffer_log), "%s%s:[%s]\tcommand[%s]\tscore[%s]\t",
                 kws, result, tag, pure_cmd, PFloat2String(&score));
  }
  ArptWrite(buffer_log, is_more);
  /* more content */
  if (!is_more) {
    ArptWrite(json, false);
  }
  ArptWrite("\n", false);
}

/**
 * Usage:   To analyze the result from ASR
 * Params:  info[in]        event to be handled
 * Return:  Result of analyzation
 *          (1) E_OK:       event is expected
 *          (2) E_FAILED:   event is not expected
 */
static Result _lasr_check_score(char *command, float score, float std_thresh,
                                float low_thresh) {
  static char last_failed_command[CMD_LENGTH] = {0};
  static uni_bool twice_judge_enable = false;
  static uni_u32 last_ts = 0;
  uni_u32 ts;
  int abs_sec;
  if (score >= std_thresh) {
    twice_judge_enable = false;
    return E_OK;
  }
  if (score < low_thresh) {
    twice_judge_enable = false;
    return E_FAILED;
  }
  if (twice_judge_enable) {
    ts = uni_get_clock_time_sec();
    abs_sec = ts - last_ts;
    if ((abs_sec < TWICE_JUDGE_TIME_INTERVAL) &&
         (0 == uni_strncmp(last_failed_command, command, CMD_LENGTH))) {
      twice_judge_enable = false;
      LOGT(RECOG_PREPROC_TAG, "[hit] %s,score=%s\n", command,
                                                     PFloat2String(&score));
      return E_OK;
    }
  }
  twice_judge_enable = true;
  uni_snprintf(last_failed_command, CMD_LENGTH, "%s", command);
  last_ts = uni_get_clock_time_sec();
  return E_FAILED;
}

static uni_s32 _assign_nlu_type(cJSON *jnlu) {
  char *cmd = NULL;
  uni_s32 type = INVALID_TYPE;
  type = ID(VUI_APP_SETTING_EVENT);
  if (0 != JsonReadItemString(jnlu, "cmd", &cmd)) {
    goto L_END;
  }
  if (0 == uni_strcmp(cmd, "volumeUpUni")
      || 0 == uni_strcmp(cmd, "volumeDownUni")
      || 0 == uni_strcmp(cmd, "volumeMinUni")
      || 0 == uni_strcmp(cmd, "volumeMaxUni")
      || 0 == uni_strcmp(cmd, "volumeMidUni")) {
    type = ID(VUI_APP_VOLUME_SETTING_EVENT);
    goto L_END;
  }
  if (uni_strstr(cmd, "wakeup_uni") != NULL) {
    type = ID(VUI_APP_WAKEUP_EVENT);
    goto L_END;

  }
  if (uni_strstr(cmd, "exitUni") != NULL) {
    type = ID(VUI_APP_SLEEP_EVENT);
    goto L_END;
  }
L_END:
  if (cmd) {
    uni_free(cmd);
  }
  return type;
}

static uni_lasr_result_t *_creat_result_context(cJSON *jnlu) {
  uni_lasr_result_t *result = (uni_lasr_result_t *)uni_malloc(sizeof(uni_lasr_result_t));
  if (result) {
    JsonReadItemString(jnlu, "asr", &(result->asr));
    JsonReadItemString(jnlu, "cmd", &(result->cmd));
    JsonReadItemString(jnlu, "pcm", &(result->pcm));
  }
  return result;
}

static void _nlu_event_free_handler(EventContent *event_content) {
  uni_lasr_result_t *result = NULL;
  if (NULL != event_content->info) {
    result = (uni_lasr_result_t *)event_content->info;
    if (result->asr) {
      uni_free(result->asr);
    }
    if (result->cmd) {
      uni_free(result->cmd);
    }
    if (result->pcm) {
      uni_free(result->pcm);
    }
    uni_free(result);
  }
  if (NULL != event_content->extend_info) {
    uni_free(event_content->extend_info);
  }
}

static void _deep_sleep_asr_result_valid_timestamp_init() {
  g_recog_preproc.last_asr_result_check_valid_time_in_sec = \
    uni_get_clock_time_sec();
  LOGT(RECOG_PREPROC_TAG, "init deep sleep timestamp=%u",
       g_recog_preproc.last_asr_result_check_valid_time_in_sec);
}

static void _set_asr_result_check_valid_timestamp() {
  g_recog_preproc.last_asr_result_check_valid_time_in_sec = \
    uni_get_clock_time_sec();
  LOGT(RECOG_PREPROC_TAG, "set current lasr success time=%u",
       g_recog_preproc.last_asr_result_check_valid_time_in_sec);
}

static uni_bool _is_step_into_deep_sleep() {
  uni_u32 now = uni_get_clock_time_sec();
  return (now - g_recog_preproc.last_asr_result_check_valid_time_in_sec >= \
          DEEP_SLEEP_TIME_THRESHOLD_SEC);
}

static void _get_deep_sleep_thresold(float *std_thresh,
                                     float *low_thresh) {
  double tmp = 0.0;
  *std_thresh = DEEP_SLEEP_WAKEUP_SCENE_STD_THRESHOLD;
  *low_thresh = DEEP_SLEEP_WAKEUP_SCENE_LOW_THRESHOLD;
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.sleep_std_threshold", &tmp)) {
    *std_thresh = tmp;
  }
  if (0 == ConfigReadItemDouble("lasr.lasr_wkp.sleep_lp_threshold", &tmp)) {
    *low_thresh = tmp;
  }
}

static Result _lasr_result_preproc(Event *event, float std_thresh,
                                   float low_thresh) {
  char command[CMD_LENGTH] = {0};
  float score = -20;
  uni_s32 event_type = INVALID_TYPE;
  cJSON *jnlu;
  char *jnlu_str;
  if (event->content.info) {
    LOGT(RECOG_PREPROC_TAG, "Asr Result[L]: %s", event->content.info);
  }
  /* 1. parse lasr result to command and score */
  if (E_OK != LasrResultParse((char *)event->content.info, command, &score)) {
    LOGE(RECOG_PREPROC_TAG, "lasr result parse failed");
    return E_FAILED;
  }
  /* 2. parse command to get nlu */
  if (NULL == (jnlu = NluParseLasr(command))) {
    return E_FAILED;
  }
  /* 3. check deep sleep mode, then get special scene thresold */
  if (_is_step_into_deep_sleep()) {
    LOGT(RECOG_PREPROC_TAG, "step into deep sleep scene mode");
    _get_deep_sleep_thresold(&std_thresh, &low_thresh);
  } else {
    LOGT(RECOG_PREPROC_TAG, "normal scene mode");
  }
  /* 4. replace event type and content when score valid */
  LOGT(RECOG_PREPROC_TAG, "score=%f, std_thresh=%f, low_thresh=%f", score, std_thresh, low_thresh);
  if (E_OK == _lasr_check_score(command, score, std_thresh, low_thresh)) {
    event_type = _assign_nlu_type(jnlu);
  }
  jnlu_str = cJSON_PrintUnformatted(jnlu);
  if (INVALID_TYPE == event_type) {
    _write_arpt_log(UNI_LP_WAKEUP, command, score, jnlu_str, false, false);
    cJSON_Delete(jnlu);
    uni_free(jnlu_str);
    return E_FAILED;
  }
  _write_arpt_log(UNI_RASR_LASR_MODE, command, score, jnlu_str, true, false);
  uni_free(jnlu_str);
  if (NULL != event->content.info) {
    uni_free(event->content.info);
  }
  event->type         = event_type;
  event->content.info = (void *)_creat_result_context(jnlu);
  event->free_handler = _nlu_event_free_handler;
  cJSON_Delete(jnlu);
  return E_OK;
}

Result RecogPreproc(Event *event, uni_s32 asr_mode, float std_thresh,
                    float low_thresh, VuiHandle vui) {
  Result ret = E_OK;
  if (event->type == ID(VUI_LOCAL_ASR_SELF_RESULT_EVENT)) {
    ret = _lasr_result_preproc(event, std_thresh, low_thresh);
  }else if (event->type == ID(VUI_LOCAL_ASR_TIMEOUT_EVENT) ||
             event->type == ID(VUI_LOCAL_ASR_FAIL_EVENT)) {
  } else {
    LOGW(RECOG_PREPROC_TAG, "unsupport event type[%d].", event->type);
    ret = E_FAILED;
  }
  if (E_OK != ret) {
    LOGT(RECOG_PREPROC_TAG, "free invalid event.");
    if (event->free_handler) {
      event->free_handler(&event->content);
    }
  } else {
    _set_asr_result_check_valid_timestamp();
  }
  return ret;
}

Result RecogPreprocInit() {
  _deep_sleep_asr_result_valid_timestamp_init();
  return E_OK;
}

void RecogPreprocFinal() {}
