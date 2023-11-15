/** @brief      Wifi application interface.
 *
 *  @file       aiio_wifi.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       Wifi application interface.
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/01/30      <td>1.0.0       <td>chenzf      <td>Define file
 *  <tr><td>2023/02/06      <td>1.0.1       <td>wusen       <td>first version
 *  <tr><td>2023/02/08      <td>1.0.2       <td>wusen       <td>fix bug & add function
 *  <tr><td>2023/02/10      <td>1.0.3       <td>wusen       <td>add wifi register/unregister cb function & fix
 *  <tr><td>2023/03/03      <td>1.0.4       <td>wusen       <td>add func & define
 *  <tr><td>2023/03/09      <td>1.0.5       <td>wusen       <td>add func & define
 *  <tr><td>2023/03/10      <td>1.0.6       <td>wusen       <td>add sniffer mode interface
 *  </table>
 *
 */

#ifndef __AIIO_WIFI_H__
#define __AIIO_WIFI_H__

#include "aiio_type.h"
#include "stdio.h"
#include "stdlib.h"
#include "aiio_chip_spec.h"
#include "aiio_os_port.h"
#include "aiio_error.h"
#include "aiio_log.h"
// #include "aiio_autoconf.h"
#define SSID_LEN (33)
#define PASSWORD_LEN (66)
#define PMK_LEN (65)
#define MAC_LEN (6)
#define POWER_RATE_TABLE_LEN (38)


#define CONFIG_AXK_WIFI_STA_DISCONNECT_DELAY  (1000)
#define CONFIG_MAX_FIXED_CHANNELS_LIMIT     (14)

#define CONFIG_MAX_STA_CONN_LIMIT     (10)

//bandwidth
#define WIFI_AP_DATA_RATE_1Mbps      (0x00)
#define WIFI_AP_DATA_RATE_2Mbps      (0x01)
#define WIFI_AP_DATA_RATE_5_5Mbps    (0x02)
#define WIFI_AP_DATA_RATE_11Mbps     (0x03)
#define WIFI_AP_DATA_RATE_6Mbps      (0x0b)
#define WIFI_AP_DATA_RATE_9Mbps      (0x0f)
#define WIFI_AP_DATA_RATE_12Mbps     (0x0a)
#define WIFI_AP_DATA_RATE_18Mbps     (0x0e)
#define WIFI_AP_DATA_RATE_24Mbps     (0x09)
#define WIFI_AP_DATA_RATE_36Mbps     (0x0d)
#define WIFI_AP_DATA_RATE_48Mbps     (0x08)
#define WIFI_AP_DATA_RATE_54Mbps     (0x0c)

//connect error code
#define AIIO_WLAN_FW_SUCCESSFUL                                        (0)
#define AIIO_WLAN_FW_TX_AUTH_FRAME_ALLOCATE_FAIILURE                   (1)
#define AIIO_WLAN_FW_AUTHENTICATION_FAIILURE                           (2)
#define AIIO_WLAN_FW_AUTH_ALGO_FAIILURE                                (3)
#define AIIO_WLAN_FW_TX_ASSOC_FRAME_ALLOCATE_FAIILURE                  (4)
#define AIIO_WLAN_FW_ASSOCIATE_FAIILURE                                (5)
#define AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_NOT_CONNECTION                  (6)
#define AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_CONNECTION                      (7)
#define AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE          (8)
#define AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_TRANSMIT_FAILURE   (9)
#define AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_ALLOCATE_FAIILURE (10)
#define AIIO_WLAN_FW_AUTH_OR_ASSOC_RESPONSE_TIMEOUT_FAILURE           (11)
#define AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL                        (12)
#define AIIO_WLAN_FW_CREATE_CHANNEL_CTX_FAILURE_WHEN_JOIN_NETWORK     (13)
#define AIIO_WLAN_FW_JOIN_NETWORK_FAILURE                             (14)
#define AIIO_WLAN_FW_ADD_STA_FAILURE                                  (15)
#define AIIO_WLAN_FW_BEACON_LOSS                                      (16)
#define AIIO_WLAN_FW_JOIN_NETWORK_SECURITY_NOMATCH                    (17)
#define AIIO_WLAN_FW_JOIN_NETWORK_WEPLEN_ERROR                        (18)
#define AIIO_WLAN_FW_DISCONNECT_BY_USER_WITH_DEAUTH                   (19)
#define AIIO_WLAN_FW_DISCONNECT_BY_USER_NO_DEAUTH                     (20)
#define AIIO_WLAN_FW_DISCONNECT_BY_FW_PS_TX_NULLFRAME_FAILURE         (21)
#define AIIO_WLAN_FW_TRAFFIC_LOSS                                     (22)
#define AIIO_WLAN_FW_CONNECT_ABORT_BY_USER_WITH_DEAUTH                (23)
#define AIIO_WLAN_FW_CONNECT_ABORT_BY_USER_NO_DEAUTH                  (24)
#define AIIO_WLAN_FW_CONNECT_ABORT_WHEN_JOINING_NETWORK               (25)
#define AIIO_WLAN_FW_CONNECT_ABORT_WHEN_SCANNING                      (26)

