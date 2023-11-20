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

#define FLASH_SSID_KEY "SSID"
#define FLASH_PWD_KEY "PASS"
#define FLASH_MQTT_IP "mqip"
#define FLASH_MQTT_TOKEN "token"
#define FLASH_MQTT_PORT "port"

typedef enum {
    AIPI_R286_STATE_SYS_START = 0,
    AIPI_R286_STATE_CONFIG_START,
    AIPI_R286_STATE_CONFIG_OK,
    AIPI_R286_STATE_CONFIG_ERR,
    AIPI_R286_STATE_WIFI_CONNECT_OK,
    AIPI_R286_STATE_WIFI_CONFIG_ERR,
    AIPI_R286_STATE_SERVER_CONNECT_OK,
    AIPI_R286_STATE_SERVER_CONFIG_ERR,
    AIPI_R286_STATE_SERVER_DATA_DOWN,
    AIPI_R286_STATE_SYS_TIME_OK,
    AIPI_R286_STATE_DEV_REALAY1_ON,
    AIPI_R286_STATE_DEV_REALAY2_ON,
    AIPI_R286_STATE_DEV_REALAY3_ON,
    AIPI_R286_STATE_DEV_REALAY1_OFF,
    AIPI_R286_STATE_DEV_REALAY2_OFF,
    AIPI_R286_STATE_DEV_REALAY3_OFF,
    AIPI_R286_STATE_DEV_REALAYALL_ON,
    AIPI_R286_STATE_DEV_REALAYALL_OFF,

}aipi_r286_state_t;

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
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
}dev_date_t;
/**
 * @brief
 *
*/
typedef struct relays_dev {
    char* relays_name;
    uint8_t gpio_pin;
    bool gpio_state;
    void (*on)(struct relays_dev*);
    void (*off)(struct relays_dev*);
}relays_dev_t;

typedef enum {
    AIPI_R286_LANGUAGE_ZH = 0,
    AIPI_R286_LANGUAGE_EN,
}r286_sys_language_t;
/**
 * @brief
 *
*/
typedef struct device_t {
    lv_ui* ui;
    wifi_sta_t wifi_sta;
    dev_date_t date;
    relays_dev_t* relay_sw1;
    relays_dev_t* relay_sw2;
    relays_dev_t* relay_sw3;
    char* mqttip;
    char* token;
    uint16_t port;
    char mq_data[512];
    r286_sys_language_t  language;
}aipi_r286_dev_t;


extern aipi_r286_dev_t* r2_device;
extern uint16_t yesr;
extern uint16_t mon;
extern uint16_t day;
extern uint16_t wday;
extern uint16_t hour;
extern uint16_t min;
extern uint16_t sec;
aipi_r286_dev_t* aipi_r286_device_init(void);
void aipi_r286_dev_state_update(aipi_r286_state_t new_state);
void flash_erase_set(char* key, char* value);
char* flash_get_data(char* key, int len);
void flash_erase_data(char* key);
int AiPi_R286_get_mqtt_control(const char* payload, uint8_t* pid);
#endif