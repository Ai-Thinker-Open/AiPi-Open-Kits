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
 * Description : user_asr.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.07
 *
 **************************************************************************/
#ifndef USER_INC_USER_ASR_H_
#define USER_INC_USER_ASR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"

/** @addtogroup uni_asr_inf
@{*/

/**
* 进入后将只会识别唤醒词
*@brief 进入睡眠状态
*@retval 0  操作成功
*/
int user_asr_goto_sleep(void);

/**
* 进入后将只会识别命令词
*@brief 进入唤醒状态
*@retval 0  操作成功
*/
int user_asr_goto_awakend(void);

/**
* mic开始接收数据
*@brief 使能语音识别
*@retval 0  操作成功
*/
int user_asr_recognize_enable(void);

/**
* mic被关闭，停止拾音
*@brief 关闭语音识别
*@retval 0  操作成功
*/
int user_asr_recognize_disable(void);

/**
* 启用识别词
*@brief 启用识别词
*@param word 要启用的识别词
*@retval 0  操作成功
*/
int user_asr_word_enable(const char *word);

/**
* 禁用识别词
*@brief 禁用识别词
*@param word 要禁用的识别词
*@retval 0  操作成功
*/
int user_asr_word_disable(const char *word);

/**
* 禁止超时自动退出识别模式
*@brief 禁止超时自动退出识别模式
*@retval 0  操作成功
*/
int user_asr_timeout_disable(void);

/**
* 使能超时自动退出识别模式
*@brief 使能超时自动退出识别模式
*@retval 0  操作成功
*/
int user_asr_timeout_enable(void);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif

