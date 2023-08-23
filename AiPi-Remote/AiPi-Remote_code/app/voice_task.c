#define __VOICE_TASK_C_
#include "voice_task.h"

TaskHandle_t voice_task_xhandle;
volatile struct bflb_device_s *uartx;

uint8_t uart_flag = 0;
uint32_t uart_count = 0;
uint8_t buff[10] = {0};
uint8_t buff_count = 0;

uint8_t uart_parse_request(uint8_t *buf, uint16_t length)
{
    uint8_t value = 0xFF;
    if (length != 4)
    {
        return 0xFF;
    }
    if ((buf[0] == 0x55) && (buf[1] == 0x55) && (buf[3] == 0x55))
    {
        value = buf[2];
    }

    return value;
}

void uart1_send(uint8_t *buf, uint16_t length)
{
    uint16_t i = 0;
    for (i = 0; i < length; i++)
    {
        bflb_uart_putchar(uartx, buf[i]);
    }
}

void voice_task_process(void *msg)
{
    struct bflb_device_s *gpio;

    uint8_t value = 0;

    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_uart_init(gpio, GPIO_PIN_26, GPIO_UART_FUNC_UART1_TX);
    bflb_gpio_uart_init(gpio, GPIO_PIN_28, GPIO_UART_FUNC_UART1_RX);

    uartx = bflb_device_get_by_name("uart1");

    struct bflb_uart_config_s cfg;

    cfg.baudrate = 115200;
    cfg.data_bits = UART_DATA_BITS_8;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.parity = UART_PARITY_NONE;
    cfg.flow_ctrl = 0;
    cfg.tx_fifo_threshold = 7;
    cfg.rx_fifo_threshold = 7;
    bflb_uart_init(uartx, &cfg);

    int ch;

    while (1)
    {
        // uart1_send("Hello World!", sizeof("Hello World!"));
        ch = bflb_uart_getchar(uartx);
        if (ch != -1)
        {
            uart_flag = 1; // 表示启动接收
            uart_count = xTaskGetTickCount();
            buff[buff_count] = (uint8_t)ch;

            buff_count++;
            if (buff_count > 9)
            {
                buff_count = 9;
            }

            // printf("0x%02x\r\n", ch);
            // bflb_uart_putchar(uartx, ch);
        }

        if (uart_flag)
        {
            if (xTaskGetTickCount() - uart_count > 200)
            {
                value = uart_parse_request(buff, buff_count);
                xQueueSend(button_queue, &value, 0);
                // printf("uart value is 0x%02x\r\n", value);
                buff_count = 0;
                uart_flag = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
