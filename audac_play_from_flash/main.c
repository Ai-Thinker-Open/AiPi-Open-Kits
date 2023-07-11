#include "bflb_audac.h"
#include "bflb_gpio.h"
#include "bflb_dma.h"

#include "bl616_glb.h"
#include "board.h"
#include "board.h"
#include "bflb_flash.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

#define TEMP_BUF_SIZE 10 * 1024
#define OPEN_ADDR 0x210000//0x210000
#define OPEN_SIZE (140822)
#define CLOSE_ADDR 0x2e4000
#define CLOSE_SIZE (145056)

uint16_t temp1[TEMP_BUF_SIZE];
uint16_t temp2[TEMP_BUF_SIZE];
uint32_t pcm_size = 827212;
uint32_t pcm_addr = 0;
uint32_t play_size = 0;
static int tmp_flag = 0;
uint8_t audac_dma_stop = 0;


struct bflb_device_s* audac_dma_hd;
struct bflb_device_s* audac_hd;
struct bflb_device_s* gpio;

#define button_PROCESS_STACK_SIZE  (1024)
#define button_PROCESS_PRIORITY (14)
static TaskHandle_t http_server_task_hd;
static TaskHandle_t cam_process_task_hd;
static TaskHandle_t button_process_task_hd;

void audio_dma_callback(void* arg)
{
    static uint32_t num = 0;
    num++;
    // printf("scyle_n:%d\r\n", num);
    if (audac_dma_stop == 1)
    {
        audac_dma_stop = 0;
        bflb_dma_channel_stop(audac_dma_hd);
    }
    else
    {
        pcm_read();
    }
    // bflb_dma_channel_stop(audac_dma_hd);
    // pcm_read();
    // bflb_dma_channel_start(audac_dma_hd);

}

/* audio gpio init*/
void audac_gpio_init(void)
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* audac pwm output mode */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);

    // bflb_gpio_init(gpio, GPIO_PIN_22, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    // bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);

    // bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    // bflb_gpio_init(gpio, GPIO_PIN_28, GPIO_FUNC_AUDAC_PWM | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);

    /* PA enable */
    // bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_set(gpio, GPIO_PIN_10);
    // bflb_gpio_set(gpio, GPIO_PIN_11);
}

void audac_dma_init(void)
{
    uint32_t dma_lli_cnt;
    static struct bflb_dma_channel_lli_pool_s lli_pool[20];
    struct bflb_dma_channel_lli_transfer_s transfers[2];
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
    bflb_dma_channel_irq_attach(audac_dma_hd, audio_dma_callback, NULL);

    transfers[0].src_addr = (uint32_t)temp1;
    transfers[0].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers[0].nbytes = sizeof(temp1);

    transfers[1].src_addr = (uint32_t)temp2;
    transfers[1].dst_addr = (uint32_t)DMA_ADDR_AUDAC_TDR;
    transfers[1].nbytes = sizeof(temp2);

    // bflb_l1c_dcache_clean_range(temp1, TEMP_BUF_SIZE);
    dma_lli_cnt = bflb_dma_channel_lli_reload(audac_dma_hd, lli_pool, 20, transfers, 2);
    bflb_dma_channel_lli_link_head(audac_dma_hd, lli_pool, dma_lli_cnt);
}

/* audio dac init */
static void audac_init(void)
{
    struct bflb_audac_init_config_s audac_init_cfg = {
        .sampling_rate = AUDAC_SAMPLING_RATE_22P05K, //AUDAC_SAMPLING_RATE_32K,
        .output_mode = AUDAC_OUTPUT_MODE_PWM,
        .source_channels_num = AUDAC_SOURCE_CHANNEL_DUAL,
        .mixer_mode = AUDAC_MIXER_MODE_ONLY_L,
        .data_format = AUDAC_DATA_FORMAT_16BIT,
        .fifo_threshold = 7,
    };

    struct bflb_audac_volume_config_s audac_volume_cfg = {
        .mute_ramp_en = true,
        .mute_up_ramp_rate = AUDAC_RAMP_RATE_FS_8,
        .mute_down_ramp_rate = AUDAC_RAMP_RATE_FS_8,
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
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-15 * 2));
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-10));
    bflb_audac_volume_init(audac_hd, &audac_volume_cfg);
    /* audac enable dma */
    bflb_audac_link_rxdma(audac_hd, true);
}


