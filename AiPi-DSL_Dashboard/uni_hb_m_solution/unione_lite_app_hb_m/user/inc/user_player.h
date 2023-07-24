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
 * Description : user_player.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.07
 *
 **************************************************************************/
#ifndef USER_INC_USER_PLAYER_H_
#define USER_INC_USER_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"
#include "user_event.h"

/** @addtogroup uni_player_inf
@{*/

/**
*@brief 播放文件
*@param type 播放类型
*@param file 播放文件名
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_player_play(AUDIO_PLAY_TYPE type, const char *file);

/**
*@brief 播放列表序号音频
*@param file_list 文件列表 [1, 2, 3]
*@param num 列表序号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_player_reply_list_num(const char *file_list, int num);

/**
*@brief 随机播放列表音频
*@param file_list 文件列表 [1, 2, 3]
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_player_reply_list_random(const char *file_list);

/**
*@brief 顺序播放列表中所有音频
*@param file_list 文件列表 [1, 2, 3]
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_player_reply_list_in_order(const char *file_list);

/**
*@brief 停止播放
*@param type 播放类型
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_player_stop(AUDIO_PLAY_TYPE type);

/**
*@brief 设置最小音量
*/
int user_player_set_volume_min(void);

/**
*@brief 设置最大音量
*/
int user_player_set_volume_max(void);

/**
*@brief 设置中等音量
*/
int user_player_set_volume_mid(void);

/**
*@brief 增加音量
*/
int user_player_set_volume_up(void);

/**
*@brief 减小音量
*/
int user_player_set_volume_down(void);

/**
*@brief 设置喇叭静音
*/
int user_player_speaker_mute(void);

/**
*@brief 取消设置喇叭静音
*/
int user_player_speaker_unmute(void);

/**
*@brief 系统进入无回复播报模式
*/
int user_player_shutup_mode(void);

/**
*@brief 系统退出无回复播报模式
*/
int user_player_shutup_exit(void);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif

