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
	lv_obj_t *Home;
	bool Home_del;
	lv_obj_t *Home_img_bg_home;
	lv_obj_t *Home_cont_home;
	lv_obj_t *Home_img_revc;
	lv_obj_t *Home_ta_ircmd;
	lv_obj_t *Home_btn_test;
	lv_obj_t *Home_btn_test_label;
	lv_obj_t *Home_btn_save;
	lv_obj_t *Home_btn_save_label;
	lv_obj_t *Home_btn_xieyi;
	lv_obj_t *Home_btn_xieyi_label;
	lv_obj_t *Home_label_xiname;
	lv_obj_t *ScreenHome;
	bool ScreenHome_del;
	lv_obj_t *ScreenHome_img_bg;
	lv_obj_t *ScreenHome_cont_1;
	lv_obj_t *ScreenHome_roller_1;
	lv_obj_t *ScreenHome_btn_2;
	lv_obj_t *ScreenHome_btn_2_label;
	lv_obj_t *screen_save;
	bool screen_save_del;
	lv_obj_t *screen_save_img_bkg;
	lv_obj_t *screen_save_cont_2;
	lv_obj_t *screen_save_roller_2;
	lv_obj_t *screen_save_btn_1;
	lv_obj_t *screen_save_btn_1_label;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_img_bk3;
	lv_obj_t *screen_1_cont_3;
	lv_obj_t *screen_1_btn_ok;
	lv_obj_t *screen_1_btn_ok_label;
	lv_obj_t *screen_1_ddlist_cmd_list;
	lv_obj_t *screen_1_btn_list;
	lv_obj_t *screen_1_btn_list_label;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_Home(lv_ui *ui);
void setup_scr_ScreenHome(lv_ui *ui);
void setup_scr_screen_save(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
LV_IMG_DECLARE(_bk_alpha_240x240);
LV_IMG_DECLARE(_ACT006_alpha_80x80);

#ifdef __cplusplus
}
#endif
#endif