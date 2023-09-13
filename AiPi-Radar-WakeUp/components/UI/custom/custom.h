// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/*
 * custom.h
 *
 *  Created on: July 29, 2020
 *      Author: nxf53801
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
    extern void* custom_status_task;

    typedef enum {
        CUSTOM_STATE_NONE = 0,
        CUSTOM_STATE_WIFI_START_CONNECT,
        CUSTOM_STATE_WIFI_IP,
        CUSTOM_STATE_WIFI_OK,
        CUSTOM_STATE_WIFI_DISCONNECT,
        CUSTOM_STATE_WIFI_SCAN,
        CUSTOM_STATE_WIFI_SCAN_DONE,
        CUSTOM_STATE_WEATHRE_GET,
        CUSTOM_STATE_USB_SLEEP,
        CUSTOM_STATE_USB_WAKEUP,
        CUSTOM_STATE_USB_KEY,
        CUSTOM_STATE_RADAR_DET,
        CUSTOM_STATE_RADAR_NDET,
    }custom_state_t;

    void custom_init(lv_ui* ui);
    void flash_erase_set(char* key, char* value);
    char* flash_get_data(char* key, uint32_t len);
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
