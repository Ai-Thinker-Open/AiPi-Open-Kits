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

static char* wday_str[7] = { "周天","周一","周二","周三","周四","周五","周六" };

static char* wday_str_en[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri","Sat" };
uint16_t yesr = 2023;
uint16_t mon = 11;
uint16_t day = 19;
uint16_t wday = 0;
uint16_t hour = 0;
uint16_t min = 0;
uint16_t sec = 0;

void screen_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&hour, &min, &sec);
	if (lv_obj_is_valid(guider_ui.screen_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.screen_digital_clock_1, "%02d:%02d", hour, min);
		lv_label_set_text_fmt(guider_ui.screen_label_day, "%02d/%02d", mon, day);
		lv_label_set_text(guider_ui.screen_label_wday, r2_device->language?  wday_str_en[wday]:wday_str[wday]);
	}
}


void screen_digital_clock_2_timer(lv_timer_t* timer)
{
	clock_count_24(&hour, &min, &sec);
	if (lv_obj_is_valid(guider_ui.screen_digital_clock_2))
	{
		lv_dclock_set_text_fmt(guider_ui.screen_digital_clock_2, "%02d:%02d", hour, min);
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
	lv_obj_set_style_bg_color(ui->screen, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_bg
	ui->screen_img_bg = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_bg, 0, 0);
	lv_obj_set_size(ui->screen_img_bg, 480, 480);
	lv_obj_set_scrollbar_mode(ui->screen_img_bg, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_bg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_bg, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_bg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_bg, &_R286_bg_alpha_480x480);
	lv_img_set_pivot(ui->screen_img_bg, 50, 50);
	lv_img_set_angle(ui->screen_img_bg, 0);

	//Write codes screen_cont_main
	ui->screen_cont_main = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_main, 0, 0);
	lv_obj_set_size(ui->screen_cont_main, 480, 480);
	lv_obj_set_scrollbar_mode(ui->screen_cont_main, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_main. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_main, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_main, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_main, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_main, 53, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_main, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_main, lv_color_make(0xff, 0x00, 0x3b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_main, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_date
	ui->screen_cont_date = lv_obj_create(ui->screen_cont_main);
	lv_obj_set_pos(ui->screen_cont_date, 4, 46);
	lv_obj_set_size(ui->screen_cont_date, 259, 193);
	lv_obj_set_scrollbar_mode(ui->screen_cont_date, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_date. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_date, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_date, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_date, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_date, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_date, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool screen_digital_clock_1_timer_enabled = false;

	//Write codes screen_digital_clock_1
	ui->screen_digital_clock_1 = lv_dclock_create(ui->screen_cont_date, "23:55");
	lv_obj_set_style_text_align(ui->screen_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->screen_digital_clock_1, 14, 35);
	lv_obj_set_size(ui->screen_digital_clock_1, 218, 81);

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
	lv_obj_set_style_text_color(ui->screen_digital_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_digital_clock_1, &lv_font_MiSans_Demibold_70, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_wday
	ui->screen_label_wday = lv_label_create(ui->screen_cont_date);
	lv_obj_set_pos(ui->screen_label_wday, 125, 115);
	lv_obj_set_size(ui->screen_label_wday, 106, 44);
	lv_obj_set_scrollbar_mode(ui->screen_label_wday, LV_SCROLLBAR_MODE_OFF);
	if (r2_device!=NULL)
		lv_label_set_text(ui->screen_label_wday, wday_str_en[1]);
	else
		lv_label_set_text(ui->screen_label_wday, "周一");

	lv_label_set_long_mode(ui->screen_label_wday, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_wday. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_wday, lv_color_make(0x09, 0x09, 0x09), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_wday, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_wday, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_wday, lv_color_make(0x7d, 0x7d, 0x7d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_wday, &lv_font_MiSans_Normal_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_wday, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_wday, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_wday, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_day
	ui->screen_label_day = lv_label_create(ui->screen_cont_date);
	lv_obj_set_pos(ui->screen_label_day, 8, 115);
	lv_obj_set_size(ui->screen_label_day, 130, 45);
	lv_obj_set_scrollbar_mode(ui->screen_label_day, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_day, "10/23");
	lv_label_set_long_mode(ui->screen_label_day, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_day. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_day, lv_color_make(0x09, 0x09, 0x09), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_day, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_day, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_day, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_day, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_day, lv_color_make(0x7d, 0x7d, 0x7d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_day, &lv_font_MiSans_Normal_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_day, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_day, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_day, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_day, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_ligth3
	ui->screen_cont_ligth3 = lv_obj_create(ui->screen_cont_main);
	lv_obj_set_pos(ui->screen_cont_ligth3, 324, 209);
	lv_obj_set_size(ui->screen_cont_ligth3, 144, 70);
	lv_obj_set_scrollbar_mode(ui->screen_cont_ligth3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_ligth3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_ligth3, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_ligth3, lv_color_make(0x24, 0x22, 0x2e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_ligth3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_ligth3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_ligth3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_ligth3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_ligth3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_ligth3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_ligth3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_ligth3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_sw3
	ui->screen_imgbtn_sw3 = lv_imgbtn_create(ui->screen_cont_ligth3);
	lv_obj_set_pos(ui->screen_imgbtn_sw3, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_sw3, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_sw3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_sw3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_sw3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_sw3. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_sw3. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw3, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw3, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_sw3, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_ddlist_list3
	ui->screen_ddlist_list3 = lv_dropdown_create(ui->screen_cont_ligth3);
	lv_obj_set_pos(ui->screen_ddlist_list3, 40, 18);
	lv_obj_set_size(ui->screen_ddlist_list3, 99, 29);
	lv_obj_set_scrollbar_mode(ui->screen_ddlist_list3, LV_SCROLLBAR_MODE_OFF);

	if (r2_device!=NULL)
		lv_dropdown_set_options(ui->screen_ddlist_list3, r2_device->language?"Bedroom\nHall\nKitchen":"卧室\n大厅\n厨房");
	else
		lv_dropdown_set_options(ui->screen_ddlist_list3, "卧室\n大厅\n厨房");

	lv_dropdown_set_symbol(ui->screen_ddlist_list3, NULL);

	lv_dropdown_set_selected(ui->screen_ddlist_list3, 2);

	//Set style for screen_ddlist_list3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ddlist_list3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ddlist_list3, lv_color_make(0x29, 0x29, 0x29), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_ddlist_list3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_list3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_ddlist_list3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_ddlist_list3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_ddlist_list3, lv_color_make(0xe1, 0xe6, 0xee), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ddlist_list3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_ddlist_list3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ddlist_list3, lv_color_make(0xb7, 0xb7, 0xb7), LV_PART_MAIN|LV_STATE_DEFAULT);

	if (r2_device!=NULL)
		lv_obj_set_style_text_font(ui->screen_ddlist_list3, r2_device->language?&lv_font_montserrat_16: &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	else
		lv_obj_set_style_text_font(ui->screen_ddlist_list3, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);

	lv_obj_set_style_pad_left(ui->screen_ddlist_list3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ddlist_list3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ddlist_list3, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_screen_ddlist_list3_extra_list_selected_checked
	static lv_style_t style_screen_ddlist_list3_extra_list_selected_checked;
	ui_init_style(&style_screen_ddlist_list3_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_ddlist_list3_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list3_extra_list_selected_checked, lv_color_make(0x5b, 0x5b, 0x5b));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list3_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list3_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list3_extra_list_selected_checked, 64);
	lv_style_set_border_color(&style_screen_ddlist_list3_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_ddlist_list3_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list3_extra_list_selected_checked, 38);
	lv_style_set_text_color(&style_screen_ddlist_list3_extra_list_selected_checked, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list3_extra_list_selected_checked, &lv_font_MiSans_Demibold_16);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list3), &style_screen_ddlist_list3_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list3_extra_list_main_default
	static lv_style_t style_screen_ddlist_list3_extra_list_main_default;
	ui_init_style(&style_screen_ddlist_list3_extra_list_main_default);
	lv_style_set_radius(&style_screen_ddlist_list3_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list3_extra_list_main_default, lv_color_make(0x24, 0x22, 0x2e));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list3_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list3_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list3_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_ddlist_list3_extra_list_main_default, lv_color_make(0x49, 0x45, 0x45));
	lv_style_set_border_width(&style_screen_ddlist_list3_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list3_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_ddlist_list3_extra_list_main_default, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list3_extra_list_main_default, &lv_font_MiSans_Demibold_16);
	lv_style_set_max_height(&style_screen_ddlist_list3_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list3), &style_screen_ddlist_list3_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list3_extra_list_scrollbar_default
	static lv_style_t style_screen_ddlist_list3_extra_list_scrollbar_default;
	ui_init_style(&style_screen_ddlist_list3_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_ddlist_list3_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list3_extra_list_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_opa(&style_screen_ddlist_list3_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list3), &style_screen_ddlist_list3_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_on3
	ui->screen_imgbtn_on3 = lv_imgbtn_create(ui->screen_cont_ligth3);
	lv_obj_set_pos(ui->screen_imgbtn_on3, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_on3, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_on3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_on3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_on3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_on3. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_on3. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on3, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget screen_imgbtn_on3
	lv_obj_add_flag(ui->screen_imgbtn_on3, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->screen_imgbtn_on3, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on3, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_on3, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_cont_ligth2
	ui->screen_cont_ligth2 = lv_obj_create(ui->screen_cont_main);
	lv_obj_set_pos(ui->screen_cont_ligth2, 324, 132);
	lv_obj_set_size(ui->screen_cont_ligth2, 144, 70);
	lv_obj_set_scrollbar_mode(ui->screen_cont_ligth2, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_ligth2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_ligth2, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_ligth2, lv_color_make(0x24, 0x22, 0x2e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_ligth2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_ligth2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_ligth2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_ligth2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_ligth2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_ligth2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_ligth2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_ligth2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_sw2
	ui->screen_imgbtn_sw2 = lv_imgbtn_create(ui->screen_cont_ligth2);
	lv_obj_set_pos(ui->screen_imgbtn_sw2, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_sw2, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_sw2, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_sw2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_sw2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_sw2. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_sw2. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw2, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw2, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_sw2, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_ddlist_list2
	ui->screen_ddlist_list2 = lv_dropdown_create(ui->screen_cont_ligth2);
	lv_obj_set_pos(ui->screen_ddlist_list2, 40, 18);
	lv_obj_set_size(ui->screen_ddlist_list2, 99, 29);
	lv_obj_set_scrollbar_mode(ui->screen_ddlist_list2, LV_SCROLLBAR_MODE_OFF);
	if (r2_device!=NULL)
		lv_dropdown_set_options(ui->screen_ddlist_list2, r2_device->language?"Bedroom\nHall\nKitchen":"卧室\n大厅\n厨房");
	else lv_dropdown_set_options(ui->screen_ddlist_list2, "卧室\n大厅\n厨房");
	lv_dropdown_set_symbol(ui->screen_ddlist_list2, NULL);
	lv_dropdown_set_selected(ui->screen_ddlist_list2, 1);
	//Set style for screen_ddlist_list2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ddlist_list2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ddlist_list2, lv_color_make(0x29, 0x29, 0x29), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_ddlist_list2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_list2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_ddlist_list2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_ddlist_list2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_ddlist_list2, lv_color_make(0xe1, 0xe6, 0xee), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ddlist_list2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_ddlist_list2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ddlist_list2, lv_color_make(0xb7, 0xb7, 0xb7), LV_PART_MAIN|LV_STATE_DEFAULT);
	if (r2_device!=NULL)
		lv_obj_set_style_text_font(ui->screen_ddlist_list2, r2_device->language?&lv_font_montserrat_16: &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	else
		lv_obj_set_style_text_font(ui->screen_ddlist_list2, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ddlist_list2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ddlist_list2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ddlist_list2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_screen_ddlist_list2_extra_list_selected_checked
	static lv_style_t style_screen_ddlist_list2_extra_list_selected_checked;
	ui_init_style(&style_screen_ddlist_list2_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_ddlist_list2_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list2_extra_list_selected_checked, lv_color_make(0x5b, 0x5b, 0x5b));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list2_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list2_extra_list_selected_checked, 64);
	lv_style_set_border_color(&style_screen_ddlist_list2_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_ddlist_list2_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list2_extra_list_selected_checked, 38);
	lv_style_set_text_color(&style_screen_ddlist_list2_extra_list_selected_checked, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list2_extra_list_selected_checked, &lv_font_MiSans_Demibold_16);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list2), &style_screen_ddlist_list2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list2_extra_list_main_default
	static lv_style_t style_screen_ddlist_list2_extra_list_main_default;
	ui_init_style(&style_screen_ddlist_list2_extra_list_main_default);
	lv_style_set_radius(&style_screen_ddlist_list2_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list2_extra_list_main_default, lv_color_make(0x24, 0x22, 0x2e));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list2_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list2_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list2_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_ddlist_list2_extra_list_main_default, lv_color_make(0x49, 0x45, 0x45));
	lv_style_set_border_width(&style_screen_ddlist_list2_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list2_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_ddlist_list2_extra_list_main_default, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list2_extra_list_main_default, &lv_font_MiSans_Demibold_16);
	lv_style_set_max_height(&style_screen_ddlist_list2_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list2), &style_screen_ddlist_list2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list2_extra_list_scrollbar_default
	static lv_style_t style_screen_ddlist_list2_extra_list_scrollbar_default;
	ui_init_style(&style_screen_ddlist_list2_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_ddlist_list2_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list2_extra_list_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_opa(&style_screen_ddlist_list2_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list2), &style_screen_ddlist_list2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_on2
	ui->screen_imgbtn_on2 = lv_imgbtn_create(ui->screen_cont_ligth2);
	lv_obj_set_pos(ui->screen_imgbtn_on2, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_on2, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_on2, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_on2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_on2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_on2. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_on2. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on2, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget screen_imgbtn_on2
	lv_obj_add_flag(ui->screen_imgbtn_on2, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->screen_imgbtn_on2, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on2, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_on2, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_cont_ligth1
	ui->screen_cont_ligth1 = lv_obj_create(ui->screen_cont_main);
	lv_obj_set_pos(ui->screen_cont_ligth1, 324, 55);
	lv_obj_set_size(ui->screen_cont_ligth1, 144, 70);
	lv_obj_set_scrollbar_mode(ui->screen_cont_ligth1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_ligth1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_ligth1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_ligth1, lv_color_make(0x24, 0x22, 0x2e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_ligth1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_ligth1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_ligth1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_ligth1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_ligth1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_ligth1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_ligth1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_ligth1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_sw1
	ui->screen_imgbtn_sw1 = lv_imgbtn_create(ui->screen_cont_ligth1);
	lv_obj_set_pos(ui->screen_imgbtn_sw1, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_sw1, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_sw1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_sw1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_sw1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_sw1. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_sw1. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_sw1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_sw1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_sw1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_sw1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_sw1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw1, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw1, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_off_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_sw1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_off_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_sw1, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_ddlist_list1
	ui->screen_ddlist_list1 = lv_dropdown_create(ui->screen_cont_ligth1);
	lv_obj_set_pos(ui->screen_ddlist_list1, 40, 18);
	lv_obj_set_size(ui->screen_ddlist_list1, 99, 29);
	lv_obj_set_scrollbar_mode(ui->screen_ddlist_list1, LV_SCROLLBAR_MODE_OFF);
	if (r2_device!=NULL)
		lv_dropdown_set_options(ui->screen_ddlist_list1, r2_device->language?"Bedroom\nHall\nKitchen":"卧室\n大厅\n厨房");
	else lv_dropdown_set_options(ui->screen_ddlist_list1, "卧室\n大厅\n厨房");
	lv_dropdown_set_symbol(ui->screen_ddlist_list1, NULL);

	//Set style for screen_ddlist_list1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_ddlist_list1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ddlist_list1, lv_color_make(0x29, 0x29, 0x29), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_ddlist_list1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_list1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_ddlist_list1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_ddlist_list1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_ddlist_list1, lv_color_make(0xe1, 0xe6, 0xee), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ddlist_list1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_ddlist_list1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ddlist_list1, lv_color_make(0xb7, 0xb7, 0xb7), LV_PART_MAIN|LV_STATE_DEFAULT);
	if (r2_device!=NULL)
		lv_obj_set_style_text_font(ui->screen_ddlist_list1, r2_device->language?&lv_font_montserrat_16: &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	else
		lv_obj_set_style_text_font(ui->screen_ddlist_list1, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ddlist_list1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ddlist_list1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ddlist_list1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_screen_ddlist_list1_extra_list_selected_checked
	static lv_style_t style_screen_ddlist_list1_extra_list_selected_checked;
	ui_init_style(&style_screen_ddlist_list1_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_ddlist_list1_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list1_extra_list_selected_checked, lv_color_make(0x5b, 0x5b, 0x5b));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list1_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list1_extra_list_selected_checked, 64);
	lv_style_set_border_color(&style_screen_ddlist_list1_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_screen_ddlist_list1_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list1_extra_list_selected_checked, 38);
	lv_style_set_text_color(&style_screen_ddlist_list1_extra_list_selected_checked, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list1_extra_list_selected_checked, &lv_font_MiSans_Demibold_16);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list1), &style_screen_ddlist_list1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list1_extra_list_main_default
	static lv_style_t style_screen_ddlist_list1_extra_list_main_default;
	ui_init_style(&style_screen_ddlist_list1_extra_list_main_default);
	lv_style_set_radius(&style_screen_ddlist_list1_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list1_extra_list_main_default, lv_color_make(0x24, 0x22, 0x2e));
	lv_style_set_bg_grad_color(&style_screen_ddlist_list1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_list1_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_ddlist_list1_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_ddlist_list1_extra_list_main_default, lv_color_make(0x49, 0x45, 0x45));
	lv_style_set_border_width(&style_screen_ddlist_list1_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_ddlist_list1_extra_list_main_default, 255);
	lv_style_set_text_color(&style_screen_ddlist_list1_extra_list_main_default, lv_color_make(0xb7, 0xb7, 0xb7));
	lv_style_set_text_font(&style_screen_ddlist_list1_extra_list_main_default, &lv_font_MiSans_Demibold_16);
	lv_style_set_max_height(&style_screen_ddlist_list1_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list1), &style_screen_ddlist_list1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_screen_ddlist_list1_extra_list_scrollbar_default
	static lv_style_t style_screen_ddlist_list1_extra_list_scrollbar_default;
	ui_init_style(&style_screen_ddlist_list1_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_ddlist_list1_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_ddlist_list1_extra_list_scrollbar_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_opa(&style_screen_ddlist_list1_extra_list_scrollbar_default, 255);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_list1), &style_screen_ddlist_list1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_on1
	ui->screen_imgbtn_on1 = lv_imgbtn_create(ui->screen_cont_ligth1);
	lv_obj_set_pos(ui->screen_imgbtn_on1, -2, 3);
	lv_obj_set_size(ui->screen_imgbtn_on1, 70, 70);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_on1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_on1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_on1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_on1. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_on1. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_on1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_on1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_on1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_on1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_on1, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget screen_imgbtn_on1
	lv_obj_add_flag(ui->screen_imgbtn_on1, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->screen_imgbtn_on1, LV_IMGBTN_STATE_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on1, LV_IMGBTN_STATE_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_ligth_on_alpha_70x70, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_on1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_ligths_on_alpha_70x70, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_on1, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_img_aixingyun
	ui->screen_img_aixingyun = lv_img_create(ui->screen_cont_main);
	lv_obj_set_pos(ui->screen_img_aixingyun, 363, 370);
	lv_obj_set_size(ui->screen_img_aixingyun, 80, 82);
	lv_obj_set_scrollbar_mode(ui->screen_img_aixingyun, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_aixingyun. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_aixingyun, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_aixingyun, 111, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_aixingyun, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_aixingyun, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_aixingyun, &_logo2_alpha_80x82);
	lv_img_set_pivot(ui->screen_img_aixingyun, 50, 50);
	lv_img_set_angle(ui->screen_img_aixingyun, 0);

	//Write codes screen_cont_title
	ui->screen_cont_title = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_title, 0, 0);
	lv_obj_set_size(ui->screen_cont_title, 480, 30);
	lv_obj_set_scrollbar_mode(ui->screen_cont_title, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_cont_title. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_title, lv_color_make(0x45, 0x45, 0x45), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_cont_title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_title, 42, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_cont_title, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_cont_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_title, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_cont_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_titlebg
	ui->screen_img_titlebg = lv_img_create(ui->screen_cont_title);
	lv_obj_set_pos(ui->screen_img_titlebg, 377, -77);
	lv_obj_set_size(ui->screen_img_titlebg, 480, 21);
	lv_obj_set_scrollbar_mode(ui->screen_img_titlebg, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_titlebg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_titlebg, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_titlebg, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_titlebg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_titlebg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_titlebg, &_Tilie_bg_alpha_480x21);
	lv_img_set_pivot(ui->screen_img_titlebg, 50, 50);
	lv_img_set_angle(ui->screen_img_titlebg, 1800);

	//Write codes screen_img_wifi
	ui->screen_img_wifi = lv_img_create(ui->screen_cont_title);
	lv_obj_set_pos(ui->screen_img_wifi, 411, 5);
	lv_obj_set_size(ui->screen_img_wifi, 20, 20);
	lv_obj_set_scrollbar_mode(ui->screen_img_wifi, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_wifi, lv_color_make(0x9d, 0x9c, 0x9c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_wifi, 212, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_wifi, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_wifi, &_wifi_alpha_20x20);
	lv_img_set_pivot(ui->screen_img_wifi, 50, 50);
	lv_img_set_angle(ui->screen_img_wifi, 0);
	static bool screen_digital_clock_2_timer_enabled = false;

	//Write codes screen_digital_clock_2
	ui->screen_digital_clock_2 = lv_dclock_create(ui->screen_cont_title, "23:55");
	lv_obj_set_style_text_align(ui->screen_digital_clock_2, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->screen_digital_clock_2, 3, 8);
	lv_obj_set_size(ui->screen_digital_clock_2, 74, 22);

	//create timer
	if (!screen_digital_clock_2_timer_enabled) {
		lv_timer_create(screen_digital_clock_2_timer, 1000, NULL);
		screen_digital_clock_2_timer_enabled = true;
	}
	//Set style for screen_digital_clock_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_digital_clock_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_digital_clock_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_digital_clock_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_digital_clock_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_digital_clock_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_digital_clock_2, lv_color_make(0xbd, 0xbb, 0xbb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_digital_clock_2, &lv_font_MiSans_Demibold_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_log
	ui->screen_img_log = lv_img_create(ui->screen_cont_title);
	lv_obj_set_pos(ui->screen_img_log, 372, 5);
	lv_obj_set_size(ui->screen_img_log, 20, 20);
	lv_obj_set_scrollbar_mode(ui->screen_img_log, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_log. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_log, lv_color_make(0x1a, 0x1a, 0x1a), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_log, 87, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_log, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_log, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_log, &_logo_off_alpha_20x20);
	lv_img_set_pivot(ui->screen_img_log, 50, 50);
	lv_img_set_angle(ui->screen_img_log, 0);

	//Write codes screen_img_logo_on
	ui->screen_img_logo_on = lv_img_create(ui->screen_cont_title);
	lv_obj_set_pos(ui->screen_img_logo_on, 372, 5);
	lv_obj_set_size(ui->screen_img_logo_on, 20, 20);
	lv_obj_set_scrollbar_mode(ui->screen_img_logo_on, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_logo_on. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_logo_on, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_logo_on, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_logo_on, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_logo_on, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget screen_img_logo_on
	lv_obj_add_flag(ui->screen_img_logo_on, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->screen_img_logo_on, &_logo_on_alpha_20x20);
	lv_img_set_pivot(ui->screen_img_logo_on, 50, 50);
	lv_img_set_angle(ui->screen_img_logo_on, 0);

	//Init events for screen
	events_init_screen(ui);
}