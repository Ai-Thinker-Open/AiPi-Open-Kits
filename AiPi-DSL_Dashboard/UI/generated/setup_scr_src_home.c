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

int src_home_digital_clock_1_hour_value = 11;
int src_home_digital_clock_1_min_value = 25;
int src_home_digital_clock_1_sec_value = 50;
int src_home_digital_date_yesr_value = 1970;
int src_home_digital_date_mont_value = 7;
int src_home_digital_date_day_value = 11;
int src_home_digital_date_wday_value = 2;
void src_home_digital_clock_1_timer(lv_timer_t* timer)
{
	clock_count_24(&src_home_digital_clock_1_hour_value, &src_home_digital_clock_1_min_value, &src_home_digital_clock_1_sec_value);
	if (lv_obj_is_valid(guider_ui.src_home_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.src_home_digital_clock_1, "%02d:%02d:%02d", src_home_digital_clock_1_hour_value, src_home_digital_clock_1_min_value, src_home_digital_clock_1_sec_value);
	}
}
static lv_obj_t* g_kb_src_home;
static void kb_src_home_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_src_home_event_cb(lv_event_t* e)
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

void setup_scr_src_home(lv_ui* ui) {

	//Write codes src_home
	ui->src_home = lv_obj_create(NULL);

	//Create keyboard on src_home
	g_kb_src_home = lv_keyboard_create(ui->src_home);
	lv_obj_add_event_cb(g_kb_src_home, kb_src_home_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_src_home, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_src_home, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->src_home, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->src_home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_cont_img
	ui->src_home_cont_img = lv_obj_create(ui->src_home);
	lv_obj_set_pos(ui->src_home_cont_img, 39, 95);
	lv_obj_set_size(ui->src_home_cont_img, 163, 130);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_img, LV_SCROLLBAR_MODE_OFF);

	//Hidden for widget src_home_cont_img
	lv_obj_add_flag(ui->src_home_cont_img, LV_OBJ_FLAG_HIDDEN);

	//Write codes src_home_img_13
	ui->src_home_img_13 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_13, 70, 110);
	lv_obj_set_size(ui->src_home_img_13, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_13, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_13. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_13, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_13, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_13
	lv_obj_add_flag(ui->src_home_img_13, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_13, &_tianqiqing_i_yintian_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_13, 50, 50);
	lv_img_set_angle(ui->src_home_img_13, 0);

	//Write codes src_home_img_7
	ui->src_home_img_7 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_7, 131, 212);
	lv_obj_set_size(ui->src_home_img_7, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_7, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_7. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_7, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_7, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_7
	lv_obj_add_flag(ui->src_home_img_7, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_7, &_tianqiqing_i_baoyu_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_7, 50, 50);
	lv_img_set_angle(ui->src_home_img_7, 0);

	//Write codes src_home_img_6
	ui->src_home_img_6 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_6, 70, 110);
	lv_obj_set_size(ui->src_home_img_6, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_6, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_6, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_6
	lv_obj_add_flag(ui->src_home_img_6, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_6, &_tianqiqingyeduoyun_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_6, 50, 50);
	lv_img_set_angle(ui->src_home_img_6, 0);

	//Write codes src_home_img_5
	ui->src_home_img_5 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_5, 70, 110);
	lv_obj_set_size(ui->src_home_img_5, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_5, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_5, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_5
	lv_obj_add_flag(ui->src_home_img_5, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_5, &_tianqiduoyunxiaoyuzhuanqing_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_5, 50, 50);
	lv_img_set_angle(ui->src_home_img_5, 0);

	//Write codes src_home_img_4
	ui->src_home_img_4 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_4, 70, 110);
	lv_obj_set_size(ui->src_home_img_4, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_4, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_4, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_4
	lv_obj_add_flag(ui->src_home_img_4, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_4, &_tianqiduoyun_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_4, 50, 50);
	lv_img_set_angle(ui->src_home_img_4, 0);

	//Write codes src_home_img_3
	ui->src_home_img_3 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_3, 70, 110);
	lv_obj_set_size(ui->src_home_img_3, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_3, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_3
	lv_obj_add_flag(ui->src_home_img_3, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_3, &_tianqidayu_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_3, 50, 50);
	lv_img_set_angle(ui->src_home_img_3, 0);

	//Write codes src_home_img_2
	ui->src_home_img_2 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_2, 70, 110);
	lv_obj_set_size(ui->src_home_img_2, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_2, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_2
	lv_obj_add_flag(ui->src_home_img_2, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_2, &_tianqidabaoyu_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_2, 50, 50);
	lv_img_set_angle(ui->src_home_img_2, 0);

	//Write codes src_home_img_12
	ui->src_home_img_12 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_12, 70, 110);
	lv_obj_set_size(ui->src_home_img_12, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_12, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_12. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_12, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_12, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_12
	lv_obj_add_flag(ui->src_home_img_12, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_12, &_tianqiqing_i_xiaoyuzhuanqing_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_12, 50, 50);
	lv_img_set_angle(ui->src_home_img_12, 0);

	//Write codes src_home_img_8
	ui->src_home_img_8 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_8, 70, 110);
	lv_obj_set_size(ui->src_home_img_8, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_8, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_8. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_8, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_8, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_8
	lv_obj_add_flag(ui->src_home_img_8, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_8, &_tianqiqing_i_dayu_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_8, 50, 50);
	lv_img_set_angle(ui->src_home_img_8, 0);

	//Write codes src_home_img_14
	ui->src_home_img_14 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_14, 70, 110);
	lv_obj_set_size(ui->src_home_img_14, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_14, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_14. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_14, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_14, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_14
	lv_obj_add_flag(ui->src_home_img_14, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_14, &_tianqiqing_i_zhongyu_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_14, 50, 50);
	lv_img_set_angle(ui->src_home_img_14, 0);

	//Write codes src_home_img_11
	ui->src_home_img_11 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_11, 70, 110);
	lv_obj_set_size(ui->src_home_img_11, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_11, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_11. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_11, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_11, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_11
	lv_obj_add_flag(ui->src_home_img_11, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_11, &_tianqiqing_i_xiaoyu_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_11, 50, 50);
	lv_img_set_angle(ui->src_home_img_11, 0);

	//Write codes src_home_img_10
	ui->src_home_img_10 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_10, 70, 110);
	lv_obj_set_size(ui->src_home_img_10, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_10, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_10. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_10, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_10, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_10
	lv_obj_add_flag(ui->src_home_img_10, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_10, &_tianqiqing_i_leiyu_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_10, 50, 50);
	lv_img_set_angle(ui->src_home_img_10, 0);

	//Write codes src_home_img_9
	ui->src_home_img_9 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_9, 70, 110);
	lv_obj_set_size(ui->src_home_img_9, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_9, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_9. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_9, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_9, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_9
	lv_obj_add_flag(ui->src_home_img_9, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_9, &_tianqiqing_i_duoyun_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_9, 50, 50);
	lv_img_set_angle(ui->src_home_img_9, 0);

	//Write codes src_home_img_20
	ui->src_home_img_20 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_20, 70, 110);
	lv_obj_set_size(ui->src_home_img_20, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_20, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_20. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_20, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_20, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_20
	lv_obj_add_flag(ui->src_home_img_20, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_20, &_tianqizhongyu_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_20, 50, 50);
	lv_img_set_angle(ui->src_home_img_20, 0);

	//Write codes src_home_img_19
	ui->src_home_img_19 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_19, 70, 110);
	lv_obj_set_size(ui->src_home_img_19, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_19, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_19. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_19, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_19, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_19
	lv_obj_add_flag(ui->src_home_img_19, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_19, &_tianqiyeleiyu_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_19, 50, 50);
	lv_img_set_angle(ui->src_home_img_19, 0);

	//Write codes src_home_img_18
	ui->src_home_img_18 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_18, 70, 110);
	lv_obj_set_size(ui->src_home_img_18, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_18, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_18. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_18, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_18, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_18
	lv_obj_add_flag(ui->src_home_img_18, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_18, &_tianqiye_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_18, 50, 50);
	lv_img_set_angle(ui->src_home_img_18, 0);

	//Write codes src_home_img_15
	ui->src_home_img_15 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_15, 70, 110);
	lv_obj_set_size(ui->src_home_img_15, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_15, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_15. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_15, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_15, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_15
	lv_obj_add_flag(ui->src_home_img_15, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_15, &_tianqiwumai_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_15, 50, 50);
	lv_img_set_angle(ui->src_home_img_15, 0);

	//Write codes src_home_img_16
	ui->src_home_img_16 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_16, 70, 110);
	lv_obj_set_size(ui->src_home_img_16, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_16, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_16. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_16, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_16, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_16
	lv_obj_add_flag(ui->src_home_img_16, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_16, &_tianqiwumaiyintian_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_16, 50, 50);
	lv_img_set_angle(ui->src_home_img_16, 0);

	//Write codes src_home_img_17
	ui->src_home_img_17 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_17, 70, 110);
	lv_obj_set_size(ui->src_home_img_17, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_17, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_17. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_17, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_17, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_17
	lv_obj_add_flag(ui->src_home_img_17, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_17, &_tianqixiaoyu_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_17, 50, 50);
	lv_img_set_angle(ui->src_home_img_17, 0);

	//Write codes src_home_img_21
	ui->src_home_img_21 = lv_img_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_img_21, 70, 110);
	lv_obj_set_size(ui->src_home_img_21, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_21, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_21. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_21, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_21, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_21, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget src_home_img_21
	lv_obj_add_flag(ui->src_home_img_21, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->src_home_img_21, &_BLE_ok_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_21, 50, 50);
	lv_img_set_angle(ui->src_home_img_21, 0);

	//Write codes src_home_label_5
	ui->src_home_label_5 = lv_label_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_label_5, 70, 144);
	lv_obj_set_size(ui->src_home_label_5, 100, 32);
	lv_obj_set_scrollbar_mode(ui->src_home_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_5, "北京市天津上海重庆阿坝藏族羌族自治州克苏地区拉善盟勒泰地里安康庆顺阳鞍山彦淖尔音郭楞蒙古自治州巴中城山白银百色蚌埠包头宝鸡定保山北海本溪毕节地滨博尔塔拉蒙古沧昌都地昌吉回族长春长沙长治常德常州巢湖朝阳潮州郴成都承德池赤峰崇左滁楚雄彝族达大理白连同兴安岭丹东德宏傣族景颇族德阳德迪庆藏族定西东莞东营鄂尔多斯鄂恩施土家族苗族防城港佛山福州抚顺抚州阜新阳甘南甘孜藏族赣州固原广安广元广州贵港贵阳桂林果洛藏族哈尔滨哈密地区海北藏族东海口南藏族西蒙古族藏邯郸汉中杭州毫合肥和田河池河源菏泽贺州鹤壁鹤岗黑河衡水衡阳红河哈尼族彝族呼和浩特呼伦贝尔湖州葫芦岛怀化淮安淮北淮南黄冈黄南藏族黄山黄石惠州鸡西吉安吉林济南济宁佳木斯嘉兴\n嘉峪关江门焦作揭阳金昌金华锦州晋城晋中荆门荆州景德镇九江酒泉喀什开封克拉玛依克孜勒苏柯尔克孜昆明拉萨来宾莱芜兰州廊坊乐山丽江丽水连云港凉山彝族辽阳辽源聊城林芝临沧临汾临夏\n临沂柳州六安六盘水龙岩陇南娄底泸州吕梁洛阳漯河马鞍茂名眉山梅州绵阳牡丹江内江那曲南昌南充南京南宁南平南通南阳宁波宁德怒江傈僳族攀枝花盘锦\n平顶山平凉萍乡莆田濮阳普洱七台河齐齐哈尔黔东南苗族侗族黔南布依族苗族\n黔西南布依族苗族钦州秦皇岛青岛清远庆阳曲靖衢州泉州日喀则地区日照三门峡三明三亚山南地区汕头汕尾商洛商丘上饶韶关邵阳绍兴深圳沈阳十堰石家庄石嘴山双鸭山朔州四平松原苏州宿迁宿州绥化随州遂宁塔城地区台州太原泰安泰州唐山天水铁岭通化通辽铜川铜陵铜仁吐鲁番威海潍坊渭南温州文山壮族苗族乌海乌兰察布乌鲁木齐无锡吴忠芜湖梧州武汉武威西安西宁西双版纳傣族锡林郭勒盟厦门咸宁咸阳湘潭湘西土家族苗族襄樊孝感忻州新乡新余信阳兴安盟邢台徐州许昌宣城雅安烟台延安延边朝鲜族盐城扬州阳江阳泉伊春伊犁哈萨克宜宾宜昌宜春益阳银川鹰潭营口永州榆林玉林玉树藏族溪岳阳云浮运城枣庄湛江张家界张家口张掖漳州昭通肇庆镇江\n郑州中山中卫舟山周口株洲珠海驻马店资阳淄博自贡遵义二三四五六七");
	lv_label_set_long_mode(ui->src_home_label_5, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_5, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_label_5
	lv_obj_add_flag(ui->src_home_label_5, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_label_6
	ui->src_home_label_6 = lv_label_create(ui->src_home_cont_img);
	lv_obj_set_pos(ui->src_home_label_6, 70, 144);
	lv_obj_set_size(ui->src_home_label_6, 100, 32);
	lv_obj_set_scrollbar_mode(ui->src_home_label_6, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_6, "多云雷雨雪晴天大中小转阵暴舞霾冰雹特");
	lv_label_set_long_mode(ui->src_home_label_6, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_6, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_label_6
	lv_obj_add_flag(ui->src_home_label_6, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_tabview_1
	ui->src_home_tabview_1 = lv_tabview_create(ui->src_home, LV_DIR_BOTTOM, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_tabview_1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_pos(ui->src_home_tabview_1, 0, 0);
	lv_obj_set_size(ui->src_home_tabview_1, 240, 320);

	//Home
	ui->src_home_tabview_1_Home = lv_tabview_add_tab(ui->src_home_tabview_1, "Home");

	//Shortcuts
	ui->src_home_tabview_1_Shortcuts = lv_tabview_add_tab(ui->src_home_tabview_1, "Shortcuts");

	//SmartCtrl
	ui->src_home_tabview_1_SmartCtrl = lv_tabview_add_tab(ui->src_home_tabview_1, "SmartCtrl");


	//Set style for src_home_tabview_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tabview_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_tabview_1, lv_color_make(0xea, 0xef, 0xf3), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_tabview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_tabview_1, lv_color_make(0x24, 0x24, 0x24), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_tabview_1, lv_color_make(0xc0, 0xc0, 0xc0), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_tabview_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_tabview_1, lv_color_make(0x4d, 0x4d, 0x4d), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_tabview_1, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_src_home_tabview_1_extra_btnm_main_default
	static lv_style_t style_src_home_tabview_1_extra_btnm_main_default;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_main_default);
	lv_style_set_radius(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_bg_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0x0f, 0x0f, 0x0f));
	lv_style_set_bg_grad_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_src_home_tabview_1_extra_btnm_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_border_color(&style_src_home_tabview_1_extra_btnm_main_default, lv_color_make(0x11, 0x11, 0x11));
	lv_style_set_border_width(&style_src_home_tabview_1_extra_btnm_main_default, 0);
	lv_style_set_border_opa(&style_src_home_tabview_1_extra_btnm_main_default, 80);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_src_home_tabview_1_extra_btnm_items_default
	static lv_style_t style_src_home_tabview_1_extra_btnm_items_default;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_items_default);
	lv_style_set_text_color(&style_src_home_tabview_1_extra_btnm_items_default, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_font(&style_src_home_tabview_1_extra_btnm_items_default, &lv_font_simhei_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_src_home_tabview_1_extra_btnm_items_checked
	static lv_style_t style_src_home_tabview_1_extra_btnm_items_checked;
	ui_init_style(&style_src_home_tabview_1_extra_btnm_items_checked);
	lv_style_set_radius(&style_src_home_tabview_1_extra_btnm_items_checked, 0);
	lv_style_set_bg_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_src_home_tabview_1_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_tabview_1_extra_btnm_items_checked, 60);
	lv_style_set_border_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_src_home_tabview_1_extra_btnm_items_checked, 4);
	lv_style_set_border_opa(&style_src_home_tabview_1_extra_btnm_items_checked, 255);
	lv_style_set_border_side(&style_src_home_tabview_1_extra_btnm_items_checked, LV_BORDER_SIDE_BOTTOM);
	lv_style_set_text_color(&style_src_home_tabview_1_extra_btnm_items_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_text_font(&style_src_home_tabview_1_extra_btnm_items_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_tabview_get_tab_btns(ui->src_home_tabview_1), &style_src_home_tabview_1_extra_btnm_items_checked, LV_PART_ITEMS|LV_STATE_CHECKED);

	//Write codes src_home_tileview
	ui->src_home_tileview = lv_tileview_create(ui->src_home_tabview_1_Home);
	lv_obj_set_pos(ui->src_home_tileview, 0, 0);
	lv_obj_set_size(ui->src_home_tileview, 210, 260);
	lv_obj_set_scrollbar_mode(ui->src_home_tileview, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_tileview. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tileview, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_tileview, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_tileview, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_tileview, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_tileview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_tileview. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_tileview, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_tileview, lv_color_make(0xea, 0xef, 0xf3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_tileview, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//add new tile home
	ui->home = lv_tileview_add_tile(ui->src_home_tileview, 0, 1, LV_DIR_TOP);

	//Write codes src_home_img_1
	ui->src_home_img_1 = lv_img_create(ui->home);
	lv_obj_set_pos(ui->src_home_img_1, 0, 106);
	lv_obj_set_size(ui->src_home_img_1, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_1, &_tianqiqing_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_1, 50, 50);
	lv_img_set_angle(ui->src_home_img_1, 0);

	//Write codes src_home_label_temp
	ui->src_home_label_temp = lv_label_create(ui->home);
	lv_obj_set_pos(ui->src_home_label_temp, 119, 140);
	lv_obj_set_size(ui->src_home_label_temp, 66, 25);
	lv_obj_set_scrollbar_mode(ui->src_home_label_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_temp, "25℃");
	lv_label_set_long_mode(ui->src_home_label_temp, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_temp, &lv_font_simhei_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool src_home_digital_clock_1_timer_enabled = false;

	//Write codes src_home_digital_clock_1
	ui->src_home_digital_clock_1 = lv_dclock_create(ui->home, "11:25:50");
	lv_obj_set_style_text_align(ui->src_home_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->src_home_digital_clock_1, 32, 42);
	lv_obj_set_size(ui->src_home_digital_clock_1, 143, 48);

	//create timer
	if (!src_home_digital_clock_1_timer_enabled) {
		lv_timer_create(src_home_digital_clock_1_timer, 1000, NULL);
		src_home_digital_clock_1_timer_enabled = true;
	}
	//Set style for src_home_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_digital_clock_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_digital_clock_1, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_img_wifi
	ui->src_home_img_wifi = lv_img_create(ui->home);
	lv_obj_set_pos(ui->src_home_img_wifi, 186, 12);
	lv_obj_set_size(ui->src_home_img_wifi, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_wifi, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_wifi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_wifi, &_no_internet_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_wifi, 0, 0);
	lv_img_set_angle(ui->src_home_img_wifi, 0);

	//Write codes src_home_label_dizhi
	ui->src_home_label_dizhi = lv_label_create(ui->home);
	lv_obj_set_pos(ui->src_home_label_dizhi, 16, 210);
	lv_obj_set_size(ui->src_home_label_dizhi, 66, 14);
	lv_obj_set_scrollbar_mode(ui->src_home_label_dizhi, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_dizhi, "齐齐哈尔市");
	lv_label_set_long_mode(ui->src_home_label_dizhi, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_dizhi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_dizhi, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_dizhi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_dizhi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_dizhi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_dizhi, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_dizhi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_dizhi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_waether
	ui->src_home_label_waether = lv_label_create(ui->home);
	lv_obj_set_pos(ui->src_home_label_waether, -3, 229);
	lv_obj_set_size(ui->src_home_label_waether, 105, 13);
	lv_obj_set_scrollbar_mode(ui->src_home_label_waether, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_waether, "雷阵雨转多云");
	lv_label_set_long_mode(ui->src_home_label_waether, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_waether. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_waether, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_waether, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_waether, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_waether, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_waether, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_waether, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_waether, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_cont_1
	ui->src_home_cont_1 = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->src_home_cont_1, 70, 190);
	lv_obj_set_size(ui->src_home_cont_1, 140, 50);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_img_day1
	ui->src_home_img_day1 = lv_img_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_img_day1, 23, 15);
	lv_obj_set_size(ui->src_home_img_day1, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_day1, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_day1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_day1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_day1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_day1, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_day1, 0, 0);
	lv_img_set_angle(ui->src_home_img_day1, 0);

	//Write codes src_home_label_day2
	ui->src_home_label_day2 = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_label_day2, 53, 4);
	lv_obj_set_size(ui->src_home_label_day2, 33, 9);
	lv_obj_set_scrollbar_mode(ui->src_home_label_day2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_day2, "后天");
	lv_label_set_long_mode(ui->src_home_label_day2, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_day2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_day2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_day2, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_day2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_day3
	ui->src_home_label_day3 = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_label_day3, 93, 3);
	lv_obj_set_size(ui->src_home_label_day3, 33, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_label_day3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_day3, "大后天");
	lv_label_set_long_mode(ui->src_home_label_day3, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_day3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_day3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_day3, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_day3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_day3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_day1
	ui->src_home_label_day1 = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_label_day1, 18, 3);
	lv_obj_set_size(ui->src_home_label_day1, 33, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_label_day1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_day1, "明天");
	lv_label_set_long_mode(ui->src_home_label_day1, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_day1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_day1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_day1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_day1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_day1, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_day1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_img_day2
	ui->src_home_img_day2 = lv_img_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_img_day2, 61, 15);
	lv_obj_set_size(ui->src_home_img_day2, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_day2, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_day2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_day2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_day2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_day2, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_day2, 0, 0);
	lv_img_set_angle(ui->src_home_img_day2, 0);

	//Write codes src_home_img_day3
	ui->src_home_img_day3 = lv_img_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_img_day3, 99, 15);
	lv_obj_set_size(ui->src_home_img_day3, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_day3, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_day3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_day3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_day3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_day3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_day3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_day3, &_tianqiqing_i_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_day3, 0, 0);
	lv_img_set_angle(ui->src_home_img_day3, 0);

	//Write codes src_home_day1_temp
	ui->src_home_day1_temp = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_day1_temp, 27, 37);
	lv_obj_set_size(ui->src_home_day1_temp, 20, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_day1_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_day1_temp, "25°");
	lv_label_set_long_mode(ui->src_home_day1_temp, LV_LABEL_LONG_WRAP);

	//Set style for src_home_day1_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_day1_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_day1_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_day1_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_day1_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_day1_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_day1_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_day2_temp
	ui->src_home_day2_temp = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_day2_temp, 66, 37);
	lv_obj_set_size(ui->src_home_day2_temp, 20, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_day2_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_day2_temp, "25°");
	lv_label_set_long_mode(ui->src_home_day2_temp, LV_LABEL_LONG_WRAP);

	//Set style for src_home_day2_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_day2_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_day2_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_day2_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_day2_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_day2_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_day2_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_day3_temp
	ui->src_home_day3_temp = lv_label_create(ui->src_home_cont_1);
	lv_obj_set_pos(ui->src_home_day3_temp, 104, 37);
	lv_obj_set_size(ui->src_home_day3_temp, 20, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_day3_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_day3_temp, "25°");
	lv_label_set_long_mode(ui->src_home_day3_temp, LV_LABEL_LONG_WRAP);

	//Set style for src_home_day3_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_day3_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_day3_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_day3_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_day3_temp, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_day3_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_day3_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_cont_2
	ui->src_home_cont_2 = lv_obj_create(ui->home);
	lv_obj_set_pos(ui->src_home_cont_2, 10, 76);
	lv_obj_set_size(ui->src_home_cont_2, 196, 31);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_wday
	ui->src_home_label_wday = lv_label_create(ui->src_home_cont_2);
	lv_obj_set_pos(ui->src_home_label_wday, 126, 5);
	lv_obj_set_size(ui->src_home_label_wday, 46, 16);
	lv_obj_set_scrollbar_mode(ui->src_home_label_wday, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_wday, "星期一");
	lv_label_set_long_mode(ui->src_home_label_wday, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_wday. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_wday, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_wday, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_wday, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_wday, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_wday, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_wday, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_date
	ui->src_home_label_date = lv_label_create(ui->src_home_cont_2);
	lv_obj_set_pos(ui->src_home_label_date, 26, 5);
	lv_obj_set_size(ui->src_home_label_date, 80, 10);
	lv_obj_set_scrollbar_mode(ui->src_home_label_date, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_date, "1970年7月10日");
	lv_label_set_long_mode(ui->src_home_label_date, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_date. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_date, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_date, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_date, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_date, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_date, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//add new tile config
	ui->config = lv_tileview_add_tile(ui->src_home_tileview, 0, 0, LV_DIR_BOTTOM);

	//Write codes src_home_ta_1
	ui->src_home_ta_1 = lv_textarea_create(ui->config);
	lv_obj_set_pos(ui->src_home_ta_1, 42, 100);
	lv_obj_set_size(ui->src_home_ta_1, 130, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_1, lv_color_make(0x58, 0x58, 0x58), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_1, 249, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_1. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_1, "ASD");

	//use keyboard on src_home_ta_1
	lv_obj_add_event_cb(ui->src_home_ta_1, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_label_ip
	ui->src_home_label_ip = lv_label_create(ui->config);
	lv_obj_set_pos(ui->src_home_label_ip, 44, 2);
	lv_obj_set_size(ui->src_home_label_ip, 127, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_label_ip, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_ip, "IP:00.00.00.00");
	lv_label_set_long_mode(ui->src_home_label_ip, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_ip. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_ip, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_ip, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_ip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_ip, lv_color_make(0xf9, 0xf9, 0xf9), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_ip, &lv_font_arial_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_ip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_ip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_ddlist_1
	ui->src_home_ddlist_1 = lv_dropdown_create(ui->config);
	lv_obj_set_pos(ui->src_home_ddlist_1, 42, 47);
	lv_obj_set_size(ui->src_home_ddlist_1, 130, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_ddlist_1, LV_SCROLLBAR_MODE_OFF);
	lv_dropdown_set_options(ui->src_home_ddlist_1, "SSID1\nSSID2\nSSID3\nSSID4\nSSID5\nSSID6\nSSID7\nSSID8\nSSID9\nSSID10");

	//Set style for src_home_ddlist_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ddlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ddlist_1, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ddlist_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ddlist_1, lv_color_make(0x68, 0x68, 0x68), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ddlist_1, lv_color_make(0xfd, 0xfd, 0xfd), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ddlist_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_CHECKED for style_src_home_ddlist_1_extra_list_selected_checked
	static lv_style_t style_src_home_ddlist_1_extra_list_selected_checked;
	ui_init_style(&style_src_home_ddlist_1_extra_list_selected_checked);
	lv_style_set_radius(&style_src_home_ddlist_1_extra_list_selected_checked, 3);
	lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0x50, 0x50, 0x50));
	lv_style_set_bg_grad_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_src_home_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_selected_checked, 173);
	lv_style_set_border_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0xe1, 0xe6, 0xee));
	lv_style_set_border_width(&style_src_home_ddlist_1_extra_list_selected_checked, 1);
	lv_style_set_border_opa(&style_src_home_ddlist_1_extra_list_selected_checked, 0);
	lv_style_set_text_color(&style_src_home_ddlist_1_extra_list_selected_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_src_home_ddlist_1_extra_list_selected_checked, &lv_font_montserratMedium_12);
	lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Set style state: LV_STATE_DEFAULT for style_src_home_ddlist_1_extra_list_main_default
	static lv_style_t style_src_home_ddlist_1_extra_list_main_default;
	ui_init_style(&style_src_home_ddlist_1_extra_list_main_default);
	lv_style_set_radius(&style_src_home_ddlist_1_extra_list_main_default, 3);
	lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_src_home_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_main_default, 82);
	lv_style_set_border_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_src_home_ddlist_1_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_src_home_ddlist_1_extra_list_main_default, 0);
	lv_style_set_text_color(&style_src_home_ddlist_1_extra_list_main_default, lv_color_make(0xbb, 0xbb, 0xbb));
	lv_style_set_text_font(&style_src_home_ddlist_1_extra_list_main_default, &lv_font_montserratMedium_12);
	lv_style_set_max_height(&style_src_home_ddlist_1_extra_list_main_default, 150);
	lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style state: LV_STATE_DEFAULT for style_src_home_ddlist_1_extra_list_scrollbar_default
	static lv_style_t style_src_home_ddlist_1_extra_list_scrollbar_default;
	ui_init_style(&style_src_home_ddlist_1_extra_list_scrollbar_default);
	lv_style_set_radius(&style_src_home_ddlist_1_extra_list_scrollbar_default, 3);
	lv_style_set_bg_color(&style_src_home_ddlist_1_extra_list_scrollbar_default, lv_color_make(0xb0, 0xb0, 0xb0));
	lv_style_set_bg_opa(&style_src_home_ddlist_1_extra_list_scrollbar_default, 221);
	lv_obj_add_style(lv_dropdown_get_list(ui->src_home_ddlist_1), &style_src_home_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes src_home_label_ssid
	ui->src_home_label_ssid = lv_label_create(ui->config);
	lv_obj_set_pos(ui->src_home_label_ssid, -2, 55);
	lv_obj_set_size(ui->src_home_label_ssid, 48, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_ssid, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_ssid, "SSID:");
	lv_label_set_long_mode(ui->src_home_label_ssid, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_ssid. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_ssid, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_ssid, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_ssid, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_ssid, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_ssid, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_ssid, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_pass
	ui->src_home_label_pass = lv_label_create(ui->config);
	lv_obj_set_pos(ui->src_home_label_pass, -2, 108);
	lv_obj_set_size(ui->src_home_label_pass, 45, 14);
	lv_obj_set_scrollbar_mode(ui->src_home_label_pass, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_pass, "PASS:");
	lv_label_set_long_mode(ui->src_home_label_pass, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_pass, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_pass, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_btn_connect
	ui->src_home_btn_connect = lv_btn_create(ui->config);
	lv_obj_set_pos(ui->src_home_btn_connect, 59, 155);
	lv_obj_set_size(ui->src_home_btn_connect, 87, 39);
	lv_obj_set_scrollbar_mode(ui->src_home_btn_connect, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_btn_connect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_btn_connect, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_btn_connect, lv_color_make(0x08, 0x24, 0x39), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_btn_connect, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_btn_connect, 220, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_btn_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_btn_connect, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_btn_connect, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_btn_connect, &lv_font_simhei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_btn_connect, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->src_home_btn_connect_label = lv_label_create(ui->src_home_btn_connect);
	lv_label_set_text(ui->src_home_btn_connect_label, "连接");
	lv_obj_set_style_pad_all(ui->src_home_btn_connect, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->src_home_btn_connect_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes src_home_label_BLEConter
	ui->src_home_label_BLEConter = lv_label_create(ui->src_home_tabview_1_Shortcuts);
	lv_obj_set_pos(ui->src_home_label_BLEConter, 63, 10);
	lv_obj_set_size(ui->src_home_label_BLEConter, 111, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_BLEConter, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_BLEConter, "BLE:Connecting");
	lv_label_set_long_mode(ui->src_home_label_BLEConter, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_BLEConter. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_BLEConter, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_BLEConter, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_BLEConter, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_BLEConter, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_BLEConter, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_BLEConter, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_BLEConter, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_BLEConter, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_img_BLE
	ui->src_home_img_BLE = lv_img_create(ui->src_home_tabview_1_Shortcuts);
	lv_obj_set_pos(ui->src_home_img_BLE, 166, 7);
	lv_obj_set_size(ui->src_home_img_BLE, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_img_BLE, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_BLE. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_BLE, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_BLE, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_BLE, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_BLE, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_BLE, &_BLE_no_alpha_20x20);
	lv_img_set_pivot(ui->src_home_img_BLE, 50, 50);
	lv_img_set_angle(ui->src_home_img_BLE, 0);

	//Write codes src_home_cont_BLE_TEXT
	ui->src_home_cont_BLE_TEXT = lv_obj_create(ui->src_home_tabview_1_Shortcuts);
	lv_obj_set_pos(ui->src_home_cont_BLE_TEXT, 31, 32);
	lv_obj_set_size(ui->src_home_cont_BLE_TEXT, 183, 128);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_BLE_TEXT, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_BLE_TEXT. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_BLE_TEXT, lv_color_make(0x66, 0x66, 0x66), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_BLE_TEXT, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_BLE_TEXT, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_BLE_TEXT, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_BLE_TEXT, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_BLE_TEXT, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_BLE_TEXT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_4
	ui->src_home_label_4 = lv_label_create(ui->src_home_cont_BLE_TEXT);
	lv_obj_set_pos(ui->src_home_label_4, 24, 88);
	lv_obj_set_size(ui->src_home_label_4, 127, 16);
	lv_obj_set_scrollbar_mode(ui->src_home_label_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_4, "“AiPi-Dashboard”");
	lv_label_set_long_mode(ui->src_home_label_4, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_4, lv_color_make(0xf5, 0x54, 0x54), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_4, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_4, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_2
	ui->src_home_label_2 = lv_label_create(ui->src_home_cont_BLE_TEXT);
	lv_obj_set_pos(ui->src_home_label_2, 5, 73);
	lv_obj_set_size(ui->src_home_label_2, 189, 13);
	lv_obj_set_scrollbar_mode(ui->src_home_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_2, "请使用电脑连接以下蓝牙设备：");
	lv_label_set_long_mode(ui->src_home_label_2, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_2, lv_color_make(0x21, 0x95, 0xF6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_2, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_teas
	ui->src_home_label_teas = lv_label_create(ui->src_home_cont_BLE_TEXT);
	lv_obj_set_pos(ui->src_home_label_teas, 6, 55);
	lv_obj_set_size(ui->src_home_label_teas, 80, 14);
	lv_obj_set_scrollbar_mode(ui->src_home_label_teas, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_teas, "蓝牙未连接：\n");
	lv_label_set_long_mode(ui->src_home_label_teas, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_teas. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_teas, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_teas, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_teas, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_teas, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_teas, lv_color_make(0xd8, 0xcf, 0x74), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_teas, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_teas, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_teas, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_cont_dis
	ui->src_home_cont_dis = lv_obj_create(ui->src_home_tabview_1_Shortcuts);
	lv_obj_set_pos(ui->src_home_cont_dis, 0, 0);
	lv_obj_set_size(ui->src_home_cont_dis, 210, 260);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_dis, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_dis. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_dis, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_dis, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_dis, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_dis, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_dis, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_dis, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_dis, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_cont_dis
	lv_obj_add_flag(ui->src_home_cont_dis, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_imgbtn_1
	ui->src_home_imgbtn_1 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_1, 30, 50);
	lv_obj_set_size(ui->src_home_imgbtn_1, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_1, lv_color_make(0x77, 0x77, 0x77), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_1. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_1, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_1, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, &_VSopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_1, LV_IMGBTN_STATE_PRESSED, NULL, &_VSclose_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_VSopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_VSclose_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_2
	ui->src_home_imgbtn_2 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_2, 97, 50);
	lv_obj_set_size(ui->src_home_imgbtn_2, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_2. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_2. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_2, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_2, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, &_baiduopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_2, LV_IMGBTN_STATE_PRESSED, NULL, &_baiduclose_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_baiduopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_baiduclose_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_3
	ui->src_home_imgbtn_3 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_3, 160, 50);
	lv_obj_set_size(ui->src_home_imgbtn_3, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_3. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_3. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_3, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_3, LV_IMGBTN_STATE_RELEASED, NULL, &_gooleopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_3, LV_IMGBTN_STATE_PRESSED, NULL, &_gooleclose_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_gooleopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_gooleclose_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_3, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_4
	ui->src_home_imgbtn_4 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_4, 30, 140);
	lv_obj_set_size(ui->src_home_imgbtn_4, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_4, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_4, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_4. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_4, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_4, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_4. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_4, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_4, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_4, LV_IMGBTN_STATE_RELEASED, NULL, &_windopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_4, LV_IMGBTN_STATE_PRESSED, NULL, &_dindclose_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_4, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_windopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_4, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_dindclose_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_4, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_5
	ui->src_home_imgbtn_5 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_5, 160, 140);
	lv_obj_set_size(ui->src_home_imgbtn_5, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_5, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_5, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_5. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_5, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_5, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_5. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_5, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_5, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_5, LV_IMGBTN_STATE_RELEASED, NULL, &_jlcopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_5, LV_IMGBTN_STATE_PRESSED, NULL, &_jlcclose_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_5, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_jlcopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_5, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_jlcclose_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_5, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_6
	ui->src_home_imgbtn_6 = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_6, 97, 140);
	lv_obj_set_size(ui->src_home_imgbtn_6, 45, 45);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_6, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_6, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_6. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_6, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_6, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_6. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_6, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_6, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_6, LV_IMGBTN_STATE_RELEASED, NULL, &_OBSopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_6, LV_IMGBTN_STATE_PRESSED, NULL, &_OBScloes_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_6, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_OBSopen_alpha_45x45, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_6, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_OBScloes_alpha_45x45, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_6, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_label_7
	ui->src_home_label_7 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_7, 27, 100);
	lv_obj_set_size(ui->src_home_label_7, 50, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_7, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_7, "Vscode");
	lv_label_set_long_mode(ui->src_home_label_7, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_7. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_7, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_7, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_8
	ui->src_home_label_8 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_8, 95, 100);
	lv_obj_set_size(ui->src_home_label_8, 50, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_8, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_8, "百度网盘");
	lv_label_set_long_mode(ui->src_home_label_8, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_8. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_8, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_8, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_9
	ui->src_home_label_9 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_9, 154, 100);
	lv_obj_set_size(ui->src_home_label_9, 50, 27);
	lv_obj_set_scrollbar_mode(ui->src_home_label_9, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_9, "Google\nChrome");
	lv_label_set_long_mode(ui->src_home_label_9, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_9. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_9, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_9, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_9, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_9, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_10
	ui->src_home_label_10 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_10, 19, 190);
	lv_obj_set_size(ui->src_home_label_10, 68, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_10, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_10, "WindTerm");
	lv_label_set_long_mode(ui->src_home_label_10, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_10. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_10, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_11
	ui->src_home_label_11 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_11, 92, 190);
	lv_obj_set_size(ui->src_home_label_11, 55, 29);
	lv_obj_set_scrollbar_mode(ui->src_home_label_11, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_11, "OBS Studio");
	lv_label_set_long_mode(ui->src_home_label_11, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_11. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_11, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_11, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_11, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_11, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_11, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_12
	ui->src_home_label_12 = lv_label_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_label_12, 154, 190);
	lv_obj_set_size(ui->src_home_label_12, 57, 29);
	lv_obj_set_scrollbar_mode(ui->src_home_label_12, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_12, "立创EDA专业版");
	lv_label_set_long_mode(ui->src_home_label_12, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_12. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_12, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_12, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_12, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_12, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_12, lv_color_make(0x86, 0x86, 0x86), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_12, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_cont_music_ctrl
	ui->src_home_cont_music_ctrl = lv_obj_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_cont_music_ctrl, 15, 217);
	lv_obj_set_size(ui->src_home_cont_music_ctrl, 177, 39);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_music_ctrl, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_music_ctrl. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_music_ctrl, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_music_ctrl, lv_color_make(0x8a, 0x8a, 0x8a), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_music_ctrl, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_music_ctrl, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_music_ctrl, 22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_music_ctrl, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_music_ctrl, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_music_ctrl, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_music_ctrl, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_imgbtn_stop
	ui->src_home_imgbtn_stop = lv_imgbtn_create(ui->src_home_cont_music_ctrl);
	lv_obj_set_pos(ui->src_home_imgbtn_stop, 71, 2);
	lv_obj_set_size(ui->src_home_imgbtn_stop, 30, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_stop, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_stop. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_stop, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_stop, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_stop, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_stop, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_stop. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_stop, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_stop, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_stop, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_stop. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_stop, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_stop, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_stop, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_stop, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_stop, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget src_home_imgbtn_stop
	lv_obj_add_flag(ui->src_home_imgbtn_stop, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->src_home_imgbtn_stop, LV_IMGBTN_STATE_RELEASED, NULL, &_staopopen_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_stop, LV_IMGBTN_STATE_PRESSED, NULL, &_staopclose_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_stop, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_staopopen_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_stop, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_staopclose_alpha_30x30, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_stop, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_7
	ui->src_home_imgbtn_7 = lv_imgbtn_create(ui->src_home_cont_music_ctrl);
	lv_obj_set_pos(ui->src_home_imgbtn_7, 148, 9);
	lv_obj_set_size(ui->src_home_imgbtn_7, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_7, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_7. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_7, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_7, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_7. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_7, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_7, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_7. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_7, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_7, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_7, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_7, LV_IMGBTN_STATE_RELEASED, NULL, &_nextopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_7, LV_IMGBTN_STATE_PRESSED, NULL, &_nextclose_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_7, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_nextopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_7, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_nextclose_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_7, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_satrt
	ui->src_home_imgbtn_satrt = lv_imgbtn_create(ui->src_home_cont_music_ctrl);
	lv_obj_set_pos(ui->src_home_imgbtn_satrt, 70, 3);
	lv_obj_set_size(ui->src_home_imgbtn_satrt, 30, 30);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_satrt, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_satrt. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_satrt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_satrt, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_satrt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_satrt, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_satrt. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_satrt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_satrt, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_satrt, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_satrt. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_satrt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_satrt, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_satrt, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_satrt, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_satrt, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_satrt, LV_IMGBTN_STATE_RELEASED, NULL, &_bofangopen_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_satrt, LV_IMGBTN_STATE_PRESSED, NULL, &_bofangclose_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_satrt, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_bofangopen_alpha_30x30, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_satrt, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_bofangclose_alpha_30x30, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_satrt, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_8
	ui->src_home_imgbtn_8 = lv_imgbtn_create(ui->src_home_cont_music_ctrl);
	lv_obj_set_pos(ui->src_home_imgbtn_8, 5, 9);
	lv_obj_set_size(ui->src_home_imgbtn_8, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_8, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_8. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_8, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_8, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_8. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_8, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_8, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_8. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_8, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_8, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_8, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_8, LV_IMGBTN_STATE_RELEASED, NULL, &_next0open_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_8, LV_IMGBTN_STATE_PRESSED, NULL, &_next0cloes_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_8, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_next0open_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_8, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_next0cloes_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_8, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_slider_voicse
	ui->src_home_slider_voicse = lv_slider_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_slider_voicse, 5, 60);
	lv_obj_set_size(ui->src_home_slider_voicse, 6, 150);
	lv_obj_set_scrollbar_mode(ui->src_home_slider_voicse, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_slider_voicse. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_slider_voicse, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_slider_voicse, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_slider_voicse, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_opa(ui->src_home_slider_voicse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_slider_voicse. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_slider_voicse, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_slider_voicse, lv_color_make(0x8b, 0x8b, 0x8b), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_slider_voicse, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_slider_voicse, 173, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Set style for src_home_slider_voicse. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_slider_voicse, 50, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_slider_voicse, lv_color_make(0x8d, 0x8d, 0x8d), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_slider_voicse, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_slider_voicse, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_slider_voicse, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_slider_set_range(ui->src_home_slider_voicse, 0, 100);
	lv_slider_set_value(ui->src_home_slider_voicse, 60, false);

	//Write codes src_home_imgbtn_voice
	ui->src_home_imgbtn_voice = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_voice, 0, 34);
	lv_obj_set_size(ui->src_home_imgbtn_voice, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_voice, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_voice. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_voice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_voice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_voice, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_voice, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_voice. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_voice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_voice, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_voice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_voice. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_voice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_voice, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_voice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_voice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_voice, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_voice, LV_IMGBTN_STATE_RELEASED, NULL, &_voice_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_voice, LV_IMGBTN_STATE_PRESSED, NULL, &_voiceclose_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_voice, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_voice_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_voice, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_voiceclose_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_voice, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_novoice
	ui->src_home_imgbtn_novoice = lv_imgbtn_create(ui->src_home_cont_dis);
	lv_obj_set_pos(ui->src_home_imgbtn_novoice, 0, 34);
	lv_obj_set_size(ui->src_home_imgbtn_novoice, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_novoice, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_novoice. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_novoice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_novoice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_novoice, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_novoice, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_novoice. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_novoice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_novoice, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_novoice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_novoice. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_novoice, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_novoice, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_novoice, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_novoice, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_novoice, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget src_home_imgbtn_novoice
	lv_obj_add_flag(ui->src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->src_home_imgbtn_novoice, LV_IMGBTN_STATE_RELEASED, NULL, &_novoiceopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_novoice, LV_IMGBTN_STATE_PRESSED, NULL, &_novoiceopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_novoice, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_novoiceopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_novoice, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_novoiceclose_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_novoice, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_openL
	ui->src_home_imgbtn_openL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_imgbtn_openL, 74, 103);
	lv_obj_set_size(ui->src_home_imgbtn_openL, 80, 110);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_openL, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_openL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_CHECKED);

	//Hidden for widget src_home_imgbtn_openL
	lv_obj_add_flag(ui->src_home_imgbtn_openL, LV_OBJ_FLAG_HIDDEN);

	lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_RELEASED, NULL, &_diaodengopen_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_PRESSED, NULL, &_diaodengopen_s_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_diaodengopen_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_diaodengopen_s_alpha_80x110, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_openL, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_imgbtn_confiL
	ui->src_home_imgbtn_confiL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_imgbtn_confiL, 185, 34);
	lv_obj_set_size(ui->src_home_imgbtn_confiL, 25, 25);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_confiL, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_confiL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 163, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_RELEASED, NULL, &_configL_alpha_25x25, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_PRESSED, NULL, &_configset_alpha_25x25, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_configL_alpha_25x25, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_configset_alpha_25x25, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_confiL, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_label_18
	ui->src_home_label_18 = lv_label_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_label_18, 178, 54);
	lv_obj_set_size(ui->src_home_label_18, 27, 22);
	lv_obj_set_scrollbar_mode(ui->src_home_label_18, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_18, "点击编辑");
	lv_label_set_long_mode(ui->src_home_label_18, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_18. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_18, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_18, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_imgbtn_closeL
	ui->src_home_imgbtn_closeL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_imgbtn_closeL, 74, 103);
	lv_obj_set_size(ui->src_home_imgbtn_closeL, 80, 110);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_closeL, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_closeL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_RELEASED, NULL, &_diaodeng_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_PRESSED, NULL, &_diaodeng_s_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_diaodeng_alpha_80x110, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_diaodeng_s_alpha_80x110, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_closeL, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_label_14
	ui->src_home_label_14 = lv_label_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_label_14, 25, 10);
	lv_obj_set_size(ui->src_home_label_14, 156, 14);
	lv_obj_set_scrollbar_mode(ui->src_home_label_14, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_14, "wx.ai-thinker.com:1833");
	lv_label_set_long_mode(ui->src_home_label_14, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_14. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_14, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_imgbtn_user
	ui->src_home_imgbtn_user = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_imgbtn_user, 191, 6);
	lv_obj_set_size(ui->src_home_imgbtn_user, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_user, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_user, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_RELEASED, NULL, &_useropen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_PRESSED, NULL, &_userclose_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_useropen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_userclose_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_user, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_cont_3
	ui->src_home_cont_3 = lv_obj_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_cont_3, 8, 5);
	lv_obj_set_size(ui->src_home_cont_3, 205, 216);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_3, lv_color_make(0x47, 0x47, 0x47), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_3, 87, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_cont_3
	lv_obj_add_flag(ui->src_home_cont_3, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_cont_4
	ui->src_home_cont_4 = lv_obj_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_cont_4, 0, 0);
	lv_obj_set_size(ui->src_home_cont_4, 205, 24);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_4, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_4, lv_color_make(0x66, 0x66, 0x66), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_4, 44, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_imgbtn_10
	ui->src_home_imgbtn_10 = lv_imgbtn_create(ui->src_home_cont_4);
	lv_obj_set_pos(ui->src_home_imgbtn_10, 172, 0);
	lv_obj_set_size(ui->src_home_imgbtn_10, 20, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_10, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_imgbtn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);

	//Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
	lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_RELEASED, NULL, &_closeopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_PRESSED, NULL, &_closeopen1_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_closeopen_alpha_20x20, NULL);
	lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_closeopen1_alpha_20x20, NULL);
	lv_obj_add_flag(ui->src_home_imgbtn_10, LV_OBJ_FLAG_CHECKABLE);

	//Write codes src_home_label_13
	ui->src_home_label_13 = lv_label_create(ui->src_home_cont_4);
	lv_obj_set_pos(ui->src_home_label_13, 3, 3);
	lv_obj_set_size(ui->src_home_label_13, 43, 11);
	lv_obj_set_scrollbar_mode(ui->src_home_label_13, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_13, "MQTT");
	lv_label_set_long_mode(ui->src_home_label_13, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_13. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_13, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_13, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_host
	ui->src_home_label_host = lv_label_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_label_host, 6, 38);
	lv_obj_set_size(ui->src_home_label_host, 29, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_label_host, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_host, "Host:");
	lv_label_set_long_mode(ui->src_home_label_host, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_host. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_host, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_host, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_host, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_username
	ui->src_home_label_username = lv_label_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_label_username, 3, 82);
	lv_obj_set_size(ui->src_home_label_username, 63, 13);
	lv_obj_set_scrollbar_mode(ui->src_home_label_username, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_username, "UserName:");
	lv_label_set_long_mode(ui->src_home_label_username, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_username. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_username, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_username, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_username, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_port
	ui->src_home_label_port = lv_label_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_label_port, 129, 40);
	lv_obj_set_size(ui->src_home_label_port, 29, 12);
	lv_obj_set_scrollbar_mode(ui->src_home_label_port, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_port, "Port:");
	lv_label_set_long_mode(ui->src_home_label_port, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_port. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_port, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_port, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_port, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_passworld
	ui->src_home_label_passworld = lv_label_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_label_passworld, 2, 124);
	lv_obj_set_size(ui->src_home_label_passworld, 63, 13);
	lv_obj_set_scrollbar_mode(ui->src_home_label_passworld, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_passworld, "Passworld:");
	lv_label_set_long_mode(ui->src_home_label_passworld, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_passworld. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_passworld, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_passworld, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_passworld, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_ta_mqHost
	ui->src_home_ta_mqHost = lv_textarea_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_ta_mqHost, 35, 28);
	lv_obj_set_size(ui->src_home_ta_mqHost, 94, 32);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_mqHost, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_mqHost. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqHost, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqHost, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqHost, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqHost, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_mqHost, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_mqHost, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_mqHost, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_mqHost, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_mqHost, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_mqHost, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_mqHost, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_mqHost. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqHost, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqHost, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqHost, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_mqHost, "wx.ai-thinker.com");

	//use keyboard on src_home_ta_mqHost
	lv_obj_add_event_cb(ui->src_home_ta_mqHost, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_ta_mqPort
	ui->src_home_ta_mqPort = lv_textarea_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_ta_mqPort, 156, 34);
	lv_obj_set_size(ui->src_home_ta_mqPort, 33, 23);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_mqPort, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_mqPort. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqPort, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqPort, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqPort, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqPort, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_mqPort, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_mqPort, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_mqPort, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_mqPort, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_mqPort, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_mqPort, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_mqPort, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_mqPort, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_mqPort. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqPort, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqPort, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqPort, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_mqPort, "1883");

	//use keyboard on src_home_ta_mqPort
	lv_obj_add_event_cb(ui->src_home_ta_mqPort, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_ta_mqUsername
	ui->src_home_ta_mqUsername = lv_textarea_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_ta_mqUsername, 63, 71);
	lv_obj_set_size(ui->src_home_ta_mqUsername, 128, 32);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_mqUsername, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_mqUsername. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqUsername, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqUsername, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqUsername, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqUsername, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_mqUsername, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_mqUsername, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_mqUsername, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_mqUsername, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_mqUsername, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_mqUsername, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_mqUsername, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_mqUsername. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqUsername, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqUsername, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqUsername, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_mqUsername, "AiPi_Dashboard");

	//use keyboard on src_home_ta_mqUsername
	lv_obj_add_event_cb(ui->src_home_ta_mqUsername, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_ta_mqtt_pass
	ui->src_home_ta_mqtt_pass = lv_textarea_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_ta_mqtt_pass, 63, 113);
	lv_obj_set_size(ui->src_home_ta_mqtt_pass, 128, 32);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_mqtt_pass, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_mqtt_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqtt_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqtt_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqtt_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_mqtt_pass, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_mqtt_pass, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_mqtt_pass, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_mqtt_pass, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_mqtt_pass, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_mqtt_pass. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_mqtt_pass, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_mqtt_pass, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_mqtt_pass, "123456");

	//use keyboard on src_home_ta_mqtt_pass
	lv_obj_add_event_cb(ui->src_home_ta_mqtt_pass, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_btn_connect_mqtt
	ui->src_home_btn_connect_mqtt = lv_btn_create(ui->src_home_cont_3);
	lv_obj_set_pos(ui->src_home_btn_connect_mqtt, 73, 151);
	lv_obj_set_size(ui->src_home_btn_connect_mqtt, 51, 26);
	lv_obj_set_scrollbar_mode(ui->src_home_btn_connect_mqtt, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_btn_connect_mqtt. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_btn_connect_mqtt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_btn_connect_mqtt, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_btn_connect_mqtt, 90, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_btn_connect_mqtt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_btn_connect_mqtt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_btn_connect_mqtt, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_btn_connect_mqtt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->src_home_btn_connect_mqtt_label = lv_label_create(ui->src_home_btn_connect_mqtt);
	lv_label_set_text(ui->src_home_btn_connect_mqtt_label, "连接");
	lv_obj_set_style_pad_all(ui->src_home_btn_connect_mqtt, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->src_home_btn_connect_mqtt_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes src_home_cont_6
	ui->src_home_cont_6 = lv_obj_create(ui->src_home_tabview_1_SmartCtrl);
	lv_obj_set_pos(ui->src_home_cont_6, 35, 39);
	lv_obj_set_size(ui->src_home_cont_6, 152, 226);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_6, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_6, lv_color_make(0x3f, 0x3f, 0x3f), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_6, 238, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_cont_6
	lv_obj_add_flag(ui->src_home_cont_6, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_label_15
	ui->src_home_label_15 = lv_label_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_label_15, 2, 19);
	lv_obj_set_size(ui->src_home_label_15, 41, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_15, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_15, "Topic:");
	lv_label_set_long_mode(ui->src_home_label_15, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_15. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_15, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_16
	ui->src_home_label_16 = lv_label_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_label_16, 0, 123);
	lv_obj_set_size(ui->src_home_label_16, 73, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_16, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_16, "关闭时发送:");
	lv_label_set_long_mode(ui->src_home_label_16, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_16. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_16, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_16, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_ta_topic
	ui->src_home_ta_topic = lv_textarea_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_ta_topic, 0, 35);
	lv_obj_set_size(ui->src_home_ta_topic, 142, 20);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_topic, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_topic. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_topic, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_topic, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_topic, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_topic, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_topic, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_topic, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_topic, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_topic, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_topic. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_topic, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_topic, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_topic, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_topic, "dev/topic");

	//use keyboard on src_home_ta_topic
	lv_obj_add_event_cb(ui->src_home_ta_topic, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_ta_msg_close
	ui->src_home_ta_msg_close = lv_textarea_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_ta_msg_close, 1, 141);
	lv_obj_set_size(ui->src_home_ta_msg_close, 142, 38);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_msg_close, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_msg_close. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_msg_close, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_msg_close, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_msg_close, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_msg_close, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_msg_close, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_msg_close, 118, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_msg_close, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_msg_close, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_msg_close. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_msg_close, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_msg_close, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_msg_close, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_msg_close, "{status:0}");

	//use keyboard on src_home_ta_msg_close
	lv_obj_add_event_cb(ui->src_home_ta_msg_close, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_btn_topicOK
	ui->src_home_btn_topicOK = lv_btn_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_btn_topicOK, 40, 194);
	lv_obj_set_size(ui->src_home_btn_topicOK, 60, 24);
	lv_obj_set_scrollbar_mode(ui->src_home_btn_topicOK, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_btn_topicOK. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_btn_topicOK, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_btn_topicOK, lv_color_make(0x12, 0x5e, 0x95), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_btn_topicOK, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_btn_topicOK, 138, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_btn_topicOK, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_btn_topicOK, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->src_home_btn_topicOK_label = lv_label_create(ui->src_home_btn_topicOK);
	lv_label_set_text(ui->src_home_btn_topicOK_label, "确定");
	lv_obj_set_style_pad_all(ui->src_home_btn_topicOK, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->src_home_btn_topicOK_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes src_home_label_17
	ui->src_home_label_17 = lv_label_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_label_17, 23, 3);
	lv_obj_set_size(ui->src_home_label_17, 100, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_17, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_17, "智能灯控制配置");
	lv_label_set_long_mode(ui->src_home_label_17, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_17. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_17, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_17, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_label_19
	ui->src_home_label_19 = lv_label_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_label_19, 1, 62);
	lv_obj_set_size(ui->src_home_label_19, 70, 15);
	lv_obj_set_scrollbar_mode(ui->src_home_label_19, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->src_home_label_19, "打开时发送:");
	lv_label_set_long_mode(ui->src_home_label_19, LV_LABEL_LONG_WRAP);

	//Set style for src_home_label_19. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_label_19, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_label_19, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes src_home_ta_msg_open
	ui->src_home_ta_msg_open = lv_textarea_create(ui->src_home_cont_6);
	lv_obj_set_pos(ui->src_home_ta_msg_open, 1, 81);
	lv_obj_set_size(ui->src_home_ta_msg_open, 142, 38);
	lv_obj_set_scrollbar_mode(ui->src_home_ta_msg_open, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_ta_msg_open. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_msg_open, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_ta_msg_open, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_ta_msg_open, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_ta_msg_open, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_ta_msg_open, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_ta_msg_open, 118, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->src_home_ta_msg_open, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->src_home_ta_msg_open, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for src_home_ta_msg_open. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_ta_msg_open, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_ta_msg_open, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_ta_msg_open, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_textarea_set_text(ui->src_home_ta_msg_open, "{status:1}");

	//use keyboard on src_home_ta_msg_open
	lv_obj_add_event_cb(ui->src_home_ta_msg_open, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

	//Write codes src_home_cont_5
	ui->src_home_cont_5 = lv_obj_create(ui->src_home);
	lv_obj_set_pos(ui->src_home_cont_5, 0, 0);
	lv_obj_set_size(ui->src_home_cont_5, 240, 320);
	lv_obj_set_scrollbar_mode(ui->src_home_cont_5, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_cont_5. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->src_home_cont_5, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->src_home_cont_5, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->src_home_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->src_home_cont_5, 125, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->src_home_cont_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->src_home_cont_5, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->src_home_cont_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->src_home_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Hidden for widget src_home_cont_5
	lv_obj_add_flag(ui->src_home_cont_5, LV_OBJ_FLAG_HIDDEN);


	//Write codes src_home_img_loding
	ui->src_home_img_loding = lv_img_create(ui->src_home_cont_5);
	lv_obj_set_pos(ui->src_home_img_loding, 62, 95);
	lv_obj_set_size(ui->src_home_img_loding, 100, 100);
	lv_obj_set_scrollbar_mode(ui->src_home_img_loding, LV_SCROLLBAR_MODE_OFF);

	//Set style for src_home_img_loding. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->src_home_img_loding, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->src_home_img_loding, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->src_home_img_loding, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->src_home_img_loding, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->src_home_img_loding, &_loding_alpha_100x100);
	lv_img_set_pivot(ui->src_home_img_loding, 50, 50);
	lv_img_set_angle(ui->src_home_img_loding, 0);

	//Init events for screen
	events_init_src_home(ui);
}