/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
static uint8_t tmp_switch=0;
static uint8_t tmp_switch1=0;

void events_init(lv_ui *ui)
{
}

static void screen_img_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
        case LV_EVENT_CLICKED: {
			tmp_switch=!tmp_switch;
            lv_animimg_t *animimg1 = (lv_animimg_t *)guider_ui.screen_animimg_2;
            lv_anim_set_values(&animimg1->anim, tmp_switch, tmp_switch);
            lv_animimg_start(guider_ui.screen_animimg_2);
        } break;
        default:
            break;
    }
}

static void screen_img_3_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
        case LV_EVENT_CLICKED: {
			tmp_switch1=!tmp_switch1;
            lv_animimg_t *animimg1 = (lv_animimg_t *)guider_ui.screen_animimg_1;
            lv_anim_set_values(&animimg1->anim, tmp_switch1, tmp_switch1);
            lv_animimg_start(guider_ui.screen_animimg_1);
        } break;
        default:
            break;
    }
}

void events_init_screen(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_img_2, screen_img_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_img_3, screen_img_3_event_handler, LV_EVENT_ALL, ui);
}
