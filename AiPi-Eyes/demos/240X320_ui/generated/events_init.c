/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "FreeRTOS.h"
#include  "queue.h"
#include "cJSON.h"


extern xQueueHandle queue;
void events_init(lv_ui* ui)
{
}

static void cont_4_img_connet_event_handler(lv_event_t* e)
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
				// lv_obj_clean(act_scr);
				if (guider_ui.WiFi_config_del == true)
					setup_scr_WiFi_config(&guider_ui);
				lv_scr_load_anim(guider_ui.WiFi_config, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 50, true);
				guider_ui.cont_4_del = true;
				guider_ui.screen_type = false;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_cont_4(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->cont_4_img_connet, cont_4_img_connet_event_handler, LV_EVENT_ALL, ui);
}

static void WiFi_config_imgbtn_1_event_handler(lv_event_t* e)
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
				// lv_obj_clean(act_scr);
				if (guider_ui.cont_4_del == true)
					setup_scr_cont_4(&guider_ui);
				lv_scr_load_anim(guider_ui.cont_4, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 50, true);
				guider_ui.WiFi_config_del = true;
				guider_ui.screen_type = true;
			}

		}
		break;
		default:
			break;
	}
}

static void WiFi_config_btn_1_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	char* buff = NULL;
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			buff = pvPortMalloc(256);
			if (lv_textarea_get_text(ui->WiFi_config_ta_1)==NULL) {
				printf("SSID is NULL,please enter");
				break;
			}
			// printf("%s:ssid:%s  password:%s\r\n", __func__, lv_textarea_get_text(ui->WiFi_config_ta_1), lv_textarea_get_text(ui->WiFi_config_ta_2));
			memset(ui->ssid, 0, 32);
			memset(ui->password, 0, 64);
			strcpy(ui->ssid, lv_textarea_get_text(ui->WiFi_config_ta_1));
			strcpy(ui->password, lv_textarea_get_text(ui->WiFi_config_ta_2));
			sprintf(buff, "{\"WiFi\":{\"ssid\":\"%s\",\"password\":\"%s\"}}", lv_textarea_get_text(ui->WiFi_config_ta_1), lv_textarea_get_text(ui->WiFi_config_ta_2));
			xQueueSend(queue, (void*)buff, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
}

void events_init_WiFi_config(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->WiFi_config_imgbtn_1, WiFi_config_imgbtn_1_event_handler, LV_EVENT_ALL, ui);
	// lv_obj_add_event_cb(ui->WiFi_config_ta_1, WiFi_config_ta_1_event_handler, LV_EVENT_ALL, ui);
	// lv_obj_add_event_cb(ui->WiFi_config_ta_2, WiFi_config_ta_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->WiFi_config_btn_1, WiFi_config_btn_1_event_handler, LV_EVENT_ALL, ui);
}
