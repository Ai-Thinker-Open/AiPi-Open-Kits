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
#include "device.h"


void config_digital_clock_3_timer(lv_timer_t* timer)
{
	clock_count_24(&hour, &min, &sec);

	if (lv_obj_is_valid(guider_ui.config_digital_clock_3))
	{
		lv_dclock_set_text_fmt(guider_ui.config_digital_clock_3, "%02d:%02d", hour, min);
	}
}
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
	lv_obj_set_style_bg_color(ui->config, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_bg1
	ui->config_img_bg1 = lv_img_create(ui->config);
	lv_obj_set_pos(ui->config_img_bg1, 0, 0);
	lv_obj_set_size(ui->config_img_bg1, 480, 480);
	lv_obj_set_scrollbar_mode(ui->config_img_bg1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_bg1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_bg1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_bg1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_bg1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_bg1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_bg1, &_R286_bg_alpha_480x480);
	lv_img_set_pivot(ui->config_img_bg1, 50, 50);
	lv_img_set_angle(ui->config_img_bg1, 0);

	//Write codes config_cont_title1
	ui->config_cont_title1 = lv_obj_create(ui->config);
	lv_obj_set_pos(ui->config_cont_title1, 0, 0);
	lv_obj_set_size(ui->config_cont_title1, 480, 30);
	lv_obj_set_scrollbar_mode(ui->config_cont_title1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_cont_title1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_cont_title1, lv_color_make(0x45, 0x45, 0x45), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_cont_title1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_cont_title1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_cont_title1, 42, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_cont_title1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_cont_title1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_cont_title1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_cont_title1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_cont_title1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_1
	ui->config_img_1 = lv_img_create(ui->config_cont_title1);
	lv_obj_set_pos(ui->config_img_1, 411, 0);
	lv_obj_set_size(ui->config_img_1, 25, 25);
	lv_obj_set_scrollbar_mode(ui->config_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_1, lv_color_make(0x9d, 0x9c, 0x9c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_1, 212, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_1, &_wifi_alpha_25x25);
	lv_img_set_pivot(ui->config_img_1, 50, 50);
	lv_img_set_angle(ui->config_img_1, 0);
	static bool config_digital_clock_3_timer_enabled = false;

	//Write codes config_digital_clock_3
	ui->config_digital_clock_3 = lv_dclock_create(ui->config_cont_title1, "23:55");
	lv_obj_set_style_text_align(ui->config_digital_clock_3, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->config_digital_clock_3, 3, 8);
	lv_obj_set_size(ui->config_digital_clock_3, 74, 22);
	//create timer
	if (!config_digital_clock_3_timer_enabled) {
		lv_timer_create(config_digital_clock_3_timer, 1000, NULL);
		config_digital_clock_3_timer_enabled = true;
	}
	//Set style for config_digital_clock_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_digital_clock_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_digital_clock_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_digital_clock_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_digital_clock_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_digital_clock_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_digital_clock_3, lv_color_make(0xbd, 0xbb, 0xbb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_digital_clock_3, &lv_font_MiSans_Demibold_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_digital_clock_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_LOGO
	ui->config_img_LOGO = lv_img_create(ui->config_cont_title1);
	lv_obj_set_pos(ui->config_img_LOGO, 372, 5);
	lv_obj_set_size(ui->config_img_LOGO, 20, 20);
	lv_obj_set_scrollbar_mode(ui->config_img_LOGO, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_LOGO. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_LOGO, lv_color_make(0x1a, 0x1a, 0x1a), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_LOGO, 87, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_LOGO, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_LOGO, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_LOGO, &_logo_off_alpha_20x20);
	lv_img_set_pivot(ui->config_img_LOGO, 50, 50);
	lv_img_set_angle(ui->config_img_LOGO, 0);

	//Write codes config_cont_ssid
	ui->config_cont_ssid = lv_obj_create(ui->config);
	lv_obj_set_pos(ui->config_cont_ssid, 54, 63);
	lv_obj_set_size(ui->config_cont_ssid, 373, 55);
	lv_obj_set_scrollbar_mode(ui->config_cont_ssid, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_cont_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_cont_ssid, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_cont_ssid, lv_color_make(0x24, 0x22, 0x2e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_cont_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_cont_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_cont_ssid, 254, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_cont_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_cont_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_cont_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_cont_ssid, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_cont_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_label_ssid
	ui->config_label_ssid = lv_label_create(ui->config_cont_ssid);
	lv_obj_set_pos(ui->config_label_ssid, 8, 15);
	lv_obj_set_size(ui->config_label_ssid, 139, 22);
	lv_obj_set_scrollbar_mode(ui->config_label_ssid, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->config_label_ssid, "FAE@AIOT");
	if (r2_device->wifi_sta.ssid)
		lv_label_set_text(ui->config_label_ssid, r2_device->wifi_sta.ssid);
	lv_label_set_long_mode(ui->config_label_ssid, LV_LABEL_LONG_WRAP);

	//Set style for config_label_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_label_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_label_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_label_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_label_ssid, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_label_ssid, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_label_ip
	ui->config_label_ip = lv_label_create(ui->config_cont_ssid);
	lv_obj_set_pos(ui->config_label_ip, 146, 15);
	lv_obj_set_size(ui->config_label_ip, 208, 20);
	lv_obj_set_scrollbar_mode(ui->config_label_ip, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->config_label_ip, "192.168.222.254");
	if (r2_device->wifi_sta.ip_addrs)
		lv_label_set_text(r2_device->ui->config_label_ip, r2_device->wifi_sta.ip_addrs);
	lv_label_set_long_mode(ui->config_label_ip, LV_LABEL_LONG_WRAP);

	//Set style for config_label_ip. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_label_ip, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_label_ip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_label_ip, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_label_ip, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_label_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->config_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes config_img_tilebg2
	ui->config_img_tilebg2 = lv_img_create(ui->config);
	lv_obj_set_pos(ui->config_img_tilebg2, 0, 448);
	lv_obj_set_size(ui->config_img_tilebg2, 480, 30);
	lv_obj_set_scrollbar_mode(ui->config_img_tilebg2, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_img_tilebg2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->config_img_tilebg2, lv_color_make(0x18, 0x18, 0x18), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->config_img_tilebg2, 147, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->config_img_tilebg2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->config_img_tilebg2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->config_img_tilebg2, &_Tilie_bg_alpha_480x30);
	lv_img_set_pivot(ui->config_img_tilebg2, 50, 50);
	lv_img_set_angle(ui->config_img_tilebg2, 0);

	//Write codes config_btn_wificonfig
	ui->config_btn_wificonfig = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->config_btn_wificonfig, 87, 156);
	lv_obj_set_size(ui->config_btn_wificonfig, 100, 50);
	lv_obj_set_scrollbar_mode(ui->config_btn_wificonfig, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_btn_wificonfig. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_btn_wificonfig, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_btn_wificonfig, lv_color_make(0x24, 0x22, 0x2E), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_btn_wificonfig, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_btn_wificonfig, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_btn_wificonfig, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_btn_wificonfig, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_btn_wificonfig, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_btn_wificonfig, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_btn_wificonfig, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_btn_wificonfig, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_btn_wificonfig, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_btn_wificonfig, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_btn_wificonfig, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_btn_wificonfig, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_btn_wificonfig, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_btn_wificonfig, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_btn_wificonfig, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->config_btn_wificonfig_label = lv_label_create(ui->config_btn_wificonfig);
	lv_label_set_text(ui->config_btn_wificonfig_label, "启动配网");
	lv_obj_set_style_pad_all(ui->config_btn_wificonfig, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->config_btn_wificonfig_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes config_btn_disconnect
	ui->config_btn_disconnect = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->config_btn_disconnect, 288, 156);
	lv_obj_set_size(ui->config_btn_disconnect, 100, 50);
	lv_obj_set_scrollbar_mode(ui->config_btn_disconnect, LV_SCROLLBAR_MODE_OFF);

	//Set style for config_btn_disconnect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->config_btn_disconnect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->config_btn_disconnect, lv_color_make(0x24, 0x22, 0x2E), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->config_btn_disconnect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->config_btn_disconnect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->config_btn_disconnect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->config_btn_disconnect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->config_btn_disconnect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->config_btn_disconnect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->config_btn_disconnect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->config_btn_disconnect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->config_btn_disconnect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->config_btn_disconnect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->config_btn_disconnect, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->config_btn_disconnect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->config_btn_disconnect, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->config_btn_disconnect, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->config_btn_disconnect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->config_btn_disconnect_label = lv_label_create(ui->config_btn_disconnect);
	lv_label_set_text(ui->config_btn_disconnect_label, "断开连接");
	lv_obj_set_style_pad_all(ui->config_btn_disconnect, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->config_btn_disconnect_label, LV_ALIGN_CENTER, 0, 0);

	static const char* config_msgbox_1_btns[] = { "" };
	ui->config_msgbox_1 = lv_msgbox_create(ui->config, "提示", "网络未连接，请配置网络！", config_msgbox_1_btns, true);
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

	lv_obj_add_flag(ui->config_msgbox_1, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_style(lv_msgbox_get_btns(ui->config_msgbox_1), &style_config_msgbox_1_extra_btns_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);
	lv_obj_set_size(lv_msgbox_get_btns(ui->config_msgbox_1), 0, 30);

	//Init events for screen
	events_init_config(ui);
}
