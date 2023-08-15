#define __IR_RECEIVE_C_
#include "ir_receive.h"
#include "module_wireless.h"
#include "driver/gpio.h"
#include "driver/hw_timer.h"

TaskHandle_t ir_receive_system_task_xhandle;

// GPIO_INTR_POSEDGE = 1,    /*!< GPIO interrupt type : rising edge */
// GPIO_INTR_NEGEDGE = 2,    /*!< GPIO interrupt type : falling edge */

static xQueueHandle gpio_evt_queue = NULL;

module_wireless_receive_config_t module_wireless_receive_config;
module_wireless_receive_type_t module_wireless_receive;
module_wireless_dat_t module_wireless_receive_dat_buf[50];
uint16_t module_wireless_receive_lenght;
module_wireless_dat_t module_wireless_receive_buf[50];

module_wireless_nec_dat_t module_wireless_nec_dat;
uint8_t nec_buf[4];
;
uint8_t isr_flag = 0;

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
    uint32_t io_num;

    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            // printf("GPIO[%d] intr, val: %d\n", io_num, 1);
            if (isr_flag)
            {
                isr_flag = 0;
                gpio_set_intr_type(4, GPIO_INTR_POSEDGE);
                // printf("GPIO[%d] intr, val: %d\n", io_num, 1);
                // 设置成上升沿触发，当前触发为下降沿
                module_wireless_receive.module_wireless_receive_falling(&module_wireless_receive);
            }
            else
            {
                isr_flag = 1;
                gpio_set_intr_type(4, GPIO_INTR_NEGEDGE);
                // printf("GPIO[%d] intr, val: %d\n", io_num, 2);
                // 设置成下降沿触发，当前触发为上升沿
                module_wireless_receive.module_wireless_receive_rising(&module_wireless_receive);
            }
        }
    }
}

static volatile uint32_t tick_count = 0;
void hw_timer_callback1(void *arg)
{
    tick_count++;
}

uint32_t module_wireless_receive_config_tick(void)
{
    return tick_count;
}

void ir_receive_system_task(void *p_arg)
{
    printf("ir_receive_system_task\r\n");
    uint16_t i = 0;
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = (1ULL << 4);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;

    gpio_config(&io_conf);

    hw_timer_init(hw_timer_callback1, NULL);
    hw_timer_alarm_us(51, true);

    // create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    // start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 25, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(4, gpio_isr_handler, (void *)4);

    module_wireless_receive_config.module_wireless_receive_start_flag = 0;
    module_wireless_receive_config.module_wireless_receive_start_direction = 1;
    module_wireless_receive_config.module_wireless_receive_encoding_offset = 0;
    module_wireless_receive_config.module_wireless_receive_waiting_time = 20; // 200ms
    module_wireless_receive_config.module_wireless_receive_buf = module_wireless_receive_buf;
    module_wireless_receive_config.module_wireless_receive_buf_count = 0;

    module_wireless_receive_config.module_wireless_receive_config_tick = module_wireless_receive_config_tick;

    module_wireless_receive_create(&module_wireless_receive, &module_wireless_receive_config);

    module_wireless_nec_dat.module_wireless_nec_start.encoding_low = 90;
    module_wireless_nec_dat.module_wireless_nec_start.encoding_high = 180;
    module_wireless_nec_dat.module_wireless_nec_zero.encoding_low = 11;
    module_wireless_nec_dat.module_wireless_nec_zero.encoding_high = 11;
    module_wireless_nec_dat.module_wireless_nec_one.encoding_low = 34;
    module_wireless_nec_dat.module_wireless_nec_one.encoding_high = 11;

    while (1)
    {
        if (module_wireless_receive.module_wireless_receive_read(&module_wireless_receive, module_wireless_receive_dat_buf, &module_wireless_receive_lenght) == 1)
        {
            if (module_wireless_receive.module_wireless_receive_nec_analysis(&module_wireless_receive, &module_wireless_nec_dat, module_wireless_receive_dat_buf, module_wireless_receive_lenght, nec_buf) == 0)
            {
                for (i = 0; i < 4; i++)
                {
                    printf("0x%02x ", nec_buf[i]);
                }
                printf("\r\n");
            }
            // for (i = 0; i < module_wireless_receive_lenght; i++)
            // {
            //     printf("high_level_time:%d low_level_time:%d \r\n", module_wireless_receive_dat_buf[i].encoding_high, module_wireless_receive_dat_buf[i].encoding_low);
            // }
            // printf("lenght:%d\r\n", module_wireless_receive_lenght);
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
