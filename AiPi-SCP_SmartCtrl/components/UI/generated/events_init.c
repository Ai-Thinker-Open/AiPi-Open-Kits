/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "log.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#define GBD_TAG "LV_EVENT"

static xTimerHandle loading_time;

extern lv_ui guider_ui;
static wifi_mgmr_scan_params_t config;
extern xQueueHandle queue;

static void TimerCallbackFunction_cb(TimerHandle_t xTimer)
{
	int	time_cout = (int)pvTimerGetTimerID(xTimer);
	lv_img_set_angle(guider_ui.screen_img_loading, time_cout*30*10);
	if (time_cout>=11) time_cout = 0;
	else
		time_cout++;
	vTimerSetTimerID(xTimer, (void*)time_cout);
}

void events_init(lv_ui* ui)
{
	loading_time = xTimerCreate("loading timer", 100/portTICK_PERIOD_MS, pdTRUE, 0, TimerCallbackFunction_cb);
}

static void screen_btn_scan_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("btn_scan LV_EVENT_CLICKED");
			lv_obj_clear_flag(guider_ui.screen_cont_loading, LV_OBJ_FLAG_HIDDEN); //显示加载图片
			xTimerStart(loading_time, 100/portTICK_PERIOD_MS);//开始旋转
			wifi_mgmr_sta_scan(&config); //扫描SSID
		}
		break;
		default:
			break;
	}
}

static void screen_btn_connect_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	char* buff = pvPortMalloc(64);
	char* wifi_msg = pvPortMalloc(256);
	memset(wifi_msg, 0, 256);
	memset(buff, 0, 64);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("btn_connect CLICKED");
			lv_obj_clear_flag(guider_ui.screen_cont_loading, LV_OBJ_FLAG_HIDDEN);

			xTimerStart(loading_time, 100/portTICK_PERIOD_MS);
			lv_dropdown_get_selected_str(ui->screen_ddlist_ssid_list, buff, 64);
			char* data_pass = lv_textarea_get_text(ui->screen_ta_pass);
			// lv_dropdown_set_options();
			LOG_I("src_home_ddlist_1 selected_str HEX;%02X", buff[0]);
			sprintf(wifi_msg, "{\"WiFi\":{\"ssid\":\"%s\",\"password\":\"%s\"}}", buff, data_pass);
			xQueueSend(queue, wifi_msg, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
	vPortFree(buff);
	vPortFree(wifi_msg);
}

static void screen_img_rgb_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("img_rgb LV_EVENT_CLICKED:screen is Ai-WB2 screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_rgb1_del == true)
					setup_scr_screen_rgb1(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_rgb1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 100, true);
				guider_ui.screen_del = true;
				guider_ui.screen_type = 1;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_img_wb2_open_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.screen_img_wb2_open, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.screen_img_wb2_close, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_wb2_close_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.screen_img_wb2_close, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.screen_img_wb2_open, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_rgb1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("img_rgb1 LV_EVENT_CLICKED:screen is Ai-M61 screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_rgb2_del == true)
					setup_scr_screen_rgb2(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_rgb2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 100, true);
				guider_ui.screen_del = true;
				guider_ui.screen_type = 2;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_img_m62_open_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_img_m62_close, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_img_m62_open, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_m62_close_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.screen_img_m62_close, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.screen_img_m62_open, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_rgb3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("img_rgb3 LV_EVENT_CLICKED:screen is BW16 screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_rgb3_del == true)
					setup_scr_screen_rgb3(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_rgb3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 100, true);
				guider_ui.screen_del = true;
				guider_ui.screen_type = 3;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_img_bw16_open_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.screen_img_bw16_close, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.screen_img_bw16_open, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_bw16_close_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.screen_img_bw16_close, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.screen_img_bw16_open, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_img_loading_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("img_loading LV_EVENT_CLICKED");
			lv_obj_add_flag(guider_ui.screen_cont_loading, LV_OBJ_FLAG_HIDDEN);
			xTimerStop(loading_time, 100/portTICK_PERIOD_MS);
		}
		break;
		default:
			break;
	}
}

