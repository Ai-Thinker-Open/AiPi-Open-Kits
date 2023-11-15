#include "aiio_ble.h"
#include "aiio_os_port.h"
#include "aiio_log.h"

#include "gatt.h"
#include "conn.h"
#include "conn_internal.h"
#include "hci_core.h"
#include "hci_driver.h"
#include "btble_lib_api.h"
#include "bl616_glb.h"
#include "rfparam_adapter.h"

static aiio_ble_config_t ble_config_s;
static volatile uint8_t ble_conn_flag;
static aiio_ble_cb_t ble_cb;
static uint8_t specific_data[16];
static char adv_name[30] = {};
static uint16_t appearance;
static uint8_t server_uuid[4];
static struct bt_data salve_rsp[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, adv_name, 3),
};
static struct bt_data salve_adv[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL),
    BT_DATA(BT_DATA_GAP_APPEARANCE, &appearance, 2),
    BT_DATA(BT_DATA_UUID16_ALL, server_uuid, 4),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, specific_data, 16),
};

static uint8_t ble_ibeacon_data[0x19] = {
    0x4C, 0x00,       /* Company: Apple */
    0x02,             /* Type: iBeacon */
    0x15,             /* iBeacon data length 0x15 (21) = UUID (16) + major (2) + minor (2) + RSSI (1) */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, /* 16 byte UUID (example) */
    0x00, 0x7B,       /* major (example: 123)*/
    0x01, 0xC8,       /* minor (example: 456)*/
    0xBF,             /* rssi: (example: -65 dBm) */
};
static struct bt_data ad_ibeacon[2] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, "BL602", 0x19),
};
static struct bt_uuid_128 srv_uuid[2];
static struct bt_uuid_128 tx_uuid[2];
static struct bt_uuid_128 rx_uuid[2];

static ssize_t ble_srv1_write_val(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, u16_t len, u16_t offset, u8_t flags);
static ssize_t ble_srv2_write_val(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, u16_t len, u16_t offset, u8_t flags);
static void ble_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                       u16_t value);

static struct bt_gatt_attr salve_server_1[]= {
    /* Primary Service */
    BT_GATT_PRIMARY_SERVICE(&srv_uuid[0]),

    /* Characteristic && Characteristic User Declaration */
    BT_GATT_CHARACTERISTIC(&tx_uuid[0],
                   BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                   BT_GATT_PERM_READ, NULL, NULL,
                   NULL),
    BT_GATT_CCC(ble_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    /* Characteristic && Characteristic User Declaration */
    BT_GATT_CHARACTERISTIC(&rx_uuid[0],
                   BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE, NULL, ble_srv1_write_val,
                   NULL),
};
static struct bt_gatt_attr salve_server_2[]= {
    /* Primary Service */
    BT_GATT_PRIMARY_SERVICE(&srv_uuid[1]),

    /* Characteristic && Characteristic User Declaration */
    BT_GATT_CHARACTERISTIC(&tx_uuid[1],
                   BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                   BT_GATT_PERM_READ, NULL, NULL,
                   NULL),
    BT_GATT_CCC(ble_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    /* Characteristic && Characteristic User Declaration */
    BT_GATT_CHARACTERISTIC(&rx_uuid[1],
                   BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE, NULL, ble_srv2_write_val,
                   NULL),
};

#define SRV_TX_INDEX 2
#define SRV_TX_CCD_INDEX 3
#define SRV_RX_INDEX 5

static struct bt_gatt_service ble_server_1 = BT_GATT_SERVICE(salve_server_1);
static struct bt_gatt_service ble_server_2 = BT_GATT_SERVICE(salve_server_2);

static void ble_reverse_byte(uint8_t *arr, uint32_t size)
{
    uint8_t i, tmp;

    for (i = 0; i < size / 2; i++) {
        tmp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = tmp;
    }
}

static ssize_t ble_srv1_write_val(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, u16_t len, u16_t offset,
             u8_t flags)
{
    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_DATA;
        evt.data.handle = AIIO_BLE_DEF_SRV1;
        evt.data.type = AIIO_BLE_WRITE_WITHOUT_RESP;
        evt.data.conn = (void *)conn;
        evt.data.data = (uint8_t *)buf;
        evt.data.length = len;
        ble_cb(&evt);
    }

    return len;
}

static ssize_t ble_srv2_write_val(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, u16_t len, u16_t offset,
             u8_t flags)
{
    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_DATA;
        evt.data.handle = AIIO_BLE_DEF_SRV2;
        evt.data.type = AIIO_BLE_WRITE_WITHOUT_RESP;
        evt.data.conn = (void *)conn;
        evt.data.data = (uint8_t *)buf;
        evt.data.length = len;
        ble_cb(&evt);
    }

