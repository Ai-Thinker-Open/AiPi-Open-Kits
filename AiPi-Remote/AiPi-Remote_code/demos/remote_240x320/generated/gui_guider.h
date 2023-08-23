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
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_backgroud_img;
	lv_obj_t *screen_arc_1;
	lv_obj_t *screen_open_btn;
	lv_obj_t *screen_open_btn_label;
	lv_obj_t *screen_close_btn;
	lv_obj_t *screen_close_btn_label;
	lv_obj_t *screen_wind_btn;
	lv_obj_t *screen_wind_btn_label;
	lv_obj_t *screen_mode_btn;
	lv_obj_t *screen_mode_btn_label;
	lv_obj_t *screen_downarrow_btn;
	lv_obj_t *screen_downarrow_btn_label;
	lv_obj_t *screen_uparrow_btn;
	lv_obj_t *screen_uparrow_btn_label;
	lv_obj_t *screen_temp_lab;
	lv_obj_t *screen_unit_lab;
	lv_obj_t *screen_wind_sign;
	lv_obj_t *screen_mode_sign;
	lv_obj_t *screen_wind_status;
	lv_obj_t *screen_mode_status;
	lv_obj_t *screen_lowpower;
	lv_obj_t *screen_fullcharge;
	lv_obj_t *screen_normalcharge;
	lv_obj_t *screen_Charging;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_uparrow_alpha_24x24);
LV_IMG_DECLARE(_backgroup_alpha_240x320);
LV_IMG_DECLARE(_downarrow_alpha_24x24);
LV_IMG_DECLARE(_wind_alpha_40x40);
LV_IMG_DECLARE(_lowpower_alpha_36x36);
LV_IMG_DECLARE(_normalcharge_alpha_36x36);
LV_IMG_DECLARE(_Charging_alpha_36x36);
LV_IMG_DECLARE(_close_alpha_40x40);
LV_IMG_DECLARE(_fullcharge_alpha_36x36);
LV_IMG_DECLARE(_open_alpha_40x40);
LV_IMG_DECLARE(_mode_alpha_40x40);

#ifdef __cplusplus
}
#endif
#endif