#ifndef __PWM_HAL_H__
#define __PWM_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"

#define PWM_MAX_FREQ_DIV_VALUE   (65535 << 15)

//PWM 输出通道GPIO复用关系
#define   TIMER3_PWM_A27_B0_B2   0    //TIMER3的PWM引脚可复用在A27, B0或者B2
#define   TIMER4_PWM_A28_B1_B3   1    //TIMER4的PWM引脚可复用在A28, B1或者B3


typedef enum __TIMER_INDEX
{
	TIMER1 = 0,
    TIMER2 = 1,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
}TIMER_INDEX;
/**
 * PWM IO Mode select definition
 */ 
typedef enum __PWM_IO_MODE
{
    PWM_IO_MODE_NONE = 0,    //还原为GPIO
    PWM_IO_MODE_OUT  = 1,    //直接输出
    PWM_IO_MODE_PD1  = 2,    //PWM下拉2.4mA输出
}PWM_IO_MODE;

/**
 * @brief  PWM输出类型定义  
 */
typedef enum __PWM_OUTPUT_TYPE
{
    PWM_OUTPUT_FORCE_LOW    = 4,      //强制输出低电平
    PWM_OUTPUT_FORCE_HIGH   = 5,      //强制输出高电平 
    PWM_OUTPUT_SINGLE_1     = 6,      //标准输出：Duty为高脉冲占空比    
    PWM_OUTPUT_SINGLE_2     = 7,      //标准输出：Duty为负脉冲占空比  
    PWM_OUTPUT_ONE_PULSE	= 8	      //输出一个脉冲后结束
}PWM_OUTPUT_TYPE;

/**
 * @brief  PWM计数模式 
 */
typedef enum __PWM_COUNTER_MODE
{
    PWM_COUNTER_MODE_DOWN = 0,        //从高到低计数
    PWM_COUNTER_MODE_UP,              //从低到高计数
    PWM_COUNTER_MODE_CENTER_ALIGNED1, //先从低加到高，再从高减至低，只在下溢出时产生中断
    PWM_COUNTER_MODE_CENTER_ALIGNED2, //先从低加到高，再从高减至低，只在上溢出时产生中断
    PWM_COUNTER_MODE_CENTER_ALIGNED3  //先从低加到高，再从高减至低，在上溢出和下溢出时都产生中断
}PWM_COUNTER_MODE;

/**
 * @brief  PWM触发模式：中断或DMA
 */
typedef enum _PWM_DMA_INTERRUPT_MODE
{
	PWM_REQ_INTERRUPT_MODE = 0,  /**< 中断模式  */
	PWM_REQ_DMA_MODE             /**< DMA模式  */

} PWM_DMA_INTERRUPT_MODE;

/**
 * @brief  PWM配置错误类型定义  
 */
typedef enum __PWM_ERROR_CODE
{
    PWM_ERROR_INVALID_TIMER_INDEX = -128,
    PWM_ERROR_INVALID_PWM_TYPE,
    PWM_ERROR_INVALID_PWM_COUNTER_MODE,
    PWM_ERROR_OK = 0
}PWM_ERROR_CODE;


typedef struct __PWM_StructInit
{
    uint8_t     CounterMode;        //PWM计数模式，参数取值范围 #PWM_COUNTER_MODE   
    uint8_t     OutputType;         //PWM输出类型,参数取值为#PWM_OUTPUT_TYPE   
    uint32_t    FreqDiv;            //PWM频率与系统时钟的分频比，取值范围[1, PWM_MAX_FREQ_DIV_VALUE]   
    uint16_t    Duty;               //占空比,取值范围[0~100]对应占空比为[0%~100%]   
    bool        DMAReqEnable;       //是否使能DMA从Mem搬运占空比到外设中，参数取值： 1 -- 使能DMA请求； 0 -- 禁能DMA请求
}PWM_StructInit;   


/**
 * @brief PWM IOCTRL 命令
 */
typedef enum _PWM_IOCTRL_CMD
{
	//update and Select cmd
	OUTPUT_SOFTWARE_UPDATE 		= 0x1,	    /**< TIMERx软件update寄存器  */
	OUTPUT_TYPE_UPDATE 			= 0x2,		/**< #PWM_OUTPUT_TYPE  */
	OUTPUT_FREQUENCY_UPDATE 	= 0x10,	    /**< 频率更新  */
	OUTPUT_DUTY_UPDATE 			= 0x20,		/**< 占空比更新  */
} PWM_IOCTRL_CMD;
/**
 * @brief PWM IOCTRL 参数
 */
typedef struct _PWM_IOCTRL_ARG
{
	PWM_OUTPUT_TYPE     	   OutputType;         /**< PWM输出类型  */
	uint32_t    			   FreqDiv;            /**< PWM频率与系统时钟的分频比，取值范围[1, PWM_MAX_FREQ_DIV_VALUE]  */
	uint16_t				   Duty;			   /**< 占空比  */
} PWM_IOCTRL_ARG;

/**
 *PwmIoSel : 
            当选择TIMER3_PWM_A27_B0_B2:
 *               PwmIoSel = 1,则复用于A27,
 *               PwmIoSel = 2,则复用于B0，
 *               PwmIoSel = 3,则复用于B2
 *          当选择TIMER4_PWM_A28_B1_B3:
  *              PwmIoSel = 1,则复用于A28,
 *               PwmIoSel = 2,则复用于B1，
 *               PwmIoSel = 3,则复用于B3

 **/
 
extern void uni_hal_pwm_uninit(TIMER_INDEX TimerIdx,uint8_t PwmIoSel);

extern PWM_ERROR_CODE uni_hal_pwm_init(TIMER_INDEX TimerIdx,uint8_t PwmIoSel,PWM_StructInit *PWMParam);

extern PWM_ERROR_CODE uni_hal_pwm_enable(TIMER_INDEX TimerIdx);

extern PWM_ERROR_CODE uni_hal_pwm_disable(TIMER_INDEX TimerIdx);

extern PWM_ERROR_CODE uni_hal_pwm_pause(TIMER_INDEX TimerIdx,bool IsPause);//1: PWM暂停，保持在最后输出电平  0：PWM正常输出

extern PWM_ERROR_CODE uni_hal_pwm_update_duty(TIMER_INDEX TimerIdx,uint8_t duty);//duty:0-100

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__PWM_HAL_H__
