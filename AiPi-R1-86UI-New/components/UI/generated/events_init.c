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
#include "relays_dev.h"
#include "device_ctrl.h"
#define DBG_TAG "lvgl event"
static device_mode_t mode;

static bool light_all;
static bool light_1;
static bool light_2;
static bool light_3;

static void screen_img_2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
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
static void screen_btn_sw_all_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{

			if (!light_all)
			{
				//修改UI
				lv_obj_set_style_bg_color(guider_ui.screen_cont_7, lv_color_hex(0xFFE1F1), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_cont_9, lv_color_hex(0xFFE3B5), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_cont_8, lv_color_hex(0xF7FFDC), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_one_L, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_14, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_15, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_16, lv_color_hex(0x000000), LV_PART_MAIN);

				lv_obj_set_style_text_color(guider_ui.screen_label_14, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_img_5, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_img_5, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_btn_sw_all, lv_color_hex(0xf5f0f0), LV_PART_MAIN);

				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw1, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw1, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw2, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw2, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw3, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw3, lv_color_hex(0x000000), LV_PART_MAIN);

				// 控制继电器
				relays_on(&relay_sw1);
				relays_on(&relay_sw2);
				relays_on(&relay_sw2);
				light_all = true;
				light_1 = true;
				light_2 = true;
				light_3 = true;
			}
			else
			{
				//修改UI
				lv_obj_set_style_bg_color(guider_ui.screen_cont_7, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_cont_9, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_cont_8, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_img_5, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_img_5, 0, LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_btn_sw_all, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_14, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_one_L, lv_color_hex(0xc4c4c4), LV_PART_MAIN);


				lv_obj_set_style_text_color(guider_ui.screen_label_15, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_16, lv_color_hex(0xc4c4c4), LV_PART_MAIN);

				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw1, 0, LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw2, 0, LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw3, 0, LV_PART_MAIN);
				// 控制继电器
				relays_off(&relay_sw1);
				relays_off(&relay_sw2);
				relays_off(&relay_sw2);

				light_all = false;
				light_1 = false;
				light_2 = false;
				light_3 = false;

			}

		}
		break;
		case LV_EVENT_CHILD_CHANGED:
		{
			if (light_all) {
				lv_obj_set_style_text_color(guider_ui.screen_label_14, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_img_5, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_img_5, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_btn_sw_all, lv_color_hex(0xf5f0f0), LV_PART_MAIN);

			}
			else {

				lv_obj_set_style_img_recolor(guider_ui.screen_img_5, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_img_5, 0, LV_PART_MAIN);
				lv_obj_set_style_bg_color(guider_ui.screen_btn_sw_all, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_14, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
			}
		}
		break;
		default:
			break;
	}
}
static void screen_imgbtn_sw1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			LOG_I("light1 value=%d", light_1);
			if (!light_1) {
				lv_obj_set_style_bg_color(guider_ui.screen_cont_7, lv_color_hex(0xFFE1F1), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw1, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw1, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_one_L, lv_color_hex(0x000000), LV_PART_MAIN);
				relays_on(&relay_sw1);
				light_1 = true;
				if (light_1 && light_2 && light_3) {
					light_all = true;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}
			}
			else {
				lv_obj_set_style_bg_color(guider_ui.screen_cont_7, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw1, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw1, 0, LV_PART_MAIN);

				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw1, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_one_L, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
				relays_off(&relay_sw1);
				light_1 = false;
				if (!light_1 && !light_2 && !light_3) {
					light_all = false;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}
			}
		}
		break;


		default:
			break;
	}
}
static void screen_imgbtn_sw2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			LOG_I("light2 value=%d", light_2);
			if (!light_2)
			{
				lv_obj_set_style_bg_color(guider_ui.screen_cont_8, lv_color_hex(0xF7FFDC), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw2, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw2, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_15, lv_color_hex(0x000000), LV_PART_MAIN);
				relays_on(&relay_sw2);
				light_2 = true;
				if (light_1 && light_2 && light_3) {
					light_all = true;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}
			}
			else {
				lv_obj_set_style_bg_color(guider_ui.screen_cont_8, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw2, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw2, 0, LV_PART_MAIN);

				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw2, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_15, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
				relays_off(&relay_sw2);
				light_2 = false;

				if (!light_1 && !light_2 && !light_3) {
					light_all = false;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}

			}

			break;
		}
		default:
			break;
	}
}
static void screen_imgbtn_sw3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
		case LV_EVENT_CLICKED:
		{
			LOG_I("light3 value=%d", light_3);
			if (!light_3)
			{
				lv_obj_set_style_bg_color(guider_ui.screen_cont_9, lv_color_hex(0xFFE3B5), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw3, 255, LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw3, lv_color_hex(0x000000), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_16, lv_color_hex(0x000000), LV_PART_MAIN);
				relays_on(&relay_sw3);
				light_3 = true;
				if (light_1 && light_2 && light_3) {
					light_all = true;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}
			}
			else {
				lv_obj_set_style_bg_color(guider_ui.screen_cont_9, lv_color_hex(0x3d4253), LV_PART_MAIN);
				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw3, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_img_recolor_opa(guider_ui.screen_imgbtn_sw3, 0, LV_PART_MAIN);

				lv_obj_set_style_img_recolor(guider_ui.screen_imgbtn_sw3, lv_color_hex(0xffffff), LV_PART_MAIN);
				lv_obj_set_style_text_color(guider_ui.screen_label_16, lv_color_hex(0xc4c4c4), LV_PART_MAIN);
				relays_off(&relay_sw3);
				light_3 = false;
				if (!light_1 && !light_2 && !light_3) {
					light_all = false;
					lv_event_send(guider_ui.screen_btn_sw_all, LV_EVENT_CHILD_CHANGED, NULL);

				}
			}

			break;
		}
		default:
			break;
	}
}

static void screen_btn_connec_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code) {
		case LV_EVENT_CLICKED:
		{
			device_send_state_notify(DEVICE_STATE_WIFI_CONNECT, 0);
		}
		break;
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
	lv_obj_add_event_cb(ui->screen_btn_4, screen_btn_4_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->screen_cont_2, screen_img_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_3, screen_img_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_4, screen_img_3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_cont_5, screen_img_4_event_handler, LV_EVENT_ALL, NULL);

	lv_obj_add_event_cb(ui->screen_btn_sw_all, screen_btn_sw_all_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw1, screen_imgbtn_sw1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw2, screen_imgbtn_sw2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw3, screen_imgbtn_sw3_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_btn_connect, screen_btn_connec_event_handler, LV_EVENT_ALL, NULL);
}

void events_init(lv_ui* ui)
{

}
