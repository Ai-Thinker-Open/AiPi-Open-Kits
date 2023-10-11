/**
 * @file        audio_record_play.c
 * @brief
 * @author      linqiwei
 * @version     v1.0.0
 * @date        2023-09-07
 * 
 * @copyright   Copyright (c) 2020-2023 Shenzhen Ai-Thinker Technology Co., Ltd. All Rights Reserved.
 * 
 * Disclaimer: This program is for informational purposes only, please keep author information and source code license.
 * 
 */

#include "bflb_auadc.h"
#include "bflb_audac.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"

#include "bl616_glb.h"
#include "board.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#define AUADC_SAMPLING_NUM (8*1024)
#define AUDIO_QUEUES_NUM 4

#define audio_play_STACK_SIZE  (2*1024)
#define baudio_play_PRIORITY (11)

typedef struct{
    uint8_t frame_data[AUADC_SAMPLING_NUM];
    uint32_t frame_size;
    int16_t frame_id;
} audio_stream_t;

static ATTR_NOCACHE_NOINIT_RAM_SECTION audio_stream_t record_buf;
static ATTR_NOCACHE_NOINIT_RAM_SECTION audio_stream_t play_buf;

/* audio adc config */
struct bflb_auadc_init_config_s auadc_init_cfg = {
    .sampling_rate = AUDAC_SAMPLING_RATE_16K,
    .input_mode = AUADC_INPUT_MODE_ADC,
    .data_format = AUADC_DATA_FORMAT_16BIT,
    .fifo_threshold = 3,
};

/* audio adc analog config */
struct bflb_auadc_adc_init_config_s auadc_analog_init_cfg = {
    .auadc_analog_en = true,
    .adc_mode = AUADC_ADC_MODE_AUDIO,
    .adc_pga_mode = AUADC_ADC_PGA_MODE_AC_SINGLE,
    .adc_pga_posi_ch = AUADC_ADC_ANALOG_CH_3,
    .adc_pga_nega_ch = AUADC_ADC_ANALOG_CH_3,
    .adc_pga_gain = 20,
};

struct bflb_audac_init_config_s audac_init_cfg = {
    .sampling_rate = AUDAC_SAMPLING_RATE_8K, //AUDAC_SAMPLING_RATE_32K,
    .output_mode = AUDAC_OUTPUT_MODE_PWM,
    .source_channels_num = AUDAC_SOURCE_CHANNEL_DUAL,
    .mixer_mode = AUDAC_MIXER_MODE_ONLY_L,
    .data_format = AUDAC_DATA_FORMAT_16BIT,
    .fifo_threshold = 7,
};

struct bflb_audac_volume_config_s audac_volume_cfg = {
    .mute_ramp_en = true,
    .mute_up_ramp_rate = AUDAC_RAMP_RATE_FS_32,
    .mute_down_ramp_rate = AUDAC_RAMP_RATE_FS_8,
    .volume_update_mode = AUDAC_VOLUME_UPDATE_MODE_RAMP,
    .volume_ramp_rate = AUDAC_RAMP_RATE_FS_128,
    .volume_zero_cross_timeout = AUDAC_RAMP_RATE_FS_128,
};


struct bflb_device_s *auadc_hd;
struct bflb_device_s *auadc_dma_hd;
struct bflb_device_s *audac_hd;
struct bflb_device_s *auadc_dma_tx;
struct bflb_device_s *gpio;

static QueueHandle_t audio_stream_queue;
static TaskHandle_t audio_play_task_hd;

static uint8_t audio_play_status = 0;

static int audio_stream_push(audio_stream_t *audio_stream_info)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;


    if (xPortIsInsideInterrupt()) {
        uintptr_t flag = bflb_irq_save();

        ret = xQueueSendFromISR(audio_stream_queue, audio_stream_info, &pxHigherPriorityTaskWoken);
        if (pxHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }

        bflb_irq_restore(flag);

    } else {
        ret = xQueueSend(audio_stream_queue, audio_stream_info, 0);
    }

    if (ret == pdTRUE) {
        return 0;
    } else {
        printf("ERROR: jpeg push queue full\r\n");
        return -2;
    }
}

static void audio_dma_callback(void *arg)
{

    if(audio_stream_push(&record_buf) == 0){
        bflb_dma_channel_start(auadc_dma_hd);
    }
    else{
        bflb_dma_channel_stop(auadc_dma_hd);
    }
}

static void audio_dma_tx_callback(void *arg)
{
    bflb_dma_channel_stop(auadc_dma_tx);
    audio_play_status = 0;
}

static void auadc_init(void)
{
    /* clock config */
    GLB_Config_AUDIO_PLL_To_491P52M();
    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_AUDIO);

    /* auadc init */
    auadc_hd = bflb_device_get_by_name("auadc");
    bflb_auadc_init(auadc_hd, &auadc_init_cfg);
    bflb_auadc_adc_init(auadc_hd, &auadc_analog_init_cfg);
    /* set volume */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_SET_VOLUME_VAL, (size_t)(1));
    /* auadc enable dma */
    bflb_auadc_link_rxdma(auadc_hd, true);
}

/* audio dac init */
static void audac_init(void)
{

    /* clock cfg */
    // GLB_Config_AUDIO_PLL_To_491P52M();
    // GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_AUDIO);

    /* audac init */
    audac_hd = bflb_device_get_by_name("audac");
    bflb_audac_init(audac_hd, &audac_init_cfg);
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-15 * 2));
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-1));
    bflb_audac_volume_init(audac_hd, &audac_volume_cfg);
    /* audac enable dma */
    bflb_audac_link_rxdma(audac_hd, true);
}

