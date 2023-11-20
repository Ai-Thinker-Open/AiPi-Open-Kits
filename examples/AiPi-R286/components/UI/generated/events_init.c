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

			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY3_ON);
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

			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY3_OFF);
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
			// r2_device->relay_sw2->off(r2_device->relay_sw2);
			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY2_ON);
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
			// r2_device->relay_sw2->on(r2_device->relay_sw2);
			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY2_OFF);
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
			// r2_device->relay_sw1->off(r2_device->relay_sw1);
			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY1_ON);
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
			// r2_device->relay_sw1->on(r2_device->relay_sw1);
			aipi_r286_dev_state_update(AIPI_R286_STATE_DEV_REALAY1_OFF);
		}
		break;
		default:
			break;
	}
}


static void screen_img_aixingyun_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			//如果当前语言是中文,就切换到英文
			if (r2_device->language==AIPI_R286_LANGUAGE_ZH) {
				lv_obj_set_style_text_font(guider_ui.screen_ddlist_list1, &lv_font_montserrat_16, LV_PART_MAIN);
				lv_obj_set_style_text_font(guider_ui.screen_ddlist_list2, &lv_font_montserrat_16, LV_PART_MAIN);
				lv_obj_set_style_text_font(guider_ui.screen_ddlist_list3, &lv_font_montserrat_16, LV_PART_MAIN);
				lv_dropdown_set_options(guider_ui.screen_ddlist_list1, "Bedroom\nHall\nKitchen");
				lv_dropdown_set_options(guider_ui.screen_ddlist_list2, "Bedroom\nHall\nKitchen");
				lv_dropdown_set_options(guider_ui.screen_ddlist_list3, "Bedroom\nHall\nKitchen");
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list1, 0);
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list2, 1);
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list3, 2);
				r2_device->language = AIPI_R286_LANGUAGE_EN;
			}
			else {
				//切换到中文
				lv_obj_set_style_text_font(r2_device->ui->screen_ddlist_list1, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
				lv_obj_set_style_text_font(r2_device->ui->screen_ddlist_list2, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
				lv_obj_set_style_text_font(r2_device->ui->screen_ddlist_list3, &lv_font_MiSans_Demibold_20, LV_PART_MAIN|LV_STATE_DEFAULT);
				lv_dropdown_set_options(r2_device->ui->screen_ddlist_list1, "卧室\n大厅\n厨房");
				lv_dropdown_set_options(r2_device->ui->screen_ddlist_list2, "卧室\n大厅\n厨房");
				lv_dropdown_set_options(r2_device->ui->screen_ddlist_list3, "卧室\n大厅\n厨房");
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list1, 0);
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list2, 1);
				lv_dropdown_set_selected(guider_ui.screen_ddlist_list3, 2);
				r2_device->language = AIPI_R286_LANGUAGE_ZH;
			}
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
	lv_obj_add_event_cb(ui->screen_img_aixingyun, screen_img_aixingyun_event_handler, LV_EVENT_ALL, ui);
}

static void config_btn_wificonfig_event_handler(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_ui* ui = lv_event_get_user_data(e);
	switch (code)
	{
		case LV_EVENT_CLICKED:
		{
			Create_tips(ui, r2_device->language? "The distribution program has started. Please open the mobile app for distribution": "配网程序已启动，请打开手机App进行配网");
			aipi_r286_dev_state_update(AIPI_R286_STATE_CONFIG_START);
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
