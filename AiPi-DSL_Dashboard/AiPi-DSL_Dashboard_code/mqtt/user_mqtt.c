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

#include "user_mqtt.h"
#include "custom.h"
#include <lwip/inet.h>
#include "bflb_uart.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <lwip/errno.h>
#include <netdb.h>
#include "bflb_uart.h"
#include "voice_uart.h"
#include "utils_getopt.h"
#define DBG_TAG "MQTT"


// #define USER_MQTT
extern xQueueHandle queue;
int test_sockfd;
struct mqtt_client client;
static mqtt_event_t event;
uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

static TaskHandle_t client_daemon;
static int open_socket(const char* host, const char* port);

static int open_socket(const char* host, const char* port)
{
    struct addrinfo hints = { 0 };
    hints.ai_family = AF_UNSPEC; /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Must be TCP */
    int sockfd = -1;
    int rv;
    struct addrinfo* p, * servinfo;

    /* get address information */
    LOG_I("host:%s, port=%s", host, port);
    rv = getaddrinfo(host, port, &hints, &servinfo);
    if (rv != 0) {
        LOG_E("Failed to open socket (getaddrinfo): %d", rv);
        return -1;
    }

    /* open the first possible socket */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        /* connect to server */
        rv = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if (rv == -1) {
            close(sockfd);
            sockfd = -1;
            continue;
        }
        break;
    }

    /* free servinfo */
    freeaddrinfo(servinfo);

    /* make non-blocking */
    if (sockfd != -1) {
        int iMode = 1;
        ioctlsocket(sockfd, FIONBIO, &iMode);
    }

    return sockfd;
}
/**
 * @brief
 *
 * @param sig
*/
static void test_close(void)
{
    if (test_sockfd)
    {
        close(test_sockfd);
    }
    vTaskDelete(client_daemon);

}
/**
 * @brief
 *
 * @param unused
 * @param published
*/
static void publish_callback_1(void** unused, struct mqtt_response_publish* published)
{
    /* not used in this example */

    char* topic_name = (char*)malloc(published->topic_name_size + 1);
    memcpy(topic_name, published->topic_name, published->topic_name_size);
    topic_name[published->topic_name_size] = '\0';

    char* topic_msg = (char*)malloc(published->application_message_size + 1);
    memcpy(topic_msg, published->application_message, published->application_message_size);
    topic_msg[published->application_message_size] = '\0';
    printf("%s:[%s]\r\n", topic_name, topic_msg);
    char* queue_buff = pvPortMalloc(512);
    memset(queue_buff, 0, 512);
    sprintf(queue_buff, "{\"mqtt_msg\":{\"topic\":\"%s\",\"data\":%.*s}}", topic_name, published->application_message_size, topic_msg);
    xQueueSendFromISR(queue, queue_buff, pdTRUE);
    vPortFree(queue_buff);
    free(topic_name);
    free(topic_msg);
}
/**
 * @brief  _inspector_callback
 *
 * @param client_arg
 * @return enum MQTTErrors
*/
enum MQTTErrors _inspector_callback(struct mqtt_client* client_arg)
{
    if (client_arg->mq.queue_tail->state!=MQTT_QUEUED_UNSENT) return MQTT_OK; //识别消息状态是否有更新，没有更新就返回

