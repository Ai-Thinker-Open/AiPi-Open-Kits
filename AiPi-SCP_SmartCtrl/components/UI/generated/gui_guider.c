/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"

static mqtt_rgb_dev_t ai_wb2;
static mqtt_rgb_dev_t ai_m62;
static mqtt_rgb_dev_t ai_bw16;
void ui_init_style(lv_style_t* style)
{
	if (style->prop_cnt > 1)
		lv_style_reset(style);
	else
		lv_style_init(style);
}

void init_scr_del_flag(lv_ui* ui)
{
	ui->screen_del = true;
	ui->screen_rgb1_del = true;
	ui->screen_rgb2_del = true;
	ui->screen_rgb3_del = true;
}

void setup_ui(lv_ui* ui)
{
	ui->ai_wb2_dev = &ai_wb2;
	ui->ai_m62_dev = &ai_m62;
	ui->bw16_dev = &ai_bw16;
	init_scr_del_flag(ui);
	setup_scr_screen(ui);
	lv_scr_load(ui->screen);

}
