/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"

#include "custom.h"
#include "https_client.h"
extern TaskHandle_t https_Handle;

int Home_digital_date_yesr_value = 1970;
int Home_digital_date_mon_value = 07;
int Home_digital_date_day_value = 03;
int Home_digital_date_wday_value = 1;

int Home_digital_clock_1_hour_value = 11;
int Home_digital_clock_1_min_value = 25;
int Home_digital_clock_1_sec_value = 50;

void Home_digital_clock_1_timer(lv_timer_t* timer)
{
	int hour_type = Home_digital_clock_1_hour_value;
	clock_count_24(&Home_digital_clock_1_hour_value, &Home_digital_clock_1_min_value, &Home_digital_clock_1_sec_value);
	if (lv_obj_is_valid(guider_ui.Home_digital_clock_1))
	{
		lv_dclock_set_text_fmt(guider_ui.Home_digital_clock_1, "%02d:%02d:%02d", Home_digital_clock_1_hour_value, Home_digital_clock_1_min_value, Home_digital_clock_1_sec_value);
		if (Home_digital_clock_1_hour_value>12) {
			lv_img_set_angle(guider_ui.Home_img_hour, (Home_digital_clock_1_hour_value-12)*30*10);
		}
		else
			lv_img_set_angle(guider_ui.Home_img_hour, (Home_digital_clock_1_hour_value)*30*10);

		lv_img_set_angle(guider_ui.Home_img_min, Home_digital_clock_1_min_value*6*10);
		lv_img_set_angle(guider_ui.Home_img_sec, Home_digital_clock_1_sec_value*6*10);

		lv_label_set_text_fmt(guider_ui.Home_label_2, "%02d/", Home_digital_date_mon_value);
		lv_label_set_text_fmt(guider_ui.Home_label_3, "%02d", Home_digital_date_day_value);
		lv_label_set_text_fmt(guider_ui.Home_label_4, "%04d", Home_digital_date_yesr_value);
	}
}
#define MULDIV(a,b,c) (a*b)/c
void lv_Home_textprogress_b_set_range_value(uint32_t min, uint32_t max, uint32_t steps, uint32_t steps_min);
void lv_Home_textprogress_b_set_decimal(uint32_t decimal);
void lv_Home_textprogress_b_set_value(lv_obj_t* obj, uint32_t value);
static uint32_t Home_textprogress_b_range_max;
static uint32_t Home_textprogress_b_range_min;
static uint32_t Home_textprogress_b_initial_value;
static uint32_t Home_textprogress_b_range_steps;
static uint32_t Home_textprogress_b_range_steps_min;
static uint16_t Home_textprogress_b_decimals;
void lv_Home_textprogress_b_set_range_value(uint32_t min, uint32_t max, uint32_t steps, uint32_t steps_min)
{
	Home_textprogress_b_range_min = min;
	Home_textprogress_b_range_max = max;
	Home_textprogress_b_range_steps_min = steps_min;
	if (steps == 0)
	{
		Home_textprogress_b_range_steps = max - min;
	}
	else
	{
		Home_textprogress_b_range_steps = steps;
	}
}
void lv_Home_textprogress_b_set_decimal(uint32_t decimal)
{
	Home_textprogress_b_decimals = (((2) < (decimal)) ? (2) : (decimal));
}
uint32_t lv_Home_textprogress_b_get_progress(uint32_t range)
{
	int32_t step, prog;
	step = Home_textprogress_b_range_steps_min + MULDIV(((Home_textprogress_b_initial_value)-(Home_textprogress_b_range_min)), ((Home_textprogress_b_range_steps)-(Home_textprogress_b_range_steps_min)), ((Home_textprogress_b_range_max)-(Home_textprogress_b_range_min)));
	prog = MULDIV(step, range, Home_textprogress_b_range_steps);
	return prog;
}

