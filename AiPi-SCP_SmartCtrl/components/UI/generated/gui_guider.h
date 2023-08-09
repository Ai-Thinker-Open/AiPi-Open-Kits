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
		bool connect_status;
		bool switch_status;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	}mqtt_rgb_dev_t;


	typedef struct
	{
		lv_obj_t* screen;
		bool screen_del;
		lv_obj_t* screen_cont_temp;
		lv_obj_t* screen_img_1;
		lv_obj_t* screen_img_3;
		lv_obj_t* screen_label_2;
		lv_obj_t* screen_img_bg;
		lv_obj_t* screen_tileview_home;
		lv_obj_t* config;
		lv_obj_t* screen_label_ssid;
		lv_obj_t* screen_label_pass;
		lv_obj_t* screen_btn_scan;
		lv_obj_t* screen_btn_scan_label;
		lv_obj_t* screen_ta_pass;
		lv_obj_t* screen_btn_connect;
		lv_obj_t* screen_btn_connect_label;
		lv_obj_t* screen_ddlist_ssid_list;
		lv_obj_t* home;
		lv_obj_t* screen_label_tile;
		lv_obj_t* screen_digital_clock_1;
		lv_obj_t* screen_img_wifi;
		lv_obj_t* screen_cont_sensor;
		lv_obj_t* screen_label_sensorD;
		lv_obj_t* screen_label_O2;
		lv_obj_t* screen_label_1;
		lv_obj_t* screen_label_PH;
		lv_obj_t* screen_label_O2_vlue;
		lv_obj_t* screen_label_temp_value;
		lv_obj_t* screen_label_PH_value;
		lv_obj_t* screen_label_3;
		lv_obj_t* screen_label_host;
		lv_obj_t* screen_cont_dev1;
		lv_obj_t* screen_img_rgb;
		lv_obj_t* screen_img_wb2_open;
		lv_obj_t* screen_label_rgbtext1;
		lv_obj_t* screen_label_status1;
		lv_obj_t* screen_img_wb2_close;
		lv_obj_t* screen_cont_dev2;
		lv_obj_t* screen_img_rgb1;
		lv_obj_t* screen_img_m62_open;
		lv_obj_t* screen_label_rgbtext2;
		lv_obj_t* screen_label_status2;
		lv_obj_t* screen_img_m62_close;
		lv_obj_t* screen_cont_dev3;
		lv_obj_t* screen_img_rgb3;
		lv_obj_t* screen_img_bw16_open;
		lv_obj_t* screen_label_rgbtext3;
		lv_obj_t* screen_label_status3;
		lv_obj_t* screen_img_bw16_close;
		lv_obj_t* screen_label_date;
		lv_obj_t* screen_label_wday;
		lv_obj_t* screen_label_dizhi;
		lv_obj_t* screen_cont_dev1_no;
		lv_obj_t* screen_cont_dev2_no;
		lv_obj_t* screen_cont_dev_no;
		lv_obj_t* screen_cont_sensor_no;
		lv_obj_t* screen_cont_loading;
		lv_obj_t* screen_img_loading;
		lv_obj_t* screen_rgb1;
		bool screen_rgb1_del;
		lv_obj_t* screen_rgb1_img_rbg1_bg;
		lv_obj_t* screen_rgb1_imgbtn_brack;
		lv_obj_t* screen_rgb1_imgbtn_brack_label;
		lv_obj_t* screen_rgb1_label_rgb1_tile;
		lv_obj_t* screen_rgb1_cpicker_wb2_rgb;
		lv_obj_t* screen_rgb1_cont_wb2_sw;
		lv_obj_t* screen_rgb1_wb2_sw;
		lv_obj_t* screen_rgb1_label_4;
		lv_obj_t* screen_rgb2;
		bool screen_rgb2_del;
		lv_obj_t* screen_rgb2_img_rgb2_gb;
		lv_obj_t* screen_rgb2_imgbtn_brack2;
		lv_obj_t* screen_rgb2_imgbtn_brack2_label;
		lv_obj_t* screen_rgb2_label_M62_tile;
		lv_obj_t* screen_rgb2_cont_m62_sw;
		lv_obj_t* screen_rgb2_Ai_M62_sw;
		lv_obj_t* screen_rgb2_label_5;
		lv_obj_t* screen_rgb2_cpicker_M62_rgb;
		lv_obj_t* screen_rgb3;
		bool screen_rgb3_del;
		lv_obj_t* screen_rgb3_img_rgb3_bg;
		lv_obj_t* screen_rgb3_imgbtn_brack3;
		lv_obj_t* screen_rgb3_imgbtn_brack3_label;
		lv_obj_t* screen_rgb3_label_bw16_tile;
		lv_obj_t* screen_rgb3_cont_bw16_sw;
		lv_obj_t* screen_rgb3_bw16_sw;
		lv_obj_t* screen_rgb3_label_6;
		lv_obj_t* screen_rgb3_cpicker_bw16_rgb;
		mqtt_rgb_dev_t* ai_wb2_dev;
		mqtt_rgb_dev_t* ai_m62_dev;
		mqtt_rgb_dev_t* bw16_dev;

		bool fish_dev_connect_status;
		char ssid[32];
		char ssid_list[256];
		char password[64];
		bool wifi_status;
		bool wifi_scan_done;
		bool system_reset;
		int screen_type;
		bool mqtt_connect_status;
		char city[32];
		char waether[32];

	}lv_ui;

	extern int screen_digital_clock_1_hour_value;
	extern int screen_digital_clock_1_min_value;
	extern int screen_digital_clock_1_sec_value;
	extern int src_home_digital_date_yesr_value;
	extern int src_home_digital_date_mont_value;
	extern int src_home_digital_date_day_value;
	extern int src_home_digital_date_wday_value;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;
	void setup_scr_screen(lv_ui* ui);
	void setup_scr_screen_rgb1(lv_ui* ui);
	void setup_scr_screen_rgb2(lv_ui* ui);
	void setup_scr_screen_rgb3(lv_ui* ui);
	LV_IMG_DECLARE(_RGB_close_alpha_32x32);
	LV_IMG_DECLARE(_RGB_open_alpha_32x32);
	LV_IMG_DECLARE(_bak_a_alpha_40x40);
	LV_IMG_DECLARE(_Ligth_open_alpha_30x30);
	LV_IMG_DECLARE(_img_bg_alpha_320x240);
	LV_IMG_DECLARE(_loading_alpha_100x100);
	LV_IMG_DECLARE(_Ligth_close_alpha_30x30);
	LV_IMG_DECLARE(_bak_s_alpha_40x40);
	LV_IMG_DECLARE(_wifi_alpha_22x18);
	LV_IMG_DECLARE(_no_internet_alpha_22x18);

#ifdef __cplusplus
}
#endif
#endif