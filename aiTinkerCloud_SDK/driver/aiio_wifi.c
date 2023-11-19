/** @brief      Wifi application interface.
 *
 *  @file       aiio_wifi.c
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       Wifi application interface.
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/06/08      <td>1.0.0       <td>wusen       <td>wifi init
 *  </table>
 *
 */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "mem.h"

#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"

#include "bflb_irq.h"
#include "bflb_uart.h"

#include "bl616_glb.h"
#include "rfparam_adapter.h"

#include "board.h"

 // #include <fhost_connect.h>
 // #include <fhost_wpa.h>
 // #include "fhost_config.h"
 // #include "fhost_cntrl.h"


#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "bl_fw_api.h"

#include "platform_al.h"
#include "rtos_al.h"
// #include "net_al_ext.h"

#include "aiio_wifi.h"
#include "log.h"

#define WIFI_STACK_SIZE  (1536)
#define TASK_PRIORITY_FW (16)


//////////////
//add
#define NX_REMOTE_STA_MAX 4
#define MAC_ADDR_LEN 6
#define MAC_SSID_LEN 32

/// 5G lower bound freq
#define PHY_FREQ_5G 5000

enum mac_chan_band
{
    /// 2.4GHz Band
    PHY_BAND_2G4,
    /// 5GHz band
    PHY_BAND_5G,
    /// Number of bands
    PHY_BAND_MAX,
};
enum mac_chan_bandwidth
{
    /// 20MHz BW
    PHY_CHNL_BW_20,
    /// 40MHz BW
    PHY_CHNL_BW_40,
    /// 80MHz BW
    PHY_CHNL_BW_80,
    /// 160MHz BW
    PHY_CHNL_BW_160,
    /// 80+80MHz BW
    PHY_CHNL_BW_80P80,
    /// Reserved BW
    PHY_CHNL_BW_OTHER,
};

struct mac_chan_op
{
    /// Band (@ref mac_chan_band)
    uint8_t band;
    /// Channel type (@ref mac_chan_bandwidth)
    uint8_t type;
    /// Frequency for Primary 20MHz channel (in MHz)
    uint16_t prim20_freq;
    /// Frequency center of the contiguous channel or center of Primary 80+80 (in MHz)
    uint16_t center1_freq;
    /// Frequency center of the non-contiguous secondary 80+80 (in MHz)
    uint16_t center2_freq;
    /// Max transmit power allowed on this channel (dBm)
    int8_t tx_power;
    /// Additional information (@ref mac_chan_flags)
    uint8_t flags;
};

enum
{
    /// Primary radar detection chain (i.e for the operating channel)
    PHY_PRIM,
    /// Secondary radar detection chain
    PHY_SEC,
};

struct mac_addr
{
    /// Array of 16-bit words that make up the MAC address.
    uint16_t array[MAC_ADDR_LEN/2];
};

struct mac_hdr
{
    /// Frame control
    uint16_t fctl;
    /// Duration/ID
    uint16_t durid;
    /// Address 1
    struct mac_addr addr1;
    /// Address 2
    struct mac_addr addr2;
    /// Address 3
    struct mac_addr addr3;
    /// Sequence control
    uint16_t seq;
} __PACKED;

uint16_t phy_channel_to_freq(uint8_t band, int channel)
{
    if ((band == PHY_BAND_2G4) && (channel >= 1) && (channel <= 14))
    {
        if (channel == 14)
            return 2484;
        else
            return 2407 + channel * 5;
    }
    else if ((band == PHY_BAND_5G) && (channel >= 1) && (channel <= 177))
    {
        return PHY_FREQ_5G + channel * 5;
    }

    return 0;
}

//////////

static aiio_wifi_conf_t aiio_conf =
{
    .country_code = "CN",
};

#define IP_SET_ADDR(a, b, c, d) (((uint32_t)((a)&0xff)) |       \
                                 ((uint32_t)((b)&0xff) << 8) |  \
                                 ((uint32_t)((c)&0xff) << 16) | \
                                 ((uint32_t)((d)&0xff) << 24))

static uint8_t g_wifi_ap_interface = 0;
static uint8_t g_wifi_sta_interface = 0;

static uint8_t g_wifi_ap_init_flag = 0;
static uint8_t g_wifi_sta_init_flag = 0;

static aiio_wifi_ip_params_t ap_ip_params = { 0 };
static aiio_wifi_ip_params_t sta_ip_params = { 0 };

static TaskHandle_t wifi_fw_task;
aiio_event_cb wifi_cb;
wifi_mgmr_sniffer_item_t sniffer_item;

aiio_wifi_config_t wifi_config = { 0 };

static int32_t currentState = AIIO_WIFI_STATE_UNKNOWN;

static aiio_scan_complete_cb_t scan_cb;
static void* scan_data;

static aiio_sniffer_cb_t aiio_sniffer_cb;

// extern uint8_t g_sta_idx;

static aiio_os_thread_handle_t            wifi_config_task_handle;
static aiio_os_queue_handle_t             wifi_config_queue_handle = NULL;

typedef struct
{
    char* queue_data;
    uint16_t queue_data_len;
    uint32_t evt;
}aiio_wifi_config_queue_t;


static int32_t wifi_mgmr_scan_complete_callback()
{
    int32_t status = 0;

    if (scan_cb != NULL) {
        status = 1;
        scan_cb(scan_data, &status);
    }
    return AIIO_OK;
}

