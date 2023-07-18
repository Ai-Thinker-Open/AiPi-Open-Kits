/**
 * @file ble_hid_dev.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-17
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef BLE_HID_DEV_H
#define BLE_HID_DEV_H
typedef enum {
    BLE_STATUS_ENABLE = 0,
    BLE_STATUS_CONNECT,
    BLE_STATUS_DISCONNECT,

    BLE_STATUS_DATA_ERR,
}ble_status_t;

typedef  enum {
    HID_KEY_NUMBLE_NONE = 0,
    HID_LEFT_CONTROL_KEY,
    HID_LEFT_SHIFT_KEY,
    HID_LEFT_ALT_KEY,
    HID_LEFT_GUI_KEY,
    HID_RIGHT_CONTROL,
    HID_RIGHT_SHIFT_KEY,
    HID_RIGHT_ALT_KEY,
    HID_RIGHT_GUI_KEY,
    //
    HID_KEY_NUMBLE_A,
    HID_KEY_NUMBLE_B,
    HID_KEY_NUMBLE_C,
    HID_KEY_NUMBLE_D,
    HID_KEY_NUMBLE_E,
    HID_KEY_NUMBLE_F,
    HID_KEY_NUMBLE_G,
    //
    HID_KEY_NUMBLE_VOL_CHENG,
    HID_KEY_NUMBLE_VOL_UP,
    HID_KEY_NUMBLE_VOL_DOWN,
    HID_KEY_NUMBLE_VOL_MUTE,
}hid_key_num_t;

void ble_hid_task(void* arg);
#endif