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
#include "gui_guider.h"
#include "ble_hid_dev.h"

#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
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
weather_t weathers[4] = { 0 };
/**
 * @brief cjson__analysis_type
 *
 * @param json_data
 * @return int
*/
static custom_event_t cjson__analysis_type(char* json_data);
static char* cjson_analysis_ssid(char* json_data);
static char* cjson_analysis_password(char* json_data);
static char* cjson__analysis_ip(char* cjson_data);
static void cjson_get_weather(char* weather_data);
char* compare_wea_output_img_100x100(const char* weather_data);
char* compare_wea_output_img_20x20(const char* weather_data);
static ble_status_t cjson_analysis_ble_status(char* ble_status_data);
static int cjson_analysis_wifi_scan(char* json_data, char* ssid_arry[64]);
/**
 * @brief queue_receive_task
 *
 * @param arg
*/
static wifi_mgmr_scan_params_t wifi_scan_config[32];
static void queue_receive_task(void* arg)
{
    ble_status_t ble_status = BLE_STATUS_DATA_ERR;
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
            //扫描
            case CUSTOM_EVENT_WIFI_SCAN:
            {
                cjson_analysis_wifi_scan(queue_buff, NULL);
            }
            break;
            //WiFi
            case CUSTOM_EVENT_GET_WIFI:
            {
                ssid = cjson_analysis_ssid(queue_buff);
                password = cjson_analysis_password(queue_buff);
                LOG_I("ssid=%s password:%s", ssid, password);
                wifi_connect(ssid, password);
            }
            break;
            //接收ip地址
            case CUSTOM_EVENT_GOT_IP:
            {
                ipv4_addr = cjson__analysis_ip(queue_buff);
                LOG_I(" ipv4 addr=%s", ipv4_addr);
                memset(queue_buff, 0, 1024*2);
                sprintf(queue_buff, "IP:%s", ipv4_addr);
                ui->wifi_stayus = true;
                strcpy(ui->ssid, ssid);
                strcpy(ui->password, password);
                //识别当前界面

                if (ui->wifi_stayus)
                    lv_img_set_src(ui->src_home_img_wifi, &_wifi_alpha_20x20);
                else
                    lv_img_set_src(ui->src_home_img_wifi, &_no_internet_alpha_20x20);
                // lv_label_set_text(ui->WiFi_config_label_10, queue_buff);
                lv_event_send(ui->src_home_img_loding, LV_EVENT_CLICKED, NULL);
                lv_textarea_set_text(ui->src_home_ta_1, ui->password);
                lv_label_set_text(ui->src_home_label_ip, queue_buff);
                lv_dropdown_set_selected(ui->src_home_ddlist_1, 1);

                vPortFree(ssid);
                vPortFree(password);
                vPortFree(ipv4_addr);

                if (https_Handle!=NULL) {
                    vTaskDelete(https_Handle);
                }
                xTaskCreate(https_get_weather_task, "https task", 1024*2, NULL, 4, &https_Handle);

            }
            break;
            case CUSTOM_EVENT_GET_WEATHER:
            {
                cjson_get_weather(queue_buff);
                //今天天气
                lv_img_set_src(ui->src_home_img_1, compare_wea_output_img_100x100(weathers[0].wea)); //天气图片
                lv_label_set_text(ui->src_home_label_dizhi, weathers[0].city);//地址
                lv_label_set_text(ui->src_home_label_waether, weathers[0].wea);
                lv_label_set_text_fmt(ui->src_home_label_temp, "%*.2s℃", 2, weathers[0].tem_day);
                //明天天气
                lv_img_set_src(ui->src_home_img_day1, compare_wea_output_img_20x20(weathers[1].wea));
                lv_label_set_text_fmt(ui->src_home_day1_temp, "%*.2s°", 2, weathers[1].tem_day);
                //后天天气
                lv_img_set_src(ui->src_home_img_day2, compare_wea_output_img_20x20(weathers[2].wea));
                lv_label_set_text_fmt(ui->src_home_day2_temp, "%*.2s°", 2, weathers[2].tem_day);
                //大后天天气
                lv_img_set_src(ui->src_home_img_day3, compare_wea_output_img_20x20(weathers[3].wea));
                lv_label_set_text_fmt(ui->src_home_day3_temp, "%*.2s°", 2, weathers[3].tem_day);
            }
            break;
            //BLE 状态
            case CUSTOM_EVENT_GET_BLE:
            {
                ble_status = cjson_analysis_ble_status(queue_buff);
                switch (ble_status) {
                    case BLE_STATUS_ENABLE:
                    {
                        LOG_I("BLE status is Enable,wating connect");
                        lv_label_set_text(ui->src_home_label_BLEConter, "BLE:opened");
                    }
                    break;
                    case BLE_STATUS_CONNECT:
                    {
                        LOG_I("BLE Connect OK");
                        lv_label_set_text(ui->src_home_label_BLEConter, "BLE:Connected");
                        lv_img_set_src(ui->src_home_img_BLE, &_BLE_ok_alpha_20x20);
                        lv_obj_add_flag(ui->src_home_cont_BLE_TEXT, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_clear_flag(ui->src_home_cont_dis, LV_OBJ_FLAG_HIDDEN);
                    }
                    break;
                    case BLE_STATUS_DISCONNECT:
                    {
                        LOG_F("BLE disconnect!");
                        lv_label_set_text(ui->src_home_label_BLEConter, "BLE:opnened");
                        lv_img_set_src(ui->src_home_img_BLE, &_BLE_no_alpha_20x20);
                        lv_obj_add_flag(ui->src_home_cont_dis, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_clear_flag(ui->src_home_cont_BLE_TEXT, LV_OBJ_FLAG_HIDDEN);
                    }
                    break;
                    default:
                        break;
                }
            }
            break;
            default:
                break;
        }


        vPortFree(queue_buff);
    }
}
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
    }

}
/**********************
 *  STATIC VARIABLES
 **********************/

 /**
  * Create a demo application
  */

