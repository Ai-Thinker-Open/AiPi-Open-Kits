#include "bflb_mtimer.h"
#include "bflb_mjpeg.h"
#include "bflb_cam.h"
#include "bflb_l1c.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>

#include "board.h"
#include "image_sensor.h"

#include "mjpeg.h"
#include "jpeg_head.h"

#include "pbuff_dvp.h"

#if (0)
#define DVP_JPEG_DBG(a, ...) printf("[DVP_JPEG dbg]:" a, ##__VA_ARGS__)
#else
#define DVP_JPEG_DBG(a, ...)
#endif

#if (1)
#define DVP_JPEG_INFO(a, ...) printf("[DVP_JPEG info]:" a, ##__VA_ARGS__)
#else
#define DVP_JPEG_INFO(a, ...)
#endif

#define DVP_JPEG_ERR(a, ...)  printf("[DVP_JPEG err]:" a, ##__VA_ARGS__)


/***** frame ctrl *****/
static QueueHandle_t jpeg_frame_queue;
static QueueHandle_t jpeg_frame_pool_queue;
static uint8_t __attribute__((section(".psram_data"), aligned(32))) jpeg_buffer[MJPEG_QUEUES_NUM][MJPEG_MAX_SIZE]; /* Multi buffer queue */

#if (PBUFF_SHARE_EN)
uint8_t __attribute__((section(".psram_data"), aligned(32))) picture_buffer[1][PBUFF_DATA_SIZE]; /* YUYV raw data */
#else
uint8_t __attribute__((section(".psram_data"), aligned(32))) picture_buffer[PBUFF_QUEUES_NUM][PBUFF_DATA_SIZE]; /* YUYV raw data */
#endif
/***** mjpeg ctrl *****/
volatile uint32_t g_mjpeg_fps = 0;

static struct bflb_device_s* mjpeg;
static struct bflb_mjpeg_config_s config;

static volatile jpeg_frame_t mjpeg_compress_frame = {
    .frame_address = NULL,
    .frame_id = -1,
};


/* cam ctrl */
static TaskHandle_t dvp_task_handle;

volatile uint32_t g_dvp_fps = 0;

static struct bflb_device_s* i2c0;
static struct bflb_device_s* cam0;

static struct bflb_cam_config_s cam_config;
static struct image_sensor_config_s* sensor_config;
TaskHandle_t dvp_mjpeg_task_hd;

#if 0
void mjpeg_dump_hex(uint8_t* data, uint32_t len)
{
    uint32_t i = 0;

    for (i = 0; i < len; i++) {
        if (i % 16 == 0) {
            printf("\r\n");
        }

        printf("%02x ", data[i]);
    }

    printf("\r\n");
}
#endif
static void cam_isr(int irq, void* arg)
{
    static uint64_t time_last = 0;
    static uint16_t cnt = 0;
    pbuff_yuyv_frame_t dvp_frame;
    uint16_t i;

    uint8_t* buff_using;

    if (time_last == 0) {
        time_last = bflb_mtimer_get_time_us();
    }

    /* get buff */
    // bflb_cam_get_frame_info(cam0, &buff_using);
    // bflb_cam_pop_one_frame(cam0);
    bflb_cam_stop(cam0);
    bflb_cam_int_clear(cam0, CAM_INTCLR_NORMAL);
    DVP_JPEG_DBG("cam_isr\r\n");

    /* update dvp fps */
    cnt++;
    if (cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        g_dvp_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        cnt = 0;
    }

    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(dvp_mjpeg_task_hd, DVP_ISR_NOTIFY_INDEX, &pxHigherPriorityTaskWoken);
    if (pxHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
    }
}
static void mjpeg_isr(int irq, void* arg)
{
    // uint8_t *pic;
    // uint32_t len;

    uint32_t intstatus = bflb_mjpeg_get_intstatus(mjpeg);

    // dvp_cam_stop();
    bflb_mjpeg_stop(mjpeg);
    DVP_JPEG_DBG("mjpeg_isr\r\n");

    if (intstatus & MJPEG_INTSTS_ONE_FRAME) {
        // len = bflb_mjpeg_get_frame_info(mjpeg, &pic);
        // bflb_mjpeg_pop_one_frame(mjpeg);
        bflb_mjpeg_int_clear(mjpeg, MJPEG_INTCLR_ONE_FRAME);
        /* clear over size isr */
        bflb_mjpeg_int_clear(mjpeg, 1 << 10);

        // if (mjpeg_compress_frame.frame_address == NULL) {
        //     printf("ERROR: mjpeg no frame\r\n");
        //     return;
        // }

        // if ((void *)pic != (void *)(mjpeg_compress_frame.frame_address)) {
        //     printf("ERROR: mjpeg output address error! 0x%08X -> 0x%08X \r\n", (uint32_t)pic, (uint32_t)mjpeg_compress_frame.frame_address);
        //     return;
        // }

        // if (len > MJPEG_MAX_SIZE) {
        //     /* frame size over, free the buff */
        //     jpeg_frame_free((jpeg_frame_t *)&mjpeg_compress_frame);
        //     printf("ERROR: mjpeg frame size over: %d > %d\r\n", len, MJPEG_MAX_SIZE);

        // } else {
        //     mjpeg_compress_frame.frame_size = len;
        //     jpeg_frame_push((jpeg_frame_t *)&mjpeg_compress_frame);
        //     printf("frame_size:%d\r\n", len);
        // }
        // printf("mjpeg_compress_frame_push:%d\r\n", mjpeg_compress_frame_push(picture_buffer[0]));
        /* notify to task */
        BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyIndexedFromISR(dvp_mjpeg_task_hd, JPEG_ISR_NOTIFY_INDEX, JPEG_FRAME_COMP, eSetValueWithOverwrite, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }
    }
    else if (intstatus & (1 << 6)) {
        /* over size error */
        /* clear over size isr */
        bflb_mjpeg_int_clear(mjpeg, 1 << 10);

        /* notify to task */
        BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyIndexedFromISR(dvp_mjpeg_task_hd, JPEG_ISR_NOTIFY_INDEX, JPEG_OVER_SIZE, eSetValueWithOverwrite, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }
    }
}


