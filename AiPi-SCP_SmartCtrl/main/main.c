/**
 * @file mian.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-03
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
#include "mem.h"

//easy flash
#include "easyflash.h"
#include "bflb_mtd.h"
#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
//lvgl
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

int main(void)
{
    board_init();
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    //init easyflash
    bflb_mtd_init();
    easyflash_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    vTaskStartScheduler();
}