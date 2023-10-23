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
	lv_obj_t *screen_img_bg;
	lv_obj_t *screen_tileview_1;
	lv_obj_t *tile_main;
	lv_obj_t *screen_cont_main;
	lv_obj_t *screen_cont_date;
	lv_obj_t *screen_digital_clock_1;
	lv_obj_t *screen_label_wday;
	lv_obj_t *screen_label_day;
	lv_obj_t *screen_cont_ligth2;
	lv_obj_t *screen_imgbtn_sw2;
	lv_obj_t *screen_imgbtn_sw2_label;
	lv_obj_t *screen_cont_ligth3;
	lv_obj_t *screen_imgbtn_sw3;
	lv_obj_t *screen_imgbtn_sw3_label;
	lv_obj_t *screen_cont_ligth1;
	lv_obj_t *screen_imgbtn_sw1;
	lv_obj_t *screen_imgbtn_sw1_label;
	lv_obj_t *screen_cont_title;
	lv_obj_t *screen_img_wifi;
	lv_obj_t *screen_line_1;
	lv_obj_t *screen_digital_clock_2;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_sw_all_s_alpha_50x50);
LV_IMG_DECLARE(_R286_bg_alpha_480x480);
LV_IMG_DECLARE(_sw_all_alpha_50x50);
LV_IMG_DECLARE(_wifi_alpha_25x25);

#ifdef __cplusplus
}
#endif
#endif