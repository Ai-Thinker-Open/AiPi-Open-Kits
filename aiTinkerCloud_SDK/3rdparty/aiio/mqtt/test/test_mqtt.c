/*
 * This test code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#include <FreeRTOS.h>
#include <event_groups.h>
#include "aiio_mqtt_client.h"
#include "sdkconfig.h"
#include "test_mqtt_client_broker.h"


void aiio_mqtt_init_invalid_url_case(void)
{
    const aiio_mqtt_client_config_t mqtt_cfg = {
            .uri = "INVALID",
    };
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);
}


void aiio_mqtt_init_deinit_case(void)
{
    const aiio_mqtt_client_config_t mqtt_cfg = {
            // no connection takes place, but the uri has to be valid for init() to succeed
            .uri = "mqtts://localhost:8883",
    };
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);
    aiio_mqtt_client_destroy(client);
}


void aiio_mqtt_enqueue_destroy_outbox_case(void)
{
    const int messages = 20;
    const int size = 0;
    const aiio_mqtt_client_config_t mqtt_cfg = {
            // no connection takes place, but the uri has to be valid for init() to succeed
            .uri = "mqtts://localhost:8883",
    };
    aiio_mqtt_client_handle_t client = aiio_mqtt_client_init(&mqtt_cfg);

    for (int i=0; i<messages; ++i) {
        aiio_mqtt_client_publish(client, "test", "date", size, 1, 0);
    }
    aiio_mqtt_client_destroy(client);
}

#if SOC_EMAC_SUPPORTED
/**
 * This test cases uses ethernet kit, so build and use it only if EMAC supported
 */
TEST_CASE("mqtt broker tests", "[mqtt][test_env=UT_T2_Ethernet]")
{
    test_case_uses_tcpip();
    connect_test_fixture_setup();

    RUN_MQTT_BROKER_TEST(mqtt_connect_disconnect);
    RUN_MQTT_BROKER_TEST(mqtt_subscribe_publish);
    RUN_MQTT_BROKER_TEST(mqtt_lwt_clean_disconnect);
    RUN_MQTT_BROKER_TEST(mqtt_subscribe_payload);

    connect_test_fixture_teardown();
}
#endif // SOC_EMAC_SUPPORTED
