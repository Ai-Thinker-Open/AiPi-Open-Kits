/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



int screen_digital_clock_date_min_value = 25;
int screen_digital_clock_date_hour_value = 11;
int screen_digital_clock_date_sec_value = 50;void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 480, 480);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0x2f3541), LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_tileview_home
	ui->screen_tileview_home = lv_tileview_create(ui->screen);
	ui->screen_tileview_home_tile_home = lv_tileview_add_tile(ui->screen_tileview_home, 0, 0, LV_DIR_BOTTOM);
	ui->screen_tileview_home_tile_ctrl = lv_tileview_add_tile(ui->screen_tileview_home, 0, 1, LV_DIR_TOP);
	lv_obj_set_pos(ui->screen_tileview_home, 0, 0);
	lv_obj_set_size(ui->screen_tileview_home, 480, 480);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_home, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_tileview_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_home, lv_color_hex(0x2f3541), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_tileview_home, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_home, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_home, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_home, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes screen_cont_date
	ui->screen_cont_date = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_date, 20, 35);
	lv_obj_set_size(ui->screen_cont_date, 266, 124);
	lv_obj_set_scrollbar_mode(ui->screen_cont_date, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_date, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_date, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_date, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_digital_clock_date
	static bool screen_digital_clock_date_timer_enabled = false;
	ui->screen_digital_clock_date = lv_dclock_create(ui->screen_cont_date, "11:25:50");
	if (!screen_digital_clock_date_timer_enabled) {
		lv_timer_create(screen_digital_clock_date_timer, 1000, NULL);
		screen_digital_clock_date_timer_enabled = true;
	}
	lv_obj_set_pos(ui->screen_digital_clock_date, 40, 6);
	lv_obj_set_size(ui->screen_digital_clock_date, 187, 62);

	//Write style for screen_digital_clock_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_digital_clock_date, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_digital_clock_date, &lv_font_Antonio_Regular_60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_digital_clock_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_digital_clock_date, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_digital_clock_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_datetext_date
	ui->screen_datetext_date = lv_label_create(ui->screen_cont_date);
	lv_label_set_text(ui->screen_datetext_date, "2023/07/31");
	lv_obj_set_style_text_align(ui->screen_datetext_date, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_flag(ui->screen_datetext_date, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->screen_datetext_date, screen_datetext_date_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_set_pos(ui->screen_datetext_date, 4, 83);
	lv_obj_set_size(ui->screen_datetext_date, 154, 27);

	//Write style for screen_datetext_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->screen_datetext_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_datetext_date, lv_color_hex(0xa2a2a2), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_datetext_date, &lv_font_Alatsi_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_datetext_date, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_datetext_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_datetext_date, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_datetext_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_datetext_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_datetext_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_wday
	ui->screen_label_wday = lv_label_create(ui->screen_cont_date);
	lv_label_set_text(ui->screen_label_wday, "星期五");
	lv_label_set_long_mode(ui->screen_label_wday, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_wday, 154, 87);
	lv_obj_set_size(ui->screen_label_wday, 73, 23);

	//Write style for screen_label_wday, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_wday, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_wday, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_wday, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_wday, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_wday, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_wday, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_wday, lv_color_hex(0xffe0a8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_wday, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_2
	ui->screen_cont_2 = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_2, 19, 232);
	lv_obj_set_size(ui->screen_cont_2, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_2, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_2, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_1
	ui->screen_btn_1 = lv_btn_create(ui->screen_cont_2);
	ui->screen_btn_1_label = lv_label_create(ui->screen_btn_1);
	lv_label_set_text(ui->screen_btn_1_label, "");
	lv_label_set_long_mode(ui->screen_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_btn_1, 23, 10);
	lv_obj_set_size(ui->screen_btn_1, 50, 50);

	//Write style for screen_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_1, lv_color_hex(0xfbdcd5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_1, 31, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_1, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_1, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen_cont_2);
	lv_label_set_text(ui->screen_label_2, "离家模式");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_2, 3, 75);
	lv_obj_set_size(ui->screen_label_2, 92, 21);

	//Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_2
	ui->screen_img_2 = lv_img_create(ui->screen_cont_2);
	lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_2, &_outHome_40x40_alpha_40x40);
	lv_img_set_pivot(ui->screen_img_2, 50,50);
	lv_img_set_angle(ui->screen_img_2, 0);
	lv_obj_set_pos(ui->screen_img_2, 27, 14);
	lv_obj_set_size(ui->screen_img_2, 40, 40);

	//Write style for screen_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_3
	ui->screen_cont_3 = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_3, 132, 232);
	lv_obj_set_size(ui->screen_cont_3, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_3, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_3, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_2
	ui->screen_btn_2 = lv_btn_create(ui->screen_cont_3);
	ui->screen_btn_2_label = lv_label_create(ui->screen_btn_2);
	lv_label_set_text(ui->screen_btn_2_label, "");
	lv_label_set_long_mode(ui->screen_btn_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_btn_2, 23, 10);
	lv_obj_set_size(ui->screen_btn_2, 50, 50);

	//Write style for screen_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_2, lv_color_hex(0xe6e4fc), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_2, 31, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_2, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_2, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_label_3, "工作模式");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_3, 3, 75);
	lv_obj_set_size(ui->screen_label_3, 92, 21);

	//Write style for screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen_cont_3);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1, &_pc_40x40_alpha_40x40);
	lv_img_set_pivot(ui->screen_img_1, 50,50);
	lv_img_set_angle(ui->screen_img_1, 0);
	lv_obj_set_pos(ui->screen_img_1, 27, 14);
	lv_obj_set_size(ui->screen_img_1, 40, 40);

	//Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_4
	ui->screen_cont_4 = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_4, 245, 232);
	lv_obj_set_size(ui->screen_cont_4, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_cont_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_4, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_4, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_4, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_3
	ui->screen_btn_3 = lv_btn_create(ui->screen_cont_4);
	ui->screen_btn_3_label = lv_label_create(ui->screen_btn_3);
	lv_label_set_text(ui->screen_btn_3_label, "");
	lv_label_set_long_mode(ui->screen_btn_3_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_3_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_3, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_btn_3, 23, 10);
	lv_obj_set_size(ui->screen_btn_3, 50, 50);

	//Write style for screen_btn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_3, lv_color_hex(0xd1e1f7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_3, 31, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_3, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_3, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_3, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen_cont_4);
	lv_label_set_text(ui->screen_label_4, "睡眠模式");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_4, 3, 75);
	lv_obj_set_size(ui->screen_label_4, 92, 21);

	//Write style for screen_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_4, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_3
	ui->screen_img_3 = lv_img_create(ui->screen_cont_4);
	lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_3, &_sleep_40x40_alpha_40x40);
	lv_img_set_pivot(ui->screen_img_3, 50,50);
	lv_img_set_angle(ui->screen_img_3, 0);
	lv_obj_set_pos(ui->screen_img_3, 27, 14);
	lv_obj_set_size(ui->screen_img_3, 40, 40);

	//Write style for screen_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_5
	ui->screen_cont_5 = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_5, 357, 232);
	lv_obj_set_size(ui->screen_cont_5, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_cont_5, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_5, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_5, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_5, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_4
	ui->screen_btn_4 = lv_btn_create(ui->screen_cont_5);
	ui->screen_btn_4_label = lv_label_create(ui->screen_btn_4);
	lv_label_set_text(ui->screen_btn_4_label, "");
	lv_label_set_long_mode(ui->screen_btn_4_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_4_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_4, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_btn_4, 23, 10);
	lv_obj_set_size(ui->screen_btn_4, 50, 50);

	//Write style for screen_btn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_4, lv_color_hex(0xffe1a3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_4, 31, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_4, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_btn_4, lv_color_hex(0x0d4b3b), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_btn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_btn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_btn_4, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_btn_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_4, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen_cont_5);
	lv_label_set_text(ui->screen_label_5, "电影模式");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_5, 3, 75);
	lv_obj_set_size(ui->screen_label_5, 92, 21);

	//Write style for screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_5, lv_color_hex(0xc4c4c4), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_5, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_4
	ui->screen_img_4 = lv_img_create(ui->screen_cont_5);
	lv_obj_add_flag(ui->screen_img_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_4, &_vido_40x40_alpha_40x40);
	lv_img_set_pivot(ui->screen_img_4, 50,50);
	lv_img_set_angle(ui->screen_img_4, 0);
	lv_obj_set_pos(ui->screen_img_4, 27, 14);
	lv_obj_set_size(ui->screen_img_4, 40, 40);

	//Write style for screen_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_6
	ui->screen_label_6 = lv_label_create(ui->screen_tileview_home_tile_home);
	lv_label_set_text(ui->screen_label_6, "场景|Scene");
	lv_label_set_long_mode(ui->screen_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_6, -2, 207);
	lv_obj_set_size(ui->screen_label_6, 125, 20);

	//Write style for screen_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_6, lv_color_hex(0xefefef), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_6, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_7
	ui->screen_label_7 = lv_label_create(ui->screen_tileview_home_tile_home);
	lv_label_set_text(ui->screen_label_7, "检测|Detection");
	lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_7, 12, 344);
	lv_obj_set_size(ui->screen_label_7, 171, 22);

	//Write style for screen_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_7, lv_color_hex(0xefefef), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_7, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_7, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_6
	ui->screen_cont_6 = lv_obj_create(ui->screen_tileview_home_tile_home);
	lv_obj_set_pos(ui->screen_cont_6, 21, 374);
	lv_obj_set_size(ui->screen_cont_6, 432, 79);
	lv_obj_set_scrollbar_mode(ui->screen_cont_6, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_6, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_6, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_6, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_8
	ui->screen_label_8 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_8, "温度：25 ℃");
	lv_label_set_long_mode(ui->screen_label_8, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_8, 18, 11);
	lv_obj_set_size(ui->screen_label_8, 113, 19);

	//Write style for screen_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_8, lv_color_hex(0xc7c7c7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_8, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_8, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_9
	ui->screen_label_9 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_9, "湿度：60%");
	lv_label_set_long_mode(ui->screen_label_9, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_9, 18, 47);
	lv_obj_set_size(ui->screen_label_9, 113, 19);

	//Write style for screen_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_9, lv_color_hex(0xc7c7c7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_9, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_9, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_10
	ui->screen_label_10 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_10, "火焰：无");
	lv_label_set_long_mode(ui->screen_label_10, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_10, 160, 12);
	lv_obj_set_size(ui->screen_label_10, 113, 19);

	//Write style for screen_label_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_10, lv_color_hex(0xc7c7c7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_10, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_10, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_10, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_11
	ui->screen_label_11 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_11, "可燃气体浓度：0 ppm");
	lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_11, 160, 47);
	lv_obj_set_size(ui->screen_label_11, 238, 20);

	//Write style for screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_11, lv_color_hex(0xc7c7c7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_11, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_12
	ui->screen_label_12 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_12, "PM2.5：00 PPM");
	lv_label_set_long_mode(ui->screen_label_12, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_12, 264, 12);
	lv_obj_set_size(ui->screen_label_12, 160, 16);

	//Write style for screen_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_12, lv_color_hex(0xc7c7c7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_12, &lv_font_MiSans_Medium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_12, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes screen_label_13
	ui->screen_label_13 = lv_label_create(ui->screen_tileview_home_tile_ctrl);
	lv_label_set_text(ui->screen_label_13, "控件测试");
	lv_label_set_long_mode(ui->screen_label_13, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_13, 150, 49);
	lv_obj_set_size(ui->screen_label_13, 149, 36);

	//Write style for screen_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_13, lv_color_hex(0xeaeaea), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_13, &lv_font_MiSans_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_13, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_tileview_1
	ui->screen_tileview_1 = lv_tileview_create(ui->screen_tileview_home_tile_ctrl);
	ui->screen_tileview_1_tile_1 = lv_tileview_add_tile(ui->screen_tileview_1, 0, 0, LV_DIR_RIGHT);
	ui->screen_tileview_1_tile_2 = lv_tileview_add_tile(ui->screen_tileview_1, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
	ui->screen_tileview_1_tile_3 = lv_tileview_add_tile(ui->screen_tileview_1, 2, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
	ui->screen_tileview_1_tile_4 = lv_tileview_add_tile(ui->screen_tileview_1, 3, 0, LV_DIR_LEFT);
	lv_obj_set_pos(ui->screen_tileview_1, 30, 134);
	lv_obj_set_size(ui->screen_tileview_1, 421, 210);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_1, LV_SCROLLBAR_MODE_ON);

	//Write style for screen_tileview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_hex(0x3d4253), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_tileview_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_hex(0xfde4e4), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes screen_label_14
	ui->screen_label_14 = lv_label_create(ui->screen_tileview_1_tile_1);
	lv_label_set_text(ui->screen_label_14, "开关");
	lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_14, 146, 25);
	lv_obj_set_size(ui->screen_label_14, 105, 36);

	//Write style for screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_14, lv_color_hex(0xeeebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_14, &lv_font_MiSans_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_1
	ui->screen_sw_1 = lv_switch_create(ui->screen_tileview_1_tile_1);
	lv_obj_set_pos(ui->screen_sw_1, 144, 84);
	lv_obj_set_size(ui->screen_sw_1, 103, 40);

	//Write style for screen_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0x009efb), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_1, 20, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_opa(ui->screen_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_color(ui->screen_sw_1, lv_color_hex(0x009efb), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_side(ui->screen_sw_1, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0xe6a030), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_1, 15, LV_PART_KNOB|LV_STATE_DEFAULT);



	//Write codes screen_label_15
	ui->screen_label_15 = lv_label_create(ui->screen_tileview_1_tile_2);
	lv_label_set_text(ui->screen_label_15, "按钮");
	lv_label_set_long_mode(ui->screen_label_15, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_15, 146, 25);
	lv_obj_set_size(ui->screen_label_15, 105, 36);

	//Write style for screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_15, lv_color_hex(0xeeebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_15, &lv_font_MiSans_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_15, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_5
	ui->screen_btn_5 = lv_btn_create(ui->screen_tileview_1_tile_2);
	ui->screen_btn_5_label = lv_label_create(ui->screen_btn_5);
	lv_label_set_text(ui->screen_btn_5_label, "Button");
	lv_label_set_long_mode(ui->screen_btn_5_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_5_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_5, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_btn_5, 145, 77);
	lv_obj_set_size(ui->screen_btn_5, 100, 45);

	//Write style for screen_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_btn_5, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_5, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes screen_label_16
	ui->screen_label_16 = lv_label_create(ui->screen_tileview_1_tile_3);
	lv_label_set_text(ui->screen_label_16, "圆弧");
	lv_label_set_long_mode(ui->screen_label_16, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_16, 146, 25);
	lv_obj_set_size(ui->screen_label_16, 105, 36);

	//Write style for screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_16, lv_color_hex(0xf9f8f8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_16, &lv_font_MiSans_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_16, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_arc_1
	ui->screen_arc_1 = lv_arc_create(ui->screen_tileview_1_tile_3);
	lv_arc_set_mode(ui->screen_arc_1, LV_ARC_MODE_NORMAL);
	lv_arc_set_range(ui->screen_arc_1, 0, 100);
	lv_arc_set_bg_angles(ui->screen_arc_1, 0, 360);
	lv_arc_set_angles(ui->screen_arc_1, 0, 45);
	lv_arc_set_rotation(ui->screen_arc_1, 0);
	lv_obj_set_style_arc_rounded(ui->screen_arc_1, 0,  LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_rounded(ui->screen_arc_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_arc_1, 147, 63);
	lv_obj_set_size(ui->screen_arc_1, 100, 100);

	//Write style for screen_arc_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_width(ui->screen_arc_1, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_opa(ui->screen_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_color(ui->screen_arc_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_arc_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_arc_width(ui->screen_arc_1, 12, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_opa(ui->screen_arc_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_color(ui->screen_arc_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style for screen_arc_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_arc_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_arc_1, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->screen_arc_1, 5, LV_PART_KNOB|LV_STATE_DEFAULT);



	//Write codes screen_slider_1
	ui->screen_slider_1 = lv_slider_create(ui->screen_tileview_1_tile_4);
	lv_slider_set_range(ui->screen_slider_1, 0, 100);
	lv_slider_set_mode(ui->screen_slider_1, LV_SLIDER_MODE_NORMAL);
	lv_slider_set_value(ui->screen_slider_1, 50, LV_ANIM_OFF);
	lv_obj_set_pos(ui->screen_slider_1, 69, 100);
	lv_obj_set_size(ui->screen_slider_1, 293, 28);

	//Write style for screen_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_slider_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_slider_1, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0xede9bd), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_slider_1, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style for screen_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0xEDE9BD), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_slider_1, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_label_18
	ui->screen_label_18 = lv_label_create(ui->screen_tileview_1_tile_4);
	lv_label_set_text(ui->screen_label_18, "滑动条");
	lv_label_set_long_mode(ui->screen_label_18, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_18, 146, 25);
	lv_obj_set_size(ui->screen_label_18, 105, 36);

	//Write style for screen_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_18, lv_color_hex(0xeeebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_18, &lv_font_MiSans_Medium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_18, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

	
	//Init events for screen.
	events_init_screen(ui);
}