    return len;
}

static void ble_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                       u16_t value)
{
    char *str = "disabled";

    if (value == BT_GATT_CCC_NOTIFY) {
        str = "notify";
    } else if (value == BT_GATT_CCC_INDICATE) {
        str = "indicate";
    }

    aiio_log_d("ccc change %s", str);
}

static void _connected(struct bt_conn *conn, u8_t err)
{
    uint16_t mtu = 0;
    int tx_octets = 0x00fb;
    int tx_time = 0x0848;
    int ret = -1;
    
    if (err) {
        aiio_log_i("connected err:%d", err);
        bt_conn_unref(conn);
        return ;
    }
    aiio_log_d("connected");
    ble_conn_flag = 1;

    //set data length after connected.
    ret = bt_le_set_data_len(conn, tx_octets, tx_time);
    if(!ret)
    {
        aiio_log_d("ble tp set data length success.");
    }
    else
    {
        aiio_log_d("ble tp set data length failure, err: %d\n", ret);
    }

    aiio_ble_mtu_req(conn, mtu);

    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_CONNECT;
        evt.connect.conn = (void *)conn;
        ble_cb(&evt);
    }

    return ;
}

static void _disconnected(struct bt_conn *conn, u8_t reason)
{
    aiio_log_d("disconnected, reason:%d", reason);

    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_DISCONNECT;
        evt.disconnect.conn = (void *)conn;
        evt.disconnect.reason = reason;
        ble_cb(&evt);
    }
}

static bool _le_param_req(struct bt_conn *conn,
             struct bt_le_conn_param *param)
{
    aiio_log_d("conn param request: int 0x%04x-0x%04x lat %d to %d",
                param->interval_min,
                param->interval_max,
                param->latency,
                param->timeout);

    return true;
}

static void _le_param_updated(struct bt_conn *conn, u16_t interval,
                 u16_t latency, u16_t timeout)
{
    aiio_log_d("conn param updated: int 0x%04x lat %d to %d", interval, latency, timeout);
}

static void _le_phy_updated(struct bt_conn *conn, u8_t tx_phy, u8_t rx_phy)
{
    aiio_log_d("phy updated: rx_phy %d, rx_phy %d ", tx_phy, rx_phy);
}

static void _ble_mtu_changed_cb(struct bt_conn *conn, int mtu)
{
    aiio_log_d("mtu updated:%d", mtu);

    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_MTU;
        evt.mtu.conn = (void *)conn;
        evt.mtu.mtu = mtu;
        ble_cb(&evt);
    }
}

static struct bt_conn_cb conn_callbacks = {
    .connected = _connected,
    .disconnected = _disconnected,
    .le_param_req = _le_param_req,
    .le_param_updated = _le_param_updated,
    .le_phy_updated = _le_phy_updated,
};

static void bt_enable_cb(int err)
{
    if (!err) {
        bt_addr_le_t bt_addr;
        bt_get_local_public_address(&bt_addr);
        aiio_log_d("BD_ADDR:(MSB)%02x:%02x:%02x:%02x:%02x:%02x(LSB)",
            bt_addr.a.val[5], bt_addr.a.val[4], bt_addr.a.val[3], bt_addr.a.val[2], bt_addr.a.val[1], bt_addr.a.val[0]);
    }
}

static void ble_stack_start(void)
{
    // Initialize BLE controller
    btble_controller_init(configMAX_PRIORITIES - 1);
    // Initialize BLE Host stack
    hci_driver_init();
    bt_enable(bt_enable_cb);
}

void ble_uuid_convert(struct bt_uuid *dest, aiio_ble_uuid_t *src)
{
    switch (src->type) {
        case AIIO_BLE_UUID_TYPE_16:
            dest->type = BT_UUID_TYPE_16;
            memcpy(&(BT_UUID_16(dest)->val), ((aiio_ble_uuid_16_t *)src)->val, 2);
            ble_reverse_byte(&(BT_UUID_16(dest)->val), 2);
            break;
        case AIIO_BLE_UUID_TYPE_32:
            dest->type = BT_UUID_TYPE_32;
            memcpy(&(BT_UUID_32(dest)->val), ((aiio_ble_uuid_32_t *)src)->val, 4);
            ble_reverse_byte(&(BT_UUID_32(dest)->val), 4);
            break;
        case AIIO_BLE_UUID_TYPE_128:
            dest->type = BT_UUID_TYPE_128;
            memcpy(BT_UUID_128(dest)->val, ((aiio_ble_uuid_128_t *)src)->val, 16);
            ble_reverse_byte(&(BT_UUID_128(dest)->val), 16);
            break;
        default:
            return ;
    }
}

