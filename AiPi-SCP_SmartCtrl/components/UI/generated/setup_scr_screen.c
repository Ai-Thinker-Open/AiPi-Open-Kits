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

int screen_digital_clock_1_hour_value = 11;
int screen_digital_clock_1_min_value = 25;
int screen_digital_clock_1_sec_value = 50;

int src_home_digital_date_yesr_value = 2023;
int src_home_digital_date_mont_value = 8;
int src_home_digital_date_day_value = 7;
int src_home_digital_date_wday_value = 1;
char* week_day[7] = { "日","一","二","三","四","五","六" };

void screen_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&screen_digital_clock_1_hour_value, &screen_digital_clock_1_min_value, &screen_digital_clock_1_sec_value);
	if (lv_obj_is_valid(guider_ui.screen_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.screen_digital_clock_1, "%02d:%02d:%02d", screen_digital_clock_1_hour_value, screen_digital_clock_1_min_value, screen_digital_clock_1_sec_value);
		lv_label_set_text_fmt(guider_ui.screen_label_date, "%04d年%02d月%02d日", src_home_digital_date_yesr_value, src_home_digital_date_mont_value, src_home_digital_date_day_value);
		lv_label_set_text_fmt(guider_ui.screen_label_wday, "星期%s", week_day[src_home_digital_date_wday_value]);
	}
}

static lv_obj_t* g_kb_screen;
static void kb_screen_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_screen_event_cb(lv_event_t* e)
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

