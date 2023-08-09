/**
 * @file buttom_led.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-08
 *
 * @copyright Copyright (c) 2023
 *
*/
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "bflb_gpio.h"
#include "board.h"
#include "buttom_led.h"
#include "log.h"

#define GBD_TAG "BOTTUM LED"

struct bflb_device_s* gpio;
extern xQueueHandle queue;

static void AiPi_SCP_LED_set(uint8_t _gpio, bool value);
/**
 * @brief gpio_isr
 *      GPIO 中断处理函数
 * @param irq
 * @param arg
*/
static void gpio_isr(int irq, void* arg)
{
    static int i_red = 1, i_green = 1, i_blue = 1;
    char* queue_buff = pvPortMalloc(64);
    memset(queue_buff, 0, 64);
    bool intstatus = bflb_gpio_get_intstatus(gpio, BUTTOM_RED_IO);
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    if (intstatus) {
        bflb_gpio_int_clear(gpio, BUTTOM_RED_IO); //清除中断标志
        AiPi_SCP_LED_set(LED_RED_IO, i_red%2);//点亮红灯
        //重置所有灯的控制按钮

        //发送消息
        sprintf(queue_buff, "{\"board_id\":3,\"status\":%d,\"RGB\":{\"R\":31,\"G\":0,\"B\":0}}", i_red%2); //所有设备亮红灯
        xQueueSendFromISR(queue, queue_buff, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        //红灯cout，用于下次led亮灭的控制
        i_red++;
        if (i_red>255)i_red = 0;

        goto __exit;
    }

    intstatus = bflb_gpio_get_intstatus(gpio, BUTTOM_GREEN_IO);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, BUTTOM_GREEN_IO);
        AiPi_SCP_LED_set(LED_GREEN_IO, i_green%2);
        sprintf(queue_buff, "{\"board_id\":3,\"status\":%d,\"RGB\":{\"R\":0,\"G\":31,\"B\":0}}", i_green%2);
        xQueueSendFromISR(queue, queue_buff, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        i_green++; if (i_green>255)i_green = 0;
        goto __exit;
    }
    intstatus = bflb_gpio_get_intstatus(gpio, BUTTOM_BLUE_IO);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, BUTTOM_BLUE_IO);
        AiPi_SCP_LED_set(LED_BLUE_IO, i_blue%2);

        sprintf(queue_buff, "{\"board_id\":3,\"status\":%d,\"RGB\":{\"R\":0,\"G\":0,\"B\":31}}", i_blue%2);
        xQueueSendFromISR(queue, queue_buff, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        i_blue++; if (i_blue>255) i_blue = 0;
        goto __exit;
    }

    intstatus = bflb_gpio_get_intstatus(gpio, BUTTOM_ALL_ON_IO);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, BUTTOM_ALL_ON_IO);
        AiPi_SCP_LED_set(LED_RED_IO, 1);
        AiPi_SCP_LED_set(LED_GREEN_IO, 1);
        AiPi_SCP_LED_set(LED_BLUE_IO, 1);

        i_red = 2;
        i_green = 2;
        i_blue = 2;


        sprintf(queue_buff, "{\"board_id\":3,\"status\":1,\"RGB\":{\"R\":0,\"G\":0,\"B\":0}}");//所有设备开
        xQueueSendFromISR(queue, queue_buff, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        goto __exit;
    }
    intstatus = bflb_gpio_get_intstatus(gpio, BUTTOM_ALL_OFF_IO);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, BUTTOM_ALL_OFF_IO);
        AiPi_SCP_LED_set(LED_RED_IO, 0);
        AiPi_SCP_LED_set(LED_GREEN_IO, 0);
        AiPi_SCP_LED_set(LED_BLUE_IO, 0);

        i_red = 1;
        i_green = 1;
        i_blue = 1;

        sprintf(queue_buff, "{\"board_id\":3,\"status\":0,\"RGB\":{\"R\":0,\"G\":0,\"B\":0}}");//所有设备关
        xQueueSendFromISR(queue, queue_buff, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        goto __exit;
    }
__exit:
    vPortFree(queue_buff);
}
/**
 * @brief
 *
*/
void aipi_scp_buttom_led_init(void)
{
    gpio = bflb_device_get_by_name("gpio");
    //LED 
    bflb_gpio_init(gpio, LED_RED_IO, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, LED_GREEN_IO, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, LED_BLUE_IO, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_reset(gpio, LED_RED_IO);
    bflb_gpio_reset(gpio, LED_GREEN_IO);
    bflb_gpio_reset(gpio, LED_BLUE_IO);

    //buttom
    bflb_gpio_int_init(gpio, BUTTOM_RED_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, BUTTOM_RED_IO, false);

    bflb_gpio_int_init(gpio, BUTTOM_GREEN_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, BUTTOM_GREEN_IO, false);

    bflb_gpio_int_init(gpio, BUTTOM_BLUE_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, BUTTOM_BLUE_IO, false);

    bflb_gpio_int_init(gpio, BUTTOM_ALL_ON_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, BUTTOM_ALL_ON_IO, false);

    bflb_gpio_int_init(gpio, BUTTOM_ALL_OFF_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, BUTTOM_ALL_OFF_IO, false);

    bflb_irq_attach(gpio->irq_num, gpio_isr, gpio);
    bflb_irq_enable(gpio->irq_num);
}

static void AiPi_SCP_LED_set(uint8_t _gpio, bool value)
{
    LOG_I("GPIO_PIN_%d is:%d", _gpio, bflb_gpio_read(gpio, _gpio));
    if (value) bflb_gpio_set(gpio, _gpio);
    else bflb_gpio_reset(gpio, _gpio);
}