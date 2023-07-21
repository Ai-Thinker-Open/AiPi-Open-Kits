/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *speed;
	bool speed_del;
	lv_obj_t *speed_btn_next;
	lv_obj_t *speed_btn_next_label;
	lv_obj_t *speed_cont_box;
	lv_obj_t *speed_meter_board;
	lv_meter_indicator_t *speed_meter_board_scale_1_ndline_0;
	lv_obj_t *speed_img_logo;
	lv_obj_t *speed_label_title;
	lv_obj_t *speed_label_speed_unit;
	lv_obj_t *speed_label_digit;
	lv_obj_t *record;
	bool record_del;
	lv_obj_t *record_chart_board;
	lv_obj_t *record_label_title;
	lv_obj_t *record_btn_before;
	lv_obj_t *record_btn_before_label;
	lv_obj_t *record_img_logo;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_speed(lv_ui *ui);
void setup_scr_record(lv_ui *ui);
LV_IMG_DECLARE(_logo_alpha_69x20);
LV_IMG_DECLARE(_logo_alpha_75x26);

#ifdef __cplusplus
}
#endif
#endif