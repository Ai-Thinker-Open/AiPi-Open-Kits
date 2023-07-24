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
 * Description : user_uart_ucp.h
 * Author      : shangjinlong@unisound.com
 * Date        : 2020.04.20
 *
 **************************************************************************/
#ifndef USER_INC_USER_UART_UCP_H_
#define USER_INC_USER_UART_UCP_H_
  
#ifdef __cplusplus
extern "C" {
#endif
  
#include "unione.h"

/** @addtogroup uni_communication_inf
@{*/

/**
*@brief 接收数据回调函数定义
*@param cmd 命令值
*@param payload 接收字符指针
*@param len 接收字符长度
*/
typedef void (*_user_uni_recv)(uni_u16 cmd, char *payload, int len);

/**
*@brief 通讯初始化
*@param cb_recv 接收数据回调函数
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_uni_ucp_init(_user_uni_recv cb_recv);

/**
*@brief 通讯逆初始化
*/
void user_uni_ucp_final(void);

/**
*@brief 通讯数据发送
*@param cmd 命令值
*@param payload 发送字符指针
*@param len 发送字符长度
*@param is_block 是否堵塞
*@retval else 实际发送字符长度
*@retval -1 操作失败
*/
int user_uni_ucp_send(uni_u16 cmd, char *payload, int len, uni_bool is_block);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif // USER_INC_USER_UART_UCP_H_


