/**
 * @file hid_keyboard.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef HID_KEYBOARD_H
#define HID_KEYBOARD_H

typedef enum {
    KB_NUMBLE_1 = 0,
    KB_NUMBLE_2,
    KB_NUMBLE_3,
    KB_NUMBLE_4,
    KB_NUMBLE_5,
    KB_NUMBLE_6,
    KB_NUMBLE_7,
    KB_NUMBLE_8,
    KB_NUMBLE_9,
    KB_NUMBLE_0,
}keyboard_numble_t;

void hid_keyboard_init(void);
void usb_hid_keyboard_send(char Keyboard_ctrl, char keybord_key);
#if 1
void usb_hid_keyboard_setSleep(void);
void usb_hid_keyboard_setWakeup(void);
void usb_hid_keyboard_stop_send(void);
#endif
void usb_hid_keyboard_inputpassword(char* PIN);
void usb_hid_keyboard_lock(void);
#endif