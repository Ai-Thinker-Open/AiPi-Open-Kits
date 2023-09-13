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

void hid_keyboard_init(void);
void usb_hid_keyboard_send(char Keyboard_ctrl, char keybord_key);
void usb_hid_keyboard_setSleep(void);
void usb_hid_keyboard_setWakeup(void);
#endif