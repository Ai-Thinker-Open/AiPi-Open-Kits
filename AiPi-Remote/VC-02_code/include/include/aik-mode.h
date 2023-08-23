/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef INCLUDE_AIK_MODE_H_
#define INCLUDE_AIK_MODE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * WARNING: 修改该文件后必须同步修改 AikMode.java,
 * 新增 mode 放最后，数值 +1, 废弃的 mode 不能删除，保证各个版本取值的一致性
 */
typedef enum AikMode {
  AIK_MODE_NULL = 0,
  /* LP 离线唤醒 + 识别 */
  AIK_MODE_KWS_LP = 1,
  /* STD 离线唤醒 + 识别 */
  AIK_MODE_KWS_STD = 2,
  /* 单 ASR 模式 */
  AIK_MODE_ASR = 3,
  /* LP + ASR 混合识别 */
  AIK_MODE_KWS_LP_ASR = 4,
  /* STD + ASR 混合识别 */
  AIK_MODE_KWS_STD_ASR = 5,
  /* 离线 TTS */
  AIK_MODE_TTS = 6,
  /* 在线 TTS */
  AIK_MODE_ONLINE_TTS = 7,
  /* 定向降噪 + VAD 测试模式 */
  AIK_MODE_DSSP_VADNN = 8,
  /* 降噪模式 */
  AIK_MODE_SSP = 9,
  /* 仅 AEC 模式 */
  AIK_MODE_SSP_AEC_ONLY = 10,
  /* 定向降噪模式 */
  AIK_MODE_DIRECTIONAL_SSP = 11,
  /* 单 ASR 模式, Android 使用 adapter */
  AIK_MODE_ASR_ANDROID = 12,
  /* SSP + KWS LP */
  AIK_MODE_SSP_KWS_LP = 13,

  /* 人脸检测模式，检测人脸数量 */
  AIK_MODE_FACE_DETECT = 201,
  /* 人脸识别模式，识别人脸 ID */
  AIK_MODE_FACE_RECOGNIZE = 202,
  /* 人脸注册模式，注册人脸特征 */
  AIK_MODE_FACE_REGISTER = 203,
  /* 唇动检测模式 */
  AIK_MODE_LIP = 204,
  /* 唇动检测 + 在线 ASR */
  AIK_MODE_LIPASR = 205,
  /* 唇动 + VADNN + 在线 ASR */
  AIK_MODE_LIPASR_VADNN = 206,
  /* 唇动 + VADNN + 在线 ASR Android 使用 adapter */
  AIK_MODE_LIPASR_VADNN_ANDROID = 207,
  /* VAD 触发 ASR 启动 */
  AIK_MODE_LIPASR_ASR = 208,
  /* 专门用于非智能断句模式下的 ASR stop */
  AIK_MODE_LIPASR_ASRVAD = 209,
  /* 唇动检测 + 在线 ASR Android 使用 adapter */
  AIK_MODE_LIPASR_ANDROID = 210,
  /* 接口测试模式，无引擎链路测试模式 */
  AIK_MODE_API_TEST = 9999,
  /* TODO: 持续更新新模式 */
  AIK_MODE_MAX
} AikMode;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_AIK_MODE_H_