void custom_init(lv_ui* ui)
{
    /* Add your codes here */
    queue = xQueueCreate(2, 1024*2);
    xTaskCreate(queue_receive_task, "queue_receive_task", 1024*2, ui, 3, NULL);
    http_timers = xTimerCreate("http_timers", pdMS_TO_TICKS(1000), pdTRUE, 0, http_hour_requst_time);

}

static custom_event_t cjson__analysis_type(char* json_data)
{

    cJSON* root = cJSON_Parse(json_data);

    if (root==NULL) {
        LOG_E("\"%s\"is not json", json_data);
        return CUSTOM_EVENT_NONE;
    }
    cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
    if (wifi) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GET_WIFI;
    }
    cJSON* ip = cJSON_GetObjectItem(root, "ip");
    if (ip) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GOT_IP;
    }

    cJSON* weather = cJSON_GetObjectItem(root, "weather");
    if (weather) {
        cJSON_Delete(root);
        return  CUSTOM_EVENT_GET_WEATHER;
    }

    cJSON* BLE_STA = cJSON_GetObjectItem(root, "BLE_HID");
    if (BLE_STA) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GET_BLE;
    }

    cJSON* WIFI_SCAN = cJSON_GetObjectItem(root, "wifi_scan");
    if (WIFI_SCAN) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_WIFI_SCAN;
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
 * @param weather_data
