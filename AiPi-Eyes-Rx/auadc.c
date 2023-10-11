#include "bflb_auadc.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"

#include "bl616_glb.h"
#include "board.h"

#define AUADC_SAMPLING_NUM (48 * 1024)

__attribute((aligned(32))) uint16_t record_buff[AUADC_SAMPLING_NUM];
static void auadc_init(void);
static void auadc_dma_init(void);

/* audio adc config */
struct bflb_auadc_init_config_s auadc_init_cfg = {
    .sampling_rate = AUADC_SAMPLING_RATE_48K,
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
    .adc_pga_gain = 30,
};

struct bflb_device_s *auadc_hd;
struct bflb_device_s *auadc_dma_hd;

void audio_dma1_callback(void *arg)
{
    for (int i = 0; i < 128; i++) {
        printf("%02x ", record_buff[i * 2]);
    }
    printf("\r\n");

    bflb_l1c_dcache_invalidate_range(record_buff, sizeof(record_buff));
    memset(record_buff, 0x00,sizeof(record_buff));
    /* dma init and start */
    auadc_dma_init();
    bflb_dma_channel_start(auadc_dma_hd);
    /* auadc start */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_RECORD_START, 0);
    printf("auadc record end\r\n");
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
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_SET_VOLUME_VAL, (size_t)(-10));
    /* auadc enable dma */
    bflb_auadc_link_rxdma(auadc_hd, true);
}

static void auadc_dma_init(void)
{
    static struct bflb_dma_channel_lli_pool_s lli_pool[20];
    struct bflb_dma_channel_lli_transfer_s transfers[1];
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

    auadc_dma_hd = bflb_device_get_by_name("dma0_ch1");
    bflb_dma_channel_init(auadc_dma_hd, &auadc_dma_cfg);

    bflb_dma_channel_irq_attach(auadc_dma_hd, audio_dma1_callback, NULL);

    transfers[0].src_addr = (uint32_t)DMA_ADDR_AUADC_RDR;
    transfers[0].dst_addr = (uint32_t)record_buff;
    transfers[0].nbytes = sizeof(record_buff);

    bflb_dma_channel_lli_reload(auadc_dma_hd, lli_pool, 20, transfers, 1);
}

/* gpio init */
static void auadc_gpio_init(void)
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* auadc ch3 */
    bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

}

int audio_record_task_init(void)
{
    printf("auadc start \r\n");

    /* gpio init */
    auadc_gpio_init();

    /* auadc init */
    auadc_init();

    /* dma init and start */
    auadc_dma_init();
    bflb_dma_channel_start(auadc_dma_hd);

    /* auadc start */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_RECORD_START, 0);

    return 0;
}
