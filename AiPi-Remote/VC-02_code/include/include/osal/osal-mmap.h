/*
 * Copyright 2021 Unisound AI Technology Co., Ltd.
 * Author: Sun zhangyang
 * All Rights Reserved.
 */

#ifndef OSAL_OSAL_MMAP_H_
#define OSAL_OSAL_MMAP_H_

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
 * @Description:映射初始化
 * @Input params: model_addr,模型在flash的起始地址
 *                model_addr,模型大小
 *                idma_dst_buf_len,一次搬运idma 目的buf的最大长度
 *                flash_read_func，flash读取函数定义，参考FLASH_READ
 *                wait_dma,  等待dma完成函数，定义见KWS_SCHEDULE
 *                wait_idma,  等待idma完成函数，定义见KWS_SCHEDULE
 *                clear_event，dma status 清除定义，定义见KWS_SCHEDULE
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */

OSAL_EXPORT int32_t OsalMMapInit(int32_t modle_addr, int32_t modle_len,
                                 int32_t idma_dst_buf_len,
                                 void* flash_read_func, void* flash_get_data,
                                 void* wait_dma, void* wait_idma,
                                 void* clear_event);

/*
 * @Description: flash数据读取函数（libumd使用）
 * @Input params: current_base,当前要读取数据在flash的地址
 *                current_len, 读取数据的长度
 *                next_base,   下次要读取数据在flash的地址
 *                next_len，   下次读取数据的长度
 *                is_new_matrix,
 * 1:表示一个新矩阵数据的开始，0：表示同一个矩阵内部数据搬运，
 *                               在一个新矩阵开始时，此参数设置为1，矩阵内部搬运设置0
 * @Output params: *data, idma目的地地址
 *                *data_len, 搬运数据长度
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
OSAL_EXPORT int32_t OsalMMapRead(int32_t current_base, int32_t current_len,
                                 int32_t next_base, int32_t next_len,
                                 void** data, int32_t* data_len,
                                 int8_t is_new_matrix);

/*
 * @Description: flash数据读取函数（kws初始化使用）
 * @Input params: addr,模型地址
 *                len,数据长度
 * @Output params: buffer,内存由kws负责管理
 * @Return:
 */
OSAL_EXPORT void OsalMmapGetRomData(intptr_t addr, uint32_t len,
                                    uint8_t* buffer);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_MMAP_H_
