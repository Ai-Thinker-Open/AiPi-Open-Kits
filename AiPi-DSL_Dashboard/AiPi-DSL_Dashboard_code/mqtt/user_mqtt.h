/**
 * @file user_mqtt.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-21
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef USER_MQTT_H
#define USER_MQTT_H

#define MQTT_CLIENT_ID "AiPi-Dashboard"

typedef struct {
    char host[512];
    int port;
    char user_name[512];
    char pass[512];
}user_mqtt_client_t;

typedef enum {
    MQTT_EVENT_NONE = 0,
    MQTT_EVENT_CONNECT,
    MQTT_EVENT_CONNACK,
    MQTT_EVENT_PUBLISH,
    MQTT_EVENT_PUBACK,
    MQTT_EVENT_PUBREC,
    MQTT_EVENT_PUBREL,
    MQTT_EVENT_PUBCOMP,
    MQTT_EVENT_SUBSCRIBE,
    MQTT_EVENT_SUBACK,
    MQTT_EVENT_UNSUBSCRIBE,
    MQTT_EVENT_UNSUBACK,
    MQTT_EVENT_PINGREQ,
    MQTT_EVENT_PINGRESP,
    MQTT_EVENT_DISCONNECT
}mqtt_event_t;

void  mqtt_client_init(const char* host, int port);
int mqtt_start_connect(char* host, uint16_t port, char* user_name, char* pass);
int mqtt_app_subscribe(char* topic, int qos);
int mqtt_app_publish(const char* topic, const char* payload, int qos);
void mqtt_app_diconnect(void);
void mqtt_client_register_event(void);
#endif