/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef INCLUDE_AIK_EVENT_H_
#define INCLUDE_AIK_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* WARNING: 修改该文件后必须同步修改 jni/AikEvent.java! */
typedef enum AikEvent {
  AIK_EVENT_NONE = 0,
  /* 离线唤醒结果 */
  AIK_EVENT_KWS_WAKEUP = 1,
  /* 离线识别结果 */
  AIK_EVENT_KWS_COMMAND = 2,
  /* 离线识别超时 */
  AIK_EVENT_KWS_TIMEOUT = 3,
  /* VAD start */
  AIK_EVENT_VAD_START = 4,
  /* VAD end */
  AIK_EVENT_VAD_END = 5,
  /* 在线语音识别结果 */
  AIK_EVENT_ASR_RESULT = 6,
  /* 在线语音 VAD 触发的识别结果 */
  AIK_EVENT_ASR_VAD_RESULT = 7,
  /* 无 VAD 触发的识别结果 */
  AIK_EVENT_NO_ASR_VAD_RESULT = 8,
  /* 在线识别网络失败 */
  AIK_EVENT_ASR_NETWORK_ERROR = 9,
  /* 在线识别超时 */
  AIK_EVENT_ASR_TIMEOUT = 10,
  /* Datacallback 数据类型为录音数据 */
  AIK_EVENT_AUDIO_DATA = 11,
  /* Heartbeat */
  AIK_EVENT_HEARTBEAT = 12,

  /* 人脸检测结果 */
  AIK_EVENT_FACE_DETECT = 201,
  /* 人脸识别结果 */
  AIK_EVENT_FACE_RECOGNIZE = 202,
  /* 人脸识别结果 */
  AIK_EVENT_FACE_REGISTER = 203,
  /* 人脸数量错误 */
  AIK_EVENT_FACE_NUM_ERROR = 204,
  /* 人脸位置偏离异常 */
  AIK_EVENT_FACE_ASKEW_ERROR = 205,
  /* 人脸比例太小 */
  AIK_EVENT_FACE_SMALL_ERROR = 206,
  /* 人脸出现  */
  AIK_EVENT_LIP_FACE_IN = 207,
  /* 人脸消失 */
  AIK_EVENT_LIP_FACE_OUT = 208,
  /* 人脸比例太小 */
  AIK_EVENT_LIP_FACE_SMALL_ERROR = 209,
  /* 人脸位置歪斜 */
  AIK_EVENT_LIP_FACE_ASKEW_ERROR = 210,
  /* 人脸出框异常 */
  AIK_EVENT_LIP_FACE_OUT_REGION_ERROR = 211,
  /* 唇动嘴唇张开  */
  AIK_EVENT_LIP_OPEN = 212,
  /* 唇动嘴唇闭合 */
  AIK_EVENT_LIP_CLOSE = 213,
  /* 唇动嘴唇状态抖动异常 */
  AIK_EVENT_LIP_MOVING = 214,
  /* 唇动超时 */
  AIK_EVENT_LIP_TIMEOUT = 215,
  /* END */
  AIK_EVENT_END
} AikEvent;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_AIK_EVENT_H_
