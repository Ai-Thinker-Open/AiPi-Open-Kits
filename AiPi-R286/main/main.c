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
//wifi
#include "easyflash.h"
#include "bflb_mtd.h"
#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"

#include "wifi_event.h"
//lvgl
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lcd.h"
//GuiGuider UI
#include "gui_guider.h"
#include "lv_user_config.h"
#include "custom.h"
#include "events_init.h"

#define DBG_TAG "MAIN"

lv_ui guider_ui;

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

    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    events_init(&guider_ui);

    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 2048, NULL, 3, NULL);
    vTaskStartScheduler();
}
