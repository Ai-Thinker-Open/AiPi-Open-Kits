/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

static lv_obj_t* g_kb_config;
static void kb_config_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_config_event_cb(lv_event_t* e)
{

	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* ta = lv_event_get_target(e);
	lv_obj_t* kb = lv_event_get_user_data(e);
	if (code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED)
	{
		lv_keyboard_set_textarea(kb, ta);
		lv_obj_move_foreground(kb);
		lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
	if (code == LV_EVENT_CANCEL || code == LV_EVENT_DEFOCUSED)
	{
		lv_keyboard_set_textarea(kb, NULL);
		lv_obj_move_background(kb);
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

void setup_scr_config(lv_ui* ui) {

	//Write codes config
	ui->config = lv_obj_create(NULL);

	//Create keyboard on config
	g_kb_config = lv_keyboard_create(ui->config);
	lv_obj_add_event_cb(g_kb_config, kb_config_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_config, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_config, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->config, LV_SCROLLBAR_MODE_OFF);

	//Set style for config. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config, 100, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_1
	ui->config_img_1 = lv_img_create(ui->config);
	lv_obj_set_pos(ui->config_img_1, 0, 0);
	lv_obj_set_size(ui->config_img_1, 239, 239);
	lv_obj_set_scrollbar_mode(ui->config_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_1, lv_color_make(0x0b, 0x0b, 0x0b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_1, &_bg_alpha_239x239);
	lv_img_set_pivot(ui->config_img_1, 50, 50);
	lv_img_set_angle(ui->config_img_1, 0);

	//Write codes config_ta_pass
	ui->config_ta_pass = lv_textarea_create(ui->config);
	lv_obj_set_pos(ui->config_ta_pass, 48, 87);
	lv_obj_set_size(ui->config_ta_pass, 144, 26);
	lv_obj_set_scrollbar_mode(ui->config_ta_pass, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_ta_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_ta_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_ta_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_ta_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_ta_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_ta_pass, lv_color_make(0x3b, 0x36, 0x36), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_ta_pass, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_ta_pass, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_ta_pass, lv_color_make(0xeb, 0xeb, 0xeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_ta_pass, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_ta_pass, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for config_ta_pass. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_ta_pass, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_ta_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_ta_pass, 17, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	if (ui->ssid!=NULL)
		lv_textarea_set_text(ui->config_ta_pass, ui->password);
	else
		lv_textarea_set_text(ui->config_ta_pass, "");
	//use keyboard on config_ta_pass
	lv_obj_add_event_cb(ui->config_ta_pass, ta_config_event_cb, LV_EVENT_ALL, g_kb_config);

	//Write codes config_label_PASS
	ui->config_label_PASS = lv_label_create(ui->config);
	lv_obj_set_pos(ui->config_label_PASS, 49, 74);
	lv_obj_set_size(ui->config_label_PASS, 104, 15);
	lv_obj_set_scrollbar_mode(ui->config_label_PASS, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->config_label_PASS, "PASSWORD:");
	lv_label_set_long_mode(ui->config_label_PASS, LV_LABEL_LONG_WRAP);

	//Set style for config_label_PASS. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_label_PASS, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_label_PASS, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_label_PASS, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_label_PASS, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_label_PASS, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_label_PASS, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_label_SSID
	ui->config_label_SSID = lv_label_create(ui->config);
	lv_obj_set_pos(ui->config_label_SSID, 49, 30);
	lv_obj_set_size(ui->config_label_SSID, 46, 14);
	lv_obj_set_scrollbar_mode(ui->config_label_SSID, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->config_label_SSID, "SSID:");
	lv_label_set_long_mode(ui->config_label_SSID, LV_LABEL_LONG_WRAP);

	//Set style for config_label_SSID. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_label_SSID, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_label_SSID, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_label_SSID, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_label_SSID, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_label_SSID, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_label_SSID, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_label_SSID, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_label_SSID, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_label_SSID, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_label_SSID, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_ta_ssid
	ui->config_ta_ssid = lv_textarea_create(ui->config);
	lv_obj_set_pos(ui->config_ta_ssid, 48, 44);
	lv_obj_set_size(ui->config_ta_ssid, 144, 26);
	lv_obj_set_scrollbar_mode(ui->config_ta_ssid, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_ta_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_ta_ssid, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_ta_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_ta_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_ta_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_ta_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_ta_ssid, lv_color_make(0x3b, 0x36, 0x36), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_ta_ssid, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_ta_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_ta_ssid, lv_color_make(0xf2, 0xf2, 0xf2), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_ta_ssid, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_ta_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_ta_ssid, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_ta_ssid, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_ta_ssid, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_ta_ssid, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_ta_ssid, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for config_ta_ssid. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_ta_ssid, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_ta_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_ta_ssid, 17, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	if (ui->password!=NULL)
		lv_textarea_set_text(ui->config_ta_ssid, ui->ssid);
	else
		lv_textarea_set_text(ui->config_ta_ssid, "");

	//use keyboard on config_ta_ssid
	lv_obj_add_event_cb(ui->config_ta_ssid, ta_config_event_cb, LV_EVENT_ALL, g_kb_config);

	//Write codes config_btn_1
	ui->config_btn_1 = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->config_btn_1, 95, 150);
	lv_obj_set_size(ui->config_btn_1, 50, 30);
	lv_obj_set_scrollbar_mode(ui->config_btn_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_btn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_btn_1, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_btn_1, lv_color_make(0x8d, 0x07, 0x07), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_btn_1, lv_color_make(0xd6, 0xa9, 0xa9), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_btn_1, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->config_btn_1_label = lv_label_create(ui->config_btn_1);
	lv_label_set_text(ui->config_btn_1_label, "连接");
	lv_obj_set_style_pad_all(ui->config_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->config_btn_1_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes config_img_h
	ui->config_img_h = lv_img_create(ui->config);
	lv_obj_set_pos(ui->config_img_h, 21, 110);
	lv_obj_set_size(ui->config_img_h, 20, 20);
	lv_obj_set_scrollbar_mode(ui->config_img_h, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_h. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_h, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_h, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_h, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_h, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_h, &_house_alpha_20x20);
	lv_img_set_pivot(ui->config_img_h, 0, 0);
	lv_img_set_angle(ui->config_img_h, 0);

	//Write codes config_cont_1
	ui->config_cont_1 = lv_obj_create(ui->config);
	lv_obj_set_pos(ui->config_cont_1, 0, 0);
	lv_obj_set_size(ui->config_cont_1, 240, 240);
	lv_obj_set_scrollbar_mode(ui->config_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_cont_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_cont_1, 91, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget config_cont_1
	lv_obj_add_flag(ui->config_cont_1, LV_OBJ_FLAG_HIDDEN);


	//Write codes config_img_loding
	ui->config_img_loding = lv_img_create(ui->config_cont_1);
	lv_obj_set_pos(ui->config_img_loding, 41, 42);
	lv_obj_set_size(ui->config_img_loding, 150, 150);
	lv_obj_set_scrollbar_mode(ui->config_img_loding, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_loding. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_loding, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_loding, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_loding, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_loding, &_loding_alpha_150x150);
	lv_img_set_pivot(ui->config_img_loding, 75, 75);
	lv_img_set_angle(ui->config_img_loding, 0);

	//Write codes config_label_5
	ui->config_label_5 = lv_label_create(ui->config_cont_1);
	lv_obj_set_pos(ui->config_label_5, 14, 165);
	lv_obj_set_size(ui->config_label_5, 209, 27);
	lv_obj_set_scrollbar_mode(ui->config_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->config_label_5, "正在连接:ssid");
	lv_label_set_long_mode(ui->config_label_5, LV_LABEL_LONG_WRAP);

	//Set style for config_label_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_label_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_label_5, lv_color_make(0xfa, 0xf4, 0xf4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_label_5, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_label_5, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_3
	ui->config_img_3 = lv_img_create(ui->config);
	lv_obj_set_pos(ui->config_img_3, 23, 159);
	lv_obj_set_size(ui->config_img_3, 15, 80);
	lv_obj_set_scrollbar_mode(ui->config_img_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_3, &_5_alpha_15x80);
	lv_img_set_pivot(ui->config_img_3, 0, 0);
	lv_img_set_angle(ui->config_img_3, 1800);

	//Init events for screen
	events_init_config(ui);
}

void ui_config_creat_msg_box(lv_ui* ui, char* Prompt_content)
{
	static const char* config_msgbox_btns[] = { "" };
	ui->config_msgbox = lv_msgbox_create(ui->config, "Error", Prompt_content, config_msgbox_btns, true);
	lv_obj_set_scrollbar_mode(ui->config_msgbox, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_pos(ui->config_msgbox, 20, 20);
	lv_obj_set_size(ui->config_msgbox, 200, 200);

	//Set style for config_msgbox. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_msgbox, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_msgbox, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_msgbox, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_msgbox, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_msgbox, 209, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_msgbox, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_msgbox, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_msgbox, 100, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_extra_title_main_default
	static lv_style_t style_config_msgbox_extra_title_main_default;
	ui_init_style(&style_config_msgbox_extra_title_main_default);
	lv_style_set_text_color(&style_config_msgbox_extra_title_main_default, lv_color_make(0xeb, 0xff, 0x00));
	lv_style_set_text_font(&style_config_msgbox_extra_title_main_default, &lv_font_Amiko_Regular_20);
	lv_style_set_text_letter_space(&style_config_msgbox_extra_title_main_default, 0);
	lv_style_set_text_line_space(&style_config_msgbox_extra_title_main_default, 30);
	lv_obj_add_style(lv_msgbox_get_title(ui->config_msgbox), &style_config_msgbox_extra_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_extra_content_main_default
	static lv_style_t style_config_msgbox_extra_content_main_default;
	ui_init_style(&style_config_msgbox_extra_content_main_default);
	lv_style_set_text_color(&style_config_msgbox_extra_content_main_default, lv_color_make(0xfb, 0xfb, 0xfb));
	lv_style_set_text_font(&style_config_msgbox_extra_content_main_default, &lv_font_simhei_16);
	lv_style_set_text_letter_space(&style_config_msgbox_extra_content_main_default, 0);
	lv_style_set_text_line_space(&style_config_msgbox_extra_content_main_default, 10);
	lv_obj_add_style(lv_msgbox_get_text(ui->config_msgbox), &style_config_msgbox_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_config_msgbox_extra_btns_items_default
	static lv_style_t style_config_msgbox_extra_btns_items_default;
	ui_init_style(&style_config_msgbox_extra_btns_items_default);
	lv_style_set_radius(&style_config_msgbox_extra_btns_items_default, 10);
	lv_style_set_bg_color(&style_config_msgbox_extra_btns_items_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_bg_grad_color(&style_config_msgbox_extra_btns_items_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_bg_grad_dir(&style_config_msgbox_extra_btns_items_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_config_msgbox_extra_btns_items_default, 255);
	lv_style_set_border_color(&style_config_msgbox_extra_btns_items_default, lv_color_make(0xe6, 0xe6, 0xe6));
	lv_style_set_border_width(&style_config_msgbox_extra_btns_items_default, 0);
	lv_style_set_border_opa(&style_config_msgbox_extra_btns_items_default, 255);
	lv_style_set_text_color(&style_config_msgbox_extra_btns_items_default, lv_color_make(0x4e, 0x4e, 0x4e));
	lv_style_set_text_font(&style_config_msgbox_extra_btns_items_default, &lv_font_montserratMedium_12);

	//Hidden for widget config_msgbox
	// lv_obj_add_flag(ui->config_msgbox, LV_OBJ_FLAG_HIDDEN);
	// lv_msgbox_close(ui->config_msgbox);
	lv_obj_add_style(lv_msgbox_get_btns(ui->config_msgbox), &style_config_msgbox_extra_btns_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_size(lv_msgbox_get_btns(ui->config_msgbox), 0, 30);
}