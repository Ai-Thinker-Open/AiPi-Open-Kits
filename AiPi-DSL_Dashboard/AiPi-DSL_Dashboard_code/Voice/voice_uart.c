/**
 * @file voice_uart.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <timers.h>
#include <task.h>
#include "custom.h"
#include "bflb_uart.h"
#include "bflb_gpio.h"
#include "voice_uart.h"
#include "user_mqtt.h"
#include "log.h"

#define GBD_TAG "UART"

static void uart_init(void);


extern xQueueHandle queue;
uart_rx_cmd_t uart_cmd;
static struct bflb_device_s* uart1;
static char uart_buff[4] = { 0 };

static const uart_data_t g_uart_buf[] = {
  {{0x5A, 0x4A, 0x01, 0x00}, 4}, //wakeup_uni
  {{0x5A, 0x4A, 0x02, 0x01}, 4}, //openL
  {{0x5A, 0x4A, 0x02, 0x02}, 4}, //closeL
  {{0x5A, 0x4A, 0x02, 0x03}, 4}, //checkWaether
  {{0x5A, 0x4A, 0x02, 0x04}, 4}, //connectMQTT
  {{0x5A, 0x4A, 0x02, 0x05}, 4}, //connectWiFi
  {{0x5A, 0x4A, 0x02, 0x06}, 4}, //openAPP1
  {{0x5A, 0x4A, 0x02, 0x07}, 4}, //openAPP2
  {{0x5A, 0x4A, 0x02, 0x08}, 4}, //openAPP3
  {{0x5A, 0x4A, 0x02, 0x09}, 4}, //openAPP4
  {{0x5A, 0x4A, 0x02, 0x0A}, 4}, //openAPP5
  {{0x5A, 0x4A, 0x02, 0x0B}, 4}, //openAPP6
  {{0x5A, 0x4A, 0x02, 0x0C}, 4}, //wifiScan

  {{0x5A, 0x4A, 0x02, 0x0D}, 4}, //volumeMute
  {{0x5A, 0x4A, 0x02, 0x0E}, 4}, //volumeNoMute
  {{0x5A, 0x4A, 0x02, 0x0F}, 4}, //volumeUp
  {{0x5A, 0x4A, 0x02, 0x10}, 4}, //volumeDown

  {{0x5A, 0x4A, 0x02, 0x11}, 4}, //openMusic
  {{0x5A, 0x4A, 0x02, 0x12}, 4}, //Music_next
  {{0x5A, 0x4A, 0x02, 0x13}, 4}, //Music_past
  {{0x5A, 0x4A, 0x02, 0x14}, 4}, //playMusic
  {{0x5A, 0x4A, 0x02, 0x15}, 4}, //playMusicPlus
  {{0x5A, 0x4A, 0x02, 0x16}, 4}, //openG
  {{0x5A, 0x4A, 0x02, 0x17}, 4}, //stopMusic
};

user_data_t user_data[10] = {
  {{{0x5B, 0x4B, 0x02, 0x00}, 4},"[0]"},
  {{{0x5B, 0x4B, 0x02, 0x03}, 4},"[111]"},
  {{{0x5B, 0x4B, 0x02, 0x04}, 4},"[113]"},
  {{{0x5B, 0x4B, 0x02, 0x05}, 4},"[114]"},
  {{{0x5B, 0x4B, 0x02, 0x06}, 4},"[116]"},
  {{{0x5B, 0x4B, 0x02, 0x07}, 4},"[117]"},
  {{{0x5B, 0x4B, 0x02, 0x08}, 4},"[118]"},
  {{{0x5B, 0x4B, 0x02, 0x09}, 4},"[119]"},
  {{{0x5B, 0x4B, 0x02, 0x0A}, 4},"[120]"},
  {{{0x5B, 0x4B, 0x02, 0x0B}, 4},"[121]"},
  {{{0x5B, 0x4B, 0x02, 0x0C}, 4},"[124]"},
  {{{0x5B, 0x4B, 0x02, 0x0D}, 4},"[125]"},

  {{{0x5B, 0x4B, 0x02, 0x0E}, 4},"[129]"},//切歌
  {{{0x5B, 0x4B, 0x02, 0x0F}, 4},"[123]"},//蓝牙已连接
  {{{0x5B, 0x4B, 0x02, 0x10}, 4},"[126]"},// 电脑已静音
  {{{0x5B, 0x4B, 0x02, 0x11}, 4},"[128]"},//静音取消
};

void voice_uart_task(void* arg)
{

    uart_init();
    while (1) {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

static int voice_cmd_check(char* buff, int len)
{

    int data_cnt = 0, j = 0;
    // char* uart_txbuf = pvPortMalloc(4);
    // memset(uart_txbuf, 0, 4);
    while (1) {
        for (size_t i = 0; i < 4; i++)
        {
            if (buff[i]==g_uart_buf[j].data[i]) data_cnt++;
            else break;
        }
        if (data_cnt==g_uart_buf->len) break;
        data_cnt = 0;
        j++;
        if (j>23) return -1;
    }
    LOG_I("check uart cmd=%d", j);
    return j;
}

static void uart_isr(int irq, void* arg)
{
    uint32_t intstatus = bflb_uart_get_intstatus(uart1);
    uint32_t rx_data_len = 0;
    char* queue_buff = pvPortMalloc(64);
    char data_type[1] = { 0 };
    if (intstatus & UART_INTSTS_RX_FIFO) {
        LOG_I("rx fifo\r\n");
        while (bflb_uart_rxavailable(uart1)) {
            LOG_I("0x%02x\r\n", bflb_uart_getchar(uart1));
        }
    }
    if (intstatus & UART_INTSTS_RTO) {
        LOG_I("rto");
        memset(uart_buff, 0, 4);
        while (bflb_uart_rxavailable(uart1)) {
            sprintf(data_type, "%c", bflb_uart_getchar(uart1));
            strcat(uart_buff, data_type);
        }
        LOG_I("uart recv:%02X %02X %02X %02X", uart_buff[0], uart_buff[1], uart_buff[2], uart_buff[3]);

        uart_cmd = voice_cmd_check(uart_buff, 4);

        bflb_uart_int_clear(uart1, UART_INTCLR_RTO);
        LOG_I("uart int clear");
    }

    if (intstatus & UART_INTSTS_TX_FIFO) {

        LOG_I("tx fifo\r\n");
        for (uint8_t i = 0; i < 27; i++) {
            bflb_uart_putchar(uart1, uart_buff[i]);
        }
        bflb_uart_txint_mask(uart1, true);

    }
    vPortFree(queue_buff);
}
/**
 * @brief
 *
*/
static void uart_init(void)
{
    struct bflb_device_s* gpio;
    struct bflb_uart_config_s cfg = {
        .baudrate = 115200,
        .data_bits = UART_DATA_BITS_8,
        .stop_bits = UART_STOP_BITS_1,
        .parity = UART_PARITY_NONE,
        .flow_ctrl = 0,
        .tx_fifo_threshold = 4,
        .rx_fifo_threshold = 4,
    };

    gpio = bflb_device_get_by_name("gpio");
    uart1 = bflb_device_get_by_name("uart1");

    bflb_gpio_uart_init(gpio, GPIO_PIN_25, GPIO_UART_FUNC_UART1_TX);
    bflb_gpio_uart_init(gpio, GPIO_PIN_26, GPIO_UART_FUNC_UART1_RX);

    bflb_uart_init(uart1, &cfg);

    bflb_uart_txint_mask(uart1, false);
    bflb_uart_rxint_mask(uart1, false);
    bflb_irq_attach(uart1->irq_num, uart_isr, NULL);
    bflb_irq_enable(uart1->irq_num);


    // static char data_type[1] = { 0 };
    // while (1) {
    //     while (bflb_uart_rxavailable(uart1)) {
    //         sprintf(data_type, "%c", bflb_uart_getchar(uart1));
    //         strcat(uart_buff, data_type);
    //     }
    //     LOG_I("uart recv:%02X %02X %02X %02X", uart_buff[0], uart_buff[1], uart_buff[2], uart_buff[3]);

    // }
}