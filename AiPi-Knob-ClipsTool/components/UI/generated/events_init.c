/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

void events_init(lv_ui* ui)
{
}

static void screen_imgbtn_JY_event_handler(lv_event_t* e)
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
				if (guider_ui.screen_JY_del == true)
					setup_scr_screen_JY(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_JY, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 50, false);
				guider_ui.screen_del = false;
				guider_ui.screen_type = 2;
			}
		}
		break;
		default:
			break;
	}
}

static void screen_imgbtn_Pr_event_handler(lv_event_t* e)
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
				if (guider_ui.screen_Pr_del == true)
					setup_scr_screen_Pr(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_Pr, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 50, false);
				guider_ui.screen_del = false;
				guider_ui.screen_type = 1;
			}
		}
		break;
		default:
			break;
	}
}


void events_init_screen(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_imgbtn_JY, screen_imgbtn_JY_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_Pr, screen_imgbtn_Pr_event_handler, LV_EVENT_ALL, ui);

}

static void screen_Pr_img_pr_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_LONG_PRESSED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				lv_obj_clean(act_scr);
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_OVER_TOP, 200, 50, true);
				guider_ui.screen_Pr_del = true;
				guider_ui.screen_type = 0;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_screen_Pr(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_Pr_img_pr, screen_Pr_img_pr_event_handler, LV_EVENT_ALL, ui);
}

static void screen_JY_img_jy_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_LONG_PRESSED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				lv_obj_clean(act_scr);
				if (guider_ui.screen_del == true)
					setup_scr_screen(&guider_ui);
				lv_scr_load_anim(guider_ui.screen, LV_SCR_LOAD_ANIM_OVER_TOP, 200, 50, true);
				guider_ui.screen_JY_del = true;
				guider_ui.screen_type = 0;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_screen_JY(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_JY_img_jy, screen_JY_img_jy_event_handler, LV_EVENT_ALL, ui);
}