void dvp_mjpeg_task(void* pvParameters)
{
    int ret;
    uint64_t time_last;
    uint16_t cnt = 0;

    // static jpeg_frame_t mjpeg_compress_frame;

    uint8_t* pic;
    uint32_t len;

    vTaskDelay(1000);

    /* clear */
    // ulTaskNotifyTake(pdTRUE, 0);

    time_last = bflb_mtimer_get_time_us();

    while (1) {
        vTaskDelay(50);
        /* allco frame buff */
        ret = mjpeg_compress_frame_push(picture_buffer[0]);
        if (ret < 0) {
            printf("jpeg buff alloc timeout %d, continue wait... \r\n", ret);
            continue;
        }

        /* waiting dvp isr */
        ulTaskNotifyTakeIndexed(DVP_ISR_NOTIFY_INDEX, pdTRUE, portMAX_DELAY);
        DVP_JPEG_DBG("DVP_ISR_NOTIFY\r\n");

        /* waiting jpeg isr */
        int mjpeg_status = ulTaskNotifyTakeIndexed(JPEG_ISR_NOTIFY_INDEX, pdTRUE, portMAX_DELAY);
        DVP_JPEG_DBG("JPEG_ISR_NOTIFY:%d\r\n", mjpeg_status);
        if (mjpeg_status == JPEG_FRAME_COMP) {
            len = bflb_mjpeg_get_frame_info(mjpeg, &pic);
            bflb_mjpeg_pop_one_frame(mjpeg);

            if (mjpeg_compress_frame.frame_address == NULL) {
                printf("ERROR: mjpeg no frame\r\n");
                return;
            }

            if ((void*)pic != (void*)(mjpeg_compress_frame.frame_address)) {
                printf("ERROR: mjpeg output address error! 0x%08X -> 0x%08X \r\n", (uint32_t)pic, (uint32_t)mjpeg_compress_frame.frame_address);
                return;
            }

            if (len > MJPEG_MAX_SIZE) {
                /* frame size over, free the buff */
                jpeg_frame_free((jpeg_frame_t*)&mjpeg_compress_frame);
                printf("ERROR: mjpeg frame size over: %d > %d\r\n", len, MJPEG_MAX_SIZE);

            }
            else {
                mjpeg_compress_frame.frame_size = len;
                jpeg_frame_push((jpeg_frame_t*)&mjpeg_compress_frame);
                DVP_JPEG_DBG("frame_size:%d\r\n", len);
            }
        }
        else {
            jpeg_frame_free((jpeg_frame_t*)&mjpeg_compress_frame);
            if (mjpeg_status == JPEG_OVER_SIZE) {
                printf("mjpeg over size, frame drop\r\n");
            }
            else {
                printf("mjpeg unkown error %d\r\n", mjpeg_status);
            }
        }
    }

}


