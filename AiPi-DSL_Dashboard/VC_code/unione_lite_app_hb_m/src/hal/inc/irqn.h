#include "type.h"

#ifndef __IRQN_H__
#define __IRQN_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus 

/**
 * @brief	�����ж�ID
 */
typedef enum IRQn
{
    /*
     * internal interrupt(Exception/Fault)
     */
    RST_IRQn		= -9,
    TLB_Fill_IRQn		= -8,
    PTE_IRQn	= -7,
    TLB_Misc_IRQn		= -6,//programmable as below
    TLB_VLPT_Miss_IRQn		= -5,
    Machine_ERR_IRQn		= -4,
    Debug_IRQn		= -3,
    General_Exception_IRQn		= -2,
    Syscall_IRQn		= -1,
    /*
     * SOC interrupt(External Interrupt)
     */

    TMR1_IRQn	    = 0,
    Wakeup_IRQn		= 1,
    Gpio_IRQn		= 2,
    Rtc_IRQn		= 3,
    Spdif_IRQn		= 4,
    SWI_IRQn		= 5,
    I2C_InIRQn		= 6,
    UART0_IRQn		= 7,
    Timer2_IRQn		= 8,
    DMA0_IRQn		= 9,
    DMA1_IRQn		= 10,
    DMA2_IRQn		= 11,
    DMA3_IRQn		= 12,
    DMA4_IRQn		= 13,
    DMA5_IRQn		= 14,
    DMA6_IRQn		= 15,
    DMA7_IRQn		= 16,
    DMA8_IRQn		= 17,
    UART1_IRQn		= 18,
    USI_IRQn		= 19,
    I2s_IRQn	    = 20,
    Timer3_IRQn		= 21,
    Timer4_IRQn	    = 22,
    Timer5_IRQn		= 23,
    Timer6_IRQn		= 24,
    SDIO0_IRQn		= 25,
    SDIO1_IRQn		= 26,
    Usb_IRQn		= 27,
    SPIM_IRQn		= 28,
    PSR_IRQn		= 29,
    SPIS_IRQn		= 30,
    FFTInt_IRQn		= 31

} IRQn_Type;

/**
 * @brief	����Ĭ���ж����ȼ�
 */
typedef enum IRQn_PRIO
{
    /*
     * SOC interrupt(External Interrupt)
     */
	TMR1_IRQn_PRIO	    = 3,
	Wakeup_IRQn_PRIO	= 2,
    GPIO_IRQn_PRIO		= 2,
    RTC_IRQn_PRIO		= 2,
	WDG_IRQn_PRIO		= 2,
	SWI_IRQn_PRIO		= 3,
	I2C_IRQn_PRIO		= 2,
    UART0_IRQn_PRIO		= 2,
    TMR2_IRQn_PRIO		= 2,
	DMA0_IRQn_PRIO		= 2,
	DMA1_IRQn_PRIO		= 2,
	DMA2_IRQn_PRIO		= 2,
	DMA3_IRQn_PRIO		= 2,
	DMA4_IRQn_PRIO		= 2,
	DMA5_IRQn_PRIO		= 2,
	DMA6_IRQn_PRIO		= 2,
	DMA7_IRQn_PRIO		= 2,
    UART1_IRQn_PRIO		= 2,
    UART2_IRQn_PRIO		= 2,
	I2S_IRQn_PRIO		= 2,
	TMR3_IRQn_PRIO	    = 2,
    TMR4_IRQn_PRIO		= 2,
	TMR5_IRQn_PRIO	    = 2,
    TMR6_IRQn_PRIO		= 2,
    TMR7_IRQn_PRIO		= 2,
    TMR8_IRQn_PRIO		= 2,
	SDIO_IRQn_PRIO		= 2,
    OTG_IRQn_PRIO		= 2,
	SPIM0_IRQn_PRIO		= 2,
	SPIM1_IRQn_PRIO		= 2,
	SPIS_IRQn_PRIO		= 2,
	FDMA_IRQn_PRIO		= 2
} IRQn_PRIO_TYPE;

