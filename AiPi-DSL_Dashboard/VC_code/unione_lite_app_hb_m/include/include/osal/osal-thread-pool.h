/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Liu Linghan
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_THREAD_POOL_H_
#define OSAL_OSAL_THREAD_POOL_H_

#include "osal/osal-mutex.h"
#include "osal/osal-sem.h"
#include "osal/osal-thread.h"
#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define POOL_STATUS_EXIT 0
#define POOL_STATUS_ALIVE 1

/* 线程运行函数 */
typedef void* (*OsalThreadFunction)(void* parameters);

/* 线程池内任务 */
typedef struct OsalThreadPoolJob {
  OsalThreadFunction job_function;
  void* parameters;
  struct OsalThreadPoolJob* previous_job;
  struct OsalThreadPoolJob* next_job;
} OsalThreadPoolJob;

/* 线程池内任务列表 */
typedef struct {
  OsalThreadPoolJob* head_job;
  OsalThreadPoolJob* tail_job;
  int32_t job_count;
  OsalSemHandle queue_semaphore;
} OsalThreadPoolJobQueue;

/* 线程池 */
typedef struct {
  volatile int32_t alive;
  OsalThreadHandle* threads;
  int32_t thread_number;
  OsalThreadPoolJobQueue* job_queue;
  OsalSemHandle thread_stop;
  OsalMutexHandle mutex;
  OsalSemHandle thread_start;
} OsalThreadPool;

/* 线程池相关资源 */
typedef struct {
  OsalMutexHandle data_mutex;
  OsalThreadPool* thread_pool;
} OsalThreadData;

/*
 * @Description: 线程池初始化
 * @Input params: thread_number: 线程池中所需的线程数量
 * @Output params: thread_pool: 线程池
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalThreadPoolInitialize(int32_t thread_number,
                                 OsalThreadPool** thread_pool);

/*
 * @Description: 线程池任务添加
 * @Input params: thread_pool: 线程池
 *                ThreadFunction: 线程运行函数
 *                function_parameters: 线程运行函数参数
 * @Output params:
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalThreadPoolAddJob(OsalThreadPool* thread_pool,
                             void* (*ThreadFunction)(void*),
                             void* function_parameters);

/*
 * @Description: 线程池释放
 * @Input params: thread_pool: 线程池
 * @Output params:
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalThreadPoolDestroy(OsalThreadPool* thread_pool);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_THREAD_POOL_H_
