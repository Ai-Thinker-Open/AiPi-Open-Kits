/*
 */

#include "lvgl.h"
#include <stdio.h>

#include "lv_example_pub.h"
#include "lv_example_image.h"


static lv_obj_t *temp_arc;
// static lv_obj_t *page;
static lv_obj_t *temp_wheel;
static lv_obj_t *label_A_C;
static time_out_count time_500ms;

static lv_obj_t *page_bg;
static lv_obj_t *page_control;
static lv_obj_t *page_brand;

static lv_obj_t *btn_power;
static lv_obj_t *btn_forward;
static lv_obj_t *btn_backward;
static lv_obj_t *btn_increase;
static lv_obj_t *btn_reduce;

typedef struct {
    const char *name;
    const char *label;
    lv_coord_t width;
    lv_coord_t height;
    lv_coord_t x_ofs;
    lv_coord_t y_ofs;
    const lv_img_dsc_t *icon;
    lv_color_t theme_color;
    lv_obj_t *btn_obj;
    lv_obj_t *label_obj;
} ui_btn_t;

static bool remote_Layer_enter_cb(void *layer);
static bool remote_Layer_exit_cb(void *layer);
static void remote_Layer_timer_cb(lv_timer_t *tmr);

lv_layer_t remote_Layer = {
    .lv_obj_name    = "remote_Layer",
    .lv_obj_parent  = NULL,
    .lv_obj_layer   = NULL,
    .lv_show_layer  = NULL,
    .enter_cb       = remote_Layer_enter_cb,
    .exit_cb        = remote_Layer_exit_cb,
    .timer_cb       = remote_Layer_timer_cb,
};


ui_btn_t remote_btn[] = {
    {"power",   "P",  60,40, 0, -80,NULL,LV_COLOR_MAKE(183, 183, 183), &btn_power},
    {"left",    "<",  60,40,-40,-20,NULL,LV_COLOR_MAKE(183, 183, 183), &btn_forward},
    {"right",   ">",  60,40, 40,-20,NULL,LV_COLOR_MAKE(183, 183, 183), &btn_backward},
    {"add",     "+",  60,40,-40, 40,NULL,LV_COLOR_MAKE(183, 183, 183), &btn_backward},
    {"reduce",  "-",  60,40, 40, 40,NULL,LV_COLOR_MAKE(183, 183, 183), &btn_backward},
};