/**
 * @brief	����CPU Powerbrakeʱʹ��
 */
#define NDS_THROTTLE_LEVEL_0  (0x00)
#define NDS_THROTTLE_LEVEL_1  (0x10)
#define NDS_THROTTLE_LEVEL_2  (0x20)
#define NDS_THROTTLE_LEVEL_3  (0x30)
#define NDS_THROTTLE_LEVEL_4  (0x40)
#define NDS_THROTTLE_LEVEL_5  (0x50)
#define NDS_THROTTLE_LEVEL_6  (0x60)
#define NDS_THROTTLE_LEVEL_7  (0x70)
#define NDS_THROTTLE_LEVEL_8  (0x80)
#define NDS_THROTTLE_LEVEL_9  (0x90)
#define NDS_THROTTLE_LEVEL_10 (0xA0)
#define NDS_THROTTLE_LEVEL_11 (0xB0)
#define NDS_THROTTLE_LEVEL_12 (0xC0)
#define NDS_THROTTLE_LEVEL_13 (0xD0)
#define NDS_THROTTLE_LEVEL_14 (0xE0)
#define NDS_THROTTLE_LEVEL_15 (0xF0)

/**
 * @brief	ʹ��ȫ���ж�
 * @param	None
 * @return	None
 * @note
 */
void GIE_ENABLE(void);

/**
 * @brief	����ȫ���ж�
 * @param	None
 * @return	None
 * @note
 */
void GIE_DISABLE(void);

/**
 * @brief	��ȡȫ���ж�״̬
 * @param	None
 * @return	TRUE��ȫ���ж�ʹ�ܣ� FALSE��ȫ���жϽ���
 * @note
 */
bool GIE_STATE_GET();
/**
 * @brief	ʹ��PowerBrake����
 * @param	None
 * @return	None
 * @note
 */
void Throttle_Enable(void);

/**
 * @brief	����PowerBrake����
 * @param	None
 * @return	None
 * @note
 */
void Throttle_Disable(void);

/**
 * @brief	����Throttle level
 * @param	setting
 * 			NDS_THROTTLE_LEVEL_0��   16��CPU clockȫ����Ч��������ߵ�����
 * 			...
 * 			NDS_THROTTLE_LEVEL_15��16��CPU clock��ֻ��1��clock������,������͵�����
 * @return	None
 * @note
 */
void Throttle_Setup(uint32_t setting);

/**
 * @brief	ʹ�������ж�
 * @param	IRQn	����ID
 * @return	None
 * @note
 */
void NVIC_EnableIRQ(IRQn_Type IRQn);

/**
 * @brief	���������ж�
 * @param	IRQn	����ID
 * @return	None
 * @note
 */
void NVIC_DisableIRQ(IRQn_Type IRQn);

/**
 * @brief	����������ж����ȼ�
 * @param	IRQn	����ID
 * @param	priority
 * 			0,1,2,3 0=highest 3=lowest
 * @return	None
 * @note
 */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

/**
 * @brief	��ȡ������ж����ȼ�
 * @param	IRQn	����ID
 * @return	�ж����ȼ�
 * @note
 */
uint32_t NVIC_GetPriority(IRQn_Type IRQn);

/**
 * @brief	�����жϵ����ȼ���ʹ���жϣ�ֻ�����ȼ�����Level���жϲŻᱻʹ��
 * @param	Level
 * 			0,1,2,3 0=highest 3=lowest
 * @return	None
 * @note
 */
void InterruptLevelSet(uint8_t Level);

/**
 * @brief	�ָ������ж�
 * @return	None
 * @note
 */
void InterruptLevelRestore(void);

#ifdef __cplusplus
}
#endif // __cplusplus 

#endif //__IRQN_H__