void ble_uuid_convert_to_aiio(aiio_ble_uuid_t *dest, struct bt_uuid *src)
{
    switch (src->type) {
        case BT_UUID_TYPE_16:
            dest->type = AIIO_BLE_UUID_TYPE_16;
            memcpy(((aiio_ble_uuid_16_t *)dest)->val, &(BT_UUID_16(src)->val), 2);
            ble_reverse_byte(((aiio_ble_uuid_16_t *)dest)->val, 2);
            break;
        case BT_UUID_TYPE_32:
            dest->type = AIIO_BLE_UUID_TYPE_32;
            memcpy(((aiio_ble_uuid_32_t *)dest)->val, &(BT_UUID_32(src)->val), 4);
            ble_reverse_byte(((aiio_ble_uuid_32_t *)dest)->val, 4);
            break;
        case BT_UUID_TYPE_128:
            dest->type = AIIO_BLE_UUID_TYPE_128;
            memcpy(((aiio_ble_uuid_128_t *)dest)->val, BT_UUID_128(src)->val, 16);
            ble_reverse_byte(((aiio_ble_uuid_128_t *)dest)->val, 16);
            break;
        default:
            return ;
    }
}

static void ble_notification_all_cb(struct bt_conn *conn, u16_t handle, const void *data, u16_t length)
{
    if (ble_cb) {
        aiio_ble_evt_t evt;

        evt.type = AIIO_BLE_EVT_DATA;
        evt.data.handle = (aiio_ble_att_handle_t)handle;
        evt.data.type = AIIO_BLE_NOTIFY;
        evt.data.conn = (void *)conn;
        evt.data.data = (uint8_t *)data;
        evt.data.length = length;
        ble_cb(&evt);
    }
}

static int ble_salve_init(void)
{
    return AIIO_OK;
}

static int ble_master_init(void)
{
    bt_gatt_register_notification_callback(ble_notification_all_cb);

    return AIIO_OK;
}

static int ble_notify_data(void *conn, struct bt_gatt_attr *char_val, uint8_t *data, uint16_t length)
{
    int ret;
    uint16_t mtu;
    uint16_t offset;
    uint16_t send_len;

    offset = 0;
    mtu = bt_gatt_get_mtu(conn) - 3;
    while (length > 0) {
        /* calculate send_len */
        send_len = length > mtu ? mtu : length;
        /* send data */
        ret = bt_gatt_notify(conn, char_val, data + offset, send_len);
        /* set offset */
        offset += send_len;
        length -= send_len;

        if (ret != 0) {
            break;
        }
    }

    return ret;
}

static int ble_write_norsp_data(void *conn, uint16_t handle, uint8_t *data, uint16_t length)
{
    int ret;
    uint16_t mtu;
    uint16_t offset;
    uint16_t send_len;

    offset = 0;
    mtu = bt_gatt_get_mtu(conn) - 3;
    while (length > 0) {
        /* calculate send_len */
        send_len = length > mtu ? mtu : length;
        /* send data */
        ret = bt_gatt_write_without_response(conn, handle, data + offset, send_len, 0);
        /* set offset */
        offset += send_len;
        length -= send_len;

        if (ret != 0) {
            break;
        }
    }

    return ret;
}

static void ble_disconnect_all(struct bt_conn *conn, void *data)
{
    if (conn->state == BT_CONN_CONNECTED) {
        aiio_log_d("[BLE] disconn id:%d", conn->id);
        bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
    }
}

static void exchange_func(struct bt_conn *conn, u8_t err,
              struct bt_gatt_exchange_params *params)
{
    if (conn) {
        aiio_log_d("[BLE] Exchange %s MTU Size =%d", err == 0U ? "successful" : "failed", bt_gatt_get_mtu(conn));
    }
}

static void scan_device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t evtype,
             struct net_buf_simple *buf)
{
    #define ADV_SIZE 31
    static struct {
        bt_addr_le_t addr;
        uint16_t ad_len;
        uint8_t ad_data[ADV_SIZE];
    } adv_rec;

