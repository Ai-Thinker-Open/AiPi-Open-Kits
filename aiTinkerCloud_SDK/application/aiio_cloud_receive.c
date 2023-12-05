/**
 * @brief   Define some interface about cloud, such as Lauch function of cloud or some callback function
 *
 * @file    aiio_cloud_receive.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly desctrbing cloud interface
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The interface about cloud
 */
#include "aiio_common.h"
#include "aiio_cloud_receive.h"
#include "aiio_wan_service.h"
#include "aiio_ble_config.h"
#include "aiio_system.h"
#include "aiio_cloud_control.h"
#include "config.h"
#include "aiio_stdio.h"
 // #include "aiio_ota_transmit.h"

#define         DEVICE_TYPE                 "bl618"                   /*!< 设置该固件运行于哪个芯片平台上 */

static bool service_init_state = false;


/* The callback function of receive mqtt data*/

void aiio_user_service_init(bool activity, aiio_cloud_receive_t* cloud_data, aiio_cloid_receive_cb reveive_cb)
{
    uint8_t mac[6];
    char mac_str[20] = { 0 };
    aiio_service_config_t  service_config = { 0 };

    if (service_init_state)
    {
        aiio_log_e("service already init \r\n");
        return;
    }

    aiio_wifi_sta_mac_get((uint8_t*)mac);
    aiio_log_i("wifi mac = %02x%02x%02x%02x%02x%02x!!", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    snprintf(mac_str, 20, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    aiio_log_i("mac_str: %s\r\n", mac_str);

    service_config.service_data.deviceid = DEVICE_CLIENT_ID;
    service_config.service_data.productid = PRODUCT_ID;
    service_config.service_data.user_name = DEVICE_USER_NAME;
    service_config.service_data.passwd = DEVICE_PASSWD;
    service_config.dev_info.fwVer = DEVICE_FW_VERSION;
    // service_config.dev_info.mcuVer = mcuVer;
    service_config.dev_info.devType = DEVICE_TYPE;
    service_config.dev_info.mac = mac_str;

    memcpy(&service_config.wifi_info, cloud_data->wifi_info, sizeof(aiio_wifi_info_t));
    service_config.service_netif.host = cloud_data->mqtt_host;
    service_config.service_netif.port = cloud_data->mqtt_port;
    service_config.service_netif.keepalive = 50;
    service_config.service_netif.transport = MQTT_TRANSPORT_OVER_SSL;
    service_config.activity = activity;
    service_config.token = cloud_data->device_token;
    service_config.rev_service_data_cb = reveive_cb;

    aiio_service_init(&service_config);
    service_init_state = true;
}



void aiio_user_service_deinit(void)
{
    if (!service_init_state)
    {
        aiio_log_e("service already deinit \r\n");
        return;
    }
    aiio_service_deinit();
    service_init_state = false;
}

