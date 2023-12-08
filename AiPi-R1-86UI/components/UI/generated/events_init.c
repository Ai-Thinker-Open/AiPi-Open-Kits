/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "log.h"

#define DBG_TAG "lvgl event"
static device_mode_t mode;

static void screen_img_2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			// if (mode==DEVICE_MODE_OUT_HOME) mode = DEVICE_MODE_NONE;
			if (mode!=DEVICE_MODE_OUT_HOME||mode==DEVICE_MODE_NONE) mode = DEVICE_MODE_OUT_HOME;
			else {

				mode = DEVICE_MODE_NONE;
			}
			LOG_I("mode=%s", mode==DEVICE_MODE_OUT_HOME?"DEVICE_MODE_OUT_HOME":"DEVICE_MODE_NONE");

			lv_obj_set_style_bg_color(guider_ui.screen_cont_2, mode!=DEVICE_MODE_NONE? lv_color_hex(0xf8fffe):lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_2, mode!=DEVICE_MODE_NONE?lv_color_hex(0x000000):lv_color_hex(0xc4c4c4), LV_PART_MAIN);

			lv_obj_set_style_bg_color(guider_ui.screen_cont_3, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_4, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_5, lv_color_hex(0x3d4253), LV_PART_MAIN);

			lv_obj_set_style_text_color(guider_ui.screen_label_3, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_4, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_5, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			break;
		}
		default:
			break;
	}
}
static void screen_img_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			if (mode!=DEVICE_MODE_WORK||mode==DEVICE_MODE_NONE) mode = DEVICE_MODE_WORK;
			else {
				mode = DEVICE_MODE_NONE;
			}
			LOG_I("mode=%s", mode==DEVICE_MODE_WORK?"DEVICE_MODE_WORK":"DEVICE_MODE_NONE");
			lv_obj_set_style_bg_color(guider_ui.screen_cont_3, mode!=DEVICE_MODE_NONE? lv_color_hex(0xf8fffe):lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_3, mode!=DEVICE_MODE_NONE?lv_color_hex(0x000000):lv_color_hex(0xc4c4c4), LV_PART_MAIN);

			lv_obj_set_style_bg_color(guider_ui.screen_cont_2, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_4, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_5, lv_color_hex(0x3d4253), LV_PART_MAIN);

			lv_obj_set_style_text_color(guider_ui.screen_label_2, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_4, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_5, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			break;
		}
		default:
			break;
	}
}
static void screen_img_3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			if (mode!=DEVICE_MODE_SLEEP||mode==DEVICE_MODE_NONE) mode = DEVICE_MODE_SLEEP;
			else {
				mode = DEVICE_MODE_NONE;
			}
			LOG_I("mode=%s", mode==DEVICE_MODE_SLEEP?"DEVICE_MODE_SLEEP":"DEVICE_MODE_NONE");
			lv_obj_set_style_bg_color(guider_ui.screen_cont_4, mode!=DEVICE_MODE_NONE? lv_color_hex(0xf8fffe):lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_4, mode!=DEVICE_MODE_NONE?lv_color_hex(0x000000):lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_2, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_3, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_5, lv_color_hex(0x3d4253), LV_PART_MAIN);

			lv_obj_set_style_text_color(guider_ui.screen_label_2, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_3, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_5, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			break;
		}
		default:
			break;
	}
}
static void screen_btn_4_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{

			if (mode!=DEVICE_MODE_VIDEO||mode==DEVICE_MODE_NONE) mode = DEVICE_MODE_VIDEO;
			else {
				mode = DEVICE_MODE_NONE;
			}
			LOG_I("mode=%s", mode==DEVICE_MODE_VIDEO?"DEVICE_MODE_VIDEO":"DEVICE_MODE_NONE");
			lv_obj_set_style_bg_color(guider_ui.screen_cont_5, mode!=DEVICE_MODE_NONE? lv_color_hex(0xf8fffe):lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_5, mode!=DEVICE_MODE_NONE?lv_color_hex(0x000000):lv_color_hex(0xc4c4c4), LV_PART_MAIN);

			lv_obj_set_style_bg_color(guider_ui.screen_cont_4, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_3, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_2, lv_color_hex(0x3d4253), LV_PART_MAIN);

			lv_obj_set_style_text_color(guider_ui.screen_label_2, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_3, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_4, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			break;
		}
		default:
			break;
	}
}
static void screen_img_4_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			if (mode!=DEVICE_MODE_VIDEO||mode==DEVICE_MODE_NONE) mode = DEVICE_MODE_VIDEO;
			else {
				mode = DEVICE_MODE_NONE;
			}
			LOG_I("mode=%s", mode==DEVICE_MODE_VIDEO?"DEVICE_MODE_VIDEO":"DEVICE_MODE_NONE");
			lv_obj_set_style_bg_color(guider_ui.screen_cont_5, mode!=DEVICE_MODE_NONE? lv_color_hex(0xf8fffe):lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_5, mode!=DEVICE_MODE_NONE?lv_color_hex(0x000000):lv_color_hex(0xc4c4c4), LV_PART_MAIN);

			lv_obj_set_style_bg_color(guider_ui.screen_cont_4, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_3, lv_color_hex(0x3d4253), LV_PART_MAIN);
			lv_obj_set_style_bg_color(guider_ui.screen_cont_2, lv_color_hex(0x3d4253), LV_PART_MAIN);

			lv_obj_set_style_text_color(guider_ui.screen_label_2, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_3, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			lv_obj_set_style_text_color(guider_ui.screen_label_4, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			break;
		}
		default:
			break;
	}
}
void events_init_screen(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_img_2, screen_img_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_img_1, screen_img_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_img_3, screen_img_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_img_4, screen_img_4_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->screen_btn_1, screen_img_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_2, screen_img_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_3, screen_img_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_4, screen_img_4_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->screen_cont_2, screen_img_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_3, screen_img_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_4, screen_img_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_5, screen_img_4_event_handler, LV_EVENT_ALL, NULL);
}

void events_init(lv_ui* ui)
{

}
