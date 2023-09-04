#define __APP_BLE_C_
#include "app_ble.h"

TaskHandle_t app_ble_process_task_hd;
xQueueHandle ble_queue = NULL;

#define BLE_DEV_NAME "AXK_KB"

static struct bt_le_adv_param adv_param;
static char adv_name[] = BLE_DEV_NAME;
static uint16_t appearance = BLE_APPEARANCE_HID_KEYBOARD;
static uint16_t server_uuid = BLE_UUID_HID;
static struct bt_data salve_rsp[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, adv_name, sizeof adv_name),
};

static struct bt_data salve_adv[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL),
    BT_DATA(BT_DATA_GAP_APPEARANCE, &appearance, sizeof appearance),
    BT_DATA(BT_DATA_UUID16_ALL, &server_uuid, sizeof server_uuid),
    // BT_DATA(BT_DATA_MANUFACTURER_DATA, specific_data, 16),
};
static struct bt_conn* ble_conn_handle;

static void _connected(struct bt_conn* conn, u8_t err)
{
    uint16_t value = 0x01;
    if (conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    printf("[BLE] connected \r\n");
    ble_conn_handle = conn;

    // xQueueSend(ble_queue, &value, 0);

    // bt_conn_set_security(conn, BT_SECURITY_L3);

    return;
}

static void _disconnected(struct bt_conn* conn, u8_t reason)
{
    uint16_t value = 0x02;
    if (conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    printf("[BLE] disconnected, reason:%d \r\n", reason);
    ble_conn_handle = NULL;

    /* restart adv */
    printf("restart adv\r\n");
    int err = set_adv_enable(1);
    if (err != 0)
    {
        printf("set adv err:%d\r\n", err);
    }

    xQueueSend(ble_queue, &value, 0);
}

static bool _le_param_req(struct bt_conn* conn,
                          struct bt_le_conn_param* param)
{
    printf("[BLE] conn param request: int 0x%04x-0x%04x lat %d to %d \r\n",
           param->interval_min,
           param->interval_max,
           param->latency,
           param->timeout);

    return true;
}

static void _le_param_updated(struct bt_conn* conn, u16_t interval,
                              u16_t latency, u16_t timeout)
{
    printf("[BLE] conn param updated: int 0x%04x lat %d to %d \r\n", interval, latency, timeout);
}

static void security_changed(struct bt_conn* conn, bt_security_t level,
                             enum bt_security_err err)
{
    printf("[BLE] sec to level %u (err %u)\n", level, err);
}

static void _ble_mtu_changed_cb(struct bt_conn* conn, int mtu)
{
    printf("[BLE] mtu updated:%d \r\n", mtu);
}

static void bt_enable_cb(int err)
{
    if (!err)
    {
        bt_addr_le_t bt_addr;
        bt_get_local_public_address(&bt_addr);
        printf("BD_ADDR:(MSB)%02x:%02x:%02x:%02x:%02x:%02x(LSB) \r\n",
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

static struct bt_conn_cb conn_callbacks = {
    .connected = _connected,
    .disconnected = _disconnected,
    .le_param_req = _le_param_req,
    .le_param_updated = _le_param_updated,
    .le_phy_updated = NULL,
    .security_changed = security_changed,
};

static void ble_init()
{
    ble_stack_start();
    bt_gatt_register_mtu_callback(_ble_mtu_changed_cb);
    bt_conn_cb_register(&conn_callbacks);
    /* avoid callback infinite loop */
    conn_callbacks._next = NULL;
}

static void ble_kb_start()
{
    int err;

    bt_set_name(BLE_DEV_NAME);
    adv_param.id = 0;
    adv_param.interval_min = 100;
    adv_param.interval_max = 200;
    adv_param.options = (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_ONE_TIME);

    err = bt_le_adv_start(&adv_param, salve_adv, ARRAY_SIZE(salve_adv), salve_rsp, ARRAY_SIZE(salve_rsp));
    if (err)
    {
        printf("[BLE] adv fail(err %d) \r\n", err);
        return;
    }
}

int btblecontroller_em_config(void)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    volatile uint32_t em_size;

    em_size = (uint32_t)&__LD_CONFIG_EM_SEL;

    if (em_size == 0)
    {
        GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);
    }
    else if (em_size == 32 * 1024)
    {
        GLB_Set_EM_Sel(GLB_WRAM128KB_EM32KB);
    }
    else if (em_size == 64 * 1024)
    {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }
    else
    {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }

    return 0;
}

void test_task(void* msg)
{
    int8_t rssi;
    int8_t power_status = 0;
    int8_t count1 = 0;
    int8_t count2 = 0;
    uint16_t value = 0x01;

    while (1)
    {
        if (ble_conn_handle != NULL)
        {
            bt_le_read_rssi(ble_conn_handle->handle, &rssi);
            if ((rssi & 0xFF) > 0xC0)
            {
                count1++;
                count2 = 0;
                if (count1 > 1)
                {
                    value = 0x01;
                    count1 = 0;
                    power_status = 1;
                    xQueueSend(ble_queue, &value, 0);
                }
            }
            else
            {
                count2++;
                if (count2 > 3)
                {
                    value = 0x02;
                    count2 = 0;
                    count1 = 0;

                    if (power_status == 1)
                    {
                        xQueueSend(ble_queue, &value, 0);
                    }
                    power_status = 0;
                }
            }
            printf("rssi:0x%02x\r\n", rssi);
        }
        else
        {
            power_status = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_ble_task(void* msg)
{
    struct bflb_device_s* gpio;
    uint16_t ble_flag = 0;

    ble_queue = xQueueCreate(10, sizeof(uint32_t));

    btblecontroller_em_config();
    ble_init();
    bas_init();
    dis_init(0x01, 0x07AF, 0x707, 0x2A50);
    hog_kb_init();
    ble_kb_start();

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_18, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    //开启状态，默认息屏，如果想要开启亮屏，需使用 bflb_gpio_set 函数。
    bflb_gpio_reset(gpio, GPIO_PIN_18);

    xTaskCreate(test_task, (char*)"test_task", 1024, NULL, 14, NULL);

    while (1)
    {
        // vTaskDelay(pdMS_TO_TICKS(500));
        if (xQueueReceive(ble_queue, &ble_flag, portMAX_DELAY))
        {
            switch (ble_flag)
            {
                case 0x01:
                    bflb_gpio_set(gpio, GPIO_PIN_18);
                    break;
                case 0x02:
                    vTaskDelay(pdMS_TO_TICKS(3000));
                    bflb_gpio_reset(gpio, GPIO_PIN_18);
                    break;
                default:
                    break;
            }
        }
    }

    vTaskDelete(NULL);
}
