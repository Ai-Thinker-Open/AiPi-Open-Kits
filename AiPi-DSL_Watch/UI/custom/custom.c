// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
#include "lvgl.h"
#include "log.h"
#include "custom.h"
#include "cJSON.h"
#include "easyflash.h"
#include "wifi_event.h"
#include "https_client.h"
  /*********************
   *      DEFINES
   *********************/
#define DBG_TAG "CUSTOM"
   /**********************
    *      TYPEDEFS
    **********************/
xQueueHandle queue;
/**********************
 *  STATIC PROTOTYPES
 **********************/
TaskHandle_t https_Handle;
TimerHandle_t http_timers;
/**
 * @brief cjson__analysis_type
 *
 * @param json_data
 * @return int
*/
static int cjson__analysis_type(char* json_data);
static char* cjson_analysis_ssid(char* json_data);
static char* cjson_analysis_password(char* json_data);
static char* cjson__analysis_ip(char* cjson_data);
/**
 * @brief queue_receive_task
 *
 * @param arg
*/
static void queue_receive_task(void* arg)
{
    char* queue_buff = NULL;
    char* ssid = NULL;
    char* password = NULL;
    char* ipv4_addr = NULL;
    lv_ui* ui = (lv_ui*)arg;

    ssid = flash_get_data(SSID_KEY, 32);
    password = flash_get_data(PASS_KEY, 32);
    if (ssid!=NULL)
    {
        LOG_I("read flash ssid:%s password:%s", ssid, password);
        wifi_connect(ssid, password);
    }
    else {
        LOG_E("ssid read value is NULL:%06X", SSID_KEY);
    }

    while (1) {
        queue_buff = pvPortMalloc(1024*2);
        memset(queue_buff, 0, 1024*2);

        xQueueReceive(queue, queue_buff, portMAX_DELAY);
        switch (cjson__analysis_type(queue_buff))
        {
            case 1:
                ssid = cjson_analysis_ssid(queue_buff);
                password = cjson_analysis_password(queue_buff);
                LOG_I("ssid=%s password:%s", ssid, password);
                wifi_connect(ssid, password);
                break;
                //接收ip地址
            case 2:
                ipv4_addr = cjson__analysis_ip(queue_buff);
                LOG_I(" ipv4 addr=%s", ipv4_addr);
                memset(queue_buff, 0, 1024*2);
                sprintf(queue_buff, "IP:%s", ipv4_addr);
                // ui->wifi_stayus = true;
                strcpy(ui->ssid, ssid);
                strcpy(ui->password, password);
                //识别当前界面
                if (ui->screen_type) {
                    if (ui->wifi_stayus)
                        lv_img_set_src(ui->Home_img_wifi, &_wifi_alpha_20x20);
                    else
                        lv_img_set_src(ui->Home_img_wifi, &_no_internet_alpha_20x20);
                }
                else {
                    // lv_label_set_text(ui->WiFi_config_label_10, queue_buff);
                    lv_event_send(ui->config_img_loding, LV_EVENT_CLICKED, NULL);
                    lv_event_send(ui->config_img_h, LV_EVENT_CLICKED, NULL);//手动发送事件，返回首页
                }
                vPortFree(ssid);
                vPortFree(password);
                vPortFree(ipv4_addr);

                if (https_Handle!=NULL) {
                    vTaskDelete(https_Handle);
                }
                xTaskCreate(https_get_weather_task, "https task", 1024*6, NULL, 4, &https_Handle);
                break;
            case 3:
                // cjson_get_weather(queue_buff);
                break;
            default:
                break;
        }
        vPortFree(queue_buff);
    }
}
/**********************
 *  STATIC VARIABLES
 **********************/
 /**
  * @brief http_hour_requst_time
  *        定时1小时 更新时间及天气情况
  * @param arg
 */
static uint16_t timers_http = 0;
static void http_hour_requst_time(TimerHandle_t timer)
{
    if (timers_http>=60*60) {
        LOG_I("Timed to http update,start https request");
        vTaskResume(https_Handle);
        timers_http = 0;
    }
    else {
        timers_http++;
        if (timers_http%10==0)
            LOG_I("timer conect =%d", timers_http);
    }

}
/**
 * Create a demo application
 */

void custom_init(lv_ui* ui)
{
    /* Add your codes here */
    queue = xQueueCreate(1, 1024*2);
    xTaskCreate(queue_receive_task, "queue_receive_task", 1024*4, ui, 3, NULL);
    http_timers = xTimerCreate("http_timers", pdMS_TO_TICKS(1000), pdTRUE, 0, http_hour_requst_time);

}

static int cjson__analysis_type(char* json_data)
{

    cJSON* root = cJSON_Parse(json_data);

    if (root==NULL) {
        LOG_E("\"%s\"is not json", json_data);
        return 0;
    }
    cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
    if (wifi) {
        cJSON_Delete(root);
        return 1;
    }
    cJSON* ip = cJSON_GetObjectItem(root, "ip");
    if (ip) {
        cJSON_Delete(root);
        return 2;
    }

    cJSON* weather = cJSON_GetObjectItem(root, "weather");
    if (weather) {
        cJSON_Delete(root);
        return 3;
    }

    cJSON_Delete(root);
    return 0;
}
/**
 * @brief 获取WiFi名称
 *
 * @param json_data
 * @return char*
*/
static char* cjson_analysis_ssid(char* json_data)
{
    static char* ssid_str;
    ssid_str = pvPortMalloc(32);
    memset(ssid_str, 0, 32);
    cJSON* root = cJSON_Parse(json_data);
    if (root==NULL) {
        LOG_I("[%s] is not json", json_data);
        return NULL;
    }
    cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
    if (wifi==NULL)return NULL;
    cJSON* ssid = cJSON_GetObjectItem(wifi, "ssid");
    memcpy(ssid_str, ssid->valuestring, sizeof(ssid->valuestring)*8);
    cJSON_Delete(root);
    return ssid_str;
}
/**
 * @brief 获取WiFi 密码
 *
 * @param json_data
 * @return char*
*/
static char* cjson_analysis_password(char* json_data)
{
    static char* pass_str;
    pass_str = pvPortMalloc(32);
    memset(pass_str, 0, 32);
    cJSON* root = cJSON_Parse(json_data);
    if (root==NULL) {
        LOG_I("[%s] is not json", json_data);
        return NULL;
    }
    cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
    if (wifi==NULL)return NULL;
    cJSON* password = cJSON_GetObjectItem(wifi, "password");
    memcpy(pass_str, password->valuestring, sizeof(password->valuestring)*8);
    cJSON_Delete(root);
    return pass_str;
}
/**
 * @brief 获取IP地址
 *
 * @param cjson_data
 * @return char*
*/
static char* cjson__analysis_ip(char* cjson_data)
{
    static char* IP_str;

    IP_str = pvPortMalloc(64);
    memset(IP_str, 0, 64);

    cJSON* root = cJSON_Parse(cjson_data);
    if (root==NULL) {
        LOG_I("[%s] is not json", cjson_data);
        return NULL;
    }
    cJSON* ip_s = cJSON_GetObjectItem(root, "ip");
    cJSON* ip = cJSON_GetObjectItem(ip_s, "IP");
    memcpy(IP_str, ip->valuestring, sizeof(ip->valuestring)*16);
    cJSON_Delete(root);
    return IP_str;
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