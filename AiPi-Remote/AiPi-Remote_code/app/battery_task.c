#define __BATTERY_TASK_C_
#include "battery_task.h"

TaskHandle_t battery_task_xhandle;

struct bflb_device_s *gpio_battery;
uint8_t battery_flag_new = 0;
uint8_t battery_flag_old = 0;
uint8_t power_status;

extern uint8_t batt_status;
uint8_t batt_status_old;

void battery_task_process(void *msg)
{
    uint16_t lvgl_button = 0;
    gpio_battery = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio_battery, GPIO_PIN_14, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0); // ON/OFF
    while (1)
    {
        if (bflb_gpio_read(gpio_battery, GPIO_PIN_14))
        {
            battery_flag_new = 1;
        }
        else
        {
            battery_flag_new = 0;
        }
        if (battery_flag_new != battery_flag_old)
        {
            battery_flag_old = battery_flag_new;
            if (battery_flag_old == 1)
            {
                printf("battery trigger recharge!\r\n");
                power_status = 0x03;
                lvgl_button = 0x05;
                xQueueSend(lvgl_queue, &lvgl_button, 0);
            }
            else
            {
                printf("battery trigger Charge unplug!\r\n");
                power_status = batt_status_old;
                lvgl_button = 0x05;
                xQueueSend(lvgl_queue, &lvgl_button, 0);
            }
        }
        else
        {
            if (battery_flag_old == 0)
            {
                if (batt_status_old != batt_status)
                {
                    batt_status_old = batt_status;
                    power_status = batt_status_old;
                    lvgl_button = 0x05;
                    xQueueSend(lvgl_queue, &lvgl_button, 0);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
