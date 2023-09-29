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
#include "FreeRTOS.h"

static lv_obj_t* g_kb_screen_save;

static struct ops_t {
	char* name;
	uint64_t code;
};

struct ops_t ops[] = {
	{"开关",0x00000000},
	{"模式",0x00000000},
	{"升温",0x00000000},
	{"降温",0x00000000},
	{"风向",0x00000000},
	{"风速",0x00000000},
};

static void kb_screen_save_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_screen_save_event_cb(lv_event_t* e)
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

void setup_scr_screen_save(lv_ui* ui) {

	//Write codes screen_save
	ui->screen_save = lv_obj_create(NULL);

	//Create keyboard on screen_save
	g_kb_screen_save = lv_keyboard_create(ui->screen_save);
	lv_obj_add_event_cb(g_kb_screen_save, kb_screen_save_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_screen_save, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_screen_save, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->screen_save, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_save. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen_save, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_save, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_save, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_save, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_save_img_bkg
	ui->screen_save_img_bkg = lv_img_create(ui->screen_save);
	lv_obj_set_pos(ui->screen_save_img_bkg, 0, 0);
	lv_obj_set_size(ui->screen_save_img_bkg, 240, 240);
	lv_obj_set_scrollbar_mode(ui->screen_save_img_bkg, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_save_img_bkg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->screen_save_img_bkg, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_save_img_bkg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_save_img_bkg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_save_img_bkg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_save_img_bkg, &_bk_alpha_240x240);
	lv_img_set_pivot(ui->screen_save_img_bkg, 50, 50);
	lv_img_set_angle(ui->screen_save_img_bkg, 0);

	//Write codes screen_save_cont_2
	ui->screen_save_cont_2 = lv_obj_create(ui->screen_save);
	lv_obj_set_pos(ui->screen_save_cont_2, 0, 0);
	lv_obj_set_size(ui->screen_save_cont_2, 240, 240);
	lv_obj_set_scrollbar_mode(ui->screen_save_cont_2, LV_SCROLLBAR_MODE_AUTO);

	//Set style for screen_save_cont_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_save_cont_2, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_save_cont_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_save_cont_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_save_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_save_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_save_cont_2, lv_color_make(0x9a, 0x91, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_save_cont_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_save_cont_2, 225, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_save_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_save_roller_2
	ui->screen_save_roller_2 = lv_roller_create(ui->screen_save_cont_2);
	lv_obj_set_pos(ui->screen_save_roller_2, 20, 42);
	lv_obj_set_size(ui->screen_save_roller_2, 200, 100);
	lv_obj_set_scrollbar_mode(ui->screen_save_roller_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_save_roller_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_save_roller_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_save_roller_2, lv_color_make(0x06, 0x06, 0x06), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_save_roller_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_save_roller_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_save_roller_2, 105, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_save_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_save_roller_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_save_roller_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_save_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_save_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_save_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_save_roller_2, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_save_roller_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_save_roller_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_save_roller_2, lv_color_make(0x70, 0x65, 0x65), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_save_roller_2, &lv_font_simhei_18, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for screen_save_roller_2. Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->screen_save_roller_2, lv_color_make(0x2e, 0x2f, 0x30), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_save_roller_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_save_roller_2, LV_GRAD_DIR_NONE, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_save_roller_2, 0, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_save_roller_2, lv_color_make(0xFF, 0xFF, 0xFF), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_save_roller_2, &lv_font_simhei_20, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_roller_set_visible_row_count(ui->screen_save_roller_2, 4);
	//加载列表
	{
		dev_ir_t* pcodeTyep = pIRList;
		char* ops_str = pvPortMalloc(128);
		memset(ops_str, 0, 128);
		while (pcodeTyep!=NULL) {
			ops[pcodeTyep->code_type].code = pcodeTyep->code;
			pcodeTyep = pcodeTyep->next;
		}
		sprintf(ops_str,
		"%s:0x%llX\n%s:0x%llX\n%s:0x%llX\n%s:0x%llX\n%s:0x%llX\n%s:0x%llX",
		ops[0].name, ops[0].code, ops[1].name, ops[1].code, ops[2].name, ops[2].code, ops[3].name, ops[3].code, ops[4].name, ops[4].code, ops[5].name, ops[5].code);

		lv_roller_set_options(ui->screen_save_roller_2, ops_str, LV_ROLLER_MODE_INFINITE);
		vPortFree(ops_str);
	}
	//Write codes screen_save_btn_1
	ui->screen_save_btn_1 = lv_btn_create(ui->screen_save_cont_2);
	lv_obj_set_pos(ui->screen_save_btn_1, 89, 189);
	lv_obj_set_size(ui->screen_save_btn_1, 47, 27);
	lv_obj_set_scrollbar_mode(ui->screen_save_btn_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for screen_save_btn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->screen_save_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_save_btn_1, lv_color_make(0x9A, 0x91, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->screen_save_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_save_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_save_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_save_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->screen_save_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->screen_save_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->screen_save_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->screen_save_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->screen_save_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_save_btn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_save_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_save_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_save_btn_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_save_btn_1, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_save_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->screen_save_btn_1_label = lv_label_create(ui->screen_save_btn_1);
	lv_label_set_text(ui->screen_save_btn_1_label, "返回");
	lv_obj_set_style_pad_all(ui->screen_save_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->screen_save_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	ui->screen_save_del = false;
	//Init events for screen
	events_init_screen_save(ui);
}