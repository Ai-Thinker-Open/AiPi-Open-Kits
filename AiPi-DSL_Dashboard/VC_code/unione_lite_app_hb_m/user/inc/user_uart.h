/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
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
 * Description : user_uart.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.14
 *
 **************************************************************************/
#ifndef USER_INC_USER_UART_H_
#define USER_INC_USER_UART_H_
  
#ifdef __cplusplus
  extern "C" {
#endif
  
#include "unione.h"

/** @addtogroup uni_uart_inf
@{*/

/**
*@brief 接收数据回调函数定义
*@param buf 接收字符指针
*@param len 接收字符长度
*/
typedef void (*_user_uart_recv)(char *buf, int len);

/**
*@brief 串口初始化
*@param cb_recv 接收数据回调函数
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_uart_init(_user_uart_recv cb_recv);

/**
*@brief 串口逆初始化
*/
void user_uart_final(void);

/**
*@brief 串口数据发送
*@param buf 发送字符指针
*@param len 发送字符长度
*@retval else 实际发送字符长度
*@retval -1 操作失败
*/
int user_uart_send(const char *buf, int len);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif


