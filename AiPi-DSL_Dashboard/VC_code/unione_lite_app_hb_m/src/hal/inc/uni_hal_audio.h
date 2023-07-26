#ifndef __AUDIO_HAL_H__
#define __AUDIO_HAL_H__

#include "type.h"

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#define DAC_BUF_LEN   512   //the actual len is 512*4 bytes

typedef enum _ADC_MODULE
{
    ADC0_MODULE,
    ADC1_MODULE
    
} ADC_MODULE;

typedef enum _DAC_MODULE
{
	DAC0 = 0,
	DAC1 = 1,
	ALL = 2		//DAC0和DAC1同时工作
}DAC_MODULE;

/******************************************************************************************************************************
mic_gain: [0-63]
default mic_gain is 27, if you want to change the mic gain,youn need firstly call uni_hal_mic_gain_set,then call 
uni_hal_record_init.
*******************************************************************************************************************************/
extern void uni_hal_mic_gain_set(uint8_t mic_gain);
extern void uni_hal_record_init(ADC_MODULE ADCModule,uint32_t SampleRate,void* Buf, uint16_t Len);
extern void uni_hal_play_init(DAC_MODULE DACModule, uint32_t SampleRate, void* Buf1, uint16_t Len1, void* Buf2, uint16_t Len2);
extern void uni_hal_record_dmarestart(ADC_MODULE Module, void* Buf, uint16_t Len);
extern void uni_hal_record_deinit(ADC_MODULE Module);
extern uint16_t uni_hal_record_datalenget(ADC_MODULE Module);
extern uint16_t uni_hal_record_dataget(ADC_MODULE Module, void* Buf, uint16_t Len);
extern uint16_t uni_hal_play_spacelenget(DAC_MODULE DACModule);
extern uint16_t uni_hal_play(DAC_MODULE DACModule, void* Buf, uint16_t Len);
extern void uni_hal_play_vol_set(DAC_MODULE DACModule,uint16_t LeftVol, uint16_t RightVol);//0-100
extern void uni_hal_play_mute(DAC_MODULE DACModule,bool LeftMuteEn, bool RightMuteEn);
typedef void (*record_cb)(uint8_t* buf, uint32_t size);
extern void uni_hal_record_cb(uint8_t* buf, uint32_t size);
extern void uni_hal_register_record_cb(record_cb cb);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AUDIO_HAL_H__
