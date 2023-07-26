#ifndef __POWER_HAL_H__
#define __POWER_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include "type.h"

#define     WAKEUP_GPIOA0          				(0)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOA1          				(1)	/**<GPIO bit1 macro*/
#define     WAKEUP_GPIOA2          				(2)	/**<GPIO bit2 macro*/
#define     WAKEUP_GPIOA3          				(3)	/**<GPIO bit3 macro*/
#define     WAKEUP_GPIOA4          				(4)	/**<GPIO bit4 macro*/
#define     WAKEUP_GPIOA5         	 			(5)	/**<GPIO bit5 macro*/
#define     WAKEUP_GPIOA6          				(6)	/**<GPIO bit6 macro*/
#define     WAKEUP_GPIOA7          				(7)	/**<GPIO bit7 macro*/
#define     WAKEUP_GPIOA8          				(8)	/**<GPIO bit8 macro*/
#define     WAKEUP_GPIOA9          				(9)	/**<GPIO bit9 macro*/
#define     WAKEUP_GPIOA10         				(10)	/**<GPIO bit10 macro*/
#define     WAKEUP_GPIOA11         				(11)	/**<GPIO bit11 macro*/
#define     WAKEUP_GPIOA12         				(12)	/**<GPIO bit12 macro*/
#define     WAKEUP_GPIOA13         				(13)	/**<GPIO bit13 macro*/
#define     WAKEUP_GPIOA14         				(14)	/**<GPIO bit14 macro*/
#define     WAKEUP_GPIOA15         				(15)	/**<GPIO bit15 macro*/
#define     WAKEUP_GPIOA16         				(16)	/**<GPIO bit16 macro*/
#define     WAKEUP_GPIOA17         				(17)	/**<GPIO bit17 macro*/
#define     WAKEUP_GPIOA18         				(18)	/**<GPIO bit18 macro*/
#define     WAKEUP_GPIOA19         				(19)	/**<GPIO bit19 macro*/
#define     WAKEUP_GPIOA20         				(20)	/**<GPIO bit20 macro*/
#define     WAKEUP_GPIOA21         				(21)	/**<GPIO bit21 macro*/
#define     WAKEUP_GPIOA22         				(22)	/**<GPIO bit22 macro*/
#define     WAKEUP_GPIOA23         				(23)	/**<GPIO bit23 macro*/
#define     WAKEUP_GPIOA24         				(24)	/**<GPIO bit24 macro*/
#define     WAKEUP_GPIOA25         				(25)	/**<GPIO bit25 macro*/
#define     WAKEUP_GPIOA26         				(26)	/**<GPIO bit26 macro*/
#define     WAKEUP_GPIOA27         				(27)	/**<GPIO bit27 macro*/
#define     WAKEUP_GPIOA28         				(28)	/**<GPIO bit28 macro*/
#define     WAKEUP_GPIOA29        				(29)	/**<GPIO bit29 macro*/
#define     WAKEUP_GPIOA30        			    (30)	/**<GPIO bit30 macro*/
#define     WAKEUP_GPIOA31         				(31)	/**<GPIO bit31 macro*/

#define     WAKEUP_GPIOB0          				(32)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB1          				(33)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB2          				(34)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB3          				(35)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB4          				(36)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB5          				(37)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB6          				(38)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB7          				(39)	/**<GPIO bit0 macro*/
#define     WAKEUP_GPIOB8          				(40)	/**<GPIO bit0 macro*/

#define     WAKEUP_GPIOPOSE          			(1) 	/**posedge trigger*/
#define     WAKEUP_GPIONEGE          			(0) 	/**negedge trigger*/

/**********************************************************************************
gpio:
    WAKEUP_GPIOA0---WAKEUP_GPIOA31 or WAKEUP_GPIOB0--WAKEUP_GPIOB8
edge:
    WAKEUP_GPIOPOSE or WAKEUP_GPIONEGE
***********************************************************************************/
extern void uni_hal_enterdeepsleep(void (*cb)(int flag),uint32_t gpio,uint8_t edge);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__POWER_HAL_H__
