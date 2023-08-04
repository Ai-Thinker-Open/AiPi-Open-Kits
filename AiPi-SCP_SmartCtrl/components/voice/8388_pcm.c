#include "board.h"

#include "bflb_clock.h"
#include "bflb_mtimer.h"
#include "bflb_i2s.h"
#include "bflb_i2c.h"
#include "bflb_dma.h"
#include "bflb_gpio.h"

#include "bl616_glb.h"

// #include "fhm_onechannel_16k_20.h"
#include "bsp_es8388.h"

struct bflb_device_s *i2s0;
struct bflb_device_s *i2c0;
struct bflb_device_s *dma0_ch0;


static ES8388_Cfg_Type ES8388Cfg = {
    .work_mode = ES8388_CODEC_MDOE,          /*!< ES8388 work mode */
    .role = ES8388_SLAVE,                    /*!< ES8388 role */
    .mic_input_mode = ES8388_DIFF_ENDED_MIC, /*!< ES8388 mic input mode */
    .mic_pga = ES8388_MIC_PGA_3DB,           /*!< ES8388 mic PGA */
    .i2s_frame = ES8388_LEFT_JUSTIFY_FRAME,  /*!< ES8388 I2S frame */
    .data_width = ES8388_DATA_LEN_16,        /*!< ES8388 I2S dataWitdh */
};

#define TEMP_BUF_SIZE 10 * 1024
#define OPEN_ADDR  (0x210000 + 0x2000)
#define OPEN_SIZE 73856//(73856 - 22656)
#define CLOSE_ADDR 0x224000
#define CLOSE_SIZE 76160//(76160 - 24960)
static uint16_t temp1[TEMP_BUF_SIZE];
static uint16_t temp2[TEMP_BUF_SIZE];
static uint32_t pcm_size = 0;
static uint32_t pcm_addr = 0;
static uint32_t play_size = 0;
static int tmp_flag = 0;
static uint8_t audac_dma_stop = 0;
uint32_t read_size1 = 0;
uint32_t read_size2 = 0;


static int pcm_read(void);
void dma_i2s_tx_start(char *buf, uint32_t size);

void dma0_ch0_isr(void *arg)
{
    // printf("tc done\r\n");
    bflb_dma_channel_stop(dma0_ch0);
    if(audac_dma_stop == 1)
    {
        audac_dma_stop = 0;
        // bflb_dma_channel_stop(dma0_ch0);
    }
    else
    {
        pcm_read();
        if (tmp_flag & 0x01) {
            dma_i2s_tx_start(temp2, read_size2);
            printf("temp2, read_size2:%d\r\n", read_size2);
        }else{
            dma_i2s_tx_start(temp1, read_size1);
            printf("temp1, read_size1:%d\r\n", read_size1);
        }
    }
}