typedef enum {
    //wifi encryption mode
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_OPEN=0,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WEP,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA_PSK,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA2_PSK,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA_WPA2_PSK,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA_ENT,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA3_SAE,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_WPA2_PSK_WPA3_SAE,
    AIIO_WIFI_EVENT_BEACON_IND_AUTH_UNKNOWN=255,
} aiio_wifi_beacon_auth_t;


enum AIIO_WIFI_STATE_ENUM_LIST {
    AIIO_WIFI_STATE_UNKNOWN                              = 0x00,
    AIIO_WIFI_STATE_IDLE                                 = 0x01,
    AIIO_WIFI_STATE_CONNECTING                           = 0x02,
    AIIO_WIFI_STATE_CONNECTED_IP_GETTING                 = 0x03,
    AIIO_WIFI_STATE_CONNECTED_IP_GOT                     = 0x04,
    AIIO_WIFI_STATE_DISCONNECT                           = 0x05,
    AIIO_WIFI_STATE_WITH_AP_IDLE                         = 0x11,
    AIIO_WIFI_STATE_WITH_AP_CONNECTING                   = 0x12,
    AIIO_WIFI_STATE_WITH_AP_CONNECTED_IP_GETTING         = 0x13,
    AIIO_WIFI_STATE_WITH_AP_CONNECTED_IP_GOT             = 0x14,
    AIIO_WIFI_STATE_WITH_AP_DISCONNECT                   = 0x15,
    AIIO_WIFI_STATE_IFDOWN                               = 0x06,
    AIIO_WIFI_STATE_SNIFFER                              = 0x07,
    AIIO_WIFI_STATE_PSK_ERROR                            = 0x08,
    AIIO_WIFI_STATE_NO_AP_FOUND                          = 0x09,
};


typedef enum {
    AIIO_WIFI_EVENT_WIFI_UNKNOWN = 0,
    AIIO_WIFI_EVENT_WIFI_READY,               /**< WiFi ready */
    AIIO_WIFI_EVENT_SCAN_DONE,                /**< finish scanning AP */
    AIIO_WIFI_EVENT_STA_CONNECTING,           /**< station connecting to AP */
    AIIO_WIFI_EVENT_STA_CONNECTED,            /**< station connected to AP */
    AIIO_WIFI_EVENT_STA_DISCONNECTED,         /**< station disconnected from AP */
    AIIO_WIFI_EVENT_STA_GOT_IP,

    AIIO_WIFI_EVENT_AP_START,                 /**< soft-AP start */
    AIIO_WIFI_EVENT_AP_STOP,                  /**< soft-AP stop */
    AIIO_WIFI_EVENT_AP_STACONNECTED,          /**< a station connected to soft-AP */
    AIIO_WIFI_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from soft-AP */

    AIIO_WIFI_EVENT_STA_STARTUP,
    AIIO_WIFI_EVENT_STA_SCANING,
    AIIO_WIFI_EVENT_STA_DISCONNECTING,

    AIIO_WIFI_EVENT_MAX,                      /**< Invalid WiFi event ID */
} aiio_wifi_event_t;

