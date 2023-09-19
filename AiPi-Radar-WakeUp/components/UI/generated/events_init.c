/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "log.h"
#include "custom.h"
#include "wifi_event.h"
#include "rd_01_recv.h"
#define DBG_TAG "LV-EVENT"



static xTimerHandle  loding_time;
//定时器旋转图片
static void TimerCallbackFunction_cb(TimerHandle_t xTimer)
{
    int	time_cout = (int)pvTimerGetTimerID(xTimer);
    lv_img_set_angle(guider_ui.Home_img_loding, time_cout*30*10);
    if (time_cout>=11) time_cout = 0;
    else
        time_cout++;
    vTimerSetTimerID(xTimer, (void*)time_cout);
}

void events_init(lv_ui* ui)
{
    loding_time = xTimerCreate("loding_time", pdMS_TO_TICKS(100), pdTRUE, 0, TimerCallbackFunction_cb);
}

static void Home_btn_connect_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            lv_obj_clear_flag(guider_ui.Home_img_loding, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.Home_cont_loding, LV_OBJ_FLAG_HIDDEN);
            xTimerStart(loding_time, pdMS_TO_TICKS(10));

            xTaskNotify(custom_status_task, CUSTOM_STATE_WIFI_START_CONNECT, eSetValueWithOverwrite);
        }
        break;
        default:
            break;
    }
}

static void Home_btn_2_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            lv_obj_clear_flag(guider_ui.Home_img_loding, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.Home_cont_loding, LV_OBJ_FLAG_HIDDEN);
            xTimerStart(loding_time, pdMS_TO_TICKS(10));
            xTaskNotify(custom_status_task, CUSTOM_STATE_WIFI_SCAN, eSetValueWithOverwrite);
        }
        break;
        default:
            break;
    }
}

static void Home_img_loding_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            lv_obj_add_flag(guider_ui.Home_img_loding, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.Home_cont_loding, LV_OBJ_FLAG_HIDDEN);
            xTimerStop(loding_time, pdMS_TO_TICKS(10));
        }
        break;
        default:
            break;
    }
}

static void Home_img_btn_3_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);
    uint16_t timer_out_min = 0;
    uint8_t timerout_sec = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            //获取PIN 码
            strcpy(ui->PIN, lv_textarea_get_text(ui->Home_ta_PIN));
            flash_erase_set("PIN", ui->PIN);
            //读取锁屏时间
            timer_out_min = atoi(lv_textarea_get_text(ui->Home_ta_min));
            timerout_sec = atoi(lv_textarea_get_text(ui->Home_ta_sec));

            if (timer_out_min>0) {
                ui->timerout = timer_out_min*60+timerout_sec;
            }
            else
                ui->timerout = timerout_sec;
            char* timerout_str = pvPortMalloc(16);
            memset(timerout_str, 0, 16);
            sprintf(timerout_str, "%d", ui->timerout);
            flash_erase_set("TIMER", timerout_str);
            vPortFree(timerout_str);

            LOG_I("Lock PC time=%d s", ui->timerout);
            if (xTimerIsTimerActive(ui->rd_01_nodet_time)!=pdFALSE)
                xTimerStopFromISR(ui->rd_01_nodet_time, &xHigherPriorityTaskWoken);
            xTimerChangePeriodFromISR(ui->rd_01_nodet_time, pdMS_TO_TICKS(ui->timerout*1000), &xHigherPriorityTaskWoken);//重新设置锁屏的时间
            rd_01_detected = 1;
        }
        break;
        default:
            break;
    }
}

void events_init_Home(lv_ui* ui)
{
    lv_obj_add_event_cb(ui->Home_btn_connect, Home_btn_connect_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Home_btn_2, Home_btn_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Home_img_loding, Home_img_loding_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Home_btn_3, Home_img_btn_3_event_handler, LV_EVENT_ALL, ui);
}
