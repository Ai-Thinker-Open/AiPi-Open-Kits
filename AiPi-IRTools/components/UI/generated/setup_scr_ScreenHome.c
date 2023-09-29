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
#include "dev_ir.h"
static lv_obj_t* g_kb_ScreenHome;
static void kb_ScreenHome_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_ScreenHome_event_cb(lv_event_t* e)
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

void setup_scr_ScreenHome(lv_ui* ui) {

	//Write codes ScreenHome
	ui->ScreenHome = lv_obj_create(NULL);

	//Create keyboard on ScreenHome
	g_kb_ScreenHome = lv_keyboard_create(ui->ScreenHome);
	lv_obj_add_event_cb(g_kb_ScreenHome, kb_ScreenHome_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_ScreenHome, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_ScreenHome, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->ScreenHome, LV_SCROLLBAR_MODE_OFF);

	//Set style for ScreenHome. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->ScreenHome, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ScreenHome, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ScreenHome, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ScreenHome, 254, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ScreenHome_img_bg
	ui->ScreenHome_img_bg = lv_img_create(ui->ScreenHome);
	lv_obj_set_pos(ui->ScreenHome_img_bg, 0, 0);
	lv_obj_set_size(ui->ScreenHome_img_bg, 240, 240);
	lv_obj_set_scrollbar_mode(ui->ScreenHome_img_bg, LV_SCROLLBAR_MODE_OFF);

	//Set style for ScreenHome_img_bg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->ScreenHome_img_bg, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->ScreenHome_img_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->ScreenHome_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->ScreenHome_img_bg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->ScreenHome_img_bg, &_bk_alpha_240x240);
	lv_img_set_pivot(ui->ScreenHome_img_bg, 50, 50);
	lv_img_set_angle(ui->ScreenHome_img_bg, 0);

	//Write codes ScreenHome_cont_1
	ui->ScreenHome_cont_1 = lv_obj_create(ui->ScreenHome);
	lv_obj_set_pos(ui->ScreenHome_cont_1, 0, 0);
	lv_obj_set_size(ui->ScreenHome_cont_1, 240, 240);
	lv_obj_set_scrollbar_mode(ui->ScreenHome_cont_1, LV_SCROLLBAR_MODE_AUTO);

	//Set style for ScreenHome_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ScreenHome_cont_1, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ScreenHome_cont_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ScreenHome_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ScreenHome_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ScreenHome_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->ScreenHome_cont_1, lv_color_make(0xa9, 0x4b, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->ScreenHome_cont_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->ScreenHome_cont_1, 225, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->ScreenHome_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes ScreenHome_roller_1
	ui->ScreenHome_roller_1 = lv_roller_create(ui->ScreenHome_cont_1);
	lv_obj_set_pos(ui->ScreenHome_roller_1, 73, 48);
	lv_obj_set_scrollbar_mode(ui->ScreenHome_roller_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for ScreenHome_roller_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ScreenHome_roller_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ScreenHome_roller_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ScreenHome_roller_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ScreenHome_roller_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ScreenHome_roller_1, 35, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ScreenHome_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ScreenHome_roller_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ScreenHome_roller_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ScreenHome_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ScreenHome_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ScreenHome_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->ScreenHome_roller_1, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->ScreenHome_roller_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->ScreenHome_roller_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ScreenHome_roller_1, lv_color_make(0x76, 0x74, 0x74), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ScreenHome_roller_1, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for ScreenHome_roller_1. Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->ScreenHome_roller_1, lv_color_make(0x8e, 0x8e, 0x8e), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ScreenHome_roller_1, lv_color_make(0xb4, 0x11, 0x4d), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ScreenHome_roller_1, LV_GRAD_DIR_VER, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ScreenHome_roller_1, 0, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ScreenHome_roller_1, lv_color_make(0xFF, 0xFF, 0xFF), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ScreenHome_roller_1, &lv_font_montserratMedium_24, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_roller_set_options(ui->ScreenHome_roller_1, "NEC\nRC5\nSWM", LV_ROLLER_MODE_INFINITE);
	lv_roller_set_visible_row_count(ui->ScreenHome_roller_1, 3);

	lv_roller_set_selected(ui->ScreenHome_roller_1, dev_ir_type, LV_ANIM_ON);
	//Write codes ScreenHome_btn_2
	ui->ScreenHome_btn_2 = lv_btn_create(ui->ScreenHome_cont_1);
	lv_obj_set_pos(ui->ScreenHome_btn_2, 89, 189);
	lv_obj_set_size(ui->ScreenHome_btn_2, 47, 27);
	lv_obj_set_scrollbar_mode(ui->ScreenHome_btn_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for ScreenHome_btn_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->ScreenHome_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->ScreenHome_btn_2, lv_color_make(0xa9, 0x4b, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->ScreenHome_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->ScreenHome_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->ScreenHome_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->ScreenHome_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->ScreenHome_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->ScreenHome_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->ScreenHome_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->ScreenHome_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->ScreenHome_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->ScreenHome_btn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->ScreenHome_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->ScreenHome_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->ScreenHome_btn_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->ScreenHome_btn_2, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->ScreenHome_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->ScreenHome_btn_2_label = lv_label_create(ui->ScreenHome_btn_2);
	lv_label_set_text(ui->ScreenHome_btn_2_label, "返回");
	lv_obj_set_style_pad_all(ui->ScreenHome_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->ScreenHome_btn_2_label, LV_ALIGN_CENTER, 0, 0);
	ui->ScreenHome_del = false;
	//Init events for screen
	events_init_ScreenHome(ui);
}