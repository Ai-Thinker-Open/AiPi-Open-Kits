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
		lv_obj_t* Home;
		bool Home_del;
		lv_obj_t* Home_img_bg;
		lv_obj_t* Home_label_temp;
		lv_obj_t* Home_arc_1;
		lv_obj_t* Home_img_brtty;
		lv_obj_t* Home_img_wea;
		lv_obj_t* Home_digital_clock_1;
		lv_obj_t* Home_img_text;
		lv_obj_t* Home_cont_clock;
		lv_obj_t* Home_img_hour;
		lv_obj_t* Home_img_min;
		lv_obj_t* Home_img_sec;
		lv_obj_t* Home_label_2;
		lv_obj_t* Home_label_3;
		lv_obj_t* Home_label_4;
		lv_obj_t* Home_img_wifi;
		lv_obj_t* Home_textprogress_b;
		lv_obj_t* Home_textprogress_D;
		lv_obj_t* Home_img_wifi_connect;
		lv_obj_t* Home_img_2;
		lv_obj_t* config;
		bool config_del;
		lv_obj_t* config_img_1;
		lv_obj_t* config_ta_pass;
		lv_obj_t* config_label_PASS;
		lv_obj_t* config_label_SSID;
		lv_obj_t* config_ta_ssid;
		lv_obj_t* config_btn_1;
		lv_obj_t* config_btn_1_label;
		lv_obj_t* config_img_h;
		lv_obj_t* config_cont_1;
		lv_obj_t* config_img_loding;
		lv_obj_t* config_label_5;
		lv_obj_t* config_msgbox;
		lv_obj_t* config_img_3;
		char ssid[32];
		char password[64];
		bool wifi_stayus;
		bool screen_type;
	}lv_ui;

	extern int Home_digital_date_yesr_value;
	extern int Home_digital_date_mon_value;
	extern int Home_digital_date_day_value;
	extern int Home_digital_date_wday_value;

	extern int Home_digital_clock_1_hour_value;
	extern int Home_digital_clock_1_min_value;
	extern int Home_digital_clock_1_sec_value;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;
	void setup_scr_Home(lv_ui* ui);
	void setup_scr_config(lv_ui* ui);
	void ui_config_creat_msg_box(lv_ui* ui, char* Prompt_content);
	LV_IMG_DECLARE(_clockwise_min_alpha_12x70);
	LV_IMG_DECLARE(_5_alpha_10x80);
	LV_IMG_DECLARE(_loding_alpha_150x150);
	LV_IMG_DECLARE(_5_alpha_15x80);
	LV_IMG_DECLARE(_text2_alpha_30x57);
	LV_IMG_DECLARE(_wifi_alpha_20x20);
	LV_IMG_DECLARE(_house_alpha_20x20);
	LV_IMG_DECLARE(_bg_alpha_239x239);
	LV_IMG_DECLARE(_clockwise_hour_alpha_11x50);
	LV_IMG_DECLARE(_bg_alpha_240x240);
	LV_IMG_DECLARE(_no_internet_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_duoyun_alpha_40x40);
	LV_IMG_DECLARE(_flash_alpha_10x11);
	LV_IMG_DECLARE(_clockwise_sec_alpha_18x105);

#ifdef __cplusplus
}
#endif
#endif