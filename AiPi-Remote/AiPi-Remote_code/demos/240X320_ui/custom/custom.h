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
#define SSID_KEY "SSID"
#define PASS_KEY "PASS"

    typedef struct {
        char city[16];
        char wea[16];
        char tem_day[2];
    } weather_t;
    extern int cont_4_digital_clock_1_hour_value;
    extern int cont_4_digital_clock_1_min_value;
    extern int cont_4_digital_clock_1_sec_value;
    extern int cont_4_lable_1_yers;
    extern int cont_4_lable_1_month;
    extern int cont_4_lable_1_day;
    extern int cont_4_lable_1_wday;
    extern weather_t weathers[4];
    void custom_init(lv_ui* ui);
    void flash_erase_set(char* key, char* value);
    char* flash_get_data(char* key, uint32_t len);
    char* compare_wea_output_img_100x100(const char* weather_data);
    char* compare_wea_output_img_20x20(const char* weather_data);
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
