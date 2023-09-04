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
#include "ble_hid_dev.h"
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
	lv_obj_set_style_bg_grad_color(ui->screen, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_tileview_1
	ui->screen_tileview_1 = lv_tileview_create(ui->screen);
	lv_obj_set_pos(ui->screen_tileview_1, 0, 0);
	lv_obj_set_size(ui->screen_tileview_1, 240, 240);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_1, LV_SCROLLBAR_MODE_ON);

	//Set style for screen_tileview_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_tileview_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_tileview_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_tileview_1. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_1, lv_color_make(0xea, 0xef, 0xf3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Hidden for widget screen_tileview_1
	lv_obj_add_flag(ui->screen_tileview_1, LV_OBJ_FLAG_HIDDEN);

	//add new tile tile_JY
	ui->tile_JY = lv_tileview_add_tile(ui->screen_tileview_1, 1, 0, LV_DIR_LEFT);

	//Write codes screen_imgbtn_JY
	ui->screen_imgbtn_JY = lv_imgbtn_create(ui->tile_JY);
	lv_obj_set_pos(ui->screen_imgbtn_JY, 70, 70);
	lv_obj_set_size(ui->screen_imgbtn_JY, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_JY, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_JY. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_JY, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_JY, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_JY, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_JY, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_JY. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_JY, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_JY, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_JY, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_JY. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_JY, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_JY, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_JY, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_JY, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_JY, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_imgbtn_JY, LV_IMGBTN_STATE_RELEASED, NULL, &_JY_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_JY, LV_IMGBTN_STATE_PRESSED, NULL, &_JY_s_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_JY, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_JY_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_JY, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_JY_s_alpha_100x100, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_JY, LV_OBJ_FLAG_CHECKABLE);

	//add new tile tile_Pr
	ui->tile_Pr = lv_tileview_add_tile(ui->screen_tileview_1, 0, 0, LV_DIR_RIGHT);

	//Write codes screen_imgbtn_Pr
	ui->screen_imgbtn_Pr = lv_imgbtn_create(ui->tile_Pr);
	lv_obj_set_pos(ui->screen_imgbtn_Pr, 70, 70);
	lv_obj_set_size(ui->screen_imgbtn_Pr, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_imgbtn_Pr, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_imgbtn_Pr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_Pr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_imgbtn_Pr, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_Pr, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_Pr, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_imgbtn_Pr. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_Pr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_Pr, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_Pr, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_imgbtn_Pr. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_imgbtn_Pr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_Pr, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_imgbtn_Pr, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_Pr, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_imgbtn_Pr, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_imgbtn_Pr, LV_IMGBTN_STATE_RELEASED, NULL, &_Pr_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_Pr, LV_IMGBTN_STATE_PRESSED, NULL, &_Pr_s_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_Pr, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_Pr_alpha_100x100, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_Pr, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_Pr_s_alpha_100x100, NULL);
	lv_obj_add_flag(ui->screen_imgbtn_Pr, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_1, 70, 40);
	lv_obj_set_size(ui->screen_img_1, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_img_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1, &_BLE_NO_alpha_100x100);
	lv_img_set_pivot(ui->screen_img_1, 50, 50);
	lv_img_set_angle(ui->screen_img_1, 0);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_3, 30, 140);
	lv_obj_set_size(ui->screen_label_3, 180, 40);
	lv_obj_set_scrollbar_mode(ui->screen_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_3, "BLE Disconnect\nPlease connect:");
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
	lv_obj_set_style_text_color(ui->screen_label_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_4, 28, 174);
	lv_obj_set_size(ui->screen_label_4, 182, 25);
	lv_obj_set_scrollbar_mode(ui->screen_label_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_4, BLE_DEV_NAME);
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);

	//Set style for screen_label_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_4, lv_color_make(0x2c, 0xb1, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_4, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen(ui);
}