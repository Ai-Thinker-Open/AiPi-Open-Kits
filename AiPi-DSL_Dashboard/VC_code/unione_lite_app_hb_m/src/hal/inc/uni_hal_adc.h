#ifndef __ADC_HAL_H__
#define __ADC_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"

void SarADC_Init(void);
int16_t ADC_SingleModeDataGet(uint32_t ChannalNum);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__ADC_HAL_H__
