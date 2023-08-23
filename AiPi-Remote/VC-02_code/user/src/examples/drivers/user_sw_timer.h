/**************************************************************************
 * Copyright (C) 2020-2020  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : user_sw_timer.h
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#ifndef USER_INC_USER_SW_TIMER_H_
#define USER_INC_USER_SW_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_timer.h"

#define MAX_TIMER_SUPPORT     10
#define INVALID_TIMER_HANDLE  0xFFFFFFFF

typedef unsigned long timer_handle_t;
typedef void(*usr_sw_timer_callback)(timer_handle_t timer);

/**
*@brief 定时器初始化
*@param idx 定时器索引,使用HW timer作为基础timer
*@param particle 精度(1~60),单位毫秒(ms).HW timer以particle一半的周期产生中断，
*                扫描timer list，如果有timer到期，就调用其callback
*/
void user_sw_timer_init(eTIMER_IDX timer_idx, uint32_t particle);

/**
*@brief 添加定时器
*@param msec 定时设置, 单位：毫秒(ms)
*@param single_shot 0: 连续模式，超时之后，定时器会重新装载计数值.
*                   1: 单次模式, 超时之后，定时器停止工作。
*@param cb timer中断处理回调函数，会在中断处理中调用此函数

*@retval 操作成功，返回定时器句柄
*@retval INVALID_TIMER_HANDLE 添加定时器失败
*/
timer_handle_t user_sw_timer_add(uint32_t msec, bool single_shot,
                               usr_sw_timer_callback cb);

/**
*@brief 移除定时器
*@param handle 定时器句柄
*/
void user_sw_timer_remove(timer_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /*USER_INC_USER_SW_TIMER_H_*/

