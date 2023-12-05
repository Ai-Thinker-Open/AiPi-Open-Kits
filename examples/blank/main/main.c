#include "shell.h"
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "board.h"

#include "bluetooth.h"
#include "conn.h"
#include "conn_internal.h"
#if defined(BL702) || defined(BL602)
#include "ble_lib_api.h"
#elif defined(BL616)
#include "btble_lib_api.h"
#include "bl616_glb.h"
#include "rfparam_adapter.h"
#elif defined(BL808)
#include "btble_lib_api.h"
#include "bl808_glb.h"
#endif

#include "ble_tp_svc.h"
#include "hci_driver.h"
#include "hci_core.h"

#include "bflb_mtd.h"
#include "easyflash.h"

#include "aiio_os_port.h"
#include "aiio_common.h"
#include "aiio_wan_service.h"
#include "aiio_cloud_receive.h"

#define QUEUE_MAX_SIZE                      (5)             /* max number of queue*/

#define WIFI_MAX_DISCONNECT_NUMBER          (10)

static aiio_os_thread_handle_t* aiio_test_sta_wifi_thread = NULL;           /*!< handle of task thread*/
static aiio_os_thread_handle_t* aiio_start_thread = NULL;           /*!< handle of task thread*/
bool wifi_connect = false;                                           /*!< status of wifi connection*/

bool ble_config_start = false;                                       /*!< Launch state of ble distribution network*/
bool wifi_config_start = false;

static bool wifi_start_connected = false;
static uint8_t wifi_disconnect_count = 0;                                   /*!< The number  of wifi disconnection*/
static aiio_wifi_info_t wifi_info = { 0 };                                    /*!< Information of wifi*/
static wifi_config_data_t wifi_config_data = { 0 };                             /*!< The data of ble distribution network */

static bool cloud_connect = false;
/* Define global variable */
QueueHandle_t    cloud_rev_queue_handle = NULL;                             /*!< handle of queue*/
aiio_device_info DeviceInfo = { 0 };

static void iot_connect_wifi(char* ssid, char* passwd)
{
    if (ssid == NULL)
    {
        aiio_log_e("ssid is NULL \r\n");
        return;
    }

    aiio_wifi_set_mode(AIIO_WIFI_MODE_STA);

    aiio_wifi_config_t wifi_sta_config = { 0 };

    memcpy(wifi_sta_config.sta.ssid, ssid, strlen(ssid));
    if (passwd)
    {
        memcpy(wifi_sta_config.sta.password, passwd, strlen(passwd));
    }
    wifi_sta_config.sta.channel = 0;
    wifi_sta_config.sta.use_dhcp = 1;
    wifi_sta_config.sta.flags = 0;

    aiio_log_d("ssid = %s \r\n", wifi_sta_config.sta.ssid);
    aiio_log_d("password = %s \r\n", wifi_sta_config.sta.password);
    aiio_wifi_set_config(AIIO_WIFI_IF_STA, &wifi_sta_config);

    aiio_wifi_start();
    wifi_disconnect_count = 0;
    wifi_start_connected = true;
}



