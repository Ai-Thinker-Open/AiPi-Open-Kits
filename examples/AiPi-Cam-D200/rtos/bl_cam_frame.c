#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <semphr.h>
#include <queue.h>
#include <stream_buffer.h>
// #include <mjpeg_booster.h>
// #include <mjpeg_sw_buffer.h>
#include <bl_cam_frame.h>

#include <bflb_core.h>



void bl_cam_frame_init(void)
{

}

#ifdef SIMU_DATA_TEST
#include "static_mjpeg.h"
#endif

#ifndef SIMU_DATA_TEST
#include "mjpeg.h"
#include "bflb_l1c.h"

static jpeg_frame_t jpeg_frame_info[4] = { 0 };
#endif

static int bl_get_jpeg_frame(uint8_t **c_ptr1, uint32_t *c_len1, uint8_t **c_ptr2, uint32_t *c_len2)
{
#ifndef SIMU_DATA_TEST
    int ret = 0;
    static uint32_t get_count = 0;
    uint32_t count = get_count & 0x03;

    ret = jpeg_frame_pop(&jpeg_frame_info[count], 1000);
    if (ret < 0) {
        printf("mjpeg_frame_pop timeout!\r\n");
        return -1;
    } else {
        // printf("get jpeg id: %d\r\n", jpeg_frame_info.frame_id);

        bflb_l1c_dcache_invalidate_range(jpeg_frame_info[count].frame_address, jpeg_frame_info[count].frame_size + 31);

        *c_ptr1 = (uint8_t *)(jpeg_frame_info[count].frame_address);
        *c_len1 = (uint32_t)(jpeg_frame_info[count].frame_size);
        *c_ptr2 = NULL;
        *c_len2 = 0;
        get_count++;
    }
#else

    *c_ptr1 = color_bar_test_jpg;
    *c_len1 = sizeof(color_bar_test_jpg);
    *c_ptr2 = NULL;
    *c_len2 = 0;

    vTaskDelay(30);

#endif

    return 0;
}



int bl_cam_frame_fifo_get(uint32_t *c_frames, uint8_t **c_ptr1, uint32_t *c_len1, uint8_t **c_ptr2, uint32_t *c_len2, int pic_mode)
{
    int ret = 0;

    *c_frames = 1;
    *c_ptr2 = NULL;
    *c_len2 = 0;
#ifdef CPU_D0
    switch (pic_mode) {
        case H264_MAIN:
            //ret = mm_get_h264_frame(c_ptr1, c_len1);
            //ret = sample_get_h264_frame(c_ptr1, c_len1, c_ptr2, c_len2, 0);
            ret = bl_get_h264_frame(c_ptr1, c_len1, c_ptr2, c_len2);
            break;
        case H264_SUB:
            ret = sample_get_h264_frame(c_ptr1, c_len1, c_ptr2, c_len2, 1);
            /*TODO add H264_SUB stream*/
            break;
        case MJPEG:
            ret = sample_get_jpeg_pic(c_ptr1, c_len1, 2);
            break;
        case RAW:
            sample_start_yuv_pic(1);
            ret = sample_get_raw_pic(c_ptr1, c_len1, 0);
            break;
        case YUV:
            ret = sample_get_yuv_pic(c_ptr1, c_len1, 1);
            break;
        default:
            break;
    }
#else
    switch (pic_mode) {
        // case MJPEG:
        case 2:
            ret = bl_get_jpeg_frame(c_ptr1, c_len1, c_ptr2, c_len2);
            break;
        default:
            ret = -1;
            //printf("wrong image channel\r\n");
            break;
    }
#endif
    return ret;
}

int bl_cam_frame_get_info(uint8_t *quality, uint16_t *width, uint16_t *height)
{
    *quality = 50;
    *width = 640;
    *height = 480;
    return 0;
}

int bl_cam_frame_config(uint8_t quality, uint16_t width, uint16_t height)
{
    return 0;
}

int bl_cam_frame_pop(void)
{
#ifndef SIMU_DATA_TEST

    static int pop_count = 0;
    jpeg_frame_free(&jpeg_frame_info[pop_count & 0x03]);
    pop_count++;

#endif
    return 0;
}

int bl_cam_frame_reset(void)
{
#ifndef SIMU_DATA_TEST

    for (uint16_t i = 0; i < 4; i++) {
        if (jpeg_frame_info[i].frame_address != NULL) {
            jpeg_frame_free(&jpeg_frame_info[i]);
        }
    }

#endif
    return 0;
}