int dvp_cam_init(void)
{
    /* dvp init */
    board_dvp_gpio_init();

    i2c0 = bflb_device_get_by_name("i2c0");
    cam0 = bflb_device_get_by_name("cam0");

    /* image_sensor init */
    if (image_sensor_scan(i2c0, &sensor_config)) {
        DVP_JPEG_INFO("Sensor name: %s\r\n", sensor_config->name);
    }
    else {
        DVP_JPEG_ERR("Error! Can't identify sensor!\r\n");
        return -1;
    }

    memcpy(&cam_config, sensor_config, IMAGE_SENSOR_INFO_COPY_SIZE);
    cam_config.with_mjpeg = true;
    cam_config.input_source = CAM_INPUT_SOURCE_DVP;
    cam_config.output_format = CAM_OUTPUT_FORMAT_AUTO;
    cam_config.output_bufaddr = (uint32_t)picture_buffer[0];
#if (PBUFF_SHARE_EN)
    cam_config.output_bufsize = cam_config.resolution_x * 2 * 16;
#else
    cam_config.output_bufsize = PBUFF_DATA_SIZE * PBUFF_QUEUES_NUM;
#endif

    DVP_JPEG_INFO("dvp x: %d\r\n", cam_config.resolution_x);
    DVP_JPEG_INFO("dvp y: %d\r\n", cam_config.resolution_y);

    // if (cam_config.resolution_x > DVP_JPEG_W) {
    //     DVP_JPEG_ERR("dvp line_buff size over: dvp:%d, buff:%d\r\n", cam_config.resolution_x, DVP_JPEG_W);
    //     return -2;
    // }

    bflb_cam_init(cam0, &cam_config);
    // bflb_cam_start(cam0);

    bflb_cam_int_mask(cam0, CAM_INTMASK_NORMAL, false);
    bflb_irq_attach(cam0->irq_num, cam_isr, NULL);
    bflb_irq_enable(cam0->irq_num);

    return 0;
}

void mjpeg_init(uint8_t quality)
{
    uint8_t jpg_head_buf[800] = { 0 };
    uint32_t jpg_head_len;

    mjpeg = bflb_device_get_by_name("mjpeg");

    config.format = MJPEG_FORMAT_YUV422_YUYV;
    config.quality = quality;
    config.rows = 16;
    config.resolution_x = cam_config.resolution_x;
    config.resolution_y = cam_config.resolution_y;
    config.input_bufaddr0 = (uint32_t)picture_buffer[0];
    config.input_bufaddr1 = 0;
    config.output_bufaddr = (uint32_t)jpeg_buffer[0];
    config.output_bufsize = MJPEG_MAX_SIZE;
    config.input_yy_table = NULL; /* use default table */
    config.input_uv_table = NULL; /* use default table */

    bflb_mjpeg_init(mjpeg, &config);
    jpg_head_len = JpegHeadCreate(YUV_MODE_422, quality, cam_config.resolution_x, cam_config.resolution_y, jpg_head_buf);
    bflb_mjpeg_fill_jpeg_header_tail(mjpeg, jpg_head_buf, jpg_head_len);

    // bflb_mjpeg_tcint_mask(mjpeg, false);
    // bflb_irq_attach(mjpeg->irq_num, mjpeg_isr, NULL);
    // bflb_irq_enable(mjpeg->irq_num);
    uint32_t reg_temp = getreg32(mjpeg->reg_base + 0x00);
    putreg32((reg_temp | (1 << 3)), (mjpeg->reg_base + 0x00)); /* enable over_size int */

    bflb_mjpeg_tcint_mask(mjpeg, false);
    reg_temp = getreg32(mjpeg->reg_base + 0x1C);
    putreg32((reg_temp | (1 << 2)), (mjpeg->reg_base + 0x1C)); /* enable over_size int */
    bflb_irq_attach(mjpeg->irq_num, mjpeg_isr, NULL);
    bflb_irq_enable(mjpeg->irq_num);

    // printf("mjpeg_init:%d\r\n", mjpeg_compress_frame_push(picture_buffer[0]));
    // bflb_mjpeg_start(mjpeg);

    xTaskCreate(dvp_mjpeg_task, (char*)"dvp_mjpeg_task", 512, NULL, 19, &dvp_mjpeg_task_hd);
}

