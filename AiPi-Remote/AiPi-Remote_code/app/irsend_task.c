#define __IRSEND_TASK_C_
#include "irsend_task.h"
#include "module_wireless.h"

TaskHandle_t irsend_task_xhandle;

module_wireless_send_type_t module_wireless_send;
module_wireless_send_config_t module_wireless_send_config;
module_wireless_dat_t module_wireless_send_nec_dat_one;
module_wireless_dat_t module_wireless_send_nec_dat_zero;
module_wireless_dat_t module_wireless_send_nec_start;
module_wireless_dat_t module_wireless_send_nec_end;

#include "gui_guider.h"
#include "custom.h"

struct bflb_device_s *gpio_ir;

typedef union
{
    struct
    {
        uint8_t reserve : 5;
        uint8_t wind : 3;
    } bits;
    uint8_t data; // 风速
} data2_type_t;

typedef union
{
    struct
    {
        uint8_t reserve : 2;
        uint8_t mode : 2;
        uint8_t temp : 4;
    } bits;
    uint8_t data; // 风速
} data3_type_t;

uint8_t data1_value = 0xB2;
data2_type_t data2_value;
data3_type_t data3_value;
uint8_t onoff_status = 0;

uint8_t temp_list[] = {0x0, 0x1, 0x3, 0x2, 0x6, 0x7, 0x5, 0x4, 0xC, 0xD, 0x9, 0x8, 0xA, 0xB, 0xE};
uint8_t mode_list[] = {0x0, 0x1, 0x2, 0x3};
uint8_t wind_list[] = {0x0, 0x1, 0x2, 0x4, 0x5};

uint8_t temp_count = 0;
uint8_t mode_count = 0;
uint8_t wind_count = 0;

void module_wireless_send_config_delay(void)
{
    arch_delay_us(5);
}

void module_wireless_send_config_38k_delay(void)
{
    arch_delay_us(12);
}

void module_wireless_send_config_write(uint8_t status)
{
    if (status)
    {
        bflb_gpio_set(gpio_ir, GPIO_PIN_13);
    }
    else
    {
        bflb_gpio_reset(gpio_ir, GPIO_PIN_13);
    }
}

