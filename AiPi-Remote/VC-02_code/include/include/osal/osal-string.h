/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_STRING_H_
#define OSAL_OSAL_STRING_H_

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

/*
 * @Description: 同 memset
 * @Input params: mem：内存首地址
 *                value：赋值
 *                len：长度
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void* OsalMemset(void* mem, int32_t value, size_t len);

/*
 * @Description: 同 memcpy
 * @Input params: dest: 目的内存首地址
 *                src: 源内存首地址
 *                n: 拷贝字节数
 * @Output params: 无
 * @Return: 指向 dest 的指针
 */
OSAL_EXPORT void* OsalMemcpy(void* dest, const void* src, size_t n);

/*
 * @Description: 同 memmove
 * @Input params: dest: 目的内存首地址
 *                src: 源内存首地址
 *                n: 移动字节数
 * @Output params: 无
 * @Return: 指向 dest 的指针
 */
OSAL_EXPORT void* OsalMemmove(void* dest, const void* src, size_t n);

/*
 * @Description: 同 strcpy
 * @Input params: dest: 目标内存首地址
 *                src: 源内存首地址
 * @Output params: 无
 * @Return: 指向 dest 的指针
 */
OSAL_EXPORT char* OsalStrcpy(char* dest, const char* src);

/*
 * @Description: 同 strncpy
 * @Input params: dest: 目标内存首地址
 *                src: 源内存首地址
 *                n: 字节数
 * @Output params: 无
 * @Return: 指向 dest 的指针
 */
OSAL_EXPORT char* OsalStrncpy(char* dest, const char* src, size_t n);

/*
 * @Description: 同 strstr
 * @Input params: haystack: 被检索字符串
 *                needle: 被匹配的字符串
 * @Output params: 无
 * @Return: 成功：子串首地址
 *          失败：NULL
 */
OSAL_EXPORT char* OsalStrstr(const char* haystack, const char* needle);

/*
 * @Description: 同 strchr
 * @Input params: str: 被检索字符串
 *                c：被匹配字符
 * @Output params: 无
 * @Return: 成功：匹配到 c 的位置的指针
 *          失败：NULL
 */
OSAL_EXPORT char* OsalStrchr(const char* str, int32_t c);

/*
 * @Description: 同 strrchr
 * @Input params: str: 被检索字符串
 *                c：被匹配字符
 * @Output params: 无
 * @Return: 成功：匹配到 c 的位置的指针
 *          失败：NULL
 */
OSAL_EXPORT char* OsalStrrchr(const char* str, int32_t c);

/*
 * @Description: 同 strlen
 * @Input params: str: 字符串
 * @Output params: 无
 * @Return: 字符串长度
 */
OSAL_EXPORT size_t OsalStrlen(const char* str);

/*
 * @Description: 同 strcmp
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalStrcmp(const char* s1, const char* s2);

/*
 * @Description: 同 strncmp
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalStrncmp(const char* s1, const char* s2, size_t n);

/*
 * @Description: 同 memcmp
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT int32_t OsalMemcmp(const void* dest, const void* src, size_t n);

/*
 * @Description: 同 strcat
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT char* OsalStrcat(char* dest, const char* src);

/*
 * @Description: 同 strncat
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT char* OsalStrncat(char* dest, const char* src, size_t n);

/*
 * @Description: 同 strtok
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT char* OsalStrtok(char* str, const char* delim);

/*
 * @Description: 同 strtok_r
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT char* OsalStrtok_r(char* str, const char* delim, char** saveptr);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_STRING_H_
