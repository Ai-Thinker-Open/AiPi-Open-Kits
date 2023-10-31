/**
 * @file wifi6_led.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "wifi6_led.h"
#include "log.h"
#include "bflb_gpio.h"
#include "user_state.h"
#define DBG_TAG "LED"

struct bflb_device_s* gpio;
static xTaskHandle configNet_task;
static xTaskHandle configNet_stop;
/**
 * @brief  configNET_start
 *
 * @param arg
*/
static void configNET_start(void* arg)
{
    while (1) {
        bflb_gpio_set(gpio, LED_CONFIG_NET);
        vTaskDelay(pdMS_TO_TICKS(200));
        bflb_gpio_reset(gpio, LED_CONFIG_NET);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
/**
 * @brief
 *
*/
void led_init(void)
{
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, LED_CONFIG_NET, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, LED_DEV, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    bflb_gpio_reset(gpio, LED_DEV);
    xTaskCreate(configNET_start, "configNET_task", 1024, NULL, 2, &configNet_task);
    vTaskSuspend(configNet_task);
}
/**
 * @brief
 *
*/
static void led_configNET_start(void)
{
    if (configNet_task==NULL) {
        LOG_E("config NET task not created");
        return;
    }
    vTaskResume(configNet_task);
}

static void configNET_stop(void* arg)
{
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    bflb_gpio_set(gpio, LED_CONFIG_NET);
    vTaskDelay(pdMS_TO_TICKS(50));
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    vTaskDelay(pdMS_TO_TICKS(50));
    bflb_gpio_set(gpio, LED_CONFIG_NET);
    vTaskDelay(pdMS_TO_TICKS(50));
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    vTaskDelete(NULL);
}


/**
 * @brief
 *
*/
static void led_configNET_stop(void)
{
    if (configNet_task==NULL) {
        LOG_E("config NET task not created");
        return;
    }
    vTaskSuspend(configNet_task);
    xTaskCreate(configNET_stop, "configNET_task", 1024, NULL, 4, &configNet_stop);
}

static void configNET_server_CNT(void* arg)
{
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    bflb_gpio_set(gpio, LED_CONFIG_NET);
    vTaskDelay(pdMS_TO_TICKS(2000));
    bflb_gpio_reset(gpio, LED_CONFIG_NET);
    vTaskDelete(NULL);
}

void led_indicator_change(int led_state)
{
    user_state_t state = led_state;
    switch (state)
    {
        case STATE_WIFI_CONNECTING:
            /* code */
            led_configNET_start();
            break;
        case STATE_WIFI_CONNECTED_OK:
            led_configNET_stop();
            break;
        case STATE_MQTT_SERVER_CONENCT_OK:
        {
            //创建任务，常亮2S 灯
            xTaskCreate(configNET_server_CNT, "server led", 1024, NULL, 4, NULL);
        }
        break;
        default:
            break;
    }
}

void led_dev_open(int* led_state)
{
    bflb_gpio_set(gpio, LED_DEV);
    *led_state = true;
}

void led_dev_cloes(int* led_state)
{
    bflb_gpio_reset(gpio, LED_DEV);
    *led_state = false;
}



