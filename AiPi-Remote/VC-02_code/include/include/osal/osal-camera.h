/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_CAMERA_H_
#define OSAL_OSAL_CAMERA_H_

#include <stdint.h>

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

typedef enum OsalCameraFormat {
  IMG_FMT_RGB332 = 1,
  IMG_FMT_RGB444,
  IMG_FMT_RGB555,
  IMG_FMT_RGB565,
  IMG_FMT_RGB555X,
  IMG_FMT_RGB565X,
  IMG_FMT_RGB888,
  IMG_FMT_BGR666,
  IMG_FMT_BGR24,
  IMG_FMT_RGB24,
  IMG_FMT_BGR32,
  IMG_FMT_RGB32,
  IMG_FMT_GREY = 20,
  IMG_FMT_Y4,
  IMG_FMT_Y6,
  IMG_FMT_Y10,
  IMG_FMT_Y12,
  IMG_FMT_Y16,
  IMG_FMT_Y10BPACK,
  IMG_FMT_PAL8,
  IMG_FMT_UV8,
  IMG_FMT_YVU410 = 100,
  IMG_FMT_YVU420,
  IMG_FMT_YUYV,
  IMG_FMT_YVYU,
  IMG_FMT_UYVY,
  IMG_FMT_VYUY,
  IMG_FMT_YUV422P,
  IMG_FMT_YUV411P,
  IMG_FMT_Y41P,
  IMG_FMT_YUV444,
  IMG_FMT_YUV555,
  IMG_FMT_YUV565,
  IMG_FMT_YUV32,
  IMG_FMT_YUV410,
  IMG_FMT_YUV420,
  IMG_FMT_HI240 = 200,
  IMG_FMT_HM12,
  IMG_FMT_M420,
  IMG_FMT_NV12,
  IMG_FMT_NV21,
  IMG_FMT_NV16,
  IMG_FMT_NV61,
  IMG_FMT_NV24,
  IMG_FMT_NV42,
  IMG_FMT_MJPEG = 300,
  IMG_FMT_JPEG,
  IMG_FMT_DV,
  IMG_FMT_MPEG,
  IMG_FMT_MAX
} OsalCameraFormat;

/* Camera handle */
typedef void* OsalCameraHandle;

/* Camera config */
typedef struct OsalCameraConfig {
  uint32_t width;
  uint32_t height;
  uint32_t fps;
  uint32_t pixel_format;  // 格式：YUV420/YUYV/JPEG
} OsalCameraConfig;

/*
 * @Description: Camera open
 * @Input params: config: 配置
 * @Output params: handle: 句柄
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
typedef int32_t (*OsalCameraOpen)(OsalCameraConfig* config,
                                  OsalCameraHandle* handle);

/*
 * @Description: 开始抓取图片
 * @Input params: handle: 句柄
 * @Output params:
 * @Return:
 */
typedef int32_t (*OsalCameraStartCapturing)(OsalCameraHandle handle);

/*
 * @Description: 读取一帧
 * @Input params: handle: video handle
 *                image: 图片缓冲区
 *                len: 缓冲长度
 * @Output params:
 * @Return:
 */
typedef int32_t (*OsalCameraReadImage)(OsalCameraHandle handle, uint8_t* image,
                                       uint32_t len);

/*
 * @Description: 停止抓取
 * @Input params: handle: video handle
 * @Output params:
 * @Return:
 */
typedef int32_t (*OsalCameraStopCapturing)(OsalCameraHandle handle);

/*
 * @Description: 关闭设备
 * @Input params: handle: video handle
 * @Output params:
 * @Return:
 */
typedef int32_t (*OsalCameraClose)(OsalCameraHandle handle);

typedef struct OsalCameraDev {
  OsalCameraOpen camera_open;
  OsalCameraStartCapturing camera_start;
  OsalCameraReadImage camera_read;
  OsalCameraStopCapturing camera_stop;
  OsalCameraClose camera_close;
} OsalCameraDev;

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_CAMERA_H_