static void cb_wifi_event(aiio_input_event_t* event, void* data)
{
    int32_t ret = 0;
    aiio_rev_queue_t  rev_queue = { 0 };

    switch (event->code)
    {
        case AIIO_WIFI_EVENT_WIFI_READY:
        {
            aiio_log_i("<<<<<<<<<  WIFI INIT OK <<<<<<<<<<");
            // iot_connect_wifi("ZLM_24G", "12345678");
        }
        break;

        case AIIO_WIFI_EVENT_SCAN_DONE:
        {
            aiio_log_i("<<<<<<<<<  SCAN DONE OK <<<<<<<<<<");
        }
        break;

        case AIIO_WIFI_EVENT_STA_CONNECTING:
        {
            aiio_log_i("<<<<<<<<< STA_CONNECTING <<<<<<<<<<<");
        }
        break;

        case AIIO_WIFI_EVENT_STA_CONNECTED:
        {
            aiio_log_i("<<<<<<<<< CONNECTED<<<<<<<<<<<");
        }
        break;

        case AIIO_WIFI_EVENT_STA_DISCONNECTED:
        {
            aiio_log_i("<<<<<<<<<  DISCONNECTED <<<<<<<<<<");
            wifi_disconnect_count++;
            if (event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE ||
                    event->value == AIIO_WLAN_FW_AUTH_OR_ASSOC_RESPONSE_TIMEOUT_FAILURE)
            {
                //connect timeout
                aiio_log_i("connect timeout");
                if (ble_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);
                }
            }
            else if (event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_TRANSMIT_FAILURE ||
                    event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_ALLOCATE_FAIILURE ||
                    event->value == AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_NOT_CONNECTION)
            {
                //password error
                aiio_log_i("password error");
                if (ble_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    aiio_ble_config_response_status(AIIO_BLE_CODE_WIFI_PAWD_ERR);
                }
            }
            else if (event->value == AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL)
            {
                //not found AP
                aiio_log_i("not found AP");
                if (ble_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    aiio_ble_config_response_status(AIIO_BLE_CODE_WIFI_NO_SSID);
                }
            }
            else if ((event->value == AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_CONNECTION) || (event->value == AIIO_WLAN_FW_DISCONNECT_BY_USER_WITH_DEAUTH))
            {
                //wifi disconnect
                aiio_log_i("wifi disconnect");
                if (ble_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);
                }
            }
            else
            {
                //connect error
                aiio_log_i("connect error");
                if (ble_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);
                }
            }

            if (wifi_config_start && wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
            {
                rev_queue.common_event = REV_CONFIG_FAIL_EVENT;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                wifi_disconnect_count = 0;
            }
            else
            {
                if (wifi_disconnect_count > WIFI_MAX_DISCONNECT_NUMBER)
                {
                    wifi_disconnect_count = 0;
                    rev_queue.common_event = REV_WIFI_CONNECTED_EVENT;
                    if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                    {
                        aiio_log_i("queue send failed\r\n");
                    }
                    memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                }
            }
            wifi_connect = false;
            wifi_start_connected = false;

        }
        break;
        case AIIO_WIFI_EVENT_STA_GOT_IP:
        {
            aiio_log_i("<<<<<<<<< CONNECTED GOT IP <<<<<<<<<<<");
            aiio_ble_config_response_status(AIIO_BLE_CODE_WIFI_CONN_OK);
            wifi_connect = true;

            aiio_wifi_sta_connect_ind_stat_info_t wifi_ind_stat = { 0 };
            aiio_wifi_ip_params_t wifi_sta_ip_praram = { 0 };
            uint8_t mac[MAC_LEN];
            int32_t rssi = 0;

            aiio_wifi_rssi_get(&rssi);
            aiio_log_d("wifi cur_rssi = %d!!", rssi);

            wifi_info.rssi = rssi;

            aiio_wifi_sta_mac_get(mac);
            aiio_log_d("wifi mac = %02x%02x%02x%02x%02x%02x!!", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            aiio_wifi_sta_connect_ind_stat_get(&wifi_ind_stat);
            aiio_log_d("wifi ssid = %s \r\n", wifi_ind_stat.ssid);
            aiio_log_d("wifi passphr = %s \r\n", wifi_ind_stat.passphr);
            aiio_log_d("wifi sta_bssid = %02x%02x%02x%02x%02x%02x!!", wifi_ind_stat.bssid[0], wifi_ind_stat.bssid[1], wifi_ind_stat.bssid[2], wifi_ind_stat.bssid[3], wifi_ind_stat.bssid[4], wifi_ind_stat.bssid[5]);


            aiio_wifi_sta_ip_get(&wifi_sta_ip_praram.ip, &wifi_sta_ip_praram.gateway, &wifi_sta_ip_praram.netmask);
            aiio_log_d("wifi ip = 0x%08x \r\n", wifi_sta_ip_praram.ip);
            aiio_log_d("wifi gateway = 0x%08x \r\n", wifi_sta_ip_praram.gateway);
            aiio_log_d("wifi netmask = 0x%08x \r\n", wifi_sta_ip_praram.netmask);
            aiio_log_d("wifi ip = %s \r\n", ip4addr_ntoa(&wifi_sta_ip_praram.ip));
            aiio_log_d("wifi gateway = %s \r\n", ip4addr_ntoa(&wifi_sta_ip_praram.gateway));
            aiio_log_d("wifi netmask = %s \r\n", ip4addr_ntoa(&wifi_sta_ip_praram.netmask));

            wifi_info.ssid = malloc(strlen(wifi_ind_stat.ssid) + 1);
            if (wifi_info.ssid)
            {
                memset(wifi_info.ssid, 0, strlen(wifi_ind_stat.ssid) + 1);
                memcpy(wifi_info.ssid, wifi_ind_stat.ssid, strlen(wifi_ind_stat.ssid));
            }

            wifi_info.bssid = malloc(sizeof(wifi_ind_stat.bssid) * 2 + 1);
            if (wifi_info.bssid)
            {
                memset(wifi_info.bssid, 0, sizeof(wifi_ind_stat.bssid) * 2 + 1);
                snprintf(wifi_info.bssid, sizeof(wifi_ind_stat.bssid) * 2 + 1, "%02x%02x%02x%02x%02x%02x",
                        wifi_ind_stat.bssid[0], wifi_ind_stat.bssid[1], wifi_ind_stat.bssid[2], wifi_ind_stat.bssid[3], wifi_ind_stat.bssid[4], wifi_ind_stat.bssid[5]);
            }

            wifi_info.ip = malloc(strlen(ip4addr_ntoa(&wifi_sta_ip_praram.ip)) + 1);
            if (wifi_info.ip)
            {
                memset(wifi_info.ip, 0, strlen(ip4addr_ntoa(&wifi_sta_ip_praram.ip)) + 1);
                memcpy(wifi_info.ip, ip4addr_ntoa(&wifi_sta_ip_praram.ip), strlen(ip4addr_ntoa(&wifi_sta_ip_praram.ip)));
            }

            wifi_info.mask = malloc(strlen(ip4addr_ntoa(&wifi_sta_ip_praram.netmask)) + 1);
            if (wifi_info.mask)
            {
                memset(wifi_info.mask, 0, strlen(ip4addr_ntoa(&wifi_sta_ip_praram.netmask)) + 1);
                memcpy(wifi_info.mask, ip4addr_ntoa(&wifi_sta_ip_praram.netmask), strlen(ip4addr_ntoa(&wifi_sta_ip_praram.netmask)));
            }


            wifi_info.gw = malloc(strlen(ip4addr_ntoa(&wifi_sta_ip_praram.gateway)) + 1);
            if (wifi_info.gw)
            {
                memset(wifi_info.gw, 0, strlen(ip4addr_ntoa(&wifi_sta_ip_praram.gateway)) + 1);
                memcpy(wifi_info.gw, ip4addr_ntoa(&wifi_sta_ip_praram.gateway), strlen(ip4addr_ntoa(&wifi_sta_ip_praram.gateway)));
            }


            rev_queue.common_event = REV_CLOUD_CONNECTED_EVENT;
            if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            {
                aiio_log_i("queue send failed\r\n");
            }
            memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            wifi_disconnect_count = 0;
            wifi_start_connected = false;
        }
        break;

        case AIIO_WIFI_EVENT_AP_START:
        {
            aiio_log_i("<<<<<<<<< start soft ap OK<<<<<<<<<<<");
            // aiio_ap_config_notify(AIIO_WIFI_SOFT_AP_START_EVENT);
        }
        break;

        case AIIO_WIFI_EVENT_AP_STOP:
        {
            aiio_log_i("<<<<<<<<< stop soft ap OK<<<<<<<<<<<");
            // aiio_ap_config_notify(AIIO_WIFI_SOFT_AP_STOP_EVENT);
        }
        break;

        case AIIO_WIFI_EVENT_AP_STACONNECTED:
        {
            aiio_log_i("<<<<<<<<< station connent ap <<<<<<<<<<<");
        }
        break;

        case AIIO_WIFI_EVENT_AP_STADISCONNECTED:
        {
            aiio_log_i("<<<<<<<<< station disconnet ap <<<<<<<<<<<");
        }
        break;

        default:
            break;
    }
}

