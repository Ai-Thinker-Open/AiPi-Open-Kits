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

int cont_4_digital_clock_1_hour_value = 11;
int cont_4_digital_clock_1_min_value = 25;
int cont_4_digital_clock_1_sec_value = 50;
int cont_4_lable_1_yers = 1970;
int cont_4_lable_1_month = 00;
int cont_4_lable_1_day = 30;
int cont_4_lable_1_wday = 6;

char* week_day[7] = { "日","一","二","三","四","五","六" };

void cont_4_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&cont_4_digital_clock_1_hour_value, &cont_4_digital_clock_1_min_value, &cont_4_digital_clock_1_sec_value);

	if (lv_obj_is_valid(guider_ui.cont_4_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.cont_4_digital_clock_1, "%02d:%02d:%02d", cont_4_digital_clock_1_hour_value, cont_4_digital_clock_1_min_value, cont_4_digital_clock_1_sec_value);
		lv_label_set_text_fmt(guider_ui.cont_4_label_1, "%04d年%02d月%02d日", cont_4_lable_1_yers, cont_4_lable_1_month, cont_4_lable_1_day);
		lv_label_set_text_fmt(guider_ui.cont_4_label_2, "星期%s", week_day[cont_4_lable_1_wday]);
	}
}

void setup_scr_cont_4(lv_ui* ui) {

	//Write codes cont_4
	ui->cont_4 = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->cont_4, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->cont_4, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_cont_2
	ui->cont_4_cont_2 = lv_obj_create(ui->cont_4);
	lv_obj_set_pos(ui->cont_4_cont_2, 0, 0);
	lv_obj_set_size(ui->cont_4_cont_2, 240, 30);
	lv_obj_set_scrollbar_mode(ui->cont_4_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_cont_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_cont_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_cont_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->cont_4_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->cont_4_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_img_connet
	ui->cont_4_img_connet = lv_img_create(ui->cont_4_cont_2);
	lv_obj_set_pos(ui->cont_4_img_connet, 205, 7);
	lv_obj_set_size(ui->cont_4_img_connet, 16, 16);
	lv_obj_set_scrollbar_mode(ui->cont_4_img_connet, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_img_connet. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->cont_4_img_connet, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->cont_4_img_connet, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->cont_4_img_connet, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->cont_4_img_connet, LV_OBJ_FLAG_CLICKABLE);
	// lv_img_set_src(ui->cont_4_img_connet, &_wifi_alpha_16x16);
	if (ui->wifi_stayus)
		lv_img_set_src(ui->cont_4_img_connet, &_wifi_alpha_16x16);
	else
		lv_img_set_src(ui->cont_4_img_connet, &_no_internet_alpha_16x16);

	lv_img_set_pivot(ui->cont_4_img_connet, 50, 50);
	lv_img_set_angle(ui->cont_4_img_connet, 0);


	//Write codes cont_4_cont_1
	ui->cont_4_cont_1 = lv_obj_create(ui->cont_4);
	lv_obj_set_pos(ui->cont_4_cont_1, 0, 30);
	lv_obj_set_size(ui->cont_4_cont_1, 240, 100);
	lv_obj_set_scrollbar_mode(ui->cont_4_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->cont_4_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->cont_4_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_1
	ui->cont_4_label_1 = lv_label_create(ui->cont_4_cont_1);
	lv_obj_set_pos(ui->cont_4_label_1, 13, 49);
	lv_obj_set_size(ui->cont_4_label_1, 141, 25);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text_fmt(guider_ui.cont_4_label_1, "%04d年%02d月%02d日", cont_4_lable_1_yers, cont_4_lable_1_month, cont_4_lable_1_day);
	lv_label_set_long_mode(ui->cont_4_label_1, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_1, lv_color_make(0x16, 0x18, 0x19), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_1, lv_color_make(0x06, 0x07, 0x07), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_1, &lv_font_simhei_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_2
	ui->cont_4_label_2 = lv_label_create(ui->cont_4_cont_1);
	lv_obj_set_pos(ui->cont_4_label_2, 145, 49);
	lv_obj_set_size(ui->cont_4_label_2, 57, 21);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text_fmt(guider_ui.cont_4_label_2, "星期%s", week_day[cont_4_lable_1_wday]);
	lv_label_set_long_mode(ui->cont_4_label_2, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_2, lv_color_make(0x17, 0x17, 0x17), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_2, lv_color_make(0x5a, 0x61, 0x73), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_2, lv_color_make(0x0f, 0x10, 0x11), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_2, &lv_font_simhei_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool cont_4_digital_clock_1_timer_enabled = false;

	//Write codes cont_4_digital_clock_1
	ui->cont_4_digital_clock_1 = lv_dclock_create(ui->cont_4_cont_1, "11:25:50");
	lv_dclock_set_text_fmt(guider_ui.cont_4_digital_clock_1, "%02d:%02d:%02d", cont_4_digital_clock_1_hour_value, cont_4_digital_clock_1_min_value, cont_4_digital_clock_1_sec_value);
	lv_obj_set_style_text_align(ui->cont_4_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->cont_4_digital_clock_1, 41, 1);
	lv_obj_set_size(ui->cont_4_digital_clock_1, 156, 58);

	//create timer
	if (!cont_4_digital_clock_1_timer_enabled) {
		lv_timer_create(cont_4_digital_clock_1_timer, 1000, NULL);
		cont_4_digital_clock_1_timer_enabled = true;
	}
	//Set style for cont_4_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_digital_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_digital_clock_1, &lv_font_Antonio_Regular_43, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_cont_3
	ui->cont_4_cont_3 = lv_obj_create(ui->cont_4);
	lv_obj_set_pos(ui->cont_4_cont_3, 0, 130);
	lv_obj_set_size(ui->cont_4_cont_3, 240, 130);
	lv_obj_set_scrollbar_mode(ui->cont_4_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_cont_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_cont_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_cont_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->cont_4_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->cont_4_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_img_clear
	ui->cont_4_img_clear = lv_img_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_img_clear, 13, 15);
	lv_obj_set_size(ui->cont_4_img_clear, 100, 100);
	lv_obj_set_scrollbar_mode(ui->cont_4_img_clear, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_img_clear. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->cont_4_img_clear, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->cont_4_img_clear, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->cont_4_img_clear, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->cont_4_img_clear, LV_OBJ_FLAG_CLICKABLE);
	if (weathers[0].wea[0])
		lv_img_set_src(ui->cont_4_img_clear, compare_wea_output_img_100x100(weathers[0].wea));
	else
		lv_img_set_src(ui->cont_4_img_clear, &_tianqiqing_alpha_100x100);
	lv_img_set_pivot(ui->cont_4_img_clear, 50, 50);
	lv_img_set_angle(ui->cont_4_img_clear, 0);

	//Write codes cont_4_label_3
	ui->cont_4_label_3 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_3, 13, 117);
	lv_obj_set_size(ui->cont_4_label_3, 39, 20);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_3, LV_SCROLLBAR_MODE_OFF);
	if (weathers[0].city[0])
		lv_label_set_text_fmt(ui->cont_4_label_3, "%s市", weathers[0].city);
	else
		lv_label_set_text(ui->cont_4_label_3, "深圳市");
	lv_label_set_long_mode(ui->cont_4_label_3, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_3, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_4
	ui->cont_4_label_4 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_4, 50, 117);
	lv_obj_set_size(ui->cont_4_label_4, 60, 12);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_4, LV_SCROLLBAR_MODE_OFF);
	if (weathers[0].wea[0])
		lv_label_set_text(ui->cont_4_label_4, weathers[0].wea);
	else
		lv_label_set_text(ui->cont_4_label_4, "多云转小雨");
	lv_label_set_long_mode(ui->cont_4_label_4, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_4, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_temp
	ui->cont_4_label_temp = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_temp, 131, 36);
	lv_obj_set_size(ui->cont_4_label_temp, 57, 40);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_temp, LV_SCROLLBAR_MODE_OFF);
	if (weathers[0].tem_day[0])
		lv_label_set_text_fmt(ui->cont_4_label_temp, "%s°", weathers[0].tem_day);
	else
		lv_label_set_text(ui->cont_4_label_temp, "23°");
	lv_label_set_long_mode(ui->cont_4_label_temp, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_temp, &lv_font_simhei_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_temp, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_acquired
	ui->cont_4_label_acquired = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_acquired, 118, 72);
	lv_obj_set_size(ui->cont_4_label_acquired, 29, 25);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_acquired, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->cont_4_label_acquired, "明天");
	lv_label_set_long_mode(ui->cont_4_label_acquired, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_acquired. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_acquired, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_acquired, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_acquired, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_acquired, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_acquired, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_acquired, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_acquired, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_acquired, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_acquired, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_acquired, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_5
	ui->cont_4_label_5 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_5, 155, 72);
	lv_obj_set_size(ui->cont_4_label_5, 29, 25);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->cont_4_label_5, "后天");
	lv_label_set_long_mode(ui->cont_4_label_5, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_5, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_5, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_6
	ui->cont_4_label_6 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_6, 188, 72);
	lv_obj_set_size(ui->cont_4_label_6, 44, 25);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_6, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->cont_4_label_6, "大后天");
	lv_label_set_long_mode(ui->cont_4_label_6, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_6, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_6, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_img_3
	ui->cont_4_img_3 = lv_img_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_img_3, 121, 93);
	lv_obj_set_size(ui->cont_4_img_3, 20, 20);
	lv_obj_set_scrollbar_mode(ui->cont_4_img_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_img_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->cont_4_img_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->cont_4_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->cont_4_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->cont_4_img_3, LV_OBJ_FLAG_CLICKABLE);
	if (weathers[1].wea[0])
		lv_img_set_src(ui->cont_4_img_3, compare_wea_output_img_20x20(weathers[1].wea));
	else
		lv_img_set_src(ui->cont_4_img_3, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->cont_4_img_3, 50, 50);
	lv_img_set_angle(ui->cont_4_img_3, 0);

	//Write codes cont_4_img_2
	ui->cont_4_img_2 = lv_img_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_img_2, 159, 93);
	lv_obj_set_size(ui->cont_4_img_2, 20, 20);
	lv_obj_set_scrollbar_mode(ui->cont_4_img_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_img_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->cont_4_img_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->cont_4_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->cont_4_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->cont_4_img_2, LV_OBJ_FLAG_CLICKABLE);

	if (weathers[2].wea[0])
		lv_img_set_src(ui->cont_4_img_2, compare_wea_output_img_20x20(weathers[2].wea));
	else
		lv_img_set_src(ui->cont_4_img_2, &_tianqiqing_i_alpha_20x20);

	lv_img_set_src(ui->cont_4_img_2, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->cont_4_img_2, 50, 50);
	lv_img_set_angle(ui->cont_4_img_2, 0);

	//Write codes cont_4_img_1
	ui->cont_4_img_1 = lv_img_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_img_1, 202, 93);
	lv_obj_set_size(ui->cont_4_img_1, 20, 20);
	lv_obj_set_scrollbar_mode(ui->cont_4_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for cont_4_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->cont_4_img_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->cont_4_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->cont_4_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->cont_4_img_1, LV_OBJ_FLAG_CLICKABLE);
	if (weathers[3].wea[0])
		lv_img_set_src(ui->cont_4_img_1, compare_wea_output_img_20x20(weathers[3].wea));
	else
		lv_img_set_src(ui->cont_4_img_1, &_tianqiqing_i_alpha_20x20);

	lv_img_set_src(ui->cont_4_img_1, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->cont_4_img_1, 50, 50);
	lv_img_set_angle(ui->cont_4_img_1, 0);

	//Write codes cont_4_label_7
	ui->cont_4_label_7 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_7, 123, 117);
	lv_obj_set_size(ui->cont_4_label_7, 26, 13);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_7, LV_SCROLLBAR_MODE_OFF);
	if (weathers[1].tem_day[0])
		lv_label_set_text_fmt(ui->cont_4_label_7, "%.*s°", 2, weathers[1].tem_day);
	else
		lv_label_set_text(ui->cont_4_label_7, "25°");
	lv_label_set_long_mode(ui->cont_4_label_7, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_7. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_7, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_7, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_8
	ui->cont_4_label_8 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_8, 160, 117);
	lv_obj_set_size(ui->cont_4_label_8, 26, 13);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_8, LV_SCROLLBAR_MODE_OFF);
	if (weathers[2].tem_day[0])
		lv_label_set_text_fmt(ui->cont_4_label_8, "%.*s°", 2, weathers[2].tem_day);
	else
		lv_label_set_text(ui->cont_4_label_8, "25°");
	lv_label_set_long_mode(ui->cont_4_label_8, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_8. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_8, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_8, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes cont_4_label_9
	ui->cont_4_label_9 = lv_label_create(ui->cont_4_cont_3);
	lv_obj_set_pos(ui->cont_4_label_9, 204, 117);
	lv_obj_set_size(ui->cont_4_label_9, 26, 13);
	lv_obj_set_scrollbar_mode(ui->cont_4_label_9, LV_SCROLLBAR_MODE_OFF);
	if (weathers[3].tem_day[0])
		lv_label_set_text_fmt(ui->cont_4_label_9, "%.*s°", 2, weathers[3].tem_day);
	else
		lv_label_set_text(ui->cont_4_label_9, "25°");
	lv_label_set_long_mode(ui->cont_4_label_9, LV_LABEL_LONG_WRAP);

	//Set style for cont_4_label_9. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->cont_4_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->cont_4_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->cont_4_label_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->cont_4_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->cont_4_label_9, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->cont_4_label_9, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->cont_4_label_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->cont_4_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->cont_4_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


	//Init events for screen
	events_init_cont_4(ui);
}