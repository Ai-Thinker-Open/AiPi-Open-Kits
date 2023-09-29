/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "dev_ir.h"
static char* ir_type[3] = { "NEC","RC5","SWM" };

void setup_scr_Home(lv_ui* ui) {

	//Write codes Home
	ui->Home = lv_obj_create(NULL);

	//Set style for Home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_bg_color(ui->Home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_bg_home
	ui->Home_img_bg_home = lv_img_create(ui->Home);
	lv_obj_set_pos(ui->Home_img_bg_home, 0, 0);
	lv_obj_set_size(ui->Home_img_bg_home, 240, 240);
	lv_obj_set_scrollbar_mode(ui->Home_img_bg_home, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_bg_home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_bg_home, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_bg_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_bg_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_bg_home, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_bg_home, &_bk_alpha_240x240);
	lv_img_set_pivot(ui->Home_img_bg_home, 50, 50);
	lv_img_set_angle(ui->Home_img_bg_home, 0);

	//Write codes Home_cont_home
	ui->Home_cont_home = lv_obj_create(ui->Home);
	lv_obj_set_pos(ui->Home_cont_home, 0, 0);
	lv_obj_set_size(ui->Home_cont_home, 240, 240);
	lv_obj_set_scrollbar_mode(ui->Home_cont_home, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_cont_home. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_cont_home, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_cont_home, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_cont_home, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_cont_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_cont_home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_cont_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_cont_home, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_cont_home, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_cont_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_cont_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Home_img_revc
	ui->Home_img_revc = lv_img_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_img_revc, 75, 11);
	lv_obj_set_size(ui->Home_img_revc, 80, 80);
	lv_obj_set_scrollbar_mode(ui->Home_img_revc, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_img_revc. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_img_recolor(ui->Home_img_revc, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->Home_img_revc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->Home_img_revc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->Home_img_revc, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->Home_img_revc, &_ACT006_alpha_80x80);
	lv_img_set_pivot(ui->Home_img_revc, 50, 50);
	lv_img_set_angle(ui->Home_img_revc, 0);

	//Write codes Home_ta_ircmd
	// ui->Home_ta_ircmd = lv_textarea_create(ui->Home_cont_home);
	ui->Home_ta_ircmd = lv_label_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_ta_ircmd, 20, 109);
	lv_obj_set_size(ui->Home_ta_ircmd, 194, 29);
	lv_obj_set_scrollbar_mode(ui->Home_ta_ircmd, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->Home_ta_ircmd, " ");
	//Set style for Home_ta_ircmd. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_ircmd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_ircmd, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_ta_ircmd, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_ta_ircmd, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_ta_ircmd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_ta_ircmd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_ta_ircmd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_ta_ircmd, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_ta_ircmd, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_ta_ircmd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_ta_ircmd, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_ta_ircmd, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_ta_ircmd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_ta_ircmd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_ta_ircmd, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_ta_ircmd, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Set style for Home_ta_ircmd. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_ta_ircmd, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_ta_ircmd, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_ta_ircmd, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//use keyboard on Home_ta_ircmd


	//Write codes Home_btn_test
	ui->Home_btn_test = lv_btn_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_btn_test, 45, 149);
	lv_obj_set_size(ui->Home_btn_test, 47, 27);
	lv_obj_set_scrollbar_mode(ui->Home_btn_test, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_test. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_test, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_test, lv_color_make(0x36, 0x91, 0xac), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_test, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_test, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_test, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_test, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_test, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_test, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_test, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_test, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_test, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_test, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_test, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_test, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_test, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_test, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_test, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_test_label = lv_label_create(ui->Home_btn_test);
	lv_label_set_text(ui->Home_btn_test_label, "测试");
	lv_obj_set_style_pad_all(ui->Home_btn_test, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_test_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_btn_save
	ui->Home_btn_save = lv_btn_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_btn_save, 140, 149);
	lv_obj_set_size(ui->Home_btn_save, 47, 27);
	lv_obj_set_scrollbar_mode(ui->Home_btn_save, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_save. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_save, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_save, lv_color_make(0x9a, 0x91, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_save, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_save, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_save, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_save, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_save, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_save, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_save, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_save, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_save, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_save, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_save, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_save, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_save, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_save, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_save, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_save_label = lv_label_create(ui->Home_btn_save);
	lv_label_set_text(ui->Home_btn_save_label, "保存");
	lv_obj_set_style_pad_all(ui->Home_btn_save, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_save_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_btn_xieyi
	ui->Home_btn_xieyi = lv_btn_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_btn_xieyi, 90, 189);
	lv_obj_set_size(ui->Home_btn_xieyi, 47, 27);
	lv_obj_set_scrollbar_mode(ui->Home_btn_xieyi, LV_SCROLLBAR_MODE_OFF);

	//Set style for Home_btn_xieyi. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_btn_xieyi, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_btn_xieyi, lv_color_make(0xa9, 0x4b, 0x76), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_btn_xieyi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_btn_xieyi, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_btn_xieyi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_btn_xieyi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_btn_xieyi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_btn_xieyi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_btn_xieyi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_btn_xieyi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_btn_xieyi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Home_btn_xieyi, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->Home_btn_xieyi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Home_btn_xieyi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_btn_xieyi, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_btn_xieyi, &lv_font_simhei_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_btn_xieyi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	ui->Home_btn_xieyi_label = lv_label_create(ui->Home_btn_xieyi);
	lv_label_set_text(ui->Home_btn_xieyi_label, "协议");
	lv_obj_set_style_pad_all(ui->Home_btn_xieyi, 0, LV_STATE_DEFAULT);
	lv_obj_align(ui->Home_btn_xieyi_label, LV_ALIGN_CENTER, 0, 0);

	//Write codes Home_label_xiname
	ui->Home_label_xiname = lv_label_create(ui->Home_cont_home);
	lv_obj_set_pos(ui->Home_label_xiname, 22, 90);
	lv_obj_set_size(ui->Home_label_xiname, 50, 16);
	lv_obj_set_scrollbar_mode(ui->Home_label_xiname, LV_SCROLLBAR_MODE_OFF);

	lv_label_set_text(ui->Home_label_xiname, ir_type[dev_ir_type]);
	lv_label_set_long_mode(ui->Home_label_xiname, LV_LABEL_LONG_WRAP);

	//Set style for Home_label_xiname. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
	lv_obj_set_style_radius(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Home_label_xiname, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_color(ui->Home_label_xiname, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Home_label_xiname, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_color(ui->Home_label_xiname, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_opa(ui->Home_label_xiname, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_spread(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_x(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_ofs_y(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Home_label_xiname, lv_color_make(0xf3, 0x2c, 0x2c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Home_label_xiname, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Home_label_xiname, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Home_label_xiname, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	ui->Home_del = false;
	//Init events for screen
	events_init_Home(ui);
}