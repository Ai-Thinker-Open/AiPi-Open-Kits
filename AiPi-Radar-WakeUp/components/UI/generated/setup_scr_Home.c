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
#include "https_client.h"
#include "FreeRTOS.h"
int Home_digital_clock_1_hour_value = 11;
int Home_digital_clock_1_min_value = 25;
int Home_digital_clock_1_sec_value = 50;

char* week_day[7] = { "日","一","二","三","四","五","六" };

void Home_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&Home_digital_clock_1_hour_value, &Home_digital_clock_1_min_value, &Home_digital_clock_1_sec_value);
	if (lv_obj_is_valid(guider_ui.Home_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.Home_digital_clock_1, "%02d:%02d:%02d", Home_digital_clock_1_hour_value, Home_digital_clock_1_min_value, Home_digital_clock_1_sec_value);
		lv_label_set_text_fmt(guider_ui.Home_label_date, "%04d年%02d月%02d日", date_yesr_value, date_mont_value, date_day_value);
		lv_label_set_text_fmt(guider_ui.Home_label_wday, "星期%s", week_day[date_wday_value]);
	}
}
static lv_obj_t* g_kb_Home;

static lv_obj_t* g_kb_numble;

static void kb_Home_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}

__attribute__((unused)) static void ta_Home_event_cb(lv_event_t* e)
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

