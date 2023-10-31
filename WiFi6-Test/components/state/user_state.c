/**
 * @file user_state.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cJSON.h"
#include "log.h"
#include "wifi6_led.h"
#include "user_state.h"
#include "wifi_event.h"
#include "user_esflash.h"
#include "user_mqtt.h"
#include <lwip/def.h>
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#define DBG_TAG "STATE"

static xTaskHandle stateTaskHandle;
static user_state_t state_s;
static int led_state;
static char payload[128];
mq_msg_t mq_msg;

static void state_task(void* arg)
{
    while (1) {
        xTaskNotifyWait(0xffffffff, 0, &state_s, portMAX_DELAY);
        switch (state_s) {
            //系统启动
            case STATE_SYSTEM_START:
            {
                //从flash读取wifi连接数据
                if (flash_get_key(SSID_KEY)>0) {
                    char* flash_ssid = flash_get_data(SSID_KEY, 32);
                    char* flash_pass = flash_get_data(PASS_KEY, 64);
                    char* flash_channel = flash_get_data(CHANNEL_KEY, 3);
                    wifi_connect(flash_ssid, flash_pass, atoi(flash_channel));
                    vPortFree(flash_ssid);
                    vPortFree(flash_pass);
                    vPortFree(flash_channel);
                }
                else {
                    wifi_connect(SSID, PASS, 1);
                }
                //读取LED状态
                if (flash_get_key(LED_KEY)>0) {
                    char* flash_led = flash_get_data(LED_KEY, 1);
                    led_state = atoi(flash_led);
                    if (led_state) {
                        led_dev_open(&led_state);
                    }
                    else {
                        led_dev_cloes(&led_state);
                    }
                    vPortFree(flash_led);
                }
            }
            break;
            //正在连接WiFi
            case STATE_WIFI_CONNECTING:
            {
                led_indicator_change(state_s);
            }
            break;
            case STATE_WIFI_CONNECTED_OK://WiFi 连接成功
            {
                //闪灯
                led_indicator_change(state_s);
                //连接MQTT服务器
                mqtt_client_init(MQ_DEF_HOST, MQ_DEF_PORT);
                mqtt_start_connect(MQ_DEF_HOST, MQ_DEF_PORT, MQ_USERNAME, "123456");
                //保存WiFi
                wifi_mgmr_connect_ind_stat_info_t wifi_mgmr_ind_stat;
                wifi_mgmr_sta_connect_ind_stat_get(&wifi_mgmr_ind_stat);

                flash_erase_set(SSID_KEY, wifi_mgmr_ind_stat.ssid);
                flash_erase_set(PASS_KEY, wifi_mgmr_ind_stat.passphr);
                char* ch = pvPortMalloc(3);
                lwip_itoa(ch, 3, wifi_mgmr_ind_stat.channel);
                flash_erase_set(CHANNEL_KEY, ch);
                vPortFree(ch);
            }
            break;
            case STATE_MQTT_SERVER_CONENCT_OK://服务器连接成功
            {
                LOG_I("mqtt server connect OK");
                led_indicator_change(state_s);

                mqtt_app_subscribe(MQ_SUB_TOPIC, 0);//订阅
                mqtt_app_publish(MQ_PUB_TOPIC, led_payload_update(led_state), 0);//上电发布状态
            }
            break;
            case STATE_MQTT_MSG_ARRIVES: //服务器消息下发
            {
                LOG_I("%s:[%s]", mq_msg.topic, mq_msg.data);
                int _led_state = get_server_led(mq_msg.data);

                if (_led_state)led_dev_open(&led_state);
                else led_dev_cloes(&led_state);
                //控制完成后保存状态
                flash_erase_set(LED_KEY, led_state?"1":"0");
            }
            break;

            default:
                break;
        }
    }
}

void user_state_start(void)
{
    xTaskCreate(state_task, "state_task", 1024*2, NULL, 3, &stateTaskHandle);
}
/**
 * @brief user_state_send_notify
 *
 * @param state
*/
void user_state_send_notify(user_state_t state, int is_irq)
{
    if (stateTaskHandle==NULL) {
        LOG_E("\"state_task\" not create");
        return;
    }
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    if (is_irq)
        xTaskNotifyFromISR(stateTaskHandle, state, eSetValueWithoutOverwrite, &xHigherPriorityTaskWoken);
    else
        xTaskNotify(stateTaskHandle, state, eSetValueWithoutOverwrite);
}
/**
 * @brief Get the server led object
 * { "msgType":2,"LEDstate": state}
 * @param data
 * @return true
 * @return false
*/
int get_server_led(char* data)
{
    if (data==NULL) {
        LOG_E("data is no's json");
        return -1;
    }
    cJSON* root = cJSON_Parse(data);
    if (root==NULL) {
        LOG_E("root is null");
        return -1;
    }
    cJSON* msgType = cJSON_GetObjectItem(root, "msgType");
    if (msgType==NULL) {
        LOG_E("msgType Item is NULL");
        cJSON_Delete(root);
        return -1;
    }
    if (msgType==1) {
        LOG_E("msgType Item value is not \"2\"");
        cJSON_Delete(root);
        return -1;
    }
    cJSON* LED = cJSON_GetObjectItem(root, "LEDstate");
    if (LED==NULL) {
        LOG_E("LED Item is NULL");
        cJSON_Delete(root);
        return -1;
    }

    int _led_state = LED->valueint;
    cJSON_Delete(root);
    return _led_state;
}
/**
 * @brief led_payload_update
 *
 * @param led_state
 * @return char*
*/
char* led_payload_update(int led_state)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "msgType", 1);
    cJSON_AddBoolToObject(root, "LEDstate", led_state);
    strcpy(payload, cJSON_PrintUnformatted(root));
    LOG_F("led payload=%s", payload);
    cJSON_Delete(root);
    return payload;
}