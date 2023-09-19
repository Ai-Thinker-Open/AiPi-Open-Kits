#include "bflb_auadc.h"
#include "bflb_audac.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"

#include "bl616_glb.h"
#include "board.h"

#define AUADC_SAMPLING_NUM (50 * 1024)

static ATTR_NOCACHE_NOINIT_RAM_SECTION uint16_t record_buff[AUADC_SAMPLING_NUM];
// static ATTR_PSRAM_SECTION uint16_t record_buff222[AUADC_SAMPLING_NUM];
// static ATTR_TCM_CONST_SECTIION uint16_t record_buff[AUADC_SAMPLING_NUM];

/* audio adc config */
struct bflb_auadc_init_config_s auadc_init_cfg = {
    .sampling_rate = AUDAC_SAMPLING_RATE_32K,
    .input_mode = AUADC_INPUT_MODE_ADC,
    .data_format = AUADC_DATA_FORMAT_16BIT,
    .fifo_threshold = 3,
};

/* audio adc analog config */
struct bflb_auadc_adc_init_config_s auadc_analog_init_cfg = {
    .auadc_analog_en = true,
    .adc_mode = AUADC_ADC_MODE_AUDIO,
    .adc_pga_mode = AUADC_ADC_PGA_MODE_AC_DIFFER,
    .adc_pga_posi_ch = AUADC_ADC_ANALOG_CH_0,
    .adc_pga_nega_ch = AUADC_ADC_ANALOG_CH_7,
    .adc_pga_gain = 30,
};

struct bflb_audac_init_config_s audac_init_cfg = {
    .sampling_rate = AUDAC_SAMPLING_RATE_16K, //AUDAC_SAMPLING_RATE_32K,
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


void audio_dma_callback(void *arg)
{
    bflb_dma_channel_stop(auadc_dma_hd);
    bflb_dma_channel_start(auadc_dma_tx);
    printf("auadc record end\r\n");
}
void audio_dma_tx_callback(void *arg)
{
    bflb_dma_channel_stop(auadc_dma_tx);
    bflb_dma_channel_start(auadc_dma_hd);
    // bflb_auadc_feature_control(auadc_hd, AUADC_CMD_RECORD_START, 0);
    printf("auadc play end\r\n");
}

void auadc_init(void)
{
    /* clock config */
    GLB_Config_AUDIO_PLL_To_491P52M();
    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_AUDIO);

    /* auadc init */
    auadc_hd = bflb_device_get_by_name("auadc");
    bflb_auadc_init(auadc_hd, &auadc_init_cfg);
    bflb_auadc_adc_init(auadc_hd, &auadc_analog_init_cfg);
    /* set volume */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_SET_VOLUME_VAL, (size_t)(-1));
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

void auadc_dma_init(void)
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
    transfers[0].dst_addr = (uint32_t)record_buff;
    transfers[0].nbytes = sizeof(record_buff);

    uint32_t num = bflb_dma_channel_lli_reload(auadc_dma_hd, lli_pool, 20, transfers, 1);
    bflb_dma_channel_lli_link_head(auadc_dma_hd, lli_pool, num);
}

void audac_dma_init(void)
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

    transfers_tx[0].src_addr = (uint32_t)record_buff;
    transfers_tx[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers_tx[0].nbytes = sizeof(record_buff);

    uint32_t num = bflb_dma_channel_lli_reload(auadc_dma_tx, lli_pool_tx, 20, transfers_tx, 1);
    bflb_dma_channel_lli_link_head(auadc_dma_tx, lli_pool_tx, num);
}


/* gpio init */
void auadc_gpio_init(void)
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* auadc ch0 */
    bflb_gpio_init(gpio, GPIO_PIN_20, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);
    /* auadc ch1 */
    //bflb_gpio_init(gpio, GPIO_PIN_21, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

    /* auadc ch2 */
    //bflb_gpio_init(gpio, GPIO_PIN_22, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);
    /* auadc ch3 */
    // bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

    /* auadc ch4 */
    //bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);
    /* auadc ch5 */
    //bflb_gpio_init(gpio, GPIO_PIN_28, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

    // /* auadc ch6 */
    // bflb_gpio_init(gpio, GPIO_PIN_29, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);
    // /* auadc ch7 */
    // bflb_gpio_init(gpio, GPIO_PIN_30, GPIO_ANALOG | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_2);

    /* audac pwm output mode */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    // bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

int audio_init(void)
{
    /* board init */
    // board_init();

    printf("auadc record to mem start \r\n");

    /* gpio init */
    auadc_gpio_init();

    /* auadc init */
    auadc_init();
    audac_init();

    /* dma init and start */
    auadc_dma_init();
    audac_dma_init();
    bflb_dma_channel_start(auadc_dma_hd);
    // bflb_dma_channel_start(auadc_dma_tx);

    /* auadc start */
    bflb_auadc_feature_control(auadc_hd, AUADC_CMD_RECORD_START, 0);
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);

    // printf("Wait for dma to complete... \r\n");

    bflb_mtimer_delay_ms(100);

    // while (1) {
    //     bflb_mtimer_delay_ms(100);
    // }
}
