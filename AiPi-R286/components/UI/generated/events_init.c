/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "device.h"

void events_init(lv_ui* ui)
{
}

static void screen_imgbtn_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.config_del == true)
					setup_scr_config(&guider_ui);
				lv_scr_load_anim(guider_ui.config, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 20, true);
				guider_ui.screen_del = true;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_sw3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_imgbtn_on3, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_imgbtn_sw3, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw3->off(r2_device->relay_sw3);
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_on3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.screen_imgbtn_on3, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.screen_imgbtn_sw3, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw3->on(r2_device->relay_sw3);
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_sw2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_imgbtn_on2, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_imgbtn_sw2, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw2->off(r2_device->relay_sw2);
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_on2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_imgbtn_sw2, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_imgbtn_on2, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw2->on(r2_device->relay_sw2);
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_sw1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_imgbtn_on1, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_imgbtn_sw1, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw1->off(r2_device->relay_sw1);
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_on1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_imgbtn_sw1, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_imgbtn_on1, LV_OBJ_FLAG_HIDDEN);
			r2_device->relay_sw1->on(r2_device->relay_sw1);
		}
		break;
		default:
			break;
	}
}

void events_init_screen(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_cont_title, screen_imgbtn_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_titlebg, screen_imgbtn_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw3, screen_imgbtn_sw3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_on3, screen_imgbtn_on3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw2, screen_imgbtn_sw2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_on2, screen_imgbtn_on2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_sw1, screen_imgbtn_sw1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_on1, screen_imgbtn_on1_event_handler, LV_EVENT_ALL, ui);
}

static void config_btn_wificonfig_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			Create_tips(ui, "配网程序已启动，请打开手机App进行配网");
		}
		break;
		default:
			break;
	}
}

static void config_btn_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 20, true);
				guider_ui.config_del = true;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_config(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->config_btn_wificonfig, config_btn_wificonfig_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->config_cont_title1, config_btn_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->config_img_bg1, config_btn_1_event_handler, LV_EVENT_ALL, ui);
}
