#define __BUTTON_TASK_C_
#include "button_task.h"
#include "module_button.h"

TaskHandle_t button_task_xhandle;

module_button_type_t module_button;
module_button_config_t module_button_config;
uint16_t module_button_contimer_value[8] = {20, 20, 20, 20, 20, 20, 20, 20};          // 短按与双击的连续时间
uint16_t module_button_longtimer_value[8] = {300, 300, 300, 300, 300, 300, 300, 300}; // 长按时间值s

struct bflb_device_s *gpio_button;

uint8_t module_button_statusin(void) // 需要适配，按键输入引脚
{
    if (bflb_gpio_read(gpio_button, GPIO_PIN_19) == 0)
    {
        return 1;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_27) == 0)
    {
        return 2;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_29) == 0)
    {
        return 3;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_30) == 0)
    {
        return 4;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_31) == 0)
    {
        return 5;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_32) == 0)
    {
        return 6;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_33) == 0)
    {
        return 7;
    }
    if (bflb_gpio_read(gpio_button, GPIO_PIN_34) == 0)
    {
        return 8;
    }
    return 0;
}

void button_task_process(void *msg)
{
    uint16_t value = 0;
    gpio_button = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio_button, GPIO_PIN_19, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // ON/OFF   1
    bflb_gpio_init(gpio_button, GPIO_PIN_27, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // HOME     2
    bflb_gpio_init(gpio_button, GPIO_PIN_29, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // BACK     3
    bflb_gpio_init(gpio_button, GPIO_PIN_30, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // UP       4
    bflb_gpio_init(gpio_button, GPIO_PIN_31, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // DOWN     5
    bflb_gpio_init(gpio_button, GPIO_PIN_32, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // LEFT     6
    bflb_gpio_init(gpio_button, GPIO_PIN_33, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // RIGHT    7 
    bflb_gpio_init(gpio_button, GPIO_PIN_34, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0); // ENTER    8

    module_button_config.module_button_contimer_value = null;
    module_button_config.module_button_longtimer_value = module_button_longtimer_value;
    module_button_config.module_button_number = 8;
    module_button_config.module_button_statusin = module_button_statusin;
    module_button_create(&module_button, &module_button_config);

    while (1)
    {
        if (module_button.module_button_get_buttonvalue(&module_button, &value) == 1)
        {
            xQueueSend(button_queue, &value, 0);
            // printf("button value is %d\r\n", value);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
