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
	ui->screen_del = true;
	ui->config_del = true;
}

void setup_ui(lv_ui* ui)
{
	init_scr_del_flag(ui);
	setup_scr_screen(ui);
	lv_scr_load(ui->screen);
}

void Create_tips(lv_ui* ui, char* value)
{
	static const char* config_msgbox_1_btns[] = { "" };
	ui->config_msgbox_1 = lv_msgbox_create(ui->config, "提示", value, config_msgbox_1_btns, true);
	lv_obj_set_scrollbar_mode(ui->config_msgbox_1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_pos(ui->config_msgbox_1, 60, 133);
	lv_obj_set_size(ui->config_msgbox_1, 360, 184);

	//Set style for config_msgbox_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_msgbox_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_msgbox_1, lv_color_make(0x24, 0x22, 0x2E), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_msgbox_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_msgbox_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_msgbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_msgbox_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_msgbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_msgbox_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_msgbox_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_1_extra_title_main_default
	static lv_style_t style_config_msgbox_1_extra_title_main_default;
	ui_init_style(&style_config_msgbox_1_extra_title_main_default);
	lv_style_set_text_color(&style_config_msgbox_1_extra_title_main_default, lv_color_make(0xa6, 0xa6, 0xa6));
	lv_style_set_text_font(&style_config_msgbox_1_extra_title_main_default, &lv_font_MiSans_Demibold_20);
	lv_style_set_text_letter_space(&style_config_msgbox_1_extra_title_main_default, 0);
	lv_style_set_text_line_space(&style_config_msgbox_1_extra_title_main_default, 30);
	lv_obj_add_style(lv_msgbox_get_title(ui->config_msgbox_1), &style_config_msgbox_1_extra_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_1_extra_content_main_default
	static lv_style_t style_config_msgbox_1_extra_content_main_default;
	ui_init_style(&style_config_msgbox_1_extra_content_main_default);
	lv_style_set_text_color(&style_config_msgbox_1_extra_content_main_default, lv_color_make(0xa8, 0xa8, 0xa8));
	lv_style_set_text_font(&style_config_msgbox_1_extra_content_main_default, &lv_font_MiSans_Demibold_18);
	lv_style_set_text_letter_space(&style_config_msgbox_1_extra_content_main_default, 0);
	lv_style_set_text_line_space(&style_config_msgbox_1_extra_content_main_default, 10);
	lv_obj_add_style(lv_msgbox_get_text(ui->config_msgbox_1), &style_config_msgbox_1_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_1_extra_btns_items_default
	static lv_style_t style_config_msgbox_1_extra_btns_items_default;
	ui_init_style(&style_config_msgbox_1_extra_btns_items_default);
	lv_style_set_radius(&style_config_msgbox_1_extra_btns_items_default, 10);
	lv_style_set_bg_color(&style_config_msgbox_1_extra_btns_items_default, lv_color_make(0x25, 0xa9, 0xee));
	lv_style_set_bg_grad_color(&style_config_msgbox_1_extra_btns_items_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_bg_grad_dir(&style_config_msgbox_1_extra_btns_items_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_config_msgbox_1_extra_btns_items_default, 255);
	lv_style_set_border_color(&style_config_msgbox_1_extra_btns_items_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_config_msgbox_1_extra_btns_items_default, 0);
	lv_style_set_border_opa(&style_config_msgbox_1_extra_btns_items_default, 255);
	lv_style_set_text_color(&style_config_msgbox_1_extra_btns_items_default, lv_color_make(0xf7, 0xf7, 0xf7));
	lv_style_set_text_font(&style_config_msgbox_1_extra_btns_items_default, &lv_font_MiSans_Demibold_12);
	lv_obj_add_style(lv_msgbox_get_btns(ui->config_msgbox_1), &style_config_msgbox_1_extra_btns_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_size(lv_msgbox_get_btns(ui->config_msgbox_1), 0, 30);
}