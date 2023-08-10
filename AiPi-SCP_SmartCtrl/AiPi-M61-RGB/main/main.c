/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "log.h"
#include "board.h"

#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"

#include "wifi_event.h"
#include "sys_status.h"
#include "user_mqtt.h"
#include "m62_rgb.h"
#define DBG_TAG "MAIN"

int main(void)
{
    board_init();
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    system_status_init();

    xTaskCreate(wifi_connect_task, "wifi task", 1024, NULL, 1, NULL);
    dev_rgb_init();
    vTaskStartScheduler();
}