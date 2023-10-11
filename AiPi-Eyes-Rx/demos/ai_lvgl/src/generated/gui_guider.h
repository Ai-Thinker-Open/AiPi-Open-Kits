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
#include "./guider_fonts/guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_img_3;
	lv_obj_t *screen_img_4;
	lv_obj_t *screen_img_5;
	lv_obj_t *screen_img_6;
	lv_obj_t *screen_img_7;
	lv_obj_t *screen_animimg_1;
	lv_obj_t *screen_animimg_2;
	lv_obj_t *screen_img_8;
	lv_obj_t *screen_img_9;
	lv_obj_t *screen_img_10;
	lv_obj_t *screen_img_11;
	lv_obj_t *screen_line_1;
	lv_obj_t *screen_line_2;
	lv_obj_t *screen_img_12;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_line_3;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_label_10;
	lv_obj_t *screen_label_11;
	lv_obj_t *screen_label_12;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);

#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_animimg_12new)
LV_IMG_DECLARE(screen_animimg_11new)

#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_animimg_22new)
LV_IMG_DECLARE(screen_animimg_21new)
LV_IMG_DECLARE(_9_alpha_40x40);
LV_IMG_DECLARE(_10_alpha_42x26);
LV_IMG_DECLARE(_100_alpha_480x480);
LV_IMG_DECLARE(_11_alpha_40x40);
LV_IMG_DECLARE(_7_alpha_40x40);
LV_IMG_DECLARE(_8_alpha_40x40);
LV_IMG_DECLARE(_4_alpha_130x215);
LV_IMG_DECLARE(_3_alpha_130x102);

#ifdef __cplusplus
}
#endif
#endif