typedef struct {
    uint32_t time;
    uint16_t type;
    uint16_t code;
    unsigned long value;
    unsigned long extra;
} aiio_input_event_t;

typedef enum {
    AIIO_WIFI_IF_STA = 1,
    AIIO_WIFI_IF_AP  = 2,
} aiio_wifi_port_t;


typedef enum {
    AIIO_WIFI_MODE_NULL = 0,  /**< null mode */
    AIIO_WIFI_MODE_STA,       /**< WiFi station mode */
    AIIO_WIFI_MODE_AP,        /**< WiFi soft-AP mode */
    AIIO_WIFI_MODE_APSTA,     /**< WiFi station + soft-AP mode */
    AIIO_WIFI_MODE_MAX
} aiio_wifi_mode_t;

typedef struct {
    uint16_t lease_time;    //default:120
    uint8_t start;          //default:2
    uint8_t end;            //default:100
} aiio_wifi_dhcp_server_t;

typedef struct {
    uint8_t ssid[SSID_LEN];                 /**< SSID of  soft-AP. */
    uint8_t password[PASSWORD_LEN];         /**< Password of  soft-AP. */
    uint8_t ssid_len;                       /**< Optional length of SSID field. */
    int32_t channel;                        /**< Channel of  soft-AP */
    int32_t ssid_hidden;                    /**< Broadcast SSID or not, default 0, broadcast the SSID */
    uint8_t max_connection;                 /**< Max number of stations allowed to connect in, default 4, max 10 */
    uint8_t use_dhcp;                       /*Ap dhcp mode   default:1 enable*/
    aiio_wifi_dhcp_server_t dhcp_params;    /*Ap dhcp mode params.  This parameter is valid when dhcp is enabled */
} aiio_wifi_ap_config_t;


typedef struct {
    uint8_t ssid[SSID_LEN];                 /**< SSID of target AP. */
    uint8_t password[PASSWORD_LEN];         /**< Password of target AP. */
    uint8_t bssid[MAC_LEN];                 /**< MAC address of target AP*/
    uint8_t band;                           /**< RF bands 2.4.GHz or 5 GHz*/
    uint8_t channel;                        /**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP.
                                            If the channel of AP is unknown, set it to 0.*/
    uint8_t use_dhcp;
    /*following flag values and connection flags*/
    #define WIFI_CONNECT_STOP_SCAN_ALL_CHANNEL_IF_TARGET_AP_FOUND     (1 << 6)
    #define WIFI_CONNECT_PCI_EN                                       (1 << 7)
    #define WIFI_CONNECT_STOP_SCAN_CURRENT_CHANNEL_IF_TARGET_AP_FOUND (1 << 8)
    #define WIFI_CONNECT_PMF_CAPABLE                                  (1 << 9)
    #define WIFI_CONNECT_PMF_REQUIRED                                 (1 << 10)
    uint32_t flags;
} aiio_wifi_sta_config_t;


typedef struct {
    aiio_wifi_ap_config_t  ap;  /**< configuration of AP */
    aiio_wifi_sta_config_t sta; /**< configuration of STA */
} aiio_wifi_config_t;


typedef struct {
    uint32_t ip;
    uint32_t gateway;
    uint32_t netmask;
} aiio_wifi_ip_params_t;

typedef struct aiio_wifi_conf {
    char country_code[4];  //"CN","JP","US","EU"
    int32_t channel_nums;
} aiio_wifi_conf_t;

typedef enum {
    AIIO_SCAN_PASSIVE=0,
    AIIO_SCAN_ACTIVE
}aiio_scam_mode_t;

