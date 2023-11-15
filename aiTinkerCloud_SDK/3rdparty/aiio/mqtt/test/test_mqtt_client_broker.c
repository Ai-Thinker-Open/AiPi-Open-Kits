
#include <FreeRTOS.h>
#include <event_groups.h>
#include "aiio_mqtt_client.h"
#include "aiio_log.h"
#include "aiio_wifi.h"

#define WAIT_FOR_EVENT(event) \
    TEST_ASSERT_TRUE(xEventGroupWaitBits(s_event_group, event, pdTRUE, pdTRUE, pdMS_TO_TICKS(COMMON_OPERATION_TIMEOUT)) & event);

#define TEST_ASSERT_TRUE(condition) TEST_ASSERT_TRUE_LINE(condition, __LINE__)
#define TEST_ASSERT_TRUE_LINE(condition, line) \
    do {                                       \
        if (!(condition)) {                    \
                aiio_log_e("test_mqtt_client_broker.c", \
                "Assertion failed in line %d", line); \
                return false;                         \
            } \
    } while(0)


static const int COMMON_OPERATION_TIMEOUT = 10000;
static const int CONNECT_BIT = BIT0;
static const int DISCONNECT_BIT = BIT1;
static const int DATA_BIT = BIT2;

static EventGroupHandle_t s_event_group;

static char* append_mac(const char* string)
{
    uint8_t mac[6];
    char *id_string = NULL;
    aiio_wifi_sta_mac_get(mac);
    asprintf(&id_string, "%s_%02x%02X%02X", string, mac[3], mac[4], mac[5]);
    return id_string;
}

static void mqtt_data_handler_qos(void *handler_args, aiio_event_base_t base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_DATA) {
        aiio_mqtt_event_handle_t event = event_data;
        int * qos  = handler_args;
        *qos = event->qos;
        xEventGroupSetBits(s_event_group, DATA_BIT);
    }
}

static void mqtt_data_handler_lwt(void *handler_args, aiio_event_base_t base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_DATA) {
        aiio_mqtt_event_handle_t event = event_data;
        aiio_log_i("mqtt-lwt", "MQTT_EVENT_DATA");
        aiio_log_i("mqtt-lwt", "TOPIC=%.*s", event->topic_len, event->topic);
        aiio_log_i("mqtt-lwt", "DATA=%.*s", event->data_len, event->data);
        if (strncmp(event->data, "no-lwt", event->data_len) == 0) {
            // no lwt, just to indicate the test has finished
            xEventGroupSetBits(s_event_group, DATA_BIT);
        } else {
            // count up any potential lwt message
            int * count  = handler_args;
            *count = *count + 1;
            aiio_log_e("mqtt-lwt", "count=%d", *count);
        }
    }
}

static void mqtt_data_handler_subscribe(void *handler_args, aiio_event_base_t base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_SUBSCRIBED) {
        aiio_mqtt_event_handle_t event = event_data;
        aiio_log_i("mqtt-subscribe", "MQTT_EVENT_SUBSCRIBED, data size=%d", event->data_len);
        int * sub_payload  = handler_args;
        if (event->data_len == 1) {
            aiio_log_i("mqtt-subscribe", "DATA=%d", *(uint8_t*)event->data);
            *sub_payload = *(uint8_t*)event->data;
        }
        xEventGroupSetBits(s_event_group, DATA_BIT);
    }
}


static void mqtt_event_handler(void *handler_args, aiio_event_base_t base, int32_t event_id, void *event_data)
{
    switch ((aiio_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            xEventGroupSetBits(s_event_group, CONNECT_BIT);
            break;

        case MQTT_EVENT_DISCONNECTED:
            xEventGroupSetBits(s_event_group, DISCONNECT_BIT);
            break;
        default:
            break;
    }
}

bool mqtt_connect_disconnect(void)
{
    const aiio_mqtt_client_config_t mqtt_cfg = {
            .uri = "mqtt://mqtt.eclipseprojects.io",
            .disable_auto_reconnect = true,
    };
    s_event_group = xEventGroupCreate();
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);
    TEST_ASSERT_TRUE(NULL != client );
    aiio_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    TEST_ASSERT_TRUE(AIIO_OK == aiio_mqtt_client_start(client));
    WAIT_FOR_EVENT(CONNECT_BIT);
    aiio_mqtt_client_disconnect(client);
    WAIT_FOR_EVENT(DISCONNECT_BIT);
    aiio_mqtt_client_reconnect(client);
    WAIT_FOR_EVENT(CONNECT_BIT);
    aiio_mqtt_client_destroy(client);
    vEventGroupDelete(s_event_group);
    return true;
}

