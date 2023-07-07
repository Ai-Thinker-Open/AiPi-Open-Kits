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
#include "gui_guider.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "easyflash.h"
#include "queue.h"
#include "cJSON.h"
#include "sntp.h"
#include "bflb_timestamp.h"
#include "easyflash.h"
#include "https_client.h"
  /*********************
   *      DEFINES
   *********************/
#define UTC_CHINA 8
uint8_t wifi_connect(char* ssid, char* passwd);
TaskHandle_t https_Handle;
weather_t weathers[4] = { 0 };

void sntp_set_time(uint32_t sntp_time, uint32_t fac);
/**********************
 *      TYPEDEFS
 **********************/
xQueueHandle queue;

/**********************
 *  STATIC PROTOTYPES
 **********************/

 /**
  * @brief 解析json 类型
  *
  * @param json_data
  * @return int
 */
static int cjson__analysis_type(char* json_data)
{

    cJSON* root = cJSON_Parse(json_data);

    if (root==NULL) {
        printf("[%s] is not json\r\n", __func__);

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
 * @brief cjson_analysis_ssid
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
        printf("[%s] is not json\r\n");
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
 * @brief cjson_analysis_password
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
        printf("[%s] is not json\r\n");
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
 * @brief cjson__analysis_ip
 *      解析 IP地址
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
        printf("[%s] is not json\r\n", __func__);
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
    ;
    if (strncmp(weather, "大雨", 4)==0) return &_tianqiqing_i_dayu_alpha_20x20;
    if (strncmp(weather, "爆雨", 4)==0) return &_tianqiqing_i_baoyu_alpha_20x20;
    if (strncmp(weather, "雷雨", 4)==0) return &_tianqiqing_i_leiyu_alpha_20x20;
    if (strncmp(weather, "多云", 4)==0) return &_tianqiqing_i_duoyun_alpha_20x20;
}
/**
 * @brief  void queue_task(void* arg)
 * 消息队列循环读取
 * @param arg
*/
static void queue_task(void* arg)
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
        printf("read flash ssid:%s password:%s\r\n", ssid, password);
        wifi_connect(ssid, password);
    }
    else {
        printf("ssid read value is NULL:%06X\r\n", SSID_KEY);
    }

    while (1)
    {

        queue_buff = pvPortMalloc(1024*2);
        memset(queue_buff, 0, 1024*2);

        xQueueReceive(queue, queue_buff, portMAX_DELAY);

        switch (cjson__analysis_type(queue_buff))
        {
            case 1:
                ssid = cjson_analysis_ssid(queue_buff);
                password = cjson_analysis_password(queue_buff);
                printf("[%s]:ssid=%s password:%s\r\n", __func__, ssid, password);
                wifi_connect(ssid, password);
                break;
                //接收ip地址
            case 2:
                ipv4_addr = cjson__analysis_ip(queue_buff);
                printf("[%s] ipv4 addr=%s\r\n", __func__, ipv4_addr);
                memset(queue_buff, 0, 1024*2);
                sprintf(queue_buff, "IP:%s", ipv4_addr);
                ui->wifi_stayus = true;

                strcpy(ui->ssid, ssid);
                strcpy(ui->password, password);
                //识别当前界面
                if (ui->screen_type) {
                    lv_img_set_src(ui->cont_4_img_connet, &_wifi_alpha_16x16);
                }
                else {
                    lv_label_set_text(ui->WiFi_config_label_10, queue_buff);
                    lv_event_send(ui->WiFi_config_imgbtn_1, LV_EVENT_CLICKED, NULL);//手动发送事件，返回首页
                }
                vPortFree(ssid);
                vPortFree(password);
                vPortFree(ipv4_addr);
                if (https_Handle!=NULL) {
                    vTaskDelete(https_Handle);
                }
                xTaskCreate(https_get_weather_task, "https task", 1024*6, NULL, 3, &https_Handle);
                break;
                //接收天气情况
            case 3:

                cjson_get_weather(queue_buff);

                if (ui->screen_type) {
                    lv_label_set_text_fmt(ui->cont_4_label_3, "%s市", weathers[0].city);
                    lv_label_set_text(ui->cont_4_label_4, weathers[0].wea);
                    lv_label_set_text_fmt(ui->cont_4_label_temp, "%s°", weathers[0].tem_day);
                    lv_label_set_text_fmt(ui->cont_4_label_7, "%.*s°", 2, weathers[1].tem_day);
                    lv_label_set_text_fmt(ui->cont_4_label_8, "%.*s°", 2, weathers[2].tem_day);
                    lv_label_set_text_fmt(ui->cont_4_label_9, "%.*s°", 2, weathers[3].tem_day);
                    lv_img_set_src(ui->cont_4_img_clear, compare_wea_output_img_100x100(weathers[0].wea));
                    lv_img_set_src(ui->cont_4_img_3, compare_wea_output_img_20x20(weathers[1].wea));
                    lv_img_set_src(ui->cont_4_img_2, compare_wea_output_img_20x20(weathers[2].wea));
                    lv_img_set_src(ui->cont_4_img_1, compare_wea_output_img_20x20(weathers[3].wea));

                }
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
  * Create a demo application
  */

void custom_init(lv_ui* ui)
{
    /* Add your codes here */
    queue = xQueueCreate(1, 1024*2);
    xTaskCreate(queue_task, "queue task", 1024*6, ui, 2, NULL);

}
/**
 * @brief 设置时间
 *
 * @param sntp_time
 * @param fac
*/
void sntp_set_time(uint32_t sntp_time, uint32_t fac)
{
    uint32_t stamp;

    stamp = sntp_time - 2208988800;

    bflb_timestamp_t time_s;

    bflb_timestamp_utc2time(stamp, &time_s);

    cont_4_digital_clock_1_hour_value = time_s.hour;
    cont_4_digital_clock_1_min_value = time_s.min;
    cont_4_digital_clock_1_sec_value = time_s.sec;
    cont_4_lable_1_yers = time_s.year;
    cont_4_lable_1_month = time_s.mon;
    cont_4_lable_1_day = time_s.mday;
    cont_4_lable_1_wday = time_s.wday;
    if (https_Handle!=NULL)
        vTaskResume(https_Handle);
    printf(" %d/%d/%d.week%d-%02d:%02d:%02d\r\n", time_s.year, time_s.mon, time_s.mday, time_s.wday, cont_4_digital_clock_1_hour_value, cont_4_digital_clock_1_min_value, cont_4_digital_clock_1_sec_value);

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