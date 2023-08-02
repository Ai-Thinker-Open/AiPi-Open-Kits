/**
 * @file ble_hid_dev.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-17
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "queue.h"
#include "bluetooth.h"
#include "hci_driver.h"
#include "hci_core.h"
#include "btble_lib_api.h"
#include "conn.h"
#include "conn_internal.h"
#include "gatt.h"
#include "bl_port.h"
#include "bl616_glb.h"

#include "bas.h"
#include "dis.h"
#include "hog_kb.h"
#include "log.h"
#include "ble_hid_dev.h"
#include "custom.h"
#define GBD_TAG "BLE HID"

#define BLE_DEV_NAME "AiPi-Dashboard"

#define BLE_USE_WINDOWS_BEACON 

QueueHandle_t ble_hid_queue;
xQueueHandle ble_queue;
extern xQueueHandle queue;
static struct bt_le_adv_param adv_param;
static char adv_name[] = BLE_DEV_NAME;
static uint16_t appearance = BLE_APPEARANCE_HID_KEYBOARD;
static uint16_t server_uuid = BLE_UUID_HID;
static struct bt_conn* ble_conn_handle;

static struct bt_data salve_rsp[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, adv_name, sizeof adv_name),
};

#ifdef BLE_USE_WINDOWS_BEACON
static uint8_t windows_pair_beacon[] = {
    0x06, /* Microsoft Vendor ID */
    0x00, /* Microsoft Vendor ID */
    0x03, /* Microsoft Beacon ID */
    0x00, /* Microsoft Beacon Sub Scenario */
    0x80, /* Reserved RSSI Byte */
};
#endif

static struct bt_data salve_adv[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL),
    BT_DATA(BT_DATA_GAP_APPEARANCE, &appearance, sizeof appearance),
    BT_DATA(BT_DATA_UUID16_ALL, &server_uuid, sizeof server_uuid),
    // BT_DATA(BT_DATA_MANUFACTURER_DATA, specific_data, 16),
#ifdef BLE_USE_WINDOWS_BEACON
    BT_DATA(BT_DATA_MANUFACTURER_DATA, windows_pair_beacon, sizeof windows_pair_beacon),
#endif
};