int mjpeg_compress_frame_push(void* yuv_src)
{
    static uint64_t time_last;
    static uint16_t cnt = 0;

    if (mjpeg_compress_frame.frame_address != NULL) {
        /* mjpeg is busy */
        return -1;
    }

    /* allco frame buff */
    if (jpeg_frame_alloc((jpeg_frame_t*)&mjpeg_compress_frame, 0) < 0) {
        /* no buff */
        return -2;
    }

    // printf("mjpeg start, id %d, address in 0x%08X, out 0x%08X\r\n", mjpeg_compress_frame.frame_id, (uint32_t)yuv_src, (uint32_t)mjpeg_compress_frame.frame_address);

    /* start mjpeg compress */
    // bflb_mjpeg_stop(mjpeg);
    bflb_mjpeg_update_input_output_buff(mjpeg, yuv_src, NULL, mjpeg_compress_frame.frame_address, MJPEG_MAX_SIZE);
    // bflb_mjpeg_sw_run(mjpeg, 1);
    bflb_mjpeg_start(mjpeg);
    bflb_cam_start(cam0);

    cnt++;
    if (cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        // g_mjpeg_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        cnt = 0;
    }
    g_mjpeg_fps++;

    return 0;
}


/******************** stream frame ctrl ********************/

int jpeg_frame_init(void)
{
    printf("jpeg_frame_init\r\n");

    /* create queue */
    jpeg_frame_queue = xQueueCreate(MJPEG_QUEUES_NUM, sizeof(jpeg_frame_t));

    /* create pool queue */
    jpeg_frame_pool_queue = xQueueCreate(MJPEG_QUEUES_NUM, sizeof(jpeg_frame_t));

    /* reset queue */
    jpeg_frame_reset();

    return 0;
}

int jpeg_frame_alloc(jpeg_frame_t* jpeg_frame_info, uint32_t timeout)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueReceiveFromISR(jpeg_frame_pool_queue, jpeg_frame_info, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    }
    else {
        ret = xQueueReceive(jpeg_frame_pool_queue, jpeg_frame_info, timeout);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_size = MJPEG_MAX_SIZE;
        return 0;
    }
    else {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return -1;
    }
}

int jpeg_frame_push(jpeg_frame_t* jpeg_frame_info)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (jpeg_frame_info->frame_id < 0) {
        printf("ERROR: jpeg push id invalid\r\n");
        return -1;
    }

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueSendFromISR(jpeg_frame_queue, jpeg_frame_info, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    }
    else {
        ret = xQueueSend(jpeg_frame_queue, jpeg_frame_info, 0);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return 0;
    }
    else {
        printf("ERROR: jpeg push queue full\r\n");
        return -2;
    }
}

int jpeg_frame_pop(jpeg_frame_t* jpeg_frame_info, uint32_t timeout)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueReceiveFromISR(jpeg_frame_queue, jpeg_frame_info, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    }
    else {
        ret = xQueueReceive(jpeg_frame_queue, jpeg_frame_info, timeout);
    }

    if (ret == pdTRUE) {
        return 0;
    }
    else {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return -1;
    }
}

int jpeg_frame_free(jpeg_frame_t* jpeg_frame_info)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if (jpeg_frame_info->frame_id < 0) {
        printf("ERROR: jpeg free id invalid\r\n");
        return -1;
    }

    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueSendFromISR(jpeg_frame_pool_queue, jpeg_frame_info, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    }
    else {
        ret = xQueueSend(jpeg_frame_pool_queue, jpeg_frame_info, 0);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return 0;
    }
    else {
        printf("ERROR: jpeg free queue timeout\r\n");
        return -1;
    }
}

int jpeg_frame_reset(void)
{
    jpeg_frame_t jpeg_frame;

    /* invalid dcache */
    bflb_l1c_dcache_invalidate_range(jpeg_buffer, MJPEG_QUEUES_NUM * MJPEG_MAX_SIZE);

    xQueueReset(jpeg_frame_queue);
    xQueueReset(jpeg_frame_pool_queue);

    for (uint16_t i = 0; i < MJPEG_QUEUES_NUM; i++) {
        jpeg_frame.frame_address = jpeg_buffer[i];
        jpeg_frame.frame_id = i;
        jpeg_frame.frame_size = MJPEG_MAX_SIZE;
        xQueueSend(jpeg_frame_pool_queue, &jpeg_frame, portMAX_DELAY);
    }

    return 0;
}
