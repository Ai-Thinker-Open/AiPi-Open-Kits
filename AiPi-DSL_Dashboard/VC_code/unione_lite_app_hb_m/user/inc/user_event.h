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
 * Description : user_event.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.03
 *
 **************************************************************************/
#ifndef USER_INC_USER_EVENT_H_
#define USER_INC_USER_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "unione.h"

/** @addtogroup uni_event_def
@{*/

/** @brief 用户可订阅的事件类型*/
typedef enum {
  USER_INVALID_EVENT = 0, ///< 错误的事件类型
  USER_AUDIO_PLAY_START,  ///< 音频开始播放时发送
  USER_AUDIO_PLAY_END,    ///< 音频播放完毕或被停止时发送
  USER_CUSTOM_SETTING,    ///< 识别到客户自定义的识别词时发送
  USER_VOLUME_SETTING,    ///< 识别到音量调节指令时发送
  USER_GOTO_SLEEPING,     ///< 进入待唤醒状态时发送
  USER_GOTO_AWAKENED,     ///< 进入识别状态（已唤醒）时发送
  USER_EVENT_MAX
}USER_EVENT_TYPE;

/** @brief 音频播放类型枚举（目前只支持AUDIO_PLAY_REPLY）*/
typedef enum {
  AUDIO_PLAY_REPLY = 0,   ///< 回复语播报
  AUDIO_PLAY_MUSIC,       ///< 音乐播放
  AUDIO_PLAY_SKILL        ///< 技能TTS回复播报
}AUDIO_PLAY_TYPE;

/** @brief 对应USER_AUDIO_PLAY_START事件*/
typedef struct {
  AUDIO_PLAY_TYPE type;         ///< 音频类型
  char            *file_name;   ///< 播放音频文件名
}event_audio_play_t;

/** @brief 对应USER_AUDIO_PLAY_END事件*/
typedef struct {
  AUDIO_PLAY_TYPE type;         ///< 音频类型
  uni_bool        is_auto;      ///< 是否自动播放完毕，true：自动播放完毕，false：主动停止
}event_audio_end_t;

/** @brief 对应USER_CUSTOM_SETTING事件*/
typedef struct {
  char            *cmd;         ///< 命令词意图，对应UDP平台上用户定义脚本中的action
  char            *word_str;    ///< 识别到的命令词
  char            *reply_files; ///< 回复语列表[1, 2, 3]，对应UDP平台上用户定义脚本中的回复语列表，在pcm_map.txt中可以找到对应关系
}event_custom_setting_t;

/** @brief 事件触发来源*/
typedef enum {
  EVENT_TRIGGER_ASR = 0,        ///< 语音识别指令触发
  EVENT_TRIGGER_AUTO,           ///< 自动状态切换（比如超时等）
  EVENT_TRIGGER_USER            ///< 用户接口主动触发
}EVENT_TRIGGER;

/** @brief 对应USER_GOTO_SLEEPING事件*/
typedef struct {
  EVENT_TRIGGER   trigger;      ///< 触发来源
  char            *cmd;         ///< 命令词意图，对应UDP平台上用户定义脚本中的action
  char            *word_str;    ///< 识别到的命令词
  char            *reply_files; ///< 回复语列表[1, 2, 3]，对应UDP平台上用户定义脚本中的回复语列表，在pcm_map.txt中可以找到对应关系
}event_goto_sleeping_t;

/** @brief 对应USER_GOTO_AWAKENED事件*/
typedef struct {
  EVENT_TRIGGER   trigger;      ///< 触发来源
  char            *cmd;         ///< 命令词意图，对应UDP平台上用户定义脚本中的action
  char            *word_str;    ///< 识别到的命令词
  char            *reply_files; ///< 回复语列表[1, 2, 3]，对应UDP平台上用户定义脚本中的回复语列表，在pcm_map.txt中可以找到对应关系
}event_goto_awakend_t;

/** @brief 对应USER_VOLUME_SETTING事件*/
typedef struct {
  int             value;        ///< 当前音量值
  int             min_val;      ///< 可设置最小音量值
  int             max_val;      ///< 可设置最大音量值
  EVENT_TRIGGER   trigger;      ///< 触发来源
  char            *cmd;         ///< 命令词意图，对应UDP平台上用户定义脚本中的action
  char            *word_str;    ///< 识别到的命令词
  char            *reply_files; ///< 回复语列表[1, 2, 3]，对应UDP平台上用户定义脚本中的回复语列表，在pcm_map.txt中可以找到对应关系
}event_volume_setting_t;

/** @brief 事件内容，共用体，根据事件类型具体处理*/
typedef union {
  event_audio_play_t      audio_play;     ///< 对应USER_AUDIO_PLAY_START事件
  event_audio_end_t       audio_end;      ///< 对应USER_AUDIO_PLAY_END事件
  event_custom_setting_t  custom_setting; ///< 对应USER_CUSTOM_SETTING事件
  event_volume_setting_t  voluem_setting; ///< 对应USER_VOLUME_SETTING事件
  event_goto_sleeping_t   goto_sleeping;  ///< 对应USER_GOTO_SLEEPING事件
  event_goto_awakend_t    goto_awakend;   ///< 对应USER_GOTO_AWAKENED事件
}user_event_context_t;

/** @brief 事件回调函数，当用户订阅事件发生时会被调用
* @param event 事件类型
* @param context 事件信息
*/
typedef void (*_user_event_cb)(USER_EVENT_TYPE event,
                                    user_event_context_t *context);
/** @}*/

/** @addtogroup uni_event_inf
@{*/

/**
*@brief 订阅一个事件
*@param event 待订阅事件类型
*@param cb 事件回调函数指针
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_event_subscribe_event(USER_EVENT_TYPE event,
                                        _user_event_cb cb);

/**
*@brief 清空事件订阅队列，不再接受任何事件
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_event_clear_observers(void);

/**
*@brief 查询事件回调函数
*@param event 待查询事件类型
*@return 事件回调函数指针
*/
_user_event_cb user_event_get_observer(USER_EVENT_TYPE event);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif


