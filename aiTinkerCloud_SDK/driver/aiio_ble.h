/** @brief      Ble application interface.
 *
 *  @file       aiio_ble.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       Ble application interface.
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/02/06      <td>1.0.0       <td>chenzf      <td>Define BLE API
 *  </table>
 *
 */

#ifndef __AIIO_BLE_H__
#define __AIIO_BLE_H__

#include "aiio_type.h"
#include "aiio_error.h"

#include <stdint.h>

#define AIIO_BLE_ADV_DATA_MAX  31

typedef int aiio_err_t;
typedef void *aiio_ble_conn_t;
typedef void *aiio_ble_att_handle_t;

typedef enum {
    AIIO_BLE_DEF_SRV1,
    AIIO_BLE_DEF_SRV2,
} aiio_ble_default_srv_t;

typedef enum {
    AIIO_BLE_UUID_TYPE_16,
    AIIO_BLE_UUID_TYPE_32,
    AIIO_BLE_UUID_TYPE_128,
} aiio_ble_uuid_type_t;

typedef enum {
    AIIO_BLE_EVT_DISCONNECT,
    AIIO_BLE_EVT_CONNECT,
    AIIO_BLE_EVT_MTU,
    AIIO_BLE_EVT_DATA,
    AIIO_BLE_EVT_SCAN,
    AIIO_BLE_EVT_DISC,
} aiio_ble_evt_type_t;

typedef enum {
    AIIO_BLE_NOTIFY,
    AIIO_BLE_INDICATE,
    AIIO_BLE_WRITE,
    AIIO_BLE_WRITE_WITHOUT_RESP,
} aiio_ble_data_type_t;

typedef enum {
    AIIO_BLE_CONN_MODE_NON,
    AIIO_BLE_CONN_MODE_DIR,
    AIIO_BLE_CONN_MODE_UND,
} aiio_ble_conn_mode_t;

typedef enum {
    AIIO_BLE_DISC_MODE_NON,
    AIIO_BLE_DISC_MODE_LTD,
    AIIO_BLE_DISC_MODE_GEN,
} aiio_ble_disc_mode_t;

typedef enum {
    AIIO_BLE_ADDR_PUBLIC,
    AIIO_BLE_ADDR_RANDOM,
} aiio_ble_addr_type_t;

typedef enum {
    AIIO_BLE_DISCOVER_PRIMARY,
    AIIO_BLE_DISCOVER_SECONDARY,
    AIIO_BLE_DISCOVER_INCLUDE,
    AIIO_BLE_DISCOVER_CHARACTERISTIC,
    AIIO_BLE_DISCOVER_DESCRIPTOR,
    AIIO_BLE_DISCOVER_ATTRIBUTE,
} aiio_ble_discover_type_t;

typedef enum {
    /* Scan without requesting additional information from advertisers. */
    AIIO_BLE_SCAN_TYPE_PASSIVE = 0x00,
    /* Scan and request additional information from advertisers. */
    AIIO_BLE_SCAN_TYPE_ACTIVE = 0x01,
} aiio_ble_scan_type_t;

#define AIIO_BLE_UUID_INIT_16(value...)     \
{                                           \
    .uuid = { AIIO_BLE_UUID_TYPE_16 },      \
    .val = { value },                       \
}

#define AIIO_BLE_UUID_INIT_32(value...)     \
{                                           \
    .uuid = { AIIO_BLE_UUID_TYPE_32 },      \
    .val = { value },                       \
}

#define AIIO_BLE_UUID_INIT_128(value...)    \
{                                           \
    .uuid = { AIIO_BLE_UUID_TYPE_128 },     \
    .val = { value },                       \
}

typedef struct {
    uint8_t type;
} aiio_ble_uuid_t;

typedef struct {
    uint16_t handle;
    aiio_ble_uuid_t *uuid;
} aiio_ble_gatt_attr_t;

