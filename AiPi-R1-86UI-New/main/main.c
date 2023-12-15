/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include "board.h"
#include "log.h"

#include "lv_user_config.h"
#include "gui_guider.h"
#include "custom.h"
#include "events_init.h"
#include "lv_port_fs.h"
#include "device_ctrl.h"
#include "wifi_event.h"
#include "relays_dev.h"
#define DBG_TAG "MAIN"

lv_ui guider_ui;

int main(void)
{
    board_init();
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    relays_device_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    setup_ui(&guider_ui);

    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 2, NULL);
    xTaskCreate(device_state_task, "deivce", 1024*2, &guider_ui, 3, (TaskHandle_t*)&device_task);

    vTaskStartScheduler();
}
