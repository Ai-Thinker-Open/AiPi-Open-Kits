#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_audac.h"
#include "bflb_dma.h"
#include "bflb_l1c.h"

#include "bl616_glb.h"

#include "board.h"
#include "bflb_flash.h"

#include "app_main.h"

#include "audac.h"
#include "smaninke.h"

//Pay attention to the maximum size of flash and check it yourself
#define XIP_FLASH_ADDR(x)   ((uint32_t)0xA0000000+(x-bflb_flash_get_image_offset()))

romfs_file_t img_file;
// clang-format off
static const uint16_t sin_0db_16point[16] = {
    0x0000, 0x30FB, 0x5A81, 0x7640, 0x7FFF, 0x7640, 0x5A81, 0x30FB, 0x0000, 0xCF05, 0xA57F, 0x89C0, 0x8001, 0x89C0, 0xA57F, 0xCF05,
};

static const uint16_t sin_0db_32point[32] = {
    0x0000, 0x18F8, 0x30FB, 0x471C, 0x5A81, 0x6A6C, 0x7640, 0x7D89, 0x7FFF, 0x7D89, 0x7640, 0x6A6C, 0x5A81, 0x471C, 0x30FB, 0x18F8,
    0x0000, 0xE708, 0xCF05, 0xB8E4, 0xA57F, 0x9594, 0x89C0, 0x8277, 0x8001, 0x8277, 0x89C0, 0x9594, 0xA57F, 0xB8E4, 0xCF05, 0xE708,
    };

const static uint16_t sin_0db_l32_r16_2ch[] = {
    0x0000,0x0000, 0x18F8,0x30FB, 0x30FB,0x5A81, 0x471C,0x7640, 0x5A81,0x7FFF, 0x6A6C,0x7640, 0x7640,0x5A81, 0x7D89,0x30FB,
    0x7FFF,0x0000, 0x7D89,0xCF05, 0x7640,0xA57F, 0x6A6C,0x89C0, 0x5A81,0x8001, 0x471C,0x89C0, 0x30FB,0xA57F, 0x18F8,0xCF05,
    0x0000,0x0000, 0xE708,0x30FB, 0xCF05,0x5A81, 0xB8E4,0x7640, 0xA57F,0x7FFF, 0x9594,0x7640, 0x89C0,0x5A81, 0x8277,0x30FB,
    0x8001,0x0000, 0x8277,0xCF05, 0x89C0,0xA57F, 0x9594,0x89C0, 0xA57F,0x8001, 0xB8E4,0x89C0, 0xCF05,0xA57F, 0xE708,0xCF05,
};

#if (1)
#define AUDAC_INFO(a, ...) printf("[AUDAC info]:" a, ##__VA_ARGS__)
#else
#define AUDAC_INFO(a, ...)
#endif

static struct bflb_device_s *audac_hd;
static struct bflb_device_s *audac_dma_hd;

/* audio gpio init*/
static void audac_gpio_init(void)
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* audac pwm output mode */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    // bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

/* audio dac init */
static void audac_init(void)
{
    struct bflb_audac_init_config_s audac_init_cfg = {
        .sampling_rate = AUDAC_SAMPLING_RATE_16K,
        .output_mode = AUDAC_OUTPUT_MODE_PWM,
        .source_channels_num = AUDAC_SOURCE_CHANNEL_SINGLE,
        .mixer_mode = AUDAC_MIXER_MODE_ONLY_L,
        .data_format = AUDAC_DATA_FORMAT_16BIT,
        .fifo_threshold = 7,
    };

    struct bflb_audac_volume_config_s audac_volume_cfg = {
        .mute_ramp_en = true,
        .mute_up_ramp_rate = AUDAC_RAMP_RATE_FS_2,
        .mute_down_ramp_rate = AUDAC_RAMP_RATE_FS_2,
        .volume_update_mode = AUDAC_VOLUME_UPDATE_MODE_RAMP,
        .volume_ramp_rate = AUDAC_RAMP_RATE_FS_128,
        .volume_zero_cross_timeout = AUDAC_RAMP_RATE_FS_128,
    };

    /* clock cfg */
    GLB_Config_AUDIO_PLL_To_491P52M();
    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_AUDIO);

    /* audac init */
    audac_hd = bflb_device_get_by_name("audac");
    bflb_audac_init(audac_hd, &audac_init_cfg);
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-10));
    bflb_audac_volume_init(audac_hd, &audac_volume_cfg);
    /* audac enable dma */
    bflb_audac_link_rxdma(audac_hd, true);
}

