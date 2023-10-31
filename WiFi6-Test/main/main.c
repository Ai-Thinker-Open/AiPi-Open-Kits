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

#include "wifi_event.h"
#include "wifi6_led.h"
#include "user_state.h"
#include "user_esflash.h"

#define DBG_TAG "MAIN"


int main(void)
{
    board_init();
    esay_flash_init();
    led_init();
    user_state_start();
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    user_state_send_notify(STATE_SYSTEM_START, 0);
    vTaskStartScheduler();
}
