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
 * Description : user_uart_sucp.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.06.09
 *
 **************************************************************************/
#ifndef USER_INC_USER_UART_SUCP_H_
#define USER_INC_USER_UART_SUCP_H_
  
#ifdef __cplusplus
extern "C" {
#endif
  
#include "unione.h"

#define PACKED              __attribute__ ((packed))

/** @addtogroup uni_sample_communication_inf
@{*/

/** @brief 简易串口协议数据包定义*/
typedef struct {
  uni_u8  start_code;   ///< 起始码用于同步，固定为0x5A
  uni_u8  cmd_code;     ///< 命令码，用于传递指令信息
  uni_u8  reserve[2];   ///< 保留字用于扩展，未使用固定为0x00
  uni_u8  checksum;     ///< 8位加法校验和
}PACKED sucp_pack_t;

/**
*@brief 接收数据回调函数定义
*@param pack 接收到的数据包
*/
typedef void (*_user_uni_sucp_recv)(sucp_pack_t *pack);

/**
*@brief 通讯初始化
*@param cb_recv 接收数据回调函数
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_uni_sucp_init(_user_uni_sucp_recv cb_recv);

/**
*@brief 通讯逆初始化
*/
void user_uni_sucp_final(void);

/**
*@brief 通讯数据发送
*@param cmd_code 命令码
*@param reserve 协议预留字节，固定长度为2
*@retval else 实际发送字符长度
*@retval -1 操作失败
*/
int user_uni_sucp_send(uni_u8 cmd_code, uni_u8 reserve[2]);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif // USER_INC_USER_UART_UCP_H_


