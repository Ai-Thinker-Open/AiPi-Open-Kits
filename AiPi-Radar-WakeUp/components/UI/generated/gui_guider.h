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
		lv_obj_t* Home_tileview_1;
		lv_obj_t* tile_config;
		lv_obj_t* Home_label_ssd;
		lv_obj_t* Home_label_PASS;
		lv_obj_t* Home_ta_passInput;
		lv_obj_t* Home_label_IP;
		lv_obj_t* Home_btn_connect;
		lv_obj_t* Home_btn_connect_label;
		lv_obj_t* Home_btn_2;
		lv_obj_t* Home_btn_2_label;
		lv_obj_t* Home_ddlist_ssid;
		lv_obj_t* tile_hemo;
		lv_obj_t* Home_cont_weather;
		lv_obj_t* Home_img_1;
		lv_obj_t* Home_label_temp;
		lv_obj_t* Home_label_addr;
		lv_obj_t* Home_label_weather;
		lv_obj_t* Home_label_to;
		lv_obj_t* Home_label_day2;
		lv_obj_t* Home_label_day3;
		lv_obj_t* Home_img_day1;
		lv_obj_t* Home_img_day2;
		lv_obj_t* Home_img_day3;
		lv_obj_t* Home_label_day1_temp;
		lv_obj_t* Home_label_day2_temp;
		lv_obj_t* Home_label_day3_temp;
		lv_obj_t* Home_time;
		lv_obj_t* Home_digital_clock_1;
		lv_obj_t* Home_label_date;
		lv_obj_t* Home_label_wday;
		lv_obj_t* Home_cont_1;
		lv_obj_t* Home_img_wifi;
		lv_obj_t* Home_cont_loding;
		lv_obj_t* Home_img_loding;
	}lv_ui;

	void ui_init_style(lv_style_t* style);
	void init_scr_del_flag(lv_ui* ui);
	void setup_ui(lv_ui* ui);
	extern lv_ui guider_ui;

	extern	int Home_digital_clock_1_hour_value;
	extern	int Home_digital_clock_1_min_value;
	extern	int Home_digital_clock_1_sec_value;
	extern	int  src_home_digital_date_day_value;
	extern	int  src_home_digital_date_wday_value;
	extern	int  src_home_digital_date_mont_value;
	extern	int  src_home_digital_date_yesr_value;

	void setup_scr_Home(lv_ui* ui);

	LV_IMG_DECLARE(_loading_alpha_100x100);
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