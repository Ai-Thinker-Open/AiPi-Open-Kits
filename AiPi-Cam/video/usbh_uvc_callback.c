#include "usbh_uvc_uac.h"

#include "bflb_mtimer.h"

volatile uint32_t g_uvc_fps;

void usbh_video_fps_calcul()
{
    static uint64_t time_last;
    static uint16_t fps_cnt;

    fps_cnt++;
    if (fps_cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        g_uvc_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        fps_cnt = 0;
    }
}

#if defined(TEST_MJPEG) && (TEST_MJPEG == 1)

#include "mjpeg.h"
void usbh_video_one_frame_callback_mjpeg(struct usbh_videostreaming *stream)
{
    static jpeg_frame_t jpeg_frame = { 0 };

    if (g_uvc_frame_buff != NULL) {
        /* check */
        if (g_uvc_frame_buff != jpeg_frame.frame_address) {
            printf("uvc unkown pbuff\r\n");
            while (1) {
            };
        }

        /* jpeg frame push */
        jpeg_frame.frame_size = stream->bufoffset;
        if (stream->bufoffset && stream->bufoffset <= jpeg_frame.frame_size) {
            jpeg_frame.frame_size = stream->bufoffset;
            jpeg_frame_push(&jpeg_frame);
        } else {
            // jpeg_frame_free(&jpeg_frame);
            printf("uvc jpeg size over: %d -> %d\r\n", jpeg_frame.frame_size, stream->bufoffset);
            while (1) {
            };
        }
    }

    /* alloc new jpeg frame */
    if (jpeg_frame_alloc(&jpeg_frame, 0) < 0) {
        g_uvc_frame_buff = NULL;
    } else {
        g_uvc_frame_buff = jpeg_frame.frame_address;
    }

    if (g_uvc_frame_buff) {
        usbh_video_fps_calcul();
    }
}

#else

#include "pbuff_dvp.h"
#include "mjpeg.h"

void usbh_video_one_frame_callback_yuyv(struct usbh_videostreaming *stream)
{
    static pbuff_yuyv_frame_t pbuff_frame = {
        .frame_address = NULL,
        .pbuff_id = -1,
    };

    if (g_uvc_frame_buff != NULL) {
        if (g_uvc_frame_buff != pbuff_frame.frame_address) {
            printf("uvc unkown pbuff\r\n");
            while (1) {
            };
        }

        /* mjpeg compress push */
        mjpeg_compress_frame_push((void *)g_uvc_frame_buff);

        /* picture push */
        pbuff_frame.x_start = 0;
        pbuff_frame.y_start = 0;
        pbuff_frame.x_end = WIDTH - 1;
        pbuff_frame.y_end = HEIGTH - 1;

        picture_frame_push(&pbuff_frame);
    }

    /* alloc new frame */
    if (picture_frame_alloc(&pbuff_frame, 0) < 0) {
        g_uvc_frame_buff = NULL;
    } else {
        g_uvc_frame_buff = pbuff_frame.frame_address;
    }

    if (g_uvc_frame_buff) {
        usbh_video_fps_calcul();
    }
}

#endif