typedef enum {
    AIIO_WIFI_SCAN_DONE_EVENT_OK                         = 0x00,
    AIIO_WIFI_SCAN_DONE_EVENT_BUSY                       = 0x01,
}aiio_scan_event_type_t;


typedef struct wifi_scan_params {
    uint8_t bssid[MAC_LEN];                             //scan specify mac address
    uint16_t channel_num;                               //scan Number of channels
    uint16_t channels[CONFIG_MAX_FIXED_CHANNELS_LIMIT]; //scan channels
    char ssid[SSID_LEN];                                //scan specify ssid
    aiio_scam_mode_t scan_mode;                         //scan mode
    uint32_t duration_scan;                             //scan duration_scan   unit:us
} aiio_wifi_scan_params_t;

typedef struct aiio_wifi_ap_item {
    char ssid[32];
    char ssid_tail[1];                                  //always put ssid_tail after ssid
    uint32_t ssid_len;
    uint8_t bssid[MAC_LEN];
    uint8_t channel;
    uint8_t auth;
    uint8_t cipher;
    int8_t rssi;
} aiio_wifi_ap_item_t;

typedef struct aiio_wifi_sta_connect_ind_stat_info {
    uint16_t status_code;
    /*mgmr recv ind event from fw when connect or disconnect  */
    uint8_t type_ind;
    char ssid[32];
    char passphr[65];
    char psk[64];
    uint8_t bssid[MAC_LEN];
    uint8_t chan_id;
    uint8_t chan_band;
    uint8_t security;  //This parameter is invalid in 602
} aiio_wifi_sta_connect_ind_stat_info_t;


typedef struct aiio_wifi_sta_basic_info {
    uint8_t sta_idx;
    uint8_t is_used;
    uint8_t sta_mac[MAC_LEN];
    uint32_t tsfhi;
    uint32_t tsflo;
    int32_t rssi;
    uint8_t data_rate;
} aiio_wifi_sta_basic_info_t;


struct wifi_ap_data_rate {
    uint8_t data_rate;
    const char *val;
};

static const struct wifi_ap_data_rate data_rate_list[] = {
    {WIFI_AP_DATA_RATE_1Mbps, "1.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_2Mbps, "2.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_5_5Mbps, "5.5 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_11Mbps, "11.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_6Mbps, "6.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_9Mbps, "9.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_12Mbps, "12.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_18Mbps, "18.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_24Mbps, "24.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_36Mbps, "36.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_48Mbps, "48.0 Mbit/s, 20Mhz"},
    {WIFI_AP_DATA_RATE_54Mbps, "54.0 Mbit/s, 20Mhz"},
};

typedef struct {
    int8_t rssi;
}aiio_bl_rx_info_t;

typedef int aiio_err_t;
typedef void (*aiio_scan_complete_cb_t)(void *data, void *param);
typedef void (*aiio_scan_item_cb_t)(aiio_wifi_ap_item_t *env, uint32_t *param1, aiio_wifi_ap_item_t *item);
typedef void (*aiio_event_cb)(aiio_input_event_t *event, void *data);
typedef void (*aiio_sniffer_cb_t)(void *env, uint8_t *pkt, int32_t len, aiio_bl_rx_info_t *info);


