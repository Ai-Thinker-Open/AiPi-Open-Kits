/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Li Dapeng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_WAV_H_
#define OSAL_OSAL_WAV_H_

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
#ifdef DLL_EXPORT
#define OSAL_EXPORT __declspec(dllexport)
#else
#define OSAL_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* OsalWavHandle;

/* 音频信息 */
typedef struct {
  uint32_t wav_header_length;
  uint16_t channels;
  uint32_t sample_rate;
  uint32_t bytes_per_second;
  uint16_t bytes_per_sample;
  uint16_t bits_per_sample;
} OsalWavHeaderInfo;

/*
 * @Description: 获取音频wav头长度
 * @Input params: fpath: 音频文件路径
 * @Output params: 无
 * @Return: 成功: wav头长度
 *          失败: OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalWavGetHeaderLength(const char* file_path);

/*
 * @Description: 获取音频wav头信息
 * @Input params: fpath: 音频文件路径
 * @Output params: wav_header_info：音频wav头信息
 * @Return: 成功: OSAL_OK
 *          失败: OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalWavGetInfo(const char* file_path,
                                   OsalWavHeaderInfo* wav_header_info);

/*
 * @Description: 给pcm数据流添加wav头部
 * @Input params: buf: 数据缓冲
 *                size: 数据大小
 *                sample_rate: 采样率
 *                channels：通道数
 * @Output params: out_wav_path: 输出wav文件路径
 * @Return: 成功: OSAL_OK
 *          失败: OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalWavAddHeader(const uint8_t* buf, uint32_t lenth,
                                     uint32_t sample_rate, uint16_t channels,
                                     const char* out_wav_path);

/*
 * @Description: open wave file
 * @Input params: path: the path to save wave file
 *                channels: channales of wave file
 *                rate: rate of wave file
 * @Output params: NONE
 * @Return: success：wave file handle
 *          fail：NULL
 */
OSAL_EXPORT OsalWavHandle OsalWavOpen(const char* path, const int32_t channels,
                                      const int32_t rate);

/*
 * @Description: write wave file
 * @Input params: handle: handle of wave file
 *                data: audio buffer to write
 *                length: length of audio to write
 * @Output params: NONE
 * @Return: success：length wroten
 *          fail：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalWavWrite(OsalWavHandle wav_handle, const void* data,
                                 const int32_t length);

/*
 * @Description: close wave file
 * @Input params: handle: handle of wave file
 * @Output params: NONE
 * @Return: success：OSAL_OK
 *          fail：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalWavClose(OsalWavHandle wav_handle);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_WAV_H_
