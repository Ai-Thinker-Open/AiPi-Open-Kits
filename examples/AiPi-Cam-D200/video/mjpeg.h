#ifndef __MJPEG_H__
#define __MJPEG_H__

#include "stdint.h"

#define MJPEG_MAX_SIZE   (100 * 1024)//(50 * 1024)
#define MJPEG_QUEUES_NUM (6)

typedef struct {
    void *frame_address;
    uint32_t frame_size;
    int16_t frame_id;
} jpeg_frame_t;

volatile uint32_t g_mjpeg_fps;

void mjpeg_init(uint8_t quality);
int mjpeg_compress_frame_push(void *yuv_src);

int jpeg_frame_init(void);
int jpeg_frame_reset(void);
int jpeg_frame_alloc(jpeg_frame_t *jpeg_frame_info, uint32_t timeout);
int jpeg_frame_push(jpeg_frame_t *jpeg_frame_info);
int jpeg_frame_pop(jpeg_frame_t *jpeg_frame_info, uint32_t timeout);
int jpeg_frame_free(jpeg_frame_t *jpeg_frame_info);

#endif /* __DVP_MJPEG_H__ */