void setup_scr_screen(lv_ui* ui) {

	//Write codes screen
	ui->screen = lv_obj_create(NULL);

	//Create keyboard on screen
	g_kb_screen = lv_keyboard_create(ui->screen);
	lv_obj_add_event_cb(g_kb_screen, kb_screen_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_screen, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_temp
	ui->screen_cont_temp = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_temp, 10, 20);
	lv_obj_set_size(ui->screen_cont_temp, 300, 200);
	lv_obj_set_scrollbar_mode(ui->screen_cont_temp, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_bg
	ui->screen_img_bg = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_bg, 0, 0);
	lv_obj_set_size(ui->screen_img_bg, 320, 240);
	lv_obj_set_scrollbar_mode(ui->screen_img_bg, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_bg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_bg, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_bg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_bg, &_img_bg_alpha_320x240);
	lv_img_set_pivot(ui->screen_img_bg, 50, 50);
	lv_img_set_angle(ui->screen_img_bg, 0);

	//Write codes screen_tileview_home
	ui->screen_tileview_home = lv_tileview_create(ui->screen);
	lv_obj_set_pos(ui->screen_tileview_home, 0, 0);
	lv_obj_set_size(ui->screen_tileview_home, 320, 240);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_home, LV_SCROLLBAR_MODE_ON);

	//Set style for screen_tileview_home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_home, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_tileview_home, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_tileview_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_tileview_home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_tileview_home. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_tileview_home, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_home, lv_color_make(0xea, 0xef, 0xf3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_tileview_home, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile config
	ui->config = lv_tileview_add_tile(ui->screen_tileview_home, 0, 1, LV_DIR_TOP);

	//Write codes screen_label_ssid
	ui->screen_label_ssid = lv_label_create(ui->config);
	lv_obj_set_pos(ui->screen_label_ssid, 46, 44);
	lv_obj_set_size(ui->screen_label_ssid, 45, 17);
	lv_obj_set_scrollbar_mode(ui->screen_label_ssid, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_ssid, "SSID:");
	lv_label_set_long_mode(ui->screen_label_ssid, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_ssid, &lv_font_arial_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_ssid, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_pass
	ui->screen_label_pass = lv_label_create(ui->config);
	lv_obj_set_pos(ui->screen_label_pass, 45, 102);
	lv_obj_set_size(ui->screen_label_pass, 48, 17);
	lv_obj_set_scrollbar_mode(ui->screen_label_pass, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_pass, "PASS:");
	lv_label_set_long_mode(ui->screen_label_pass, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_pass, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_pass, &lv_font_arial_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_pass, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_scan
	ui->screen_btn_scan = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->screen_btn_scan, 236, 33);
	lv_obj_set_size(ui->screen_btn_scan, 58, 36);
	lv_obj_set_scrollbar_mode(ui->screen_btn_scan, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_btn_scan. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_btn_scan, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_scan, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_btn_scan, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_btn_scan, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_btn_scan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_scan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_scan, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_scan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_scan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_scan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_scan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_btn_scan, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_scan, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_btn_scan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_scan, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_scan, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_scan, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_scan_label = lv_label_create(ui->screen_btn_scan);
	lv_label_set_text(ui->screen_btn_scan_label, "搜索");
	lv_obj_set_style_pad_all(ui->screen_btn_scan, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_scan_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_ta_pass
	ui->screen_ta_pass = lv_textarea_create(ui->config);
	lv_obj_set_pos(ui->screen_ta_pass, 98, 90);
	lv_obj_set_size(ui->screen_ta_pass, 130, 36);
	lv_obj_set_scrollbar_mode(ui->screen_ta_pass, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_ta_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ta_pass, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_ta_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ta_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ta_pass, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_ta_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_ta_pass, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_ta_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ta_pass, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_ta_pass, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ta_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_ta_pass, &lv_font_arial_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_ta_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_ta_pass, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ta_pass, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_ta_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_ta_pass. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ta_pass, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ta_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ta_pass, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->screen_ta_pass, "Hello World");

	//use keyboard on screen_ta_pass
	lv_obj_add_event_cb(ui->screen_ta_pass, ta_screen_event_cb, LV_EVENT_ALL, g_kb_screen);

	//Write codes screen_btn_connect
	ui->screen_btn_connect = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->screen_btn_connect, 126, 158);
	lv_obj_set_size(ui->screen_btn_connect, 65, 42);
	lv_obj_set_scrollbar_mode(ui->screen_btn_connect, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_btn_connect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_btn_connect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_connect, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_btn_connect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_connect, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_connect, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_connect, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_connect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_btn_connect_label = lv_label_create(ui->screen_btn_connect);
	lv_label_set_text(ui->screen_btn_connect_label, "连接");
	lv_obj_set_style_pad_all(ui->screen_btn_connect, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_btn_connect_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_ddlist_ssid_list
	ui->screen_ddlist_ssid_list = lv_dropdown_create(ui->config);
	lv_obj_set_pos(ui->screen_ddlist_ssid_list, 96, 33);
	lv_obj_set_size(ui->screen_ddlist_ssid_list, 131, 36);
	lv_obj_set_scrollbar_mode(ui->screen_ddlist_ssid_list, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_ddlist_ssid_list, "list1\nlist2\nlist3");

	//Set style for screen_ddlist_ssid_list. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ddlist_ssid_list, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ddlist_ssid_list, lv_color_make(0xad, 0xad, 0xad), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_ddlist_ssid_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_ssid_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_ssid_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_ssid_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_ddlist_ssid_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_ddlist_ssid_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_ddlist_ssid_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_ddlist_ssid_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_ddlist_ssid_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_ddlist_ssid_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ddlist_ssid_list, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_ddlist_ssid_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ddlist_ssid_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_ddlist_ssid_list, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ddlist_ssid_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ddlist_ssid_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ddlist_ssid_list, 10, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_screen_ddlist_ssid_list_extra_list_selected_checked
	static lv_style_t style_screen_ddlist_ssid_list_extra_list_selected_checked;
	ui_init_style(&style_screen_ddlist_ssid_list_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_ddlist_ssid_list_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_ddlist_ssid_list_extra_list_selected_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_ddlist_ssid_list_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_ssid_list_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_ssid_list_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_ddlist_ssid_list_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_ddlist_ssid_list_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_ddlist_ssid_list_extra_list_selected_checked, 0);
	lv_style_set_text_color(&style_screen_ddlist_ssid_list_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_ddlist_ssid_list_extra_list_selected_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_ssid_list), &style_screen_ddlist_ssid_list_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_ssid_list_extra_list_main_default
	static lv_style_t style_screen_ddlist_ssid_list_extra_list_main_default;
	ui_init_style(&style_screen_ddlist_ssid_list_extra_list_main_default);
	lv_style_set_radius(&style_screen_ddlist_ssid_list_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_ssid_list_extra_list_main_default, lv_color_make(0x22, 0x35, 0x5f));
	lv_style_set_bg_grad_color(&style_screen_ddlist_ssid_list_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_ssid_list_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_ssid_list_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_ddlist_ssid_list_extra_list_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_ddlist_ssid_list_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_ddlist_ssid_list_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_ddlist_ssid_list_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_ddlist_ssid_list_extra_list_main_default, &lv_font_montserratMedium_12);
	lv_style_set_max_height(&style_screen_ddlist_ssid_list_extra_list_main_default, 130);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_ssid_list), &style_screen_ddlist_ssid_list_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_ssid_list_extra_list_scrollbar_default
	static lv_style_t style_screen_ddlist_ssid_list_extra_list_scrollbar_default;
	ui_init_style(&style_screen_ddlist_ssid_list_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_ddlist_ssid_list_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_ssid_list_extra_list_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_opa(&style_screen_ddlist_ssid_list_extra_list_scrollbar_default, 0);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_ssid_list), &style_screen_ddlist_ssid_list_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile home
	ui->home = lv_tileview_add_tile(ui->screen_tileview_home, 0, 0, LV_DIR_BOTTOM);

	//Write codes screen_label_tile
	ui->screen_label_tile = lv_label_create(ui->home);
	lv_obj_set_pos(ui->screen_label_tile, 107, 6);
	lv_obj_set_size(ui->screen_label_tile, 113, 19);
	lv_obj_set_scrollbar_mode(ui->screen_label_tile, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_tile, "智能监控终端");
	lv_label_set_long_mode(ui->screen_label_tile, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_tile. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_tile, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_tile, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_tile, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_tile, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool screen_digital_clock_1_timer_enabled = false;

	//Write codes screen_digital_clock_1
	ui->screen_digital_clock_1 = lv_dclock_create(ui->home, "11:25:50");
	lv_obj_set_style_text_align(ui->screen_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->screen_digital_clock_1, 3, 40);
	lv_obj_set_size(ui->screen_digital_clock_1, 154, 65);
	lv_dclock_set_text_fmt(guider_ui.screen_digital_clock_1, "%02d:%02d:%02d", screen_digital_clock_1_hour_value, screen_digital_clock_1_min_value, screen_digital_clock_1_sec_value);
	//create timer
	if (!screen_digital_clock_1_timer_enabled) {
		lv_timer_create(screen_digital_clock_1_timer, 1000, NULL);
		screen_digital_clock_1_timer_enabled = true;
	}
	//Set style for screen_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_digital_clock_1, lv_color_make(0x20, 0x6d, 0xc0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_digital_clock_1, &lv_font_Antonio_Regular_45, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_wifi
	ui->screen_img_wifi = lv_img_create(ui->home);
	lv_obj_set_pos(ui->screen_img_wifi, 286, 6);
	lv_obj_set_size(ui->screen_img_wifi, 22, 18);
	lv_obj_set_scrollbar_mode(ui->screen_img_wifi, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_wifi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_wifi, LV_OBJ_FLAG_CLICKABLE);

	if (ui->wifi_status)
		lv_img_set_src(ui->screen_img_wifi, &_wifi_alpha_22x18);
	else
		lv_img_set_src(ui->screen_img_wifi, &_no_internet_alpha_22x18);
	lv_img_set_pivot(ui->screen_img_wifi, 0, 0);
	lv_img_set_angle(ui->screen_img_wifi, 0);

	//Write codes screen_cont_sensor
	ui->screen_cont_sensor = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_sensor, 155, 37);
	lv_obj_set_size(ui->screen_cont_sensor, 157, 105);
	lv_obj_set_scrollbar_mode(ui->screen_cont_sensor, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_sensor. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_sensor, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_sensor, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_sensor, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_sensor, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_sensor, 189, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_sensor, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_sensor, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_sensor, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_sensor, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_sensorD
	ui->screen_label_sensorD = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_sensorD, 6, 1);
	lv_obj_set_size(ui->screen_label_sensorD, 68, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_sensorD, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_sensorD, "鱼缸监控");
	lv_label_set_long_mode(ui->screen_label_sensorD, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_sensorD. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_sensorD, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_sensorD, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_sensorD, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_sensorD, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_sensorD, lv_color_make(0xdd, 0x57, 0x8b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_sensorD, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_sensorD, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_sensorD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_O2
	ui->screen_label_O2 = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_O2, 9, 27);
	lv_obj_set_size(ui->screen_label_O2, 56, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_O2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_O2, "含氧量：");
	lv_label_set_long_mode(ui->screen_label_O2, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_O2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_O2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_O2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_O2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_O2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_O2, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_O2, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_O2, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_O2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_1, 9, 52);
	lv_obj_set_size(ui->screen_label_1, 56, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_1, "水 温：");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_1, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_PH
	ui->screen_label_PH = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_PH, 9, 78);
	lv_obj_set_size(ui->screen_label_PH, 56, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_PH, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_PH, "PH值：");
	lv_label_set_long_mode(ui->screen_label_PH, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_PH. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_PH, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_PH, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_PH, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_PH, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_PH, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_PH, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_PH, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_PH, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_O2_vlue
	ui->screen_label_O2_vlue = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_O2_vlue, 65, 27);
	lv_obj_set_size(ui->screen_label_O2_vlue, 42, 12);
	lv_obj_set_scrollbar_mode(ui->screen_label_O2_vlue, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_O2_vlue, "70%");
	lv_label_set_long_mode(ui->screen_label_O2_vlue, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_O2_vlue. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_O2_vlue, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_O2_vlue, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_O2_vlue, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_O2_vlue, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_O2_vlue, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_O2_vlue, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_O2_vlue, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_O2_vlue, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_temp_value
	ui->screen_label_temp_value = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_temp_value, 65, 52);
	lv_obj_set_size(ui->screen_label_temp_value, 42, 12);
	lv_obj_set_scrollbar_mode(ui->screen_label_temp_value, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_temp_value, "25℃");
	lv_label_set_long_mode(ui->screen_label_temp_value, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_temp_value. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_temp_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_temp_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_temp_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_temp_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_temp_value, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_temp_value, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_temp_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_temp_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_PH_value
	ui->screen_label_PH_value = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_PH_value, 65, 78);
	lv_obj_set_size(ui->screen_label_PH_value, 42, 12);
	lv_obj_set_scrollbar_mode(ui->screen_label_PH_value, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_PH_value, "7.02");
	lv_label_set_long_mode(ui->screen_label_PH_value, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_PH_value. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_PH_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_PH_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_PH_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_PH_value, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_PH_value, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_PH_value, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_PH_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_PH_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen_cont_sensor);
	lv_obj_set_pos(ui->screen_label_3, 101, 4);
	lv_obj_set_size(ui->screen_label_3, 37, 12);
	lv_obj_set_scrollbar_mode(ui->screen_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_3, "未连接");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_3, lv_color_make(0xb4, 0xb4, 0xb4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_host
	ui->screen_label_host = lv_label_create(ui->home);
	lv_obj_set_pos(ui->screen_label_host, 6, 9);
	lv_obj_set_size(ui->screen_label_host, 111, 12);
	lv_obj_set_scrollbar_mode(ui->screen_label_host, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_host, "mqtt.com:1883");
	lv_label_set_long_mode(ui->screen_label_host, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_host. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_host, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_host, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_host, &lv_font_arial_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_host, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_dev1
	ui->screen_cont_dev1 = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev1, 9, 154);
	lv_obj_set_size(ui->screen_cont_dev1, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev1, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev1, 191, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_rgb
	ui->screen_img_rgb = lv_img_create(ui->screen_cont_dev1);
	lv_obj_set_pos(ui->screen_img_rgb, 0, 8);
	lv_obj_set_size(ui->screen_img_rgb, 32, 32);
	lv_obj_set_scrollbar_mode(ui->screen_img_rgb, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_rgb. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_rgb, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_rgb, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_rgb, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_rgb, &_RGB_open_alpha_32x32);
	lv_img_set_pivot(ui->screen_img_rgb, 15, 15);
	lv_img_set_angle(ui->screen_img_rgb, 0);

	//Write codes screen_img_wb2_open
	ui->screen_img_wb2_open = lv_img_create(ui->screen_cont_dev1);
	lv_obj_set_pos(ui->screen_img_wb2_open, 47, 19);
	lv_obj_set_size(ui->screen_img_wb2_open, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_wb2_open, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_wb2_open. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_wb2_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_wb2_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_wb2_open, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_wb2_open, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_wb2_open, &_Ligth_close_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_wb2_open, 0, 0);
	lv_img_set_angle(ui->screen_img_wb2_open, 0);

	//Write codes screen_label_rgbtext1
	ui->screen_label_rgbtext1 = lv_label_create(ui->screen_cont_dev1);
	lv_obj_set_pos(ui->screen_label_rgbtext1, 6, 46);
	lv_obj_set_size(ui->screen_label_rgbtext1, 24, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_rgbtext1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_rgbtext1, "RGB");
	lv_label_set_long_mode(ui->screen_label_rgbtext1, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_rgbtext1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_rgbtext1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_rgbtext1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_rgbtext1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_rgbtext1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_rgbtext1, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_rgbtext1, &lv_font_arial_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_rgbtext1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_rgbtext1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_status1
	ui->screen_label_status1 = lv_label_create(ui->screen_cont_dev1);
	lv_obj_set_pos(ui->screen_label_status1, 4, 59);
	lv_obj_set_size(ui->screen_label_status1, 74, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_status1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_status1, "Ai-WB2/未连接");
	lv_label_set_long_mode(ui->screen_label_status1, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_status1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_status1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_status1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_status1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_status1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_status1, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_status1, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_status1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_status1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_wb2_close
	ui->screen_img_wb2_close = lv_img_create(ui->screen_cont_dev1);
	lv_obj_set_pos(ui->screen_img_wb2_close, 47, 19);
	lv_obj_set_size(ui->screen_img_wb2_close, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_wb2_close, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_wb2_close. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_wb2_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_wb2_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_wb2_close, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_wb2_close, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget screen_img_wb2_close
	lv_obj_add_flag(ui->screen_img_wb2_close, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->screen_img_wb2_close, &_Ligth_open_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_wb2_close, 50, 50);
	lv_img_set_angle(ui->screen_img_wb2_close, 0);

	//Write codes screen_cont_dev2
	ui->screen_cont_dev2 = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev2, 114, 154);
	lv_obj_set_size(ui->screen_cont_dev2, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev2, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev2, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev2, 191, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_rgb1
	ui->screen_img_rgb1 = lv_img_create(ui->screen_cont_dev2);
	lv_obj_set_pos(ui->screen_img_rgb1, 0, 8);
	lv_obj_set_size(ui->screen_img_rgb1, 32, 32);
	lv_obj_set_scrollbar_mode(ui->screen_img_rgb1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_rgb1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_rgb1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_rgb1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_rgb1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_rgb1, &_RGB_open_alpha_32x32);
	lv_img_set_pivot(ui->screen_img_rgb1, 15, 15);
	lv_img_set_angle(ui->screen_img_rgb1, 0);

	//Write codes screen_img_m62_open
	ui->screen_img_m62_open = lv_img_create(ui->screen_cont_dev2);
	lv_obj_set_pos(ui->screen_img_m62_open, 47, 19);
	lv_obj_set_size(ui->screen_img_m62_open, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_m62_open, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_m62_open. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_m62_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_m62_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_m62_open, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_m62_open, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_m62_open, &_Ligth_close_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_m62_open, 0, 0);
	lv_img_set_angle(ui->screen_img_m62_open, 0);

	//Write codes screen_label_rgbtext2
	ui->screen_label_rgbtext2 = lv_label_create(ui->screen_cont_dev2);
	lv_obj_set_pos(ui->screen_label_rgbtext2, 6, 46);
	lv_obj_set_size(ui->screen_label_rgbtext2, 24, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_rgbtext2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_rgbtext2, "RGB");
	lv_label_set_long_mode(ui->screen_label_rgbtext2, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_rgbtext2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_rgbtext2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_rgbtext2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_rgbtext2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_rgbtext2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_rgbtext2, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_rgbtext2, &lv_font_arial_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_rgbtext2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_rgbtext2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_status2
	ui->screen_label_status2 = lv_label_create(ui->screen_cont_dev2);
	lv_obj_set_pos(ui->screen_label_status2, 4, 59);
	lv_obj_set_size(ui->screen_label_status2, 74, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_status2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_status2, "Ai-M62/已连接");
	lv_label_set_long_mode(ui->screen_label_status2, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_status2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_status2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_status2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_status2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_status2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_status2, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_status2, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_status2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_status2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_m62_close
	ui->screen_img_m62_close = lv_img_create(ui->screen_cont_dev2);
	lv_obj_set_pos(ui->screen_img_m62_close, 47, 19);
	lv_obj_set_size(ui->screen_img_m62_close, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_m62_close, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_m62_close. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_m62_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_m62_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_m62_close, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_m62_close, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget screen_img_m62_close
	lv_obj_add_flag(ui->screen_img_m62_close, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->screen_img_m62_close, &_Ligth_open_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_m62_close, 50, 50);
	lv_img_set_angle(ui->screen_img_m62_close, 0);

	//Write codes screen_cont_dev3
	ui->screen_cont_dev3 = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev3, 221, 154);
	lv_obj_set_size(ui->screen_cont_dev3, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev3, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev3, 191, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_rgb3
	ui->screen_img_rgb3 = lv_img_create(ui->screen_cont_dev3);
	lv_obj_set_pos(ui->screen_img_rgb3, 0, 8);
	lv_obj_set_size(ui->screen_img_rgb3, 32, 32);
	lv_obj_set_scrollbar_mode(ui->screen_img_rgb3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_rgb3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_rgb3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_rgb3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_rgb3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_rgb3, &_RGB_open_alpha_32x32);
	lv_img_set_pivot(ui->screen_img_rgb3, 15, 15);
	lv_img_set_angle(ui->screen_img_rgb3, 0);

	//Write codes screen_img_bw16_open
	ui->screen_img_bw16_open = lv_img_create(ui->screen_cont_dev3);
	lv_obj_set_pos(ui->screen_img_bw16_open, 47, 19);
	lv_obj_set_size(ui->screen_img_bw16_open, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_bw16_open, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_bw16_open. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_bw16_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_bw16_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_bw16_open, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_bw16_open, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_bw16_open, &_Ligth_close_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_bw16_open, 0, 0);
	lv_img_set_angle(ui->screen_img_bw16_open, 0);

	//Write codes screen_label_rgbtext3
	ui->screen_label_rgbtext3 = lv_label_create(ui->screen_cont_dev3);
	lv_obj_set_pos(ui->screen_label_rgbtext3, 6, 46);
	lv_obj_set_size(ui->screen_label_rgbtext3, 24, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_rgbtext3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_rgbtext3, "RGB");
	lv_label_set_long_mode(ui->screen_label_rgbtext3, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_rgbtext3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_rgbtext3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_rgbtext3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_rgbtext3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_rgbtext3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_rgbtext3, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_rgbtext3, &lv_font_arial_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_rgbtext3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_rgbtext3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_status3
	ui->screen_label_status3 = lv_label_create(ui->screen_cont_dev3);
	lv_obj_set_pos(ui->screen_label_status3, 4, 59);
	lv_obj_set_size(ui->screen_label_status3, 74, 10);
	lv_obj_set_scrollbar_mode(ui->screen_label_status3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_status3, "BW16/已连接");
	lv_label_set_long_mode(ui->screen_label_status3, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_status3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_status3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_status3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_status3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_status3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_status3, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_status3, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_status3, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_status3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_bw16_close
	ui->screen_img_bw16_close = lv_img_create(ui->screen_cont_dev3);
	lv_obj_set_pos(ui->screen_img_bw16_close, 47, 19);
	lv_obj_set_size(ui->screen_img_bw16_close, 30, 30);
	lv_obj_set_scrollbar_mode(ui->screen_img_bw16_close, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_bw16_close. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_bw16_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_bw16_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_bw16_close, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_bw16_close, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget screen_img_bw16_close
	lv_obj_add_flag(ui->screen_img_bw16_close, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->screen_img_bw16_close, &_Ligth_open_alpha_30x30);
	lv_img_set_pivot(ui->screen_img_bw16_close, 50, 50);
	lv_img_set_angle(ui->screen_img_bw16_close, 0);

	//Write codes screen_label_date
	ui->screen_label_date = lv_label_create(ui->home);
	lv_obj_set_pos(ui->screen_label_date, 5, 92);
	lv_obj_set_size(ui->screen_label_date, 103, 15);
	lv_obj_set_scrollbar_mode(ui->screen_label_date, LV_SCROLLBAR_MODE_OFF);
	// lv_label_set_text(ui->screen_label_date, "2023年08月05日");
	lv_label_set_text_fmt(guider_ui.screen_label_date, "%04d年%02d月%02d日", src_home_digital_date_yesr_value, src_home_digital_date_mont_value, src_home_digital_date_day_value);
	lv_label_set_long_mode(ui->screen_label_date, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_date. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_date, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_date, lv_color_make(0xf3, 0xe5, 0xba), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_date, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_wday
	ui->screen_label_wday = lv_label_create(ui->home);
	lv_obj_set_pos(ui->screen_label_wday, 105, 92);
	lv_obj_set_size(ui->screen_label_wday, 46, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_wday, LV_SCROLLBAR_MODE_OFF);
	// lv_label_set_text(ui->screen_label_wday, "星期六");
	lv_label_set_text_fmt(guider_ui.screen_label_wday, "星期%s", week_day[src_home_digital_date_wday_value]);
	lv_label_set_long_mode(ui->screen_label_wday, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_wday. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_wday, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_wday, lv_color_make(0xc9, 0xb3, 0xe1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_wday, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_wday, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_dizhi
	ui->screen_label_dizhi = lv_label_create(ui->home);
	lv_obj_set_pos(ui->screen_label_dizhi, 12, 118);
	lv_obj_set_size(ui->screen_label_dizhi, 124, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_dizhi, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_dizhi, "深圳市  晴转多云");
	lv_label_set_long_mode(ui->screen_label_dizhi, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_dizhi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_dizhi, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_dizhi, lv_color_make(0xe1, 0x78, 0x72), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_dizhi, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_dizhi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_dev1_no
	ui->screen_cont_dev1_no = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev1_no, 9, 154);
	lv_obj_set_size(ui->screen_cont_dev1_no, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev1_no, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev1_no. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev1_no, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev1_no, lv_color_make(0x76, 0x76, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev1_no, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev1_no, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev1_no, 108, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev1_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev1_no, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev1_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev1_no, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev1_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


	//Write codes screen_cont_dev2_no
	ui->screen_cont_dev2_no = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev2_no, 114, 154);
	lv_obj_set_size(ui->screen_cont_dev2_no, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev2_no, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev2_no. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev2_no, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev2_no, lv_color_make(0x76, 0x76, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev2_no, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev2_no, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev2_no, 108, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev2_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev2_no, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev2_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev2_no, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev2_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_dev_no
	ui->screen_cont_dev_no = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_dev_no, 221, 154);
	lv_obj_set_size(ui->screen_cont_dev_no, 87, 77);
	lv_obj_set_scrollbar_mode(ui->screen_cont_dev_no, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_dev_no. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_dev_no, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_dev_no, lv_color_make(0x76, 0x76, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_dev_no, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_dev_no, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_dev_no, 108, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_dev_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_dev_no, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_dev_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_dev_no, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_dev_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_sensor_no
	ui->screen_cont_sensor_no = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->screen_cont_sensor_no, 155, 37);
	lv_obj_set_size(ui->screen_cont_sensor_no, 157, 105);
	lv_obj_set_scrollbar_mode(ui->screen_cont_sensor_no, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_sensor_no. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_sensor_no, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_sensor_no, lv_color_make(0x76, 0x76, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_sensor_no, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_sensor_no, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_sensor_no, 108, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_sensor_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_sensor_no, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_sensor_no, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_sensor_no, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_sensor_no, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_loading
	ui->screen_cont_loading = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_loading, 0, 0);
	lv_obj_set_size(ui->screen_cont_loading, 320, 240);
	lv_obj_set_scrollbar_mode(ui->screen_cont_loading, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_loading. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_loading, lv_color_make(0x30, 0x30, 0x30), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_loading, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_loading, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_loading, 96, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_loading, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_loading, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_loading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget screen_cont_loading
	lv_obj_add_flag(ui->screen_cont_loading, LV_OBJ_FLAG_HIDDEN);


	//Write codes screen_img_loading
	ui->screen_img_loading = lv_img_create(ui->screen_cont_loading);
	lv_obj_set_pos(ui->screen_img_loading, 110, 70);
	lv_obj_set_size(ui->screen_img_loading, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_img_loading, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_loading. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_loading, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_loading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_loading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_loading, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_loading, &_loading_alpha_100x100);
	lv_img_set_pivot(ui->screen_img_loading, 50, 50);
	lv_img_set_angle(ui->screen_img_loading, 0);


	lv_obj_add_flag(ui->screen_cont_dev1_no, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->screen_cont_dev2_no, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui->screen_cont_dev_no, LV_OBJ_FLAG_HIDDEN);
	//Init events for screen
	events_init_screen(ui);
}