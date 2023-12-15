/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *g_kb_screen;
	lv_obj_t *screen_tileview_home;
	lv_obj_t *screen_tileview_home_tile_home;
	lv_obj_t *screen_tileview_home_tile_ctrl;
	lv_obj_t *screen_tileview_home_tile_config;
	lv_obj_t *screen_tileview_home_tile_code;
	lv_obj_t *screen_cont_date;
	lv_obj_t *screen_digital_clock_date;
	lv_obj_t *screen_datetext_date;
	lv_obj_t *screen_label_wday;
	lv_obj_t *screen_cont_2;
	lv_obj_t *screen_btn_1;
	lv_obj_t *screen_btn_1_label;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_cont_3;
	lv_obj_t *screen_btn_2;
	lv_obj_t *screen_btn_2_label;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_cont_4;
	lv_obj_t *screen_btn_3;
	lv_obj_t *screen_btn_3_label;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_img_3;
	lv_obj_t *screen_cont_5;
	lv_obj_t *screen_btn_4;
	lv_obj_t *screen_btn_4_label;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_img_4;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_cont_6;
	lv_obj_t *screen_label_temp_day;
	lv_obj_t *screen_label_temp_night;
	lv_obj_t *screen_label_win;
	lv_obj_t *screen_label_pre;
	lv_obj_t *screen_label_air;
	lv_obj_t *screen_label_humi;
	lv_obj_t *screen_btn_sw_all;
	lv_obj_t *screen_btn_sw_all_label;
	lv_obj_t *screen_label_14;
	lv_obj_t *screen_img_5;
	lv_obj_t *screen_label_13;
	lv_obj_t *screen_cont_7;
	lv_obj_t *screen_imgbtn_sw1;
	lv_obj_t *screen_imgbtn_sw1_label;
	lv_obj_t *screen_label_one_L;
	lv_obj_t *screen_cont_8;
	lv_obj_t *screen_imgbtn_sw2;
	lv_obj_t *screen_imgbtn_sw2_label;
	lv_obj_t *screen_label_15;
	lv_obj_t *screen_cont_9;
	lv_obj_t *screen_imgbtn_sw3;
	lv_obj_t *screen_imgbtn_sw3_label;
	lv_obj_t *screen_label_16;
	lv_obj_t *screen_label_tile_2;
	lv_obj_t *screen_cont_10;
	lv_obj_t *screen_ta_SSID;
	lv_obj_t *screen_label_17;
	lv_obj_t *screen_label_18;
	lv_obj_t *screen_ta_PASS;
	lv_obj_t *screen_btn_connect;
	lv_obj_t *screen_btn_connect_label;
	lv_obj_t *screen_label_19;
	lv_obj_t *screen_img_6;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;

void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_outHome_40x40_alpha_40x40);
LV_IMG_DECLARE(_pc_40x40_alpha_40x40);
LV_IMG_DECLARE(_sleep_40x40_alpha_40x40);
LV_IMG_DECLARE(_vido_40x40_alpha_40x40);
LV_IMG_DECLARE(_sw_alpha_43x42);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_sw_alpha_61x62);
LV_IMG_DECLARE(_sws_alpha_61x62);
LV_IMG_DECLARE(_code_alpha_230x230);

LV_FONT_DECLARE(lv_font_Antonio_Regular_60)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Alatsi_Regular_20)
LV_FONT_DECLARE(lv_font_MiSans_Medium_16)
LV_FONT_DECLARE(lv_font_simsun_18)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_MiSans_Medium_30)
LV_FONT_DECLARE(lv_font_MiSans_Medium_20)
LV_FONT_DECLARE(lv_font_MiSans_Demibold_20)


#ifdef __cplusplus
}
#endif
#endif
