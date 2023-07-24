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

#define MQTT_CLIENT_ID "AiPi-DSL_Dashboard"

typedef struct {
    char* host;
    int port;
    char* user_name;
    char* pass;
}user_mqtt_client_t;

void mqtt_client_init(void);
int mqtt_start_connect(char* host, uint16_t port, char* user_name, char* pass);
int mqtt_app_subscribe(char* topic, int qos);
int mqtt_app_publish(char* topic, char* payload, int qos);
#endif