/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_THREAD_H_
#define OSAL_OSAL_THREAD_H_

#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 线程名长度 */
#define OSAL_THREAD_TASKNAME_LENGTH 32

/* 线程 / 任务优先级 */
#define OSAL_THREAD_PRIORITY_LOWEST 1
#define OSAL_THREAD_PRIORITY_LOW 2
#define OSAL_THREAD_PRIORITY_MEDIUM 4
#define OSAL_THREAD_PRIORITY_HIGH 5
#define OSAL_THREAD_PRIORITY_REALTIME 6

/* 线程 / 任务栈大小*/
#define OSAL_THREAD_STACK_SIZE_1K 1024

/* 线程参数结构 */
typedef struct {
  int32_t stack_size;                          /* 线程栈大小 */
  int32_t priority;                            /* 线程优先级 */
  char task_name[OSAL_THREAD_TASKNAME_LENGTH]; /* 线程名 */
} OsalThreadParam;

typedef enum {
  kThreadNone = 0,
  kThreadFpuEnable,
  kThreadPriority,
  kThreadEnd
} OsalThreadType;

/* pthread 线程库句柄 */
typedef void* OsalThreadHandle;

/*
 * @Description: 线程 / 任务创建
 * @Input params: thread: OsalThread 类型，接口内分配内存
 *                param: OsalThreadParam 类型，线程属性参数
 *                start_routine： 线程入口函数
 *                args: 线程入参
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 * @Notice: handle 为 void*，不支持下面写法
 *          { OsalThreadHandle handle;
 *            OsalThreadCreate(&handle, NULL, start_routine, handle/&handle);}
 *          args 实参不能为 void 或 void*类型
 */
int32_t OsalThreadCreate(OsalThreadHandle* thread, OsalThreadParam* param,
                         void* (*start_routine)(void*), void* args);

/*
 * @Description: 线程 / 任务退出时自动回收资源 (RTOS 不一定支持）
 * @Input params: thread: 线程句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalThreadDetach(OsalThreadHandle thread);

/*
 * @Description: 线程 / 任务退出
 * @Input params: thread: 线程句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalThreadExit(OsalThreadHandle thread);

/*
 * @Description: 配置线程属性
 * @Input params: thread: 线程句柄
 *                type: OsalThreadType
 *                args: 配置参数
 * @Output params:
 * @Return: 无
 */
void OsalThreadSet(OsalThreadHandle thread, OsalThreadType type, void* args);

/*
 * @Description: 获取线程属性
 * @Input params: thread: 线程句柄
 *                type: OsalThreadType
 * @Output params: args: 配置参数
 * @Return: 无
 */
void OsalThreadGet(OsalThreadHandle thread, OsalThreadType type, void* args);

/*
 * @Description: 延迟
 * @Input params: millisecond：毫秒
 * @Output params:
 * @Return:
 */
void OsalThreadDelayMs(uint32_t millisecond);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_THREAD_H_
