/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "chip_include.h"

void events_init(lv_ui *ui)
{
}

static void screen_open_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA1;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

static void screen_close_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA1;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

static void screen_wind_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA2;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

static void screen_mode_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA3;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

static void screen_downarrow_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA4;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

static void screen_uparrow_btn_event_handler(lv_event_t *e)
{
	uint16_t value = 0xA5;
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		xQueueSend(button_queue, &value, 0);
	}
	break;
	default:
		break;
	}
}

extern uint8_t arc_flag;
extern uint8_t arc_count;

static void screen_arc_1_btn_event_handler(lv_event_t *e)
{
	
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		arc_flag = 1;
		arc_count = 0;
	}
	break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_open_btn, screen_open_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_close_btn, screen_close_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_wind_btn, screen_wind_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_mode_btn, screen_mode_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_downarrow_btn, screen_downarrow_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_uparrow_btn, screen_uparrow_btn_event_handler, LV_EVENT_CLICKED, ui);
	lv_obj_add_event_cb(ui->screen_arc_1, screen_arc_1_btn_event_handler, LV_EVENT_VALUE_CHANGED, ui);
}
