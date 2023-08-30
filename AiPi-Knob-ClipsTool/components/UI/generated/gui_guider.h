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
		lv_obj_t* screen;
		bool screen_del;
		lv_obj_t* screen_tileview_1;
		lv_obj_t* tile_JY;
		lv_obj_t* screen_imgbtn_JY;
		lv_obj_t* screen_imgbtn_JY_label;
		lv_obj_t* tile_Pr;
		lv_obj_t* screen_imgbtn_Pr;
		lv_obj_t* screen_imgbtn_Pr_label;
		lv_obj_t* screen_img_1;
		lv_obj_t* screen_label_3;
		lv_obj_t* screen_label_4;
		lv_obj_t* screen_Pr;
		bool screen_Pr_del;
		lv_obj_t* screen_Pr_img_pr;
		lv_obj_t* screen_Pr_label_1;
		lv_obj_t* screen_JY;
		bool screen_JY_del;
		lv_obj_t* screen_JY_img_jy;
		lv_obj_t* screen_JY_label_2;
		lv_group_t* group;
		int screen_type;
		bool BLE_status;
	}lv_ui;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;
	void setup_scr_screen(lv_ui* ui);
	void setup_scr_screen_Pr(lv_ui* ui);
	void setup_scr_screen_JY(lv_ui* ui);
	LV_IMG_DECLARE(_JY_alpha_100x100);
	LV_IMG_DECLARE(_Pr_alpha_100x100);
	LV_IMG_DECLARE(_Pr_s_alpha_100x100);
	LV_IMG_DECLARE(_JY_s_alpha_100x100);
	LV_IMG_DECLARE(_BLE_NO_alpha_100x100);

#ifdef __cplusplus
}
#endif
#endif