static void remote_event_cb(lv_event_t *e)
{
    static uint8_t current = 0;

    lv_event_code_t code = lv_event_get_code(e);

    printf("remote_event_cb code:%d\r\n", code);
    if (LV_EVENT_FOCUSED == code) {
        lv_group_set_editing(lv_group_get_default(), true);
    } else if (LV_EVENT_KEY == code) {

        if (is_time_out(&time_500ms)) {
            uint32_t key = lv_event_get_key(e);

            if (LV_KEY_RIGHT == key) {
                if (current < sizeof(remote_btn)/sizeof(ui_btn_t) -1) {
                    current++;
                }
            } else {
                if (current > 0) {
                    current--;
                }
            }
        }
        for(int i=0; i < sizeof(remote_btn)/sizeof(ui_btn_t); i++)
        {
            if(remote_btn[i].btn_obj){
                lv_obj_clear_state(remote_btn[i].btn_obj, LV_STATE_FOCUS_KEY);
            }
        }
        printf("current:%d\r\n", current);
        if(remote_btn[current].btn_obj){
            lv_obj_add_state(remote_btn[current].btn_obj, LV_STATE_FOCUS_KEY);
            // lv_obj_set_style_border_color(remote_btn[current].btn_obj, lv_color_hex(0xF880F8), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_border_opa(remote_btn[current].btn_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_border_width(remote_btn[current].btn_obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        

    } else if (LV_EVENT_LONG_PRESSED == code) {
        lv_indev_wait_release(lv_indev_get_next(NULL));
        ui_remove_all_objs_from_encoder_group();
        lv_func_goto_layer(&menu_layer);
        // lv_obj_add_flag(page_control, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_clear_flag(page_brand, LV_OBJ_FLAG_HIDDEN);
    }
    else if (LV_EVENT_CLICKED == code) {
        // if(lv_obj_has_state(temp_btn, LV_STATE_CHECKED)){
        //     lv_obj_clear_state(temp_btn, LV_STATE_CHECKED);
        //     lv_label_set_text(label_A_C, "OFF");
        // }
        // else{
        //     lv_obj_add_state(temp_btn, LV_STATE_CHECKED);
        //     lv_label_set_text(label_A_C, "ON");
        // }
        if(remote_btn[current].btn_obj){
            // lv_obj_add_state(remote_btn[current].btn_obj, LV_STATE_CHECKED);
            // lv_obj_add_flag(remote_btn[current].btn_obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        }

    }
}
static void page_brand_event_cb(lv_event_t *e)
{
    static uint8_t current_opt = 0;

    lv_event_code_t code = lv_event_get_code(e);

    printf("page_brand_event_cb code:%d\r\n", code);
    if (LV_EVENT_FOCUSED == code) {
        lv_group_set_editing(lv_group_get_default(), true);
    } else if (LV_EVENT_KEY == code) {

        if (is_time_out(&time_500ms)) {
            uint32_t key = lv_event_get_key(e);
            if (LV_KEY_RIGHT == key) {
                if (current_opt < lv_roller_get_option_cnt(temp_wheel)) {
                    current_opt++;
                }
            } else {
                if (current_opt > 0) {
                    current_opt--;
                }
            }
            lv_roller_set_selected(temp_wheel, current_opt, LV_ANIM_ON);
            printf("current_opt:%d\r\n", current_opt);
        }
        

    } else if (LV_EVENT_LONG_PRESSED == code) {
        lv_indev_wait_release(lv_indev_get_next(NULL));
        ui_remove_all_objs_from_encoder_group();
        lv_func_goto_layer(&menu_layer);
    }
    else if (LV_EVENT_CLICKED == code) {
        lv_obj_add_flag(page_brand, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(page_control, LV_OBJ_FLAG_HIDDEN);
        // ui_remove_all_objs_from_encoder_group();//roll will add event default.
        // ui_add_obj_to_encoder_group(page_bg);
        printf("lv_obj_remove_event_cb:%d\r\n",lv_obj_remove_event_cb(page_bg, page_brand_event_cb));
        printf("lv_obj_remove_event_cb:%d\r\n",lv_obj_remove_event_cb(page_bg, page_brand_event_cb));
        printf("lv_obj_remove_event_cb:%d\r\n",lv_obj_remove_event_cb(page_bg, page_brand_event_cb));
        lv_obj_add_event_cb(page_bg, remote_event_cb, LV_EVENT_FOCUSED, NULL);
        lv_obj_add_event_cb(page_bg, remote_event_cb, LV_EVENT_KEY, NULL);
        lv_obj_add_event_cb(page_bg, remote_event_cb, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_add_event_cb(page_bg, remote_event_cb, LV_EVENT_CLICKED, NULL);
    }
}


static void mask_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK) {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected value: %s", buf);
    } else if (code == LV_EVENT_DRAW_MAIN_BEGIN) {


    } else if (code == LV_EVENT_DRAW_POST_END) {

    }
}

/**
 * Add a fade mask to roller.
 */
void lv_create_obj_roller(lv_obj_t *parent)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_bg_opa(&style, LV_OPA_0);
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_obj_add_style(lv_scr_act(), &style, 0);

    temp_wheel = lv_roller_create(parent);
    lv_obj_add_style(temp_wheel, &style, 0);
    printf("line_space:%d\r\n", lv_obj_get_style_text_line_space(temp_wheel, LV_PART_MAIN));
    lv_obj_set_style_text_line_space(temp_wheel, 40, LV_PART_MAIN);
    //lv_obj_set_style_border_width(temp_wheel, 10, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(temp_wheel, LV_OPA_TRANSP, LV_PART_SELECTED);

#if LV_FONT_MONTSERRAT_38
    lv_obj_set_style_text_font(temp_wheel, &lv_font_montserrat_38, LV_PART_SELECTED);
#endif
    lv_roller_set_options(temp_wheel,
                        "TCL\n"
                        "Sony\n"
                        "Samsung\n"
                        "Xiaomi\n"
                        "Vidda\n"
                        "Skyworth",
                          LV_ROLLER_MODE_NORMAL);

    lv_obj_align(temp_wheel, LV_ALIGN_CENTER, 0, 20);
    lv_roller_set_visible_row_count(temp_wheel, 3);
    // lv_obj_add_event_cb(temp_wheel, mask_event_cb, LV_EVENT_ALL, NULL);
}

void ui_remote_init(lv_obj_t *parent)
{
    page_bg = lv_obj_create(parent);
    lv_obj_set_size(page_bg, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_border_width(page_bg, 0, 0);
    lv_obj_set_style_radius(page_bg, 0, 0);
    lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_center(page_bg);
//page_control
    page_control = lv_obj_create(page_bg);
    lv_obj_remove_style_all(page_control);
    lv_obj_set_size(page_control, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(page_control, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *page_control_bg = lv_obj_create(page_control);
    lv_obj_set_width(page_control_bg, 240);
    lv_obj_set_height(page_control_bg, 240);
    lv_obj_set_x(page_control_bg, 0);
    lv_obj_set_y(page_control_bg, 0);
    lv_obj_set_align(page_control_bg, LV_ALIGN_CENTER);
    lv_obj_clear_flag(page_control_bg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(page_control_bg, 360, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(page_control_bg, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(page_control_bg, lv_color_hex(0xF880F8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(page_control_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(page_control_bg, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_blend_mode(page_control_bg, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(page_control_bg, 200, LV_PART_MAIN | LV_STATE_DEFAULT);


    for(int i=0; i < sizeof(remote_btn)/sizeof(ui_btn_t); i++)
    {
        
        remote_btn[i].btn_obj = lv_btn_create(page_control);
        lv_obj_set_width(remote_btn[i].btn_obj, remote_btn[i].width);
        lv_obj_set_height(remote_btn[i].btn_obj, remote_btn[i].height);
        lv_obj_add_flag(remote_btn[i].btn_obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_clear_flag(remote_btn[i].btn_obj, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(remote_btn[i].btn_obj, remote_btn[i].theme_color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(remote_btn[i].btn_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(remote_btn[i].btn_obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(remote_btn[i].btn_obj, LV_ALIGN_CENTER, remote_btn[i].x_ofs, remote_btn[i].y_ofs);
        // lv_obj_add_event_cb(remote_btn[i].btn_obj, remote_event_cb, LV_EVENT_CLICKED, NULL);

        remote_btn[i].label_obj = lv_label_create(page_control);
        lv_obj_set_style_text_color(remote_btn[i].label_obj, lv_color_hex(0x260676), 0);
        lv_obj_set_style_text_font(remote_btn[i].label_obj, &lv_font_montserrat_18, 0);
        lv_label_set_text(remote_btn[i].label_obj, remote_btn[i].label);
        lv_obj_align(remote_btn[i].label_obj, LV_ALIGN_CENTER, remote_btn[i].x_ofs, remote_btn[i].y_ofs);
    }
//page_brand
    page_brand = lv_obj_create(page_bg);
    lv_obj_remove_style_all(page_brand);
    lv_obj_set_size(page_brand, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(page_brand, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *page_brand_bg = lv_obj_create(page_brand);
    lv_obj_set_width(page_brand_bg, 240);
    lv_obj_set_height(page_brand_bg, 240);
    lv_obj_set_x(page_brand_bg, 0);
    lv_obj_set_y(page_brand_bg, 0);
    lv_obj_set_align(page_brand_bg, LV_ALIGN_CENTER);
    lv_obj_clear_flag(page_brand_bg, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(page_brand_bg, 360, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(page_brand_bg, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(page_brand_bg, lv_color_hex(0xF880F8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(page_brand_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(page_brand_bg, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_blend_mode(page_brand_bg, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(page_brand_bg, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_create_obj_roller(page_brand);

    lv_obj_add_event_cb(page_bg, page_brand_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(page_bg, page_brand_event_cb, LV_EVENT_KEY, NULL);
    lv_obj_add_event_cb(page_bg, page_brand_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(page_bg, page_brand_event_cb, LV_EVENT_CLICKED, NULL);
    ui_remove_all_objs_from_encoder_group();//roll will add event default.
    ui_add_obj_to_encoder_group(page_bg); 
    

    
    // lv_obj_add_event_cb(page_control, remote_event_cb, LV_EVENT_FOCUSED, NULL);
    // lv_obj_add_event_cb(page_control, remote_event_cb, LV_EVENT_KEY, NULL);
    // lv_obj_add_event_cb(page_control, remote_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    // lv_obj_add_event_cb(page_control, remote_event_cb, LV_EVENT_CLICKED, NULL);
    // ui_remove_all_objs_from_encoder_group();//roll will add event default.
    // ui_add_obj_to_encoder_group(page_control);

    lv_obj_add_flag(page_control, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(page_brand, LV_OBJ_FLAG_HIDDEN);
}

static bool remote_Layer_enter_cb(void *layer)
{
    bool ret = false;

    LV_LOG_USER("");
    lv_layer_t *create_layer = layer;
    if (NULL == create_layer->lv_obj_layer) {
        ret = true;
        create_layer->lv_obj_layer = lv_obj_create(lv_scr_act());
        lv_obj_remove_style_all(create_layer->lv_obj_layer);
        lv_obj_set_size(create_layer->lv_obj_layer, LV_HOR_RES, LV_VER_RES);

        ui_remote_init(create_layer->lv_obj_layer);
        set_time_out(&time_500ms, 100);
    }
    return ret;
}

static bool remote_Layer_exit_cb(void *layer)
{
    LV_LOG_USER("");
    return true;
}

static void remote_Layer_timer_cb(lv_timer_t *tmr)
{
    feed_clock_time();
}
