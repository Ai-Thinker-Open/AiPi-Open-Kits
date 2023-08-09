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
#include "lv_user_config.h"
//guiGuider
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "buttom_led.h"
//esay flashg

#include "user_esflash.h"
lv_ui guider_ui;


/*****************  extern  ************/
static int bl61x_get_heap_size(void);

static void bl61x_show_heap_size_task(void* arg)
{

    while (1) {
        bl61x_get_heap_size();
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}
/**************************************/
int main(void)
{
    board_init();
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    //init easyflash
    esay_flash_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    events_init(&guider_ui);
    aipi_scp_buttom_led_init();
    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 1, NULL);

    xTaskCreate(bl61x_show_heap_size_task, (char*)"heap", 512, NULL, 2, NULL);
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
    LOG_F("[%s:%d]heap_size------->KMEM_HEAP=%d  PMEM_HEAP=%d", DBG_TAG, __LINE__, info1.free_size, info2.free_size);
    return total_free_size;
}