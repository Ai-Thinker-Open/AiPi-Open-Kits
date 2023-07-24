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
#include "log.h"

#define GBD_TAG "UART"

static void uart_init(void);

static struct bflb_device_s* uart1;
static uint8_t uart_txbuf[128] = { 0 };

void voice_uart_task(void* arg)
{
    uart_init();
    while (1) {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

static void uart_isr(int irq, void* arg)
{
    uint32_t intstatus = bflb_uart_get_intstatus(uart1);
    uint32_t rx_data_len = 0;

    if (intstatus & UART_INTSTS_RX_FIFO) {
        LOG_I("rx fifo\r\n");
        while (bflb_uart_rxavailable(uart1)) {
            LOG_I("0x%02x\r\n", bflb_uart_getchar(uart1));
            rx_data_len++;
        }
    }
    if (intstatus & UART_INTSTS_RTO) {
        LOG_I("rto\r\n");
        char* data_type = pvPortMalloc(1);
        memset(uart_txbuf, 0, 128);
        while (bflb_uart_rxavailable(uart1)) {
            sprintf(data_type, "%c", bflb_uart_getchar(uart1));
            LOG_I("0x%02x\r\n", data_type);
            strcat(uart_txbuf, data_type);
        }
        vPortFree(data_type);
        bflb_uart_get(uart1, uart_txbuf, 5);
        LOG_I("uart recv:%s", uart_txbuf);
        bflb_uart_int_clear(uart1, UART_INTCLR_RTO);
    }

    if (intstatus & UART_INTSTS_TX_FIFO) {
        LOG_I("tx fifo\r\n");
        for (uint8_t i = 0; i < 27; i++) {
            bflb_uart_putchar(uart1, uart_txbuf[i]);
        }
        bflb_uart_txint_mask(uart1, true);
    }
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
        .flow_ctrl = 0,
        .tx_fifo_threshold = 7,
        .rx_fifo_threshold = 7,
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
}