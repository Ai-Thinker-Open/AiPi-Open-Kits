/**
 * @file device.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#include "device.h"

#define DBG_TAG "DEVICE"

lv_ui guider_ui;

static aipi_r286_dev_t device;

aipi_r286_dev_t* aipi_r286_device_init(void)
{

    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    bflb_mtd_init();
    easyflash_init();

    relays_device_init(&device);

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    device.ui = &guider_ui;
    setup_ui(device.ui);
    custom_init(device.ui);
    events_init(device.ui);

    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 1, NULL);
    return &device;
}