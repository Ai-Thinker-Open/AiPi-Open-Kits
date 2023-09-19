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
#include "timers.h"
#include "log.h"
#include "wifi_event.h"
#include "cJSON.h"
#include "https_client.h"
#include "rd_01_recv.h"
#include "hid_keyboard.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "usbd_core.h"
#include "usbd_hid.h"
#include "bflb_gpio.h"
  /*********************
   *      DEFINES
   *********************/
#define DBG_TAG "custom"
#define AP_NUMBLE_MAX 20

#define PC_DESKTOP_PIN "1423"
   /**********************
    *      TYPEDEFS
    **********************/
void* custom_status_task;
int system_new_start = false;
weather_t weathers[4] = { 0 };
static xTimerHandle weathers_time;
/**********************
 *  STATIC PROTOTYPES
 **********************/
static wifi_mgmr_scan_params_t config;
static wifi_mgmr_scan_item_t wifi_aps[AP_NUMBLE_MAX];
static struct bflb_device_s* gpio;
static uint16_t http_timer_cont = 0;
/**********************
 *  STATIC VARIABLES
 **********************/
static void custom_state_task(void* arg);
static int cjson_get_weather(char* weather_data);
static char* compare_wea_output_img_100x100(const char* weather_data);
static char* compare_wea_output_img_20x20(const char* weather_data);
static void http_weather_timer_cb(xTimerHandle xtime);
/**
 * Create a demo application
 */

