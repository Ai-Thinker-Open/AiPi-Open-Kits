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
	lv_obj_t *screen_tileview_home;
	lv_obj_t *screen_tileview_home_tile_home;
	lv_obj_t *screen_tileview_home_tile_ctrl;
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
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_label_10;
	lv_obj_t *screen_label_11;
	lv_obj_t *screen_label_12;
	lv_obj_t *screen_label_13;
	lv_obj_t *screen_tileview_1;
	lv_obj_t *screen_tileview_1_tile_1;
	lv_obj_t *screen_tileview_1_tile_2;
	lv_obj_t *screen_tileview_1_tile_3;
	lv_obj_t *screen_tileview_1_tile_4;
	lv_obj_t *screen_label_14;
	lv_obj_t *screen_sw_1;
	lv_obj_t *screen_label_15;
	lv_obj_t *screen_btn_5;
	lv_obj_t *screen_btn_5_label;
	lv_obj_t *screen_label_16;
	lv_obj_t *screen_arc_1;
	lv_obj_t *screen_slider_1;
	lv_obj_t *screen_label_18;
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

LV_FONT_DECLARE(lv_font_Antonio_Regular_60)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Alatsi_Regular_20)
LV_FONT_DECLARE(lv_font_MiSans_Medium_16)
LV_FONT_DECLARE(lv_font_simsun_18)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_MiSans_Medium_30)


#ifdef __cplusplus
}
#endif
#endif
