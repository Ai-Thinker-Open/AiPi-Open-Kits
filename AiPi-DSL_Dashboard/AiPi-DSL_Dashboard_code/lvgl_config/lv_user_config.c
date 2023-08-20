/**
 * @file lv_user_config.c
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

#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
/**
 * @brief
 *
 * @param buf
*/
void lv_log_print_g_cb(const char* buf)
{
    LOG_I("[LVGL] %s", buf);
}

void lvgl_tick_task(void* param)
{

    while (1)
    {
        lv_task_handler();
        vTaskDelay(30/portTICK_PERIOD_MS);
    }
}