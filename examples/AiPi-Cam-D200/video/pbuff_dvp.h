#ifndef __DVP_MJPEG_H__
#define __DVP_MJPEG_H__

#include "stdint.h"

#define PBUFF_SHARE_EN   (1)

#define PBUFF_DATA_SIZE  (1600 * 2 *16)//(640 * 480 * 2)
#define PBUFF_QUEUES_NUM (2)

#define DVP_ISR_NOTIFY_INDEX  (0)
#define JPEG_ISR_NOTIFY_INDEX (1)

#define JPEG_FRAME_COMP       (2)
#define JPEG_OVER_SIZE        (3)

typedef struct {
    void *frame_address;
    int16_t pbuff_id;

    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;
} pbuff_yuyv_frame_t;

extern volatile uint32_t g_dvp_fps;

extern uint8_t picture_buffer[1][PBUFF_DATA_SIZE];
// extern TaskHandle_t dvp_mjpeg_task_hd;

int dvp_cam_init(void);

int picture_frame_init(void);
int picture_frame_reset(void);
int picture_frame_alloc(pbuff_yuyv_frame_t *new_frame, uint32_t timeout);
int picture_frame_push(pbuff_yuyv_frame_t *new_frame);
int picture_frame_pop(pbuff_yuyv_frame_t *new_frame, uint32_t timeout);
int picture_frame_free(pbuff_yuyv_frame_t *new_frame);

void dvp_cam_start(void);
void dvp_cam_stop(void);

#endif /* __DVP_MJPEG_H__ */