bool mqtt_subscribe_publish(void)
{
    const aiio_mqtt_client_config_t mqtt_cfg = {
            .uri = "mqtt://mqtt.eclipseprojects.io",
    };
    char* topic = append_mac("topic");
    TEST_ASSERT_TRUE(NULL != topic);
    s_event_group = xEventGroupCreate();
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);
    TEST_ASSERT_TRUE(NULL != client );
    aiio_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    TEST_ASSERT_TRUE(AIIO_OK == aiio_mqtt_client_start(client));
    WAIT_FOR_EVENT(CONNECT_BIT);
    int qos = -1;
    aiio_mqtt_client_register_event(client, MQTT_EVENT_DATA, mqtt_data_handler_qos, &qos);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client, topic, 2) != -1);
    TEST_ASSERT_TRUE(aiio_mqtt_client_publish(client, topic, "message", 0, 2, 0) != -1);
    WAIT_FOR_EVENT(DATA_BIT);
    TEST_ASSERT_TRUE(qos == 2);
    TEST_ASSERT_TRUE(aiio_mqtt_client_publish(client, topic, "message", 0, 1, 0) != -1);
    WAIT_FOR_EVENT(DATA_BIT);
    TEST_ASSERT_TRUE(qos == 1);
    aiio_mqtt_client_destroy(client);
    vEventGroupDelete(s_event_group);
    free(topic);
    return true;
}

bool mqtt_lwt_clean_disconnect(void)
{
    char* lwt = append_mac("lwt");
    TEST_ASSERT_TRUE(lwt);
    const aiio_mqtt_client_config_t mqtt_cfg1 = {
            .uri = "mqtt://mqtt.eclipseprojects.io",
            .set_null_client_id = true,
            .lwt_topic = lwt,
            .lwt_msg = "lwt_msg"
    };
    const aiio_mqtt_client_config_t mqtt_cfg2 = {
            .uri = "mqtt://mqtt.eclipseprojects.io",
            .set_null_client_id = true,
            .lwt_topic = lwt,
            .lwt_msg = "lwt_msg"
    };
    s_event_group = xEventGroupCreate();

    aiio_mqtt_client_handle_t client1 = aiio_mqtt_client_init(&mqtt_cfg1);
    aiio_mqtt_client_handle_t client2 = aiio_mqtt_client_init(&mqtt_cfg2);
    TEST_ASSERT_TRUE(NULL != client1 && NULL != client2 );
    aiio_mqtt_client_register_event(client1, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    aiio_mqtt_client_register_event(client2, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    TEST_ASSERT_TRUE(aiio_mqtt_client_start(client1) == AIIO_OK);
    WAIT_FOR_EVENT(CONNECT_BIT);
    TEST_ASSERT_TRUE(aiio_mqtt_client_start(client2) == AIIO_OK);
    WAIT_FOR_EVENT(CONNECT_BIT);
    int counter = 0;
    aiio_mqtt_client_register_event(client1, MQTT_EVENT_DATA, mqtt_data_handler_lwt, &counter);
    aiio_mqtt_client_register_event(client2, MQTT_EVENT_DATA, mqtt_data_handler_lwt, &counter);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client1, lwt, 0) != -1);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client2, lwt, 0) != -1);
    aiio_mqtt_client_disconnect(client1);
    WAIT_FOR_EVENT(DISCONNECT_BIT);
    aiio_mqtt_client_reconnect(client1);
    WAIT_FOR_EVENT(CONNECT_BIT);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client1, lwt, 0) != -1);
    aiio_mqtt_client_stop(client2);
    aiio_mqtt_client_start(client2);
    WAIT_FOR_EVENT(CONNECT_BIT);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client2, lwt, 0) != -1);
    TEST_ASSERT_TRUE(aiio_mqtt_client_publish(client1, lwt, "no-lwt", 0, 0, 0) != -1);
    WAIT_FOR_EVENT(DATA_BIT);
    TEST_ASSERT_TRUE(counter == 0);
    aiio_mqtt_client_destroy(client1);
    aiio_mqtt_client_destroy(client2);
    vEventGroupDelete(s_event_group);
    free(lwt);
    return true;
}

bool mqtt_subscribe_payload(void)
{
    const aiio_mqtt_client_config_t mqtt_cfg = {
            .uri = "mqtt://mqtt.eclipseprojects.io",
            .disable_auto_reconnect = true,
    };
    char* topic = append_mac("topic");
    TEST_ASSERT_TRUE(NULL != topic);
    s_event_group = xEventGroupCreate();
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);
    TEST_ASSERT_TRUE(NULL != client );
    aiio_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
    TEST_ASSERT_TRUE(AIIO_OK == aiio_mqtt_client_start(client));
    WAIT_FOR_EVENT(CONNECT_BIT);
    int qos_payload = -1;
    aiio_mqtt_client_register_event(client, MQTT_EVENT_SUBSCRIBED, mqtt_data_handler_subscribe, &qos_payload);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client, topic, 2) != -1);
    WAIT_FOR_EVENT(DATA_BIT);
    TEST_ASSERT_TRUE(qos_payload == 2);
    TEST_ASSERT_TRUE(aiio_mqtt_client_subscribe(client, topic, 0) != -1);
    WAIT_FOR_EVENT(DATA_BIT);
    TEST_ASSERT_TRUE(qos_payload == 0);
    aiio_mqtt_client_destroy(client);
    vEventGroupDelete(s_event_group);
    free(topic);
    return true;
}
