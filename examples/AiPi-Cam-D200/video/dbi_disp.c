#include "bflb_l1c.h"
#include "bflb_mtimer.h"
#include "bflb_gpio.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>

#include "dbi_disp.h"
#include "pbuff_dvp.h"
#include "mjpeg.h"

extern volatile uint32_t g_uvc_fps;

volatile uint32_t g_dbi_disp_fps = 0;

static TaskHandle_t dbi_disp_free_handle;

static EventGroupHandle_t disp_event_group = NULL;

static void dbi_disp_free_task(void *pvParameters)
{
    pbuff_yuyv_frame_t disp_src_frame;

    xEventGroupSetBits(disp_event_group, DISP_EVENT_FREE_QUEUE);

    while (1) {
        xEventGroupWaitBits(disp_event_group, DISP_EVENT_FREE_QUEUE, pdFALSE, pdTRUE, portMAX_DELAY);
        /* Drop the all frame */
        picture_frame_pop(&disp_src_frame, portMAX_DELAY);
        vTaskDelay(20);
        picture_frame_free(&disp_src_frame);
    }
}

void dbi_disp_free_init(void)
{
    if (disp_event_group == NULL) {
        disp_event_group = xEventGroupCreate();
    }

    printf("dbi_disp_free_init\r\n");

    xTaskCreate(dbi_disp_free_task, (char *)"disp_lcd_task", 256, NULL, 3, &dbi_disp_free_handle);
}