/**
 * @brief BLE connect callback
 *
 * @param conn
 * @param err
*/
static void _connected(struct bt_conn* conn, u8_t err)
{
    char* ble_status_msg = pvPortMalloc(128);
    if (conn->type != BT_CONN_TYPE_LE) {
        return;
    }

    LOG_I("[BLE] connected \r\n");
    ble_conn_handle = conn;
    // bt_conn_set_security(conn, BT_SECURITY_L3);
    sprintf(ble_status_msg, "{\"BLE_HID\":{\"status\":%d}}", BLE_STATUS_CONNECT);

    xQueueSend(ble_queue, ble_status_msg, portMAX_DELAY);
    vPortFree(ble_status_msg);
    return;
}
/**
 * @brief BLE disconnect callback
 *
 * @param conn
 * @param reason
*/
static void _disconnected(struct bt_conn* conn, u8_t reason)
{
    char* ble_status_msg = pvPortMalloc(128);
    if (conn->type != BT_CONN_TYPE_LE) {
        return;
    }

    LOG_I("[BLE] disconnected, reason:%d ", reason);
    ble_conn_handle = NULL;

    /* restart adv */
    LOG_I("restart adv");
    int err = set_adv_enable(1);
    if (err != 0) {
        LOG_E("set adv err:%d", err);
    }
    sprintf(ble_status_msg, "{\"BLE_HID\":{\"status\":%d}}", BLE_STATUS_DISCONNECT);

    xQueueSend(ble_queue, ble_status_msg, portMAX_DELAY);
    vPortFree(ble_status_msg);
}
/**
 * @brief _le_param_req
 *    参数请求回调
 * @param conn
 * @param param
 * @return true
 * @return false
*/
static bool _le_param_req(struct bt_conn* conn, struct bt_le_conn_param* param)
{
    LOG_I("[BLE] conn param request: int 0x%04x-0x%04x lat %d to %d",
            param->interval_min,
            param->interval_max,
            param->latency,
            param->timeout);

    return true;
}
/**
 * @brief _le_param_updated
 *          参数更新回调
 * @param conn
 * @param interval
 * @param latency
 * @param timeout
*/
static void _le_param_updated(struct bt_conn* conn, u16_t interval, u16_t latency, u16_t timeout)
{
    LOG_I("[BLE] conn param updated: int 0x%04x lat %d to %d ", interval, latency, timeout);
}
/**
 * @brief security_changed
 *
 * @param conn
 * @param level
 * @param err
*/
static void security_changed(struct bt_conn* conn, bt_security_t level, enum bt_security_err err)
{
    LOG_I("[BLE] sec to level %u (err %u)", level, err);
}
/**
 * @brief _ble_mtu_changed_cb
 *      MTU 更新回调
 * @param conn
 * @param mtu
*/
static void _ble_mtu_changed_cb(struct bt_conn* conn, int mtu)
{
    printf("[BLE] mtu updated:%d \r\n", mtu);
}
/**
 * @brief BLE 启动回调
 *
 * @param err
*/
static void bt_enable_cb(int err)
{
    char* ble_status_msg = pvPortMalloc(128);
    memset(ble_status_msg, 0, 128);
    if (!err) {
        bt_addr_le_t bt_addr;
        bt_get_local_public_address(&bt_addr);
        LOG_I("BD_ADDR:(MSB)%02x:%02x:%02x:%02x:%02x:%02x(LSB)",
            bt_addr.a.val[5], bt_addr.a.val[4], bt_addr.a.val[3], bt_addr.a.val[2], bt_addr.a.val[1], bt_addr.a.val[0]);
        //向界面发送状态
        sprintf(ble_status_msg, "{\"BLE_HID\":{\"status\":%d,\"MAC\":\"%02x:%02x:%02x:%02x:%02x:%02x\"}}", BLE_STATUS_ENABLE,
        bt_addr.a.val[5], bt_addr.a.val[4], bt_addr.a.val[3], bt_addr.a.val[2], bt_addr.a.val[1], bt_addr.a.val[0]);

        xQueueSend(ble_queue, ble_status_msg, portMAX_DELAY);
    }
    vPortFree(ble_status_msg);
}
/**
 * @brief BLE 启动
 *
*/
static void ble_stack_start(void)
{
    // Initialize BLE controller
    btble_controller_init(configMAX_PRIORITIES - 1);
    // Initialize BLE Host stack
    hci_driver_init();
    bt_enable(bt_enable_cb);
    LOG_I("BLE HID start success");
}
/**
 * @brief 设备配置
 *
*/
static struct bt_conn_cb conn_callbacks = {
    .connected = _connected,
    .disconnected = _disconnected,
    .le_param_req = _le_param_req,
    .le_param_updated = _le_param_updated,
    .le_phy_updated = NULL,
    .security_changed = security_changed,
};
/**
 * @brief BLE 初始化
 *
*/
static void ble_init()
{
    ble_stack_start();
    bt_gatt_register_mtu_callback(_ble_mtu_changed_cb);
    bt_conn_cb_register(&conn_callbacks);
    /* avoid callback infinite loop */
    conn_callbacks._next = NULL;
}
/**
 * @brief BLE 键盘启动
 *
*/
static void ble_kb_start()
{
    int err;

    bt_set_name(BLE_DEV_NAME);
    adv_param.id = 0;
    adv_param.interval_min = 100;
    adv_param.interval_max = 200;
    adv_param.options = (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_ONE_TIME);

    err = bt_le_adv_start(&adv_param, salve_adv, ARRAY_SIZE(salve_adv), salve_rsp, ARRAY_SIZE(salve_rsp));
    if (err) {
        LOG_I("[BLE] adv fail(err %d) \r\n", err);
        return;
    }
}
/**
 * @brief 内存分配
 *
 * @return int
*/
static int btblecontroller_em_config(void)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    volatile uint32_t em_size;

    em_size = (uint32_t)&__LD_CONFIG_EM_SEL;
    LOG_I("__LD_CONFIG_EM_SEL=%d", em_size);
    if (em_size == 0) {
        GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);
    }
    else if (em_size == 32*1024) {
        GLB_Set_EM_Sel(GLB_WRAM128KB_EM32KB);
    }
    else if (em_size == 64*1024) {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }
    else {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }

    return 0;
}

