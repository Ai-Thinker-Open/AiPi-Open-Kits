/**
 * @file relays_dev.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "relays_dev.h"
#include "aipi_cloud.h"
#define DBG_TAG "relays"

static struct bflb_device_s* gpio;

static relays_dev_t relay_sw1 = {
    .gpio_pin = RELAYS_SW1,
    .relays_name = "SW1",
};

static relays_dev_t relay_sw2 = {
    .gpio_pin = RELAYS_SW2,
    .relays_name = "SW2",
};
static relays_dev_t relay_sw3 = {
    .gpio_pin = RELAYS_SW3,
    .relays_name = "SW3",
};
/**
 * @brief
 *
 * @param relays
*/
static void relays_on(relays_dev_t* relays)
{
    bflb_gpio_reset(gpio, relays->gpio_pin);
    relays->gpio_state = true;
    aiio_log_d("%s->on", relays->relays_name);
}
/**
 * @brief
 *
 * @param relays
*/
static void relays_off(relays_dev_t* relays)
{
    bflb_gpio_set(gpio, relays->gpio_pin);
    relays->gpio_state = false;
    aiio_log_d("%s->off", relays->relays_name);

}

void relays_device_init(void* device_t)
{
    aipi_r286_dev_t* device = (aipi_r286_dev_t*)device_t;
    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_init(gpio, RELAYS_SW1, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, RELAYS_SW2, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, RELAYS_SW3, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    bflb_gpio_set(gpio, RELAYS_SW1);
    bflb_gpio_set(gpio, RELAYS_SW2);
    bflb_gpio_set(gpio, RELAYS_SW3);

    relay_sw1.on = relays_on;
    relay_sw2.on = relays_on;
    relay_sw3.on = relays_on;
    relay_sw1.off = relays_off;
    relay_sw2.off = relays_off;
    relay_sw3.off = relays_off;
    device->relay_sw1 = &relay_sw1;
    device->relay_sw2 = &relay_sw2;
    device->relay_sw3 = &relay_sw3;
}
