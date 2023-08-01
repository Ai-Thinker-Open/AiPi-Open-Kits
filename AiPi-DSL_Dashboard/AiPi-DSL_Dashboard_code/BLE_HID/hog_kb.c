#include <string.h>
#include <sys/errno.h>
#include <byteorder.h>
#include <zephyr.h>

#include <bluetooth.h>
#include <conn.h>
#include <uuid.h>
#include <gatt.h>
#include "hog_kb.h"
#include "log.h"

enum {
    HIDS_REMOTE_WAKE = BIT(0),
    HIDS_NORMALLY_CONNECTABLE = BIT(1),
};

struct hids_info {
    uint16_t version; /* version number of base USB HID Specification */
    uint8_t code;     /* country HID Device hardware is localized for. */
    uint8_t flags;
} __packed;

struct hids_report {
    uint8_t id; /* report id */
    uint8_t type; /* report type */
} __packed;

static struct hids_info info = {
    .version = 0x0111,    /* USB HID version BCD 0x0111 */
    .code = 0x00,
    .flags = HIDS_NORMALLY_CONNECTABLE,
};

enum {
    HIDS_INPUT = 0x01,
    HIDS_OUTPUT = 0x02,
    HIDS_FEATURE = 0x03,
};

static struct hids_report kb_input = {
    .id = 0x00,
    .type = HIDS_INPUT,
};

static uint8_t hid_input_ccc;
static uint8_t ctrl_point;
static uint8_t report_map[] =
{
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x06,       // Usage (Keyboard)
    0xA1, 0x01,       // Collection (Application)
    0x05, 0x07,       //   Usage Page (Key Codes)
    0x19, 0xe0,       //   Usage Minimum (224)
    0x29, 0xe7,       //   Usage Maximum (231)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x08,       //   Report Count (8)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)

    0x95, 0x01,       //   Report Count (1)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x01,       //   Input (Constant) reserved byte(1)

    // 0x95, 0x05,       //   Report Count (5)
    // 0x75, 0x01,       //   Report Size (1)
    // 0x05, 0x08,       //   Usage Page (Page# for LEDs)
    // 0x19, 0x01,       //   Usage Minimum (1)
    // 0x29, 0x05,       //   Usage Maximum (5)
    // 0x91, 0x02,       //   Output (Data, Variable, Absolute), Led report
    // 0x95, 0x01,       //   Report Count (1)
    // 0x75, 0x03,       //   Report Size (3)
    // 0x91, 0x01,       //   Output (Data, Variable, Absolute), Led report padding

    0x95, 0x06,       //   Report Count (6)
    0x75, 0x08,       //   Report Size (8)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x65,       //   Logical Maximum (101)
    0x05, 0x07,       //   Usage Page (Key codes)
    0x19, 0x00,       //   Usage Minimum (0)
    0x29, 0x65,       //   Usage Maximum (101)
    0x81, 0x00,       //   Input (Data, Array) Key array(6 bytes)

    // 0x09, 0x05,       //   Usage (Vendor Defined)
    // 0x15, 0x00,       //   Logical Minimum (0)
    // 0x26, 0xFF, 0x00, //   Logical Maximum (255)
    // 0x75, 0x08,       //   Report Size (8 bit)
    // 0x95, 0x02,       //   Report Count (2)
    // 0xB1, 0x02,       //   Feature (Data, Variable, Absolute)

    0xC0              // End Collection (Application)
};

#define TUD_HID_REPORT_DESC_CONSUMER(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_CONSUMER    )              ,\
  HID_USAGE      ( HID_USAGE_CONSUMER_CONTROL )              ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )              ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_LOGICAL_MIN  ( 0x00                                ) ,\
    HID_LOGICAL_MAX_N( 0x03FF, 2                           ) ,\
    HID_USAGE_MIN    ( 0x00                                ) ,\
    HID_USAGE_MAX_N  ( 0x03FF, 2                           ) ,\
    HID_REPORT_COUNT ( 1                                   ) ,\
    HID_REPORT_SIZE  ( 16                                  ) ,\
    HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END \

static ssize_t read_info(struct bt_conn* conn,
              const struct bt_gatt_attr* attr, void* buf,
              uint16_t len, uint16_t offset)
{
    printf("read_info:%d \r\n", len);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data,
                 sizeof(struct hids_info));
}

static ssize_t read_report_map(struct bt_conn* conn,
                   const struct bt_gatt_attr* attr, void* buf,
                   uint16_t len, uint16_t offset)
{
    printf("read_report_map:%d \r\n", len);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, report_map,
                 sizeof(report_map));
}

static ssize_t read_report(struct bt_conn* conn,
               const struct bt_gatt_attr* attr, void* buf,
               uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data,
                 sizeof(struct hids_report));
}

static void input_ccc_changed(const struct bt_gatt_attr* attr, uint16_t value)
{
    hid_input_ccc = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
    printf("hid_input_ccc = [%d]\r\n", hid_input_ccc);
}

static ssize_t read_input_report(struct bt_conn* conn,
                 const struct bt_gatt_attr* attr, void* buf,
                 uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, NULL, 0);
}

static ssize_t write_ctrl_point(struct bt_conn* conn,
                const struct bt_gatt_attr* attr,
                const void* buf, uint16_t len, uint16_t offset,
                uint8_t flags)
{
    uint8_t* value = attr->user_data;

    if (offset + len > sizeof(ctrl_point)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);

    return len;
}

/* HID Service Declaration */
static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HIDS),

    BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_INFO, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_info, NULL, &info),

    BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT_MAP, BT_GATT_CHRC_READ,
                   BT_GATT_PERM_READ, read_report_map, NULL, NULL),

    BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_REPORT,
                   BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                   BT_GATT_PERM_READ,
                   read_input_report, NULL, NULL),
    BT_GATT_CCC(input_ccc_changed,
            BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_DESCRIPTOR(BT_UUID_HIDS_REPORT_REF, BT_GATT_PERM_READ,
               read_report, NULL, &kb_input),

    BT_GATT_CHARACTERISTIC(BT_UUID_HIDS_CTRL_POINT,
                   BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                   BT_GATT_PERM_WRITE,
                   NULL, write_ctrl_point, &ctrl_point),
};

int hog_send_keyboard_value(struct bt_conn* conn, key_mask_t special_key_mask, uint8_t* keyboard_cmd, uint8_t num_key)
{
    struct bt_gatt_attr* attr;

    uint8_t buffer[HID_KEYBOARD_IN_RPT_LEN] = { 0 };

    // if (hid_input_ccc != BT_GATT_CCC_NOTIFY) {
    //     printf("input ccc invaild \r\n");
    //     return -EPERM;
    // }

    buffer[0] = special_key_mask;
    for (int i = 0; i < num_key; i++) {
        buffer[i + 2] = keyboard_cmd[i];
    }

    attr = &attrs[BT_CHAR_BLE_HID_REPORT_ATTR_VAL_INDEX];

    // printf("dump:%02x %02x %02x %02x %02x %02x %02x %02x \r\n", 
    //         buffer[0], buffer[1], buffer[2], buffer[3], 
    //         buffer[4], buffer[5], buffer[6], buffer[7]);
    return bt_gatt_notify(conn, attr, buffer, HID_KEYBOARD_IN_RPT_LEN);
}

struct bt_gatt_service hog_srv = BT_GATT_SERVICE(attrs);

void hog_kb_init(void)
{
    bt_gatt_service_register(&hog_srv);
}
