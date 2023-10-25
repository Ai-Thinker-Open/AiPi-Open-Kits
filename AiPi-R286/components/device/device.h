/**
 * @file device.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef DEVICE_H
#define DEVICE_H
#include "log.h"
//wifi
#include "easyflash.h"
#include "bflb_mtd.h"
#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "wifi_event.h"
//lvgl
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lcd.h"
//GuiGuider UI
#include "gui_guider.h"
#include "lv_user_config.h"
#include "custom.h"
#include "events_init.h"
#include "bflb_gpio.h"
#include "relays_dev.h"
/**
 * @brief
 *
*/
typedef struct device_wifi {
    char* ssid;
    char* passworld;
    char* ip_addrs;
    bool state;
}wifi_sta_t;
/**
 * @brief
 *
*/
typedef struct device_date {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t wday;
    char* wday_str;
}dev_date_t;
/**
 * @brief
 *
*/
typedef struct relays_dev {
    char* relays_name;
    uint8_t gpio_pin;
    void (*on)(struct relays_dev*);
    void (*off)(struct relays_dev*);
}relays_dev_t;
/**
 * @brief
 *
*/
typedef struct device_t {
    lv_ui* ui;
    wifi_sta_t* wifi_sta;
    dev_date_t* date;
    relays_dev_t* relay_sw1;
    relays_dev_t* relay_sw2;
    relays_dev_t* relay_sw3;
}aipi_r286_dev_t;

extern aipi_r286_dev_t* r2_device;

aipi_r286_dev_t* aipi_r286_device_init(void);

#endif