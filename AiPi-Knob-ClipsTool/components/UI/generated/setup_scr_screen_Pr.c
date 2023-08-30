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

static lv_obj_t * g_kb_screen_Pr;
static void kb_screen_Pr_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *kb = lv_event_get_target(e);
	if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL){
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_screen_Pr_event_cb(lv_event_t *e)
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

void setup_scr_screen_Pr(lv_ui *ui){

	//Write codes screen_Pr
	ui->screen_Pr = lv_obj_create(NULL);

	//Create keyboard on screen_Pr
	g_kb_screen_Pr = lv_keyboard_create(ui->screen_Pr);
	lv_obj_add_event_cb(g_kb_screen_Pr, kb_screen_Pr_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen_Pr, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_screen_Pr, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->screen_Pr, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_Pr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen_Pr, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_Pr, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_Pr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_Pr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_Pr_img_pr
	ui->screen_Pr_img_pr = lv_img_create(ui->screen_Pr);
	lv_obj_set_pos(ui->screen_Pr_img_pr, 70, 70);
	lv_obj_set_size(ui->screen_Pr_img_pr, 100, 100);
	lv_obj_set_scrollbar_mode(ui->screen_Pr_img_pr, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_Pr_img_pr. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_Pr_img_pr, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_Pr_img_pr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_Pr_img_pr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_Pr_img_pr, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_Pr_img_pr,&_Pr_alpha_100x100);
	lv_img_set_pivot(ui->screen_Pr_img_pr, 50,50);
	lv_img_set_angle(ui->screen_Pr_img_pr, 0);

	//Write codes screen_Pr_label_1
	ui->screen_Pr_label_1 = lv_label_create(ui->screen_Pr);
	lv_obj_set_pos(ui->screen_Pr_label_1, 21, 191);
	lv_obj_set_size(ui->screen_Pr_label_1, 198, 17);
	lv_obj_set_scrollbar_mode(ui->screen_Pr_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_Pr_label_1, "Long press to end");
	lv_label_set_long_mode(ui->screen_Pr_label_1, LV_LABEL_LONG_WRAP);

	//Set style for screen_Pr_label_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_Pr_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_Pr_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_Pr_label_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_Pr_label_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_Pr_label_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_Pr_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_Pr_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_Pr_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen_Pr(ui);
}