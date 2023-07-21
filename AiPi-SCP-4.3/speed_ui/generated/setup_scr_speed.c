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


void setup_scr_speed(lv_ui *ui){

	//Write codes speed
	ui->speed = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->speed, LV_SCROLLBAR_MODE_OFF);

	//Set style for speed. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->speed, lv_color_make(0x36, 0x44, 0x98), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed, lv_color_make(0x0d, 0x2c, 0x73), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed, LV_GRAD_DIR_HOR, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes speed_btn_next
	ui->speed_btn_next = lv_btn_create(ui->speed);
	lv_obj_set_pos(ui->speed_btn_next, 374, 110);
	lv_obj_set_size(ui->speed_btn_next, 52, 52);
	lv_obj_set_scrollbar_mode(ui->speed_btn_next, LV_SCROLLBAR_MODE_OFF);

	//Set style for speed_btn_next. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_btn_next, 26, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_btn_next, lv_color_make(0x00, 0x43, 0xa1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_btn_next, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_btn_next, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_btn_next, 239, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_btn_next, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_btn_next, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_btn_next, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_btn_next, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_btn_next, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_btn_next, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->speed_btn_next, lv_color_make(0x00, 0x2b, 0x70), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->speed_btn_next, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->speed_btn_next, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->speed_btn_next, lv_color_make(0xa3, 0xdd, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->speed_btn_next, &lv_font_montserratMedium_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->speed_btn_next, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->speed_btn_next_label = lv_label_create(ui->speed_btn_next);
	lv_label_set_text(ui->speed_btn_next_label, ">");
	lv_obj_set_style_pad_all(ui->speed_btn_next, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->speed_btn_next_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes speed_cont_box
	ui->speed_cont_box = lv_obj_create(ui->speed);
	lv_obj_set_pos(ui->speed_cont_box, 6, 1);
	lv_obj_set_size(ui->speed_cont_box, 356, 271);
	lv_obj_set_scrollbar_mode(ui->speed_cont_box, LV_SCROLLBAR_MODE_OFF);

	//Set style for speed_cont_box. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_cont_box, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_cont_box, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_cont_box, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_cont_box, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_cont_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->speed_cont_box, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->speed_cont_box, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->speed_cont_box, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes speed_meter_board
	ui->speed_meter_board = lv_meter_create(ui->speed_cont_box);
	lv_obj_set_pos(ui->speed_meter_board, 29, 11);
	lv_obj_set_size(ui->speed_meter_board, 250, 250);
	lv_obj_set_scrollbar_mode(ui->speed_meter_board, LV_SCROLLBAR_MODE_OFF);

	//add scale speed_meter_board_scale_1
	lv_meter_scale_t *speed_meter_board_scale_1 = lv_meter_add_scale(ui->speed_meter_board);
	lv_meter_set_scale_ticks(ui->speed_meter_board, speed_meter_board_scale_1, 61, 2, 10, lv_color_make(0x4a, 0x69, 0xff));
	lv_meter_set_scale_major_ticks(ui->speed_meter_board, speed_meter_board_scale_1, 5, 4, 16, lv_color_make(0xff, 0xff, 0xff), 15);
	lv_meter_set_scale_range(ui->speed_meter_board, speed_meter_board_scale_1, 0, 120, 300, 120);

	//add arc for speed_meter_board_scale_1
	lv_meter_indicator_t *speed_meter_board_scale_1_arc_0;
	speed_meter_board_scale_1_arc_0 = lv_meter_add_arc(ui->speed_meter_board, speed_meter_board_scale_1, 10, lv_color_make(0x00, 0xd6, 0xc2), 0);
	lv_meter_set_indicator_start_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_0, 0);
	lv_meter_set_indicator_end_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_0, 50);

	//add arc for speed_meter_board_scale_1
	lv_meter_indicator_t *speed_meter_board_scale_1_arc_1;
	speed_meter_board_scale_1_arc_1 = lv_meter_add_arc(ui->speed_meter_board, speed_meter_board_scale_1, 10, lv_color_make(0x95, 0x00, 0x80), 0);
	lv_meter_set_indicator_start_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_1, 80);
	lv_meter_set_indicator_end_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_1, 120);

	//add arc for speed_meter_board_scale_1
	lv_meter_indicator_t *speed_meter_board_scale_1_arc_2;
	speed_meter_board_scale_1_arc_2 = lv_meter_add_arc(ui->speed_meter_board, speed_meter_board_scale_1, 10, lv_color_make(0xb3, 0xdc, 0x00), 0);
	lv_meter_set_indicator_start_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_2, 50);
	lv_meter_set_indicator_end_value(ui->speed_meter_board, speed_meter_board_scale_1_arc_2, 80);

	//add needle line for speed_meter_board_scale_1
	ui->speed_meter_board_scale_1_ndline_0 = lv_meter_add_needle_line(ui->speed_meter_board, speed_meter_board_scale_1, 4, lv_color_make(0xf7, 0xf8, 0xff), -12);
	lv_meter_set_indicator_value(ui->speed_meter_board, ui->speed_meter_board_scale_1_ndline_0, 5);

	//Set style for speed_meter_board. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_meter_board, 125, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_meter_board, lv_color_make(0x14, 0x2c, 0x58), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_meter_board, lv_color_make(0x16, 0x77, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_meter_board, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_meter_board, 251, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_meter_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_meter_board, lv_color_make(0x28, 0x29, 0x29), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_meter_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_meter_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_meter_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_meter_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->speed_meter_board, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->speed_meter_board, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->speed_meter_board, 181, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for speed_meter_board. Part: LV_PART_TICKS, State: LV_STATE_DEFAULT
	lv_obj_set_style_text_color(ui->speed_meter_board, lv_color_make(0xff, 0xff, 0xff), LV_PART_TICKS|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->speed_meter_board, &lv_font_arial_15, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Write codes speed_img_logo
	// ui->speed_img_logo = lv_img_create(ui->speed_cont_box);
	// lv_obj_set_pos(ui->speed_img_logo, 122, 92);
	// lv_obj_set_size(ui->speed_img_logo, 69, 20);
	// lv_obj_set_scrollbar_mode(ui->speed_img_logo, LV_SCROLLBAR_MODE_OFF);

	// //Set style for speed_img_logo. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	// lv_obj_set_style_img_recolor(ui->speed_img_logo, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	// lv_obj_set_style_img_recolor_opa(ui->speed_img_logo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	// lv_obj_set_style_img_opa(ui->speed_img_logo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	// lv_obj_add_flag(ui->speed_img_logo, LV_OBJ_FLAG_CLICKABLE);
	// lv_img_set_src(ui->speed_img_logo,&_logo_alpha_69x20);
	// lv_img_set_pivot(ui->speed_img_logo, 50,50);
	// lv_img_set_angle(ui->speed_img_logo, 0);

	//Write codes speed_label_title
	ui->speed_label_title = lv_label_create(ui->speed_cont_box);
	lv_obj_set_pos(ui->speed_label_title, 113, 163);
	lv_obj_set_size(ui->speed_label_title, 82, 21);
	lv_obj_set_scrollbar_mode(ui->speed_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->speed_label_title, "Speed");
	lv_label_set_long_mode(ui->speed_label_title, LV_LABEL_LONG_WRAP);

	//Set style for speed_label_title. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_label_title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_label_title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_label_title, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_label_title, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->speed_label_title, lv_color_make(0xe4, 0xee, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->speed_label_title, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->speed_label_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->speed_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->speed_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes speed_label_speed_unit
	ui->speed_label_speed_unit = lv_label_create(ui->speed_cont_box);
	lv_obj_set_pos(ui->speed_label_speed_unit, 128, 228);
	lv_obj_set_size(ui->speed_label_speed_unit, 54, 29);
	lv_obj_set_scrollbar_mode(ui->speed_label_speed_unit, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->speed_label_speed_unit, "Km/h");
	lv_label_set_long_mode(ui->speed_label_speed_unit, LV_LABEL_LONG_WRAP);

	//Set style for speed_label_speed_unit. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_label_speed_unit, lv_color_make(0xfd, 0xfd, 0xfe), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_label_speed_unit, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_label_speed_unit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_label_speed_unit, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_label_speed_unit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->speed_label_speed_unit, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->speed_label_speed_unit, &lv_font_Alatsi_Regular_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->speed_label_speed_unit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->speed_label_speed_unit, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->speed_label_speed_unit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes speed_label_digit
	ui->speed_label_digit = lv_label_create(ui->speed_cont_box);
	lv_obj_set_pos(ui->speed_label_digit, 128, 202);
	lv_obj_set_size(ui->speed_label_digit, 54, 29);
	lv_obj_set_scrollbar_mode(ui->speed_label_digit, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->speed_label_digit, "0");
	lv_label_set_long_mode(ui->speed_label_digit, LV_LABEL_LONG_WRAP);

	//Set style for speed_label_digit. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->speed_label_digit, lv_color_make(0xfd, 0xfd, 0xfe), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->speed_label_digit, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->speed_label_digit, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->speed_label_digit, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->speed_label_digit, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->speed_label_digit, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->speed_label_digit, &lv_font_Alatsi_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->speed_label_digit, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->speed_label_digit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->speed_label_digit, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->speed_label_digit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_speed(ui);
}