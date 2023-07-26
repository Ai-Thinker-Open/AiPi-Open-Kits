/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Li Dapeng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_RING_BUFFER_H_
#define OSAL_OSAL_RING_BUFFER_H_

#include "osal/osal-mutex.h"
#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OSAL_RING_BUFFER_LOCK_NEED 0
#define OSAL_RING_BUFFER_LOCK_NOT_NEED 1

typedef struct {
  uint8_t* buf;
  uint32_t size;
  uint32_t head;
  uint32_t tail;
  uint32_t thread_lock;
  OsalMutexHandle mutex;
} OsalRingBuffer;

/*
 * @Description: 创建环形缓冲
 * @Input params: size：缓冲区大小
 *                thread_lock：是否需要线程锁
 * @Output params：无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILDE
 */
int32_t OsalRingBufferCreate(OsalRingBuffer** ring_buffer, uint32_t size,
                             uint32_t thread_lock);

/*
 * @Description: 销毁环形缓冲
 * @Input params: ring_buffer：环形缓冲
 * @Output params：无
 * @Return：成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferDestroy(OsalRingBuffer* ring_buffer);

/*
 * @Description: 清空环形缓冲
 * @Input params: ring_buffer：环形缓冲
 * @Output params：无
 * @Return：成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferClear(OsalRingBuffer* ring_buffer);

/*
 * @Description: 获取环形缓冲空闲空间大小
 * @Input params: ring_buffer：环形缓冲
 * @Output params：无
 * @Return: 成功：空闲空间大小
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferGetFreeSize(OsalRingBuffer* ring_buffer);

/*
 * @Description: 获取环形缓冲数据空间大小
 * @Input params: ring_buffer：环形缓冲
 * @Output params：无
 * @Return: 成功：数据空间大小
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferGetDataSize(OsalRingBuffer* ring_buffer);

/*
 * @Description: 往环形缓冲写数据
 * @Input params: ring_buffer：环形缓冲
 *                buf：数据缓冲
 *                write_size：写数据大小
 * @Output params：无
 * @Return: 成功：写入的字节数
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferWrite(OsalRingBuffer* ring_buffer, const uint8_t* buf,
                            uint32_t write_size);
/*
 * @Description: 从环形缓冲读数据
 * @Input params: ring_buffer：环形缓冲
 *                buf：数据缓冲
 *                read_size：读数据大小
 * @Output params:无
 * @Return: 成功：读到的字节数
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferRead(OsalRingBuffer* ring_buffer, uint8_t* buf,
                           uint32_t read_size);

/*
 * @Description: 从环形缓冲读数据,不更新读指针位置
 * @Input params: ring_buffer：环形缓冲
 *                buf：数据缓冲
 *                read_size：读数据大小
 * @Output params：无
 * @Return: 成功：读到的字节数
 *          失败：OSAL_FAILED
 */
int32_t OsalRingBufferPeek(OsalRingBuffer* ring_buffer, uint8_t* buf,
                           uint32_t read_size);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_RING_BUFFER_H_
