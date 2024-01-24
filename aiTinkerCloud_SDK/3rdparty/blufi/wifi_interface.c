
#include "wifi_interface.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>

// #include <bl_wifi.h>
// #include <hal_wifi.h>
#include <wifi_mgmr_ext.h>
#include <bl_fw_api.h>
// #include <aos/yloop.h>
// #include <aos/kernel.h>

#define DHCP_MAX_HLEN       6
#define AT_WIFI_MAX_STA_NUM 10

blufi_wifi_conn_event_cb_t sg_blufi_conn_cb = NULL;

// struct dhcp_client_node
// {
//     struct dhcp_client_node *next;
//     uint8_t chaddr[DHCP_MAX_HLEN];
//     ip4_addr_t ipaddr;
//     uint32_t lease_end;
// };
struct wifi_ap_sta_info
{
    long long valid_time;
    uint8_t mac[6];
    uint32_t ip;
    uint8_t sta_index;
};

/* if not 0, means wifi init down */
int g_wifi_mgmr_done;
static char _wifi_pmk[68];
static struct wifi_ap_sta_info g_wifi_ap_sta_info[AT_WIFI_MAX_STA_NUM];
// static wifi_interface_t wifi_sta_interface;
static wifi_conf_t conf =
{
        .country_code = "CN",
};

void blufi_wifi_evt_export(int evt)
{
    if (sg_blufi_conn_cb)
    {
        sg_blufi_conn_cb(evt, NULL);
    }
}

int wifi_ap_get_sta_ip(uint8_t mac[6], uint32_t* ip)
{
    int i;

    for (i = 0; i < AT_WIFI_MAX_STA_NUM; i++) {
        if (g_wifi_ap_sta_info[i].valid_time != 0 && memcmp(g_wifi_ap_sta_info[i].mac, mac, 6) == 0) {
            *ip = g_wifi_ap_sta_info[i].ip;
            return 0;
        }
    }
    return -1;
}

int wifi_conn_ap_info_get(sta_config_t* sta)
{
    wifi_mgmr_connect_ind_stat_info_t wifi_info;
    wifi_mgmr_sta_connect_ind_stat_get(&wifi_info);
    strcpy(sta->cwjap_param.ssid, wifi_info.ssid);
    memcpy(sta->cwjap_param.bssid, wifi_info.bssid, 6);


    return 0;
}

int axk_hal_conn_ap_info_set(cwjap_param_t* cwjap_param)
{

    if (strlen(cwjap_param->pwd) != 0)
    {
        wifi_mgmr_psk_cal(cwjap_param->pwd, cwjap_param->ssid, strlen(cwjap_param->ssid), _wifi_pmk);
    }

    wifi_sta_disconnect();

    // wifi_mgmr_sta_connect(wifi_sta_interface, cwjap_param->ssid, cwjap_param->pwd, NULL, NULL,0, 0);
    wifi_sta_connect(cwjap_param->ssid, cwjap_param->pwd, NULL, NULL, 0, 0, 0, 1);

    return 0;
}


int axk_hal_disconn_ap(void)
{
    wifi_sta_disconnect();

    return 0;
}


void wifi_interface_init(blufi_wifi_conn_event_cb_t cb)
{
    // aos_register_event_filter(EV_WIFI, event_cb_wifi_event, NULL);

    // wifi_start_firmware_task();
    /*Trigger to start Wi-Fi*/
    // aos_post_event(EV_WIFI, CODE_WIFI_ON_INIT_DONE, 0);

    sg_blufi_conn_cb = cb;
}
