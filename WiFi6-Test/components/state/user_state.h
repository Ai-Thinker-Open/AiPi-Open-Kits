/**
 * @file user_state.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef USER_STATE_H
#define USER_STATE_H

#define SSID "AiPi-WiFi6"
#define PASS "fae12345678"

#define SSID_KEY "SSID"
#define PASS_KEY "PASS"
#define CHANNEL_KEY "CH"
#define LED_KEY "LED"

#define MQ_USERNAME "Wi-Fi6"
#define MQ_DEF_HOST "192.168.2.2"
#define MQ_DEF_PORT 1883
#define MQ_HOST_KEY "MQ_HOST"
#define MQ_SUB_TOPIC "dev/wifi/sub"
#define MQ_PUB_TOPIC "dev/wifi/pub"

typedef enum {
    STATE_SYSTEM_START = 0,
    STATE_WIFI_CONNECTING,
    STATE_WIFI_CONNECTED_OK,
    STATE_WIFI_DISCONNECTED,
    STATE_MQTT_SERVER_CONENCT_OK,
    STATE_MQTT_SERVER_CONNECT_ERR,
    STATE_MQTT_SERVER_DISCONNECT,
    STATE_MQTT_MSG_ARRIVES,
}user_state_t;

typedef struct {
    char data[512];
    char topic[128];
}mq_msg_t;
extern mq_msg_t mq_msg;

void user_state_start(void);
void user_state_send_notify(user_state_t state, int is_irq);
int get_server_led(char* data);
char* led_payload_update(int led_state);
#endif