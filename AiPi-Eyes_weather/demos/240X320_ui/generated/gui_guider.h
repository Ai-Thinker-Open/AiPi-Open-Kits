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
		lv_obj_t* cont_4;
		bool cont_4_del;
		lv_obj_t* cont_4_cont_2;
		lv_obj_t* cont_4_img_connet;
		lv_obj_t* cont_4_img_4;
		lv_obj_t* cont_4_cont_1;
		lv_obj_t* cont_4_label_1;
		lv_obj_t* cont_4_label_2;
		lv_obj_t* cont_4_digital_clock_1;
		lv_obj_t* cont_4_cont_3;
		lv_obj_t* cont_4_img_clear;
		lv_obj_t* cont_4_img_cloudy;
		lv_obj_t* cont_4_label_3;
		lv_obj_t* cont_4_label_4;
		lv_obj_t* cont_4_label_temp;
		lv_obj_t* cont_4_label_acquired;
		lv_obj_t* cont_4_label_5;
		lv_obj_t* cont_4_label_6;
		lv_obj_t* cont_4_img_3;
		lv_obj_t* cont_4_img_2;
		lv_obj_t* cont_4_img_1;
		lv_obj_t* cont_4_label_7;
		lv_obj_t* cont_4_label_8;
		lv_obj_t* cont_4_label_9;
		lv_obj_t* cont_4_img_xiaozD;
		lv_obj_t* cont_4_img_yeqing;
		lv_obj_t* cont_4_img_ye;
		lv_obj_t* cont_4_img_xiaoyu;
		lv_obj_t* WiFi_config;
		bool WiFi_config_del;
		lv_obj_t* WiFi_config_imgbtn_1;
		lv_obj_t* WiFi_config_imgbtn_1_label;
		lv_obj_t* WiFi_config_label_10;
		lv_obj_t* WiFi_config_ta_1;
		lv_obj_t* WiFi_config_ta_2;
		lv_obj_t* WiFi_config_label_11;
		lv_obj_t* WiFi_config_label_12;
		lv_obj_t* WiFi_config_btn_1;
		lv_obj_t* WiFi_config_btn_1_label;
		char ssid[32];
		char password[64];
		bool wifi_stayus;
		bool screen_type;
	}lv_ui;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;
	void setup_scr_cont_4(lv_ui* ui);
	void setup_scr_WiFi_config(lv_ui* ui);
	LV_IMG_DECLARE(_tianqiqing_i_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_baoyu_alpha_20x20);
	LV_IMG_DECLARE(_tianqidayu_alpha_100x100);
	LV_IMG_DECLARE(_bak_a_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_yintian_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_xiaoyuzhuanqing_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_alpha_100x100);
	LV_IMG_DECLARE(_tianqiqing_i_duoyun_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_leiyu_alpha_20x20);
	LV_IMG_DECLARE(_tianqiqing_i_zhongyu_alpha_20x20);
	LV_IMG_DECLARE(_no_internet_alpha_16x16);
	LV_IMG_DECLARE(_tianqiduoyun_alpha_100x100);
	LV_IMG_DECLARE(_tianqizhongyu_alpha_100x100);
	LV_IMG_DECLARE(_bak_b_alpha_20x20);
	LV_IMG_DECLARE(_tianqiyeleiyu_alpha_100x100);
	LV_IMG_DECLARE(_tianqiqingyeduoyun_alpha_100x100);
	LV_IMG_DECLARE(_tianqixiaoyu_alpha_100x100);
	LV_IMG_DECLARE(_tianqidabaoyu_alpha_100x100);
	LV_IMG_DECLARE(_tianqiye_alpha_100x100);
	LV_IMG_DECLARE(_tianqiduoyunxiaoyuzhuanqing_alpha_100x100);
	LV_IMG_DECLARE(_tianqiqing_i_dayu_alpha_20x20);
	LV_IMG_DECLARE(_wifi_alpha_16x16);
	LV_IMG_DECLARE(_tianqiqing_i_xiaoyu_alpha_20x20);

#ifdef __cplusplus
}
#endif
#endif