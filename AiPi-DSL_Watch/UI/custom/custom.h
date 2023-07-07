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

    void custom_init(lv_ui* ui);
    void flash_erase_set(char* key, char* value);
    char* flash_get_data(char* key, uint32_t len);
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