static void ble_hid_dev_send(hid_key_num_t key_num)
{

    if (ble_conn_handle==NULL) {
        LOG_E("ble is disconnect");
        return;
    }

    uint8_t key_vaule[] = { 0x00 };
    switch (key_num)
    {
        case HID_KEY_NUMBLE_A:
        {
            // Crtl+ALT+A
            key_vaule[0] = KEY_NUMBLE_A;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);

            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_B:
        {
            //Crtl+ALT+B
            key_vaule[0] = KEY_NUMBLE_B;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_C:
        {
            key_vaule[0] = KEY_NUMBLE_C;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_D:
        {
            key_vaule[0] = KEY_NUMBLE_D;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_E:
        {
            key_vaule[0] = KEY_NUMBLE_E;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_F:
        {
            key_vaule[0] = KEY_NUMBLE_F;
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, NULL, 0);
            vTaskDelay(50/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, LEFT_CONTROL_KEY_MASK|LEFT_ALT_KEY_MASK, key_vaule, sizeof key_vaule);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%2X 0X%02X 0X%02X", LEFT_CONTROL_KEY_MASK, LEFT_ALT_KEY_MASK, key_vaule[0]);
        }
        break;
        case HID_KEY_NUMBLE_VOL_UP:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, VOLUME_INCREMENT, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_plus_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%02X", VOLUME_INCREMENT);
        }
        break;
        case HID_KEY_NUMBLE_VOL_DOWN:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, VOLUME_DECREMENT, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_plus_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%02X", VOLUME_DECREMENT);
        }
        break;
        case HID_KEY_NUMBLE_VOL_MUTE:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, VOLUME_MUTE, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_plus_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%02X", VOLUME_MUTE);
        }
        break;

        case HID_KEY_NUMBLE_MISIC_STOP:
        case HID_KEY_NUMBLE_MISIC_PLAY:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, MISIC_PLAY_STOP, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);
            hog_send_keyboard_plus_value(ble_conn_handle, 0, NULL, 0);

            LOG_I("HID SEND:0X%02X", MISIC_PLAY_STOP);
        }
        break;
        case HID_KEY_NUMBLE_MISIC_NEXT:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, MISIC_NEXT, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);

            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%02X", MISIC_NEXT);
        }
        break;
        case HID_KEY_NUMBLE_MISIC_PAST:
        {
            hog_send_keyboard_plus_value(ble_conn_handle, MISIC_PAST, NULL, 0);
            vTaskDelay(100/portTICK_RATE_MS);

            hog_send_keyboard_value(ble_conn_handle, 0, NULL, 0);
            LOG_I("HID SEND:0X%02X", MISIC_PAST);
        }
        break;
        default:
            break;
    }
}

/**
 * @brief HID 任务
 *
 * @param arg
*/
void ble_hid_task(void* arg)
{
    ble_queue = xQueueCreate(1, 512);
    xTaskCreate(queue_receive_ble_task, "queue_ble_task", 1024, arg, 7, NULL);
    vTaskDelay(1000/portTICK_RATE_MS);
    hid_key_num_t kb_num;
    btblecontroller_em_config();
    ble_init();
    bas_init();
    dis_init(0x01, 0x07AF, 0x707, 0x2A50);
    hog_kb_init();
    ble_kb_start();
    ble_hid_queue = xQueueCreate(1, 4);
    while (1) {
        xQueueReceive(ble_hid_queue, &kb_num, portMAX_DELAY);
        ble_hid_dev_send(kb_num);
    }
}

