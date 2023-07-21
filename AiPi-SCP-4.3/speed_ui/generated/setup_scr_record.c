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


void setup_scr_record(lv_ui *ui){

	//Write codes record
	ui->record = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->record, LV_SCROLLBAR_MODE_OFF);

	//Set style for record. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->record, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->record, lv_color_make(0x05, 0x37, 0x9c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->record, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->record, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes record_chart_board
	ui->record_chart_board = lv_chart_create(ui->record);
	lv_obj_set_pos(ui->record_chart_board, 99, 47);
	lv_obj_set_size(ui->record_chart_board, 366, 197);
	lv_obj_set_scrollbar_mode(ui->record_chart_board, LV_SCROLLBAR_MODE_OFF);

	//Set style for record_chart_board. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->record_chart_board, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->record_chart_board, lv_color_make(0x00, 0xcb, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->record_chart_board, lv_color_make(0x4a, 0x44, 0x44), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->record_chart_board, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->record_chart_board, 223, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->record_chart_board, lv_color_make(0x08, 0x08, 0x08), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->record_chart_board, lv_color_make(0x23, 0x3f, 0xc7), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->record_chart_board, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->record_chart_board, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->record_chart_board, lv_color_make(0x26, 0xcb, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_width(ui->record_chart_board, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->record_chart_board, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_chart_set_type(ui->record_chart_board, LV_CHART_TYPE_LINE);
	lv_chart_set_range(ui->record_chart_board,LV_CHART_AXIS_PRIMARY_Y, 0, 120);
	lv_chart_set_div_line_count(ui->record_chart_board, 5, 6);
	lv_chart_set_point_count(ui->record_chart_board, 100);
	lv_chart_series_t * record_chart_board_0 = lv_chart_add_series(ui->record_chart_board, lv_color_make(0xd0, 0xff, 0x00), LV_CHART_AXIS_PRIMARY_Y);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);
	lv_chart_set_next_value(ui->record_chart_board, record_chart_board_0, 0);

	//Write codes record_label_title
	ui->record_label_title = lv_label_create(ui->record);
	lv_obj_set_pos(ui->record_label_title, 265, 54);
	lv_obj_set_size(ui->record_label_title, 200, 16);
	lv_obj_set_scrollbar_mode(ui->record_label_title, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->record_label_title, "Dynamic Chart");
	lv_label_set_long_mode(ui->record_label_title, LV_LABEL_LONG_WRAP);

	//Set style for record_label_title. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->record_label_title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->record_label_title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->record_label_title, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->record_label_title, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->record_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->record_label_title, lv_color_make(0xfe, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->record_label_title, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->record_label_title, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->record_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->record_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes record_btn_before
	ui->record_btn_before = lv_btn_create(ui->record);
	lv_obj_set_pos(ui->record_btn_before, 19, 123);
	lv_obj_set_size(ui->record_btn_before, 52, 52);
	lv_obj_set_scrollbar_mode(ui->record_btn_before, LV_SCROLLBAR_MODE_OFF);

	//Set style for record_btn_before. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->record_btn_before, 26, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->record_btn_before, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->record_btn_before, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->record_btn_before, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->record_btn_before, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->record_btn_before, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->record_btn_before, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->record_btn_before, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->record_btn_before, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->record_btn_before, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->record_btn_before, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->record_btn_before, lv_color_make(0x52, 0xad, 0xf8), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->record_btn_before, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->record_btn_before, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->record_btn_before, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->record_btn_before, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->record_btn_before, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->record_btn_before_label = lv_label_create(ui->record_btn_before);
	lv_label_set_text(ui->record_btn_before_label, "<");
	lv_obj_set_style_pad_all(ui->record_btn_before, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->record_btn_before_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes record_img_logo
	ui->record_img_logo = lv_img_create(ui->record);
	lv_obj_set_pos(ui->record_img_logo, 11, 11);
	lv_obj_set_size(ui->record_img_logo, 75, 26);
	lv_obj_set_scrollbar_mode(ui->record_img_logo, LV_SCROLLBAR_MODE_OFF);

	//Set style for record_img_logo. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->record_img_logo, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->record_img_logo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->record_img_logo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->record_img_logo, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->record_img_logo,&_logo_alpha_75x26);
	lv_img_set_pivot(ui->record_img_logo, 50,50);
	lv_img_set_angle(ui->record_img_logo, 0);

	//Init events for screen
	events_init_record(ui);
}