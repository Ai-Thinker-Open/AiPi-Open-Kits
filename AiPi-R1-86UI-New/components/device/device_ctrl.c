/**
 * @file device_ctrl.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device_ctrl.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "gui_guider.h"
#include "user_esflash.h"
#include "https_client.h"
#include "cJSON.h"
#include <lwip/tcpip.h>
#include "wifi_event.h"
#include "bl_fw_api.h"
#include <lwip/sockets.h>
#include <lwip/inet.h>
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "lwip/dns.h"
#include "log.h"
#include "https_client.h"

#define DBG_TAG "device ctrl"
#define TIMER_STEP 3 //定时3 小时更新一次时间

void* device_task;
char wea[16] = { 0 };
char city[32] = { 0 };
char tem_day[16] = { 0 };
char tem_tem_night[16] = { 0 };
char win[16] = { 0 };
char air[16] = { 0 };
char humidity[16] = { 0 };
char pressure[16] = { 0 };

static xTimerHandle http_timer;

static void http_timer_cb(TimerHandle_t xTimer)
{
    uint32_t ulCount;
    BaseType_t xHigherPriorityTaskWoken;
    ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    ulCount++;
    if (ulCount>=TIMER_STEP) {
        ulCount = 0;

        vTimerSetTimerID(xTimer, (void*)ulCount);
        vTaskResume(https_Handle);
        xTimerStop(xTimer, 0);
    }
    else {
        vTimerSetTimerID(xTimer, (void*)ulCount);

    }

}


void device_state_task(void* arg)
{
    lv_ui* ui = (lv_ui*)arg;
    static  device_state_t device_state;
    static char* ssid;
    static char* password;
    static ip_addr_t dns_addr;
    esay_flash_init();
    dns_init();
    http_timer = xTimerCreate("http timer", pdMS_TO_TICKS(60*60*1000), pdTRUE, 0, http_timer_cb);

    xTaskCreate(https_get_weather_task, "http", 1024*3, &guider_ui, 4, (TaskHandle_t*)&https_Handle);
    while (1)
    {
        xTaskNotifyWait(0xffffffff, 0x0, &device_state, portMAX_DELAY);
        switch (device_state)
        {
            case DEVICE_STATE_SYSTEM_START:
            {
                LOG_I("DEVICE_STATE_SYSTEM_START");
                //读取Flash WiFi 节点
                if (flash_check_key(FLASH_SSID_KEY)) {
                    ssid = flash_get_data(FLASH_SSID_KEY, 64);
                    password = flash_get_data(FLASH_PASS_KEY, 32);
                    LOG_I("wifi connect ......");
                    wifi_connect(ssid, password);
                }
                else {
                    ssid = pvPortMalloc(64);
                    password = pvPortMalloc(32);
                    sprintf(ssid, "FAE@Seahi");
                    sprintf(password, "fae12345678");
                    wifi_connect(ssid, password);


                }
            }
            break;
            case DEVICE_STATE_WIFI_CONNECT:
            {
                LOG_I(" DEVICE_STATE_WIFI_CONNECT");
                ssid = pvPortMalloc(64);
                password = pvPortMalloc(32);
                strcpy(ssid, lv_textarea_get_text(ui->screen_ta_SSID));
                strcpy(password, lv_textarea_get_text(ui->screen_ta_PASS));
                wifi_connect(ssid, password);

            }
            break;
            case DEVICE_STATE_WIFI_CONNECT_OK:
            {
                LOG_I("DEVICE_STATE_WIFI_CONNECT_OK");
            }
            break;
            case  DEVICE_STATE_WIFI_GO_IP:
            {
                uint32_t ipv4_addr = 0;
                wifi_sta_ip4_addr_get(&ipv4_addr, NULL, NULL, NULL);
                LOG_I("DEVICE_STATE_WIFI_GO_IP:%s", inet_ntoa(ipv4_addr));
                // wifi_mgmr_sta_connect_params();

                lv_textarea_set_text(ui->screen_ta_SSID, ssid);
                lv_textarea_set_text(ui->screen_ta_PASS, password);

                flash_erase_set(FLASH_PASS_KEY, password);
                flash_erase_set(FLASH_SSID_KEY, ssid);
                vPortFree(ssid);
                vPortFree(password);
                //设置DNS 服务器
                inet_aton("223.5.5.5", &dns_addr.addr);
                dns_setserver(0, &dns_addr);
                vTaskResume(https_Handle);
            }
            break;
            case DEVICE_STATE_WIFI_DISCONNECT:
            {
                LOG_I("DEVICE_STATE_WIFI_DISCONNECT");
                ssid = flash_get_data(FLASH_SSID_KEY, 64);
                password = flash_get_data(FLASH_PASS_KEY, 32);
                LOG_I("wifi connect ......");
                wifi_connect(ssid, password);
            }
            break;
            case DEVICE_STATE_SYSTEM_TIME_UPDATE:
            {
                LOG_I("DEVICE_STATE_WIFI_DISCONNECT");
            }
            break;
            case DEVICE_STATE_HTTP_GET_WEATHER:
            {
                //获取天气的功能在HTTP 源码中
                LOG_I("DEVICE_STATE_WIFI_DISCONNECT");
                lv_label_set_text_fmt(ui->screen_label_7, "%s市：#ffa2c2 %s# ", city, wea);
                lv_label_set_text_fmt(ui->screen_label_temp_day, "白天温度：%s ℃", tem_day);
                lv_label_set_text_fmt(ui->screen_label_temp_night, "夜间温度：%s ℃", tem_tem_night);
                lv_label_set_text_fmt(ui->screen_label_win, "风向：%s", win);
                lv_label_set_text_fmt(ui->screen_label_pre, "气压：%s\n", pressure);
                lv_label_set_text_fmt(ui->screen_label_air, "空气质量：%s", air);
                lv_label_set_text_fmt(ui->screen_label_humi, "湿度：%s%", humidity);
                xTimerStart(http_timer, 0);
            }
            break;
            default:
                break;
        }
    }

}

void device_send_state_notify(device_state_t device_state, int is_irq)
{
    if (device_task==NULL) {
        LOG_E("device_task is no init");
        return;
    }
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    if (is_irq)xTaskNotifyFromISR((TaskHandle_t)device_task, device_state, eSetValueWithoutOverwrite, &xHigherPriorityTaskWoken);
    else xTaskNotify((TaskHandle_t)device_task, device_state, eSetValueWithoutOverwrite);
}