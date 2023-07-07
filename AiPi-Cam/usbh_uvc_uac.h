#ifndef USBH_UVC_UAC_H
#define USBH_UVC_UAC_H

#include "usbh_core.h"
#include "usbh_video.h"
#include "usbh_audio.h"

#define TEST_MJPEG         0

#define VIDEO_ISO_INTERVAL (2)
#define VIDEO_ISO_PACKETS  (8 * VIDEO_ISO_INTERVAL)
#define WIDTH              640
#define HEIGTH             480
#define ALTSETTING         6
#define VIDEO_EP_MPS       3072

#define MJPEG_MAX_BUFSIZE  (50 * 1024)

extern struct usbh_videostreaming g_uvcsteam;
extern volatile void *g_uvc_frame_buff;

extern void usbh_video_one_frame_callback_mjpeg(struct usbh_videostreaming *stream);
extern void usbh_video_one_frame_callback_yuyv(struct usbh_videostreaming *stream);

void usbh_video_fps_record(void);
void usbh_video_test(void);

#define AUDIO_SAMPLING_FREQ 16000
#define AUDIO_ISO_PACKETS   3
#define AUDIO_EP_MPS        512

extern volatile void *g_uac_frame_buff;

extern void usbh_audio_one_frame_callback(uint32_t data_size);

void usbh_audio_test(void);

#endif