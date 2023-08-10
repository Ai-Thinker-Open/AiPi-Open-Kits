/**
 * @file sys_status.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
*/
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include <stdio.h>
#include "cJSON.h"
#include "wifi_event.h"

#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "log.h"
#include "user_mqtt.h"
#include "sys_status.h"
#include "m62_rgb.h"
/*********************
 *      DEFINES
 *********************/
#define DBG_TAG "SYS_STATUS"


xQueueHandle queue;
xTimerHandle pub_time;
static custom_event_t queue_get_custom_event(char* json_data);
static char* cjson__analysis_ip(char* cjson_data);
static int cjson_ctrl_rgb(char* json_data, uint8_t* red, uint8_t* green, uint8_t* blue);
bool switch_status = true;
static void queue_receive_task(void* arg)
{
    char* queue_buff = NULL;
    char* ssid = NULL;
    char* password = NULL;

    while (1) {
        queue_buff = pvPortMalloc(1024);
        memset(queue_buff, 0, 1024);
        xQueueReceive(queue, queue_buff, portMAX_DELAY);//读取

        switch (queue_get_custom_event(queue_buff))
        {
            case CUSTOM_EVENT_GOT_IP://获取到IP地址
            {
                char* ipv4_addr = cjson__analysis_ip(queue_buff);
                LOG_I("GET IP:%s", ipv4_addr);
                vPortFree(ipv4_addr);
                mqtt_start_connect(MQTT_SERVER, MQTT_PORT, MQTT_USER_NAME, MQTT_PASSWOLD);
            }
            break;
            case CUSTOM_EVENT_GET_WIFI_DISCONNECT: //WiFi 断开
            {

            }
            break;
            case CUSTOM_EVENT_MQTT_CONNECT: //MQTT 连接服务器成功
            {
                LOG_F("CUSTOM_EVENT_MQTT_CONNECT queue data:%s", queue_buff);
                mqtt_app_subscribe(MQTT_CLIETN_PUB_TOPIC, 0);
                xTimerStart(pub_time, pdMS_TO_TICKS(10));
            }
            break;
            case CUSTOM_EVENT_MQTT_DISCONNECT: //MQTT断开
            {
                LOG_F("CUSTOM_EVENT_MQTT_DISCONNECT queue data:%s", queue_buff);
                mqtt_client_init();
                mqtt_start_connect(MQTT_SERVER, MQTT_PORT, MQTT_USER_NAME, MQTT_PASSWOLD);
                xTimerStop(pub_time, pdMS_TO_TICKS(10));
            }
            break;
            case CUSTOM_EVENT_MQTT_GET_MSG: //MQTT 接收到数据
            {
                LOG_I("MQTT recv:");
                LOG_F("%s", queue_buff);
                int ret = cjson_ctrl_rgb(queue_buff, &color_red, &color_green, &color_blue);

                if (ret==1) {
                    dev_rgb_ch_color(color_red, color_green, color_blue);
                    switch_status = true;
                }
                else if (ret==0) {
                    dev_rgb_ch_color(0, 0, 0);
                    switch_status = false;
                }
            }
            break;
        }

        vPortFree(queue_buff);
    }
}

static void pub_timer_cb(TimerHandle_t xtime)
{
    char* led_data = pvPortMalloc(256);
    memset(led_data, 0, 256);
    sprintf(led_data, "{\"board_id\":1,\"status\":%d,\"RGB\":{\"R\":%d,\"G\":%d,\"B\":%d}}", switch_status, color_red, color_green, color_blue);
    mqtt_app_publish(MQTT_CLIETN_TOPIC, led_data, 0);
    vPortFree(led_data);
}

void system_status_init(void)
{
    queue = xQueueCreate(4, 1024);
    xTaskCreate(queue_receive_task, "queue_receive_task", 1024, NULL, 3, NULL);
    mqtt_client_init();
    pub_time = xTimerCreate("pub_time", pdMS_TO_TICKS(1000), pdTRUE, 0, pub_timer_cb);
}

/**
 * @brief queue_get_custom_event
 *      读取消息队列中的事件
 * @param json_data
 * @return custom_event_t
*/
static custom_event_t queue_get_custom_event(char* json_data)
{

    cJSON* root = cJSON_Parse(json_data);

    if (root==NULL) {
        LOG_E("\"%.*s\"is not json", strlen(json_data), json_data);
        return CUSTOM_EVENT_NONE;
    }
    cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
    if (wifi) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GET_WIFI;
    }
    cJSON* wifi_dis = cJSON_GetObjectItem(root, "wifi_disconnect");
    if (wifi) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GET_WIFI_DISCONNECT;
    }

    cJSON* ip = cJSON_GetObjectItem(root, "ip");
    if (ip) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_GOT_IP;
    }


    cJSON* WIFI_SCAN = cJSON_GetObjectItem(root, "wifi_scan");
    if (WIFI_SCAN) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_WIFI_SCAN_DONE;
    }

    cJSON* mqtt_connect = cJSON_GetObjectItem(root, "mqtt_connect");
    if (mqtt_connect) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_MQTT_CONNECT;
    }

    cJSON* mqtt_disconnect = cJSON_GetObjectItem(root, "mqtt_disconnect");
    if (mqtt_disconnect) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_MQTT_DISCONNECT;
    }
    cJSON* mqtt_msg = cJSON_GetObjectItem(root, "mqtt_msg");

    if (mqtt_msg) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_MQTT_GET_MSG;
    }
    cJSON* mqtt_pub = cJSON_GetObjectItem(root, "RGB");
    if (mqtt_pub) {
        cJSON_Delete(root);
        return CUSTOM_EVENT_MQTT_PUB_MSG;
    }


    cJSON_Delete(root);

    return 0;
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
 * @brief cjson_ctrl_rgb
 *
 * @param json_data
 * @param red
 * @param green
 * @param blue
 * @return int
*/
static int cjson_ctrl_rgb(char* json_data, uint8_t* red, uint8_t* green, uint8_t* blue)
{
    int status_value = 0;
    cJSON* root = cJSON_Parse(json_data);
    if (root==NULL) {
        LOG_E("%s is no json data");
        return -1;
    }
    cJSON* mqtt_msg = cJSON_GetObjectItem(root, "mqtt_msg");
    cJSON* data = cJSON_GetObjectItem(mqtt_msg, "data");

    cJSON* board_id = cJSON_GetObjectItem(data, "board_id");
    if (board_id==NULL) {

        cJSON_Delete(root);
        return -1;
    }
    if (board_id->valueint==0||board_id->valueint==2)
    {
        LOG_E("board_id is not Ai-M1 or all");
        cJSON_Delete(root);
        return -1;
    }
    cJSON* status = cJSON_GetObjectItem(data, "status");
    status_value = status->valueint;
    if (status->valueint==0) {
        cJSON_Delete(root);
        return status_value;
    }

    cJSON* RGB = cJSON_GetObjectItem(data, "RGB");

    if (RGB->valueint==0 &&RGB->child==NULL) {
        cJSON_Delete(root);
        return status_value;
    }
    cJSON* RGB_R = cJSON_GetObjectItem(RGB, "R");
    cJSON* RGB_G = cJSON_GetObjectItem(RGB, "G");
    cJSON* RGB_B = cJSON_GetObjectItem(RGB, "B");

    *red = RGB_R->valueint;
    *green = RGB_G->valueint;
    *blue = RGB_B->valueint;

    cJSON_Delete(root);
    return status_value;
}