void setup_scr_Home(lv_ui* ui) {

	//Write codes Home
	ui->Home = lv_obj_create(NULL);

	//Create keyboard on Home
	g_kb_Home = lv_keyboard_create(ui->Home);
	lv_obj_add_event_cb(g_kb_Home, kb_Home_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_Home, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_Home, &lv_font_Amiko_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);

	g_kb_numble = lv_keyboard_create(ui->Home);
	lv_obj_add_event_cb(g_kb_numble, kb_Home_event_cb, LV_EVENT_ALL, NULL);
	lv_keyboard_set_mode(g_kb_numble, LV_KEYBOARD_MODE_NUMBER);
	lv_obj_add_flag(g_kb_numble, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_numble, &lv_font_Amiko_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_scrollbar_mode(ui->Home, LV_SCROLLBAR_MODE_OFF);
	//Set style for Home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_tileview_1
	ui->Home_tileview_1 = lv_tileview_create(ui->Home);
	lv_obj_set_pos(ui->Home_tileview_1, 0, 0);
	lv_obj_set_size(ui->Home_tileview_1, 240, 320);
	lv_obj_set_scrollbar_mode(ui->Home_tileview_1, LV_SCROLLBAR_MODE_ACTIVE);

	//Set style for Home_tileview_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_tileview_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_tileview_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_tileview_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_tileview_1. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_tileview_1, lv_color_make(0x53, 0x52, 0x52), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_tileview_1, 111, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile tile_config
	ui->tile_config = lv_tileview_add_tile(ui->Home_tileview_1, 0, 1, LV_DIR_TOP | LV_DIR_BOTTOM);

	//Write codes Home_label_ssd
	ui->Home_label_ssd = lv_label_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_label_ssd, 31, 56);
	lv_obj_set_size(ui->Home_label_ssd, 44, 17);
	lv_obj_set_scrollbar_mode(ui->Home_label_ssd, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_ssd, "SSID:");
	lv_label_set_long_mode(ui->Home_label_ssd, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_ssd. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_ssd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_ssd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_ssd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_ssd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_ssd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_ssd, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_ssd, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_ssd, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_ssd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_ssd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_label_PASS
	ui->Home_label_PASS = lv_label_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_label_PASS, 31, 111);
	lv_obj_set_size(ui->Home_label_PASS, 50, 17);
	lv_obj_set_scrollbar_mode(ui->Home_label_PASS, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_PASS, "PASS:");
	lv_label_set_long_mode(ui->Home_label_PASS, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_PASS. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_PASS, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_PASS, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_PASS, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_PASS, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_PASS, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_PASS, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_PASS, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_PASS, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_ta_passInput
	ui->Home_ta_passInput = lv_textarea_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_ta_passInput, 33, 128);
	lv_obj_set_size(ui->Home_ta_passInput, 166, 26);
	lv_obj_set_scrollbar_mode(ui->Home_ta_passInput, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_ta_passInput. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_passInput, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_passInput, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ta_passInput, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ta_passInput, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_passInput, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ta_passInput, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ta_passInput, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ta_passInput, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ta_passInput, lv_color_make(0x2d, 0x2b, 0x2b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_ta_passInput, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_ta_passInput, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ta_passInput, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ta_passInput, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ta_passInput, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_ta_passInput, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_ta_passInput. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_passInput, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_passInput, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_passInput, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//use keyboard on Home_ta_passInput
	lv_obj_add_event_cb(ui->Home_ta_passInput, ta_Home_event_cb, LV_EVENT_ALL, g_kb_Home);

	//Write codes Home_label_IP
	ui->Home_label_IP = lv_label_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_label_IP, 40, 16);
	lv_obj_set_size(ui->Home_label_IP, 155, 15);
	lv_obj_set_scrollbar_mode(ui->Home_label_IP, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_IP, "IP:000.000.000.000");
	lv_label_set_long_mode(ui->Home_label_IP, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_IP. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_IP, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_IP, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_IP, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_IP, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_IP, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_IP, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_IP, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_IP, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_btn_connect
	ui->Home_btn_connect = lv_btn_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_btn_connect, 141, 213);
	lv_obj_set_size(ui->Home_btn_connect, 50, 25);
	lv_obj_set_scrollbar_mode(ui->Home_btn_connect, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_connect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_connect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_connect, lv_color_make(0x46, 0x49, 0x4c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_connect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_connect, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_connect, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_connect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_connect_label = lv_label_create(ui->Home_btn_connect);
	lv_label_set_text(ui->Home_btn_connect_label, "确定");
	lv_obj_set_style_pad_all(ui->Home_btn_connect, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_connect_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_btn_2
	ui->Home_btn_2 = lv_btn_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_btn_2, 41, 213);
	lv_obj_set_size(ui->Home_btn_2, 50, 25);
	lv_obj_set_scrollbar_mode(ui->Home_btn_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_2, lv_color_make(0x46, 0x49, 0x4c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_2, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_2_label = lv_label_create(ui->Home_btn_2);
	lv_label_set_text(ui->Home_btn_2_label, "搜索");
	lv_obj_set_style_pad_all(ui->Home_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_2_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_ddlist_ssid
	ui->Home_ddlist_ssid = lv_dropdown_create(ui->tile_config);
	lv_obj_set_pos(ui->Home_ddlist_ssid, 33, 72);
	lv_obj_set_size(ui->Home_ddlist_ssid, 166, 26);
	lv_obj_set_scrollbar_mode(ui->Home_ddlist_ssid, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->Home_ddlist_ssid, "ssid1\nssid2\nssid3");

	//Set style for Home_ddlist_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ddlist_ssid, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ddlist_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ddlist_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ddlist_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ddlist_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ddlist_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ddlist_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ddlist_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ddlist_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ddlist_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ddlist_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ddlist_ssid, lv_color_make(0xe1, 0xe6, 0xee), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ddlist_ssid, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ddlist_ssid, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ddlist_ssid, lv_color_make(0x0D, 0x30, 0x55), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_ddlist_ssid, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ddlist_ssid, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ddlist_ssid, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ddlist_ssid, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_home_ddlist_ssid_extra_list_selected_checked
	static lv_style_t style_home_ddlist_ssid_extra_list_selected_checked;
	ui_init_style(&style_home_ddlist_ssid_extra_list_selected_checked);
	lv_style_set_radius(&style_home_ddlist_ssid_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_home_ddlist_ssid_extra_list_selected_checked, lv_color_make(0x44, 0xba, 0xf7));
	lv_style_set_bg_grad_color(&style_home_ddlist_ssid_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_home_ddlist_ssid_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_home_ddlist_ssid_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_home_ddlist_ssid_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_ssid_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_home_ddlist_ssid_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_home_ddlist_ssid_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_ssid_extra_list_selected_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_ssid), &style_home_ddlist_ssid_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_home_ddlist_ssid_extra_list_main_default
	static lv_style_t style_home_ddlist_ssid_extra_list_main_default;
	ui_init_style(&style_home_ddlist_ssid_extra_list_main_default);
	lv_style_set_radius(&style_home_ddlist_ssid_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_ssid_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_home_ddlist_ssid_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home_ddlist_ssid_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_home_ddlist_ssid_extra_list_main_default, 255);
	lv_style_set_border_color(&style_home_ddlist_ssid_extra_list_main_default, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_home_ddlist_ssid_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_home_ddlist_ssid_extra_list_main_default, 255);
	lv_style_set_text_color(&style_home_ddlist_ssid_extra_list_main_default, lv_color_make(0x0D, 0x30, 0x55));
	lv_style_set_text_font(&style_home_ddlist_ssid_extra_list_main_default, &lv_font_montserratMedium_12);
	lv_style_set_max_height(&style_home_ddlist_ssid_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_ssid), &style_home_ddlist_ssid_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_home_ddlist_ssid_extra_list_scrollbar_default
	static lv_style_t style_home_ddlist_ssid_extra_list_scrollbar_default;
	ui_init_style(&style_home_ddlist_ssid_extra_list_scrollbar_default);
	lv_style_set_radius(&style_home_ddlist_ssid_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_ssid_extra_list_scrollbar_default, lv_color_make(0x61, 0x73, 0x61));
	lv_style_set_bg_opa(&style_home_ddlist_ssid_extra_list_scrollbar_default, 165);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_ssid), &style_home_ddlist_ssid_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile tile_hemo
	ui->tile_hemo = lv_tileview_add_tile(ui->Home_tileview_1, 0, 0, LV_DIR_BOTTOM);

	//Write codes Home_cont_weather
	ui->Home_cont_weather = lv_obj_create(ui->tile_hemo);
	lv_obj_set_pos(ui->Home_cont_weather, 0, 165);
	lv_obj_set_size(ui->Home_cont_weather, 239, 133);
	lv_obj_set_scrollbar_mode(ui->Home_cont_weather, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_cont_weather. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_cont_weather, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_cont_weather, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_cont_weather, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_cont_weather, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_cont_weather, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_cont_weather, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_cont_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_1
	ui->Home_img_1 = lv_img_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_img_1, -20, 0);
	lv_obj_set_size(ui->Home_img_1, 100, 100);
	lv_obj_set_scrollbar_mode(ui->Home_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_1, LV_OBJ_FLAG_CLICKABLE);

	//Update pos for widget Home_img_1
	lv_obj_update_layout(ui->Home_img_1);

	//Write animation: Home_img_1move in x direction
	lv_anim_t Home_img_1_x;
	lv_anim_init(&Home_img_1_x);
	lv_anim_set_var(&Home_img_1_x, ui->Home_img_1);
	lv_anim_set_time(&Home_img_1_x, 500);
	lv_anim_set_exec_cb(&Home_img_1_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_img_1_x, lv_obj_get_x(ui->Home_img_1), 0);
	lv_anim_set_path_cb(&Home_img_1_x, lv_anim_path_linear);
	lv_anim_start(&Home_img_1_x);
	lv_img_set_src(ui->Home_img_1, &_tianqiyeleiyu_alpha_100x100);
	lv_img_set_pivot(ui->Home_img_1, 50, 50);
	lv_img_set_angle(ui->Home_img_1, 0);

	//Write codes Home_label_temp
	ui->Home_label_temp = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_temp, 200, 18);
	lv_obj_set_size(ui->Home_label_temp, 104, 40);
	lv_obj_set_scrollbar_mode(ui->Home_label_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_temp, "32°");
	lv_label_set_long_mode(ui->Home_label_temp, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_temp, lv_color_make(0x05, 0xbc, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_temp, &lv_font_simhei_40, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_temp
	lv_obj_update_layout(ui->Home_label_temp);

	//Write animation: Home_label_tempmove in x direction
	lv_anim_t Home_label_temp_x;
	lv_anim_init(&Home_label_temp_x);
	lv_anim_set_var(&Home_label_temp_x, ui->Home_label_temp);
	lv_anim_set_time(&Home_label_temp_x, 500);
	lv_anim_set_exec_cb(&Home_label_temp_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_label_temp_x, lv_obj_get_x(ui->Home_label_temp), 106);
	lv_anim_set_path_cb(&Home_label_temp_x, lv_anim_path_linear);
	lv_anim_start(&Home_label_temp_x);

	//Write codes Home_label_addr
	ui->Home_label_addr = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_addr, -20, 104);
	lv_obj_set_size(ui->Home_label_addr, 48, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_addr, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_addr, "深圳市");
	lv_label_set_long_mode(ui->Home_label_addr, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_addr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_addr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_addr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_addr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_addr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_addr, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_addr, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_addr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_addr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_addr
	lv_obj_update_layout(ui->Home_label_addr);

	//Write animation: Home_label_addrmove in x direction
	lv_anim_t Home_label_addr_x;
	lv_anim_init(&Home_label_addr_x);
	lv_anim_set_var(&Home_label_addr_x, ui->Home_label_addr);
	lv_anim_set_time(&Home_label_addr_x, 500);
	lv_anim_set_exec_cb(&Home_label_addr_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_label_addr_x, lv_obj_get_x(ui->Home_label_addr), 11);
	lv_anim_set_path_cb(&Home_label_addr_x, lv_anim_path_linear);
	lv_anim_start(&Home_label_addr_x);

	//Write codes Home_label_weather
	ui->Home_label_weather = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_weather, 41, 104);
	lv_obj_set_size(ui->Home_label_weather, 44, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_weather, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_weather, "雷雨");
	lv_label_set_long_mode(ui->Home_label_weather, LV_LABEL_LONG_SCROLL_CIRCULAR);

	//Set style for Home_label_weather. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_weather, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_weather, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_weather, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_weather, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_weather, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_weather, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_weather, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_weather
	lv_obj_update_layout(ui->Home_label_weather);

	//Write animation: Home_label_weathermove in x direction
	lv_anim_t Home_label_weather_x;
	lv_anim_init(&Home_label_weather_x);
	lv_anim_set_var(&Home_label_weather_x, ui->Home_label_weather);
	lv_anim_set_time(&Home_label_weather_x, 500);
	lv_anim_set_exec_cb(&Home_label_weather_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_label_weather_x, lv_obj_get_x(ui->Home_label_weather), 61);
	lv_anim_set_path_cb(&Home_label_weather_x, lv_anim_path_linear);
	lv_anim_start(&Home_label_weather_x);

	//Write codes Home_label_to
	ui->Home_label_to = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_to, 108, 88);
	lv_obj_set_size(ui->Home_label_to, 37, 13);
	lv_obj_set_scrollbar_mode(ui->Home_label_to, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_to, "明天");
	lv_label_set_long_mode(ui->Home_label_to, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_to. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_to, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_to, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_to, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_to, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_to, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_to, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_to, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_to, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_to, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_to
	lv_obj_update_layout(ui->Home_label_to);

	//Write animation: Home_label_tomove in y direction
	lv_anim_t Home_label_to_y;
	lv_anim_init(&Home_label_to_y);
	lv_anim_set_var(&Home_label_to_y, ui->Home_label_to);
	lv_anim_set_time(&Home_label_to_y, 500);
	lv_anim_set_exec_cb(&Home_label_to_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_to_y, lv_obj_get_y(ui->Home_label_to), 68);
	lv_anim_set_path_cb(&Home_label_to_y, lv_anim_path_linear);
	lv_anim_start(&Home_label_to_y);


	//Write codes Home_label_day2
	ui->Home_label_day2 = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_day2, 145, 88);
	lv_obj_set_size(ui->Home_label_day2, 37, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_day2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_day2, "后天");
	lv_label_set_long_mode(ui->Home_label_day2, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_day2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_day2, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_day2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_day2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_day2
	lv_obj_update_layout(ui->Home_label_day2);

	//Write animation: Home_label_day2move in y direction
	lv_anim_t Home_label_day2_y;
	lv_anim_init(&Home_label_day2_y);
	lv_anim_set_var(&Home_label_day2_y, ui->Home_label_day2);
	lv_anim_set_time(&Home_label_day2_y, 500);
	lv_anim_set_exec_cb(&Home_label_day2_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_day2_y, lv_obj_get_y(ui->Home_label_day2), 68);
	lv_anim_set_path_cb(&Home_label_day2_y, lv_anim_path_linear);
	lv_anim_start(&Home_label_day2_y);


	//Write codes Home_label_day3
	ui->Home_label_day3 = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_day3, 184, 88);
	lv_obj_set_size(ui->Home_label_day3, 45, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_day3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_day3, "大后天");
	lv_label_set_long_mode(ui->Home_label_day3, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_day3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_day3, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_day3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_day3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_day3
	lv_obj_update_layout(ui->Home_label_day3);

	//Write animation: Home_label_day3move in y direction
	lv_anim_t Home_label_day3_y;
	lv_anim_init(&Home_label_day3_y);
	lv_anim_set_var(&Home_label_day3_y, ui->Home_label_day3);
	lv_anim_set_time(&Home_label_day3_y, 500);
	lv_anim_set_exec_cb(&Home_label_day3_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_day3_y, lv_obj_get_y(ui->Home_label_day3), 68);
	lv_anim_set_path_cb(&Home_label_day3_y, lv_anim_path_linear);
	lv_anim_start(&Home_label_day3_y);


	//Write codes Home_img_day1
	ui->Home_img_day1 = lv_img_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_img_day1, 114, 102);
	lv_obj_set_size(ui->Home_img_day1, 20, 20);
	lv_obj_set_scrollbar_mode(ui->Home_img_day1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_day1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_day1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_day1, LV_OBJ_FLAG_CLICKABLE);

	//Update pos for widget Home_img_day1
	lv_obj_update_layout(ui->Home_img_day1);

	//Write animation: Home_img_day1move in y direction
	lv_anim_t Home_img_day1_y;
	lv_anim_init(&Home_img_day1_y);
	lv_anim_set_var(&Home_img_day1_y, ui->Home_img_day1);
	lv_anim_set_time(&Home_img_day1_y, 500);
	lv_anim_set_exec_cb(&Home_img_day1_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_img_day1_y, lv_obj_get_y(ui->Home_img_day1), 82);
	lv_anim_set_path_cb(&Home_img_day1_y, lv_anim_path_linear);
	lv_anim_start(&Home_img_day1_y);

	lv_img_set_src(ui->Home_img_day1, &_tianqiqing_i_leiyu_alpha_20x20);
	lv_img_set_pivot(ui->Home_img_day1, 50, 50);
	lv_img_set_angle(ui->Home_img_day1, 0);

	//Write codes Home_img_day2
	ui->Home_img_day2 = lv_img_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_img_day2, 152, 104);
	lv_obj_set_size(ui->Home_img_day2, 20, 20);
	lv_obj_set_scrollbar_mode(ui->Home_img_day2, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_day2, LV_OBJ_FLAG_CLICKABLE);

	//Update pos for widget Home_img_day2
	lv_obj_update_layout(ui->Home_img_day2);

	//Write animation: Home_img_day2move in y direction
	lv_anim_t Home_img_day2_y;
	lv_anim_init(&Home_img_day2_y);
	lv_anim_set_var(&Home_img_day2_y, ui->Home_img_day2);
	lv_anim_set_time(&Home_img_day2_y, 500);
	lv_anim_set_exec_cb(&Home_img_day2_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_img_day2_y, lv_obj_get_y(ui->Home_img_day2), 82);
	lv_anim_set_path_cb(&Home_img_day2_y, lv_anim_path_linear);
	lv_anim_start(&Home_img_day2_y);

	lv_img_set_src(ui->Home_img_day2, &_tianqiqing_i_leiyu_alpha_20x20);
	lv_img_set_pivot(ui->Home_img_day2, 50, 50);
	lv_img_set_angle(ui->Home_img_day2, 0);

	//Write codes Home_img_day3
	ui->Home_img_day3 = lv_img_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_img_day3, 197, 102);
	lv_obj_set_size(ui->Home_img_day3, 20, 20);
	lv_obj_set_scrollbar_mode(ui->Home_img_day3, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_day3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_day3, LV_OBJ_FLAG_CLICKABLE);

	//Update pos for widget Home_img_day3
	lv_obj_update_layout(ui->Home_img_day3);

	//Write animation: Home_img_day3move in y direction
	lv_anim_t Home_img_day3_y;
	lv_anim_init(&Home_img_day3_y);
	lv_anim_set_var(&Home_img_day3_y, ui->Home_img_day3);
	lv_anim_set_time(&Home_img_day3_y, 500);
	lv_anim_set_exec_cb(&Home_img_day3_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_img_day3_y, lv_obj_get_y(ui->Home_img_day3), 82);
	lv_anim_set_path_cb(&Home_img_day3_y, lv_anim_path_linear);
	lv_anim_start(&Home_img_day3_y);

	lv_img_set_src(ui->Home_img_day3, &_tianqiqing_i_leiyu_alpha_20x20);
	lv_img_set_pivot(ui->Home_img_day3, 50, 50);
	lv_img_set_angle(ui->Home_img_day3, 0);

	//Write codes Home_label_day1_temp
	ui->Home_label_day1_temp = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_day1_temp, 114, 124);
	lv_obj_set_size(ui->Home_label_day1_temp, 28, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_day1_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_day1_temp, "25°");
	lv_label_set_long_mode(ui->Home_label_day1_temp, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_day1_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_day1_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_day1_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_day1_temp, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_day1_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_day1_temp
	lv_obj_update_layout(ui->Home_label_day1_temp);

	//Write animation: Home_label_day1_tempmove in y direction
	lv_anim_t Home_label_day1_temp_y;
	lv_anim_init(&Home_label_day1_temp_y);
	lv_anim_set_var(&Home_label_day1_temp_y, ui->Home_label_day1_temp);
	lv_anim_set_time(&Home_label_day1_temp_y, 500);
	lv_anim_set_exec_cb(&Home_label_day1_temp_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_day1_temp_y, lv_obj_get_y(ui->Home_label_day1_temp), 104);
	lv_anim_set_path_cb(&Home_label_day1_temp_y, lv_anim_path_bounce);
	lv_anim_start(&Home_label_day1_temp_y);


	//Write codes Home_label_day2_temp
	ui->Home_label_day2_temp = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_day2_temp, 152, 124);
	lv_obj_set_size(ui->Home_label_day2_temp, 27, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_day2_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_day2_temp, "25°");
	lv_label_set_long_mode(ui->Home_label_day2_temp, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_day2_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_day2_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_day2_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_day2_temp, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_day2_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_day2_temp
	lv_obj_update_layout(ui->Home_label_day2_temp);

	//Write animation: Home_label_day2_tempmove in y direction
	lv_anim_t Home_label_day2_temp_y;
	lv_anim_init(&Home_label_day2_temp_y);
	lv_anim_set_var(&Home_label_day2_temp_y, ui->Home_label_day2_temp);
	lv_anim_set_time(&Home_label_day2_temp_y, 500);
	lv_anim_set_exec_cb(&Home_label_day2_temp_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_day2_temp_y, lv_obj_get_y(ui->Home_label_day2_temp), 104);
	lv_anim_set_path_cb(&Home_label_day2_temp_y, lv_anim_path_linear);
	lv_anim_start(&Home_label_day2_temp_y);


	//Write codes Home_label_day3_temp
	ui->Home_label_day3_temp = lv_label_create(ui->Home_cont_weather);
	lv_obj_set_pos(ui->Home_label_day3_temp, 197, 124);
	lv_obj_set_size(ui->Home_label_day3_temp, 27, 11);
	lv_obj_set_scrollbar_mode(ui->Home_label_day3_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_day3_temp, "25°");
	lv_label_set_long_mode(ui->Home_label_day3_temp, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_day3_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_day3_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_day3_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_day3_temp, &lv_font_simhei_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_day3_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_day3_temp
	lv_obj_update_layout(ui->Home_label_day3_temp);

	//Write animation: Home_label_day3_tempmove in y direction
	lv_anim_t Home_label_day3_temp_y;
	lv_anim_init(&Home_label_day3_temp_y);
	lv_anim_set_var(&Home_label_day3_temp_y, ui->Home_label_day3_temp);
	lv_anim_set_time(&Home_label_day3_temp_y, 500);
	lv_anim_set_exec_cb(&Home_label_day3_temp_y, (lv_anim_exec_xcb_t)lv_obj_set_y);
	lv_anim_set_values(&Home_label_day3_temp_y, lv_obj_get_y(ui->Home_label_day3_temp), 104);
	lv_anim_set_path_cb(&Home_label_day3_temp_y, lv_anim_path_linear);
	lv_anim_start(&Home_label_day3_temp_y);


	//Write codes Home_time
	ui->Home_time = lv_obj_create(ui->tile_hemo);
	lv_obj_set_pos(ui->Home_time, 0, 46);
	lv_obj_set_size(ui->Home_time, 240, 112);
	lv_obj_set_scrollbar_mode(ui->Home_time, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_time. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_time, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_time, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_time, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_time, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_time, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool Home_digital_clock_1_timer_enabled = false;

	//Write codes Home_digital_clock_1
	ui->Home_digital_clock_1 = lv_dclock_create(ui->Home_time, "11:25:50");
	lv_obj_set_style_text_align(ui->Home_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->Home_digital_clock_1, -20, 0);
	lv_obj_set_size(ui->Home_digital_clock_1, 130, 50);

	//create timer
	if (!Home_digital_clock_1_timer_enabled) {
		lv_timer_create(Home_digital_clock_1_timer, 1000, NULL);
		Home_digital_clock_1_timer_enabled = true;
	}
	//Set style for Home_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_digital_clock_1, 238, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xF6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_digital_clock_1, &lv_font_Antonio_Regular_40, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_digital_clock_1
	lv_obj_update_layout(ui->Home_digital_clock_1);

	//Write animation: Home_digital_clock_1move in x direction
	lv_anim_t Home_digital_clock_1_x;
	lv_anim_init(&Home_digital_clock_1_x);
	lv_anim_set_var(&Home_digital_clock_1_x, ui->Home_digital_clock_1);
	lv_anim_set_time(&Home_digital_clock_1_x, 500);
	lv_anim_set_exec_cb(&Home_digital_clock_1_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_digital_clock_1_x, lv_obj_get_x(ui->Home_digital_clock_1), 49);
	lv_anim_set_path_cb(&Home_digital_clock_1_x, lv_anim_path_ease_in_out);
	lv_anim_start(&Home_digital_clock_1_x);

	//Write codes Home_label_date
	ui->Home_label_date = lv_label_create(ui->Home_time);
	lv_obj_set_pos(ui->Home_label_date, -20, 73);
	lv_obj_set_size(ui->Home_label_date, 121, 15);
	lv_obj_set_scrollbar_mode(ui->Home_label_date, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_date, "2023年06月26日");
	lv_label_set_long_mode(ui->Home_label_date, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_date. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_date, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_date, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_date, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_date, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_date
	lv_obj_update_layout(ui->Home_label_date);

	//Write animation: Home_label_datemove in x direction
	lv_anim_t Home_label_date_x;
	lv_anim_init(&Home_label_date_x);
	lv_anim_set_var(&Home_label_date_x, ui->Home_label_date);
	lv_anim_set_time(&Home_label_date_x, 500);
	lv_anim_set_exec_cb(&Home_label_date_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_label_date_x, lv_obj_get_x(ui->Home_label_date), 22);
	lv_anim_set_path_cb(&Home_label_date_x, lv_anim_path_linear);
	lv_anim_start(&Home_label_date_x);

	//Write codes Home_label_wday
	ui->Home_label_wday = lv_label_create(ui->Home_time);
	lv_obj_set_pos(ui->Home_label_wday, 190, 73);
	lv_obj_set_size(ui->Home_label_wday, 61, 18);
	lv_obj_set_scrollbar_mode(ui->Home_label_wday, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_wday, "星期一");
	lv_label_set_long_mode(ui->Home_label_wday, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_wday. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_wday, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_wday, lv_color_make(0xd7, 0xa7, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_wday, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_wday, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_wday, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update pos for widget Home_label_wday
	lv_obj_update_layout(ui->Home_label_wday);

	//Write animation: Home_label_wdaymove in x direction
	lv_anim_t Home_label_wday_x;
	lv_anim_init(&Home_label_wday_x);
	lv_anim_set_var(&Home_label_wday_x, ui->Home_label_wday);
	lv_anim_set_time(&Home_label_wday_x, 500);
	lv_anim_set_exec_cb(&Home_label_wday_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&Home_label_wday_x, lv_obj_get_x(ui->Home_label_wday), 158);
	lv_anim_set_path_cb(&Home_label_wday_x, lv_anim_path_linear);
	lv_anim_start(&Home_label_wday_x);

	//Write codes Home_cont_1
	ui->Home_cont_1 = lv_obj_create(ui->tile_hemo);
	lv_obj_set_pos(ui->Home_cont_1, 0, 0);
	lv_obj_set_size(ui->Home_cont_1, 240, 30);
	lv_obj_set_scrollbar_mode(ui->Home_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_cont_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_wifi
	ui->Home_img_wifi = lv_img_create(ui->Home_cont_1);
	lv_obj_set_pos(ui->Home_img_wifi, 106, 5);
	lv_obj_set_size(ui->Home_img_wifi, 16, 16);
	lv_obj_set_scrollbar_mode(ui->Home_img_wifi, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_wifi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_wifi, &_no_internet_alpha_16x16);
	lv_img_set_pivot(ui->Home_img_wifi, 50, 50);
	lv_img_set_angle(ui->Home_img_wifi, 0);

	//add new tile tile_lock
	ui->tile_lock = lv_tileview_add_tile(ui->Home_tileview_1, 0, 2, LV_DIR_TOP);

	//Write codes Home_label_pin
	ui->Home_label_pin = lv_label_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_label_pin, 40, 50);
	lv_obj_set_size(ui->Home_label_pin, 44, 17);
	lv_obj_set_scrollbar_mode(ui->Home_label_pin, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_pin, "PIN:");
	lv_label_set_long_mode(ui->Home_label_pin, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_pin. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_pin, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_pin, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_pin, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_pin, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_pin, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_pin, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_pin, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_pin, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_pin, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_pin, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_label_pin, LV_OBJ_FLAG_HIDDEN);
	//Write codes Home_ta_PIN
	ui->Home_ta_PIN = lv_textarea_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_ta_PIN, 49, 68);
	lv_obj_set_size(ui->Home_ta_PIN, 141, 25);
	lv_obj_set_scrollbar_mode(ui->Home_ta_PIN, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_ta_PIN. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_PIN, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_PIN, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ta_PIN, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ta_PIN, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_PIN, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ta_PIN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ta_PIN, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ta_PIN, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ta_PIN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ta_PIN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ta_PIN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ta_PIN, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ta_PIN, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ta_PIN, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ta_PIN, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_ta_PIN, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_ta_PIN, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_ta_PIN, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ta_PIN, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ta_PIN, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ta_PIN, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_ta_PIN, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_ta_PIN. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_PIN, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_PIN, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_PIN, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_password_mode(ui->Home_ta_PIN, true);
	// lv_textarea_set_text(ui->Home_ta_PIN, "123456");
	lv_textarea_set_text(ui->Home_ta_PIN, "123456");
	//use keyboard on Home_ta_PIN

	lv_obj_add_event_cb(ui->Home_ta_PIN, ta_Home_event_cb, LV_EVENT_ALL, g_kb_numble);
	lv_obj_add_flag(ui->Home_ta_PIN, LV_OBJ_FLAG_HIDDEN);
	//Write codes Home_label_timer
	ui->Home_label_timer = lv_label_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_label_timer, 40, 102);
	lv_obj_set_size(ui->Home_label_timer, 90, 18);
	lv_obj_set_scrollbar_mode(ui->Home_label_timer, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_timer, "TimeOut:");
	lv_label_set_long_mode(ui->Home_label_timer, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_timer. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_timer, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_timer, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_timer, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_timer, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_timer, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_timer, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_timer, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_timer, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_timer, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_ta_min
	ui->Home_ta_min = lv_textarea_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_ta_min, 49, 123);
	lv_obj_set_size(ui->Home_ta_min, 44, 25);
	lv_obj_set_scrollbar_mode(ui->Home_ta_min, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_ta_min. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_min, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_min, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ta_min, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ta_min, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ta_min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ta_min, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ta_min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ta_min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ta_min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ta_min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ta_min, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ta_min, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ta_min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ta_min, lv_color_make(0xfb, 0xfb, 0xfb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_ta_min, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_ta_min, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_ta_min, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ta_min, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ta_min, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ta_min, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_ta_min, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_ta_min. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_min, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_min, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_min, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	char* timer_str = pvPortMalloc(3);
	memset(timer_str, 0, 3);
	sprintf(timer_str, "%d", ui->timerout>60?ui->timerout/60:0);
	lv_textarea_set_text(ui->Home_ta_min, timer_str);

	//use keyboard on Home_ta_min
	lv_obj_add_event_cb(ui->Home_ta_min, ta_Home_event_cb, LV_EVENT_ALL, g_kb_numble);

	//Write codes Home_ta_sec
	ui->Home_ta_sec = lv_textarea_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_ta_sec, 112, 123);
	lv_obj_set_size(ui->Home_ta_sec, 38, 25);
	lv_obj_set_scrollbar_mode(ui->Home_ta_sec, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_ta_sec. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_sec, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_sec, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ta_sec, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ta_sec, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_sec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ta_sec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ta_sec, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ta_sec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ta_sec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ta_sec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ta_sec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ta_sec, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ta_sec, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ta_sec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ta_sec, lv_color_make(0xfe, 0xfe, 0xfe), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_ta_sec, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_ta_sec, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_ta_sec, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ta_sec, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ta_sec, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ta_sec, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_ta_sec, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_ta_sec. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_sec, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_sec, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_sec, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	memset(timer_str, 0, 3);
	sprintf(timer_str, "%d", ui->timerout>60?ui->timerout%60:ui->timerout);

	lv_textarea_set_text(ui->Home_ta_sec, timer_str);
	vPortFree(timer_str);
	//use keyboard on Home_ta_sec
	lv_obj_add_event_cb(ui->Home_ta_sec, ta_Home_event_cb, LV_EVENT_ALL, g_kb_numble);

	//Write codes Home_label_2
	ui->Home_label_2 = lv_label_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_label_2, 91, 124);
	lv_obj_set_size(ui->Home_label_2, 20, 23);
	lv_obj_set_scrollbar_mode(ui->Home_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_2, ":");
	lv_label_set_long_mode(ui->Home_label_2, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_2, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_label_3
	ui->Home_label_3 = lv_label_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_label_3, 150, 130);
	lv_obj_set_size(ui->Home_label_3, 34, 14);
	lv_obj_set_scrollbar_mode(ui->Home_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_3, "秒/S");
	lv_label_set_long_mode(ui->Home_label_3, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_3, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_btn_3
	ui->Home_btn_3 = lv_btn_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_btn_3, 86, 198);
	lv_obj_set_size(ui->Home_btn_3, 58, 28);
	lv_obj_set_scrollbar_mode(ui->Home_btn_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_3, lv_color_make(0x30, 0x30, 0x30), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_3, lv_color_make(0xf3, 0xf3, 0xf3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_3, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_3_label = lv_label_create(ui->Home_btn_3);
	lv_label_set_text(ui->Home_btn_3_label, "确定");
	lv_obj_set_style_pad_all(ui->Home_btn_3, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_3_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_label_Tile
	ui->Home_label_Tile = lv_label_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_label_Tile, 69, 12);
	lv_obj_set_size(ui->Home_label_Tile, 110, 16);
	lv_obj_set_scrollbar_mode(ui->Home_label_Tile, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_Tile, "雷达检测配置");
	lv_label_set_long_mode(ui->Home_label_Tile, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_Tile. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_Tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_Tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_Tile, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_Tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_Tile, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_Tile, lv_color_make(0xe2, 0x9c, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_Tile, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_Tile, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_Tile, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_Tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_ddlist_key
	ui->Home_ddlist_key = lv_dropdown_create(ui->tile_lock);
	lv_obj_set_pos(ui->Home_ddlist_key, 40, 61);
	lv_obj_set_size(ui->Home_ddlist_key, 143, 28);
	lv_obj_set_scrollbar_mode(ui->Home_ddlist_key, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->Home_ddlist_key, "USB\nBLE");

	lv_dropdown_set_selected(ui->Home_ddlist_key, ui->keyboard_con_type);

	//Set style for Home_ddlist_key. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ddlist_key, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ddlist_key, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ddlist_key, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ddlist_key, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ddlist_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ddlist_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ddlist_key, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ddlist_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ddlist_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ddlist_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ddlist_key, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ddlist_key, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ddlist_key, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ddlist_key, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ddlist_key, lv_color_make(0xf9, 0xf9, 0xf9), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_ddlist_key, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ddlist_key, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ddlist_key, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ddlist_key, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_home_ddlist_key_extra_list_selected_checked
	static lv_style_t style_home_ddlist_key_extra_list_selected_checked;
	ui_init_style(&style_home_ddlist_key_extra_list_selected_checked);
	lv_style_set_radius(&style_home_ddlist_key_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_home_ddlist_key_extra_list_selected_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_home_ddlist_key_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_home_ddlist_key_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_home_ddlist_key_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_home_ddlist_key_extra_list_selected_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_home_ddlist_key_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_home_ddlist_key_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_home_ddlist_key_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_home_ddlist_key_extra_list_selected_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_key), &style_home_ddlist_key_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_home_ddlist_key_extra_list_main_default
	static lv_style_t style_home_ddlist_key_extra_list_main_default;
	ui_init_style(&style_home_ddlist_key_extra_list_main_default);
	lv_style_set_radius(&style_home_ddlist_key_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_key_extra_list_main_default, lv_color_make(0x0a, 0x0a, 0x0a));
	lv_style_set_bg_grad_color(&style_home_ddlist_key_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home_ddlist_key_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_home_ddlist_key_extra_list_main_default, 255);
	lv_style_set_border_color(&style_home_ddlist_key_extra_list_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_home_ddlist_key_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_home_ddlist_key_extra_list_main_default, 255);
	lv_style_set_text_color(&style_home_ddlist_key_extra_list_main_default, lv_color_make(0xf2, 0xf2, 0xf2));
	lv_style_set_text_font(&style_home_ddlist_key_extra_list_main_default, &lv_font_montserratMedium_12);
	lv_style_set_max_height(&style_home_ddlist_key_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_key), &style_home_ddlist_key_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_home_ddlist_key_extra_list_scrollbar_default
	static lv_style_t style_home_ddlist_key_extra_list_scrollbar_default;
	ui_init_style(&style_home_ddlist_key_extra_list_scrollbar_default);
	lv_style_set_radius(&style_home_ddlist_key_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_home_ddlist_key_extra_list_scrollbar_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_opa(&style_home_ddlist_key_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->Home_ddlist_key), &style_home_ddlist_key_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	//Write codes Home_cont_loding
	ui->Home_cont_loding = lv_obj_create(ui->Home);
	lv_obj_set_pos(ui->Home_cont_loding, 0, 0);
	lv_obj_set_size(ui->Home_cont_loding, 240, 320);
	lv_obj_set_scrollbar_mode(ui->Home_cont_loding, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_cont_loding. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_cont_loding, lv_color_make(0x71, 0x71, 0x71), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_cont_loding, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_cont_loding, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_cont_loding, 161, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_cont_loding, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_cont_loding, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_cont_loding, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_cont_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget Home_cont_loding
	lv_obj_add_flag(ui->Home_cont_loding, LV_OBJ_FLAG_HIDDEN);
	//Write codes Home_img_loding
	ui->Home_img_loding = lv_img_create(ui->Home_cont_loding);
	lv_obj_set_pos(ui->Home_img_loding, 65, 97);
	lv_obj_set_size(ui->Home_img_loding, 100, 100);
	lv_obj_set_scrollbar_mode(ui->Home_img_loding, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_loding. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_loding, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_loding, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_loding, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_loding, &_loading_alpha_100x100);
	lv_img_set_pivot(ui->Home_img_loding, 50, 50);
	lv_img_set_angle(ui->Home_img_loding, 0);

	//Init events for screen
	events_init_Home(ui);

}