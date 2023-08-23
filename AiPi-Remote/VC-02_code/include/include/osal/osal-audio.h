/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_AUDIO_H_
#define OSAL_OSAL_AUDIO_H_

#include "osal/osal-types.h"

#if defined(_WIN32) || defined(_WIN64)
#define _WINDOWS
#endif

#ifndef _WINDOWS
#ifdef OSAL_API_HIDDEN
#define OSAL_EXPORT __attribute__((visibility("hidden")))
#else
#define OSAL_EXPORT __attribute__((visibility("default")))
#endif

#else
#define OSAL_EXPORT __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* OSAL_AUDIO_TYPE_DEFINE */
#define OSAL_AUDIO_TYPE_PLAY 0 /* 不支持 */
#define OSAL_AUDIO_TYPE_RECORD 1

/* 音频设备句柄 */
typedef void* OsalAudioHandle;

/* 音频配置参数 */
typedef struct {
  uint32_t device_type;  /* 设备类型，OSAL_AUDIO_TYPE_DEFINE */
  uint32_t channels;     /* 通道数，OSAL_AUDIO_CHANNEL_DEFINE */
  uint32_t rate;         /* 速率，OSAL_AUDIO_RATE_DEFINE */
  uint32_t period_size;  /* 单次数据量，OSAL_AUDIO_PERIODSIZE_DEFINE */
  uint32_t period_count; /* 次数，OSAL_AUDIO_PERIODCOUNT_DEFINE */
  uint32_t bits;         /* Bit 位数，OSAL_AUDIO_BITS_DEFINE */
} OsalAudioConfig;

/*
 * @Description: 音频设备打开操作（含录音和播放两种）
 * @Input params: audio_config: Audio 配置参数，包含声道数，采样率，录音通道或
 *                放音通道等驱动必要信息
 * @Output params: 音频设备句柄
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalAudioOpen(OsalAudioConfig* audio_config,
                                  OsalAudioHandle* audio_handle);

/*
 * @Description: 关闭录音 / 放音设备
 * @Input params: audio_handle: 音频设备句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalAudioClose(OsalAudioHandle audio_handle);

/*
 * @Description: 从音频设备读取数据，用于录音
 * @Input params: audio_handle: 录音设备驱动句柄
 *                buf: 数据缓冲
 *                size: 数据大小
 * @Output params: 无
 * @Return: 成功：读到的录音字节数
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalAudioRead(OsalAudioHandle audio_handle, uint8_t* buf,
                                  uint32_t size);

/*
 * @Description: 开启音频（多用于开启录音）
 * @Input params: audio_handle: 句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalAudioStart(OsalAudioHandle audio_handle);

/*
 * @Description: 停止音频（多用于停止录音）
 * @Input params: audio_handle: 句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalAudioStop(OsalAudioHandle audio_handle);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_AUDIO_H_
