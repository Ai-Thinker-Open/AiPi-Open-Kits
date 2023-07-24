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
 * Description : user_pwm.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.26
 *
 **************************************************************************/
#ifndef USER_INC_USER_PWM_H_
#define USER_INC_USER_PWM_H_
  
#ifdef __cplusplus
  extern "C" {
#endif
  
#include "unione.h"

/** @ingroup uni_pwm_def
* PWM管脚号
*/
typedef enum {
  PWM_NUM_1_A27 = 0,
  PWM_NUM_1_B0,
  PWM_NUM_1_B2,      ///< PWM 1 only 1 pin work a time
  PWM_NUM_2_A28,     ///< used for PA enable, don't use it on HB-M demo board
  PWM_NUM_2_B1,
  PWM_NUM_2_B3,      ///< PWM 2 only 1 pin work a time
  PWM_NUM_MAX
}USER_PWM_NUM;

/** @addtogroup uni_pwm_inf 
@{*/

/**
*@brief PWM初始化
*@param num PWM管脚号
*@param hz 频率
*@param is_high_duty \a TRUE :占空比用高电平持续时间计算; \a FALSE :占空比用低电平持续时间计算
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_init(USER_PWM_NUM num, uni_u32 hz, uni_bool is_high_duty);

/**
*@brief PWM反初始化
*@param num PWM管脚号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_final(USER_PWM_NUM num);

/**
*@brief 开使PWM输出
*@param num PWM管脚号
*@param duty 占空比
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_start(USER_PWM_NUM num, uni_u8 duty);

/**
*@brief 停止PWM输出
*@param num PWM管脚号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_stop(USER_PWM_NUM num);

/**
*@brief 暂停PWM输出
*@param num PWM管脚号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_pause(USER_PWM_NUM num);

/**
*@brief 恢复PWM输出
*@param num PWM管脚号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_resume(USER_PWM_NUM num);

/**
*@brief PWM占空比切换
*@param num PWM管脚号
*@param duty 占空比
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_change_duty(USER_PWM_NUM num, uni_u8 duty);

/**
*@brief PWM占空比增加
*@param num PWM管脚号
*@param ch_duty 增加的占空比
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_duty_inc(USER_PWM_NUM num, uni_u8 ch_duty);

/**
*@brief PWM占空比减小
*@param num PWM管脚号
*@param ch_duty 减小的占空比
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_pwm_duty_dec(USER_PWM_NUM num, uni_u8 ch_duty);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
