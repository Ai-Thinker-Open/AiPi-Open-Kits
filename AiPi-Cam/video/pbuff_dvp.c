#include "bflb_mtimer.h"
#include "bflb_i2c.h"
#include "bflb_cam.h"
#include "bflb_l1c.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>

#include "board.h"
#include "image_sensor.h"

#include "pbuff_dvp.h"
#include "mjpeg.h"

/***** frame ctrl *****/
static QueueHandle_t picture_frame_queue;
static QueueHandle_t picture_frame_pool_queue;

#if (PBUFF_SHARE_EN)
static uint8_t __attribute__((section(".psram_data"), aligned(32))) picture_buffer[1][PBUFF_DATA_SIZE]; /* YUYV raw data */
#else
static uint8_t __attribute__((section(".psram_data"), aligned(32))) picture_buffer[PBUFF_QUEUES_NUM][PBUFF_DATA_SIZE]; /* YUYV raw data */
#endif

/* cam ctrl */
static TaskHandle_t dvp_task_handle;

volatile uint32_t g_dvp_fps = 0;

static struct bflb_device_s *i2c0;
static struct bflb_device_s *cam0;

static struct bflb_cam_config_s cam_config;
static struct image_sensor_config_s *sensor_config;

static volatile uint8_t dvp_pbuff_using_id = 0;

static int pbuff_dvp_force_push(pbuff_yuyv_frame_t *dvp_frame);

static void cam_isr(int irq, void *arg)
{
    static uint64_t time_last = 0;
    static uint16_t cnt = 0;
    pbuff_yuyv_frame_t dvp_frame;
    uint16_t i;

    uint8_t *buff_using;

    if (time_last == 0) {
        time_last = bflb_mtimer_get_time_us();
    }

    /* get buff */
    bflb_cam_get_frame_info(cam0, &buff_using);
    bflb_cam_pop_one_frame(cam0);
    bflb_cam_int_clear(cam0, CAM_INTCLR_NORMAL);

    for (i = 0; i < PBUFF_QUEUES_NUM; i++) {
        if (buff_using == picture_buffer[i]) {
            dvp_pbuff_using_id = i;
            break;
        }
    }

    /* yuyv frame push */
    dvp_frame.frame_address = picture_buffer[dvp_pbuff_using_id];
    dvp_frame.pbuff_id = dvp_pbuff_using_id;
    dvp_frame.x_start = 0;
    dvp_frame.y_start = 0;
    dvp_frame.x_end = 640 - 1;
    dvp_frame.y_end = 480 - 1;
    pbuff_dvp_force_push(&dvp_frame);

    /* mjpeg compress */
    int ret = mjpeg_compress_frame_push(picture_buffer[dvp_pbuff_using_id]);
    // printf("cam_isr\r\nmjpeg_frame_push:%d\r\n", ret);

    /* */
    dvp_pbuff_using_id++;
    dvp_pbuff_using_id %= PBUFF_QUEUES_NUM;

    /* update dvp fps */
    cnt++;
    if (cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        g_dvp_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        cnt = 0;
    }
}

static void dvp_start_task(void *pvParameters)
{
    vTaskDelay(1000);

    /* dvp cam start */
    bflb_cam_start(cam0);

    printf("dvp start\r\n");

    /* Delete task */
    vTaskDelete(NULL);
}

int dvp_cam_init(void)
{
    printf("dvp_init\r\n");

    board_dvp_gpio_init();

    i2c0 = bflb_device_get_by_name("i2c0");
    cam0 = bflb_device_get_by_name("cam0");

    /* image_sensor init */
    if (image_sensor_scan(i2c0, &sensor_config)) {
        printf("\r\nSensor name: %s\r\n", sensor_config->name);
    } else {
        printf("\r\nError! Can't identify sensor!\r\n");
        return -1;
    }

    memcpy(&cam_config, sensor_config, IMAGE_SENSOR_INFO_COPY_SIZE);
    cam_config.with_mjpeg = false;
    cam_config.input_source = CAM_INPUT_SOURCE_DVP;
    cam_config.output_format = CAM_OUTPUT_FORMAT_AUTO;
    cam_config.output_bufaddr = (uint32_t)picture_buffer[0];
#if (PBUFF_SHARE_EN)
    cam_config.output_bufsize = PBUFF_DATA_SIZE;
#else
    cam_config.output_bufsize = PBUFF_DATA_SIZE * PBUFF_QUEUES_NUM;
#endif

    bflb_cam_init(cam0, &cam_config);

    bflb_cam_int_mask(cam0, CAM_INTMASK_NORMAL, false);
    bflb_irq_attach(cam0->irq_num, cam_isr, NULL);
    bflb_irq_enable(cam0->irq_num);

    /* camer task */
    xTaskCreate(dvp_start_task, (char *)"dvp_start_task", 1024, NULL, 5, &dvp_task_handle);

    return 0;
}

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
