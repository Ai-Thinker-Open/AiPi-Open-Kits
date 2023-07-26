/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef INCLUDE_AIK_ID_H_
#define INCLUDE_AIK_ID_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* WARNING: 修改该文件后必须同步修改 AikId.java! */
/*
 * AIK type id 格式定义
 * 32          24         16                        0
 * |<--------->|<--------->|<---------------------->|
 *   MAIN_TYPE    SUB_TYPE        COMMAND  ID
 */

/* List of main type */
#define AIK_MAIN_ENGINE ((uint8_t)0x10)
#define AIK_MAIN_DEV ((uint8_t)0x20)

/* List of sub type */
#define AIK_SUBTYPE_START ((uint8_t)0x01)
#define AIK_SUBTYPE_SSP ((uint8_t)0x01)
#define AIK_SUBTYPE_LIP ((uint8_t)0x02)
#define AIK_SUBTYPE_FACE_DETECT ((uint8_t)0x03)
#define AIK_SUBTYPE_FACE_RECOGNIZE ((uint8_t)0x04)
#define AIK_SUBTYPE_FACE_REGISTER ((uint8_t)0x05)
#define AIK_SUBTYPE_KWS_LP ((uint8_t)0x06)
#define AIK_SUBTYPE_KWS_STD ((uint8_t)0x07)
#define AIK_SUBTYPE_ASR ((uint8_t)0x08)
#define AIK_SUBTYPE_AUDIO_STREAM ((uint8_t)0x09)
#define AIK_SUBTYPE_IMAGE_STREAM ((uint8_t)0x0A)
#define AIK_SUBTYPE_TTS ((uint8_t)0x0B)
#define AIK_SUBTYPE_ONLINE_TTS ((uint8_t)0x0C)
#define AIK_SUBTYPE_DSSP ((uint8_t)0x0D)
#define AIK_SUBTYPE_VADNN ((uint8_t)0x0E)
#define AIK_SUBTYPE_END ((uint8_t)0x0E)

#define AIK_GET_MODULE_BY_ID(id) (((id)&0x00FF0000) >> 16)

/* ID of Audio stream */
#define AIK_ID_AUDIO_STREAM \
  (((AIK_MAIN_DEV & 0xFF) << 24) | (AIK_SUBTYPE_AUDIO_STREAM & 0xFF) << 16)

#define AIK_ID_AUDIO_STREAM_RESET_INDEX (AIK_ID_AUDIO_STREAM | (uint16_t)0x01)
#define AIK_ID_AUDIO_STREAM_GET_MONITOR_COUNT \
  (AIK_ID_AUDIO_STREAM | (uint16_t)0x06)

/* ID of LIP */
#define AIK_ID_LIP \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_LIP & 0xFF) << 16))
#define AIK_ID_LIP_SET_FRAMES (AIK_ID_LIP | (uint16_t)0x01)
#define AIK_ID_LIP_GET_FRAMES (AIK_ID_LIP | (uint16_t)0x02)

/* ID of SSP */
#define AIK_ID_SSP \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_SSP & 0xFF) << 16))
/* 0: 关闭， 1：打开 */
#define AIK_ID_SSP_AEC_ONLY (AIK_ID_SSP | (uint16_t)0x01)
#define AIK_ID_SSP_CACULATE_DOA (AIK_ID_SSP | (uint16_t)0x02)

/* ID of Directional SSP */
#define AIK_ID_DIRECTIONAL_SSP \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_DSSP & 0xFF) << 16))
/* 角度值，必须是5的倍数 */
#define AIK_ID_DIRECTIONAL_SSP_SET_ANGLE \
  (AIK_ID_DIRECTIONAL_SSP | (uint16_t)0x01)
#define AIK_ID_DIRECTIONAL_SSP_GET_ANGLE \
  (AIK_ID_DIRECTIONAL_SSP | (uint16_t)0x02)

/* ID of TTS */
#define AIK_ID_TTS \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_TTS & 0xFF) << 16))
#define AIK_ID_TTS_SET_TEXT (AIK_ID_TTS | (uint16_t)0x0001)
#define AIK_ID_TTS_GET_DATA (AIK_ID_TTS | (uint16_t)0x0002)

/* ID of Online TTS */
#define AIK_ID_ONLINE_TTS \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_ONLINE_TTS & 0xFF) << 16))
#define AIK_ID_ONLINE_TTS_SET_TEXT (AIK_ID_ONLINE_TTS | (uint16_t)0x0001)
#define AIK_ID_ONLINE_TTS_SET_IP (AIK_ID_ONLINE_TTS | (uint16_t)0x0002)
#define AIK_ID_ONLINE_TTS_SET_PORT (AIK_ID_ONLINE_TTS | (uint16_t)0x0003)
#define AIK_ID_ONLINE_TTS_GET_DATA (AIK_ID_ONLINE_TTS | (uint16_t)0x0101)

/* ID of KWS */
#define AIK_ID_KWS_LP \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_KWS_LP & 0xFF) << 16))
#define AIK_ID_KWS_LP_SET_WAKEUP (AIK_ID_KWS_LP | (uint16_t)0x0001)
#define AIK_ID_KWS_LP_SET_COMMAND (AIK_ID_KWS_LP | (uint16_t)0x0002)
#define AIK_ID_KWS_LP_GET_START_TIME (AIK_ID_KWS_LP | (uint16_t)0x0003)
#define AIK_ID_KWS_LP_GET_STOP_TIME (AIK_ID_KWS_LP | (uint16_t)0x0004)
#define AIK_ID_KWS_LP_RESET (AIK_ID_KWS_LP | (uint16_t)0x0005)
#define AIK_ID_KWS_LP_GET_MONITOR_COUNT (AIK_ID_KWS_LP | (uint16_t)0x0006)
#define AIK_ID_KWS_LP_SET_MODEL (AIK_ID_KWS_LP | (uint16_t)0x0007)
#define AIK_ID_KWS_LP_SET_GRAMMAR (AIK_ID_KWS_LP | (uint16_t)0x0008)
#define AIK_ID_KWS_LP_SET_SAVE_PATH (AIK_ID_KWS_LP | (uint16_t)0x0009)

#define AIK_ID_KWS_STD \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_KWS_STD & 0xFF) << 16))
#define AIK_ID_KWS_STD_SET_WAKEUP (AIK_ID_KWS_STD | (uint16_t)0x0001)
#define AIK_ID_KWS_STD_SET_COMMAND (AIK_ID_KWS_STD | (uint16_t)0x0002)

/* ID of ASR */
#define AIK_ID_ASR \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_ASR & 0xFF) << 16))
#define AIK_ID_ASR_SET_SERVER_PARAM (AIK_ID_ASR | (uint16_t)0x0001)
#define AIK_ID_ASR_GET_START_TIME (AIK_ID_ASR | (uint16_t)0x0002)
#define AIK_ID_ASR_GET_STOP_TIME (AIK_ID_ASR | (uint16_t)0x0003)

/* ID of VADNN */
#define AIK_ID_VADNN \
  (((AIK_MAIN_ENGINE & 0xFF) << 24) | ((AIK_SUBTYPE_VADNN & 0xFF) << 16))
#define AIK_ID_VADNN_GET_START_TIME (AIK_ID_VADNN | (uint16_t)0x0001)
#define AIK_ID_VADNN_GET_END_TIME (AIK_ID_VADNN | (uint16_t)0x0002)

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_AIK_ID_H_
