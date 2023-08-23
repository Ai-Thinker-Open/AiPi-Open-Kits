/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_LOG_H_
#define OSAL_OSAL_LOG_H_

#include "osal/osal-mutex.h"
#include "osal/osal-queue.h"
#include "osal/osal-sem.h"
#include "osal/osal-thread.h"
#include "osal/osal-time.h"
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

#if defined(__GNUC__)
#define __FUNC__ ((const char*)(__PRETTY_FUNCTION__))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
#define __FUNC__ ((const char*)(__func__))
#else
#define __FUNC__ ((const char*)(__FUNCTION__))
#endif

/* TODO：测试无队列模式 Log 时打开，配置项要加入到 CMake 中 */
// #define OSAL_LOG_WITH_QUEUE

/*
 * 单个节点打印信息量约 100-150 字节，针对内存资源紧张的
 * 平台，队列节点数暂定 30，约占 4K 左右
 */
#define OSAL_LOG_QUEUE_SIZE 30

/* log 线程栈设置为 4K */
#define OSAL_LOG_STACK_SIZE (4 * 1024)
#define OSAL_LOG_TASK_NAME "osal-log"
#define OSAL_LOG_WAIT_MS 20
#define OSAL_LOG_LEN 256
#define OSAL_LOG_MAX_LEN 2048
#define OSAL_LOG_TRUNCATED_MSG "...<message truncated>"

/* log 模块状态 */
#define OSAL_LOG_STATUS_IDLE 0
#define OSAL_LOG_STATUS_RUNNING 1
#define OSAL_LOG_STATUS_EXIT 2

/* log 输出格式 */
#define OSAL_LOG_FORMAT "%s %s %s %s[%d]: "

/* log 等级定义 */
typedef enum OsalLogLevel {
  LOG_VERBOSE = 1,
  LOG_DEBUG = 2,
  LOG_INFO = 3,
  LOG_WARNING = 4,
  LOG_ERROR = 5,
  LOG_NONE = 6, /* 6:log off */
  LOG_MAX
} OsalLogLevel;

/* log 句柄 */
typedef struct OsalLogHandle {
  uint32_t level; /* 日志过滤等级 */
#ifdef OSAL_LOG_WITH_QUEUE
  uint32_t status;         /* 状态 */
  OsalQueue* queue;        /* 收发日志队列 */
  OsalThreadHandle thread; /* 写日志线程 */
  OsalSemHandle sem;       /* 同步信号 */
#endif
#ifdef OSAL_LOG_MUTEX
  OsalMutexHandle mutex;
#endif
} OsalLogHandle;

/* log 信息 */
typedef struct OsalLogInfo {
  const char* level;       /* 日志等级 */
  const char* module;      /* 模块名 */
  const char* function;    /* 函数名 */
  uint32_t line;           /* 行号 */
  OsalTimeStamp timestamp; /* 时间戳 */
  char* content;           /* 打印信息，最长 2k */
} OsalLogInfo;

/*
 * @Description: Log 模块初始化
 * @Input params:
 * @Output params:
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalLogInit(void);

/*
 * @Description: Log 模块释放资源
 * @Input params:
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalLogRelease(void);

/*
 * @Description: 设置 Log 打印等级
 * @Input params: level：等级
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalLogSetLevel(uint32_t level);

/*
 * @Description: 合成一条完整 log, 并发送队列
 * @Input params: level：OsalLogLevel
 *                module：模块名
 *                function：函数名
 *                line：行号
 *                format：标准格式
 * @Output params:
 * @Return:
 */
OSAL_EXPORT void OsalLogDump(OsalLogLevel level, const char* module,
                             const char* function, uint32_t line,
                             const char* format, ...);

/*
 * 为了避免在 release 产品中 log 相关代码增加 binary
 * 的大小以及增加执行时间，通过预编译选项来控制除 LOG_ERROR 级别外的 log
 * 代码的编译。
 */
#ifdef OSAL_LOG_ENABLED

/*
 * @Description: VERBOSE 等级日志
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LOG_VERBOSE(module, format, args...)                          \
  do {                                                                     \
    OsalLogDump(LOG_VERBOSE, #module, __FUNC__, __LINE__, format, ##args); \
  } while (0)

/*
 * @Description: DEBUG 等级日志
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LOG_DEBUG(module, format, args...)                          \
  do {                                                                   \
    OsalLogDump(LOG_DEBUG, #module, __FUNC__, __LINE__, format, ##args); \
  } while (0)

/*
 * @Description: INFO 等级日志
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LOG_INFO(module, format, args...)                          \
  do {                                                                  \
    OsalLogDump(LOG_INFO, #module, __FUNC__, __LINE__, format, ##args); \
  } while (0)

/*
 * @Description: WARNING 等级日志
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LOG_WARNING(module, format, args...)                          \
  do {                                                                     \
    OsalLogDump(LOG_WARNING, #module, __FUNC__, __LINE__, format, ##args); \
  } while (0)

#else  // OSAL_LOG_ENABLED

#define OSAL_LOG_VERBOSE(module, format, args...)
#define OSAL_LOG_DEBUG(module, format, args...)
#define OSAL_LOG_INFO(module, format, args...)
#define OSAL_LOG_WARNING(module, format, args...)

#endif  // OSAL_LOG_ENABLED

/*
 * @Description: ERROR 等级日志
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LOG_ERROR(module, format, args...)                          \
  do {                                                                   \
    OsalLogDump(LOG_ERROR, #module, __FUNC__, __LINE__, format, ##args); \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_LOG_H_
