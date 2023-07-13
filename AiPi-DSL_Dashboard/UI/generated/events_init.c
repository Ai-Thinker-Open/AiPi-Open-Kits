/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lvgl.h"
#include "log.h"

#define DBG_TAG "LV EVENT"

void events_init(lv_ui* ui)
{
}

static void src_home_imgbtn_openL_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_LONG_PRESSED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_6, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.src_home_imgbtn_openL, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_closeL, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_confiL_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_6, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_closeL_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_LONG_PRESSED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_6, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.src_home_imgbtn_closeL, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_openL, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_user_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_3, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_10_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.src_home_cont_3, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_btn_connect_mqtt_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_btn_topicOK_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.src_home_cont_6, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_stop_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_satrt, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_stop, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_satrt_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_stop, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_satrt, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_voice_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_novoice_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_img_wifi_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		default:
			break;
	}
}

static void src_home_btn_connect_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_img_loding_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		default:
			break;
	}
}

static void src_home_ddlist_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	char* buff = pvPortMalloc(64);
	memset(buff, 0, 64);
	switch (code)
	{
		case LV_EVENT_VALUE_CHANGED:
		{
			lv_dropdown_get_selected_str(ui->src_home_ddlist_1, buff, 64);
			LOG_I("dropdown is :%s", buff);
		};
		break;
		default:
			break;
	}
	vPortFree(buff);
}

void events_init_src_home(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->src_home_imgbtn_openL, src_home_imgbtn_openL_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_confiL, src_home_imgbtn_confiL_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_closeL, src_home_imgbtn_closeL_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_user, src_home_imgbtn_user_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_10, src_home_imgbtn_10_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_btn_connect_mqtt, src_home_btn_connect_mqtt_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_btn_topicOK, src_home_btn_topicOK_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_stop, src_home_imgbtn_stop_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_satrt, src_home_imgbtn_satrt_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_voice, src_home_imgbtn_voice_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_novoice, src_home_imgbtn_novoice_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_img_wifi, src_home_img_wifi_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_btn_connect, src_home_btn_connect_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_img_loding, src_home_img_loding_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_ddlist_1, src_home_ddlist_1_event_handler, LV_EVENT_ALL, ui);
}
