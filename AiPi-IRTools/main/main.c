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
//easy flash
#include "easyflash.h"
#include "bflb_mtd.h"
#include "mem.h"
//UI
#include "lv_user_config.h"
#include "gui_guider.h"
#include "custom.h"
#include "events_init.h"
//
#include "encoder.h"
//state
#include "dev_state.h"
//
#include "dev_ir.h"
//flash
#include "user_esflash.h"
#define DBG_TAG "MAIN"

lv_ui guider_ui;


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
static void bl61x_show_heap_size_task(void* arg)
{
    while (1) {
        bl61x_get_heap_size();
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

int main(void)
{

    board_init();
    esay_flash_init();
    ef_get_int(IR_TYPE_T, &dev_ir_type);
    device_ir_init(dev_ir_type);

    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    events_init(&guider_ui);

    dev_state_init(&guider_ui);

    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 2, NULL);
    xTaskCreate(bl61x_show_heap_size_task, (char*)"lvgl", 512, NULL, 1, NULL);
    vTaskStartScheduler();
}