void wifi_event_handler(uint32_t code)
{
    aiio_wifi_config_queue_t wifi_config_queue_data = { 0 };
    wifi_config_queue_data.evt = code;
    switch (code)
    {
        case CODE_WIFI_ON_INIT_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE\r\n", __func__);
            wifi_mgmr_init(&aiio_conf);
            currentState = AIIO_WIFI_STATE_IDLE;
        } break;

    }
    if (wifi_config_queue_handle)
    {
        if (aiio_os_queue_send(wifi_config_queue_handle, &wifi_config_queue_data, 0) != AIIO_OK)
        {
            LOG_I("queue send failed\r\n");
        }
    }
}


static void aiio_wifi_config_task(void* pv)
{
    aiio_wifi_config_queue_t wifi_config_queue_data = { 0 };
    aiio_input_event_t aiio_event = { 0 };
    while (1)
    {
        if (aiio_os_queue_recv(wifi_config_queue_handle, &wifi_config_queue_data, portMAX_DELAY) != AIIO_OK)
        {
            continue;
        }
        switch (wifi_config_queue_data.evt)
        {
            case CODE_WIFI_ON_INIT_DONE: {
                // LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE\r\n");
                // wifi_mgmr_init(&aiio_conf);
                // currentState=AIIO_WIFI_STATE_IDLE;
            } break;
            case CODE_WIFI_ON_MGMR_DONE: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_MGMR_DONE\r\n");
                currentState = AIIO_WIFI_STATE_IDLE;
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_WIFI_READY;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_SCAN_DONE: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_SCAN_DONE\r\n");
                // wifi_mgmr_sta_scanlist();
                wifi_mgmr_scan_complete_callback();
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_SCAN_DONE;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_CONNECTING: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_CONNECTED\r\n");
                currentState = AIIO_WIFI_STATE_CONNECTING;
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_STA_CONNECTING;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_CONNECTED: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_CONNECTED\r\n");
                void mm_sec_keydump();
                mm_sec_keydump();
                if (!wifi_config.sta.use_dhcp) { //not use dhcp
                    currentState = AIIO_WIFI_STATE_CONNECTED_IP_GOT;
                }
                else {
                    currentState = AIIO_WIFI_STATE_CONNECTED_IP_GETTING;
                }
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_STA_CONNECTED;
                    wifi_cb(&aiio_event, 0);
                    if (!wifi_config.sta.use_dhcp) { //not use dhcp
                        aiio_event.code = AIIO_WIFI_EVENT_STA_GOT_IP;
                        wifi_cb(&aiio_event, 0);
                    }
                }
            } break;
            case CODE_WIFI_ON_GOT_IP: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_GOT_IP\r\n");
                LOG_I("[SYS] Memory left is %d Bytes\r\n", kfree_size());
                currentState = AIIO_WIFI_STATE_CONNECTED_IP_GOT;
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_STA_GOT_IP;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_DISCONNECT: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_DISCONNECT\r\n");
                currentState = AIIO_WIFI_STATE_DISCONNECT;
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_STA_DISCONNECTED;
                    aiio_event.value = wifi_mgmr_sta_info_status_code_get();;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_AP_STARTED: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_AP_STARTED\r\n");
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_AP_START;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_AP_STOPPED: {
                LOG_I("[APP] [EVT], CODE_WIFI_ON_AP_STOPPED\r\n");
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_AP_STOP;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_AP_STA_ADD: {
                LOG_I("[APP] [EVT] [AP] [ADD] %lld\r\n", xTaskGetTickCount());
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_AP_STACONNECTED;
                    aiio_event.value++;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            case CODE_WIFI_ON_AP_STA_DEL: {
                LOG_I("[APP] [EVT] [AP] [DEL] %lld\r\n", xTaskGetTickCount());
                if (wifi_cb)
                {
                    aiio_event.code = AIIO_WIFI_EVENT_AP_STADISCONNECTED;
                    aiio_event.value--;
                    wifi_cb(&aiio_event, 0);
                }
            } break;
            default: {
                currentState = AIIO_WIFI_STATE_UNKNOWN;
                LOG_I("[APP] [EVT] Unknown code %u \r\n", wifi_config_queue_data.evt);
            }
        }
    }
}

