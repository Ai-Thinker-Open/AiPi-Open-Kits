#ifndef __FACTORY_HAL_H__
#define __FACTORY_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"
/*******************************************************
gpio_cb:
     return -1: failed;
     return  0: success;

********************************************************/
typedef int  (*gpio_cb)(void);

extern void uni_hal_gpio_factory_cb_register(gpio_cb cb);

/***********************************************************************
for mic test ,you need firstly choose the right  threshold
************************************************************************/
extern void uni_hal_mic_threshold_set(uint32_t threshold);


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__UART_HAL_H__
