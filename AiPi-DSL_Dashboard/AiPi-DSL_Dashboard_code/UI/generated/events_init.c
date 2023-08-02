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
#include "timers.h"
#include "lvgl.h"
#include "log.h"
#include "bflb_uart.h"
#include "voice_uart.h"
#include "ble_hid_dev.h"
#include "user_mqtt.h"
#define DBG_TAG "LV EVENT"

TimerHandle_t loading_time;

extern xQueueHandle queue;
extern QueueHandle_t ble_hid_queue;
static void loading_timer_cb(TimerHandle_t timer)
{
	int	time_cout = (int)pvTimerGetTimerID(timer);
	lv_img_set_angle(guider_ui.src_home_img_loding, time_cout*30*10);

	if (time_cout>=11) time_cout = 0;
	else
		time_cout++;
	vTimerSetTimerID(timer, (void*)time_cout);
}

void events_init(lv_ui* ui)
{
	loading_time = xTimerCreate("loading_time", pdMS_TO_TICKS(100), pdTRUE, 0, loading_timer_cb);
	if (loading_time!=NULL)LOG_I("loading time create OK");
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

			if (mqtt_app_publish(lv_textarea_get_text(guider_ui.src_home_ta_topic), lv_textarea_get_text(guider_ui.src_home_ta_msg_close), 0)!=0)
			{
				lv_label_set_text(guider_ui.src_home_label_14, "mqtt disconnect");
				lv_obj_set_style_text_color(guider_ui.src_home_label_14, lv_color_make(0xff, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
			}
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
	lv_ui* ui = (lv_ui*)lv_event_get_user_data(e);
	switch (code)
	{
		case LV_EVENT_LONG_PRESSED:
		{
			lv_obj_clear_flag(ui->src_home_cont_6, LV_OBJ_FLAG_HIDDEN);
		}
		break;
		case LV_EVENT_CLICKED:
		{
			lv_obj_add_flag(ui->src_home_imgbtn_closeL, LV_OBJ_FLAG_HIDDEN);
			lv_obj_clear_flag(ui->src_home_imgbtn_openL, LV_OBJ_FLAG_HIDDEN);

			if (mqtt_app_publish(lv_textarea_get_text(ui->src_home_ta_topic), lv_textarea_get_text(ui->src_home_ta_msg_open), 0)!=0)
			{
				lv_label_set_text(ui->src_home_label_14, "mqtt disconnect");
				lv_obj_set_style_text_color(ui->src_home_label_14, lv_color_make(0xff, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
			}
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
			LOG_I("btn_connect_mqtt CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
			xTimerStart(loading_time, 100/portTICK_PERIOD_MS);

			if (!mqtt_start_connect(lv_textarea_get_text(guider_ui.src_home_ta_mqHost), atoi(lv_textarea_get_text(guider_ui.src_home_ta_mqPort)), lv_textarea_get_text(guider_ui.src_home_ta_mqUsername), lv_textarea_get_text(guider_ui.src_home_ta_mqPort))) {
				lv_event_send(guider_ui.src_home_img_loding, LV_EVENT_CLICKED, NULL);
				lv_event_send(guider_ui.src_home_imgbtn_10, LV_EVENT_CLICKED, NULL);
				lv_label_set_text_fmt(guider_ui.src_home_label_14, "%s:%s", lv_textarea_get_text(guider_ui.src_home_ta_mqHost), lv_textarea_get_text(guider_ui.src_home_ta_mqPort));
				lv_obj_set_style_text_color(guider_ui.src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
			}
			else {
				lv_label_set_text(guider_ui.src_home_label_14, "mqtt disconnect");
				lv_obj_set_style_text_color(guider_ui.src_home_label_14, lv_color_make(0xff, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
			}

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
			mqtt_app_subscribe("sub", 0);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_stop_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_MISIC_STOP;
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_stop CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_satrt, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_stop, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_satrt_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_MISIC_PLAY;
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_satrt CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_stop, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_satrt, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_voice_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_VOL_MUTE;
	static struct bflb_device_s* uartx;
	uartx = bflb_device_get_by_name("uart1");
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_voice CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
			bflb_uart_put(uartx, user_data[UART_CMD_VOL_MUTE_OK].uart_data.data, 4);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_novoice_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_VOL_MUTE;
	static struct bflb_device_s* uartx;
	uartx = bflb_device_get_by_name("uart1");
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
			bflb_uart_put(uartx, user_data[UART_CMD_VOL_MUTE_NG].uart_data.data, 4);
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
			lv_obj_clear_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
			xTimerStart(loading_time, 100/portTICK_PERIOD_MS);
			lv_dropdown_get_selected_str(ui->src_home_ddlist_1, buff, 64);
			char* data_pass = lv_textarea_get_text(ui->src_home_ta_1);
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

static void src_home_img_loding_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("img_loding CLICKED");
			lv_obj_add_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
			xTimerStop(loading_time, 100/portTICK_PERIOD_MS);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_dashboard(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = (hid_key_num_t)lv_event_get_user_data(e);
	static struct bflb_device_s* uartx;
	uartx = bflb_device_get_by_name("uart1");
	switch (code) {
		case LV_EVENT_CLICKED:
		{
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
			switch (hid_key_num)
			{
				case HID_KEY_NUMBLE_MISIC_NEXT:
				case HID_KEY_NUMBLE_MISIC_PAST:
					bflb_uart_put(uartx, user_data[UART_CMD_MUSIC_NEXT].uart_data.data, 4);
					break;
				default:
					break;
			}
		}
		break;

		default:
			break;
	}
}

static void src_home_imgbtn_vol_I_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_VOL_UP;
	switch (code) {
		case LV_EVENT_CLICKED:
		{
			LOG_I("imgbtn_vol_I CLICKED");

			lv_obj_clear_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		case LV_EVENT_LONG_PRESSED_REPEAT:
		{
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
}

static void src_home_imgbtn_vol_n_cb(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	hid_key_num_t hid_key_num = HID_KEY_NUMBLE_VOL_DOWN;
	switch (code) {
		case LV_EVENT_PRESSED:
		{
			LOG_I("imgbtn_vol_n CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_imgbtn_voice, LV_OBJ_FLAG_HIDDEN);
			lv_obj_add_flag(guider_ui.src_home_imgbtn_novoice, LV_OBJ_FLAG_HIDDEN);
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		case LV_EVENT_LONG_PRESSED_REPEAT:
		{
			xQueueSend(ble_hid_queue, &hid_key_num, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
}
/**
 * @brief 启动搜索
 *
 * @param e
*/
static void src_home_imgbtn_scan_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	char* scan_msg = pvPortMalloc(128);
	memset(scan_msg, 0, 128);

	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			LOG_I("btn_scan CLICKED");
			lv_obj_clear_flag(guider_ui.src_home_cont_5, LV_OBJ_FLAG_HIDDEN);
			xTimerStart(loading_time, 100/portTICK_PERIOD_MS);
			sprintf(scan_msg, "{\"wifi_scan\":{\"status\":1}}");
			xQueueSend(queue, scan_msg, portMAX_DELAY);
		}
		break;
		default:
			break;
	}
	vPortFree(scan_msg);
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

	//搜索
	lv_obj_add_event_cb(ui->src_home_btn_scan, src_home_imgbtn_scan_event_handler, LV_EVENT_ALL, ui);
	//快捷按键

	lv_obj_add_event_cb(ui->src_home_imgbtn_vol_I, src_home_imgbtn_vol_I_cb, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->src_home_imgbtn_vol_n, src_home_imgbtn_vol_n_cb, LV_EVENT_ALL, ui);

	lv_obj_add_event_cb(ui->src_home_imgbtn_1, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_A);
	lv_obj_add_event_cb(ui->src_home_imgbtn_2, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_B);
	lv_obj_add_event_cb(ui->src_home_imgbtn_3, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_C);
	lv_obj_add_event_cb(ui->src_home_imgbtn_4, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_D);
	lv_obj_add_event_cb(ui->src_home_imgbtn_5, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_F);
	lv_obj_add_event_cb(ui->src_home_imgbtn_6, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_E);
	lv_obj_add_event_cb(ui->src_home_imgbtn_7, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_MISIC_NEXT);
	lv_obj_add_event_cb(ui->src_home_imgbtn_8, src_home_imgbtn_dashboard, LV_EVENT_ALL, (void*)HID_KEY_NUMBLE_MISIC_PAST);

}
