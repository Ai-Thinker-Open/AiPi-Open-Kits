/**
 * @file user_mqtt.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-21
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "log.h"
#include <mqtt.h>
#include "custom.h"
#include "user_mqtt.h"
#include <lwip/inet.h>
#define GBD_TAG "MQTT"

static mqtt_client_t* AiPi_client;


static void mqtt_event_connect_cb(mqtt_client_t* client, void* arg, mqtt_connection_status_t status);
static void mqtt_request_cb(void* arg, err_t err);
/**
 * @brief
 *
*/
void mqtt_client_init(void)
{
    if (AiPi_client!=NULL) {
        mqtt_client_free(AiPi_client);
    }
    AiPi_client = mqtt_client_new();
}


/**
 * @brief mqtt_start_connect
 *  MQTT启动连接
 * @param host
 * @param port
 * @param user_name
 * @param pass
 * @return int
*/
int mqtt_start_connect(char* host, uint16_t port, char* user_name, char* pass)
{
    struct in_addr addr;
    netconn_gethostbyname(host, &addr);
    struct mqtt_connect_client_info_t mqtt_client_info = {
       .client_id = MQTT_CLIENT_ID,
       .client_pass = pass,
       .client_user = user_name,
       .keep_alive = 120,
    };

    if (mqtt_client_is_connected(AiPi_client))
    {
        mqtt_disconnect(AiPi_client);
    }

    user_mqtt_client_t user_mqtt_client = {
        .host = host,
        .port = port,
        .user_name = user_name,
        .pass = pass
    };

    return mqtt_client_connect(AiPi_client, &addr, port, mqtt_event_connect_cb, (void*)&user_mqtt_client, &mqtt_client_info);
}


/**
 * @brief mqtt_app_subscribe
 *
 * @param topic
 * @param qos
 * @return int
*/
int mqtt_app_subscribe(char* topic, int qos)
{
    if (mqtt_client_is_connected(AiPi_client))
    {
        return mqtt_subscribe(AiPi_client, topic, qos, mqtt_request_cb, NULL);
    }
    else LOG_E("MQTT clien is no connect");
}
/**
 * @brief  mqtt_app_publish
 *
 * @param topic
 * @param payload
 * @param qos
 * @return int
*/
int mqtt_app_publish(char* topic, char* payload, int qos)
{
    if (mqtt_client_is_connected(AiPi_client))
    {
        return mqtt_publish(AiPi_client, topic, payload, strlen(payload), qos, 0, mqtt_request_cb, NULL);
    }
    else LOG_E("MQTT clien is no connect");
    return -1;
}

static void mqtt_event_connect_cb(mqtt_client_t* client, void* arg, mqtt_connection_status_t status)
{
    user_mqtt_client_t* user_mqtt_client = (user_mqtt_client_t*)arg;
    switch (status)
    {
        //connect OK
        case MQTT_CONNECT_ACCEPTED:
        {
            LOG_I("MQTT event MQTT_CONNECT_ACCEPTED");
        }
        break;
        /** Refused protocol version */
        case MQTT_CONNECT_REFUSED_PROTOCOL_VERSION:
            LOG_I("MQTT event MQTT_CONNECT_REFUSED_PROTOCOL_VERSION");
            break;
            /** Refused identifier */
        case MQTT_CONNECT_REFUSED_IDENTIFIER:
            LOG_I("MQTT event MQTT_CONNECT_REFUSED_IDENTIFIER");
            break;
            /** Refused server */
        case MQTT_CONNECT_REFUSED_SERVER:
            LOG_I("MQTT event MQTT_CONNECT_REFUSED_SERVER");
            break;
            /** Refused user credentials */
        case MQTT_CONNECT_REFUSED_USERNAME_PASS:
            LOG_I("MQTT event MQTT_CONNECT_REFUSED_USERNAME_PASS");
            break;
            /** Refused not authorized */
        case MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_:
            LOG_I("MQTT event MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_");
            break;
            /** Disconnected */
        case MQTT_CONNECT_DISCONNECTED:
        {
            LOG_I("MQTT event MQTT_CONNECT_DISCONNECTED");
            struct in_addr addr;

            netconn_gethostbyname(user_mqtt_client->host, &addr);
            struct mqtt_connect_client_info_t mqtt_client_info = {
               .client_id = MQTT_CLIENT_ID,
               .client_pass = user_mqtt_client->pass,
               .client_user = user_mqtt_client->user_name,
               .keep_alive = 120,
            };
            mqtt_client_connect(AiPi_client, &addr, user_mqtt_client->port, mqtt_event_connect_cb, (void*)&user_mqtt_client, &mqtt_client_info);
        }
        break;
        /** Timeout */
        case MQTT_CONNECT_TIMEOUT:
            LOG_I("MQTT event MQTT_CONNECT_TIMEOUT");
            break;
            /* code */
        default:
            break;
    }
}


static void mqtt_request_cb(void* arg, err_t err)
{
    LOG_I("mqtt_request MSG=%d", err);
}