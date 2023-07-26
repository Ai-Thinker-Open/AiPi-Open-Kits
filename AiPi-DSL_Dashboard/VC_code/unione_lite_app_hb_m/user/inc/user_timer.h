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
 * Description : user_timer.h
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.21
 *
 **************************************************************************/
#ifndef USER_INC_USER_TIMER_H_
#define USER_INC_USER_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "unione.h"

/** @ingroup uni_timer_def
*@brief 定时器索引
*/
typedef enum TIMER_IDX {
  eTIMER2 = 0,
  eTIMER5,
  eTIMER6,
  eTIMER_Max
}eTIMER_IDX;

/** @ingroup uni_timer_def
*@brief 定时器回调函数
*@param idx 定时器索引
*/
typedef void(*usr_timer_callback)(eTIMER_IDX idx);

/** @addtogroup uni_timer_inf
@{*/

/**
*@brief 定时器初始化
*@param idx 定时器索引
*@param usec 定时设置, 单位：微妙(us)
*@param single_shot 0: 连续模式，超时之后，定时器会重新装载计数值.
*                   1: 单次模式, 超时之后，定时器停止工作。
*@param cb timer中断处理回调函数，会在中断处理中调用此函数

*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_timer_init(eTIMER_IDX idx, uint32_t usec, bool single_shot,
                     usr_timer_callback cb);

/**
*@brief 启动定时器
*@param idx 定时器索引
*/
void user_timer_start(eTIMER_IDX idx);

/**
*@brief 暂停定时器计数
*@param idx 定时器索引
*/
void user_timer_pause(eTIMER_IDX idx);

/**
*@brief 重新使能定时器计数
*@param idx 定时器索引
*/
void user_timer_resume(eTIMER_IDX idx);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif /*USER_INC_USER_TIMER_H_*/

