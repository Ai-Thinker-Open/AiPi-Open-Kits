/**
 * @file setup_smarctrl.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-13
 *
 * @copyright Copyright (c) 2023
 *
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

lv_obj_t* g_kb_src_home;
void kb_src_home_event_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* kb = lv_event_get_target(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}
/**
 * @brief
 *
*/
__attribute__((unused))  void ta_src_home_event_cb(lv_event_t* e)
{

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* ta = lv_event_get_target(e);
    lv_obj_t* kb = lv_event_get_user_data(e);
    if (code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED)
    {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_move_foreground(kb);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
    if (code == LV_EVENT_CANCEL || code == LV_EVENT_DEFOCUSED)
    {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_move_background(kb);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}
/**
 * @brief
 *
 * @param ui
*/
void SmartCtrl_scr_set(lv_ui* ui)
{
    ui->src_home_imgbtn_openL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_imgbtn_openL, 74, 103-50);
    lv_obj_set_size(ui->src_home_imgbtn_openL, 80, 110);
    lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_openL, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_imgbtn_openL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Set style for src_home_imgbtn_openL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_openL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_openL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_openL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_openL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_openL, 255, LV_PART_MAIN|LV_STATE_CHECKED);

    //Hidden for widget src_home_imgbtn_openL
    lv_obj_add_flag(ui->src_home_imgbtn_openL, LV_OBJ_FLAG_HIDDEN);

    lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_RELEASED, NULL, &_diaodengopen_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_PRESSED, NULL, &_diaodengopen_s_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_diaodengopen_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_openL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_diaodengopen_s_alpha_80x110, NULL);
    lv_obj_add_flag(ui->src_home_imgbtn_openL, LV_OBJ_FLAG_CHECKABLE);

    //Write codes src_home_imgbtn_confiL
    ui->src_home_imgbtn_confiL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_imgbtn_confiL, 185+60, 34);
    lv_obj_set_size(ui->src_home_imgbtn_confiL, 25, 25);
    lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_confiL, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_imgbtn_confiL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 163, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Set style for src_home_imgbtn_confiL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_confiL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_confiL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_confiL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_confiL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_confiL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_RELEASED, NULL, &_configL_alpha_25x25, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_PRESSED, NULL, &_configset_alpha_25x25, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_configL_alpha_25x25, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_confiL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_configset_alpha_25x25, NULL);
    lv_obj_add_flag(ui->src_home_imgbtn_confiL, LV_OBJ_FLAG_CHECKABLE);

    //Write codes src_home_label_18
    ui->src_home_label_18 = lv_label_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_label_18, 178+60, 54);
    lv_obj_set_size(ui->src_home_label_18, 27, 22);
    lv_obj_set_scrollbar_mode(ui->src_home_label_18, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_18, "点击编辑");
    lv_label_set_long_mode(ui->src_home_label_18, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_18. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_18, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_18, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_18, &lv_font_simhei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_imgbtn_closeL
    ui->src_home_imgbtn_closeL = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_imgbtn_closeL, 74, 103-50);
    lv_obj_set_size(ui->src_home_imgbtn_closeL, 80, 110);
    lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_closeL, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_imgbtn_closeL, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Set style for src_home_imgbtn_closeL. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_closeL, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_closeL, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_closeL, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_closeL, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_closeL, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_RELEASED, NULL, &_diaodeng_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_PRESSED, NULL, &_diaodeng_s_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_diaodeng_alpha_80x110, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_closeL, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_diaodeng_s_alpha_80x110, NULL);
    lv_obj_add_flag(ui->src_home_imgbtn_closeL, LV_OBJ_FLAG_CHECKABLE);

    //Write codes src_home_label_14
    ui->src_home_label_14 = lv_label_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_label_14, 25+30, 10);
    lv_obj_set_size(ui->src_home_label_14, 156, 14);
    lv_obj_set_scrollbar_mode(ui->src_home_label_14, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_14, "mqtt diconnect");
    lv_label_set_long_mode(ui->src_home_label_14, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_14. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_14, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_14, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_14, lv_color_make(0xff, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_imgbtn_user
    ui->src_home_imgbtn_user = lv_imgbtn_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_imgbtn_user, 191+60, 6);
    lv_obj_set_size(ui->src_home_imgbtn_user, 20, 20);
    lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_user, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_imgbtn_user, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Set style for src_home_imgbtn_user. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_user, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_user, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_user, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_user, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_user, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_RELEASED, NULL, &_useropen_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_PRESSED, NULL, &_userclose_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_useropen_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_user, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_userclose_alpha_20x20, NULL);
    lv_obj_add_flag(ui->src_home_imgbtn_user, LV_OBJ_FLAG_CHECKABLE);

    //Write codes src_home_cont_3
    ui->src_home_cont_3 = lv_obj_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_cont_3, 8, 5);
    lv_obj_set_size(ui->src_home_cont_3, 205+60, 175);
    lv_obj_set_scrollbar_mode(ui->src_home_cont_3, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_cont_3. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_cont_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_cont_3, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_cont_3, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_cont_3, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_cont_3, lv_color_make(0x47, 0x47, 0x47), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_cont_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_style_border_opa(ui->src_home_cont_3, 87, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Hidden for widget src_home_cont_3
    lv_obj_add_flag(ui->src_home_cont_3, LV_OBJ_FLAG_HIDDEN);


    //Write codes src_home_cont_4
    ui->src_home_cont_4 = lv_obj_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_cont_4, 0, 0);
    lv_obj_set_size(ui->src_home_cont_4, 205+60, 24);
    lv_obj_set_scrollbar_mode(ui->src_home_cont_4, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_cont_4. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_cont_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_cont_4, lv_color_make(0x66, 0x66, 0x66), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_cont_4, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_cont_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_cont_4, 44, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_cont_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_cont_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_cont_4, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_cont_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_imgbtn_10
    ui->src_home_imgbtn_10 = lv_imgbtn_create(ui->src_home_cont_4);
    lv_obj_set_pos(ui->src_home_imgbtn_10, 172+56, 0);
    lv_obj_set_size(ui->src_home_imgbtn_10, 20, 20);
    lv_obj_set_scrollbar_mode(ui->src_home_imgbtn_10, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_imgbtn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_PRESSED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);

    //Set style for src_home_imgbtn_10. Part: LV_PART_MAIN, State: LV_STATE_CHECKED
    lv_obj_set_style_shadow_width(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(ui->src_home_imgbtn_10, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_spread(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->src_home_imgbtn_10, lv_color_make(0xFF, 0x33, 0xFF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor(ui->src_home_imgbtn_10, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_recolor_opa(ui->src_home_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->src_home_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_RELEASED, NULL, &_closeopen_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_PRESSED, NULL, &_closeopen1_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_closeopen_alpha_20x20, NULL);
    lv_imgbtn_set_src(ui->src_home_imgbtn_10, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_closeopen1_alpha_20x20, NULL);
    lv_obj_add_flag(ui->src_home_imgbtn_10, LV_OBJ_FLAG_CHECKABLE);

    //Write codes src_home_label_13
    ui->src_home_label_13 = lv_label_create(ui->src_home_cont_4);
    lv_obj_set_pos(ui->src_home_label_13, 3, 3);
    lv_obj_set_size(ui->src_home_label_13, 43, 11);
    lv_obj_set_scrollbar_mode(ui->src_home_label_13, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_13, "MQTT");
    lv_label_set_long_mode(ui->src_home_label_13, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_13. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_13, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_13, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_13, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_host
    ui->src_home_label_host = lv_label_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_label_host, 6, 40);
    lv_obj_set_size(ui->src_home_label_host, 29, 12);
    lv_obj_set_scrollbar_mode(ui->src_home_label_host, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_host, "Host:");
    lv_label_set_long_mode(ui->src_home_label_host, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_host. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_host, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_host, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_host, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_host, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_host, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_username
    ui->src_home_label_username = lv_label_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_label_username, 3, 82-10);
    lv_obj_set_size(ui->src_home_label_username, 63, 13);
    lv_obj_set_scrollbar_mode(ui->src_home_label_username, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_username, "UserName:");
    lv_label_set_long_mode(ui->src_home_label_username, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_username. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_username, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_username, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_username, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_username, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_username, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_port
    ui->src_home_label_port = lv_label_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_label_port, 129+60, 40);
    lv_obj_set_size(ui->src_home_label_port, 29, 12);
    lv_obj_set_scrollbar_mode(ui->src_home_label_port, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_port, "Port:");
    lv_label_set_long_mode(ui->src_home_label_port, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_port. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_port, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_port, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_port, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_port, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_port, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_passworld
    ui->src_home_label_passworld = lv_label_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_label_passworld, 2, 124-10-5);
    lv_obj_set_size(ui->src_home_label_passworld, 63, 13);
    lv_obj_set_scrollbar_mode(ui->src_home_label_passworld, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_passworld, "Passworld:");
    lv_label_set_long_mode(ui->src_home_label_passworld, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_passworld. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_passworld, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_passworld, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_passworld, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_passworld, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_passworld, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_ta_mqHost
    ui->src_home_ta_mqHost = lv_textarea_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_ta_mqHost, 35, 33);
    lv_obj_set_size(ui->src_home_ta_mqHost, 94+60, 22);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_mqHost, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_mqHost. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqHost, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqHost, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqHost, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqHost, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_mqHost, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_mqHost, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_mqHost, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_mqHost, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_mqHost, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_mqHost, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_mqHost, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_mqHost, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_mqHost. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqHost, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqHost, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqHost, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_mqHost, "broker.emqx.io");

    //use keyboard on src_home_ta_mqHost
    lv_obj_add_event_cb(ui->src_home_ta_mqHost, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_ta_mqPort
    ui->src_home_ta_mqPort = lv_textarea_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_ta_mqPort, 156+60, 34);
    lv_obj_set_size(ui->src_home_ta_mqPort, 33, 23);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_mqPort, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_mqPort. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqPort, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqPort, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqPort, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqPort, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_mqPort, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_mqPort, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_mqPort, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_mqPort, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_mqPort, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_mqPort, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_mqPort, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_mqPort, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_mqPort, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_mqPort. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqPort, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqPort, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqPort, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_mqPort, "1883");

    //use keyboard on src_home_ta_mqPort
    lv_obj_add_event_cb(ui->src_home_ta_mqPort, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_ta_mqUsername
    ui->src_home_ta_mqUsername = lv_textarea_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_ta_mqUsername, 63, 71-5);
    lv_obj_set_size(ui->src_home_ta_mqUsername, 128, 22);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_mqUsername, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_mqUsername. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqUsername, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqUsername, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqUsername, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqUsername, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_mqUsername, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_mqUsername, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_mqUsername, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_mqUsername, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_mqUsername, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_mqUsername, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_mqUsername, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_mqUsername, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_mqUsername. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqUsername, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqUsername, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqUsername, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_mqUsername, "AiPi_Dashboard");

    //use keyboard on src_home_ta_mqUsername
    lv_obj_add_event_cb(ui->src_home_ta_mqUsername, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_ta_mqtt_pass
    ui->src_home_ta_mqtt_pass = lv_textarea_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_ta_mqtt_pass, 63, 113-10-3);
    lv_obj_set_size(ui->src_home_ta_mqtt_pass, 128, 22);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_mqtt_pass, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_mqtt_pass. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqtt_pass, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x41, 0x41, 0x41), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_mqtt_pass, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_mqtt_pass, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x5e, 0x5e, 0x5e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_mqtt_pass, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_mqtt_pass, 124, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_mqtt_pass, lv_color_make(0xe6, 0xe6, 0xe6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_mqtt_pass, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_mqtt_pass, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_mqtt_pass, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_mqtt_pass. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_mqtt_pass, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_mqtt_pass, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_mqtt_pass, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_mqtt_pass, "123456");

    //use keyboard on src_home_ta_mqtt_pass
    lv_obj_add_event_cb(ui->src_home_ta_mqtt_pass, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_btn_connect_mqtt
    ui->src_home_btn_connect_mqtt = lv_btn_create(ui->src_home_cont_3);
    lv_obj_set_pos(ui->src_home_btn_connect_mqtt, 73+30, 151-15);
    lv_obj_set_size(ui->src_home_btn_connect_mqtt, 51, 26);
    lv_obj_set_scrollbar_mode(ui->src_home_btn_connect_mqtt, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_btn_connect_mqtt. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_btn_connect_mqtt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_btn_connect_mqtt, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_btn_connect_mqtt, 90, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_btn_connect_mqtt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_btn_connect_mqtt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_btn_connect_mqtt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_btn_connect_mqtt, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_btn_connect_mqtt, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_btn_connect_mqtt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    ui->src_home_btn_connect_mqtt_label = lv_label_create(ui->src_home_btn_connect_mqtt);
    lv_label_set_text(ui->src_home_btn_connect_mqtt_label, "连接");
    lv_obj_set_style_pad_all(ui->src_home_btn_connect_mqtt, 0, LV_STATE_DEFAULT);
    lv_obj_align(ui->src_home_btn_connect_mqtt_label, LV_ALIGN_CENTER, 0, 0);

    //Write codes src_home_cont_6
    ui->src_home_cont_6 = lv_obj_create(ui->src_home_tabview_1_SmartCtrl);
    lv_obj_set_pos(ui->src_home_cont_6, 10, 30);
    lv_obj_set_size(ui->src_home_cont_6, 152+90, 150);
    lv_obj_set_scrollbar_mode(ui->src_home_cont_6, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_cont_6. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_cont_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_cont_6, lv_color_make(0x3f, 0x3f, 0x3f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_cont_6, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_cont_6, 238, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_cont_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_cont_6, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Hidden for widget src_home_cont_6
    lv_obj_add_flag(ui->src_home_cont_6, LV_OBJ_FLAG_HIDDEN);

    //Write codes src_home_label_15
    ui->src_home_label_15 = lv_label_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_label_15, 70-36, 19);
    lv_obj_set_size(ui->src_home_label_15, 41, 15);
    lv_obj_set_scrollbar_mode(ui->src_home_label_15, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_15, "Topic:");
    lv_label_set_long_mode(ui->src_home_label_15, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_15. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_15, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_15, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_16
    ui->src_home_label_16 = lv_label_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_label_16, 0, 83);
    lv_obj_set_size(ui->src_home_label_16, 80, 15);
    lv_obj_set_scrollbar_mode(ui->src_home_label_16, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_16, "关闭时发送:");
    lv_label_set_long_mode(ui->src_home_label_16, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_16. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_16, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_16, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_16, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_ta_topic
    ui->src_home_ta_topic = lv_textarea_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_ta_topic, 75, 17);
    lv_obj_set_size(ui->src_home_ta_topic, 142, 20);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_topic, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_topic. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_topic, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_topic, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_topic, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_topic, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_topic, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_topic, 120, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_topic, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_topic, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_topic, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_topic, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_topic. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_topic, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_topic, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_topic, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_topic, "dev/topic");

    //use keyboard on src_home_ta_topic
    lv_obj_add_event_cb(ui->src_home_ta_topic, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_ta_msg_close
    ui->src_home_ta_msg_close = lv_textarea_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_ta_msg_close, 75, 50+28);
    lv_obj_set_size(ui->src_home_ta_msg_close, 142, 30);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_msg_close, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_msg_close. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_msg_close, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_msg_close, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_msg_close, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_msg_close, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_msg_close, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_msg_close, 118, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_msg_close, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_msg_close, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_msg_close, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_msg_close, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_msg_close. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_msg_close, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_msg_close, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_msg_close, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_msg_close, "{\"status\":0}");

    //use keyboard on src_home_ta_msg_close
    lv_obj_add_event_cb(ui->src_home_ta_msg_close, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);

    //Write codes src_home_btn_topicOK
    ui->src_home_btn_topicOK = lv_btn_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_btn_topicOK, 40+50, 115);
    lv_obj_set_size(ui->src_home_btn_topicOK, 60, 24);
    lv_obj_set_scrollbar_mode(ui->src_home_btn_topicOK, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_btn_topicOK. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_btn_topicOK, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_btn_topicOK, lv_color_make(0x12, 0x5e, 0x95), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_btn_topicOK, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_btn_topicOK, 138, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_btn_topicOK, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_btn_topicOK, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_btn_topicOK, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_btn_topicOK, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    ui->src_home_btn_topicOK_label = lv_label_create(ui->src_home_btn_topicOK);
    lv_label_set_text(ui->src_home_btn_topicOK_label, "确定");
    lv_obj_set_style_pad_all(ui->src_home_btn_topicOK, 0, LV_STATE_DEFAULT);
    lv_obj_align(ui->src_home_btn_topicOK_label, LV_ALIGN_CENTER, 0, 0);

    //Write codes src_home_label_17
    ui->src_home_label_17 = lv_label_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_label_17, 23+50, 3);
    lv_obj_set_size(ui->src_home_label_17, 100, 15);
    lv_obj_set_scrollbar_mode(ui->src_home_label_17, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_17, "智能灯控制配置");
    lv_label_set_long_mode(ui->src_home_label_17, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_17. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_17, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_17, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_17, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_label_19
    ui->src_home_label_19 = lv_label_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_label_19, 0, 55);
    lv_obj_set_size(ui->src_home_label_19, 80, 15);
    lv_obj_set_scrollbar_mode(ui->src_home_label_19, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui->src_home_label_19, "打开时发送:");
    lv_label_set_long_mode(ui->src_home_label_19, LV_LABEL_LONG_WRAP);

    //Set style for src_home_label_19. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_label_19, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_label_19, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_label_19, &lv_font_simhei_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes src_home_ta_msg_open
    ui->src_home_ta_msg_open = lv_textarea_create(ui->src_home_cont_6);
    lv_obj_set_pos(ui->src_home_ta_msg_open, 75, 62-20);
    lv_obj_set_size(ui->src_home_ta_msg_open, 142, 30);
    lv_obj_set_scrollbar_mode(ui->src_home_ta_msg_open, LV_SCROLLBAR_MODE_OFF);

    //Set style for src_home_ta_msg_open. Part: LV_PART_MAIN, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_msg_open, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->src_home_ta_msg_open, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->src_home_ta_msg_open, lv_color_make(0x21, 0x95, 0xf6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->src_home_ta_msg_open, lv_color_make(0x49, 0x49, 0x49), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->src_home_ta_msg_open, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->src_home_ta_msg_open, 118, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->src_home_ta_msg_open, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->src_home_ta_msg_open, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->src_home_ta_msg_open, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->src_home_ta_msg_open, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Set style for src_home_ta_msg_open. Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT
    lv_obj_set_style_radius(ui->src_home_ta_msg_open, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->src_home_ta_msg_open, lv_color_make(0x21, 0x95, 0xf6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->src_home_ta_msg_open, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_textarea_set_text(ui->src_home_ta_msg_open, "{\"status\":1}");

    //use keyboard on src_home_ta_msg_open
    lv_obj_add_event_cb(ui->src_home_ta_msg_open, ta_src_home_event_cb, LV_EVENT_ALL, g_kb_src_home);



}