static void auadc_dma_init(void)
{
    static struct bflb_dma_channel_lli_pool_s lli_pool[20];
    static struct bflb_dma_channel_lli_transfer_s transfers[1];
    struct bflb_dma_channel_config_s auadc_dma_cfg;

    auadc_dma_cfg.direction = DMA_PERIPH_TO_MEMORY;
    auadc_dma_cfg.src_req = DMA_REQUEST_AUADC_RX;
    auadc_dma_cfg.dst_req = DMA_REQUEST_NONE;
    auadc_dma_cfg.src_addr_inc = DMA_ADDR_INCREMENT_DISABLE;
    auadc_dma_cfg.dst_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    auadc_dma_cfg.src_burst_count = DMA_BURST_INCR4;
    auadc_dma_cfg.dst_burst_count = DMA_BURST_INCR4;
    auadc_dma_cfg.src_width = DMA_DATA_WIDTH_16BIT;
    auadc_dma_cfg.dst_width = DMA_DATA_WIDTH_16BIT;

    auadc_dma_hd = bflb_device_get_by_name("dma0_ch0");
    bflb_dma_channel_init(auadc_dma_hd, &auadc_dma_cfg);
    bflb_dma_channel_irq_attach(auadc_dma_hd, audio_dma_callback, NULL);

    transfers[0].src_addr = (uint32_t)DMA_ADDR_AUADC_RDR;
    transfers[0].dst_addr = (uint32_t)record_buf.frame_data;
    transfers[0].nbytes = sizeof(record_buf.frame_data);

    uint32_t num = bflb_dma_channel_lli_reload(auadc_dma_hd, lli_pool, 20, transfers, 1);
    bflb_dma_channel_lli_link_head(auadc_dma_hd, lli_pool, num);
}

static void audac_dma_init(void)
{
    static struct bflb_dma_channel_lli_pool_s lli_pool_tx[20];
    static struct bflb_dma_channel_lli_transfer_s transfers_tx[1];
    struct bflb_dma_channel_config_s auadc_dma_cfg;

    auadc_dma_cfg.direction = DMA_MEMORY_TO_PERIPH;
    auadc_dma_cfg.src_req = DMA_REQUEST_NONE;
    auadc_dma_cfg.dst_req = DMA_REQUEST_AUDAC_TX;
    auadc_dma_cfg.src_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    auadc_dma_cfg.dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE;
    auadc_dma_cfg.src_burst_count = DMA_BURST_INCR4;
    auadc_dma_cfg.dst_burst_count = DMA_BURST_INCR4;
    auadc_dma_cfg.src_width = DMA_DATA_WIDTH_16BIT;
    auadc_dma_cfg.dst_width = DMA_DATA_WIDTH_16BIT;

    auadc_dma_tx = bflb_device_get_by_name("dma0_ch1");
    bflb_dma_channel_init(auadc_dma_tx, &auadc_dma_cfg);
    bflb_dma_channel_irq_attach(auadc_dma_tx, audio_dma_tx_callback, NULL);

    transfers_tx[0].src_addr = (uint32_t)play_buf.frame_data;
    transfers_tx[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers_tx[0].nbytes = sizeof(play_buf.frame_data);

    uint32_t num = bflb_dma_channel_lli_reload(auadc_dma_tx, lli_pool_tx, 20, transfers_tx, 1);
    bflb_dma_channel_lli_link_head(auadc_dma_tx, lli_pool_tx, num);
}

/* gpio init */
static void auadc_gpio_init(void)
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

    /* audac pwm output mode */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

void record_play_on(void)
{
    bflb_dma_channel_start(auadc_dma_hd);
}
void record_play_off(void)
{
    bflb_dma_channel_stop(auadc_dma_hd);
}

static int audio_stream_init(void)
{
    /* create queue */
    audio_stream_queue = xQueueCreate(AUDIO_QUEUES_NUM, sizeof(audio_stream_t));
    xQueueReset(audio_stream_queue);

    return 0;
}

static int audio_stream_pop(audio_stream_t *audio_stream_info, uint32_t timeout)
{
    BaseType_t ret;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    ret = xQueueReceive(audio_stream_queue, audio_stream_info, timeout);

    if (ret == pdTRUE) {
        return 0;
    } else {
        return -1;
    }
}

static void audio_play_task(void *param)
{

    audio_stream_init();
    while(1){
        if(audio_play_status == 0){
            if(audio_stream_pop(&play_buf, 0) == 0){
                bflb_dma_channel_start(auadc_dma_tx);
                audio_play_status = 1;
            }
        }

        vTaskDelay(10);
    }

}


int audio_play_task_init(void)
{

    printf("[ADC/DAC] audio play init \r\n");

    /* gpio init */
    auadc_gpio_init();

    /* auadc init */
    auadc_init();
    audac_init();

    /* dma init and start */
    auadc_dma_init();
    audac_dma_init();

    /* auadc start */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_RECORD_START, 0);
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);

    bflb_mtimer_delay_ms(100);

    xTaskCreate(audio_play_task, (char *)"audio_play_task", audio_play_STACK_SIZE, NULL, baudio_play_PRIORITY, &audio_play_task_hd);

    return 0;
}
