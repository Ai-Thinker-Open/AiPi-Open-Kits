/**
 * @file device_ctrl.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef DEVICE_CTRL_H
#define DEVICE_CTRL_H

#define FLASH_SSID_KEY "SSID"
#define FLASH_PASS_KEY "PASS"

typedef enum {
    DEVICE_STATE_NONE = 0,
    DEVICE_STATE_SYSTEM_START,
    DEVICE_STATE_WIFI_CONNECT,
    DEVICE_STATE_WIFI_CONNECT_OK,
    DEVICE_STATE_WIFI_GO_IP,
    DEVICE_STATE_WIFI_DISCONNECT,
    DEVICE_STATE_SYSTEM_TIME_UPDATE,
    DEVICE_STATE_HTTP_GET_WEATHER,
}device_state_t;

extern char wea[16];
extern char city[32];
extern char tem_day[16];
extern char tem_tem_night[16];
extern char win[16];
extern char air[16];
extern char humidity[16];
extern char pressure[16];

void* device_task;
void device_state_task(void* arg);
void device_send_state_notify(device_state_t device_state, int is_irq);
#endif