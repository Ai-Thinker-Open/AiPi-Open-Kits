/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "wifi_event.h"

void ui_init_style(lv_style_t* style)
{
	if (style->prop_cnt > 1)
		lv_style_reset(style);
	else
		lv_style_init(style);
}

void init_scr_del_flag(lv_ui* ui)
{
	ui->Home_del = true;
}

void setup_ui(lv_ui* ui)
{
	init_scr_del_flag(ui);
	ui->timerout = atoi(flash_get_data("TIMER", 16));
	strcpy(ui->PIN, flash_get_data("PIN", 64));
	ui->keyboard_con_type = atoi(flash_get_data("HID_CON", 8));
	setup_scr_Home(ui);
	lv_scr_load(ui->Home);
}
