/**
 * @file sys_status.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
*/

#ifndef SYS_STATUS_H
#define SYS_STATUS_H

#define MQTT_SERVER "your mqtt host"
#define MQTT_USER_NAME "mqtt user name"
#define MQTT_PASSWOLD "mqtt passworld"

#define MQTT_PORT 1883
#define FISH_MQTT_SUB_TOPIC "/FishTank/a81710cf69dd/devPub"
#define MQTT_CLIETN_TOPIC "topic/dev/pub"
#define MQTT_CLIETN_PUB_TOPIC "topic/dev/sub"

typedef enum {
    CUSTOM_EVENT_NONE = 0,
    CUSTOM_EVENT_WIFI_SCAN_DONE,
    CUSTOM_EVENT_GET_WIFI,
    CUSTOM_EVENT_GOT_IP,
    CUSTOM_EVENT_GET_WIFI_DISCONNECT,
    CUSTOM_EVENT_MQTT_CONNECT,
    CUSTOM_EVENT_MQTT_DISCONNECT,
    CUSTOM_EVENT_MQTT_GET_MSG,
    CUSTOM_EVENT_MQTT_PUB_MSG,
}custom_event_t;

void system_status_init(void);
#endif