void lv_Home_textprogress_b_set_value(lv_obj_t* obj, uint32_t value)
{
	value = (((value) > (Home_textprogress_b_range_min)) ? (value) : (Home_textprogress_b_range_min));
	value = (((value) < (Home_textprogress_b_range_max)) ? (value) : (Home_textprogress_b_range_max));
	Home_textprogress_b_initial_value = value;
	int range[3] = { 1, 10, 100 };
	uint32_t progress;
	char textbuffer[8];
	progress = lv_Home_textprogress_b_get_progress(100 *range[Home_textprogress_b_decimals]);
	if (Home_textprogress_b_decimals > 0)
	{
		lv_snprintf(textbuffer, sizeof(textbuffer), "%d.%0*d%%", (progress / range[Home_textprogress_b_decimals]), Home_textprogress_b_decimals, (progress % range[Home_textprogress_b_decimals]));
	}
	else {
		lv_snprintf(textbuffer, sizeof(textbuffer), "%d%%", progress);
	}
	lv_label_set_text(obj, textbuffer);
}
#define MULDIV(a,b,c) (a*b)/c
void lv_Home_textprogress_D_set_range_value(uint32_t min, uint32_t max, uint32_t steps, uint32_t steps_min);
void lv_Home_textprogress_D_set_decimal(uint32_t decimal);
void lv_Home_textprogress_D_set_value(lv_obj_t* obj, uint32_t value);
static uint32_t Home_textprogress_D_range_max;
static uint32_t Home_textprogress_D_range_min;
static uint32_t Home_textprogress_D_initial_value;
static uint32_t Home_textprogress_D_range_steps;
static uint32_t Home_textprogress_D_range_steps_min;
static uint16_t Home_textprogress_D_decimals;
void lv_Home_textprogress_D_set_range_value(uint32_t min, uint32_t max, uint32_t steps, uint32_t steps_min)
{
	Home_textprogress_D_range_min = min;
	Home_textprogress_D_range_max = max;
	Home_textprogress_D_range_steps_min = steps_min;
	if (steps == 0)
	{
		Home_textprogress_D_range_steps = max - min;
	}
	else
	{
		Home_textprogress_D_range_steps = steps;
	}
}
void lv_Home_textprogress_D_set_decimal(uint32_t decimal)
{
	Home_textprogress_D_decimals = (((2) < (decimal)) ? (2) : (decimal));
}
uint32_t lv_Home_textprogress_D_get_progress(uint32_t range)
{
	int32_t step, prog;
	step = Home_textprogress_D_range_steps_min + MULDIV(((Home_textprogress_D_initial_value)-(Home_textprogress_D_range_min)), ((Home_textprogress_D_range_steps)-(Home_textprogress_D_range_steps_min)), ((Home_textprogress_D_range_max)-(Home_textprogress_D_range_min)));
	prog = MULDIV(step, range, Home_textprogress_D_range_steps);
	return prog;
}

void lv_Home_textprogress_D_set_value(lv_obj_t* obj, uint32_t value)
{
	value = (((value) > (Home_textprogress_D_range_min)) ? (value) : (Home_textprogress_D_range_min));
	value = (((value) < (Home_textprogress_D_range_max)) ? (value) : (Home_textprogress_D_range_max));
	Home_textprogress_D_initial_value = value;
	int range[3] = { 1, 10, 100 };
	uint32_t progress;
	char textbuffer[8];
	progress = lv_Home_textprogress_D_get_progress(100 *range[Home_textprogress_D_decimals]);
	if (Home_textprogress_D_decimals > 0)
	{
		lv_snprintf(textbuffer, sizeof(textbuffer), "%d.%0*d%%", (progress / range[Home_textprogress_D_decimals]), Home_textprogress_D_decimals, (progress % range[Home_textprogress_D_decimals]));
	}
	else {
		lv_snprintf(textbuffer, sizeof(textbuffer), "%d%%", progress);
	}
	lv_label_set_text(obj, textbuffer);
}
static lv_obj_t* g_kb_Home;
static void kb_Home_event_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* kb = lv_event_get_target(e);
	if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	}
}
__attribute__((unused)) static void ta_Home_event_cb(lv_event_t* e)
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