/** @brief  Initialize WiFi.
 *  @note               This API must be called before all other WiFi API can be called.
 *
 *  @return             Return the operation status. When the return value is AIIO_OK, the initialization is successful.
 *  @retval             AIIO_OK         Init successful.
 *  @retval             AIIO_ERROR      Init error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_init(void);

/** @brief  Register the WiFi event callback.
 *  @param[in] cb       event callback.
 *  @return             Return the operation status. When the return value is AIIO_OK, the register is successful.
 *  @retval             AIIO_OK         register successful.
 *  @retval             AIIO_ERROR      register error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_register_event_cb(aiio_event_cb cb);

/** @brief  Unregister the WiFi event callback.
 *  @param
 *  @return             Return the operation status. When the return value is AIIO_OK, the unregister is successful.
 *  @retval             AIIO_OK         unregister successful.
 *  @retval             AIIO_ERROR      unregister error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_unregister_event_cb(void);

/** @brief  Set the WiFi operating mode.
 *  @note               Set the WiFi operating mode as station, soft-AP or station+soft-AP
 *  @param[in] mode     WiFi operating mode.
 *  @return             Return the operation status. When the return value is AIIO_OK, the set is successful.
 *  @retval             AIIO_OK         set successful.
 *  @retval             AIIO_ERROR      set error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_set_mode(aiio_wifi_mode_t mode);

/** @brief  Get current operating mode of WiFi.
 *  @param[out] mode    WiFi operating mode.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         get successful.
 *  @retval             AIIO_ERROR      get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_get_mode(aiio_wifi_mode_t *mode);

/** @brief  Set the  WiFi configuration.
 *  @note               Set wifi config.
 *  @param[in] port     WiFi port.
 *  @param[in] conf     WiFi config.
 *  @return             Return the operation status. When the return value is AIIO_OK, the set is successful.
 *  @retval             AIIO_OK         set successful.
 *  @retval             AIIO_ERROR      set error.
 *  @see
 */

CHIP_API aiio_err_t aiio_wifi_set_config(aiio_wifi_port_t port,aiio_wifi_config_t *conf);

/** @brief  Get the  WiFi configuration.
 *  @note               Get wifi config.
 *  @param[in] port     WiFi port.
 *  @param[in] conf     WiFi config.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         get successful.
 *  @retval             AIIO_ERROR      get error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_get_config(aiio_wifi_port_t port,aiio_wifi_config_t *conf);

/** @brief  Connect the WiFi station to the AP.
 *  @note               This API only impact WIFI_MODE_STA or WIFI_MODE_APSTA mode.
 *  @return             Return the operation status. When the return value is AIIO_OK, the connect is successful.
 *  @retval             AIIO_OK         connect successful.
 *  @retval             AIIO_ERROR      connect error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_connect(void);

/** @brief  Disconnect the WiFi station from the AP.
 *
 *  @return             Return the operation status. When the return value is AIIO_OK, the disconnect is successful.
 *  @retval             AIIO_OK         disconnect successful.
 *  @retval             AIIO_ERROR      disconnect error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_disconnect(void);

/** @brief  Stop soft-AP and free soft-AP control block.
 *
 *  @return             Return the operation status. When the return value is AIIO_OK, the stop is successful.
 *  @retval             AIIO_OK         stop soft-AP successful.
 *  @retval             AIIO_ERROR      stop soft-AP error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_stop(void);

/** @brief  Gets STA information connected to the current AP .
 *  @param[out] sta_info_p    STA information.
 *  @param[out] idx     STA idx.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_sta_info_get(aiio_wifi_sta_basic_info_t *sta_info_p,uint8_t idx);

/** @brief  Gets STA information connected to the current AP.
 *  @param[out] sta_num     STA num.
 *  @param[out] sta_info_p    STA information.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_sta_list_get(uint8_t *sta_num,aiio_wifi_sta_basic_info_t *sta_info_p);

/** @brief  Gets the status code and error cause code for the current connection.
 *  @param[out] status_code    WiFi connect status_code.
 *  @param[out] reason_code    WiFi connect reason_code.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_conn_result_get(uint16_t *status_code, uint16_t *reason_code);

/** @brief  Gets the connection state converted to type str.
 *  @param[in] status_code    WiFi connect status_code.
 *  @return             Return the type str connection state.
 *  @retval
 *  @retval
 *  @note
 *  @see
 */
CHIP_API char *aiio_wifi_status_code_str(uint16_t status_code);