static void cloud_rev_cb(aiio_service_rev_event_t* rev_event)
{
    aiio_rev_queue_t rev_queue = { 0 };

    if (rev_event == NULL)
    {
        aiio_log_i("receive event is NULL \r\n");
        return;
    }

    if (rev_event->data)
    {
        aiio_log_i("receive data: %s \r\n", rev_event->data);
    }

    switch (rev_event->service_event)
    {
        case AIIO_SERVICE_LANCH_FAIL_EVNET:                                             /* mqtt connection fail*/
        {
            aiio_log_i("AIIO_SERVICE_LANCH_FAIL_EVNET \r\n");

            if (wifi_config_start)
            {
                if (ble_config_start)        aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);

                rev_queue.common_event = REV_CONFIG_FAIL_EVENT;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }
        }
        break;

        case AIIO_SERVICE_SUBSCRIBE_TIMEOUT:                                            /* The mqtt subscription is timeout*/
        {
            aiio_log_i("AIIO_SERVICE_SUBSCRIBE_TIMEOUT \r\n");
            if (wifi_config_start)
            {
                if (ble_config_start)        aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);

                rev_queue.common_event = REV_CONFIG_FAIL_EVENT;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }
        }
        break;

        case AIIO_SERVICE_MQTT_DISCONNECT:
        {
            aiio_log_i("AIIO_SERVICE_MQTT_DISCONNECT \r\n");
            if (wifi_config_start && !cloud_connect)
            {
                if (ble_config_start)        aiio_ble_config_response_status(AIIO_BLE_CODE_MQTT_CONN_ERR);

                rev_queue.common_event = REV_CONFIG_FAIL_EVENT;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }
            else
            {
                rev_queue.common_event = REV_CLOUD_DISCONNECTED;
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_i("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }
            aiio_log_i("AIIO_SERVICE_MQTT_DISCONNECT \r\n");
            cloud_connect = false;
        }
        break;

        case AIIO_SERVICE_MQTT_CONNECTING:
        {
            aiio_log_i("AIIO_SERVICE_MQTT_CONNECTING \r\n");
            if (ble_config_start)
            {
                aiio_ble_config_response_status(AIIO_BLE_CODE_MQTT_CONNING);
            }
        }
        break;

        case AIIO_SERVICE_MQTT_CONNECTED:
        {
            aiio_log_i("AIIO_SERVICE_MQTT_CONNECTED \r\n");

            aiio_ble_config_response_status(AIIO_BLE_CODE_MQTT_CONN_OK);

        }
        break;

        case AIIO_SERVICE_ACTIVITY_EVENT:                                                               /*When the device is in the distribution network, it should report the activation data to the cloud. The event is callback cause the device has sent the activation data to the cloud */
        {
            aiio_log_i("AIIO_SERVICE_ACTIVITY_EVENT \r\n");

            if (ble_config_start)
            {
                aiio_ble_config_response_status(AIIO_BLE_CODE_ACTIVITY_REQ);
            }

        }
        break;

        case AIIO_SERVICE_RESTORE_EVENT:
        {
            aiio_log_i("AIIO_SERVICE_RESTORE_EVENT \r\n");

            aiio_flash_clear_config_data();
            rev_queue.common_event = REV_CONFIG_START_EVENT;
            if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            {
                aiio_log_e("queue send failed\r\n");
            }
            memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            cloud_connect = false;
        }
        break;

        case AIIO_SERVICE_REBOOT_EVENT:
        {
            aiio_log_i("AIIO_SERVICE_REBOOT_EVENT \r\n");
            aiio_restart();
        }
        break;

        case AIIO_SERVICE_INFO_EVENT:                                                           /* The requestion command of device information come from cloud, the device needs to response device information to cloud*/
        {
            aiio_log_i("AIIO_SERVICE_INFO_EVENT \r\n");
        }
        break;

        case AIIO_SERVICE_CONFIG_EVENT:                                                             /* The configuration command come from cloud, the device needs to be configured for the configuration data*/
        {
            aiio_log_i("AIIO_SERVICE_CONFIG_EVENT \r\n");
        }
        break;

        case AIIO_SERVICE_UPGRADE_EVENT:                                                          /* The ota command come from cloud, device needs to execure ota function*/
        {
            aiio_log_i("AIIO_SERVICE_UPGRADE_EVENT \r\n");

        }
        break;

        case AIIO_SERVICE_QUERY_EVENT:                                                                      /* The query command of attribute status come from cloud, device needs to report all attribute status to cloud*/
        {
            aiio_log_i("AIIO_SERVICE_QUERY_EVENT \r\n");
            aiio_report_all_attibute_status(rev_event->msgMid, rev_event->from);
        }
        break;

        case AIIO_SERVICE_CONTROL_EVENT:
        {
            aiio_log_i("AIIO_SERVICE_CONTROL_EVENT \r\n");

            if (rev_event->data)
            {
                // aiio_parse_control_data(rev_event->msgMid, rev_event->from, rev_event->data);
                rev_queue.common_event = REV_CLOUD_CONTRL_DATA_EVENT;

                // memset(r2_device->mq_data, 0, 512);
                // memcpy(r2_device->mq_data, rev_event->data, rev_event->data_len);
                if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                {
                    aiio_log_e("queue send failed\r\n");
                }
                memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            }

        }
        break;

        case AIIO_SERVICE_UTC_EVENT:
        {
            aiio_log_i("AIIO_SERVICE_UTC_EVENT \r\n");
            if (rev_event->data)
            {
                aiio_log_i("receive data: %s \r\n", rev_event->data);
                aiio_online_update_local_time(rev_event->data, aiio_strlen(rev_event->data));
            }

            aiio_ble_config_response_status(AIIO_BLE_CODE_UTC_REQ);

        }
        break;

        case AIIO_SERVICE_ONLINE_EVENT:
        {
            aiio_log_i("AIIO_SERVICE_ONLINE_EVENT \r\n");

            if (ble_config_start)
            {
                aiio_ble_config_response_status(AIIO_BLE_CODE_ONLINE_REQ);
            }

            rev_queue.common_event = REV_CLOUD_ONLINE_EVENT;
            if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
            {
                aiio_log_i("queue send failed\r\n");
            }
            memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
            cloud_connect = true;
        }
        break;

        default:
            aiio_log_i("event id err \r\n");
            break;
    }
}
static void test_sta_wifi_entry(void* pvParameters)
{
    int msg_id;
    aiio_rev_queue_t  rev_queue = { 0 };

    while (1)
    {

        if (xQueueReceive(cloud_rev_queue_handle, &rev_queue, 0) == pdPASS)
        {
            switch (rev_queue.common_event)
            {
                case REV_CONFIG_DATA_EVENT:
                {
                    aiio_log_d("REV_CONFIG_DATA_EVENT \r\n");
                    // aiio_turn_on_led_status(GREEN_RED_LED_TURN_ON);
                    memcpy(&wifi_config_data, rev_queue.queue_data, rev_queue.queue_data_len);
                    aiio_log_d("ssid = %s \r\n", wifi_config_data.ssid);
                    aiio_log_d("passwd = %s \r\n", wifi_config_data.passwd);
                    aiio_log_d("mqttip = %s \r\n", wifi_config_data.mqttip);
                    aiio_log_d("token = %s \r\n", wifi_config_data.token);
                    aiio_log_d("time zone = %s \r\n", wifi_config_data.tz);
                    aiio_update_rtc_time_with_zone(wifi_config_data.tz, strlen(wifi_config_data.tz));
                    aiio_flash_save_wifi_config_data(&wifi_config_data);

                    rev_queue.common_event = REV_WIFI_CONNECTED_EVENT;
                    if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                    {
                        aiio_log_i("queue send failed\r\n");
                    }
                    memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                }
                break;

                case REV_CONFIG_START_EVENT:
                {
                    aiio_log_d("REV_CONFIG_START_EVENT \r\n");
                    if (wifi_connect)
                    {
                        aiio_user_service_deinit();
                        aiio_wifi_stop();
                    }
                    else if (wifi_start_connected)
                    {
                        aiio_wifi_stop();
                    }

                    aiio_os_tick_dealy(aiio_os_ms2tick(50));
                    aiio_ble_config_start();
                    ble_config_start = true;
                    // aiio_TimerStop(cloud_wait_connect_handle);

                    wifi_config_start = true;
                }
                break;

                case REV_CONFIG_STOP_EVENT:
                {
                    if (wifi_config_start)
                    {
                        aiio_log_d("REV_CONFIG_STOP_EVENT \r\n");

                        aiio_ble_config_stop();
                        ble_config_start = false;
                        wifi_config_start = false;

                        if (aiio_flash_get_wifi_config_data(&wifi_config_data) > 0)
                        {
                            aiio_log_d("ssid = %s \r\n", wifi_config_data.ssid);
                            aiio_log_d("passwd = %s \r\n", wifi_config_data.passwd);
                            iot_connect_wifi(wifi_config_data.ssid, wifi_config_data.passwd);
                        }

                    }
                }
                break;

                case REV_CONFIG_OK_EVENT:
                {
                    aiio_log_d("REV_CONFIG_OK_EVENT \r\n");
                    aiio_ble_config_response_status(AIIO_BLE_CODE_OK);
                    aiio_os_tick_dealy(aiio_os_ms2tick(2000));
                    aiio_ble_config_stop();
                    ble_config_start = false;

                    wifi_config_start = false;
                }
                break;

                case REV_BLE_DISCONNECTED_EVENT:
                {
                    aiio_log_d("REV_BLE_DISCONNECTED_EVENT \r\n");
                    if (ble_config_start)
                    {
                        aiio_ble_config_stop();
                        aiio_flash_clear_config_data();

                        rev_queue.common_event = REV_CONFIG_START_EVENT;
                        if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                        {
                            aiio_log_i("queue send failed\r\n");
                        }
                        memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                    }
                }
                break;

                case REV_CONFIG_FAIL_EVENT:
                {
                    aiio_log_d("REV_CONFIG_FAIL_EVENT \r\n");
                    aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);
                    aiio_os_tick_dealy(aiio_os_ms2tick(2000));
                    aiio_ble_config_stop();
                    aiio_flash_clear_config_data();
                    ble_config_start = false;
                    wifi_config_start = false;
                    aiio_log_i("enter ble pair \r\n");
                    rev_queue.common_event = REV_CONFIG_START_EVENT;
                    if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                    {
                        aiio_log_e("queue send failed\r\n");
                    }
                    memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                }
                break;

                case REV_WIFI_CONNECTED_EVENT:
                {
                    aiio_log_d("REV_WIFI_CONNECTED_EVENT \r\n");
                    if (wifi_connect || wifi_start_connected)
                    {
                        aiio_wifi_disconnect();
                    }
                    iot_connect_wifi(wifi_config_data.ssid, wifi_config_data.passwd);
                }
                break;

                case REV_CLOUD_CONNECTED_EVENT:
                {
                    aiio_log_d("REV_CLOUD_CONNECTED_EVENT \r\n");
                    aiio_cloud_receive_t cloud_data = { 0 };
                    cloud_data.mqtt_host = wifi_config_data.mqttip;
                    cloud_data.device_token = wifi_config_data.token;
                    cloud_data.mqtt_port = wifi_config_data.port;
                    cloud_data.wifi_info = &wifi_info;

                    if (!wifi_config_start)
                    {
                        aiio_user_service_init(true, &cloud_data, cloud_rev_cb);
                    }
                    else
                    {
                        aiio_user_service_init(false, &cloud_data, cloud_rev_cb);
                    }

                }
                break;

                case REV_CLOUD_ONLINE_EVENT:
                {
                    aiio_log_d("REV_CLOUD_ONLINE_EVENT \r\n");

                    if (wifi_config_start)
                    {
                        rev_queue.common_event = REV_CONFIG_OK_EVENT;
                        if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
                        {
                            aiio_log_i("queue send failed\r\n");
                        }
                        memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
                    }
                }
                break;

                case REV_CLOUD_DISCONNECTED:
                {
                    aiio_log_d("REV_CLOUD_DISCONNECTED \r\n");

                    aiio_user_service_deinit();
                }
                break;

                case REV_CLOUD_OTA_INFO_EVENT:
                {
                    aiio_log_d("REV_CLOUD_OTA_INFO_EVENT \r\n");

                }
                break;

                case REV_CLOUD_OTA_FAIL_EVENT:
                {
                    aiio_log_i("REV_CLOUD_OTA_FAIL_EVENT \r\n");

                }
                break;

                case REV_CLOUD_OTA_SUCCESS_EVENT:
                {
                    aiio_log_i("REV_CLOUD_OTA_SUCCESS_EVENT \r\n");
                }
                break;

                default:
                    aiio_log_e("can't find event[%d] \r\n", rev_queue.common_event);
                    break;
            }
        }

        aiio_os_tick_dealy(aiio_os_ms2tick(20));
    }

    aiio_os_thread_delete(aiio_test_sta_wifi_thread);
}

