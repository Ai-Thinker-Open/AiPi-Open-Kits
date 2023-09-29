/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "dev_state.h"
#include  "dev_ir.h"

void events_init(lv_ui* ui)
{
}

static void Home_img_revc_event_handler(lv_event_t* e)
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
				lv_obj_clean(act_scr);
				if (guider_ui.screen_save_del == true)
					setup_scr_screen_save(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_save, LV_SCR_LOAD_ANIM_MOVE_LEFT, 50, 200, true);
				guider_ui.Home_del = true;
			}
		}
		break;
		default:
			break;
	}
}

static void Home_btn_save_event_handler(lv_event_t* e)
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
				lv_obj_clean(act_scr);
				if (guider_ui.screen_1_del == true)
					setup_scr_screen_1(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 20, true);
				guider_ui.Home_del = true;
			}


		}
		break;
		default:
			break;
	}
}

static void Home_btn_xieyi_event_handler(lv_event_t* e)
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
				lv_obj_clean(act_scr);
				if (guider_ui.ScreenHome_del == true)
					setup_scr_ScreenHome(&guider_ui);
				lv_scr_load_anim(guider_ui.ScreenHome, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 20, true);
				guider_ui.Home_del = true;
			}
		}
		break;
		default:
			break;
	}
}

static void Home_btn_test_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code) {
		case LV_EVENT_CLICKED:
		{
			dev_state_send_notify(DEV_STATE_LV_EVENT_TETS);
		}
		break;
		default:
			break;
	}
}

void events_init_Home(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->Home_img_revc, Home_img_revc_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Home_btn_save, Home_btn_save_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Home_btn_xieyi, Home_btn_xieyi_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->Home_btn_test, Home_btn_test_event_handler, LV_EVENT_ALL, ui);
}

static void ScreenHome_btn_2_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);
			dev_ir_type = lv_roller_get_selected(guider_ui.ScreenHome_roller_1);
			dev_state_send_notify(DEV_STATE_IR_TYPE_CHANGE);
			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				if (guider_ui.Home_del == true)
					setup_scr_Home(&guider_ui);
				lv_scr_load_anim(guider_ui.Home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 20, true);
				guider_ui.ScreenHome_del = true;
			}

		}
		break;
		default:
			break;
	}
}

void events_init_ScreenHome(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->ScreenHome_btn_2, ScreenHome_btn_2_event_handler, LV_EVENT_ALL, ui);
}

static void screen_save_btn_1_event_handler(lv_event_t* e)
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
				lv_obj_clean(act_scr);
				if (guider_ui.Home_del == true)
					setup_scr_Home(&guider_ui);
				lv_scr_load_anim(guider_ui.Home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 20, true);
				guider_ui.screen_save_del = true;

			}
		}
		break;
		default:
			break;
	}
}

void events_init_screen_save(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_save_btn_1, screen_save_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void screen_1_btn_ok_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			lv_obj_t* act_scr = lv_scr_act();
			lv_disp_t* d = lv_obj_get_disp(act_scr);

			ir_code_type = lv_dropdown_get_selected(guider_ui.screen_1_ddlist_cmd_list);

			if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
			{
				lv_obj_clean(act_scr);
				if (guider_ui.Home_del == true)
					setup_scr_Home(&guider_ui);
				lv_scr_load_anim(guider_ui.Home, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 20, true);
				guider_ui.screen_1_del = true;
			}

			dev_state_send_notify(DEV_STATE_IR_SAVE_CODE);
		}
		break;
		default:
			break;
	}
}

static void screen_1_btn_list_event_handler(lv_event_t* e)
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
				lv_obj_clean(act_scr);
				if (guider_ui.screen_save_del == true)
					setup_scr_screen_save(&guider_ui);
				lv_scr_load_anim(guider_ui.screen_save, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 20, true);
				guider_ui.screen_1_del = true;
			}
		}
		break;
		default:
			break;
	}
}

void events_init_screen_1(lv_ui* ui)
{
	lv_obj_add_event_cb(ui->screen_1_btn_ok, screen_1_btn_ok_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_1_btn_list, screen_1_btn_list_event_handler, LV_EVENT_ALL, ui);
}
