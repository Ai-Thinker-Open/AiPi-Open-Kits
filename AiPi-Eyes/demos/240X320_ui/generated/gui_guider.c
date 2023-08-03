/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void ui_init_style(lv_style_t* style)
{
	if (style->prop_cnt > 1)
		lv_style_reset(style);
	else
		lv_style_init(style);
}

void init_scr_del_flag(lv_ui* ui)
{
	ui->cont_4_del = true;
	ui->WiFi_config_del = true;
	ui->screen_type = true;
}

void setup_ui(lv_ui* ui)
{
	init_scr_del_flag(ui);
	setup_scr_cont_4(ui);
	lv_scr_load(ui->cont_4);
}
