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
#include "kvm_dev.h"

#define DBG_TAG "MAIN"

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
    aipi_kvm_set_usb_channel(KVM_USB_CH_3);
    aipi_kvm_set_HDIM_channel(KVM_HDMI_CH_3);
    while (1) {
        bl61x_get_heap_size();
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}

int main(void)
{
    board_init();
    aipi_kvm_dev_init();
    xTaskCreate(bl61x_show_heap_size_task, (char*)"heap", 512, NULL, 2, NULL);

    vTaskStartScheduler();
}
