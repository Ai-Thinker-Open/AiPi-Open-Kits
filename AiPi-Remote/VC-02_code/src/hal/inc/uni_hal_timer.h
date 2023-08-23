#ifndef __UNI_HAL_TIMER_H__
#define __UNI_HAL_TIMER_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"
#include "uni_hal_pwm.h"

/**
 * @brief   定时器配置错误类型定义
 */
typedef enum __TIMER_ERROR_CODE
{
    TIMER_ERROR_INVALID_TIMER_INDEX = -128,
    TIMER_INTERRUPT_SRC_SEL,
    TIMER_ERROR_OK = 0
}TIMER_ERROR_CODE;

typedef void  (*timer_cb)(TIMER_INDEX TimerIdx);

/**
 * @brief      配置定时器参数，该函数单位为us
 *
 * @param[in]  TimerIndex        定时器索引, 详见 #TIMER_INDEX，目前基本定时timer是TIMER1 TIMER2 TIMER5 TIMER6，其中TIMER1用于systick，
                                 因此可选的timer是TIMER2 TIMER5 TIMER6
 * @param[in]  usec              定时设置, 单位：微妙(us)
 * @param[in]  SingleShot        0: 连续模式，超时之后，定时器会重新装载计数值.
 *                               1: 单次模式, 超时之后，定时器停止工作。
 * @param[in]  cb                timer 中断处理回调函数，会在中断处理中调用此函数
 * @return     错误号, 详见 #TIMER_ERROR_CODE
 */
extern TIMER_ERROR_CODE uni_hal_timer_init(TIMER_INDEX TimerIdx,uint32_t usec, bool SingleShot,timer_cb cb);

extern TIMER_ERROR_CODE uni_hal_timer_start(TIMER_INDEX TimerIdx);

extern TIMER_ERROR_CODE uni_hal_timer_pause(TIMER_INDEX TimerIdx,bool IsPause);//1:暂停TimerIndex计数 0：重新使能TimerIndex计数


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__UNI_HAL_TIMER_H__