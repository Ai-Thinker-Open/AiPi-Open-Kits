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

#define MQTT_CLIENT_ID "AiPi-SCP_2.4"

typedef struct {
    char host[512];
    int port;
    char user_name[512];
    char pass[512];
}user_mqtt_client_t;

void  mqtt_client_init(const char* host, int port);
int mqtt_start_connect(char* host, uint16_t port, char* user_name, char* pass);
int mqtt_app_subscribe(char* topic, int qos);
int mqtt_app_publish(const char* topic, const char* payload, int qos);
void mqtt_app_diconnect(void);
#endif