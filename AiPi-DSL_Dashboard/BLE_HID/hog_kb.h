#ifndef _HOG_KB_H_
#define _HOG_KB_H_

#define HID_PAGE_KBD   0x07
#define HID_PAGE_CONS  0x0C

// HID keyboard input report length
#define HID_KEYBOARD_IN_RPT_LEN     8

#define BLE_APPEARANCE_HID_KEYBOARD 0x03C1
#define BLE_UUID_HID 0x1812

#define BT_CHAR_BLE_HID_INFO_ATTR_VAL_INDEX                 (2)
#define BT_CHAR_BLE_HID_REPORT_MAP_ATTR_VAL_INDEX           (4)
#define BT_CHAR_BLE_HID_REPORT_ATTR_VAL_INDEX               (6)
#define BT_CHAR_BLE_HID_CTRL_POINT_ATTR_VAL_INDEX           (10)

#define LEFT_CONTROL_KEY_MASK        (1 << 0)
#define LEFT_SHIFT_KEY_MASK          (1 << 1)
#define LEFT_ALT_KEY_MASK            (1 << 2)
#define LEFT_GUI_KEY_MASK            (1 << 3)
#define RIGHT_CONTROL_KEY_MASK       (1 << 4)
#define RIGHT_SHIFT_KEY_MASK         (1 << 5)
#define RIGHT_ALT_KEY_MASK           (1 << 6)
#define RIGHT_GUI_KEY_MASK           (1 << 7)

typedef uint8_t key_mask_t;

void hog_kb_init(void);
int hog_send_keyboard_value(struct bt_conn *conn, key_mask_t special_key_mask, uint8_t *keyboard_cmd, uint8_t num_key);

#endif