    /* scannable advertising */
    if (evtype == BT_LE_ADV_IND || evtype == BT_LE_ADV_SCAN_IND) {
        if (buf->len > ADV_SIZE) {
            aiio_log_w("invalid adv size:%d", buf->len);
            return ;
        }

        memcpy(&adv_rec.addr, addr, sizeof(bt_addr_le_t));
        memcpy(adv_rec.ad_data, buf->data, buf->len);
        adv_rec.ad_len = buf->len;
        return ;
    }

    if (ble_cb) {
        aiio_ble_evt_t evt;

        memset(&evt, 0, sizeof evt);
        evt.type = AIIO_BLE_EVT_SCAN;
        evt.scan.rssi = rssi;
        memcpy(evt.scan.mac, addr->a.val, 6);
        ble_reverse_byte(evt.scan.mac, 6);

        if (evtype == BT_LE_ADV_SCAN_RSP) {
            evt.scan.sd_data = buf->data;
            evt.scan.sd_len = buf->len;

            if (memcmp(&adv_rec.addr, addr, sizeof(bt_addr_le_t)) == 0) {
                evt.scan.ad_data = adv_rec.ad_data;
                evt.scan.ad_len = adv_rec.ad_len;
            }
        } else {
            evt.scan.ad_data = buf->data;
            evt.scan.ad_len = buf->len;
        }

        ble_cb(&evt);
    }
}


static int btblecontroller_em_config(void)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    volatile uint32_t em_size;

    em_size = (uint32_t)&__LD_CONFIG_EM_SEL;

    if (em_size == 0) {
        GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);
    } else if (em_size == 32*1024) {
        GLB_Set_EM_Sel(GLB_WRAM128KB_EM32KB);
    } else if (em_size == 64*1024) {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    } else {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }

    return 0;
}


