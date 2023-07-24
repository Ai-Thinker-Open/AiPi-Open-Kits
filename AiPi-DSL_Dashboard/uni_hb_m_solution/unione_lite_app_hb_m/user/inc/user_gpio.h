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
 * Description : user_gpio.h
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.24
 *
 **************************************************************************/
#ifndef USER_INC_USER_GPIO_H_
#define USER_INC_USER_GPIO_H_
  
#ifdef __cplusplus
  extern "C" {
#endif
  
#include "unione.h"

/** @addtogroup uni_gpio_def
@{*/

/** @brief GPIO号*/
typedef enum {
  GPIO_NUM_A25 = 0,   ///<可用于SPI-MISO / ADC / DMIC-DATA
  GPIO_NUM_A26,       ///<可用于SPI-CLK / ADC / DMIC-CLK
  GPIO_NUM_A27,       ///<可用于SPI-MOSI / PWM / ADC
  GPIO_NUM_A28,       ///<已占用，PA使能控制（喇叭功放静音）
  GPIO_NUM_B0,        ///<已占用，SW-CLK（烧录器接口）
  GPIO_NUM_B1,        ///<已占用，SW-DATA（烧录器接口）
  GPIO_NUM_B2,        ///<可用于UART1-TX / PWM / I2C-SCL
  GPIO_NUM_B3,        ///<可用于UART1-RX / PWM / I2C-SDA
  GPIO_NUM_B6,        ///<UART1-RX（外设串口通信接收脚），不使能UART时可用做GPIO
  GPIO_NUM_B7,        ///<UART1-TX（外设串口通信发送脚），不使能UART是可用做GPIO
  GPIO_NUM_B8,        ///<已占用，虚拟Software UART-TX（Log输出引脚，波特率115200）
  GPIO_NUM_MAX
}GPIO_NUMBER;

/** @brief GPIO模式*/
typedef enum {
  GPIO_MODE_IN = 0,
  GPIO_MODE_OUT,
  GPIO_MODE_ADC,
  GPIO_MODE_PWM,
  GPIO_MODE_UART,
  GPIO_MODE_I2C,
  GPIO_MODE_SPI,
  GPIO_MODE_MAX
}GPIO_MODE;

/** @brief GPIO PU/PD模式选择*/
typedef enum {
  GPIO_PULL_UP,
  GPIO_PULL_DOWN,
  GPIO_PULL_UP_DOWN
}GPIO_PULL_MODE;

/** @brief GPIO 中断方式选择*/
typedef enum {
  GPIO_INT_NEG_EDGE,
  GPIO_INT_POS_EDGE,
  GPIO_INT_BOTH_EDGE
}GPIO_INT_TYPE;

/** @brief 中断回调函数
* @param num 中断io号
* @param is_high 是否高电平
*/
typedef void (*_gpio_interrupt_cb)(GPIO_NUMBER num, uni_bool is_high);
/** @}*/

/** @addtogroup uni_gpio_inf
@{*/

/**
*@brief GPIO初始化
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_init(void);

/**
*@brief GPIO反初始化
*/
void user_gpio_final(void);

/**
*@brief 设置GPIO工作模式
*@param num  GPIO端口号
*@param mode GPIO工作模式
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_set_mode(GPIO_NUMBER num, GPIO_MODE mode);

/**
*@brief 设置GPIO pull模式
*@param num  GPIO端口号
*@param mode GPIO pull模式
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_set_pull_mode(GPIO_NUMBER num, GPIO_PULL_MODE mode);

/**
*@brief 设置GPIO端口数值
*@param num  GPIO端口号
*@param val  0 or 1
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_set_value(GPIO_NUMBER num, int val);

/**
*@brief 获取GPIO端口值
*@param num GPIO端口号
*@retval else 读取数值
*@retval -1 操作失败
*/
int user_gpio_get_value(GPIO_NUMBER num);

/**
*@brief 使能GPIO中断
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_interrupt_enable(void);

/**
*@brief 关闭GPIO中断
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_interrupt_disable(void);

/**
*@brief 注册GPIO中断
*@param num GPIO端口号
*@param type GPIO电平跳变模式
*@param cb 中断处理回调函数
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_set_interrupt(GPIO_NUMBER num, GPIO_INT_TYPE type,
                                    _gpio_interrupt_cb cb);

/**
*@brief 注销GPIO中断注册回调函数
*@param num GPIO端口号
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_gpio_clear_interrupt(GPIO_NUMBER num);

/**
*@brief 设置GPIO端口输出定时脉冲
*@param num  GPIO端口号
*@param cycle  脉冲周期
*@param times  脉冲次数
*@param def_val  0 or 1，脉冲结束持续电平
*@retval 0  操作成功
*@retval -1 操作失败
*/
int user_sw_timer_gpio_pulse(GPIO_NUMBER num, int cycle, int times,
                                     int def_val);

/** @}*/

#ifdef __cplusplus
}
#endif
#endif
