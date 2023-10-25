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
		lv_obj_t* screen_img_bg;
		lv_obj_t* screen_cont_main;
		lv_obj_t* screen_cont_date;
		lv_obj_t* screen_digital_clock_1;
		lv_obj_t* screen_label_wday;
		lv_obj_t* screen_label_day;
		lv_obj_t* screen_cont_ligth3;
		lv_obj_t* screen_imgbtn_sw3;
		lv_obj_t* screen_imgbtn_sw3_label;
		lv_obj_t* screen_ddlist_list3;
		lv_obj_t* screen_imgbtn_on3;
		lv_obj_t* screen_imgbtn_on3_label;
		lv_obj_t* screen_cont_ligth2;
		lv_obj_t* screen_imgbtn_sw2;
		lv_obj_t* screen_imgbtn_sw2_label;
		lv_obj_t* screen_ddlist_list2;
		lv_obj_t* screen_imgbtn_on2;
		lv_obj_t* screen_imgbtn_on2_label;
		lv_obj_t* screen_cont_ligth1;
		lv_obj_t* screen_imgbtn_sw1;
		lv_obj_t* screen_imgbtn_sw1_label;
		lv_obj_t* screen_ddlist_list1;
		lv_obj_t* screen_imgbtn_on1;
		lv_obj_t* screen_imgbtn_on1_label;
		lv_obj_t* screen_img_aixingyun;
		lv_obj_t* screen_cont_title;
		lv_obj_t* screen_img_titlebg;
		lv_obj_t* screen_img_wifi;
		lv_obj_t* screen_digital_clock_2;
		lv_obj_t* screen_img_log;
		lv_obj_t* screen_img_logo_on;
		lv_obj_t* config;
		bool config_del;
		lv_obj_t* config_img_bg1;
		lv_obj_t* config_cont_title1;
		lv_obj_t* config_img_1;
		lv_obj_t* config_digital_clock_3;
		lv_obj_t* config_img_LOGO;
		lv_obj_t* config_cont_ssid;
		lv_obj_t* config_label_ssid;
		lv_obj_t* config_label_ip;
		lv_obj_t* config_img_tilebg2;
		lv_obj_t* config_btn_wificonfig;
		lv_obj_t* config_btn_wificonfig_label;
		lv_obj_t* config_btn_disconnect;
		lv_obj_t* config_btn_disconnect_label;
		lv_obj_t* config_msgbox_1;
	}lv_ui;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;
	void setup_scr_screen(lv_ui* ui);
	void setup_scr_config(lv_ui* ui);
	void Create_tips(lv_ui* ui, char* value);

	LV_IMG_DECLARE(_ligths_off_alpha_70x70);
	LV_IMG_DECLARE(_wifi_alpha_20x20);
	LV_IMG_DECLARE(_R286_bg_alpha_480x480);
	LV_IMG_DECLARE(_logo_off_alpha_20x20);
	LV_IMG_DECLARE(_Tilie_bg_alpha_480x30);
	LV_IMG_DECLARE(_Tilie_bg_alpha_480x21);
	LV_IMG_DECLARE(_logo2_alpha_80x82);
	LV_IMG_DECLARE(_ligth_on_alpha_70x70);
	LV_IMG_DECLARE(_ligth_off_alpha_70x70);
	LV_IMG_DECLARE(_logo_on_alpha_20x20);
	LV_IMG_DECLARE(_wifi_alpha_25x25);
	LV_IMG_DECLARE(_ligths_on_alpha_70x70);

#ifdef __cplusplus
}
#endif
#endif