void custom_init(lv_ui* ui)
{
    xTaskCreate(custom_state_task, "state task", 1024, ui, 3, (xTaskHandle*)&custom_status_task);
    weathers_time = xTimerCreate("waather", pdMS_TO_TICKS(1000), pdTRUE, 0, http_weather_timer_cb);
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
    ui->rd_01_nodet_time = rd_01_nodet_time;
    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_OUTPUT | GPIO_PULLDOWN | GPIO_SMT_EN | GPIO_DRV_0);
    //启动时自动连接WiFi
    system_start_auto_connenct(true);

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
                //发送图片点击事件，停止图片转动并隐藏
                lv_event_send(ui->Home_img_loding, LV_EVENT_CLICKED, NULL);
                lv_img_set_src(ui->Home_img_wifi, &_wifi_alpha_16x16);
                lv_label_set_text_fmt(ui->Home_label_IP, "IP:%s", ip_addr_str);

                LOG_I("wifi start connecting ......");
                //设置下来密码选择框的内容
                lv_textarea_set_placeholder_text(ui->Home_ta_passInput, ui->pass);
                //在下拉选择框添加SSID
                lv_dropdown_add_option(ui->Home_ddlist_ssid, ui->ssid, lv_dropdown_get_option_cnt(ui->Home_ddlist_ssid)+1);
                //找到列表中的SSID 并选中显示
                if (lv_dropdown_get_option_index(ui->Home_ddlist_ssid, ui->ssid)>=0) {
                    lv_dropdown_set_selected(ui->Home_ddlist_ssid, lv_dropdown_get_option_index(ui->Home_ddlist_ssid, ui->ssid));//选中连接的SSID 选项
                }
                else {
                    lv_dropdown_set_selected(ui->Home_ddlist_ssid, 0);
                }
                //如果是第一次启动，那就进行首次启动的天气获取
                if (system_new_start) {

                } goto _get_weather;
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
            _get_weather:
                {
                    char* _buff = https_get_data(https_get_request(HTTP_HOST, HTTP_PATH));
                    cjson_get_weather(_buff);
                    //更新显示
                    //今天天气，温度等
                    lv_label_set_text_fmt(ui->Home_label_addr, "%.*s市", strlen(weathers[0].city), weathers[0].city);
                    lv_label_set_text_fmt(ui->Home_label_temp, "%.*s°", 2, weathers[0].tem_day);
                    lv_label_set_text_fmt(ui->Home_label_weather, "%.*s", strlen(weathers[0].wea), weathers[0].wea);
                    lv_img_set_src(ui->Home_img_1, compare_wea_output_img_100x100(weathers[0].wea));
                    //明天
                    lv_label_set_text_fmt(ui->Home_label_day1_temp, "%.*s°", 2, weathers[1].tem_day);
                    lv_img_set_src(ui->Home_img_day1, compare_wea_output_img_20x20(weathers[1].wea));
                    //后天
                    lv_label_set_text_fmt(ui->Home_label_day2_temp, "%.*s°", 2, weathers[2].tem_day);
                    lv_img_set_src(ui->Home_img_day2, compare_wea_output_img_20x20(weathers[2].wea));
                    //大后天
                    lv_label_set_text_fmt(ui->Home_label_day3_temp, "%.*s°", 2, weathers[3].tem_day);
                    lv_img_set_src(ui->Home_img_day3, compare_wea_output_img_20x20(weathers[3].wea));
                    vPortFree(_buff);
                    system_new_start = false;
                    Home_digital_clock_1_hour_value = clock_1_hour_value;
                    Home_digital_clock_1_min_value = clock_1_min_value;
                    Home_digital_clock_1_sec_value = clock_1_sec_value;
                    //重新启动定时器
                    http_timer_cont = 0;
                    xTimerStart(weathers_time, pdMS_TO_TICKS(10));
                }
            }
            break;
            case CUSTOM_STATE_USB_KEY:
            {

            }
            break;
            case CUSTOM_STATE_RADAR_DET:
            {
                LOG_I("Ra-01 detected ");
                // usb_hid_keyboard_inputpassword(flash_get_data("PIN", 64));
                //关闭2.4寸屏的显示
                bflb_gpio_reset(gpio, GPIO_PIN_14);
                usb_hid_keyboard_setWakeup();
            }
            break;
            case CUSTOM_STATE_RADAR_NDET:
            {
                // usb_hid_keyboard_lock();
                //开启显示
                bflb_gpio_set(gpio, GPIO_PIN_14);
                usb_hid_keyboard_setSleep();

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
 * @param weather_data
*/
static int cjson_get_weather(char* weather_data)
{
    for (size_t i = 0; i < strlen(weather_data); i++)
    {
        printf("%c", weather_data[i]);
    }
    printf("\r\n");

    cJSON* root = cJSON_Parse(weather_data);
    if (root==NULL) {
        LOG_E("[%s] is not json\r\n", __func__);
        return -1;
    }

    cJSON* errcode = cJSON_GetObjectItem(root, "errcode");
    if (errcode!=NULL) {

        cJSON* errmsg = cJSON_GetObjectItem(root, "errmsg");
        LOG_E("errcode=%d:%s", errcode->valueint, errmsg->valuestring);
        cJSON_Delete(root);
        return  -1;;
    }

    cJSON* city_cjsno = cJSON_GetObjectItem(root, "city");//城市名称
    if (city_cjsno ==NULL) {

        LOG_E("[city_cjsno ] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }
    memset(weathers[0].city, 0, 16);
    strncpy(weathers[0].city, city_cjsno->valuestring, strlen(city_cjsno->valuestring));

    cJSON* wea_data = cJSON_GetObjectItem(root, "data");
    if (wea_data ==NULL) {

        LOG_E("[wea_data] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }
    cJSON* wea_today = cJSON_GetArrayItem(wea_data, 0);//今天天气
    if (wea_today==NULL) {

        LOG_E("[wea_today] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }
    cJSON* wea_tomorrow = cJSON_GetArrayItem(wea_data, 1);
    if (wea_tomorrow==NULL) {
        LOG_E("[wea_tomorrow] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }
    cJSON* wea_acquired = cJSON_GetArrayItem(wea_data, 2);
    if (wea_acquired==NULL) {
        LOG_E("[wea_acquired] is not json\r\n");

        cJSON_Delete(root);
        return-1;;
    }
    cJSON* wea_today3 = cJSON_GetArrayItem(wea_data, 3);
    if (wea_today3==NULL) {
        LOG_E("[wea_today3] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }

    cJSON* today_tem = cJSON_GetObjectItem(wea_today, "tem_day");
    memset(weathers[0].tem_day, 0, 2);
    strncpy(weathers[0].tem_day, today_tem->valuestring, strlen(today_tem->valuestring));
    //解析今天天气
    cJSON* today_wea = cJSON_GetObjectItem(wea_today, "wea");
    memset(weathers[0].wea, 0, 32);
    strncpy(weathers[0].wea, today_wea->valuestring, strlen(today_wea->valuestring));
    //解析明天的天气
    cJSON* tomorrow_wea = cJSON_GetObjectItem(wea_tomorrow, "wea");
    cJSON* tomorrow_tem = cJSON_GetObjectItem(wea_tomorrow, "tem_day");

    //解析后天的天气
    cJSON* acquired_wea = cJSON_GetObjectItem(wea_acquired, "wea");
    cJSON* acquired_tem = cJSON_GetObjectItem(wea_acquired, "tem_day");
    //解析大后天天气
    cJSON* today3_wea = cJSON_GetObjectItem(wea_today3, "wea");

    cJSON* today3_tem = cJSON_GetObjectItem(wea_today3, "tem_day");

    memset(weathers[1].wea, 0, 64);
    memset(weathers[1].tem_day, 0, 2);
    strncpy(weathers[1].wea, tomorrow_wea->valuestring, strlen(tomorrow_wea->valuestring));

    strncpy(weathers[1].tem_day, tomorrow_tem->valuestring, strlen(tomorrow_tem->valuestring));
    memset(weathers[2].wea, 0, 64);
    memset(weathers[2].tem_day, 0, 2);
    strncpy(weathers[2].wea, acquired_wea->valuestring, strlen(acquired_wea->valuestring));
    strncpy(weathers[2].tem_day, acquired_tem->valuestring, strlen(acquired_tem->valuestring));

    strncpy(weathers[3].wea, today3_wea->valuestring, strlen(today3_wea->valuestring));
    strncpy(weathers[3].tem_day, today3_tem->valuestring, strlen(today3_tem->valuestring));
    cJSON_Delete(root);
    return 0;

}


/**
 * @brief 输出图片
 *
 * @param weather_data
 * @return char*
*/
static char* compare_wea_output_img_100x100(const char* weather_data)
{
    char* weather = weather_data;
    lv_img_dsc_t* tianqi = NULL;
    if (strncmp(weather, "晴", 2)==0) tianqi = &_tianqiqing_alpha_100x100;
    if (strncmp(weather, "阵雨", 4)==0) tianqi = &_tianqiduoyunxiaoyuzhuanqing_alpha_100x100;
    if (strncmp(weather, "小雨", 4)==0) tianqi = &_tianqixiaoyu_alpha_100x100;
    if (strncmp(weather, "中雨", 4)==0)tianqi = &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "大雨", 4)==0)tianqi = &_tianqidayu_alpha_100x100;
    if (strncmp(weather, "爆雨", 4)==0) tianqi = &_tianqidabaoyu_alpha_100x100;
    if (strncmp(weather, "雷雨", 4)==0) tianqi = &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "多云", 4)==0)tianqi = &_tianqiduoyun_alpha_100x100;
    if (strncmp(weather, "中雨转雷阵雨", 12)==0) tianqi = &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "雷阵雨", 6)==0) tianqi = &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "阴转多云", 8)==0) tianqi = &_tianqiduoyun_alpha_100x100;
    if (strncmp(weather, "阴转晴", 6)==0) tianqi = &_tianqiqing_alpha_100x100;

    if (strncmp(weather, "多云转雷阵雨", 12)==0) tianqi = &_tianqiduoyun_alpha_100x100;
    if (strncmp(weather, "雷阵雨转中到大雨", 16)==0) tianqi = &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "雷阵雨转中雨", 12)==0) tianqi = &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "中到大雨转大暴雨", 16)==0) tianqi = &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "阵雨转大雨", 16)==0) tianqi = &_tianqiduoyunxiaoyuzhuanqing_alpha_100x100;
    if (strncmp(weather, "小雨转阵雨", 10)==0) tianqi = &_tianqixiaoyu_alpha_100x100;
    return tianqi;
}
/**
 * @brief
 *
 * @param weather_data
 * @return char*
*/
static char* compare_wea_output_img_20x20(const char* weather_data)
{
    char* weather = weather_data;

    if (strncmp(weather, "晴", 2)==0) return &_tianqiqing_i_alpha_20x20;
    if (strncmp(weather, "阵雨", 4)==0) return &_tianqiqing_i_xiaoyuzhuanqing_alpha_20x20;
    if (strncmp(weather, "小雨", 4)==0) return &_tianqiqing_i_xiaoyu_alpha_20x20;
    if (strncmp(weather, "中雨", 4)==0) return &_tianqiqing_i_zhongyu_alpha_20x20;
    if (strncmp(weather, "大雨", 4)==0) return &_tianqiqing_i_dayu_alpha_20x20;
    if (strncmp(weather, "爆雨", 4)==0) return &_tianqiqing_i_baoyu_alpha_20x20;
    if (strncmp(weather, "雷雨", 4)==0) return &_tianqiqing_i_leiyu_alpha_20x20;
    if (strncmp(weather, "多云", 4)==0) return &_tianqiqing_i_duoyun_alpha_20x20;
    if (strncmp(weather, "阴转多云", 8)==0) return &_tianqiqing_i_duoyun_alpha_20x20;
    if (strncmp(weather, "多云转阴", 8)==0) return &_tianqiqing_i_duoyun_alpha_20x20;

    if (strncmp(weather, "中雨转雷阵雨", 12)==0) return &_tianqiqing_i_zhongyu_alpha_20x20;
    if (strncmp(weather, "雷阵雨", 6)==0) return &_tianqiqing_i_leiyu_alpha_20x20;

    if (strncmp(weather, "多云转雷阵雨", 12)==0) return &_tianqiqing_i_duoyun_alpha_20x20;
    if (strncmp(weather, "雷阵雨转中到大雨", 16)==0) return &_tianqiqing_i_leiyu_alpha_20x20;
    if (strncmp(weather, "雷阵雨转中雨", 12)==0) return &_tianqiqing_i_leiyu_alpha_20x20;
    if (strncmp(weather, "中到大雨转大暴雨", 16)==0) return &_tianqiqing_i_zhongyu_alpha_20x20;
    if (strncmp(weather, "阵雨转大雨", 16)==0) return &_tianqiqing_i_xiaoyuzhuanqing_alpha_20x20;
    return &_tianqiqing_i_alpha_20x20;
}
/**
 * @brief 定时HTTP 定时器
 *
 * @param xtime
*/
static void http_weather_timer_cb(xTimerHandle xtime)
{
    //1s 自加一次
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    http_timer_cont++;
    if (http_timer_cont>=1*60*60) {
        xTimerStop(weathers_time, pdMS_TO_TICKS(10));
        xTaskNotifyFromISR(custom_status_task, CUSTOM_STATE_WEATHRE_GET, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }
}