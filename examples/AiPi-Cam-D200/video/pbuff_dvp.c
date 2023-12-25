#include "bflb_mtimer.h"
#include "bflb_i2c.h"
#include "bflb_cam.h"
#include "bflb_l1c.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>



#include "pbuff_dvp.h"
#include "mjpeg.h"

/***** frame ctrl *****/
static QueueHandle_t picture_frame_queue;
static QueueHandle_t picture_frame_pool_queue;


static volatile uint8_t dvp_pbuff_using_id = 0;

/* Forced write override, only for dvp cam, Work in the interrupt function */
static int pbuff_dvp_force_push(pbuff_yuyv_frame_t *dvp_frame)
{
    static uint64_t time_last;
    static uint16_t cnt = 0;
    pbuff_yuyv_frame_t pbuff_frame;

    if (uxQueueMessagesWaitingFromISR(picture_frame_queue) > PBUFF_QUEUES_NUM - 2) {
        if (picture_frame_pop(&pbuff_frame, 0) >= 0) {
            picture_frame_free(&pbuff_frame);
        }

        if (uxQueueMessagesWaitingFromISR(picture_frame_queue) > PBUFF_QUEUES_NUM - 2) {
            if (picture_frame_pop(&pbuff_frame, 0) >= 0) {
                picture_frame_free(&pbuff_frame);
            }
        }
    }

    picture_frame_alloc(&pbuff_frame, 0);

    /* Forced mode */
    picture_frame_push(dvp_frame);

    cnt++;
    if (cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        g_dvp_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        cnt = 0;
    }

    return 0;
}

/******************** stream frame ctrl ********************/

int picture_frame_init(void)
{
    printf("pbuff_init\r\n");

    /* create queue */
    picture_frame_queue = xQueueCreate(PBUFF_QUEUES_NUM, sizeof(pbuff_yuyv_frame_t));

    /* create pool queue */
    picture_frame_pool_queue = xQueueCreate(PBUFF_QUEUES_NUM, sizeof(pbuff_yuyv_frame_t));

    /* reset queue */
    picture_frame_reset();

    return 0;
}

/* Work in the interrupt function */
int picture_frame_alloc(pbuff_yuyv_frame_t *new_frame, uint32_t timeout)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueReceiveFromISR(picture_frame_pool_queue, new_frame, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    } else {
        ret = xQueueReceive(picture_frame_pool_queue, new_frame, timeout);
    }

    if (ret == pdTRUE) {
        new_frame->y_start = 0;
        new_frame->y_end = 480;
        return 0;
    } else {
        new_frame->pbuff_id = -1;
        new_frame->frame_address = NULL;
        return -1;
    }
}

/* Work in the interrupt function */
int picture_frame_push(pbuff_yuyv_frame_t *new_frame)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (new_frame->pbuff_id < 0) {
        printf("ERROR: pbuff push id invalid\r\n");
        return -1;
    }

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueSendFromISR(picture_frame_queue, new_frame, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    } else {
        ret = xQueueSend(picture_frame_queue, new_frame, 0);
    }

    if (ret == pdTRUE) {
        new_frame->pbuff_id = -1;
        new_frame->frame_address = NULL;
        return 0;
    } else {
        printf("ERROR: pbuff push queue full\r\n");
        return -2;
    }
}

/* Work in the task function */
int picture_frame_pop(pbuff_yuyv_frame_t *new_frame, uint32_t timeout)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueReceiveFromISR(picture_frame_queue, new_frame, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    } else {
        ret = xQueueReceive(picture_frame_queue, new_frame, timeout);
    }

    if (ret == pdTRUE) {
        return 0;
    } else {
        new_frame->pbuff_id = -1;
        new_frame->frame_address = NULL;
        return -1;
    }
}

/* Work in the task function */
int picture_frame_free(pbuff_yuyv_frame_t *new_frame)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (new_frame->pbuff_id < 0) {
        printf("ERROR: pbuff free id invalid");
        return -1;
    }

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueSendFromISR(picture_frame_pool_queue, new_frame, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    } else {
        ret = xQueueSend(picture_frame_pool_queue, new_frame, 0);
    }

    if (ret == pdTRUE) {
        new_frame->pbuff_id = -1;
        new_frame->frame_address = NULL;
        return 0;
    } else {
        printf("ERROR: pbuff free queue timeout\r\n");
        return -1;
    }
}

int picture_frame_reset(void)
{
    pbuff_yuyv_frame_t pbuff_frame;

    /* invalid dcache */
#if (PBUFF_SHARE_EN)
    bflb_l1c_dcache_invalidate_range(picture_buffer, 1 * PBUFF_DATA_SIZE);

#else
    bflb_l1c_dcache_invalidate_range(picture_buffer, PBUFF_QUEUES_NUM * PBUFF_DATA_SIZE);
#endif
    xQueueReset(picture_frame_queue);
    xQueueReset(picture_frame_pool_queue);

    /* send to pools queue */
    for (uint16_t i = 0; i < PBUFF_QUEUES_NUM; i++) {
#if (PBUFF_SHARE_EN)
        pbuff_frame.frame_address = picture_buffer[0];
        pbuff_frame.pbuff_id = 0;
#else
        pbuff_frame.frame_address = picture_buffer[i];
        pbuff_frame.pbuff_id = i;
#endif
        pbuff_frame.y_start = 0;
        pbuff_frame.y_end = 480;
        xQueueSend(picture_frame_pool_queue, &pbuff_frame, portMAX_DELAY);
    }

    return 0;
}
