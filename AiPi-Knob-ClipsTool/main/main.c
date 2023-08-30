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

#include "easyflash.h"
#include "bflb_mtd.h"
//lvgl
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
//GuiGuider UI
#include "lv_user_config.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
//BLE
#include "ble_hid_dev.h"
//WiFi
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "wifi_event.h"

//custom
#include "encoder.h"
#define DBG_TAG "MAIN"

/**************************************/

/*****************  extern  ************/

lv_ui guider_ui;
/**************************************/
static int bl61x_get_heap_size(void);
static void bl61x_show_heap_size_task(void* arg)
{
    while (1) {
        bl61x_get_heap_size();
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}

int main(void)
{
    board_init();

    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();//一定要使用，不然启动不了BLE

    bflb_mtd_init();
    easyflash_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    events_init(&guider_ui);
    // indev_encoder_init();

    lv_log_register_print_cb(lv_log_print_g_cb);

    //hid 任务
    xTaskCreate(ble_hid_task, "ble_hid_task", 1024, NULL, 2, (xTaskHandle*)&hid_task);
    //lvgl 心跳任务
    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 3, NULL);
    //内存打印任务
    xTaskCreate(bl61x_show_heap_size_task, (char*)"heap", 1024, NULL, 4, NULL);
    //旋钮任务
    xTaskCreate(button_process_task, (char*)"encoder", 1024, (void*)&guider_ui, 5, (xTaskHandle*)&encoder_task);
    vTaskStartScheduler();
}

static int bl61x_get_heap_size(void)
{
    struct meminfo info1 = { 0 };
    struct meminfo info2 = { 0 };
    uint32_t total_free_size = 0;
    // return xPortGetFreeHeapSize();

    bflb_mem_usage(KMEM_HEAP, &info1);
    bflb_mem_usage(PMEM_HEAP, &info2);

    total_free_size = info1.free_size;
    LOG_F("heap size  KMEM heap=%d   PMEM heap=%d", info1.free_size, info2.free_size);
    return total_free_size;
}

