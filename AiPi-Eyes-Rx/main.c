/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *
 */
#include "board.h"
#include "bflb_gpio.h"
#include "bflb_l1c.h"
#include "bflb_mtimer.h"
#include <FreeRTOS.h>
#include <task.h>

#include "app_main.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "demos/lv_demos.h"

#include "lcd.h"

#if DEF_USER_ES8388_EN
#include "es8388_task.h"
#else
#include "auadc.h"
#include "audac.h"
#endif

#if DEF_NXP_EN
#include "./demos/ai_lvgl/src/generated/gui_guider.h"
#include "./demos/ai_lvgl/src/generated/events_init.h"
lv_ui guider_ui;
#else
#include "demos/ai_lvgl/ui.h"
#endif
#include "usbh_core.h"	//usb

#define button_PROCESS_STACK_SIZE  (1024)
#define button_PROCESS_PRIORITY (14)
static TaskHandle_t button_process_task_hd;

/* lvgl log cb */
void lv_log_print_g_cb(const char *buf)
{
    printf("[LVGL] %s", buf);
}

/**
 * Create a QR Code
 */
void lv_example_qrcode_1(void)
{
    lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);

    lv_obj_t *qr = lv_qrcode_create(lv_scr_act(), 150, fg_color, bg_color);

    /*Set data*/
    const char *data = "https://www.bouffalolab.com";
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_center(qr);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(qr, bg_color, 0);
    lv_obj_set_style_border_width(qr, 5, 0);
}

static void user_task(void *pvParameters)
{
    lv_task_handler();

    printf("[LVGL] init success \r\n");
    while (1) {

        lv_task_handler();
        bflb_mtimer_delay_ms(1);
    }
}
static void button_process_task(void *param)
{
    uint32_t press_10ms_cnt = 0;
    uint32_t press_mode = 0;
    struct bflb_device_s *gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
    while(1){
        if(bflb_gpio_read(gpio, GPIO_PIN_2) == 1) {
            press_10ms_cnt++;
        }
        else if(bflb_gpio_read(gpio, GPIO_PIN_2) == 0){
            if(press_10ms_cnt > 10 && press_10ms_cnt < 100){
                press_mode = 1;
            }
            else if(press_10ms_cnt > 100){
                press_mode = 2;
            }
            press_10ms_cnt = 0;
        }

        if(1 == press_mode){
            press_mode = 0;
            printf("[key] key Press\r\n");
#if DEF_USER_ES8388_EN
            es8388_paly_en();
#else
            record_play_on();
#endif
        } else if (2 == press_mode) {
            press_mode = 0;
#if (0 == DEF_USER_ES8388_EN)
            record_play_off();
#endif
        }

        vTaskDelay(10);
    }
}

int main(void)
{
    board_init();
    printf("\r\n---BOARD:M61EVB-R%d VERSION:%s---\r\n", DEF_BOARD_INFO, DEF_BOARD_VERSION);

    //usb video init
    usbh_initialize();

    /* lvgl init */
    // lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    // test case
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_widgets();

#if DEF_NXP_EN
	setup_ui(&guider_ui);
   	events_init(&guider_ui);
#else
    ui_init();
#endif

#if DEF_USER_ES8388_EN
    /* i2s es8388 task start */
    es8388_palyer_task();
#else
    audio_play_task_init();
#endif

    xTaskCreate(user_task, (char *)"user_task", 2048, NULL, 3, NULL);
    xTaskCreate(button_process_task, (char *)"button_proc_task", button_PROCESS_STACK_SIZE, NULL, button_PROCESS_PRIORITY, &button_process_task_hd);

    /* freeRTOS start */
    vTaskStartScheduler();
}
