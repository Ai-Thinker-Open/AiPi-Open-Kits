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
#include "wifi_event.h"
#include "cJSON.h"
#include "https_client.h"
#include "rd_01_recv.h"
#include "hid_keyboard.h"

#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
  /*********************
   *      DEFINES
   *********************/
#define BDG_TAG "custom"
#define AP_NUMBLE_MAX 20
   /**********************
    *      TYPEDEFS
    **********************/
void* custom_status_task;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static wifi_mgmr_scan_params_t config;
static wifi_mgmr_scan_item_t wifi_aps[AP_NUMBLE_MAX];
/**********************
 *  STATIC VARIABLES
 **********************/
static void custom_state_task(void* arg);

/**
 * Create a demo application
 */

void custom_init(lv_ui* ui)
{
    xTaskCreate(custom_state_task, "state task", 1024, ui, 3, (xTaskHandle*)&custom_status_task);
}
/**
 * @brief custom_state_task
 *
 * @param arg
*/
static void custom_state_task(void* arg)
{
    static custom_state_t state;
    lv_ui* ui = (lv_ui*)arg;
    while (1)
    {
        xTaskNotifyWait(0xffffffff, 0, &state, portMAX_DELAY);
        switch (state) {
            case CUSTOM_STATE_WIFI_START_CONNECT:
            {
                LOG_I("wifi start connecting ......");
                char* buff = pvPortMalloc(64);
                memset(buff, 0, 64);
                //读取下拉框的选择
                lv_dropdown_get_selected_str(ui->Home_ddlist_ssid, buff, 64);
                wifi_connect(buff, lv_textarea_get_text(ui->Home_ta_passInput));
                vPortFree(buff);
            }
            break;
            case CUSTOM_STATE_WIFI_IP:
            {
                LOG_I("wifi go ip");
            }
            break;
            case CUSTOM_STATE_WIFI_OK:
            {
                LOG_I("wifi connect OK");
            }
            break;
            case CUSTOM_STATE_WIFI_SCAN:
            {
                LOG_I("wifi start scan AP ....");
                wifi_mgmr_sta_scan(&config);
            }
            break;
            case CUSTOM_STATE_WIFI_SCAN_DONE:
            {
                char* ssid_list = pvPortMalloc(256);
                memset(ssid_list, 0, 256);
                uint8_t AP_numble = wifi_mgmr_sta_scanlist_nums_get();
                LOG_I("wifi start scan done,AP num=%d", AP_numble);
                //发送图片点击事件，停止图片转动并隐藏
                lv_event_send(ui->Home_img_loding, LV_EVENT_CLICKED, NULL);
                //判断AP个数是否大于AP_NUMBLE_MAX，大于 AP_NUMBLE_MAX 就等AP_NUMBLE_MAX
                AP_numble = AP_numble>AP_NUMBLE_MAX?AP_NUMBLE_MAX:AP_numble;
                //读取AP列表
                wifi_mgmr_sta_scanlist_dump(wifi_aps, AP_numble);
                //进行字符串凭借
                for (size_t i = 0; i < AP_numble; i++)
                {
                    //进行字符串拼接
                    strcat(ssid_list, wifi_aps[i].ssid);
                    if ((AP_numble)-i != 1) {
                        strcat(ssid_list, "\n");
                    }
                }
                //设置下拉框内容
                lv_dropdown_set_options(ui->Home_ddlist_ssid, ssid_list);
                vPortFree(ssid_list);
            }
            break;
            case CUSTOM_STATE_WEATHRE_GET:
            {

            }
            break;
            case CUSTOM_STATE_USB_SLEEP:
            {

            }
            break;
            case CUSTOM_STATE_USB_WAKEUP:
            {

            }
            break;
            case CUSTOM_STATE_USB_KEY:
            {

            }
            break;
            case CUSTOM_STATE_RADAR_DET:
            {

            }
            break;
            case CUSTOM_STATE_RADAR_NDET:
            {

            }
            break;
            default:
                break;
        }
    }
}

/**
 * @brief
 *
 * @param key
 * @param value
*/
void flash_erase_set(char* key, char* value)
{
    size_t len = 0;
    int value_len = strlen(value);
    ef_set_and_save_env(key, value);
    // bflb_flash_read(key, flash_data, strlen(value));
    // printf("writer data:%s\r\n", flash_data);
    memset(value, 0, strlen(value));
    ef_get_env_blob(key, value, value_len, &len);
}
/**
 * @brief
 *
 * @param key
 * @return char*
*/
char* flash_get_data(char* key, uint32_t len)
{
    static char* flash_data;
    flash_data = pvPortMalloc(len);
    memset(flash_data, 0, len);

    ef_get_env_blob(key, flash_data, len, (size_t)&len);

    return flash_data;
}