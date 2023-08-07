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

static lv_obj_t * g_kb_screen_rgb3;
static void kb_screen_rgb3_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *kb = lv_event_get_target(e);
	if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL){
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_screen_rgb3_event_cb(lv_event_t *e)
{

	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = lv_event_get_user_data(e);
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

void setup_scr_screen_rgb3(lv_ui *ui){

	//Write codes screen_rgb3
	ui->screen_rgb3 = lv_obj_create(NULL);

	//Create keyboard on screen_rgb3
	g_kb_screen_rgb3 = lv_keyboard_create(ui->screen_rgb3);
	lv_obj_add_event_cb(g_kb_screen_rgb3, kb_screen_rgb3_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen_rgb3, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_screen_rgb3, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_rgb3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen_rgb3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_rgb3_img_rgb3_bg
	ui->screen_rgb3_img_rgb3_bg = lv_img_create(ui->screen_rgb3);
	lv_obj_set_pos(ui->screen_rgb3_img_rgb3_bg, 0, 0);
	lv_obj_set_size(ui->screen_rgb3_img_rgb3_bg, 320, 240);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_img_rgb3_bg, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_rgb3_img_rgb3_bg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_rgb3_img_rgb3_bg, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_rgb3_img_rgb3_bg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_rgb3_img_rgb3_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_rgb3_img_rgb3_bg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_rgb3_img_rgb3_bg,&_img_bg_alpha_320x240);
	lv_img_set_pivot(ui->screen_rgb3_img_rgb3_bg, 50,50);
	lv_img_set_angle(ui->screen_rgb3_img_rgb3_bg, 0);

	//Write codes screen_rgb3_imgbtn_brack3
	ui->screen_rgb3_imgbtn_brack3 = lv_imgbtn_create(ui->screen_rgb3);
	lv_obj_set_pos(ui->screen_rgb3_imgbtn_brack3, 0, 0);
	lv_obj_set_size(ui->screen_rgb3_imgbtn_brack3, 40, 40);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_imgbtn_brack3, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_rgb3_imgbtn_brack3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_rgb3_imgbtn_brack3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_rgb3_imgbtn_brack3. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for screen_rgb3_imgbtn_brack3. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->screen_rgb3_imgbtn_brack3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->screen_rgb3_imgbtn_brack3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->screen_rgb3_imgbtn_brack3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->screen_rgb3_imgbtn_brack3, LV_IMGBTN_STATE_RELEASED, NULL, &_bak_a_alpha_40x40, NULL);
	lv_imgbtn_set_src(ui->screen_rgb3_imgbtn_brack3, LV_IMGBTN_STATE_PRESSED, NULL, &_bak_s_alpha_40x40, NULL);
	lv_imgbtn_set_src(ui->screen_rgb3_imgbtn_brack3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_bak_a_alpha_40x40, NULL);
	lv_imgbtn_set_src(ui->screen_rgb3_imgbtn_brack3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_bak_s_alpha_40x40, NULL);
	lv_obj_add_flag(ui->screen_rgb3_imgbtn_brack3, LV_OBJ_FLAG_CHECKABLE);

	//Write codes screen_rgb3_label_bw16_tile
	ui->screen_rgb3_label_bw16_tile = lv_label_create(ui->screen_rgb3);
	lv_obj_set_pos(ui->screen_rgb3_label_bw16_tile, 105, 6);
	lv_obj_set_size(ui->screen_rgb3_label_bw16_tile, 112, 17);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_label_bw16_tile, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_rgb3_label_bw16_tile, "BW16 开发板");
	lv_label_set_long_mode(ui->screen_rgb3_label_bw16_tile, LV_LABEL_LONG_WRAP);

	//Set style for screen_rgb3_label_bw16_tile. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_rgb3_label_bw16_tile, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_label_bw16_tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_label_bw16_tile, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_label_bw16_tile, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_rgb3_label_bw16_tile, lv_color_make(0xc9, 0xe3, 0xf5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_rgb3_label_bw16_tile, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_rgb3_label_bw16_tile, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_rgb3_label_bw16_tile, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_rgb3_cont_bw16_sw
	ui->screen_rgb3_cont_bw16_sw = lv_obj_create(ui->screen_rgb3);
	lv_obj_set_pos(ui->screen_rgb3_cont_bw16_sw, 227, 48);
	lv_obj_set_size(ui->screen_rgb3_cont_bw16_sw, 82, 145);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_cont_bw16_sw, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_rgb3_cont_bw16_sw. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_rgb3_cont_bw16_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_rgb3_cont_bw16_sw, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_cont_bw16_sw, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_cont_bw16_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_cont_bw16_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_cont_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_cont_bw16_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_rgb3_cont_bw16_sw, lv_color_make(0x22, 0x35, 0x5f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_rgb3_cont_bw16_sw, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_rgb3_cont_bw16_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_rgb3_cont_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_rgb3_bw16_sw
	ui->screen_rgb3_bw16_sw = lv_switch_create(ui->screen_rgb3_cont_bw16_sw);
	lv_obj_set_pos(ui->screen_rgb3_bw16_sw, 9, 47);
	lv_obj_set_size(ui->screen_rgb3_bw16_sw, 61, 27);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_bw16_sw, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_rgb3_bw16_sw. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_rgb3_bw16_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x8e, 0x94, 0xc2), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_bw16_sw, lv_color_make(0xe6, 0xe2, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_bw16_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_bw16_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_bw16_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_rgb3_bw16_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_rgb3_bw16_sw. Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED
	lv_obj_set_style_bg_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x20, 0x6d, 0xc0), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_bw16_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_bw16_sw, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_rgb3_bw16_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_opa(ui->screen_rgb3_bw16_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Set style for screen_rgb3_bw16_sw. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_rgb3_bw16_sw, 100, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_rgb3_bw16_sw, lv_color_make(0xea, 0xa0, 0x55), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_bw16_sw, lv_color_make(0xff, 0xff, 0xff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_bw16_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_bw16_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_rgb3_bw16_sw, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_rgb3_bw16_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_rgb3_bw16_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_rgb3_label_6
	ui->screen_rgb3_label_6 = lv_label_create(ui->screen_rgb3_cont_bw16_sw);
	lv_obj_set_pos(ui->screen_rgb3_label_6, 23, 77);
	lv_obj_set_size(ui->screen_rgb3_label_6, 35, 23);
	lv_obj_set_scrollbar_mode(ui->screen_rgb3_label_6, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_rgb3_label_6, "开关");
	lv_label_set_long_mode(ui->screen_rgb3_label_6, LV_LABEL_LONG_WRAP);

	//Set style for screen_rgb3_label_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_rgb3_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_rgb3_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_rgb3_label_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_rgb3_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_rgb3_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_rgb3_label_6, lv_color_make(0xf2, 0xf1, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_rgb3_label_6, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_rgb3_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_rgb3_label_6, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_rgb3_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_rgb3_cpicker_bw16_rgb
	ui->screen_rgb3_cpicker_bw16_rgb = lv_colorwheel_create(ui->screen_rgb3, true);
	lv_obj_set_pos(ui->screen_rgb3_cpicker_bw16_rgb, 20, 48);
	lv_obj_set_size(ui->screen_rgb3_cpicker_bw16_rgb, 170, 170);

	//Set style for screen_rgb3_cpicker_bw16_rgb. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_arc_width(ui->screen_rgb3_cpicker_bw16_rgb, 20, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen_rgb3(ui);
}