void audio_dma0_callback(void *arg)
{
    static uint32_t num = 0;
    num++;
    // AUDAC_INFO("scyle_n:%d\r\n", num);
}

void audac_dma_chann_stop(void)
{
        bflb_dma_channel_stop(audac_dma_hd);
        bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_STOP, 0);
        // romfs_close(&img_file);
}
extern uint8_t *audio_pcm_data;
/* audio dac dma init */
static void audac_dma_init(void)
{
    uint32_t dma_lli_cnt;
    static struct bflb_dma_channel_lli_pool_s lli_pool[10];
    struct bflb_dma_channel_lli_transfer_s transfers[1];
    struct bflb_dma_channel_config_s audac_dma_cfg;

    audac_dma_cfg.direction = DMA_MEMORY_TO_PERIPH;
    audac_dma_cfg.src_req = DMA_REQUEST_NONE;
    audac_dma_cfg.dst_req = DMA_REQUEST_AUDAC_TX;
    audac_dma_cfg.src_addr_inc = DMA_ADDR_INCREMENT_ENABLE;
    audac_dma_cfg.dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE;
    audac_dma_cfg.src_burst_count = DMA_BURST_INCR8;
    audac_dma_cfg.dst_burst_count = DMA_BURST_INCR8;
    audac_dma_cfg.src_width = DMA_DATA_WIDTH_16BIT;
    audac_dma_cfg.dst_width = DMA_DATA_WIDTH_16BIT;

    audac_dma_hd = bflb_device_get_by_name("dma0_ch0");
    bflb_dma_channel_init(audac_dma_hd, &audac_dma_cfg);
    bflb_dma_channel_irq_attach(audac_dma_hd, audio_dma0_callback, NULL);
#if 1
    transfers[0].src_addr = (uint32_t)XIP_FLASH_ADDR(DEF_AUDIO_ANXINKE_PCM);
    transfers[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers[0].nbytes = sizeof(file_smaninke);

    bflb_l1c_dcache_clean_range(file_smaninke, sizeof(file_smaninke));
#elif 1
    uint32_t file_len=48 * 1024;
    // uint32_t file_addr = romfs_get_file_addr(&img_file, &file_len);

    transfers[0].src_addr = (uint32_t)record_buff;
    transfers[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers[0].nbytes = file_len;
    // bflb_l1c_dcache_clean_range((void*)audio_pcm_data, file_len);
#else
    transfers[0].src_addr = (uint32_t)sin_0db_l32_r16_2ch;
    transfers[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers[0].nbytes = sizeof(sin_0db_l32_r16_2ch);

    // bflb_l1c_dcache_clean_range(sin_0db_l32_r16_2ch, sizeof(sin_0db_l32_r16_2ch));

#endif
    dma_lli_cnt = bflb_dma_channel_lli_reload(audac_dma_hd, lli_pool, 10, transfers, 1);
    bflb_dma_channel_lli_link_head(audac_dma_hd, lli_pool, dma_lli_cnt);
}



int audio_play_task_init(void)
{
    AUDAC_INFO("audio play init\r\n");

    /* gpio init */
    audac_gpio_init();
    /* audac init */
    audac_init();
    /* audac init */
    audac_dma_init();
    bflb_dma_channel_start(audac_dma_hd);
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);
    return 0;
}
