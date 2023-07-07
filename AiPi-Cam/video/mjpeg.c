#include "bflb_mtimer.h"
#include "bflb_mjpeg.h"
#include "bflb_l1c.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>

#include "mjpeg.h"
#include "jpeg_head.h"

/***** frame ctrl *****/
static QueueHandle_t jpeg_frame_queue;
static QueueHandle_t jpeg_frame_pool_queue;
static uint8_t __attribute__((section(".psram_data"), aligned(32))) jpeg_buffer[MJPEG_QUEUES_NUM][MJPEG_MAX_SIZE]; /* Multi buffer queue */

/***** mjpeg ctrl *****/
volatile uint32_t g_mjpeg_fps = 0;

static struct bflb_device_s *mjpeg;
static struct bflb_mjpeg_config_s config;

static volatile jpeg_frame_t mjpeg_compress_frame = {
    .frame_address = NULL,
    .frame_id = -1,
};

#if 0
void mjpeg_dump_hex(uint8_t *data, uint32_t len)
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

static void mjpeg_isr(int irq, void *arg)
{
    uint8_t *pic;
    uint32_t len;

    uint32_t intstatus = bflb_mjpeg_get_intstatus(mjpeg);

    if (intstatus & MJPEG_INTSTS_ONE_FRAME) {
        len = bflb_mjpeg_get_frame_info(mjpeg, &pic);
        bflb_mjpeg_pop_one_frame(mjpeg);
        bflb_mjpeg_int_clear(mjpeg, MJPEG_INTCLR_ONE_FRAME);

        if (mjpeg_compress_frame.frame_address == NULL) {
            printf("ERROR: mjpeg no frame\r\n");
            return;
        }

        if ((void *)pic != (void *)(mjpeg_compress_frame.frame_address)) {
            printf("ERROR: mjpeg output address error! 0x%08X -> 0x%08X \r\n", (uint32_t)pic, (uint32_t)mjpeg_compress_frame.frame_address);
            return;
        }

        if (len > MJPEG_MAX_SIZE) {
            /* frame size over, free the buff */
            jpeg_frame_free((jpeg_frame_t *)&mjpeg_compress_frame);
            printf("ERROR: mjpeg frame size over: %d > %d\r\n", len, MJPEG_MAX_SIZE);

        } else {
            mjpeg_compress_frame.frame_size = len;
            jpeg_frame_push((jpeg_frame_t *)&mjpeg_compress_frame);
            // printf("frame_size:%d\r\n", len);
        }
    }
}

void mjpeg_init(uint32_t x, uint32_t y, uint8_t quality)
{
    uint8_t jpg_head_buf[800] = { 0 };
    uint32_t jpg_head_len;

    printf("mjpeg_init\r\n");

    mjpeg = bflb_device_get_by_name("mjpeg");

    config.format = MJPEG_FORMAT_YUV422_YUYV;
    config.quality = quality;
    config.resolution_x = x;
    config.resolution_y = y;
    config.input_bufaddr0 = (uint32_t)NULL;
    config.input_bufaddr1 = 0;
    config.output_bufaddr = (uint32_t)jpeg_buffer[0];
    config.output_bufsize = MJPEG_MAX_SIZE;
    config.input_yy_table = NULL; /* use default table */
    config.input_uv_table = NULL; /* use default table */

    bflb_mjpeg_init(mjpeg, &config);
    jpg_head_len = JpegHeadCreate(YUV_MODE_422, quality, x, y, jpg_head_buf);
    bflb_mjpeg_fill_jpeg_header_tail(mjpeg, jpg_head_buf, jpg_head_len);

    bflb_mjpeg_tcint_mask(mjpeg, false);
    bflb_irq_attach(mjpeg->irq_num, mjpeg_isr, NULL);
    bflb_irq_enable(mjpeg->irq_num);
}

int mjpeg_compress_frame_push(void *yuv_src)
{
    static uint64_t time_last;
    static uint16_t cnt = 0;

    if (mjpeg_compress_frame.frame_address != NULL) {
        /* mjpeg is busy */
        return -1;
    }

    /* allco frame buff */
    if (jpeg_frame_alloc((jpeg_frame_t *)&mjpeg_compress_frame, 0) < 0) {
        /* no buff */
        return -2;
    }

    // printf("mjpeg start, id %d, address in 0x%08X, out 0x%08X\r\n", mjpeg_compress_frame.frame_id, (uint32_t)yuv_src, (uint32_t)mjpeg_compress_frame.frame_address);

    /* start mjpeg compress */
    bflb_mjpeg_stop(mjpeg);
    bflb_mjpeg_update_input_output_buff(mjpeg, yuv_src, NULL, mjpeg_compress_frame.frame_address, MJPEG_MAX_SIZE);
    bflb_mjpeg_sw_run(mjpeg, 1);

    cnt++;
    if (cnt >= 10) {
        uint64_t time = bflb_mtimer_get_time_us();
        g_mjpeg_fps = (1000 * 1000 * 10) / (uint32_t)(time - time_last);
        time_last = time;
        cnt = 0;
    }

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

int jpeg_frame_alloc(jpeg_frame_t *jpeg_frame_info, uint32_t timeout)
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

    } else {
        ret = xQueueReceive(jpeg_frame_pool_queue, jpeg_frame_info, timeout);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_size = MJPEG_MAX_SIZE;
        return 0;
    } else {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return -1;
    }
}

int jpeg_frame_push(jpeg_frame_t *jpeg_frame_info)
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

    } else {
        ret = xQueueSend(jpeg_frame_queue, jpeg_frame_info, 0);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return 0;
    } else {
        printf("ERROR: jpeg push queue full\r\n");
        return -2;
    }
}

int jpeg_frame_pop(jpeg_frame_t *jpeg_frame_info, uint32_t timeout)
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

    } else {
        ret = xQueueReceive(jpeg_frame_queue, jpeg_frame_info, timeout);
    }

    if (ret == pdTRUE) {
        return 0;
    } else {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return -1;
    }
}

int jpeg_frame_free(jpeg_frame_t *jpeg_frame_info)
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

    } else {
        ret = xQueueSend(jpeg_frame_pool_queue, jpeg_frame_info, 0);
    }

    if (ret == pdTRUE) {
        jpeg_frame_info->frame_id = -1;
        jpeg_frame_info->frame_address = NULL;
        return 0;
    } else {
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