*/
static void cjson_get_weather(char* weather_data)
{
    for (size_t i = 0; i < strlen(weather_data); i++)
    {
        printf("%c", weather_data[i]);
    }
    printf("\r\n");

    cJSON* root = cJSON_Parse(weather_data);
    if (root==NULL) {
        printf("[%s] is not json\r\n", __func__);
        return NULL;
    }
    cJSON* weather_cjson = cJSON_GetObjectItem(root, "weather");
    cJSON* city_cjsno = cJSON_GetObjectItem(weather_cjson, "city");//城市名称
    if (city_cjsno ==NULL) {

        printf("[city_cjsno ] is not json\r\n");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON* wea_data = cJSON_GetObjectItem(weather_cjson, "data");
    if (wea_data ==NULL) {

        printf("[wea_data] is not json\r\n");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON* wea_today = cJSON_GetArrayItem(wea_data, 0);//今天天气
    if (wea_today==NULL) {

        printf("[wea_today] is not json\r\n");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON* wea_tomorrow = cJSON_GetArrayItem(wea_data, 1);
    if (wea_tomorrow==NULL) {
        printf("[wea_tomorrow] is not json\r\n");
        cJSON_Delete(root);
        return NULL;
    }
    cJSON* wea_acquired = cJSON_GetArrayItem(wea_data, 2);
    if (wea_acquired==NULL) {
        printf("[wea_acquired] is not json\r\n");

        cJSON_Delete(root);
        return NULL;
    }
    cJSON* wea_today3 = cJSON_GetArrayItem(wea_data, 3);
    if (wea_today3==NULL) {
        printf("[wea_today3] is not json\r\n");
        cJSON_Delete(root);
        return NULL;
    }
    //解析今天天气
    cJSON* today_wea = cJSON_GetObjectItem(wea_today, "wea");
    cJSON* today_tem = cJSON_GetObjectItem(wea_today, "tem_day");
    //解析明天的天气
    cJSON* tomorrow_wea = cJSON_GetObjectItem(wea_tomorrow, "wea");
    cJSON* tomorrow_tem = cJSON_GetObjectItem(wea_tomorrow, "tem_day");
    //解析后天的天气
    cJSON* acquired_wea = cJSON_GetObjectItem(wea_acquired, "wea");
    cJSON* acquired_tem = cJSON_GetObjectItem(wea_acquired, "tem_day");
    //解析大后天天气
    cJSON* today3_wea = cJSON_GetObjectItem(wea_today3, "wea");
    cJSON* today3_tem = cJSON_GetObjectItem(wea_today3, "tem_day");

    memcpy(weathers[0].city, city_cjsno->valuestring, strlen(city_cjsno->valuestring));
    memcpy(weathers[0].wea, today_wea->valuestring, strlen(today_wea->valuestring));
    memcpy(weathers[0].tem_day, today_tem->valuestring, strlen(today_tem->valuestring));

    memcpy(weathers[1].wea, tomorrow_wea->valuestring, strlen(tomorrow_wea->valuestring));
    memcpy(weathers[1].tem_day, tomorrow_tem->valuestring, strlen(tomorrow_tem->valuestring));

    memcpy(weathers[2].wea, acquired_wea->valuestring, strlen(acquired_wea->valuestring));
    memcpy(weathers[2].tem_day, acquired_tem->valuestring, strlen(acquired_tem->valuestring));

    memcpy(weathers[3].wea, today3_wea->valuestring, strlen(today3_wea->valuestring));
    memcpy(weathers[3].tem_day, today3_tem->valuestring, strlen(today3_tem->valuestring));

    cJSON_Delete(root);
}

/**
 * @brief 输出图片
 *
 * @param weather_data
 * @return char*
*/
char* compare_wea_output_img_100x100(const char* weather_data)
{
    char* weather = weather_data;

    if (strncmp(weather, "晴", 2)==0) return &_tianqiqing_alpha_100x100;
    if (strncmp(weather, "阵雨", 4)==0) return &_tianqiduoyunxiaoyuzhuanqing_alpha_100x100;
    if (strncmp(weather, "小雨", 4)==0) return &_tianqixiaoyu_alpha_100x100;
    if (strncmp(weather, "中雨", 4)==0) return &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "大雨", 4)==0) return &_tianqidayu_alpha_100x100;
    if (strncmp(weather, "爆雨", 4)==0) return &_tianqidabaoyu_alpha_100x100;
    if (strncmp(weather, "雷雨", 4)==0) return &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "多云", 4)==0) return &_tianqiduoyun_alpha_100x100;
    if (strncmp(weather, "中雨转雷阵雨", 12)==0) return &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "雷阵雨", 6)==0) return &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "阴转多云", 8)==0) return &_tianqiduoyun_alpha_100x100;
}
/**
 * @brief
 *
 * @param weather_data
 * @return char*
*/
char* compare_wea_output_img_20x20(const char* weather_data)
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
}

/**
 * @brief cjson_analysis_ble_status
 *      获取BLE 的状态
 * @param ble_status_data
 * @return le_status_t
*/
static ble_status_t cjson_analysis_ble_status(char* ble_status_data)
{
    ble_status_t ble_status;
    cJSON* root = cJSON_Parse(ble_status_data);
    if (root==NULL) {
        LOG_E("%s is not json", ble_status_data);
        ble_status = BLE_STATUS_DATA_ERR;
        goto __exit;
    }

    cJSON* BLE_root = cJSON_GetObjectItem(root, "BLE_HID");
    if (BLE_root ==NULL) {
        LOG_E("BLE HID is NULL");
        ble_status = BLE_STATUS_DATA_ERR;
        cJSON_Delete(root);
        goto __exit;
    }
    cJSON* hid_status = cJSON_GetObjectItem(BLE_root, "status");
    ble_status = hid_status->valueint;
    cJSON_Delete(root);
__exit:
    return  ble_status;
}
/**
 * @brief cjson_analysis_wifi_scan
 *
 * @param json_data
 * @param ssid_arry SSID list
 * @return int
*/
static int cjson_analysis_wifi_scan(char* json_data, char* ssid_arry[64])
{
    if (json_data==NULL) {
        return -1;
    }

    cJSON* root = cJSON_Parse(json_data);
    if (root==NULL) {
        LOG_E("<%s> is't JSON");
        return -1;
    }
    cJSON* root_scan = cJSON_GetObjectItem(root, "wifi_scan");
    cJSON* scan_status = cJSON_GetObjectItem(root_scan, "status");

    switch (scan_status->valueint)
    {
        case 1:
        {
            LOG_I("scan_status is start");
            cJSON_Delete(root);
            return scan_status->valueint;
        }
        /* code */
        break;

        default:
            break;
    }

    return 0;
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