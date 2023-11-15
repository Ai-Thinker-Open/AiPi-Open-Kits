
/** @brief Wrong dock file
 *
 *  @file        aiio_errcode.h
 *  @copyright   Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note
 *  @par         Change Logs:
 *  <table>
 *  <tr><th>Date                <th>Version         <th>Author          <th>Notes
 *  <tr><th>2023/02/06          <td>V1.0.0          <td>hewm            Defines the basic type of error code
 *  <table>
 *
 */

#ifndef __AIIO_ERRCODE_H__
#define __AIIO_ERRCODE_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief aios returns a status value redefinition
 */
typedef int aiio_ret_t;

/**
 * @brief Error Code Related
 */
typedef enum
{
    AIIO_SUCC = 0x00,
    AIIO_ERR_INVALID_PARA = 0x01,
    AIIO_ERR_NO_MEM = 0x02,
    AIIO_ERR_TIMEOUT = 0x03,
    AIIO_ERR_INVALID_TYPE = 0x04,
    AIIO_ERR_INVALID_CMD = 0x05,
    AIIO_ERR_INVALID_EVT = 0x06,
    AIIO_ERR_INVALID_FCS = 0x07,
    AIIO_ERR_BUSY = 0x08,
    AIIO_ERR_ERASE = 0x09,
    AIIO_ERR_WRITE = 0x0A,
    AIIO_ERR_READ = 0x0B,
    AIIO_ERR_INIT_FAIL = 0x0C,
    AIIO_ERR_TEST_FAIL = 0x0D,
    AIIO_ERR_INIT_DEFAULT = 0x0F,
    AIIO_ERR_TABLE_FULL = 0x10,
    AIIO_ERR_TABLE_EXISTED = 0x11,
    AIIO_ERR_TABLE_NOT_FOUND = 0x12,
    AIIO_ERR_PARA_RESTORE = 0x13,
    AIIO_ERR_PARA_SAVE = 0x14,
    AIIO_ERR_QUEUE_FULL = 0x15,
    AIIO_ERR_INVALID_STATE = 0x16,
    AIIO_ERR_OPEN = 0x17,
    AIIO_ERR_ENCAIOSPTION = 0x18,
    AIIO_ERR_DECAIOSPTION = 0x19,
    AIIO_ERR_TIMESTAMP = 0x20,
    AIIO_ERR_NOT_CONNECTED = 0x21,
    AIIO_ERR_CONTORL = 0x22,
    AIIO_ERR_THREAD_CREATION = 0x23,
    AIIO_ERR_TIMER = 0x24,
    AIIO_ERR_SEMAPHORE = 0x25,
    AIIO_ERR_MUTEX = 0x26,
    AIIO_ERR_QUEUE = 0x27,
    AIIO_ERR_EVENT_TIMEOUT = 0x28,
    AIIO_ERR_ENCRYPTION = 0x29,
    AIIO_ERR_DECRYPTION = 0x2A,
    AIIO_ERR_NOT_SUPPORT = 0x2B,
    AIIO_ERR_RAM_HEAP_INIT = 0x2C,
    AIIO_ERR_CONSUMED = 0x2D,
    AIIO_ERR_MAX = 0xFF,
} AIIO_ERR_E;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__AIIO_ERRCODE_H__
