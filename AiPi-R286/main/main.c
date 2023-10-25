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
#include "mem.h"
#include "device.h"

#define DBG_TAG "MAIN"

aipi_r286_dev_t* r2_device;

int main(void)
{
    board_init();

    //init easyflash
    r2_device = aipi_r286_device_init();
    vTaskStartScheduler();
}
