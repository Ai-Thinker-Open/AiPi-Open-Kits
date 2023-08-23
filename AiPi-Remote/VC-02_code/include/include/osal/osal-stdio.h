/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_STDIO_H_
#define OSAL_OSAL_STDIO_H_

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

/* 文件权限 */
#define OSAL_CREAT (0x1 << 0)
#define OSAL_RDONLY (0x1 << 1)
#define OSAL_WRONLY (0x1 << 2)
#define OSAL_RDWR (0x1 << 3)
#define OSAL_TRUNC (0x1 << 4)
#define OSAL_APPEND (0x1 << 5)
#define OSAL_BINARY (0x1 << 6)

#define OSAL_SEEK_SET 1
#define OSAL_SEEK_END 2
#define OSAL_SEEK_CUR 3

/*
 * @Description: 同 printf, 供 Log 模块使用
 * @Input params: 略
 * @Output params: 略
 * @Return: 略
 */
OSAL_EXPORT int32_t OsalPrintf(const char* format, ...);

/*
 * @Description: 同 fprintf
 * @Input params: 略
 * @Output params: 略
 * @Return: 略
 */
OSAL_EXPORT int32_t OsalFprintf(void* stream, const char* format, ...);

/*
 * @Description: 同 sprintf
 * @Input params: 略
 * @Output params: 略
 * @Return: 略
 */
OSAL_EXPORT int32_t OsalSprintf(char* str, const char* format, ...);

/*
 * @Description: 同 snprintf
 * @Input params: 略
 * @Output params: 略
 * @Return: 略
 */
OSAL_EXPORT int32_t OsalSnprintf(char* str, size_t size, const char* format,
                                 ...);

/*
 * @Description: 同 vprintf
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalVprintf(const char* format, va_list ap);

/*
 * @Description: 同 open
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalOpen(const char* path_name, int32_t flags, ...);

/*
 * @Description: 同 write
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT osal_ssize_t OsalWrite(int32_t fd, const void* buf, size_t count);

/*
 * @Description: 同 read
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT osal_ssize_t OsalRead(int32_t fd, void* buf, size_t count);

/*
 * @Description: 同 cloes
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalClose(int32_t fd);

/*
 * @Description: 同 lseek
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT osal_off_t OsalSeek(int32_t fd, osal_off_t offset, int32_t whence);

/*
 * @Description: 同 remove
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalRemove(const char* path_name);

/*
 * @Description: 同 sync
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalSync(int32_t fd);

/*
 * @Description: 读取指定路径文件内容到 buf, 使用后 free
 * @Input params: file_path: 文件路径
 * @Output params: buf: 文件内容
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalReadFileToBuffer(const char* file_path, char** buf);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_STDIO_H_