static void test_start_entry(void* pvParameters)
{
    aiio_rev_queue_t  rev_queue = { 0 };
    int32_t ret = 0;

    aiio_wifi_register_event_cb(cb_wifi_event);
    ret = aiio_wifi_init();
    if (ret!=0) {
        printf("wifi init error!!");
        return 0;
    }
    printf("Hello EasyLogger!");
    aiio_os_tick_dealy(aiio_os_ms2tick(1000));

    aiio_log_i("create wifi thread \r\n");
    aiio_os_thread_create(&aiio_test_sta_wifi_thread, "test_sta_wifi_entry", test_sta_wifi_entry, 4096, NULL, 15);

    aiio_os_tick_dealy(aiio_os_ms2tick(1000));
    aiio_log_i("enter ble pair \r\n");
    rev_queue.common_event = REV_CONFIG_START_EVENT;
    if (xQueueSendToBack(cloud_rev_queue_handle, &rev_queue, 100) != pdPASS)
    {
        aiio_log_e("queue send failed\r\n");
    }
    memset(&rev_queue, 0, sizeof(aiio_rev_queue_t));
    aiio_log_a("Hello EasyLogger!");
    aiio_log_e("Hello EasyLogger!");
    aiio_log_w("Hello EasyLogger!");
    aiio_log_i("Hello EasyLogger!");
    aiio_log_d("Hello EasyLogger!");
    aiio_log_v("Hello EasyLogger!");


    while (1)
    {
        aiio_log_i("rtos free heap size:%dk", aiio_os_get_free_heap_size() / 1024);
        aiio_os_tick_dealy(aiio_os_ms2tick(1000));
    }
}