void irsend(uint8_t data1, uint8_t data2, uint8_t data3)
{
    module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data1);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data1);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data3);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data3);
    module_wireless_send.module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_end);

    module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data1);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data1);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, data3);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~data3);
    module_wireless_send.module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_end);
}
extern lv_ui guider_ui;
void irsend_task_process(void *msg)
{
    uint16_t button_value = 0;
    gpio_ir = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio_ir, GPIO_PIN_13, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    module_wireless_send_config.module_wireless_send_config_38k_delay = module_wireless_send_config_38k_delay;
    module_wireless_send_config.module_wireless_send_config_delay = module_wireless_send_config_delay;
    module_wireless_send_config.module_wireless_send_config_write = module_wireless_send_config_write;
    module_wireless_send_create(&module_wireless_send, &module_wireless_send_config);

    module_wireless_send_nec_dat_one.encoding_low = 22;
    module_wireless_send_nec_dat_one.encoding_high = 300;
    module_wireless_send_nec_dat_one.direction = 0;

    module_wireless_send_nec_dat_zero.encoding_low = 22;
    module_wireless_send_nec_dat_zero.encoding_high = 100;
    module_wireless_send_nec_dat_zero.direction = 0;

    module_wireless_send_nec_start.encoding_low = 172;
    module_wireless_send_nec_start.encoding_high = 800;
    module_wireless_send_nec_start.direction = 0;

    module_wireless_send_nec_end.encoding_low = 22;
    module_wireless_send_nec_end.encoding_high = 800;
    module_wireless_send_nec_end.direction = 0;

    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    uint16_t lvgl_button;

    data2_value.bits.reserve = 0x1F;
    data2_value.bits.wind = 0x01;

    data3_value.bits.reserve = 0x00;
    data3_value.bits.mode = 0x00;
    data3_value.bits.temp = 0xD;

    while (1)
    {
        if (xQueueReceive(button_queue, &button_value, portMAX_DELAY))
        {
            switch (button_value)
            {
            case 1:
                printf("button value is 1\r\n");
                lvgl_button = 0x01;
                if (onoff_status == 0)
                {
                    onoff_status = 1;
                    // 打开空调
                    data1 = data1_value;
                    data2 = data2_value.data;
                    data3 = data3_value.data;
                    irsend(data1, data2, data3);
                }
                else
                {
                    onoff_status = 0;
                    // 关闭空调
                    data1 = 0xB2;
                    data2 = 0x7B;
                    data3 = 0xE0;
                    irsend(data1, data2, data3);
                }
                break;
            case 2:
                // 模式
                lvgl_button = 0x02;
                mode_count++;
                if (mode_count > 3)
                {
                    mode_count = 0;
                }
                data3_value.bits.mode = mode_list[mode_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                printf("button value is 2\r\n");
                break;
            case 3:
                // 风速
                lvgl_button = 0x03;
                wind_count++;
                if (wind_count > 4)
                {
                    wind_count = 0;
                }
                data2_value.bits.wind = wind_list[wind_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                printf("button value is 3\r\n");
                break;
            case 4:
                // 温度升高
                lvgl_button = 0x04;
                if (temp_count < 13)
                {
                    temp_count++;
                }

                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                printf("button value is 4\r\n");
                break;
            case 5:
                // 温度降低
                lvgl_button = 0x04;
                if (temp_count > 0)
                {
                    temp_count--;
                }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                printf("button value is 5\r\n");
                break;
            case 6:
                printf("button value is 6\r\n");
                break;
            case 7:
                printf("button value is 7\r\n");
                break;
            case 8:
                printf("button value is 8\r\n");
                break;
            case 0xFE:
                // 打开空调
                lvgl_button = 0x01;
                onoff_status = 1;
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xFD:
                // 关闭空调
                lvgl_button = 0x01;
                onoff_status = 0;
                data1 = 0xB2;
                data2 = 0x7B;
                data3 = 0xE0;
                irsend(data1, data2, data3);
                break;
            case 0xFC:
                // 温度升高
                lvgl_button = 0x04;
                if (temp_count < 13)
                {
                    temp_count++;
                }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xFB:
                // 温度降低
                lvgl_button = 0x04;
                if (temp_count > 0)
                {
                    temp_count--;
                }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xFA:
                // 更换模式
                lvgl_button = 0xA2;
                mode_count++;
                if (mode_count > 3)
                {
                    mode_count = 0;
                }
                data3_value.bits.mode = mode_list[mode_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xF9:
                // 风速
                lvgl_button = 0x03;
                wind_count++;
                if (wind_count > 4)
                {
                    wind_count = 0;
                }
                data2_value.bits.wind = wind_list[wind_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA1:
                lvgl_button = 0x01;
                if (onoff_status == 0)
                {
                    onoff_status = 1;
                    // 打开空调
                    data1 = data1_value;
                    data2 = data2_value.data;
                    data3 = data3_value.data;
                    irsend(data1, data2, data3);
                }
                else
                {
                    onoff_status = 0;
                    // 关闭空调
                    data1 = 0xB2;
                    data2 = 0x7B;
                    data3 = 0xE0;
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA2:
                // 风速
                lvgl_button = 0x03;
                wind_count++;
                if (wind_count > 4)
                {
                    wind_count = 0;
                }
                data2_value.bits.wind = wind_list[wind_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA3:
                lvgl_button = 0x02;
                mode_count++;
                if (mode_count > 3)
                {
                    mode_count = 0;
                }
                data3_value.bits.mode = mode_list[mode_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA4:
                lvgl_button = 0x04;
                if (temp_count > 0)
                {
                    temp_count--;
                }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA5:
                lvgl_button = 0x04;
                if (temp_count < 13)
                {
                    temp_count++;
                }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            case 0xA6:
                printf("test\r\n");
                lvgl_button = 0x04;

                temp_count = lv_arc_get_value(guider_ui.screen_arc_1) - 17;
                // if (temp_count < 13)
                // {
                //     temp_count++;
                // }
                data3_value.bits.temp = temp_list[temp_count];
                data1 = data1_value;
                data2 = data2_value.data;
                data3 = data3_value.data;
                if (onoff_status)
                {
                    irsend(data1, data2, data3);
                }
                break;
            default:
                break;
            }

            xQueueSend(lvgl_queue, &lvgl_button, 0);
        }
    }
}