int pcm_read(void)
{

    uint32_t read_size;

    if (tmp_flag & 0x01) {
        // audac_dma_init(temp2);
        // bflb_dma_channel_start(audac_dma_hd);
        if ((play_size + sizeof(temp1)) > pcm_size) {
            // printf("1play_size:%d, read_size:%d\r\n", play_size, pcm_size - play_size);
            // bflb_flash_read(pcm_addr + play_size, temp1, pcm_size - play_size);
            play_size = 0;
            audac_dma_stop = 1;
            // bflb_dma_channel_stop(audac_dma_hd);
        }
        else {
            printf("1play_size:%d, read_size:%d\r\n", play_size, sizeof(temp1));
            bflb_flash_read(pcm_addr + play_size, temp1, sizeof(temp1));
            play_size += sizeof(temp1);
        }
    }
    else {
        // audac_dma_init(temp1);
        // bflb_dma_channel_start(audac_dma_hd);
        if ((play_size + sizeof(temp2)) > pcm_size) {
            // printf("2play_size:%d, read_size:%d\r\n", play_size, pcm_size - play_size);
            // bflb_flash_read(pcm_addr + play_size, temp2, pcm_size - play_size);
            play_size = 0;
            audac_dma_stop = 1;
            // bflb_dma_channel_stop(audac_dma_hd);
        }
        else {
            printf("2play_size:%d, read_size:%d\r\n", play_size, sizeof(temp2));
            bflb_flash_read(pcm_addr + play_size, temp2, sizeof(temp2));
            play_size += sizeof(temp2);
        }
    }
    tmp_flag++;

    // printf("temp:%04x,%04x,%04x,%04x\r\n", temp[0],temp[1],temp[2],temp[3]);
    return 0;
}

void play_voice(uint32_t play_addr, uint32_t voice_size)
{
    tmp_flag = 1;
    play_size = 0;
    pcm_size = voice_size;
    pcm_addr = play_addr;


    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_STOP, 0);
    bflb_dma_channel_stop(audac_dma_hd);
    audac_dma_init();
    pcm_read();
    pcm_read();
    bflb_dma_channel_start(audac_dma_hd);
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

int audio_pcm_init(void)
{
    /* board init */
    // board_init();

    printf("audac case start \r\n");

    /* gpio init */
    audac_gpio_init();

    /* audac init */
    audac_init();

    /* dma init */
    // printf("pcm_read:%d\r\n", pcm_read());
    audac_dma_init();
    // bflb_dma_channel_start(audac_dma_hd);

    /* audac start */
    bflb_audac_feature_control(audac_hd, AUDAC_CMD_PLAY_START, 0);
    // bflb_mtimer_delay_ms(1000);

    // /* enable mute */
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_MUTE, true);
    // bflb_mtimer_delay_ms(1000);

    // /* disable mute */
    // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_MUTE, false);
    // bflb_mtimer_delay_ms(1000);

    // printf("audac case end \r\n");

    // while (1) {
        /* set volume */
        // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-35 * 2));
        // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-5 * 2));
        // bflb_mtimer_delay_ms(1500);

        // bflb_audac_feature_control(audac_hd, AUDAC_CMD_SET_VOLUME_VAL, (size_t)(-1 * 2));
        // bflb_mtimer_delay_ms(1000);
    // }
}


void button_process_task(void* param)
{
    static uint32_t press_10ms_cnt = 0;
    uint32_t press_mode = 0;
    static bool led_ctrl = 0;
    static bool button = 1;
    uint32_t frames = 1;
    uint8_t* pBuf = NULL;
    uint32_t len;
    uint8_t* pStr2 = NULL;
    uint32_t len2;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, GPIO_PIN_16, GPIO_OUTPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_reset(gpio, GPIO_PIN_16);
    while (1) {
        if (bflb_gpio_read(gpio, GPIO_PIN_2) == 1) {
            press_10ms_cnt++;
        }
        else if (bflb_gpio_read(gpio, GPIO_PIN_2) == 0) {
            if (press_10ms_cnt > 10 && press_10ms_cnt < 100) {
                press_mode = 1;
            }
            else if (press_10ms_cnt > 100) {
                press_mode = 2;
            }
            press_10ms_cnt = 0;
        }

        if (1 == press_mode) {
            press_mode = 0;
            printf("1 == press_mode\r\n");
            if (button) {
                play_voice_open();
            }
            else {
                play_voice_close();
            }
            button = !button;
        }
        else if (2 == press_mode) {
            press_mode = 0;
            led_ctrl = !led_ctrl;
            printf("led_ctrl:%d\r\n", led_ctrl);
            // if(led_ctrl){
            //     bflb_gpio_set(gpio, GPIO_PIN_16);
            // }
            // else{
            //     bflb_gpio_reset(gpio, GPIO_PIN_16);
            // }
        }

        vTaskDelay(10);
    }
}

int main(void)
{
    board_init();
    // audio_init();
    audio_pcm_init();
    // es8388_voice_init();
    // es8388test_init();

    // uart0 = bflb_device_get_by_name("uart0");
    // shell_init_with_task(uart0);

    // tcpip_init(NULL, NULL);
    // wifi_start_firmware_task();
    // lwip_sntp_init();

    printf("button_process_task\r\n");
    xTaskCreate(button_process_task, (char*)"button_proc_task", button_PROCESS_STACK_SIZE, NULL, button_PROCESS_PRIORITY, &button_process_task_hd);
    vTaskStartScheduler();


    while (1) {
    }
}

