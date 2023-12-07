#define __RADAR_UART_TASK_C_
#include "radar_uart.h"
#include "bflb_mtimer.h"
#include "bflb_uart.h"
#include "board.h"
#include "bflb_gpio.h"

TaskHandle_t radar_uart_task_xhandle;
volatile struct bflb_device_s *uartx;

volatile uart_flag = 0;
volatile uint8_t uart_flag_end = 0;
volatile uint32_t uart_count = 0;
volatile uint32_t uart_flag_count = 0;
volatile uint32_t uart_end_flag = 0;
volatile uint32_t uart_count_number = 0;
volatile uint8_t buff[30] = { 0 };
volatile uint8_t buff_count = 0;

extern xQueueHandle radar_queue;

typedef struct
{
    int32_t pos_x;
    int32_t pos_y;
    int32_t speed;
    int32_t resolution;
} radar_type_t;

typedef struct
{
    radar_type_t radar_one;
    radar_type_t radar_two;
    radar_type_t radar_three;
} radar_three_type_t;

radar_three_type_t radar_value;

uint8_t uart_parse_request(uint8_t *buf)
{
    radar_value.radar_one.pos_x = buf[0] + buf[1] * 256;
    radar_value.radar_one.pos_y = buf[2] + buf[3] * 256;
    radar_value.radar_one.speed = buf[4] + buf[5] * 256;
    radar_value.radar_one.resolution = buf[6] + buf[7] * 256;

    radar_value.radar_two.pos_x = buf[8] + buf[9] * 256;
    radar_value.radar_two.pos_y = buf[10] + buf[11] * 256;
    radar_value.radar_two.speed = buf[12] + buf[13] * 256;
    radar_value.radar_two.resolution = buf[14] + buf[15] * 256;

    radar_value.radar_three.pos_x = buf[16] + buf[17] * 256;
    radar_value.radar_three.pos_y = buf[18] + buf[19] * 256;
    radar_value.radar_three.speed = buf[20] + buf[21] * 256;
    radar_value.radar_three.resolution = buf[22] + buf[23] * 256;
}

void uart1_send(uint8_t *buf, uint16_t length)
{
    uint16_t i = 0;
    for (i = 0; i < length; i++) {
        bflb_uart_putchar(uartx, buf[i]);
    }
}

void multiplayer_uart_send(void)
{
    uint8_t many_buf[] = { 0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x90, 0x00, 0x04, 0x03, 0x02, 0x01 };
    uart1_send(many_buf, 12);
}

void single_uart_send(void)
{
    uint8_t many_buf[] = { 0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x80, 0x00, 0x04, 0x03, 0x02, 0x01 };
    uart1_send(many_buf, 12);
}

void radar_uart_task_process(void *msg)
{
    struct bflb_device_s *gpio;

    uint8_t value = 0;

    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_uart_init(gpio, GPIO_PIN_26, GPIO_UART_FUNC_UART1_TX);
    bflb_gpio_uart_init(gpio, GPIO_PIN_28, GPIO_UART_FUNC_UART1_RX);

    uartx = bflb_device_get_by_name("uart1");

    struct bflb_uart_config_s cfg;

    // cfg.baudrate = 115200;
    cfg.baudrate = 256000;
    cfg.data_bits = UART_DATA_BITS_8;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.parity = UART_PARITY_NONE;
    cfg.flow_ctrl = 0;
    cfg.tx_fifo_threshold = 7;
    cfg.rx_fifo_threshold = 7;
    bflb_uart_init(uartx, &cfg);

    int ch;

    

    while (1) {
        // uart1_send("Hello World!\r\n", sizeof("Hello World!\r\n"));
        ch = bflb_uart_getchar(uartx);
        if (ch != -1) {
            uart_flag_end = 1;
            uart_count_number = xTaskGetTickCount();
            if (uart_flag) {
                buff[buff_count] = (uint8_t)ch;
                buff_count++;
                switch (uart_flag_count) {
                    case 0:
                        if (ch == 0x55) {
                            uart_flag_count = 1;
                        } else {
                            uart_flag_count = 0;
                        }
                        break;
                    case 1:
                        if (ch == 0xCC) {
                            uart_end_flag = 1;
                            uart_flag = 0;
                        } else {
                            uart_flag_count = 0;
                        }
                        break;
                    default:
                        break;
                }
            } else {
                switch (uart_count) {
                    case 0:
                        if (ch == 0xAA) {
                            uart_count = 1;
                        } else {
                            uart_count = 0;
                        }
                        break;
                    case 1:
                        if (ch == 0xFF) {
                            uart_count = 2;
                        } else {
                            uart_count = 0;
                        }
                        break;
                    case 2:
                        if (ch == 0x03) {
                            uart_count = 3;
                        } else {
                            uart_count = 0;
                        }
                        break;
                    case 3:
                        if (ch == 0x00) {
                            uart_flag = 1;
                            buff_count = 0;
                            buff[24] = 0;
                            buff[25] = 0;
                        } else {
                            uart_count = 0;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (uart_end_flag) {
                uart_end_flag = 0;
                if ((buff[24] == 0x55) && (buff[25] == 0xCC)) {
                    uart_count = 0;
                    buff[24] = 0;
                    buff[25] = 0;
                    buff_count = 0;
                    uart_flag = 0;
                    uart_flag_end = 0;
                    uart_end_flag = 0;

                    uart_parse_request(buff);

                    xQueueSend(radar_queue, &radar_value, 0);

                    // printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", radar_one.pos_x, radar_one.pos_y, radar_one.speed, radar_one.resolution);
                    // printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", radar_two.pos_x, radar_two.pos_y, radar_two.speed, radar_two.resolution);
                    // printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", radar_three.pos_x, radar_three.pos_y, radar_three.speed, radar_three.resolution);
                    // uart1_send("Hello World222!\r\n", sizeof("Hello World222!\r\n"));
                }
            }
        }

        if (uart_flag_end) {
            if (xTaskGetTickCount() - uart_count_number > 200) {
                // value = uart_parse_request(buff, buff_count);
                // xQueueSend(button_queue, &value, 0);
                // printf("uart value is 0x%02x\r\n", value);
                // uart1_send("Hello World!\r\n", sizeof("Hello World!\r\n"));
                uart_count = 0;
                buff[24] = 0;
                buff[25] = 0;
                buff_count = 0;
                uart_flag = 0;
                uart_flag_end = 0;
                uart_end_flag = 0;
            }
        }
        vTaskDelay(1);
    }
}