void i2s_gpio_init()
{
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* I2S_FS */
    // bflb_gpio_init(gpio, GPIO_PIN_13, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // /* I2S_DI */
    // bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // /* I2S_DO */
    // bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // /* I2S_BCLK */
    // bflb_gpio_init(gpio, GPIO_PIN_20, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    // /* MCLK CLKOUT */
    // bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_FUNC_CLKOUT | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    // /* I2C0_SCL */
    // bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    // /* I2C0_SDA */
    // bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);

    bflb_gpio_init(gpio, GPIO_PIN_13, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_DI */
    bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_DO */
    bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2S_BCLK */
    bflb_gpio_init(gpio, GPIO_PIN_20, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    /* MCLK CLKOUT */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_CLKOUT | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    /* I2C0_SCL */
    bflb_gpio_init(gpio, GPIO_PIN_0, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(gpio, GPIO_PIN_1, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}
void i2s_init()
{
    struct bflb_i2s_config_s i2s_cfg = {
        //.bclk_freq_hz = 16000 * 32 * 2, /* bclk = Sampling_rate * frame_width * channel_num */
        .bclk_freq_hz = 24000 * 32 * 2,
        .role = I2S_ROLE_MASTER,
        .format_mode = I2S_MODE_LEFT_JUSTIFIED,
        .channel_mode = I2S_CHANNEL_MODE_NUM_1,
        .frame_width = I2S_SLOT_WIDTH_32,
        .data_width = I2S_SLOT_WIDTH_16,
        .fs_offset_cycle = 0,

        .tx_fifo_threshold = 0,
        .rx_fifo_threshold = 0,
    };

    printf("i2s init\r\n");
    i2s0 = bflb_device_get_by_name("i2s0");
    /* i2s init */
    bflb_i2s_init(i2s0, &i2s_cfg);
    
    /* enable dma */
    bflb_i2s_link_txdma(i2s0, true);
    bflb_i2s_link_rxdma(i2s0, true);
}
void dma_tx_init()
{
    // static struct bflb_dma_channel_lli_pool_s tx_llipool[100];
    // static struct bflb_dma_channel_lli_transfer_s tx_transfers[2];
    

    struct bflb_dma_channel_config_s tx_config = {
        .direction = DMA_MEMORY_TO_PERIPH,
        .src_req = DMA_REQUEST_NONE,
        .dst_req = DMA_REQUEST_I2S_TX,
        .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
        .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
        .src_burst_count = DMA_BURST_INCR1,
        .dst_burst_count = DMA_BURST_INCR1,
        .src_width = DMA_DATA_WIDTH_16BIT,
        .dst_width = DMA_DATA_WIDTH_16BIT,
    };



    printf("dma init\r\n");
    dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
    bflb_dma_channel_init(dma0_ch0, &tx_config);
    bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);

    // tx_transfers[0].src_addr = (uint32_t)temp1;
    // tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
    // tx_transfers[0].nbytes = sizeof(temp1);

    // tx_transfers[1].src_addr = (uint32_t)temp2;
    // tx_transfers[1].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
    // tx_transfers[1].nbytes = sizeof(temp2);

    // printf("dma lli init\r\n");
    // uint32_t num = bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 100, tx_transfers, 2);
    // bflb_dma_channel_lli_link_head(dma0_ch0, tx_llipool, num);
    // printf("dma lli num: %d \r\n", num);
    // bflb_dma_channel_start(dma0_ch0);
}

void dma_i2s_tx_start(char *buf, uint32_t size)
{
    static struct bflb_dma_channel_lli_transfer_s tx_transfers[1];
    static struct bflb_dma_channel_lli_pool_s tx_llipool[100];

    tx_transfers[0].src_addr = (uint32_t)buf;
    tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_I2S_TDR;
    tx_transfers[0].nbytes = size;
    uint32_t num = bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 100, tx_transfers, 1);
    bflb_dma_channel_lli_link_head(dma0_ch0, tx_llipool, num);
    bflb_dma_channel_start(dma0_ch0);
}

void mclk_out_init()
{
    GLB_Set_I2S_CLK(ENABLE, 2, GLB_I2S_DI_SEL_I2S_DI_INPUT, GLB_I2S_DO_SEL_I2S_DO_OUTPT);
    // GLB_Set_Chip_Clock_Out3_Sel(GLB_CHIP_CLK_OUT_3_I2S_REF_CLK);
    GLB_Set_Chip_Clock_Out2_Sel(GLB_CHIP_CLK_OUT_2_I2S_REF_CLK);
}

static int pcm_read(void)
{
    tmp_flag++;
    if(pcm_size == 0){
        audac_dma_stop = 1;
        return -1;
    }
    if (tmp_flag & 0x01) {
        if ((play_size + sizeof(temp1)) > pcm_size) {
            read_size1 = pcm_size - play_size;
            printf("1play_size:%d, read_size:%d\r\n", play_size, pcm_size - play_size);
            bflb_flash_read(pcm_addr + play_size, temp1, read_size1);
            play_size = 0;
            pcm_size = 0;
            // audac_dma_stop = 1;
            // bflb_dma_channel_stop(audac_dma_hd);
        } else {
            read_size1 = sizeof(temp1);
            printf("1play_size:%d, read_size:%d\r\n", play_size, read_size1);
            bflb_flash_read(pcm_addr + play_size, temp1, read_size1);
            play_size += read_size1;
        }
    } else {
        if ((play_size + sizeof(temp2)) > pcm_size) {
            printf("2play_size:%d, read_size:%d\r\n", play_size, pcm_size - play_size);
            read_size2 = pcm_size - play_size;
            bflb_flash_read(pcm_addr + play_size, temp2, read_size2);
            play_size = 0;
            pcm_size = 0;
            // audac_dma_stop = 1;
            // bflb_dma_channel_stop(audac_dma_hd);
        } else {
            printf("2play_size:%d, read_size:%d\r\n", play_size, sizeof(temp2));
            read_size2 = sizeof(temp2);
            bflb_flash_read(pcm_addr + play_size, temp2, read_size2);
            play_size += read_size2;
        }
    }

    // printf("temp:%04x,%04x,%04x,%04x\r\n", temp[0],temp[1],temp[2],temp[3]);
    return 0;
}

static void play_voice(uint32_t play_addr, uint32_t voice_size)
{
    tmp_flag = 0;
    play_size = 0;
    pcm_size = voice_size;
    pcm_addr = play_addr;


    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_STOP, 0);
    bflb_dma_channel_stop(dma0_ch0);
    // dma_tx_init();
    pcm_read();
    pcm_read();
    dma_i2s_tx_start(temp1, read_size1);
    printf("temp1, read_size1:%d\r\n", read_size1);
    // bflb_dma_channel_start(dma0_ch0);
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);
}

void play_voice_open(void)
{
    play_voice(OPEN_ADDR, OPEN_SIZE);
}
void play_voice_close(void)
{
    play_voice(CLOSE_ADDR, CLOSE_SIZE);
}

int es8388_voice_init(void)
{
    // board_init();

    printf("\n\ri2s dma test\n\r");

    /* gpio init */
    i2s_gpio_init();

    /* init ES8388 Codec */
    printf("es8388 init\n\r");
    ES8388_Init(&ES8388Cfg);
    ES8388_Set_Voice_Volume(90);

    /* mclk clkout init */
    mclk_out_init();

    /* i2s init */
    i2s_init();
    dma_tx_init();

    /* enable i2s tx and rx */
    bflb_i2s_feature_control(i2s0, I2S_CMD_DATA_ENABLE, I2S_CMD_DATA_ENABLE_TX | I2S_CMD_DATA_ENABLE_RX);

    // printf("test end\n\r");

    // while (1) {
    //     bflb_mtimer_delay_ms(10);
    // }
}
