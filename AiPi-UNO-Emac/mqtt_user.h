#ifndef MQTT_USER_H__
#define MQTT_USER_H__

#define MQTT_ENV_INFO "MQTT_ENV_INFO"
#define MQTT_ENV_ADDR "MQTT_ENV_ADDR"
#define MQTT_ENV_PORT "MQTT_ENV_PORT"

typedef struct 
{
    char s_addr[64];
    char s_port[8];
    char s_topic[32];

} MqttItem_t;



int MqttConnect(char *addr, char *port);


#endif //MQTT_USER_H__
