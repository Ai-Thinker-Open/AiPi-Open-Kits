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

int src_home_digital_clock_1_hour_value = 11;
int src_home_digital_clock_1_min_value = 25;
int src_home_digital_clock_1_sec_value = 50;
int src_home_digital_date_yesr_value = 1970;
int src_home_digital_date_mont_value = 7;
int src_home_digital_date_day_value = 11;
int src_home_digital_date_wday_value = 2;

void src_home_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&src_home_digital_clock_1_hour_value, &src_home_digital_clock_1_min_value, &src_home_digital_clock_1_sec_value);
	if (lv_obj_is_valid(guider_ui.src_home_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.src_home_digital_clock_1, "%02d:%02d:%02d", src_home_digital_clock_1_hour_value, src_home_digital_clock_1_min_value, src_home_digital_clock_1_sec_value);
	}
}


void setup_scr_src_home(lv_ui* ui) {

	//Write codes src_home
	ui->src_home = lv_obj_create(NULL);

	//Create keyboard on src_home
	g_kb_src_home = lv_keyboard_create(ui->src_home);
	lv_obj_add_event_cb(g_kb_src_home, kb_src_home_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_src_home, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_src_home, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->src_home, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->src_home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	ui->src_home_tabview_1 = lv_tabview_create(ui->src_home, LV_DIR_BOTTOM, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_tabview_1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_pos(ui->src_home_tabview_1, 0, 0);
	lv_obj_set_size(ui->src_home_tabview_1, 320, 240);

	//Home
	ui->src_home_tabview_1_Home = lv_tabview_add_tab(ui->src_home_tabview_1, "Home");
	//Shortcuts
	ui->src_home_tabview_1_Shortcuts = lv_tabview_add_tab(ui->src_home_tabview_1, "Shortcuts");
	//SmartCtrl
	ui->src_home_tabview_1_SmartCtrl = lv_tabview_add_tab(ui->src_home_tabview_1, "SmartCtrl");
	//Set style for src_home_tabview_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tabview_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_tabview_1, lv_color_make(0xea, 0xef, 0xf3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_tabview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_tabview_1, lv_color_make(0x24, 0x24, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_tabview_1, lv_color_make(0xc0, 0xc0, 0xc0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_tabview_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_tabview_1, lv_color_make(0x4d, 0x4d, 0x4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_tabview_1, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_src_home_tabview_1_extra_btnm_main_default
	static lv_style_t style_src_home_tabview_1_extra_btnm_main_default;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_main_default);
	lv_style_set_radius(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_bg_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0x0f, 0x0f, 0x0f));
	lv_style_set_bg_grad_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_src_home_tabview_1_extra_btnm_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_border_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0x11, 0x11, 0x11));
	lv_style_set_border_width(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_border_opa(&style_src_home_tabview_1_extra_btnm_main_default, 80);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_src_home_tabview_1_extra_btnm_items_default
	static lv_style_t style_src_home_tabview_1_extra_btnm_items_default;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_items_default);
	lv_style_set_text_color(&style_src_home_tabview_1_extra_btnm_items_default, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_font(&style_src_home_tabview_1_extra_btnm_items_default, &lv_font_simhei_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_src_home_tabview_1_extra_btnm_items_checked
	static lv_style_t style_src_home_tabview_1_extra_btnm_items_checked;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_items_checked);
	lv_style_set_radius(&style_src_home_tabview_1_extra_btnm_items_checked, 0);
	lv_style_set_bg_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_src_home_tabview_1_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_tabview_1_extra_btnm_items_checked, 60);
	lv_style_set_border_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_src_home_tabview_1_extra_btnm_items_checked, 4);
	lv_style_set_border_opa(&style_src_home_tabview_1_extra_btnm_items_checked, 255);
	lv_style_set_border_side(&style_src_home_tabview_1_extra_btnm_items_checked, LV_BORDER_SIDE_BOTTOM);
	lv_style_set_text_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_text_font(&style_src_home_tabview_1_extra_btnm_items_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_items_checked, LV_PART_ITEMS|LV_STATE_CHECKED);

	//Write codes src_home_tileview
	ui->src_home_tileview = lv_tileview_create(ui->src_home_tabview_1_Home);
	lv_obj_set_pos(ui->src_home_tileview, 0, 0);
	lv_obj_set_size(ui->src_home_tileview, 290, 180);
	lv_obj_set_scrollbar_mode(ui->src_home_tileview, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_tileview. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tileview, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_tileview, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_tileview, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_tileview, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_tileview. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tileview, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tileview, lv_color_make(0xea, 0xef, 0xf3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tileview, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile home
	ui->home = lv_tileview_add_tile(ui->src_home_tileview, 0, 0, LV_DIR_BOTTOM);
	{
		//Write codes src_home_img_1
		ui->src_home_img_1 = lv_img_create(ui->home);
		lv_obj_set_pos(ui->src_home_img_1, 0, 20);
		lv_obj_set_size(ui->src_home_img_1, 100, 100);
		lv_obj_set_scrollbar_mode(ui->src_home_img_1, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_img_recolor(ui->src_home_img_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_recolor_opa(ui->src_home_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_opa(ui->src_home_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_add_flag(ui->src_home_img_1, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(ui->src_home_img_1, &_tianqiqing_alpha_100x100);
		lv_img_set_pivot(ui->src_home_img_1, 50, 50);
		lv_img_set_angle(ui->src_home_img_1, 0);

		//Write codes src_home_label_temp
		ui->src_home_label_temp = lv_label_create(ui->home);
		lv_obj_set_pos(ui->src_home_label_temp, 190, 96+28);
		lv_obj_set_size(ui->src_home_label_temp, 100, 44);
		lv_obj_set_scrollbar_mode(ui->src_home_label_temp, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_temp, "25℃");
		lv_label_set_long_mode(ui->src_home_label_temp, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_temp, &lv_font_simhei_40, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		static bool src_home_digital_clock_1_timer_enabled = false;

		//Write codes src_home_digital_clock_1
		ui->src_home_digital_clock_1 = lv_dclock_create(ui->home, "11:25:50");
		lv_obj_set_style_text_align(ui->src_home_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
		lv_obj_set_pos(ui->src_home_digital_clock_1, 120, 42);
		lv_obj_set_size(ui->src_home_digital_clock_1, 143, 48);

		//create timer
		if (!src_home_digital_clock_1_timer_enabled) {
			lv_timer_create(src_home_digital_clock_1_timer, 1000, NULL);
			src_home_digital_clock_1_timer_enabled = true;
		}
		//Set style for src_home_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_digital_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_digital_clock_1, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_img_wifi
		ui->src_home_img_wifi = lv_img_create(ui->home);
		lv_obj_set_pos(ui->src_home_img_wifi, 140, 0);
		lv_obj_set_size(ui->src_home_img_wifi, 20, 20);
		lv_obj_set_scrollbar_mode(ui->src_home_img_wifi, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_img_recolor(ui->src_home_img_wifi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_recolor_opa(ui->src_home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_opa(ui->src_home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_add_flag(ui->src_home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(ui->src_home_img_wifi, &_no_internet_alpha_20x20);
		lv_img_set_pivot(ui->src_home_img_wifi, 0, 0);
		lv_img_set_angle(ui->src_home_img_wifi, 0);

		//Write codes src_home_label_dizhi
		ui->src_home_label_dizhi = lv_label_create(ui->home);
		lv_obj_set_pos(ui->src_home_label_dizhi, 100, 100+28);
		lv_obj_set_size(ui->src_home_label_dizhi, 100, 16);
		lv_obj_set_scrollbar_mode(ui->src_home_label_dizhi, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_dizhi, "齐齐哈尔市");
		lv_label_set_long_mode(ui->src_home_label_dizhi, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_dizhi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_dizhi, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_dizhi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_dizhi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_dizhi, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_dizhi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_waether
		ui->src_home_label_waether = lv_label_create(ui->home);
		lv_obj_set_pos(ui->src_home_label_waether, 95, 120+28);
		lv_obj_set_size(ui->src_home_label_waether, 120, 14);
		lv_obj_set_scrollbar_mode(ui->src_home_label_waether, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_waether, "雷阵雨转多云");
		lv_label_set_long_mode(ui->src_home_label_waether, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_waether. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_waether, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_waether, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_waether, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_waether, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_waether, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_cont_1
		ui->src_home_cont_1 = lv_obj_create(ui->home);
		lv_obj_set_pos(ui->src_home_cont_1, -23, 120);
		lv_obj_set_size(ui->src_home_cont_1, 140, 50);
		lv_obj_set_scrollbar_mode(ui->src_home_cont_1, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(ui->src_home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(ui->src_home_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_img_day1
		ui->src_home_img_day1 = lv_img_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_img_day1, 23, 15);
		lv_obj_set_size(ui->src_home_img_day1, 20, 20);
		lv_obj_set_scrollbar_mode(ui->src_home_img_day1, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_img_day1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_img_recolor(ui->src_home_img_day1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_recolor_opa(ui->src_home_img_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_opa(ui->src_home_img_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_add_flag(ui->src_home_img_day1, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(ui->src_home_img_day1, &_tianqiqing_i_alpha_20x20);
		lv_img_set_pivot(ui->src_home_img_day1, 0, 0);
		lv_img_set_angle(ui->src_home_img_day1, 0);

		//Write codes src_home_label_day2
		ui->src_home_label_day2 = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_label_day2, 53, 4);
		lv_obj_set_size(ui->src_home_label_day2, 33, 9);
		lv_obj_set_scrollbar_mode(ui->src_home_label_day2, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_day2, "后天");
		lv_label_set_long_mode(ui->src_home_label_day2, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_day2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_day2, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_day2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_day3
		ui->src_home_label_day3 = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_label_day3, 93, 3);
		lv_obj_set_size(ui->src_home_label_day3, 33, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_label_day3, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_day3, "大后天");
		lv_label_set_long_mode(ui->src_home_label_day3, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_day3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_day3, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_day3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_day1
		ui->src_home_label_day1 = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_label_day1, 18, 3);
		lv_obj_set_size(ui->src_home_label_day1, 33, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_label_day1, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_day1, "明天");
		lv_label_set_long_mode(ui->src_home_label_day1, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_day1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_day1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_day1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_day1, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_day1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_img_day2
		ui->src_home_img_day2 = lv_img_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_img_day2, 61, 15);
		lv_obj_set_size(ui->src_home_img_day2, 20, 20);
		lv_obj_set_scrollbar_mode(ui->src_home_img_day2, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_img_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_img_recolor(ui->src_home_img_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_recolor_opa(ui->src_home_img_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_opa(ui->src_home_img_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_add_flag(ui->src_home_img_day2, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(ui->src_home_img_day2, &_tianqiqing_i_alpha_20x20);
		lv_img_set_pivot(ui->src_home_img_day2, 0, 0);
		lv_img_set_angle(ui->src_home_img_day2, 0);

		//Write codes src_home_img_day3
		ui->src_home_img_day3 = lv_img_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_img_day3, 99, 15);
		lv_obj_set_size(ui->src_home_img_day3, 20, 20);
		lv_obj_set_scrollbar_mode(ui->src_home_img_day3, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_img_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_img_recolor(ui->src_home_img_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_recolor_opa(ui->src_home_img_day3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_img_opa(ui->src_home_img_day3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_add_flag(ui->src_home_img_day3, LV_OBJ_FLAG_CLICKABLE);
		lv_img_set_src(ui->src_home_img_day3, &_tianqiqing_i_alpha_20x20);
		lv_img_set_pivot(ui->src_home_img_day3, 0, 0);
		lv_img_set_angle(ui->src_home_img_day3, 0);

		//Write codes src_home_day1_temp
		ui->src_home_day1_temp = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_day1_temp, 27, 37);
		lv_obj_set_size(ui->src_home_day1_temp, 20, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_day1_temp, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_day1_temp, "25°");
		lv_label_set_long_mode(ui->src_home_day1_temp, LV_LABEL_LONG_WRAP);

		//Set style for src_home_day1_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_day1_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_day1_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_day1_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_day1_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_day2_temp
		ui->src_home_day2_temp = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_day2_temp, 66, 37);
		lv_obj_set_size(ui->src_home_day2_temp, 20, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_day2_temp, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_day2_temp, "25°");
		lv_label_set_long_mode(ui->src_home_day2_temp, LV_LABEL_LONG_WRAP);

		//Set style for src_home_day2_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_day2_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_day2_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_day2_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_day2_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_day3_temp
		ui->src_home_day3_temp = lv_label_create(ui->src_home_cont_1);
		lv_obj_set_pos(ui->src_home_day3_temp, 104, 37);
		lv_obj_set_size(ui->src_home_day3_temp, 20, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_day3_temp, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_day3_temp, "25°");
		lv_label_set_long_mode(ui->src_home_day3_temp, LV_LABEL_LONG_WRAP);

		//Set style for src_home_day3_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_day3_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_day3_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_day3_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_day3_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_cont_2
		ui->src_home_cont_2 = lv_obj_create(ui->home);
		lv_obj_set_pos(ui->src_home_cont_2, 10+80, 76);
		lv_obj_set_size(ui->src_home_cont_2, 196, 31);
		lv_obj_set_scrollbar_mode(ui->src_home_cont_2, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_cont_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_cont_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_cont_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(ui->src_home_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(ui->src_home_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_wday
		ui->src_home_label_wday = lv_label_create(ui->src_home_cont_2);
		lv_obj_set_pos(ui->src_home_label_wday, 126, 5);
		lv_obj_set_size(ui->src_home_label_wday, 46, 16);
		lv_obj_set_scrollbar_mode(ui->src_home_label_wday, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_wday, "星期一");
		lv_label_set_long_mode(ui->src_home_label_wday, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_wday. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_wday, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_wday, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_wday, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_wday, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_date
		ui->src_home_label_date = lv_label_create(ui->src_home_cont_2);
		lv_obj_set_pos(ui->src_home_label_date, 26, 5);
		lv_obj_set_size(ui->src_home_label_date, 80, 10);
		lv_obj_set_scrollbar_mode(ui->src_home_label_date, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_date, "1970年7月10日");
		lv_label_set_long_mode(ui->src_home_label_date, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_date. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_date, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_date, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_date, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	}
	//add new tile config
	ui->config = lv_tileview_add_tile(ui->src_home_tileview, 0, 1, LV_DIR_TOP);
	{
		//Write codes src_home_ta_1
		ui->src_home_ta_1 = lv_textarea_create(ui->config);
		lv_obj_set_pos(ui->src_home_ta_1, 42+32, 100-25);
		lv_obj_set_size(ui->src_home_ta_1, 130+20, 30);
		lv_obj_set_scrollbar_mode(ui->src_home_ta_1, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_ta_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_ta_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_ta_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(ui->src_home_ta_1, lv_color_make(0x58, 0x58, 0x58), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(ui->src_home_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(ui->src_home_ta_1, 249, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_ta_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_ta_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Set style for src_home_ta_1. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_ta_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
		lv_textarea_set_text(ui->src_home_ta_1, "ASD");

		//use keyboard on src_home_ta_1
		lv_obj_add_event_cb(ui->src_home_ta_1, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

		//Write codes src_home_label_ip
		ui->src_home_label_ip = lv_label_create(ui->config);
		lv_obj_set_pos(ui->src_home_label_ip, 80, 0);
		lv_obj_set_size(ui->src_home_label_ip, 127, 12);
		lv_obj_set_scrollbar_mode(ui->src_home_label_ip, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_ip, "IP:00.00.00.00");
		lv_label_set_long_mode(ui->src_home_label_ip, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_ip. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_ip, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_ip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_ip, lv_color_make(0xf9, 0xf9, 0xf9), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_ip, &lv_font_arial_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_ddlist_1
		ui->src_home_ddlist_1 = lv_dropdown_create(ui->config);
		lv_obj_set_pos(ui->src_home_ddlist_1, 42+32, 47-20);
		lv_obj_set_size(ui->src_home_ddlist_1, 130+20, 30);
		lv_obj_set_scrollbar_mode(ui->src_home_ddlist_1, LV_SCROLLBAR_MODE_OFF);
		lv_dropdown_set_options(ui->src_home_ddlist_1, "FAE@Seahi\nAIOT@FAE\nzhibo_01");

		//Set style for src_home_ddlist_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_ddlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_ddlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_ddlist_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(ui->src_home_ddlist_1, lv_color_make(0x68, 0x68, 0x68), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(ui->src_home_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(ui->src_home_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_ddlist_1, lv_color_make(0xfd, 0xfd, 0xfd), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_ddlist_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Set style state: LV_STATE_CHECKED for style_src_home_ddlist_1_extra_list_selected_checked
		static lv_style_t style_src_home_ddlist_1_extra_list_selected_checked;
		ui_init_style(&style_src_home_ddlist_1_extra_list_selected_checked);
		lv_style_set_radius(&style_src_home_ddlist_1_extra_list_selected_checked, 3);
		lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0x50, 0x50, 0x50));
		lv_style_set_bg_grad_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
		lv_style_set_bg_grad_dir(&style_src_home_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
		lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_selected_checked, 173);
		lv_style_set_border_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
		lv_style_set_border_width(&style_src_home_ddlist_1_extra_list_selected_checked, 1);
		lv_style_set_border_opa(&style_src_home_ddlist_1_extra_list_selected_checked, 0);
		lv_style_set_text_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
		lv_style_set_text_font(&style_src_home_ddlist_1_extra_list_selected_checked, &lv_font_montserratMedium_12);
		lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

		//Set style state: LV_STATE_DEFAULT for style_src_home_ddlist_1_extra_list_main_default
		static lv_style_t style_src_home_ddlist_1_extra_list_main_default;
		ui_init_style(&style_src_home_ddlist_1_extra_list_main_default);
		lv_style_set_radius(&style_src_home_ddlist_1_extra_list_main_default, 3);
		lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
		lv_style_set_bg_grad_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
		lv_style_set_bg_grad_dir(&style_src_home_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
		lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_main_default, 82);
		lv_style_set_border_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
		lv_style_set_border_width(&style_src_home_ddlist_1_extra_list_main_default, 1);
		lv_style_set_border_opa(&style_src_home_ddlist_1_extra_list_main_default, 0);
		lv_style_set_text_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xbb, 0xbb, 0xbb));
		lv_style_set_text_font(&style_src_home_ddlist_1_extra_list_main_default, &lv_font_montserratMedium_12);
		lv_style_set_max_height(&style_src_home_ddlist_1_extra_list_main_default, 150);
		lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Set style state: LV_STATE_DEFAULT for style_src_home_ddlist_1_extra_list_scrollbar_default
		static lv_style_t style_src_home_ddlist_1_extra_list_scrollbar_default;
		ui_init_style(&style_src_home_ddlist_1_extra_list_scrollbar_default);
		lv_style_set_radius(&style_src_home_ddlist_1_extra_list_scrollbar_default, 3);
		lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_scrollbar_default, lv_color_make(0xb0, 0xb0, 0xb0));
		lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_scrollbar_default, 221);
		lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

		//Write codes src_home_label_ssid
		ui->src_home_label_ssid = lv_label_create(ui->config);
		lv_obj_set_pos(ui->src_home_label_ssid, 30, 55-20);
		lv_obj_set_size(ui->src_home_label_ssid, 48, 15);
		lv_obj_set_scrollbar_mode(ui->src_home_label_ssid, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_ssid, "SSID:");
		lv_label_set_long_mode(ui->src_home_label_ssid, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_ssid, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_ssid, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_label_pass
		ui->src_home_label_pass = lv_label_create(ui->config);
		lv_obj_set_pos(ui->src_home_label_pass, 30, 108-25);
		lv_obj_set_size(ui->src_home_label_pass, 45, 14);
		lv_obj_set_scrollbar_mode(ui->src_home_label_pass, LV_SCROLLBAR_MODE_OFF);
		lv_label_set_text(ui->src_home_label_pass, "PASS:");
		lv_label_set_long_mode(ui->src_home_label_pass, LV_LABEL_LONG_WRAP);

		//Set style for src_home_label_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_label_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_label_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_label_pass, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_label_pass, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

		//Write codes src_home_btn_connect
		ui->src_home_btn_connect = lv_btn_create(ui->config);
		lv_obj_set_pos(ui->src_home_btn_connect, 59+40, 155-20);
		lv_obj_set_size(ui->src_home_btn_connect, 87, 39);
		lv_obj_set_scrollbar_mode(ui->src_home_btn_connect, LV_SCROLLBAR_MODE_OFF);

		//Set style for src_home_btn_connect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
		lv_obj_set_style_radius(ui->src_home_btn_connect, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(ui->src_home_btn_connect, lv_color_make(0x08, 0x24, 0x39), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(ui->src_home_btn_connect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(ui->src_home_btn_connect, 220, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_opa(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_spread(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(ui->src_home_btn_connect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(ui->src_home_btn_connect, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(ui->src_home_btn_connect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
		ui->src_home_btn_connect_label = lv_label_create(ui->src_home_btn_connect);
		lv_label_set_text(ui->src_home_btn_connect_label, "连接");
		lv_obj_set_style_pad_all(ui->src_home_btn_connect, 0, LV_STATE_DEFAULT);
		lv_obj_align(ui->src_home_btn_connect_label, LV_ALIGN_CENTER, 0, 0);

	}
	//Write codes src_home_imgbtn_openL
	shortcuts_scr_set(ui);
	SmartCtrl_scr_set(ui);
	//Write codes src_home_cont_5
	ui->src_home_cont_5 = lv_obj_create(ui->src_home);
	lv_obj_set_pos(ui->src_home_cont_5, 0, 0);
	lv_obj_set_size(ui->src_home_cont_5, 320, 240);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_5, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_5, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_5, 125, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_cont_5
	lv_obj_add_flag(ui->src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
	//Write codes src_home_img_loding
	ui->src_home_img_loding = lv_img_create(ui->src_home_cont_5);
	lv_obj_set_pos(ui->src_home_img_loding, 100, 60);
	lv_obj_set_size(ui->src_home_img_loding, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_loding, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_loding. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_loding, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_loding, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_loding, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_loding, &_loding_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_loding, 50, 50);
	lv_img_set_angle(ui->src_home_img_loding, 0);

	//Init events for screen
	events_init_src_home(ui);
}