void setup_scr_Home(lv_ui* ui) {

	//Write codes Home
	ui->Home = lv_obj_create(NULL);

	//Create keyboard on Home
	g_kb_Home = lv_keyboard_create(ui->Home);
	lv_obj_add_event_cb(g_kb_Home, kb_Home_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(g_kb_Home, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(g_kb_Home, &lv_font_simsun_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(ui->Home, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_bg
	ui->Home_img_bg = lv_img_create(ui->Home);
	lv_obj_set_pos(ui->Home_img_bg, 0, 0);
	lv_obj_set_size(ui->Home_img_bg, 240, 240);
	lv_obj_set_scrollbar_mode(ui->Home_img_bg, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_bg. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_bg, lv_color_make(0x01, 0x01, 0x01), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_bg, 31, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_bg, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_bg, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_bg, &_bg_alpha_240x240);
	lv_img_set_pivot(ui->Home_img_bg, 50, 50);
	lv_img_set_angle(ui->Home_img_bg, 0);

	//Write codes Home_label_temp
	ui->Home_label_temp = lv_label_create(ui->Home);
	lv_obj_set_pos(ui->Home_label_temp, 30, 110);
	lv_obj_set_size(ui->Home_label_temp, 46, 37);
	lv_obj_set_scrollbar_mode(ui->Home_label_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_label_temp, "25°");
	lv_label_set_long_mode(ui->Home_label_temp, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_temp. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_temp, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_temp, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_temp, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_temp, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_arc_1
	ui->Home_arc_1 = lv_arc_create(ui->Home);
	lv_obj_set_pos(ui->Home_arc_1, 5, 26);
	lv_obj_set_size(ui->Home_arc_1, 200, 200);
	lv_obj_set_scrollbar_mode(ui->Home_arc_1, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_arc_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_arc_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_arc_1, lv_color_make(0xf6, 0xf6, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_arc_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_arc_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_arc_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_arc_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_color(ui->Home_arc_1, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_width(ui->Home_arc_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_arc_1. Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT
	lv_obj_set_style_arc_color(ui->Home_arc_1, lv_color_make(0x8c, 0xc0, 0x00), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_width(ui->Home_arc_1, 4, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Set style for Home_arc_1. Part: LV_PART_KNOB, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Home_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_arc_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_arc_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(ui->Home_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_arc_set_mode(ui->Home_arc_1, LV_ARC_MODE_NORMAL);
	lv_arc_set_range(ui->Home_arc_1, 0, 100);
	lv_arc_set_bg_angles(ui->Home_arc_1, 0, 90);
	lv_arc_set_angles(ui->Home_arc_1, 0, 15);
	lv_arc_set_rotation(ui->Home_arc_1, 92);
	lv_obj_set_style_arc_rounded(ui->Home_arc_1, 0, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_rounded(ui->Home_arc_1, 0, LV_STATE_DEFAULT);

	//Write codes Home_img_brtty
	ui->Home_img_brtty = lv_img_create(ui->Home);
	lv_obj_set_pos(ui->Home_img_brtty, 93, 17);
	lv_obj_set_size(ui->Home_img_brtty, 10, 11);
	lv_obj_set_scrollbar_mode(ui->Home_img_brtty, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_brtty. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_brtty, lv_color_make(0xf1, 0x11, 0x11), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_brtty, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_brtty, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_brtty, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_brtty, &_flash_alpha_10x11);
	lv_img_set_pivot(ui->Home_img_brtty, 50, 50);
	lv_img_set_angle(ui->Home_img_brtty, 20);

	//Write codes Home_img_wea
	ui->Home_img_wea = lv_img_create(ui->Home);
	lv_obj_set_pos(ui->Home_img_wea, 32, 83);
	lv_obj_set_size(ui->Home_img_wea, 40, 40);
	lv_obj_set_scrollbar_mode(ui->Home_img_wea, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_wea. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_wea, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_wea, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_wea, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_wea, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_wea, &_tianqiqing_i_duoyun_alpha_40x40);
	lv_img_set_pivot(ui->Home_img_wea, 50, 50);
	lv_img_set_angle(ui->Home_img_wea, 0);
	static bool Home_digital_clock_1_timer_enabled = false;

	//Write codes Home_digital_clock_1
	char* date_str = pvPortMalloc(16);
	sprintf(date_str, "%d:%d:%d", Home_digital_clock_1_hour_value, Home_digital_clock_1_min_value, Home_digital_clock_1_sec_value);
	ui->Home_digital_clock_1 = lv_dclock_create(ui->Home, date_str);
	lv_obj_set_style_text_align(ui->Home_digital_clock_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_pos(ui->Home_digital_clock_1, 50, 160);
	lv_obj_set_size(ui->Home_digital_clock_1, 141, 56);

	//create timer	
	if (!Home_digital_clock_1_timer_enabled) {
		lv_timer_create(Home_digital_clock_1_timer, 1000, NULL);
		Home_digital_clock_1_timer_enabled = true;
	}
	//Set style for Home_digital_clock_1. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_digital_clock_1, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_digital_clock_1, lv_color_make(0xfe, 0xfe, 0xfe), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_digital_clock_1, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_text
	ui->Home_img_text = lv_img_create(ui->Home);
	lv_obj_set_pos(ui->Home_img_text, 13, 128);
	lv_obj_set_size(ui->Home_img_text, 30, 57);
	lv_obj_set_scrollbar_mode(ui->Home_img_text, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_text. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_text, lv_color_make(0xcc, 0xcc, 0xcc), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_text, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_text, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_text, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_text, &_text2_alpha_30x57);
	lv_img_set_pivot(ui->Home_img_text, 50, 50);
	lv_img_set_angle(ui->Home_img_text, 30);

	//Write codes Home_cont_clock
	ui->Home_cont_clock = lv_obj_create(ui->Home);
	lv_obj_set_pos(ui->Home_cont_clock, 0, 0);
	lv_obj_set_size(ui->Home_cont_clock, 240, 240);
	lv_obj_set_scrollbar_mode(ui->Home_cont_clock, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_cont_clock. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_cont_clock, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_cont_clock, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_cont_clock, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_cont_clock, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_cont_clock, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_cont_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_cont_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_hour
	ui->Home_img_hour = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_hour, 111, 76);
	lv_obj_set_size(ui->Home_img_hour, 11, 50);
	lv_obj_set_scrollbar_mode(ui->Home_img_hour, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_hour. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_hour, lv_color_make(0xf0, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_hour, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_hour, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_hour, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_hour, &_clockwise_hour_alpha_11x50);
	lv_img_set_pivot(ui->Home_img_hour, 6, 46);
	lv_img_set_angle(ui->Home_img_hour, Home_digital_clock_1_hour_value*30*10);

	//Write codes Home_img_min
	ui->Home_img_min = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_min, 111, 53);
	lv_obj_set_size(ui->Home_img_min, 12, 70);
	lv_obj_set_scrollbar_mode(ui->Home_img_min, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_min. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_min, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_min, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_min, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_min, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_min, &_clockwise_min_alpha_12x70);
	lv_img_set_pivot(ui->Home_img_min, 7, 68);
	lv_img_set_angle(ui->Home_img_min, Home_digital_clock_1_min_value*6*10);

	//Write codes Home_img_sec
	ui->Home_img_sec = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_sec, 108, 31);
	lv_obj_set_size(ui->Home_img_sec, 18, 105);
	lv_obj_set_scrollbar_mode(ui->Home_img_sec, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_sec. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_sec, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_sec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_sec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_sec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_sec, &_clockwise_sec_alpha_18x105);
	lv_img_set_pivot(ui->Home_img_sec, 9, 91);
	lv_img_set_angle(ui->Home_img_sec, Home_digital_clock_1_sec_value*6*10);

	//Write codes Home_label_2
	ui->Home_label_2 = lv_label_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_label_2, 141, 90);
	lv_obj_set_size(ui->Home_label_2, 39, 20);
	lv_obj_set_scrollbar_mode(ui->Home_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text_fmt(ui->Home_label_2, "%02d/", Home_digital_date_mon_value);
	lv_label_set_long_mode(ui->Home_label_2, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_2, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_2, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_2, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_label_3
	ui->Home_label_3 = lv_label_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_label_3, 172, 89);
	lv_obj_set_size(ui->Home_label_3, 39, 29);
	lv_obj_set_scrollbar_mode(ui->Home_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text_fmt(ui->Home_label_3, "%02d", Home_digital_date_day_value);
	lv_label_set_long_mode(ui->Home_label_3, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_3, lv_color_make(0xf0, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_3, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_label_4
	ui->Home_label_4 = lv_label_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_label_4, 136, 111);
	lv_obj_set_size(ui->Home_label_4, 73, 20);
	lv_obj_set_scrollbar_mode(ui->Home_label_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text_fmt(ui->Home_label_4, "%04d", Home_digital_date_yesr_value);
	lv_label_set_long_mode(ui->Home_label_4, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_4, lv_color_make(0x66, 0x65, 0x65), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_4, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_wifi
	ui->Home_img_wifi = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_wifi, 109, 30);
	lv_obj_set_size(ui->Home_img_wifi, 20, 20);
	lv_obj_set_scrollbar_mode(ui->Home_img_wifi, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_wifi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_wifi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
	if (ui->wifi_stayus)lv_img_set_src(ui->Home_img_wifi, &_wifi_alpha_20x20);
	else
		lv_img_set_src(ui->Home_img_wifi, &_no_internet_alpha_20x20);

	lv_img_set_pivot(ui->Home_img_wifi, 0, 0);
	lv_img_set_angle(ui->Home_img_wifi, 0);

	//Write textprogress Home_textprogress_b
	ui->Home_textprogress_b = lv_label_create(ui->Home_cont_clock);
	lv_obj_set_style_text_align(ui->Home_textprogress_b, LV_TEXT_ALIGN_CENTER, 0);
	lv_Home_textprogress_b_set_range_value(0, 100, 0, 0);
	lv_Home_textprogress_b_set_decimal(0);
	lv_Home_textprogress_b_set_value(ui->Home_textprogress_b, 80);
	lv_obj_set_pos(ui->Home_textprogress_b, 101, 210);
	lv_obj_set_size(ui->Home_textprogress_b, 38, 17);

	//Set style for Home_textprogress_b. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_textprogress_b, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_textprogress_b, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_textprogress_b, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_textprogress_b, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_textprogress_b, lv_color_make(0xf3, 0x29, 0x29), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_textprogress_b, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_textprogress_b, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write textprogress Home_textprogress_D
	ui->Home_textprogress_D = lv_label_create(ui->Home_cont_clock);
	lv_obj_set_style_text_align(ui->Home_textprogress_D, LV_TEXT_ALIGN_CENTER, 0);
	lv_Home_textprogress_D_set_range_value(0, 100, 0, 0);
	lv_Home_textprogress_D_set_decimal(0);
	lv_Home_textprogress_D_set_value(ui->Home_textprogress_D, 80);
	lv_obj_set_pos(ui->Home_textprogress_D, 101, 7);
	lv_obj_set_size(ui->Home_textprogress_D, 38, 17);

	//Set style for Home_textprogress_D. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_textprogress_D, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_textprogress_D, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_textprogress_D, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_textprogress_D, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_textprogress_D, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_textprogress_D, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_textprogress_D, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_wifi_connect
	ui->Home_img_wifi_connect = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_wifi_connect, 191, 146);
	lv_obj_set_size(ui->Home_img_wifi_connect, 20, 20);
	lv_obj_set_scrollbar_mode(ui->Home_img_wifi_connect, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_wifi_connect. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_wifi_connect, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_wifi_connect, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_wifi_connect, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_wifi_connect, LV_OBJ_FLAG_CLICKABLE);

	//Hidden for widget Home_img_wifi_connect
	lv_obj_add_flag(ui->Home_img_wifi_connect, LV_OBJ_FLAG_HIDDEN);

	lv_img_set_src(ui->Home_img_wifi_connect, &_wifi_alpha_20x20);
	lv_img_set_pivot(ui->Home_img_wifi_connect, 0, 0);
	lv_img_set_angle(ui->Home_img_wifi_connect, 0);

	//Write codes Home_img_2
	ui->Home_img_2 = lv_img_create(ui->Home_cont_clock);
	lv_obj_set_pos(ui->Home_img_2, 216, 81);
	lv_obj_set_size(ui->Home_img_2, 10, 80);
	lv_obj_set_scrollbar_mode(ui->Home_img_2, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_2. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_2, lv_color_make(0x84, 0x76, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_2, 135, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_2, &_5_alpha_10x80);
	lv_img_set_pivot(ui->Home_img_2, 50, 50);
	lv_img_set_angle(ui->Home_img_2, 20);

	vPortFree(date_str);
	//Init events for screen]
	events_init_Home(ui);
}

