/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-29
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
//GuiGuider UI
#include "gui_guider.h"
#include "custom.h"
#include "wifi_event.h"
#include "lv_user_config.h"

/**********   user define   *************/
#define DBG_TAG "MIAN"

/**************************************/
/*****************  extern  ************/

lv_ui guider_ui;
/**************************************/

/*****************  extern  ************/
static int bl61x_get_heap_size(void);

static void bl61x_show_heap_size_task(void* arg)
{
    while (1) {
        printf("[%s:%d]heap_size-------> %d\r\n", DBG_TAG, __LINE__, bl61x_get_heap_size());
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}
/**************************************/

int main(void)
{
    board_init();

    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    // wifi_mgmr_ap_stop();
    //init easyflash
    bflb_mtd_init();
    easyflash_init();
    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 1, NULL);
    xTaskCreate(bl61x_show_heap_size_task, (char*)"heap", 1024, NULL, 2, NULL);

    vTaskStartScheduler();
    // while (1) {

    //     vTaskDelay(1000/portTICK_PERIOD_MS);
    // }
}

static int bl61x_get_heap_size(void)
{
    struct meminfo info1 = { 0 };
    struct meminfo info2 = { 0 };
    uint32_t total_free_size = 0;
    // return xPortGetFreeHeapSize();

    bflb_mem_usage(KMEM_HEAP, &info1);
    bflb_mem_usage(PMEM_HEAP, &info2);

    total_free_size = info1.free_size + info2.free_size;

    return total_free_size;
}