typedef struct {
    /**
     * Indicates the type of ble event that occurred. This is one of the
     * aiio_ble_evt_type_t codes.
     */
    uint8_t type;
    union {
        struct {
            aiio_ble_conn_t conn;
        } connect;

        struct {
            aiio_ble_conn_t conn;
            int32_t reason;
        } disconnect;

        struct {
            aiio_ble_conn_t conn;
            uint16_t mtu;
        } mtu;

        struct {
            int8_t rssi;
            uint8_t mac[6];
            uint8_t *ad_data;
            uint8_t *sd_data;
            uint16_t ad_len;
            uint16_t sd_len;
        } scan;

        struct {
            aiio_ble_conn_t conn;
            /**
             * This is one of the aiio_ble_data_type_t codes.
             */
            uint8_t type;
            uint8_t *data;
            uint16_t length;
            aiio_ble_att_handle_t handle;
        } data;

        struct {
            aiio_ble_conn_t conn;
            aiio_ble_gatt_attr_t *attr;
        } disc;
    };
} aiio_ble_evt_t;

typedef struct {
    /**
     * This is one of the aiio_ble_data_type_t codes.
     */
    uint8_t type;
    uint8_t *data;
    uint16_t length;
    aiio_ble_att_handle_t handle;
    aiio_ble_conn_t conn;
} aiio_ble_send_t;

#define AIIO_BLE_UUID_DECLARE_16(value...) \
    ((aiio_ble_uuid_t *) ((aiio_ble_uuid_16_t[]) {AIIO_BLE_UUID_INIT_16(value)}))
#define AIIO_BLE_UUID_DECLARE_32(value...) \
    ((aiio_ble_uuid_t *) ((aiio_ble_uuid_32_t[]) {AIIO_BLE_UUID_INIT_32(value)}))
#define AIIO_BLE_UUID_DECLARE_128(value...) \
    ((aiio_ble_uuid_t *) ((aiio_ble_uuid_128_t[]) {AIIO_BLE_UUID_INIT_128(value)}))

typedef void (*aiio_ble_cb_t)(aiio_ble_evt_t *event);

typedef struct {
    /**
     * This is one of the aiio_ble_discover_type_t codes.
     */
    uint8_t type;
    uint16_t start_handle;
    uint16_t end_handle;
    aiio_ble_uuid_t *uuid;
    aiio_ble_conn_t conn;
} aiio_ble_disc_param_t;

typedef struct {
    aiio_ble_uuid_t uuid;
    uint8_t val[2];
} aiio_ble_uuid_16_t;

typedef struct {
    aiio_ble_uuid_t uuid;
    uint8_t val[4];
} aiio_ble_uuid_32_t;

typedef struct {
    aiio_ble_uuid_t uuid;
    uint8_t val[16];
} aiio_ble_uuid_128_t;

typedef struct {
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t latency;
    uint16_t timeout;
} aiio_ble_conn_interval_t;

typedef struct {
    /** Scan type aiio_ble_scan_type_t */
    uint8_t type;
    /** Scan interval (N * 0.625 ms) */
    uint16_t interval;
    /** Scan window (N * 0.625 ms) */
    uint16_t window;
} aiio_ble_scan_param_t;

typedef struct {
    /** Advertising mode. Can be one of following constants:
     *  - AIIO_BLE_CONN_MODE_NON (non-connectable; 3.C.9.3.2).
     *  - AIIO_BLE_CONN_MODE_DIR (directed-connectable; 3.C.9.3.3).
     *  - AIIO_BLE_CONN_MODE_UND (undirected-connectable; 3.C.9.3.4).
     */
    uint8_t conn_mode;
    /** Discoverable mode. Can be one of following constants:
     *  - AIIO_BLE_DISC_MODE_NON  (non-discoverable; 3.C.9.2.2).
     *  - AIIO_BLE_DISC_MODE_LTD (limited-discoverable; 3.C.9.2.3).
     *  - AIIO_BLE_DISC_MODE_GEN (general-discoverable; 3.C.9.2.4).
     */
    uint8_t disc_mode;
    /** Advertising Interval (N * 0.625) */
    uint16_t interval_min;
    uint16_t interval_max;
} aiio_ble_adv_param_t;

typedef struct {
    aiio_ble_conn_interval_t conn_intv;
} aiio_ble_config_t;

typedef struct {
    aiio_ble_uuid_t *server_uuid[2];
    aiio_ble_uuid_t *tx_char_uuid[2];
    aiio_ble_uuid_t *rx_char_uuid[2];
} aiio_ble_default_server_t;

