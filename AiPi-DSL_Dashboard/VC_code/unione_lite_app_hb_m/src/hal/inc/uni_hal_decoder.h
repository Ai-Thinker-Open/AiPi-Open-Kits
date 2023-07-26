#ifndef __DECODER_HAL_H__
#define __DECODER_HAL_H__

#include "type.h"

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

typedef struct {
  int num_channels;
  int sampling_rate;
  int bitrate;
  int bit_width;
}uni_song_info;

typedef struct {
  char *pcm_addr;
  int data_length;
  int error_code;
}uni_frame_info;

typedef uint32_t (*_fill_stream)(void *buffer, uint32_t length);

int uni_hal_decoder_init(char *decoder_buf, _fill_stream cb);

void uni_hal_decoder_deinit(void);

int uni_hal_decoder_song_info(uni_song_info *info);

int uni_hal_decoder_frame_info(uni_frame_info *info);

bool uni_hal_decoder_can_continue(void);

int uni_hal_decoder_decode(void);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__DECODER_HAL_H__
