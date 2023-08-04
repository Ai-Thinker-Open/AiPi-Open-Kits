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
#include "bflb_uart.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "voice_uart.h"

  /*********************
   *      DEFINES
   *********************/
#define DBG_TAG "CUSTOM"
   /**********************
    *      TYPEDEFS
    **********************/
xQueueHandle queue;

extern xQueueHandle ble_queue;
extern QueueHandle_t ble_hid_queue;
extern uart_rx_cmd_t uart_cmd;
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
static int cjson_get_weather(char* weather_data);
char* compare_wea_output_img_100x100(const char* weather_data);
char* compare_wea_output_img_20x20(const char* weather_data);
static ble_status_t cjson_analysis_ble_status(char* ble_status_data);
static int cjson_analysis_wifi_scan(char* json_data);
static int cjson_analysis_uart_cmd(char* json_data);
/**
 * @brief queue_receive_task
 *
 * @param arg
*/

static wifi_mgmr_scan_item_t wifi_aps[32];
static void queue_receive_task(void* arg)
{
    ble_status_t ble_status = BLE_STATUS_DATA_ERR;
    static wifi_mgmr_scan_params_t wifi_scan_config;
    char* queue_buff = NULL;
    char* ssid_list = NULL;
    char* ssid = NULL;
    char* password = NULL;
    char* ipv4_addr = NULL;
    lv_ui* ui = (lv_ui*)arg;
    static struct bflb_device_s* uartx;
    uartx = bflb_device_get_by_name("uart1");

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

        if (xQueueReceive(queue, queue_buff, 100/portTICK_PERIOD_MS)==pdTRUE)
        {
            switch (cjson__analysis_type(queue_buff))
            {
                //扫描
                case CUSTOM_EVENT_WIFI_SCAN:
                {
                    ssid_list = pvPortMalloc(256);
                    memset(ssid_list, 0, 256);
                    switch (cjson_analysis_wifi_scan(queue_buff))
                    {
                        case 0:
                        {
                            int ssids = wifi_mgmr_sta_scanlist_nums_get();
                            wifi_mgmr_sta_scanlist_dump(wifi_aps, ssids);
                            // if(wifi_mgmr_sta_scanlist_nums_get())
                            for (size_t i = 0; i < (ssids>=10?10:ssids); i++)
                            {
                                //进行字符串拼接
                                strcat(ssid_list, wifi_aps[i].ssid);
                                if ((ssids>=10?10:ssids)-i != 1) {
                                    strcat(ssid_list, "\n");
                                }
                            }
                            lv_dropdown_set_options(ui->src_home_ddlist_1, ssid_list);
                            vTaskDelay(100/portTICK_RATE_MS);
                            lv_event_send(ui->src_home_img_loding, LV_EVENT_CLICKED, NULL);
                            vPortFree(ssid_list);

                            bflb_uart_put(uartx, user_data[UART_CMD_WIFI_SCAN_DONE].uart_data.data, 4);
                        }
                        break;
                        case 1:
                        {
                            bflb_uart_put(uartx, user_data[UART_CMD_WIFI_SCAN_START].uart_data.data, 4);
                            vTaskDelay(1000/portTICK_RATE_MS);
                            wifi_mgmr_sta_scan(&wifi_scan_config);

                        }
                        break;
                        default:
                            break;
                    }
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
                    bflb_uart_put(uartx, user_data[UART_CMD_WIFI_CONNECT_OK].uart_data.data, 4);

                    if (ui->wifi_stayus)
                        lv_img_set_src(ui->src_home_img_wifi, &_wifi_alpha_20x20);
                    else
                        lv_img_set_src(ui->src_home_img_wifi, &_no_internet_alpha_20x20);
                    // lv_label_set_text(ui->WiFi_config_label_10, queue_buff);
                    vTaskDelay(100/portTICK_RATE_MS);
                    lv_event_send(ui->src_home_img_loding, LV_EVENT_CLICKED, NULL);
                    lv_textarea_set_text(ui->src_home_ta_1, ui->password);
                    lv_label_set_text(ui->src_home_label_ip, queue_buff);

                    lv_dropdown_add_option(ui->src_home_ddlist_1, ui->ssid, lv_dropdown_get_option_cnt(ui->src_home_ddlist_1)+1);

                    if (lv_dropdown_get_option_index(ui->src_home_ddlist_1, ui->ssid)>=0) {
                        lv_dropdown_set_selected(ui->src_home_ddlist_1, lv_dropdown_get_option_index(ui->src_home_ddlist_1, ui->ssid));
                    }
                    else {
                        lv_dropdown_set_selected(ui->src_home_ddlist_1, 0);
                    }

                    vPortFree(ssid);
                    vPortFree(password);
                    vPortFree(ipv4_addr);

                    if (https_Handle!=NULL) {
                        vTaskDelete(https_Handle);
                    }
                    vTaskDelay(1500/portTICK_RATE_MS);

                    xTaskCreate(https_get_weather_task, "https task", 1024*2, NULL, 4, &https_Handle);

                }
                break;
                case CUSTOM_EVENT_GET_WEATHER:
                {
                    if (!cjson_get_weather(queue_buff)) {
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

                        bflb_uart_put(uartx, user_data[UART_CMD_CHECK_WEATHER].uart_data.data, 4);
                    }

                }
                break;
                break;

                default:
                    break;
            }
        }

        //避免队列读取的阻塞影响LVGL 事件的发送
        switch (uart_cmd)
        {
            case UART_RX_CMD_OPENL:
            {
                LOG_I("Voice cmd: [打开灯光]");
                if (ui->mqtt_connect_status)
                    lv_event_send(ui->src_home_imgbtn_closeL, LV_EVENT_CLICKED, NULL);
                else {
                    bflb_uart_put(uartx, user_data[UART_CMD_CONRTOL_ERROR].uart_data.data, 4);
                }
            }
            break;
            case UART_RX_CMD_CLOSEL:
            {
                LOG_I("Voice cmd: [关闭灯光]");
                if (ui->mqtt_connect_status)
                    lv_event_send(ui->src_home_imgbtn_openL, LV_EVENT_CLICKED, NULL);
                else {
                    bflb_uart_put(uartx, user_data[UART_CMD_CONRTOL_ERROR].uart_data.data, 4);
                }
            }
            break;
            case UART_RX_CMD_CHECK_WEATHER:
                LOG_I("Voice cmd: [天气查询]");
                vTaskDelay(2000/portTICK_RATE_MS);
                vTaskResume(https_Handle);
                break;
            case UART_RX_CMD_CONNECT_MQTT:
            {
                LOG_I("Voice cmd: [连接服务器]");
                vTaskDelay(2000/portTICK_RATE_MS);
                lv_tabview_set_act(ui->src_home_tabview_1, 3, LV_ANIM_ON);
                vTaskDelay(500/portTICK_RATE_MS);
                lv_event_send(ui->src_home_btn_connect_mqtt, LV_EVENT_CLICKED, NULL);
            }
            break;
            case UART_RX_CMD_CONNECT_WIFI:
                LOG_I("Voice cmd: [连接WiFi]");
                lv_event_send(ui->src_home_btn_connect, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_OPEN_APP_1:
                LOG_I("Voice cmd: [打开VScode]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_1, LV_EVENT_CLICKED, NULL);
                else {
                    bflb_uart_put(uartx, user_data[UART_CMD_BLE_CONNECT_NG].uart_data.data, 4);
                }
                break;
            case UART_RX_CMD_OPEN_APP_2:
                LOG_I("Voice cmd: [打开百度网盘]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_2, LV_EVENT_CLICKED, NULL);
                else
                    bflb_uart_put(uartx, user_data[UART_CMD_BLE_CONNECT_NG].uart_data.data, 4);

                break;
            case UART_RX_CMD_OPEN_APP_3:
                LOG_I("Voice cmd: [打开谷歌浏览器]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_3, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_OPEN_APP_4:
                LOG_I("Voice cmd: [打开串口助手]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_4, LV_EVENT_CLICKED, NULL);
                else
                    bflb_uart_put(uartx, user_data[UART_CMD_BLE_CONNECT_NG].uart_data.data, 4);
                break;
            case UART_RX_CMD_OPEN_APP_5:
                LOG_I("Voice cmd: [打开OBS]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_6, LV_EVENT_CLICKED, NULL);
                else
                    bflb_uart_put(uartx, user_data[UART_CMD_BLE_CONNECT_NG].uart_data.data, 4);
                break;
            case UART_RX_CMD_OPEN_APP_6:
                LOG_I("Voice cmd: [打开立创EDA]");
                if (ui->ble_status)
                    lv_event_send(ui->src_home_imgbtn_5, LV_EVENT_CLICKED, NULL);
                else
                    bflb_uart_put(uartx, user_data[UART_CMD_BLE_CONNECT_NG].uart_data.data, 4);
                break;
            case UART_RX_CMD_WIFI_SCAN:
                LOG_I("Voice cmd: [扫描WiFi]");
                lv_event_send(ui->src_home_btn_scan, LV_EVENT_CLICKED, NULL);

                break;
            case UART_RX_CMD_VOL_MUTX_OK:
                LOG_I("Voice cmd: [电脑静音]");
                lv_event_send(ui->src_home_imgbtn_novoice, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_VOL_MUTX_NG:
                LOG_I("Voice cmd: [电脑开声音]");
                lv_event_send(ui->src_home_imgbtn_voice, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_VOL_UP:
                LOG_I("Voice cmd: [音量加]");
                lv_event_send(ui->src_home_imgbtn_vol_I, LV_EVENT_LONG_PRESSED, NULL);
                vTaskDelay(1000/portTICK_RATE_MS);
                lv_event_send(ui->src_home_imgbtn_vol_I, LV_EVENT_CLICKED, NULL);

                break;
            case UART_RX_CMD_VOL_DOWN:
                LOG_I("Voice cmd: [音量减]");
                lv_event_send(ui->src_home_imgbtn_vol_n, LV_EVENT_LONG_PRESSED, NULL);
                vTaskDelay(1000/portTICK_RATE_MS);
                lv_event_send(ui->src_home_imgbtn_vol_n, LV_EVENT_CLICKED, NULL);

                break;
            case UART_RX_CMD_MUSIC_NEXT:
                LOG_I("Voice cmd: [播放下一曲]");
                lv_event_send(ui->src_home_imgbtn_7, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_MUSIC_PAST:
                LOG_I("Voice cmd: [播放上一曲]");
                lv_event_send(ui->src_home_imgbtn_8, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_MUSIC_PALY:
                LOG_I("Voice cmd: 播放]");
                lv_event_send(ui->src_home_imgbtn_satrt, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_MUSIC_PALY_PLUS:
            {
                LOG_I("Voice cmd: [播放音乐]");
                hid_key_num_t hid_key_num = HID_KEY_NUMBLE_G;
                xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
                vTaskDelay(6000/portTICK_RATE_MS);
                lv_event_send(ui->src_home_imgbtn_satrt, LV_EVENT_CLICKED, NULL);
            }
            break;
            case UART_RX_CMD_MUSIC_OPEN:
            {
                hid_key_num_t hid_key_num = HID_KEY_NUMBLE_G;
                xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
                LOG_I("Voice cmd: [打开音乐播放器]");
            }
            break;
            case UART_RX_CMD_MUSIC_STOP:
                LOG_I("Voice cmd: [暂停播放]");
                lv_event_send(ui->src_home_imgbtn_stop, LV_EVENT_CLICKED, NULL);
                break;
            case UART_RX_CMD_GAME_OPEN:
            {
                hid_key_num_t hid_key_num = HID_KEY_NUMBLE_H;
                xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
                LOG_I("Voice cmd: [原神启动]");
            }
            break;
            default:
                break;
        }
        uart_cmd = UART_RX_CMD_NONE;

        vPortFree(queue_buff);
    }
}

void queue_receive_ble_task(void* arg)
{
    char* queue_buff = NULL;
    ble_status_t ble_status = BLE_STATUS_DATA_ERR;
    lv_ui* ui = (lv_ui*)arg;
    while (1)
    {
        queue_buff = pvPortMalloc(1024*2);
        memset(queue_buff, 0, 1024*2);
        xQueueReceive(ble_queue, queue_buff, portMAX_DELAY);
        switch (cjson__analysis_type(queue_buff)) {
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
                        ui->ble_status = true;
                    }
                    break;
                    case BLE_STATUS_DISCONNECT:
                    {
                        LOG_F("BLE disconnect!");
                        lv_label_set_text(ui->src_home_label_BLEConter, "BLE:opnened");
                        lv_img_set_src(ui->src_home_img_BLE, &_BLE_no_alpha_20x20);
                        lv_obj_add_flag(ui->src_home_cont_dis, LV_OBJ_FLAG_HIDDEN);
                        lv_obj_clear_flag(ui->src_home_cont_BLE_TEXT, LV_OBJ_FLAG_HIDDEN);
                        ui->ble_status = false;
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
    queue = xQueueCreate(1, 1024*2);
    xTaskCreate(queue_receive_task, "queue_receive_task", 1024*3, ui, 3, NULL);
    http_timers = xTimerCreate("http_timers", pdMS_TO_TICKS(1000), pdTRUE, 0, http_hour_requst_time);
    mqtt_client_init();
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

    cJSON* VOICE = cJSON_GetObjectItem(root, "voice_cmd");
    if (VOICE) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_UART_CMD;
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
    cJSON* weather_cjson = cJSON_GetObjectItem(root, "weather");
    cJSON* errcode = cJSON_GetObjectItem(weather_cjson, "errcode");
    if (errcode!=NULL) {

        cJSON* errmsg = cJSON_GetObjectItem(weather_cjson, "errmsg");
        LOG_E("errcode=%d:%s", errcode->valueint, errmsg->valuestring);
        cJSON_Delete(root);
        return  -1;;
    }

    cJSON* city_cjsno = cJSON_GetObjectItem(weather_cjson, "city");//城市名称
    if (city_cjsno ==NULL) {

        LOG_E("[city_cjsno ] is not json\r\n");
        cJSON_Delete(root);
        return -1;;
    }
    cJSON* wea_data = cJSON_GetObjectItem(weather_cjson, "data");
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
    return 0;

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
    if (strncmp(weather, "多云转雷阵雨", 12)==0) return &_tianqiduoyun_alpha_100x100;
    if (strncmp(weather, "雷阵雨转中到大雨", 16)==0) return &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "雷阵雨转中雨", 12)==0) return &_tianqiyeleiyu_alpha_100x100;
    if (strncmp(weather, "中到大雨转大暴雨", 16)==0) return &_tianqizhongyu_alpha_100x100;
    if (strncmp(weather, "阵雨转大雨", 16)==0) return &_tianqiduoyunxiaoyuzhuanqing_alpha_100x100;
    return &_tianqiqing_alpha_100x100;
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
static int cjson_analysis_wifi_scan(char* json_data)
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
        case 0:
        {
            LOG_I("scan_status is done");
            cJSON_Delete(root);
            return scan_status->valueint;
        }
        break;
        default:
            break;
    }

    return 0;
}
/**
 * @brief  cjson_analysis_uart_cmd
 *
 * @param json_data
 * @return int
*/
static int cjson_analysis_uart_cmd(char* json_data)
{
    cJSON* root = cJSON_Parse(json_data);
    if (root==NULL) {
        LOG_E("%s is NOT json");
        return -1;
    }

    cJSON* uart_cmd = cJSON_GetObjectItem(root, "voice_cmd");
    if (uart_cmd==NULL) {
        LOG_E("%s is NULL");
        cJSON_Delete(root);
        return -1;
    }
    int i = uart_cmd->valueint;
    cJSON_Delete(root);
    return i;
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