void events_init_screen(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_btn_scan, screen_btn_scan_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_btn_connect, screen_btn_connect_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_rgb, screen_img_rgb_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_wb2_open, screen_img_wb2_open_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_wb2_close, screen_img_wb2_close_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_rgb1, screen_img_rgb1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_m62_open, screen_img_m62_open_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_m62_close, screen_img_m62_close_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_rgb3, screen_img_rgb3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_bw16_open, screen_img_bw16_open_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_bw16_close, screen_img_bw16_close_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_img_loading, screen_img_loading_event_handler, LV_EVENT_ALL, ui);
}

static void screen_rgb1_imgbtn_brack_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_brack1 LV_EVENT_CLICKED:screen is HOME screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 100, true);
				guider_ui.screen_rgb1_del = true;
				guider_ui.screen_type = 0;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_rgb1_wb2_sw_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
		{
			lv_obj_add_flag(guider_ui.screen_rgb1_cpicker_wb2_rgb, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_rgb1_cpicker_wb2_rgb_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_color_t wb2_rgb;
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED: //被按住
		{
			wb2_rgb = lv_colorwheel_get_rgb(guider_ui.screen_rgb1_cpicker_wb2_rgb);
			LOG_I("wb2 RGB: %02x%02X%02X", wb2_rgb.ch.red, wb2_rgb.ch.green, wb2_rgb.ch.blue);
		}
		break;

		default:
			break;
	}
}

void events_init_screen_rgb1(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_rgb1_imgbtn_brack, screen_rgb1_imgbtn_brack_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb1_wb2_sw, screen_rgb1_wb2_sw_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb1_cpicker_wb2_rgb, screen_rgb1_cpicker_wb2_rgb_event_handler, LV_EVENT_ALL, ui);
}

static void screen_rgb2_imgbtn_brack2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_brack2 LV_EVENT_CLICKED:screen is HOME screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 100, true);
				guider_ui.screen_rgb2_del = true;
				guider_ui.screen_type = 0;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_rgb2_Ai_M62_sw_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
		{
			lv_obj_add_flag(guider_ui.screen_rgb2_cpicker_M62_rgb, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void screen_rgb2_cpicker_m62_rgb_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_color_t m62_rgb;
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED: //被按住
		{
			m62_rgb = lv_colorwheel_get_rgb(guider_ui.screen_rgb2_cpicker_M62_rgb);
			LOG_I("M62 RGB: %02x%02X%02X", m62_rgb.ch.red, m62_rgb.ch.green, m62_rgb.ch.blue);
		}
		break;

		default:
			break;
	}
}

void events_init_screen_rgb2(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_rgb2_imgbtn_brack2, screen_rgb2_imgbtn_brack2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb2_Ai_M62_sw, screen_rgb2_Ai_M62_sw_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb2_cpicker_M62_rgb, screen_rgb2_cpicker_m62_rgb_event_handler, LV_EVENT_ALL, ui);
}

static void screen_rgb3_imgbtn_brack3_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("iimgbtn_brack3 LV_EVENT_CLICKED:screen is HOME screen");
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 100, true);
				guider_ui.screen_rgb3_del = true;
				guider_ui.screen_type = 0;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_rgb3_bw16_sw_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
		{
			lv_obj_add_flag(guider_ui.screen_rgb3_cpicker_bw16_rgb, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}
static void screen_rgb3_cpicker_BW16_rgb_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_color_t BW16_rgb;
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED: //被按住
		{
			BW16_rgb = lv_colorwheel_get_rgb(guider_ui.screen_rgb3_cpicker_bw16_rgb);
			LOG_I("M62 RGB: %02x%02X%02X", BW16_rgb.ch.red, BW16_rgb.ch.green, BW16_rgb.ch.blue);
		}
		break;

		default:
			break;
	}
}

void events_init_screen_rgb3(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_rgb3_imgbtn_brack3, screen_rgb3_imgbtn_brack3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb3_bw16_sw, screen_rgb3_bw16_sw_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_rgb3_cpicker_bw16_rgb, screen_rgb3_cpicker_BW16_rgb_event_handler, LV_EVENT_ALL, ui);
}