    event = client_arg->mq.queue_tail->control_type;
    static struct bflb_device_s* uartx;
    uartx = bflb_device_get_by_name("uart1");
    switch (event) {
        case MQTT_EVENT_CONNECT:
        {
            LOG_I("MQTT_EVENT_CONNECT");
            bflb_uart_put(uartx, user_data[UART_CMD_CONNECT_SERVER_OK].uart_data.data, 4);
            guider_ui.mqtt_connect_status = true;
        }
        break;
        case MQTT_EVENT_CONNACK:
        {
            LOG_I("MQTT_EVENT_CONNACK");
        }
        break;
        case MQTT_EVENT_PUBLISH:
        {
            LOG_I("MQTT_EVENT_PUBLISH");
        }
        break;
        case MQTT_EVENT_PUBACK:
        {
            LOG_I("MQTT_EVENT_PUBACK");
        }
        break;
        case MQTT_EVENT_PUBREC:
        {
            LOG_I("MQTT_EVENT_PUBREC");
        }
        break;
        case MQTT_EVENT_PUBREL:
        {
            LOG_I("MQTT_EVENT_PUBREL");
        }
        break;
        case MQTT_EVENT_PUBCOMP:
        {
            LOG_I("MQTT_EVENT_PUBCOMP");
        }
        break;
        case MQTT_EVENT_SUBSCRIBE:
        {
            LOG_I("MQTT_EVENT_SUBSCRIBE");
        }
        break;
        case MQTT_EVENT_SUBACK:
        {
            LOG_I("MQTT_EVENT_SUBACK");
        }
        break;
        case MQTT_EVENT_UNSUBSCRIBE:
        {
            LOG_I("MQTT_EVENT_UNSUBSCRIBE");
        }
        break;
        case MQTT_EVENT_UNSUBACK:
        {
            LOG_I("MQTT_EVENT_UNSUBACK");
        }
        break;
        case MQTT_EVENT_PINGREQ:
        {
            LOG_I("MQTT_EVENT_PINGREQ");
        }
        break;
        case MQTT_EVENT_PINGRESP:
        {
            LOG_I("MQTT_EVENT_PINGRESP");
        }
        break;
        case MQTT_EVENT_DISCONNECT:
        {
            LOG_I("MQTT_EVENT_DISCONNECT");
            guider_ui.mqtt_connect_status = false;
        }
        break;
        default:
            break;
    }
    return MQTT_OK;
}

/**
 * @brief
 *
 * @param host
 * @param port
*/
void  mqtt_client_init(const char* host, int port)
{
    LOG_I("MQTT init start");
    test_sockfd = open_socket(host, "1883");
    if (test_sockfd < 0) {
        LOG_E("Failed to open socket: %d", test_sockfd);
        test_close();
        return;
    }
    LOG_I(" test_sockfd  crater OK id=%d", test_sockfd);

    mqtt_init(&client, test_sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback_1);

}
/**
 * @brief
 *
 * @param client
*/
static void client_refresher(void* arg)
{
    struct mqtt_client* client = (struct mqtt_client*)arg;
    while (1)
    {
        mqtt_sync(client);
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
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
    int ret = 0;

    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    ret = mqtt_connect(&client, MQTT_CLIENT_ID, NULL, NULL, 0, user_name, pass, connect_flags, 400);
    if (ret != MQTT_OK)
    {
        LOG_E("MQTT init fail");
    }
    /* check that we don't have any errors */
    if (client.error != MQTT_OK) {
        LOG_E("error: %s", mqtt_error_str(client.error));
        test_close();
    }
    xTaskCreate(client_refresher, (char*)"client_ref", 1024, &client, 10, &client_daemon);
    return 0;
}

void mqtt_app_diconnect(void)
{
    test_close();
}
/**
 * @brief  mqtt_app_publish
 *
 * @param topic
 * @param payload
 * @param qos
 * @return int
*/
int mqtt_app_publish(const char* topic, const char* payload, int qos)
{
    static enum MQTTPublishFlags qos_flags;
    if (qos==0)qos_flags = MQTT_PUBLISH_QOS_0;
    else if (qos==1)qos_flags = MQTT_PUBLISH_QOS_1;
    else qos_flags = MQTT_PUBLISH_QOS_2;

    mqtt_publish(&client, topic, payload, strlen(payload), qos_flags);
    if (client.error != MQTT_OK) {
        LOG_E("error: %s", mqtt_error_str(client.error));
        test_close();
        return -1;
    }
    return 0;
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
    mqtt_subscribe(&client, topic, 0);
    return 0;
}


void mqtt_client_register_event(void)
{
    client.inspector_callback = _inspector_callback;
}