/** @brief  Gets the status code for the current connection.
 *  @param[out] state   WiFi connect status_code.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_get_state(int32_t *state);

/** @brief Start WiFi according to current configuration
 *  @note               If mode is WIFI_MODE_STA, it create station control block and start station
 *                      If mode is WIFI_MODE_AP, it create soft-AP control block and start soft-AP
 *                      If mode is WIFI_MODE_APSTA, it create soft-AP and station control block and start soft-AP and station.
 *  @return             Return the operation status. When the return value is AIIO_OK, the start is successful.
 *  @retval             AIIO_OK         start successful.
 *  @retval             AIIO_ERROR      start error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_start(void);

/** @brief Stop WiFi according to current configuration
 *  @note               If mode is WIFI_MODE_STA, it stop station and free station control block
 *                      If mode is WIFI_MODE_AP, it stop soft-AP and free soft-AP control block
 *                      If mode is WIFI_MODE_APSTA, it stop station/soft-AP and free station/soft-AP control block
 *  @return             Return the operation status. When the return value is AIIO_OK, the stop is successful.
 *  @retval             AIIO_OK         stop successful.
 *  @retval             AIIO_ERROR      stop error.
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_stop(void);

/** @brief  Scan all available APs.
 *  @param[in] data     Callback data.
 *  @param[in] cb       Callback function.
 *  @param[in] filter   scan filter 1:enable  0:disable.
 *  @return             Return the operation status. When the return value is AIIO_OK, the scan is successful.
 *  @retval             AIIO_OK         scan successful.
 *  @retval             AIIO_ERROR      scan error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_scan(void *data, aiio_scan_complete_cb_t cb,bool filter);

/** @brief  Scan all available APs,parameters control the scanning conditions.
 *  @param[in] data     Callback data.
 *  @param[in] cb       Callback function.
 *  @param[in] filter   scan filter 1:enable  0:disable.
 *  @param[in] scan_params   scan params.
 *  @return             Return the operation status. When the return value is AIIO_OK, the scan is successful.
 *  @retval             AIIO_OK         scan successful.
 *  @retval             AIIO_ERROR      scan error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_scan_adv(void *data, aiio_scan_complete_cb_t cb,bool filter,aiio_wifi_scan_params_t *scan_params);

/** @brief  Scan all available APs,parameters control the scanning conditions.
 *  @param[in] env      Scanned AP information.
 *  @param[in] param1   Callback data.
 *  @param[in] cb       Callback function.
 *  @return             Return the operation status. When the return value is AIIO_OK, the scan is successful.
 *  @retval             AIIO_OK         scan successful.
 *  @retval             AIIO_ERROR      scan error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_scan_ap_all(aiio_wifi_ap_item_t *env, uint32_t *param1, aiio_scan_item_cb_t cb);


/** @brief  Get WIFI channel information.
 *  @param[out] wifi_chan_conf   channel config.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_get_wifi_channel_conf(aiio_wifi_conf_t *wifi_chan_conf);

/** @brief  Set sta MAC address.
 *  @param[in] mac      mac address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the set is successful.
 *  @retval             AIIO_OK         set successful.
 *  @retval             AIIO_ERROR      set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_mac_set(uint8_t *mac);

/** @brief  Get sta MAC address.
 *  @param[out] mac     mac address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_mac_get(uint8_t *mac);

/** @brief  Set ap MAC address.
 *  @param[in] mac      mac address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the set is successful.
 *  @retval             AIIO_OK         set successful.
 *  @retval             AIIO_ERROR      set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_mac_set(uint8_t *mac);

/** @brief  Get ap MAC address.
 *  @param[out] mac     mac address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_mac_get(uint8_t *mac);

/** @brief  Get the STA IP address of the current WIFI connection.
 *  @param[out] ip      IP address.
 *  @param[out] gw      gateway address.
 *  @param[out] mask    netmask address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 */
CHIP_API aiio_err_t aiio_wifi_sta_ip_get(uint32_t *ip, uint32_t *gw, uint32_t *mask);

/** @brief  Get the STA netif IP address of the current WIFI connection.
 *  @param[out] ip      IP address.
 *  @param[out] gw      gateway address.
 *  @param[out] mask    netmask address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 */
CHIP_API aiio_err_t aiio_wifi_sta_netif_get(uint32_t *ip, uint32_t *gw, uint32_t *mask);

