/**
 * @file device.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#include "device.h"
#include "aipi_cloud.h"
#include "cJSON.h"
#define DBG_TAG "DEVICE"

lv_ui guider_ui;

static aipi_r286_dev_t device;
static xTaskHandle sys_state_ctl;

static aipi_r286_state_t sys_state;

static void aipi_r286_devstate_start(void* arg)
{
    aiio_rev_queue_t  rev_queue = { 0 };

    while (1)
    {
        xTaskNotifyWait(0, 0xffffffff, &sys_state, portMAX_DELAY);
        switch (sys_state)
        {
            case AIPI_R286_STATE_SYS_START:
            {
                //从Flash 读取设备信息
                uint8_t len = 32;
                char* flash_ssid = flash_get_data(FLASH_SSID_KEY, len);
                //读不到SSID，开启配网
                if (strlen(flash_ssid)==0) {
                    rev_queue.common_event = REV_CONFIG_START_EVENT;
                    if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                    {
                        aiio_log_e("queue send failed\r\n");
                    }
                    memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                    break;
                }
                len = 64;
                char* flash_pass = flash_get_data(FLASH_PWD_KEY, len);

                r2_device->mqttip = flash_get_data(FLASH_MQTT_IP, 200);
                r2_device->token = flash_get_data(FLASH_MQTT_TOKEN, 65);
                char* ports = flash_get_data(FLASH_MQTT_PORT, 8);
                r2_device->port = 1883;
                aiio_log_i("ip:%s,token:%s,port:%d", r2_device->mqttip, r2_device->token, r2_device->port);

                iot_connect_wifi(flash_ssid, flash_pass);

                vPortFree(flash_pass);
                vPortFree(flash_ssid);
                // vPortFree(mqttip);
                // vPortFree(token);
                // vPortFree(ports);
            }
            break;
            case AIPI_R286_STATE_WIFI_CONNECT_OK://WiFi 连接成功
            {
                aiio_wifi_config_t conf;
                aiio_wifi_get_config(AIIO_WIFI_IF_STA, &conf);
                r2_device->wifi_sta.ssid = conf.sta.ssid;
                r2_device->wifi_sta.passworld = conf.sta.password;
                flash_erase_set(FLASH_SSID_KEY, r2_device->wifi_sta.ssid);
                flash_erase_set(FLASH_PWD_KEY, r2_device->wifi_sta.passworld);

            }
            break;
            case AIPI_R286_STATE_SERVER_CONNECT_OK://服务器连接成功
            {
                //保存信息
                flash_erase_set(FLASH_MQTT_IP, r2_device->mqttip);
                flash_erase_set(FLASH_MQTT_TOKEN, r2_device->token);
                aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
                aiio_service_report_attribute(NULL, NULL, "{ \"4\":false}");
                aiio_service_report_attribute(NULL, NULL, "{ \"5\":false}");
                // lv_label_set_text(r2_device->ui->config_label_ip, r2_device->wifi_sta.ip_addrs);
            }
            break;
            case AIPI_R286_STATE_DEV_REALAY1_ON:
            {
                r2_device->relay_sw1->on(r2_device->relay_sw1);
                aiio_service_report_attribute(NULL, NULL, "{ \"101\":true}");
                if (r2_device->relay_sw1->gpio_state==true &&r2_device->relay_sw2->gpio_state==true&&r2_device->relay_sw3->gpio_state==true)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":true}");

            }
            break;
            case AIPI_R286_STATE_DEV_REALAY1_OFF:
            {
                r2_device->relay_sw1->off(r2_device->relay_sw1);
                aiio_service_report_attribute(NULL, NULL, "{ \"101\":false}");
                if (r2_device->relay_sw1->gpio_state==false &&r2_device->relay_sw2->gpio_state==false&&r2_device->relay_sw3->gpio_state==false)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
            }
            break;
            case AIPI_R286_STATE_DEV_REALAY2_ON:
            {
                r2_device->relay_sw2->on(r2_device->relay_sw2);
                aiio_service_report_attribute(NULL, NULL, "{ \"4\":true}");
                if (r2_device->relay_sw1->gpio_state==true &&r2_device->relay_sw2->gpio_state==true&&r2_device->relay_sw3->gpio_state==true)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":true}");

            }
            break;
            case AIPI_R286_STATE_DEV_REALAY2_OFF:
            {
                r2_device->relay_sw2->off(r2_device->relay_sw2);
                aiio_service_report_attribute(NULL, NULL, "{ \"4\":false}");
                if (r2_device->relay_sw1->gpio_state==false &&r2_device->relay_sw2->gpio_state==false&&r2_device->relay_sw3->gpio_state==false)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
            }
            break;
            case AIPI_R286_STATE_DEV_REALAY3_ON:
            {
                r2_device->relay_sw3->on(r2_device->relay_sw3);
                aiio_service_report_attribute(NULL, NULL, "{ \"5\":true}");
                if (r2_device->relay_sw1->gpio_state==true &&r2_device->relay_sw2->gpio_state==true&&r2_device->relay_sw3->gpio_state==true)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":true}");
            }
            break;
            case AIPI_R286_STATE_DEV_REALAY3_OFF:
            {
                r2_device->relay_sw3->off(r2_device->relay_sw3);
                aiio_service_report_attribute(NULL, NULL, "{ \"5\":false}");
                if (r2_device->relay_sw1->gpio_state==false &&r2_device->relay_sw2->gpio_state==false&&r2_device->relay_sw3->gpio_state==false)
                    aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
            }
            break;
            case AIPI_R286_STATE_DEV_REALAYALL_ON:
                aiio_service_report_attribute(NULL, NULL, "{ \"1\":true}");
                break;
            case AIPI_R286_STATE_DEV_REALAYALL_OFF:
                aiio_service_report_attribute(NULL, NULL, "{ \"1\":false}");
                break;
            case AIPI_R286_STATE_CONFIG_START:
            {

                rev_queue.common_event = REV_CLOUD_DISCONNECTED;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                vTaskDelay(pdMS_TO_TICKS(100));
                rev_queue.common_event = REV_CONFIG_START_EVENT;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }
            break;
            default:
                break;
        }



    }

}

aipi_r286_dev_t* aipi_r286_device_init(void)
{

    // tcpip_init(NULL, NULL);
    // wifi_start_firmware_task();
    bflb_mtd_init();
    easyflash_init();

    relays_device_init(&device);
    device.language = AIPI_R286_LANGUAGE_ZH;
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    device.ui = &guider_ui;
    setup_ui(device.ui);
    custom_init(device.ui);
    events_init(device.ui);

    xTaskCreate(lvgl_tick_task, (char*)"lvgl", 1024, NULL, 1, NULL);
    xTaskCreate(aipi_r286_devstate_start, "sys_state", 1024, NULL, 2, &sys_state_ctl);
    return &device;
}

void aipi_r286_dev_state_update(aipi_r286_state_t new_state)
{
    if (sys_state_ctl==NULL)
    {
        aiio_log_e("sys_state stask is not Create OK!");
        return;
    }
    xTaskNotify(sys_state_ctl, new_state, eSetValueWithoutOverwrite);
}

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

void flash_erase_data(char* key)
{
    ef_del_and_save_env(key);
}
/**
 * @brief
 *
 * @param key
 * @return char*
*/
char* flash_get_data(char* key, int len)
{
    static char* flash_data;
    flash_data = pvPortMalloc(len);
    memset(flash_data, 0, len);

    ef_get_env_blob(key, flash_data, len, (size_t)&len);

    return flash_data;
}

int AiPi_R286_get_mqtt_control(const char* payload, uint8_t* pid)
{
    int ret = -1;
    if (payload==NULL) {
        aiio_log_e("payload is null");
        return -1;
    }
    cJSON* root = cJSON_Parse(payload);
    if (root==NULL) {
        aiio_log_e("payload is not json data");
        return -1;
    }

    cJSON* control = cJSON_GetObjectItem(root, STR_NAME_CONTROL);
    cJSON* json_pid = cJSON_GetObjectItem(control, "1");
    if (json_pid!=NULL) {
        *pid = CMD_SWALL;
        goto __exit;
    }
    json_pid = cJSON_GetObjectItem(control, "4");
    if (json_pid!=NULL) {
        *pid = CMD_SW2;
        goto __exit;
    }
    json_pid = cJSON_GetObjectItem(control, "5");
    if (json_pid!=NULL) {
        *pid = CMD_SW3;
        goto __exit;
    }
    json_pid = cJSON_GetObjectItem(control, "101");
    if (json_pid!=NULL) {
        *pid = CMD_SW1;
        goto __exit;
    }

__exit:

    ret = json_pid->valueint;
    cJSON_Delete(root);
    return ret;
}