aiio_err_t aiio_ble_init(aiio_ble_mode_t mode, const aiio_ble_config_t *config)
{
    // btblecontroller_em_config();
    // /* Init rf */
    // if (0 != rfparam_init(0, NULL, 0)) {
    //     aiio_log_e("PHY RF init failed!\r\n");
    //     return 0;
    // }
    ble_stack_start();
    bt_gatt_register_mtu_callback(_ble_mtu_changed_cb);
    bt_conn_cb_register(&conn_callbacks);
    /* avoid callback infinite loop */
    conn_callbacks._next = NULL;

    memcpy(&ble_config_s, config, sizeof ble_config_s);

    if (mode == AIIO_BLE_MODE_PERIPHERAL) {
        ble_salve_init();
    } else if (mode == AIIO_BLE_MODE_CENTRAL) {
        ble_master_init();
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_deinit(void)
{
    int ret;

    bt_conn_foreach(BT_CONN_TYPE_ALL, ble_disconnect_all, NULL);

    /* wait for all connection to be disconnected */
    int disconn_cnt = 0;
    while (le_check_valid_conn() && disconn_cnt++ < 10) {
        aiio_log_w("[BLE] wait for ble_disconnect_all");
        aiio_os_tick_dealy(aiio_os_ms2tick(500));
    }

    bt_le_adv_stop();
    ret = bt_disable();
    if (ret) {
        return AIIO_ERROR;
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_register_event_cb(aiio_ble_cb_t cb)
{
    ble_cb = cb;

    return AIIO_OK;
}

aiio_err_t aiio_ble_adv_start(aiio_ble_adv_param_t *param,
                              const uint8_t *ad, uint16_t ad_len,
                              const uint8_t *sd, uint16_t sd_len)
{
    int ret;
    struct bt_le_adv_param adv_param;

    if (param->conn_mode == AIIO_BLE_CONN_MODE_NON &&
        param->disc_mode == AIIO_BLE_DISC_MODE_NON) {
        /* unsupport in the current SDK */
        return AIIO_ERROR;
    }

    adv_param.id = 0;
    adv_param.interval_min = param->interval_min;
    adv_param.interval_max = param->interval_max;
    adv_param.options = BT_LE_ADV_OPT_CONNECTABLE;
    if (sd == NULL) {
        adv_param.options = BT_LE_ADV_OPT_USE_IDENTITY;
    }

    if (param->conn_mode == AIIO_BLE_CONN_MODE_DIR ||
        param->conn_mode == AIIO_BLE_CONN_MODE_UND) {
        adv_param.options |= (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_ONE_TIME);
    }

    set_adv_param(&adv_param);

    ret = set_ad_and_rsp_d(BT_HCI_OP_LE_SET_ADV_DATA, ad, ad_len);
    if (sd) {
        ret |= set_ad_and_rsp_d(BT_HCI_OP_LE_SET_SCAN_RSP_DATA, sd, sd_len);
    }

    if (ret != 0) {
        aiio_log_e("set adv data fail");
        return AIIO_ERROR;
    }

    if (set_adv_enable(1) != 0) {
        aiio_log_e("start adv fail");
        return AIIO_ERROR;
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_adv_stop(void)
{
    bt_le_adv_stop();

    return AIIO_OK;
}

aiio_err_t ble_gatts_add_default_svcs(aiio_ble_default_server_t *cfg, aiio_ble_default_handle_t *handle)
{
    int ret = 0;

    ble_uuid_convert(&srv_uuid[0], cfg->server_uuid[0]);
    ble_uuid_convert(&tx_uuid[0], cfg->tx_char_uuid[0]);
    ble_uuid_convert(&rx_uuid[0], cfg->rx_char_uuid[0]);

    ble_uuid_convert(&srv_uuid[1], cfg->server_uuid[1]);
    ble_uuid_convert(&tx_uuid[1], cfg->tx_char_uuid[1]);
    ble_uuid_convert(&rx_uuid[1], cfg->rx_char_uuid[1]);

    ret = bt_gatt_service_register(&ble_server_1);
    ret |= bt_gatt_service_register(&ble_server_2);

    if (ret != 0) {
        return AIIO_ERROR;
    }

    handle->tx_char_handle[0] = (aiio_ble_att_handle_t)&salve_server_1[SRV_TX_INDEX];
    handle->rx_char_handle[0] = (aiio_ble_att_handle_t)&salve_server_1[SRV_RX_INDEX];

    handle->tx_char_handle[1] = (aiio_ble_att_handle_t)&salve_server_2[SRV_TX_INDEX];
    handle->rx_char_handle[1] = (aiio_ble_att_handle_t)&salve_server_2[SRV_RX_INDEX];

    return AIIO_OK;
}

aiio_err_t aiio_ble_get_mac(uint8_t *mac)
{
    bt_addr_le_t bt_addr;

    bt_get_local_public_address(&bt_addr);
    memcpy(mac, bt_addr.a.val, 6);
    ble_reverse_byte(mac, 6);

    return AIIO_OK;
}

aiio_err_t aiio_ble_set_mac(const uint8_t *mac)
{
    bt_addr_t bt_addr;

    memcpy(bt_addr.val, mac, 6);
    ble_reverse_byte(bt_addr.val, 6);
    bt_set_bd_addr(&bt_addr);

    return AIIO_OK;
}

aiio_err_t aiio_ble_get_limit_power(int8_t *min, int8_t *max)
{
    *min = 0;
    *max = 20;

    return AIIO_OK;
}

aiio_err_t aiio_ble_mtu_req(aiio_ble_conn_t conn, uint16_t mtu)
{
    static struct bt_gatt_exchange_params exchange_params;

    /* req mtu to max */
    exchange_params.func = exchange_func;
    bt_gatt_exchange_mtu((struct bt_conn *)conn, &exchange_params);

    return AIIO_OK;
}

aiio_err_t aiio_ble_mtu_get(aiio_ble_conn_t conn, uint16_t *mtu)
{
    *mtu = bt_gatt_get_mtu(conn);

    return AIIO_OK;
}

aiio_err_t aiio_ble_restart_advertising(void)
{
    if (set_adv_enable(1) != 0) {
        return AIIO_ERROR;
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_disconnect(aiio_ble_conn_t conn)
{
    bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);

    return AIIO_OK;
}

aiio_err_t aiio_ble_connect(uint8_t addr_type, uint8_t *addr, uint32_t timeout)
{
    struct bt_conn *conn;
    bt_addr_le_t target_addr;
    struct bt_le_conn_param conn_param;

    ble_conn_flag = 0;

    conn_param.interval_min = ble_config_s.conn_intv.min_interval;
    conn_param.interval_max = ble_config_s.conn_intv.max_interval;
    conn_param.latency = ble_config_s.conn_intv.latency;
    conn_param.timeout = ble_config_s.conn_intv.timeout;

    target_addr.type = addr_type;
    memcpy(target_addr.a.val, addr, 6);
    ble_reverse_byte(target_addr.a.val, 6);
    conn = bt_conn_create_le(&target_addr, &conn_param);
    if (!conn) {
        return AIIO_ERROR;
    }

    timeout /= 10;
    while (ble_conn_flag == 0) {
        timeout--;
        if (timeout == 0) {
            bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
            return AIIO_ERROR;
        }

        aiio_os_tick_dealy(aiio_os_ms2tick(10));
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_scan(const aiio_ble_scan_param_t *cfg)
{
    int ret;
    struct bt_le_scan_param scan_param;

    scan_param.type = cfg->type;
    scan_param.filter_dup = BT_LE_SCAN_FILTER_DUPLICATE;
    scan_param.interval = cfg->interval;
    scan_param.window = cfg->window;

    ret = bt_le_scan_start(&scan_param, scan_device_found);
    if (ret != 0) {
        return AIIO_ERROR;
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_scan_stop(void)
{
    int ret;

    ret = bt_le_scan_stop();
    if (ret != 0) {
        return AIIO_ERROR;
    }

    return AIIO_OK;
}

aiio_err_t aiio_ble_send(const aiio_ble_send_t *data)
{
    struct bt_gatt_attr *char_val;

    switch (data->type) {
        case AIIO_BLE_NOTIFY:
            ble_notify_data(data->conn, (struct bt_gatt_attr *)data->handle, data->data, data->length);
            break;
        case AIIO_BLE_WRITE_WITHOUT_RESP:
            ble_write_norsp_data(data->conn, (uint16_t)data->handle, data->data, data->length);
            break;
        default:
            return AIIO_ERROR;
    }

    return AIIO_OK;
}

static SemaphoreHandle_t sem_disc;
static uint8_t gatts_disc_func(struct bt_conn *conn,
                 const struct bt_gatt_attr *attr,
                 struct bt_gatt_discover_params *params)
{
    int err;
    struct bt_gatt_chrc *gatt_chrc;
    struct bt_gatt_service_val *gatt_service;

    if (!attr) {
        aiio_log_i("Discover complete\r\n");
        (void)memset(params, 0, sizeof(*params));
        if (sem_disc) {
            xSemaphoreGive(sem_disc);
        }
        goto stop;
    }

    if (ble_cb) {
        aiio_ble_evt_t evt;
        aiio_ble_gatt_attr_t aiio_attr;
        aiio_ble_uuid_128_t aiio_uuid = {0};

        if (params->type == BT_GATT_DISCOVER_CHARACTERISTIC) {
            gatt_chrc = attr->user_data;

            ble_uuid_convert_to_aiio(&aiio_uuid, gatt_chrc->uuid);
            aiio_attr.handle = gatt_chrc->value_handle;
        } else {
            ble_uuid_convert_to_aiio(&aiio_uuid, attr->uuid);
            aiio_attr.handle = attr->handle;
        }
        aiio_attr.uuid = &aiio_uuid;

        evt.type = AIIO_BLE_EVT_DISC;
        evt.disc.conn = (aiio_ble_conn_t)conn;
        evt.disc.attr = &aiio_attr;
        ble_cb(&evt);
    }

    return BT_GATT_ITER_CONTINUE;

stop:
    return BT_GATT_ITER_STOP;
}

aiio_err_t aiio_ble_discover(const aiio_ble_disc_param_t *param, uint16_t timeout)
{
    int err;
    int ret = AIIO_ERROR;
    struct bt_uuid_128 uuid_128;
    static struct bt_gatt_discover_params discover_params;

    sem_disc = xSemaphoreCreateBinary();
    if (!sem_disc) {
        aiio_log_e("[BLE] sem_disc fail\r\n");
        goto _exit;
    }

    if (param->uuid) {
        ble_uuid_convert(&uuid_128, param->uuid);
    }

    discover_params.uuid = param->uuid ? &uuid_128 : NULL;
    discover_params.func = gatts_disc_func;
    discover_params.start_handle = param->start_handle;
    discover_params.end_handle = param->end_handle;
    discover_params.type = param->type;

    err = bt_gatt_discover(param->conn, &discover_params);
    if (err) {
        aiio_log_e("[BLE] discover failed(err %d)\r\n", err);
        goto _exit;
    }

    if (xSemaphoreTake(sem_disc, timeout / portTICK_PERIOD_MS) != pdTRUE) {
        printf("[BLE] disc timeout\r\n");
        goto _exit;
    }

    ret = AIIO_OK;

_exit:
    if (sem_disc) {
        /* disable interrupt? */
        vSemaphoreDelete(sem_disc);
        sem_disc = NULL;
        /* enable interrupt? */
    }

    return ret;
}

aiio_err_t aiio_ble_set_name(const char *name)
{
    if (bt_set_name(name) != 0) {
        return AIIO_ERROR;
    }

    return AIIO_OK;
}