/** @brief  Set the STA IP address of the current WIFI connection.
 *  @param[in] sta_ip   STA IP address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_ip_set(char* sta_ip);

/** @brief  Set the STA IP parameter of the current WIFI connection.
 *  @param[in] ip       IP address.
 *  @param[in] mask     mask address.
 *  @param[in] gw       gw address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_ip_parameter_set(uint32_t ip, uint32_t mask, uint32_t gw);

/** @brief  Reset the STA IP address of the current WIFI connection.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_ip_unset(void);

/** @brief  Set the AP IP address of the current AP.
 *  @param[in] ap_ip    AP IP address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_ip_set(char* ap_ip);

/** @brief  Get the AP IP address of the current WIFI .
 *  @param[out] ip      IP address.
 *  @param[out] gw      gateway address.
 *  @param[out] mask    netmask address.
 *  @return             Return the operation status. When the return value is AIIO_OK, the get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_ap_ip_get(uint32_t *ip, uint32_t *gw, uint32_t *mask);

/** @brief  set the country code.
 *  @param[in] country_code   country code.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_set_country_code(char *country_code);

/** @brief  Get the country code.
 *  @param[out] country_code   country code.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Get is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_get_country_code(char *country_code);

/** @brief  Get the channel.
 *  @param[out] channel wifi ap channel.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Get is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_channel_get(int32_t *channel);

/** @brief  Set the channel.
 *  @param[out] channel wifi ap channel.
 *  @return             Return the operation status. When the return value is AIIO_OK, the Set is successful.
 *  @retval             AIIO_OK         Set successful.
 *  @retval             AIIO_ERROR      Set error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_channel_set(int32_t channel);

/** @brief  Get the rssi.
 *  @param[out] rssi    wifi rssi
 *  @return             Return the operation status. When the return value is AIIO_OK, the Get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_rssi_get(int32_t *rssi);

/** @brief  Get the sta connect info.
 *  @param[out] wifi_ind_stat    wifi conncet info
 *  @return             Return the operation status. When the return value is AIIO_OK, the Get is successful.
 *  @retval             AIIO_OK         Get successful.
 *  @retval             AIIO_ERROR      Get error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sta_connect_ind_stat_get(aiio_wifi_sta_connect_ind_stat_info_t *wifi_ind_stat);

/** @brief  Wifi sniffer mode enable.
 *  @return             Return the operation status. When the return value is AIIO_OK, the enable is successful.
 *  @retval             AIIO_OK         enable successful.
 *  @retval             AIIO_ERROR      enable error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sniffer_enable(void);

/** @brief  Wifi sniffer mode disable.
 *  @return             Return the operation status. When the return value is AIIO_OK, the disable is successful.
 *  @retval             AIIO_OK         disable successful.
 *  @retval             AIIO_ERROR      disable error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sniffer_disable(void);

/** @brief  Register the WiFi sniffer callback.
 *  @param[in] env      Callback data.
 *  @param[in] cb       Callback function.
 *  @return             Return the operation status. When the return value is AIIO_OK, the register is successful.
 *  @retval             AIIO_OK         register successful.
 *  @retval             AIIO_ERROR      register error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sniffer_register(void *env, aiio_sniffer_cb_t cb);

/** @brief  Unregister the WiFi sniffer callback.
 *  @param[in] env      Callback data.
 *  @return             Return the operation status. When the return value is AIIO_OK, the unregister is successful.
 *  @retval             AIIO_OK         unregister successful.
 *  @retval             AIIO_ERROR      unregister error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_wifi_sniffer_unregister(void *env);

/** @brief  Gets the encryption method of the sta mode.
 *  @return             Return the encryption type of the currently connected hotspot.
 *  @retval             aiio_wifi_beacon_auth_t
 *  @retval
 *  @note
 *  @see
 */
CHIP_API aiio_wifi_beacon_auth_t aiio_wifi_get_sta_security(void);

#endif
