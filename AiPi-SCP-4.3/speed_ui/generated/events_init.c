/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
static lv_timer_t  * task_meter;

static lv_timer_t * task_chart;

void events_init(lv_ui *ui)
{
}

static void speed_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_SCREEN_UNLOAD_START:
	{
		lv_timer_del(task_meter);
	}
		break;
	case LV_EVENT_SCREEN_LOADED:
	{
		task_meter = lv_timer_create(speed_meter_timer_cb, 100, &guider_ui);
	}
		break;
	default:
		break;
	}
}

static void speed_btn_next_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_t * act_scr = lv_scr_act();
		lv_disp_t * d = lv_obj_get_disp(act_scr);
		if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
		{
			if (guider_ui.record_del == true)
				setup_scr_record(&guider_ui);
			lv_scr_load_anim(guider_ui.record, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
			guider_ui.speed_del = true;
		}
	}
		break;
	default:
		break;
	}
}

void events_init_speed(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->speed, speed_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->speed_btn_next, speed_btn_next_event_handler, LV_EVENT_ALL, ui);
}

static void record_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_SCREEN_UNLOAD_START:
	{
		lv_timer_del(task_chart);
	}
		break;
	case LV_EVENT_SCREEN_LOADED:
	{
		task_chart = lv_timer_create(record_chart_timer_cb, 100, guider_ui.record_chart_board);
	}
		break;
	default:
		break;
	}
}

static void record_btn_before_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_t * act_scr = lv_scr_act();
		lv_disp_t * d = lv_obj_get_disp(act_scr);
		if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
		{
			if (guider_ui.speed_del == true)
				setup_scr_speed(&guider_ui);
			lv_scr_load_anim(guider_ui.speed, LV_SCR_LOAD_ANIM_NONE, 100, 100, true);
			guider_ui.record_del = true;
		}
	}
		break;
	default:
		break;
	}
}

void events_init_record(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->record, record_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->record_btn_before, record_btn_before_event_handler, LV_EVENT_ALL, ui);
}
