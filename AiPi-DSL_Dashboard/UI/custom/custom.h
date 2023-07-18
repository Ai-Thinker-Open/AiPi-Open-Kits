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

    // int src_home_digital_clock_1_hour_value = 11;
    // int src_home_digital_clock_1_min_value = 25;
    // int src_home_digital_clock_1_sec_value = 50;
    // int src_home_digital_date_yesr_value = 1970;
    // int src_home_digital_date_mont_value = 7;
    // int src_home_digital_date_day_value = 11;
    // int src_home_digital_date_wday_value = 2;


    // extern int src_home_digital_clock_1_hour_value;
    // extern int src_home_digital_clock_1_min_value;
    // extern int src_home_digital_clock_1_sec_value;
    // extern int src_home_digital_date_yesr_value;
    // extern int src_home_digital_date_mont_value;
    // extern int src_home_digital_date_day_value;
    // extern int src_home_digital_date_wday_value;
#include "gui_guider.h"
#define SSID_KEY "SSID"
#define PASS_KEY "PASS"

    typedef struct {
        char city[16];
        char wea[16];
        char tem_day[2];
    } weather_t;



    void custom_init(lv_ui* ui);
    void flash_erase_set(char* key, char* value);
    char* flash_get_data(char* key, uint32_t len);
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
