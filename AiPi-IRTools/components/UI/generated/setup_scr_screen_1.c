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

static lv_obj_t* g_kb_screen_1;
static void kb_screen_1_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_screen_1_event_cb(lv_event_t* e)
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

void setup_scr_screen_1(lv_ui* ui) {

	//Write codes screen_1
	ui->screen_1 = lv_obj_create(NULL);

	//Create keyboard on screen_1
	g_kb_screen_1 = lv_keyboard_create(ui->screen_1);
	lv_obj_add_event_cb(g_kb_screen_1, kb_screen_1_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen_1, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_screen_1, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_1_img_bk3
	ui->screen_1_img_bk3 = lv_img_create(ui->screen_1);
	lv_obj_set_pos(ui->screen_1_img_bk3, 0, 0);
	lv_obj_set_size(ui->screen_1_img_bk3, 240, 240);
	lv_obj_set_scrollbar_mode(ui->screen_1_img_bk3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_1_img_bk3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_1_img_bk3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_1_img_bk3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_1_img_bk3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_1_img_bk3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_1_img_bk3, &_bk_alpha_240x240);
	lv_img_set_pivot(ui->screen_1_img_bk3, 50, 50);
	lv_img_set_angle(ui->screen_1_img_bk3, 0);

	//Write codes screen_1_cont_3
	ui->screen_1_cont_3 = lv_obj_create(ui->screen_1);
	lv_obj_set_pos(ui->screen_1_cont_3, 0, 0);
	lv_obj_set_size(ui->screen_1_cont_3, 240, 240);
	lv_obj_set_scrollbar_mode(ui->screen_1_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_1_cont_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_1_cont_3, lv_color_make(0x81, 0x79, 0x79), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_1_cont_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_1_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_1_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_1_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_1_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_1_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_1_btn_ok
	ui->screen_1_btn_ok = lv_btn_create(ui->screen_1_cont_3);
	lv_obj_set_pos(ui->screen_1_btn_ok, 45, 170);
	lv_obj_set_size(ui->screen_1_btn_ok, 47, 27);
	lv_obj_set_scrollbar_mode(ui->screen_1_btn_ok, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_1_btn_ok. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_1_btn_ok, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_1_btn_ok, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_1_btn_ok, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_ok, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_1_btn_ok, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_1_btn_ok, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_1_btn_ok, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_1_btn_ok, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_1_btn_ok, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_1_btn_ok, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_1_btn_ok, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_1_btn_ok, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_1_btn_ok, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_1_btn_ok, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_1_btn_ok, lv_color_make(0xeb, 0xe1, 0xe1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_1_btn_ok, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_1_btn_ok, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_1_btn_ok_label = lv_label_create(ui->screen_1_btn_ok);
	lv_label_set_text(ui->screen_1_btn_ok_label, "确定");
	lv_obj_set_style_pad_all(ui->screen_1_btn_ok, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_1_btn_ok_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes screen_1_ddlist_cmd_list
	ui->screen_1_ddlist_cmd_list = lv_dropdown_create(ui->screen_1_cont_3);
	lv_obj_set_pos(ui->screen_1_ddlist_cmd_list, 35, 73);
	lv_obj_set_size(ui->screen_1_ddlist_cmd_list, 165, 31);
	lv_obj_set_scrollbar_mode(ui->screen_1_ddlist_cmd_list, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->screen_1_ddlist_cmd_list, "开关\n模式\n升温\n降温\n风速\n风向");

	//Set style for screen_1_ddlist_cmd_list. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_1_ddlist_cmd_list, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_1_ddlist_cmd_list, lv_color_make(0x57, 0x57, 0x57), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_1_ddlist_cmd_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_1_ddlist_cmd_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_1_ddlist_cmd_list, 217, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_1_ddlist_cmd_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_1_ddlist_cmd_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_1_ddlist_cmd_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_1_ddlist_cmd_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_1_ddlist_cmd_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_1_ddlist_cmd_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_1_ddlist_cmd_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_1_ddlist_cmd_list, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_1_ddlist_cmd_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_1_ddlist_cmd_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_1_ddlist_cmd_list, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_1_ddlist_cmd_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_1_ddlist_cmd_list, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_1_ddlist_cmd_list, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_screen_1_ddlist_cmd_list_extra_list_selected_checked
	static lv_style_t style_screen_1_ddlist_cmd_list_extra_list_selected_checked;
	ui_init_style(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked);
	lv_style_set_radius(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, lv_color_make(0x89, 0x8c, 0x8c));
	lv_style_set_bg_grad_color(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, 255);
	lv_style_set_text_color(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_1_ddlist_cmd_list_extra_list_selected_checked, &lv_font_simhei_12);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_ddlist_cmd_list), &style_screen_1_ddlist_cmd_list_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_screen_1_ddlist_cmd_list_extra_list_main_default
	static lv_style_t style_screen_1_ddlist_cmd_list_extra_list_main_default;
	ui_init_style(&style_screen_1_ddlist_cmd_list_extra_list_main_default);
	lv_style_set_radius(&style_screen_1_ddlist_cmd_list_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_screen_1_ddlist_cmd_list_extra_list_main_default, lv_color_make(0x38, 0x38, 0x38));
	lv_style_set_bg_grad_color(&style_screen_1_ddlist_cmd_list_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_1_ddlist_cmd_list_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_1_ddlist_cmd_list_extra_list_main_default, 137);
	lv_style_set_border_color(&style_screen_1_ddlist_cmd_list_extra_list_main_default, lv_color_make(0x7f, 0x7f, 0x7f));
	lv_style_set_border_width(&style_screen_1_ddlist_cmd_list_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_1_ddlist_cmd_list_extra_list_main_default, 167);
	lv_style_set_text_color(&style_screen_1_ddlist_cmd_list_extra_list_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_text_font(&style_screen_1_ddlist_cmd_list_extra_list_main_default, &lv_font_simhei_12);
	lv_style_set_max_height(&style_screen_1_ddlist_cmd_list_extra_list_main_default, 90);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_ddlist_cmd_list), &style_screen_1_ddlist_cmd_list_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default
	static lv_style_t style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default;
	ui_init_style(&style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default);
	lv_style_set_radius(&style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default, lv_color_make(0xa0, 0xb1, 0xbd));
	lv_style_set_bg_opa(&style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default, 180);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_1_ddlist_cmd_list), &style_screen_1_ddlist_cmd_list_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_1_btn_list
	ui->screen_1_btn_list = lv_btn_create(ui->screen_1_cont_3);
	lv_obj_set_pos(ui->screen_1_btn_list, 137, 170);
	lv_obj_set_size(ui->screen_1_btn_list, 47, 27);
	lv_obj_set_scrollbar_mode(ui->screen_1_btn_list, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_1_btn_list. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_1_btn_list, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_1_btn_list, lv_color_make(0x99, 0x9d, 0x06), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_1_btn_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_1_btn_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_1_btn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_1_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_1_btn_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_1_btn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_1_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_1_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_1_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_1_btn_list, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_1_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_1_btn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_1_btn_list, lv_color_make(0xeb, 0xe1, 0xe1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_1_btn_list, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_1_btn_list, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_1_btn_list_label = lv_label_create(ui->screen_1_btn_list);
	lv_label_set_text(ui->screen_1_btn_list_label, "列表");
	lv_obj_set_style_pad_all(ui->screen_1_btn_list, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_1_btn_list_label, LV_ALIGN_CENTER, 0, 0);
	ui->screen_1_del = false;
	//Init events for screen
	events_init_screen_1(ui);
}