static void rtc_task_start(void* arg)
{
    aiio_rtc_time_t* rtc_time = NULL;
    uint32_t count = 0;

    while (1)
    {
        aiio_rtc_loop();
        count++;
        if (count > 30)
        {
            rtc_time = aiio_get_rtc_time();
            aiio_log_i("the Date : %02d:%02d:%02d - %02d:%02d:%02d,wday %02d", rtc_time->year, rtc_time->mon, rtc_time->day, rtc_time->hour, rtc_time->minute, rtc_time->second, rtc_time->week);
            count = 0;

        }
        aiio_os_tick_dealy(aiio_os_ms2tick(100));
    }
}

int main(void)
{
    int32_t ret = 0;

    board_init();

    configASSERT((configMAX_PRIORITIES > 4));

    aiio_log_init();
    aiio_manufacturer_info();

    bflb_mtd_init();
    /* ble stack need easyflash kv */
    easyflash_init();

    /* Init rf */
    if (0 != rfparam_init(0, NULL, 0)) {
        printf("PHY RF init failed!\r\n");
        return 0;
    }

    aiio_rtc_time_init(rtc_task_start);
    printf("Hello EasyLogger!");
    cloud_rev_queue_handle = xQueueCreate(QUEUE_MAX_SIZE, sizeof(aiio_rev_queue_t));
    if (cloud_rev_queue_handle == NULL)
    {
        return 0;
    }
    printf("Hello EasyLogger!");
    ret = xTaskCreate(test_start_entry, "aiio_start_thread", 4096, NULL, 15, &aiio_start_thread);
    if (ret != pdPASS)
    {
        printf("create thread fail \r\n");
        return 0;
    }
    printf("ret = %d!", ret);

    vTaskStartScheduler();

    while (1) {
    }
}
