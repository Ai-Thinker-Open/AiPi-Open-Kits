/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "log.h"


extern xQueueHandle queue;
extern TaskHandle_t https_Handle;

#define DBG_TAG "EVENT"
void events_init(lv_ui* ui)
{
}

static TimerHandle_t lod_timer;

static void Home_img_wifi_event_handler(lv_event_t* e)
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
				lv_scr_load_anim(guider_ui.config, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 20, true);
				guider_ui.Home_del = true;
				guider_ui.screen_type = false;
			}
		}
		break;
		default:
			break;
	}
}

static void Home_img_2_event_handler(lv_event_t* e)
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
				lv_scr_load_anim(guider_ui.config, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 20, false);
				guider_ui.Home_del = false;
				guider_ui.screen_type = false;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_Home(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->Home_img_wifi, Home_img_wifi_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Home_img_2, Home_img_2_event_handler, LV_EVENT_ALL, ui);
}

static uint8_t T_count = 0;
static uint32_t https_request_coun = 0;
static void loding_timers_cb(TimerHandle_t timers)
{

	lv_img_set_angle(guider_ui.config_img_loding, T_count*30*10);
	if (T_count>=11) T_count = 0;
	else
		T_count++;

	https_request_coun++;
	if (https_request_coun>=36000) {
		https_request_coun = 0;
		vTaskResume(https_Handle);
	}
}

static void config_btn_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	char* wifi_msg = pvPortMalloc(256);
	memset(wifi_msg, 0, 256);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			//判断SSID 输入框是否是空
			char* data_ssid = lv_textarea_get_text(ui->config_ta_ssid);
			char* data_pass = lv_textarea_get_text(ui->config_ta_pass);
			if (data_ssid[0]=='\0'||data_ssid[0]==' ')
			{
				ui_config_creat_msg_box(ui, "SSID 为空");
			}
			else {
				//显示连接加载
				lv_obj_clear_flag(guider_ui.config_cont_1, LV_OBJ_FLAG_HIDDEN);
				lv_label_set_text_fmt(ui->config_label_5, "正在连接:%s", data_ssid);
				lod_timer = xTimerCreate("xTimers", pdMS_TO_TICKS(100), pdTRUE, 0, loding_timers_cb);
				xTimerStart(lod_timer, pdMS_TO_TICKS(100));
				sprintf(wifi_msg, "{\"WiFi\":{\"ssid\":\"%s\",\"password\":\"%s\"}}", data_ssid, data_pass);
				xQueueSend(queue, wifi_msg, portMAX_DELAY);
			}
		}
		break;
		default:
			break;
	}
	vPortFree(wifi_msg);
}

static void config_img_h_event_handler(lv_event_t* e)
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
				if (guider_ui.Home_del == true)
					setup_scr_Home(&guider_ui);
				lv_scr_load_anim(guider_ui.Home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 20, true);
				guider_ui.config_del = true;
				guider_ui.screen_type = true;
			}
		}
		break;
		default:
			break;
	}
}

static void config_img_loding_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.config_cont_1, LV_OBJ_FLAG_HIDDEN);
			if (lod_timer!=NULL)
				xTimerDelete(lod_timer, pdMS_TO_TICKS(100));
		}
		break;
		default:
			break;
	}
}

static void config_img_3_event_handler(lv_event_t* e)
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
				if (guider_ui.Home_del == true)
					setup_scr_Home(&guider_ui);
				lv_scr_load_anim(guider_ui.Home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 20, true);
				guider_ui.config_del = true;
				guider_ui.screen_type = true;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_config(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->config_btn_1, config_btn_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->config_img_h, config_img_h_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->config_img_loding, config_img_loding_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->config_img_3, config_img_3_event_handler, LV_EVENT_ALL, ui);
}