typedef struct {
    aiio_ble_att_handle_t tx_char_handle[2];
    aiio_ble_att_handle_t rx_char_handle[2];
} aiio_ble_default_handle_t;

typedef enum {
    AIIO_BLE_MODE_PERIPHERAL,    /**< Peripheral mode */
    AIIO_BLE_MODE_CENTRAL,       /**< Central mode */
    AIIO_BLE_MODE_MAX
} aiio_ble_mode_t;

/** @brief Init and alloc the resource for ble
 *
 *  @param[in]      mode            BLE mode
 *  @param[in]      config          BLE config
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_init(aiio_ble_mode_t mode, const aiio_ble_config_t *config);

/** @brief Deinit and free the resource for ble
 *
 *  @return             Return the operation status.
 *  @retval             AIIO_OK         Init successful.
 *  @retval             AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_deinit(void);

/** @brief  Register the ble event callback
 *
 *  @param[in]      cb              BLE event callback function, if set NULL means unregister
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_register_event_cb(aiio_ble_cb_t cb);

/** @brief  Start advertising
 *
 *  @param[in]      cb              Advertising parameters.
 *  @param[in]      ad              Data to be used in advertisement packets.
 *  @param[in]      ad_len          advertisement packets size.
 *  @param[in]      sd              Data to be used in scan response packets.
 *  @param[in]      sd_len          scan response packets size.
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_adv_start(aiio_ble_adv_param_t *param, const uint8_t *ad, uint16_t ad_len, const uint8_t *sd, uint16_t sd_len);

/** @brief  Stop advertising
 *
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_adv_stop(void);

/** @brief Register GATT default service
 *
 *  @param[in]      cfg             Default gatt server uuid config
 *  @param[out]     handle          Operate handle
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t ble_gatts_add_default_svcs(aiio_ble_default_server_t *cfg, aiio_ble_default_handle_t *handle);

/** @brief Get ble public mac
 *
 *  @param[out]     mac             MAC address
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_get_mac(uint8_t *mac);

/** @brief Set ble public mac
 *
 *  @param[in]      mac             MAC address
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_set_mac(const uint8_t *mac);

/** @brief Get ble power Maximum/Minimum
 *
 *  @param[out]     min             Maximum power
 *  @param[out]     max             Minimum power
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_get_limit_power(int8_t *min, int8_t *max);

/** @brief Exchange MTU
 *
 *  @param[in]      conn            Connect handle
 *  @param[in]      mtu             Prefer mtu size
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_mtu_req(aiio_ble_conn_t conn, uint16_t mtu);

/** @brief Get MTU
 *
 *  @param[in]      conn            Connect handle
 *  @param[in]      mtu             MTU size
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_mtu_get(aiio_ble_conn_t conn, uint16_t *mtu);

/** @brief  Ble restart adverting
 *
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_restart_advertising(void);

/** @brief Disconnect from a remote device
 *
 *  @param[in]      conn            Connect handle
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_disconnect(aiio_ble_conn_t conn);

/** @brief LE connect
 *
 *  @param[in]      addr_type       Addr type(aiio_ble_addr_type_t)
 *  @param[in]      addr            MAC
 *  @param[in]      timeout         Timeout(ms)
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_connect(uint8_t addr_type, uint8_t *addr, uint32_t timeout);

/** @brief Start LE scanning
 *
 *  @param[in]      cfg             Scan config
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_scan(const aiio_ble_scan_param_t *cfg);

/** @brief Stop LE scanning
 *
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_scan_stop(void);

/** @brief Send ble data
 *
 *  @param[in]      data            ble data
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_send(const aiio_ble_send_t *data);

/** @brief Discover ble server
 *
 *  @param[in]      param           Discover param
 *  @param[in]      timeout         Discover timeout(ms)
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_discover(const aiio_ble_disc_param_t *param, uint16_t timeout);

/** @brief Set ble device name
 *
 *  @param[in]      name            Device name
 *  @return                         Return the operation status. When the return value is AIIO_OK, is successful.
 *  @retval         AIIO_OK         Init successful.
 *  @retval         AIIO_ERROR      Init error.
 *  @note
 *  @see
 */
CHIP_API aiio_err_t aiio_ble_set_name(const char *name);

#endif
