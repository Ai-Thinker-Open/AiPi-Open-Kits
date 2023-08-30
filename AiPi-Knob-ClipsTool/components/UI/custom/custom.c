// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"
#include "ble_hid_dev.h"

#define DBG_TAG "LV_CUSTOM"
  /*********************
   *      DEFINES
   *********************/

   /**********************
    *      TYPEDEFS
    **********************/

    /**********************
     *  STATIC PROTOTYPES
     **********************/
void* state_task;

/**********************
 *  STATIC VARIABLES
 **********************/
static void ui_state_task(void* arg);

void ui_add_obj_to_encoder_group(lv_ui* ui, lv_obj_t* obj)
{
    lv_group_add_obj(ui->group, obj);
}

void ui_remove_all_objs_from_encoder_group(lv_ui* ui)
{
    lv_group_remove_all_objs(ui->group);
}

void ui_obj_to_encoder_init(lv_ui* ui)
{
    ui->group = lv_group_create();
    lv_group_set_default(ui->group);
    lv_indev_t* indev = lv_indev_get_next(NULL);
    if (LV_INDEV_TYPE_ENCODER == lv_indev_get_type(indev)) {
        // ESP_LOGI(TAG, "add group for encoder");
        LV_LOG_INFO("add group for encoder");
        lv_indev_set_group(indev, ui->group);
        lv_group_focus_freeze(ui->group, false);
    }

}

lv_group_t* get_encoder_group(lv_ui* ui)
{
    return ui->group;
}
/**
 * Create a demo application
 */

void custom_init(lv_ui* ui)
{
    ui_obj_to_encoder_init(ui);

    ui_add_obj_to_encoder_group(ui, ui->screen_tileview_1);

    xTaskCreate(ui_state_task, "ui state", 1024, (void*)ui, 1, (xTaskHandle)&state_task);
}

static void ui_state_task(void* arg)
{
    uint32_t ui_state_event = 0;

    ble_status_t hid_event = 0;
    lv_ui* ui = (lv_ui*)arg;
    ui->screen_type = 0;
    while (1)
    {
        xTaskNotifyWait(0xffffffff, 0x00, &hid_event, portMAX_DELAY);
        LOG_F("get notify=%d", hid_event);
        switch (hid_event)
        {
            case  BLE_STATUS_ENABLE:
            {
                LOG_I("BLE HID event:BLE_STATUS_ENABLE");
            }
            break;
            case  BLE_STATUS_CONNECT:
            {
                LOG_I("BLE HID event:BLE_STATUS_CONNECT");
                ui->BLE_status = true;
                if (ui->screen_type==0) {

                    lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_label_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_label_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_tileview_1, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            case  BLE_STATUS_DISCONNECT:
            {
                LOG_I("BLE HID event:BLE_STATUS_DISCONNECT");
                ui->BLE_status = false;
                if (ui->screen_type==0) {
                    lv_obj_clear_flag(ui->screen_img_1, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_label_3, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui->screen_label_4, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui->screen_tileview_1, LV_OBJ_FLAG_HIDDEN);
                }
            }
            break;
            default:
                break;
        }
    }
}