int32_t wifi_start_firmware_task(void)
{
    aiio_log_d("Starting wifi ...\r\n");

    /* enable wifi clock */

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    /* set ble controller EM Size */

    // GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

    // if (AIIO_OK != rfparam_init(0, NULL, 0)) {
    //     aiio_log_d("PHY RF init failed!\r\n");
    //     return AIIO_OK;
    // }

    // aiio_log_d("PHY RF init success!\r\n");

    /* Enable wifi irq */

    extern void interrupt0_handler(void);
    bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    bflb_irq_enable(WIFI_IRQn);

    xTaskCreate(wifi_main, (char*)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

    return AIIO_OK;
}

aiio_err_t aiio_wifi_init(void)
{

    /*wifi fw stack and thread stuff*/
    static uint8_t stack_wifi_init = 0;

    if (1 == stack_wifi_init) {
        aiio_log_e("Wi-Fi Stack Started already!!!");
        return AIIO_ERROR;
    }
    stack_wifi_init = 1;

    aiio_os_queue_create(&wifi_config_queue_handle, 5, sizeof(aiio_wifi_config_queue_t));
    if (!wifi_config_queue_handle)
    {
        aiio_log_e("create queue fail \r\n");
        return AIIO_ERROR;
    }
    aiio_log_i("create wifi config task \r\n");
    if (aiio_os_thread_create(&wifi_config_task_handle, "wifi_config_task", aiio_wifi_config_task, 4096, NULL, 5) != AIIO_OK)
    {
        aiio_log_e("create wifi config task fail \r\n");
        return AIIO_ERROR;
    }
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();

    return AIIO_OK;
}

aiio_err_t aiio_wifi_register_event_cb(aiio_event_cb cb)
{
    wifi_cb = cb;
    return AIIO_OK;
}

aiio_err_t aiio_wifi_unregister_event_cb(void)
{
    wifi_cb = NULL;
    return AIIO_OK;
}


aiio_err_t aiio_wifi_set_mode(aiio_wifi_mode_t mode)
{

    if (mode==AIIO_WIFI_MODE_STA) {
        g_wifi_sta_interface = 1;
    }
    else if (mode==AIIO_WIFI_MODE_AP) {
        g_wifi_ap_interface = 1;
    }
    else if (mode==AIIO_WIFI_MODE_APSTA) {
        g_wifi_ap_interface = 1;
        g_wifi_sta_interface = 1;
    }
    else {
        aiio_log_e("The mode dont support");
        g_wifi_ap_interface = 0;
        g_wifi_sta_interface = 0;
        g_wifi_sta_init_flag = 0;
        g_wifi_ap_init_flag = 0;
        return AIIO_ERROR;
    }
    return AIIO_OK;

}

aiio_err_t aiio_wifi_get_mode(aiio_wifi_mode_t* mode)
{
    if (mode==NULL) {
        aiio_log_e("The get mode is null error");
        return AIIO_ERROR;
    }

    if (g_wifi_ap_interface!=0 &&g_wifi_sta_interface==0) {
        *mode = AIIO_WIFI_MODE_AP;
    }
    else if (g_wifi_sta_interface!=0 && g_wifi_ap_interface==0) {
        *mode = AIIO_WIFI_MODE_STA;
    }
    else if (g_wifi_sta_interface!=0 && g_wifi_ap_interface!=0) {
        *mode = AIIO_WIFI_MODE_APSTA;
    }
    else {
        *mode = AIIO_WIFI_MODE_NULL;
    }
    return AIIO_OK;
}


aiio_err_t aiio_wifi_set_config(aiio_wifi_port_t port, aiio_wifi_config_t* conf)
{
    if (conf==NULL) {
        aiio_log_e("The conf is null error");
        return AIIO_ERROR;
    }

    if (port==AIIO_WIFI_IF_STA) {  //sta config
        if (conf->sta.ssid) {
            strncpy((char*)wifi_config.sta.ssid, (char*)conf->sta.ssid, SSID_LEN);
        }
        if (conf->sta.password) {
            strncpy((char*)wifi_config.sta.password, (char*)conf->sta.password, PASSWORD_LEN);
        }
        if (conf->sta.bssid) {
            strncpy((char*)wifi_config.sta.bssid, (char*)conf->sta.bssid, MAC_LEN);
        }
        if (conf->sta.band) {
            wifi_config.sta.band = conf->sta.band;
        }
        if (conf->sta.channel) {
            wifi_config.sta.channel = conf->sta.channel;
        }
        wifi_config.sta.use_dhcp = conf->sta.use_dhcp;
        wifi_config.sta.flags = conf->sta.flags;
    }
    else {   //ap config
        if (conf->ap.ssid) {
            strncpy((char*)wifi_config.ap.ssid, (char*)conf->ap.ssid, SSID_LEN);
        }
        if (conf->ap.password) {
            strncpy((char*)wifi_config.ap.password, (char*)conf->ap.password, PASSWORD_LEN);
        }
        if (conf->ap.max_connection) {
            wifi_config.ap.max_connection = conf->ap.max_connection;
        }
        else {
            wifi_config.ap.max_connection = 4;
        }
        if (conf->ap.channel) {
            wifi_config.ap.channel = conf->ap.channel;
        }
        else {
            wifi_config.ap.channel = 1;
        }
        if (conf->ap.ssid_hidden) {
            wifi_config.ap.ssid_hidden = conf->ap.ssid_hidden;
        }
        else {
            wifi_config.ap.ssid_hidden = 0;
        }
        if (conf->ap.ssid_len) {
            wifi_config.ap.ssid_len = conf->ap.ssid_len;
        }
        if (conf->ap.dhcp_params.lease_time) {
            wifi_config.ap.dhcp_params.lease_time = conf->ap.dhcp_params.lease_time;
        }
        else {
            wifi_config.ap.dhcp_params.lease_time = 120;
        }
        if (conf->ap.dhcp_params.start) {
            wifi_config.ap.dhcp_params.start = conf->ap.dhcp_params.start;
        }
        else {
            wifi_config.ap.dhcp_params.start = 2;
        }
        if (conf->ap.dhcp_params.end) {
            wifi_config.ap.dhcp_params.end = conf->ap.dhcp_params.end;
        }
        else {
            wifi_config.ap.dhcp_params.end = 100;
        }
        wifi_config.ap.use_dhcp = conf->ap.use_dhcp;
    }
    return AIIO_OK;
}

aiio_err_t aiio_wifi_get_config(aiio_wifi_port_t port, aiio_wifi_config_t* conf)
{
    if (conf==NULL) {
        aiio_log_e("The conf is null error");
        return AIIO_ERROR;
    }
    if (port==AIIO_WIFI_IF_STA) {  //sta config
        strncpy((char*)conf->sta.ssid, (char*)wifi_config.sta.ssid, SSID_LEN);
        strncpy((char*)conf->sta.password, (char*)wifi_config.sta.password, PASSWORD_LEN);
        strncpy((char*)conf->sta.bssid, (char*)wifi_config.sta.bssid, MAC_LEN);
        conf->sta.band = wifi_config.sta.band;
        conf->sta.channel = wifi_config.sta.channel;
        conf->sta.use_dhcp = wifi_config.sta.use_dhcp;
        conf->sta.flags = wifi_config.sta.flags;
    }
    else {   //ap config
        strncpy((char*)conf->ap.ssid, (char*)wifi_config.ap.ssid, SSID_LEN);
        strncpy((char*)conf->ap.password, (char*)wifi_config.ap.password, PASSWORD_LEN);
        conf->ap.max_connection = wifi_config.ap.max_connection;
        conf->ap.channel = wifi_config.ap.channel;
        conf->ap.ssid_hidden = wifi_config.ap.ssid_hidden;
        conf->ap.ssid_len = wifi_config.ap.ssid_len;
        conf->ap.use_dhcp = wifi_config.ap.use_dhcp;
        conf->ap.dhcp_params.lease_time = wifi_config.ap.dhcp_params.lease_time;
        conf->ap.dhcp_params.start = wifi_config.ap.dhcp_params.start;
        conf->ap.dhcp_params.end = wifi_config.ap.dhcp_params.end;

    }
    return AIIO_OK;
}

aiio_err_t aiio_wifi_connect(void)
{
    uint16_t freq = 0;
    if (g_wifi_sta_interface) {
        wifi_mgmr_sta_autoconnect_enable();
        freq = phy_channel_to_freq(PHY_BAND_2G4, wifi_config.sta.channel);
        return wifi_sta_connect((char*)wifi_config.sta.ssid, (char*)wifi_config.sta.password,
                                wifi_config.sta.bssid, NULL, 1, freq, freq, wifi_config.sta.use_dhcp);
    }
    return AIIO_ERROR;
}
aiio_err_t aiio_wifi_disconnect(void)
{
    if (g_wifi_sta_interface) {
        wifi_mgmr_sta_autoconnect_disable();
        wifi_sta_disconnect();
        g_wifi_sta_interface = 0;
        g_wifi_sta_init_flag = 0;
        return AIIO_OK;
    }
    return AIIO_ERROR;

}

aiio_err_t aiio_wifi_ap_sta_info_get(aiio_wifi_sta_basic_info_t* sta_info_p, uint8_t idx)
{
    return wifi_mgmr_ap_sta_info_get(sta_info_p, idx);
}

aiio_err_t aiio_wifi_ap_sta_list_get(uint8_t* sta_num, aiio_wifi_sta_basic_info_t* sta_info_p)
{
    uint8_t sta_cnt = 0;
    uint8_t i = 0;
    struct wifi_sta_basic_info sta_info;
    uint8_t counter = 0;

    if (!wifi_mgmr_ap_state_get()) {
        aiio_log_e("wifi AP is not enabled");
        return AIIO_ERROR;
    }

    memset(&sta_info, 0, sizeof(struct wifi_sta_basic_info));
    for (i = 0;i < NX_REMOTE_STA_MAX;i++) {
        wifi_mgmr_ap_sta_info_get(&sta_info, i);
        if (!sta_info.is_used || (sta_info.sta_idx == 0xef)) {
            continue;
        }
        (sta_info_p+counter)->sta_idx = sta_info.sta_idx;
        (sta_info_p+counter)->sta_mac[0] = sta_info.sta_mac[0];
        (sta_info_p+counter)->sta_mac[1] = sta_info.sta_mac[1];
        (sta_info_p+counter)->sta_mac[2] = sta_info.sta_mac[2];
        (sta_info_p+counter)->sta_mac[3] = sta_info.sta_mac[3];
        (sta_info_p+counter)->sta_mac[4] = sta_info.sta_mac[4];
        (sta_info_p+counter)->sta_mac[5] = sta_info.sta_mac[5];
        // (sta_info_p+counter)->aid=sta_info.aid;
        counter++;
    }
    *sta_num = counter;
    return AIIO_OK;
}

aiio_err_t aiio_wifi_ap_stop(void)
{
    int32_t i;
    struct wifi_sta_basic_info sta_info;
    uint8_t empty[6] = { 0 };
    if (g_wifi_ap_interface)
    {
        // wifi_mgmr_ap_sta_cnt_get(&sta_cnt);
        for (i = 0; i < NX_REMOTE_STA_MAX; i++)
        {
            wifi_mgmr_ap_sta_info_get(&sta_info, i);
            if (!sta_info.is_used || (sta_info.sta_idx == 0xef))
                continue;

            if (memcmp(sta_info.sta_mac, empty, MAC_LEN) == 0)
                wifi_mgmr_ap_sta_delete(i);
        }
        wifi_mgmr_ap_stop();
        aiio_os_tick_dealy(aiio_os_ms2tick(500));
        g_wifi_ap_interface = 0;
        g_wifi_ap_init_flag = 0;
    }
    return AIIO_OK;

}


aiio_err_t aiio_wifi_conn_result_get(uint16_t* status_code, uint16_t* reason_code)
{
    if (status_code == NULL || reason_code == NULL) {
        return AIIO_ERROR;
    }
    (*status_code) = wifiMgmr.wifi_mgmr_stat_info.status_code;
    (*reason_code) = wifiMgmr.wifi_mgmr_stat_info.reason_code;
    return AIIO_OK;
}

char* aiio_wifi_status_code_str(uint16_t status_code)
{
    extern char* wifi_mgmr_get_sm_status_code_str(uint16_t status_code);
    return wifi_mgmr_get_sm_status_code_str(status_code);
}


int32_t wifi_mgmr_status_code_get_internal(int32_t* s_code)
{
    (*s_code) = wifiMgmr.wifi_mgmr_stat_info.status_code;
    return AIIO_OK;
}

int32_t wifi_mgmr_detailed_state_get_internal(int32_t* state, int32_t* state_detailed)
{
    int32_t m_state;
    int32_t s_code = 0;

    m_state = currentState;
    wifi_mgmr_status_code_get_internal(&s_code);

    if (m_state == AIIO_WIFI_STATE_IDLE) {
        if (wifi_mgmr_ap_state_get()) {
            *state = AIIO_WIFI_STATE_WITH_AP_IDLE;
        }
        else {
            *state = AIIO_WIFI_STATE_IDLE;
            if (s_code == AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE) {
                *state_detailed = AIIO_WIFI_STATE_PSK_ERROR;
            }
            else if (s_code == AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL) {
                *state_detailed = AIIO_WIFI_STATE_NO_AP_FOUND;
            }
        }
    }
    else if (m_state == AIIO_WIFI_STATE_CONNECTING) {
        if (wifi_mgmr_ap_state_get()) {
            *state = AIIO_WIFI_STATE_WITH_AP_CONNECTING;
        }
        else {
            *state = AIIO_WIFI_STATE_CONNECTING;
            if (s_code == AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE) {
                *state_detailed = AIIO_WIFI_STATE_PSK_ERROR;
            }
            else if (s_code == AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL) {
                *state_detailed = AIIO_WIFI_STATE_NO_AP_FOUND;
            }
        }
    }
    else if (m_state == AIIO_WIFI_STATE_CONNECTED_IP_GETTING) {
        if (wifi_mgmr_ap_state_get()) {
            *state = AIIO_WIFI_STATE_WITH_AP_CONNECTED_IP_GETTING;
        }
        else {
            *state = AIIO_WIFI_STATE_CONNECTED_IP_GETTING;
        }
    }
    else if (m_state == AIIO_WIFI_STATE_CONNECTED_IP_GOT) {
        if (wifi_mgmr_ap_state_get()) {
            *state = AIIO_WIFI_STATE_WITH_AP_CONNECTED_IP_GOT;
        }
        else {
            *state = AIIO_WIFI_STATE_CONNECTED_IP_GOT;
        }
    }
    else if (m_state == AIIO_WIFI_STATE_DISCONNECT) {
        if (wifi_mgmr_ap_state_get()) {
            *state = AIIO_WIFI_STATE_WITH_AP_DISCONNECT;
        }
        else {
            *state = AIIO_WIFI_STATE_DISCONNECT;
            if (s_code == AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE) {
                *state_detailed = AIIO_WIFI_STATE_PSK_ERROR;
            }
            else if (s_code == AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL) {
                *state_detailed = AIIO_WIFI_STATE_NO_AP_FOUND;
            }
        }
    }
    else if (m_state == AIIO_WIFI_STATE_IFDOWN) {
        *state = AIIO_WIFI_STATE_IFDOWN;
    }
    else if (m_state == AIIO_WIFI_STATE_SNIFFER) {
        *state = AIIO_WIFI_STATE_SNIFFER;
    }
    else {
        *state = AIIO_WIFI_STATE_UNKNOWN;
    }

    return AIIO_OK;
}

aiio_err_t aiio_wifi_get_state(int32_t* state)
{
    return wifi_mgmr_detailed_state_get_internal(state, state);
}

aiio_err_t aiio_wifi_start(void)
{
    aiio_err_t ret = AIIO_ERROR;
    wifi_mgmr_ap_params_t config;

    if (g_wifi_ap_interface && g_wifi_ap_init_flag ==0) {
        wifi_mgmr_conf_max_sta(wifi_config.ap.max_connection);

        memset(&config, 0, sizeof(config));
        config.ssid = wifi_config.ap.ssid;
        config.key = wifi_config.ap.password;
        config.channel = wifi_config.ap.channel;
        config.use_dhcpd = wifi_config.ap.use_dhcp;

        if (wifi_config.ap.use_dhcp) {
            config.ap_ipaddr = ap_ip_params.ip;
            config.ap_mask = inet_addr("255.255.255.0");
            config.start = wifi_config.ap.dhcp_params.start;
            config.limit = (wifi_config.ap.dhcp_params.end - wifi_config.ap.dhcp_params.start)+1;
        }

        wifi_mgmr_ap_start(&config);
        g_wifi_ap_init_flag = 1;
    }
    if (g_wifi_sta_interface && g_wifi_sta_init_flag ==0) {
        ret = aiio_wifi_connect();
        g_wifi_sta_init_flag = 1;
    }
    return ret;

}

aiio_err_t aiio_wifi_stop(void)
{
    if (g_wifi_ap_interface) {
        aiio_wifi_ap_stop();
    }
    if (g_wifi_sta_interface) {
        aiio_wifi_disconnect();
    }
    return AIIO_OK;
}

static int32_t wifi_mgmr_scan_filter_hidden_ssid(int32_t filter)
{
    portENTER_CRITICAL();
    if (filter) {
        wifiMgmr.features &= (~WIFI_MGMR_FEATURES_SCAN_SAVE_HIDDEN_SSID);
    }
    else {
        wifiMgmr.features |= WIFI_MGMR_FEATURES_SCAN_SAVE_HIDDEN_SSID;
    }
    portEXIT_CRITICAL();
    return AIIO_OK;
}

aiio_err_t aiio_wifi_scan(void* data, aiio_scan_complete_cb_t cb, bool filter)
{
    wifi_mgmr_scan_filter_hidden_ssid(filter);

    scan_cb = cb;
    scan_data = data;

    wifi_mgmr_scan_params_t config;
    memset(&config, 0, sizeof(wifi_mgmr_scan_params_t));

    return wifi_mgmr_sta_scan(&config);
}

aiio_err_t aiio_wifi_scan_adv(void* data, aiio_scan_complete_cb_t cb, bool filter, aiio_wifi_scan_params_t* scan_params)
{
    int32_t i;
    uint8_t bssid[MAC_ADDR_LEN] = { 0 };
    char ssid[MAC_SSID_LEN] = { 0 };

    uint16_t channels[CONFIG_MAX_FIXED_CHANNELS_LIMIT];

    wifi_mgmr_scan_filter_hidden_ssid(filter);

    scan_cb = cb;
    scan_data = data;

    wifi_mgmr_scan_params_t config;
    memset(&config, 0, sizeof(wifi_mgmr_scan_params_t));
    if (scan_params->channel_num) {
        for (i = 0; i < scan_params->channel_num; i++) {
            channels[i] = scan_params->channels[i];
        }
        config.channels_cnt = scan_params->channel_num;
        memcpy(config.channels, channels, CONFIG_MAX_FIXED_CHANNELS_LIMIT);
    }

    if (memcmp(bssid, scan_params->bssid, MAC_ADDR_LEN)!=AIIO_OK) {
        config.bssid_set_flag = 1;
        memcpy(config.bssid, scan_params->bssid, MAC_ADDR_LEN);
    }

    if (memcmp(ssid, scan_params->ssid, MAC_SSID_LEN)!=AIIO_OK) {
        memcpy(config.ssid_array, scan_params->ssid, MAC_SSID_LEN);
        config.ssid_length = strlen(scan_params->ssid);
    }

    if (scan_params->duration_scan) {
        config.duration = ((scan_params->duration_scan)/1000);
    }
    return wifi_mgmr_sta_scan(&config);
}

static inline int wifi_mgmr_scan_item_is_timeout(wifi_mgmr_t* mgmr, wifi_mgmr_scan_item_t* item)
{
    return ((unsigned int)rtos_now(0) - (unsigned int)item->timestamp_lastseen) >= mgmr->scan_item_timeout ? 1 : 0;
}

aiio_err_t aiio_wifi_scan_ap_all(aiio_wifi_ap_item_t* env, uint32_t* param1, aiio_scan_item_cb_t cb)
{
    int32_t i = 0;
    wifi_mgmr_scan_item_t* scan;
    aiio_wifi_ap_item_t item;
    for (i = 0; i < sizeof(wifiMgmr.scan_items) / sizeof(wifiMgmr.scan_items[0]); i++) {
        scan = &wifiMgmr.scan_items[i];
        if (wifiMgmr.scan_items[i].is_used && (!wifi_mgmr_scan_item_is_timeout(&wifiMgmr, &wifiMgmr.scan_items[i]))) {
            memcpy(item.ssid, scan->ssid, sizeof(item.ssid));
            item.ssid_tail[0] = '\0';
            item.ssid_len = strlen(item.ssid);
            memcpy(item.bssid, scan->bssid, sizeof(item.bssid));
            item.channel = scan->channel;
            item.rssi = scan->rssi;
            item.auth = scan->auth;
            item.cipher = scan->cipher;
            cb(env, param1, &item);
        }
    }

    return AIIO_OK;
}


aiio_err_t aiio_wifi_get_wifi_channel_conf(aiio_wifi_conf_t* wifi_chan_conf)
{
    strncpy(wifi_chan_conf->country_code, wifiMgmr.country_code, sizeof(wifiMgmr.country_code));
    wifi_chan_conf->channel_nums = wifiMgmr.channel_nums;
    return AIIO_OK;
}


aiio_err_t aiio_wifi_sta_mac_set(uint8_t* mac)
{
    memcpy(wifiMgmr.wlan_sta.mac, mac, 6);
    wifiMgmr.wlan_sta.set = 1;
    return AIIO_OK;
}

aiio_err_t aiio_wifi_sta_mac_get(uint8_t* mac)
{
    return wifi_mgmr_sta_mac_get(mac);
}

aiio_err_t aiio_wifi_ap_mac_set(uint8_t* mac)
{
    memcpy(wifiMgmr.wlan_ap.mac, mac, 6);
    wifiMgmr.wlan_ap.set = 1;
    return AIIO_OK;
}

aiio_err_t aiio_wifi_ap_mac_get(uint8_t* mac)
{
    return wifi_mgmr_ap_mac_get(mac);
}

aiio_err_t aiio_wifi_sta_ip_get(uint32_t* ip, uint32_t* gw, uint32_t* mask)
{
    uint32_t* dns;
    if (wifi_mgmr_sta_state_get())
    {
        return wifi_sta_ip4_addr_get(ip, mask, gw, dns);
    }
    else
    {
        *ip = 0;
        *gw = 0;
        *mask = 0;
    }
    return AIIO_OK;
}

static int8_t IpStringToByte(char* ip, uint8_t* container) {
    if (container==NULL || ip==NULL) {
        return AIIO_ERROR;
    }
    int32_t tmp[4] = { 0 };
    if (sscanf(ip, "%d.%d.%d.%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3])==4) {
        for (uint8_t i = 0; i < 4; i++) {
            container[i] = (uint8_t)tmp[i];
        }
    }
    else {
        return AIIO_ERROR;
    }
    return AIIO_OK;
}

// aiio_err_t aiio_wifi_sta_ip_parameter_set(uint32_t ip, uint32_t mask, uint32_t gw)
// {
//     struct net_al_ext_ip_addr_cfg ip_cfg;
//     ip_cfg.mode = IP_ADDR_STATIC_IPV4;
//     ip_cfg.default_output = true;
//     ip_cfg.dhcp.to_ms = 15000;
//     ip_cfg.ipv4.addr = ip;
//     ip_cfg.ipv4.mask = mask;
//     ip_cfg.ipv4.gw = gw;
//     return net_al_ext_set_vif_ip(MGMR_VIF_STA, &ip_cfg);
// }

// aiio_err_t aiio_wifi_sta_ip_set(char* sta_ip)
// {
//     uint8_t ipArray[4]={0};
//     uint8_t netmaskArray[4]={255,255,255,0};
//     int8_t i_ret=IpStringToByte(sta_ip ,ipArray);
//     if(AIIO_OK!=i_ret){
//         aiio_log_e("static IP:%s is error\r\n",sta_ip);
//         return AIIO_ERROR;
//     }
//     uint32_t ip = (uint32_t)ipArray[0] | (uint32_t)ipArray[1] << 8 |
//             (uint32_t)ipArray[2] << 16 | (uint32_t)ipArray[3] << 24;

//     uint32_t netmask = (uint32_t)netmaskArray[0] | (uint32_t)netmaskArray[1] << 8 |
//             (uint32_t)netmaskArray[2] << 16 | (uint32_t)netmaskArray[3] << 24;

//     uint32_t gateway = (uint32_t)ipArray[0] | (uint32_t)ipArray[1] << 8 |
//             (uint32_t)ipArray[2] << 16 | (uint32_t)1 << 24;

//     struct net_al_ext_ip_addr_cfg ip_cfg;
//     ip_cfg.mode = IP_ADDR_STATIC_IPV4;
//     ip_cfg.default_output = true;
//     ip_cfg.dhcp.to_ms = 15000;
//     ip_cfg.ipv4.addr = ip;
//     ip_cfg.ipv4.mask = netmask;
//     ip_cfg.ipv4.gw = gateway;

//     return net_al_ext_set_vif_ip(MGMR_VIF_STA, &ip_cfg);
// }

// aiio_err_t aiio_wifi_sta_ip_unset(void)
// {   
//     struct net_al_ext_ip_addr_cfg ip_cfg;
//     ip_cfg.mode = IP_ADDR_STATIC_IPV4;
//     ip_cfg.default_output = true;
//     ip_cfg.dhcp.to_ms = 15000;
//     ip_cfg.ipv4.addr = 0;
//     ip_cfg.ipv4.mask = 0;
//     ip_cfg.ipv4.gw = 0;
//     return net_al_ext_set_vif_ip(MGMR_VIF_STA, &ip_cfg);
// }

// aiio_err_t aiio_wifi_ap_ip_get(uint32_t *ip, uint32_t *gw, uint32_t *mask)
// {
//     uint8_t vif_idx = MGMR_VIF_AP;
//     struct net_al_ext_ip_addr_cfg ip_cfg;

//     if (fhost_env.vif[vif_idx].mac_vif &&
//         (mac_vif_get_type(fhost_env.vif[vif_idx].mac_vif) == VIF_AP) &&
//         mac_vif_get_active(fhost_env.vif[vif_idx].mac_vif)) {
//         if (net_al_ext_get_vif_ip(vif_idx, &ip_cfg)) {
//             return AIIO_ERROR;
//         }
//         if (ip) {
//             *ip = ip_cfg.ipv4.addr;
//         }
//         if (mask) {
//             *mask = ip_cfg.ipv4.mask;
//         }
//         if (gw) {
//             *gw = ip_cfg.ipv4.gw;
//         }
//         // if (dns) {
//         //     *dns = ip_cfg.ipv4.dns;
//         // }
//         return AIIO_OK;
//     }
// }

// aiio_err_t aiio_wifi_ap_ip_set(char* ap_ip)
// {

//     uint8_t ip[4] = { 0 }, digit= 0;
//     char* temp_arg = (char*)calloc(1, 6);
//     int32_t ret = aiio_wifi_ap_stop();
//     memset(ip, 0, sizeof(ip));
//     memset(&ap_ip_params, 0, sizeof(ap_ip_params));
//     memset(temp_arg, 0, sizeof(temp_arg));

//     for (int32_t i = 0; i < 4; i++)
//     {
//         temp_arg = ap_ip;
//         while (*(ap_ip++) != '.')
//         {
//             digit++;
//         }
//         temp_arg[digit] = '\0';
//         ip[i] = atoi(temp_arg);
//         digit = 0;
//     }
//     if (!ret)
//     {
//         ap_ip_params.ip = IP_SET_ADDR(ip[0], ip[1], ip[2], ip[3]);
//         ap_ip_params.gateway = IP_SET_ADDR(ip[0], ip[1], ip[2], 1);
//         ap_ip_params.netmask = IP_SET_ADDR(255, 255, 255, 0);
//     }
//     struct net_al_ext_ip_addr_cfg ip_cfg;
//     ip_cfg.mode = IP_ADDR_STATIC_IPV4;
//     ip_cfg.default_output = true;
//     ip_cfg.dhcp.to_ms = 15000;
//     ip_cfg.ipv4.addr = ap_ip_params.ip;
//     ip_cfg.ipv4.mask = ap_ip_params.netmask;
//     ip_cfg.ipv4.gw = ap_ip_params.gateway;
//     return net_al_ext_set_vif_ip(MGMR_VIF_AP, &ip_cfg);
// }


aiio_err_t aiio_wifi_set_country_code(char* country_code)
{
    strncpy(aiio_conf.country_code, country_code, sizeof(aiio_conf.country_code));
    aiio_conf.country_code[2] = '\0';
    wifi_mgmr_set_country_code(country_code);
    return AIIO_OK;
}

aiio_err_t aiio_wifi_get_country_code(char* country_code)
{
    strcpy(country_code, aiio_conf.country_code);
    return AIIO_OK;
}

aiio_err_t aiio_wifi_channel_get(int32_t* channel)
{
    return wifi_mgmr_sta_channel_get(channel);
}
aiio_err_t aiio_wifi_channel_set(int32_t channel)
{
    struct mac_chan_op chan;
    if (channel >= 1 && channel<CONFIG_MAX_FIXED_CHANNELS_LIMIT) {
        // Start the PHY on the default channel
        chan.band = PHY_BAND_2G4;
        chan.type = PHY_CHNL_BW_20;
        chan.prim20_freq = phy_channel_to_freq(PHY_BAND_2G4, channel);
        chan.center1_freq = phy_channel_to_freq(PHY_BAND_2G4, channel);
        chan.center2_freq = 0;
        chan.flags = 0;
        phy_set_channel(&chan, PHY_PRIM/*0*/);
        return AIIO_OK;
    }
    return AIIO_ERROR;
}

aiio_err_t aiio_wifi_rssi_get(int32_t* rssi)
{
    return wifi_mgmr_sta_rssi_get(rssi);
}


aiio_err_t aiio_wifi_sta_connect_ind_stat_get(aiio_wifi_sta_connect_ind_stat_info_t* wifi_ind_stat)
{
    wifi_mgmr_connect_ind_stat_info_t  cur_wifi_ind_stat;
    wifi_mgmr_sta_connect_ind_stat_get(&cur_wifi_ind_stat);

    int32_t ssid_len = strlen(cur_wifi_ind_stat.ssid);
    if (ssid_len > 0) {
        memcpy(wifi_ind_stat->ssid, cur_wifi_ind_stat.ssid, ssid_len);
        wifi_ind_stat->ssid[ssid_len] = '\0';
    }

    int32_t passphr_len = strlen(cur_wifi_ind_stat.passphr);
    if (passphr_len > 0) {
        memcpy(wifi_ind_stat->passphr, cur_wifi_ind_stat.passphr, passphr_len);
        wifi_ind_stat->passphr[passphr_len] = '\0';
    }
    memcpy(wifi_ind_stat->bssid, cur_wifi_ind_stat.bssid, sizeof(cur_wifi_ind_stat.bssid));
    // wifi_ind_stat->aid = cur_wifi_ind_stat.aid;
    wifi_ind_stat->chan_id = cur_wifi_ind_stat.channel;
    wifi_ind_stat->security = cur_wifi_ind_stat.security;

    return AIIO_OK;
}


static void cb_sniffer(struct bl_frame_info* info, void* arg)
{
    if (info->payload == NULL)
    {
        // TRACE_APP(INF, "Unsupported frame: length = %d", info->length)
        aiio_log_e("Unsupported frame: length = %dr\r\n", info->length);
    }
    else
    {
        // struct mac_hdr *hdr __MAYBE_UNUSED = (struct mac_hdr *)info->payload;
        // // TRACE_APP(INF, "%pM %pM %pM %fc SN:%d length = %d", TR_MAC(hdr->addr1.array),
        // //           TR_MAC(hdr->addr2.array), TR_MAC(hdr->addr3.array), hdr->fctl, hdr->seq >> 4,
        // //           info->length);
        // // printf("SN:%d length = %d\r\n", hdr->seq >> 4, info->length);
        // if(aiio_sniffer_cb){
        //     aiio_sniffer_cb(NULL,hdr,info->length,NULL);
        // }
    }
}


aiio_err_t aiio_wifi_sniffer_enable(void)
{
    sniffer_item.itf = "wl1";
    sniffer_item.type = 20;
    sniffer_item.prim20_freq = 2437;

    sniffer_item.cb = cb_sniffer;
    sniffer_item.cb_arg = NULL;
    return wifi_mgmr_sniffer_enable(sniffer_item);
}

aiio_err_t aiio_wifi_sniffer_disable(void)
{
    sniffer_item.itf = "wl1";
    return wifi_mgmr_sniffer_disable(sniffer_item);

}
aiio_err_t aiio_wifi_sniffer_register(void* env, aiio_sniffer_cb_t cb)
{
    aiio_sniffer_cb = cb;
    return AIIO_OK;
}
aiio_err_t aiio_wifi_sniffer_unregister(void* env)
{
    aiio_sniffer_cb = NULL;
    return AIIO_OK;
}


aiio_wifi_beacon_auth_t aiio_wifi_get_sta_security(void)
{
    if (wifi_mgmr_sta_state_get()) {//CONNECT
        uint8_t* bssid = wifiMgmr.wifi_mgmr_stat_info.bssid;
        for (int32_t i = 0; i < sizeof(wifiMgmr.scan_items)/sizeof(wifiMgmr.scan_items[0]); i++) {
            if (memcmp(bssid, wifiMgmr.scan_items[i].bssid, 6) == 0) {
                return wifiMgmr.scan_items[i].auth;
            }
        }
    }
    return AIIO_WIFI_EVENT_BEACON_IND_AUTH_UNKNOWN;
}

