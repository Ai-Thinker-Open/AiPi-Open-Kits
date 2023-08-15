/*

 */

#include "lvgl.h"
#include <stdio.h>

#include "lv_example_pub.h"
#include "lv_example_image.h"

lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel100;
lv_obj_t * ui_Panel75;
lv_obj_t * ui_Panel50;
lv_obj_t * ui_Panel25;
lv_obj_t * ui_Image1;


static bool light_rgb_layer_enter_cb(void *layer);
static bool light_rgb_layer_exit_cb(void *layer);
static void light_rgb_layer_timer_cb(lv_timer_t *tmr);

typedef enum {
    LIGHT_CCK_WARM,
    LIGHT_CCK_COOL,
    LIGHT_CCK_MAX,
} LIGHT_CCK_TYPE;
typedef struct {
    uint8_t light_pwm;
    LIGHT_CCK_TYPE light_cck;
} light_set_attribute_t;
typedef struct {
    const lv_img_dsc_t *img_bg[2];

    const lv_img_dsc_t *img_pwm_25[2];
    const lv_img_dsc_t *img_pwm_50[2];
    const lv_img_dsc_t *img_pwm_75[2];
    const lv_img_dsc_t *img_pwm_100[2];
} ui_light_img_t;

static lv_obj_t *page;
static time_out_count time_20ms, time_500ms;

static lv_obj_t *img_light_bg, *label_pwm_set, *img_led;

static light_set_attribute_t light_set_conf, light_xor;

static const ui_light_img_t light_image = {
    {&light_warm_bg,     &light_cool_bg},
};

lv_layer_t light_rgb_Layer = {
    .lv_obj_name    = "light_rgb_Layer",
    .lv_obj_parent  = NULL,
    .lv_obj_layer   = NULL,
    .lv_show_layer  = NULL,
    .enter_cb       = light_rgb_layer_enter_cb,
    .exit_cb        = light_rgb_layer_exit_cb,
    .timer_cb       = light_rgb_layer_timer_cb,
};

static void light_rgb_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    printf("light_rgb_event_cb code:%d\r\n", code);
    if (LV_EVENT_FOCUSED == code) {
        lv_group_set_editing(lv_group_get_default(), true);
    } else if (LV_EVENT_KEY == code) {
        uint32_t key = lv_event_get_key(e);
        if (is_time_out(&time_500ms)) {
            if (LV_KEY_RIGHT == key) {
                if (light_set_conf.light_pwm < 100) {
                    light_set_conf.light_pwm += 25;
                }
            } else if (LV_KEY_LEFT == key) {
                if (light_set_conf.light_pwm > 0) {
                    light_set_conf.light_pwm -= 25;
                }
            }
        }
    } else if (LV_EVENT_CLICKED == code) {
        light_set_conf.light_cck = \
                                   (LIGHT_CCK_WARM == light_set_conf.light_cck) ? (LIGHT_CCK_COOL) : (LIGHT_CCK_WARM);
        set_light_warm_cool();
    } else if (LV_EVENT_LONG_PRESSED == code) {
        lv_indev_wait_release(lv_indev_get_next(NULL));
        ui_remove_all_objs_from_encoder_group();
        lv_func_goto_layer(&menu_layer);
        // void play_voice_close(void);
        // play_voice_close();
    }
}

void ui_light_rgb_init(lv_obj_t *parent)
{
    light_xor.light_pwm = 0xFF;
    light_xor.light_cck = LIGHT_CCK_MAX;

    light_set_conf.light_pwm = 50;
    light_set_conf.light_cck = LIGHT_CCK_WARM;

    page = lv_obj_create(parent);
    lv_obj_set_size(page, LV_HOR_RES, LV_VER_RES);
    //lv_obj_set_size(page, lv_obj_get_width(lv_obj_get_parent(page)), lv_obj_get_height(lv_obj_get_parent(page)));

    lv_obj_set_style_border_width(page, 0, 0);
    lv_obj_set_style_radius(page, 0, 0);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_center(page);

    img_light_bg = lv_img_create(page);
    lv_img_set_src(img_light_bg, &light_warm_bg);
    lv_obj_align(img_light_bg, LV_ALIGN_CENTER, 0, 0);

    ui_Panel100 = lv_obj_create(page);
    lv_obj_set_width(ui_Panel100, 172);
    lv_obj_set_height(ui_Panel100, 175);
    lv_obj_set_x(ui_Panel100, -3);
    lv_obj_set_y(ui_Panel100, -22);
    lv_obj_set_align(ui_Panel100, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel100, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel100, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel100, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel100, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel100, lv_color_hex(0xFFFF50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel100, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel100, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel100, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel100, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel100, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel100, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel100, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Panel100, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel75 = lv_obj_create(page);
    lv_obj_set_width(ui_Panel75, 120);
    lv_obj_set_height(ui_Panel75, 112);
    lv_obj_set_x(ui_Panel75, -2);
    lv_obj_set_y(ui_Panel75, -28);
    lv_obj_set_align(ui_Panel75, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel75, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel75, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel75, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel75, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel75, lv_color_hex(0xFFFF46), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel75, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel75, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel75, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel75, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel75, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel75, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel75, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Panel75, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel50 = lv_obj_create(page);
    lv_obj_set_width(ui_Panel50, 78);
    lv_obj_set_height(ui_Panel50, 80);
    lv_obj_set_x(ui_Panel50, -5);
    lv_obj_set_y(ui_Panel50, -32);
    lv_obj_set_align(ui_Panel50, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel50, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel50, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel50, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel50, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel50, lv_color_hex(0xFFFF32), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel50, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel50, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel50, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel50, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel50, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel50, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel50, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Panel50, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel25 = lv_obj_create(page);
    lv_obj_set_width(ui_Panel25, 47);
    lv_obj_set_height(ui_Panel25, 47);
    lv_obj_set_x(ui_Panel25, -5);
    lv_obj_set_y(ui_Panel25, -36);
    lv_obj_set_align(ui_Panel25, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel25, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel25, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel25, lv_color_hex(0x282828), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel25, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel25, lv_color_hex(0xFFFF02), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel25, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel25, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel25, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel25, lv_color_hex(0xFAFA82), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel25, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel25, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Panel25, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image1 = lv_img_create(page);
    lv_img_set_src(ui_Image1, &ui_img_led3_png);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image1, -3);
    lv_obj_set_y(ui_Image1, -48);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    
    label_pwm_set = lv_label_create(page);
    lv_obj_set_style_text_font(label_pwm_set, &HelveticaNeue_Regular_24, 0);
    if (light_set_conf.light_pwm) {
        lv_label_set_text_fmt(label_pwm_set, "%d%%", light_set_conf.light_pwm);
    } else {
        lv_label_set_text(label_pwm_set, "--");
    }
    lv_obj_align(label_pwm_set, LV_ALIGN_CENTER, 0, 65);


    lv_obj_add_event_cb(page, light_rgb_event_cb, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(page, light_rgb_event_cb, LV_EVENT_KEY, NULL);
    lv_obj_add_event_cb(page, light_rgb_event_cb, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(page, light_rgb_event_cb, LV_EVENT_CLICKED, NULL);
    ui_add_obj_to_encoder_group(page);
}

static bool light_rgb_layer_enter_cb(void *layer)
{
    bool ret = false;

    LV_LOG_USER("");
    lv_layer_t *create_layer = layer;
    if (NULL == create_layer->lv_obj_layer) {
        ret = true;
        create_layer->lv_obj_layer = lv_obj_create(lv_scr_act());
        lv_obj_remove_style_all(create_layer->lv_obj_layer);
        lv_obj_set_size(create_layer->lv_obj_layer, LV_HOR_RES, LV_VER_RES);

        ui_light_rgb_init(create_layer->lv_obj_layer);
        set_time_out(&time_20ms, 20);
        set_time_out(&time_500ms, 200);
    }

    return ret;
}

static bool light_rgb_layer_exit_cb(void *layer)
{
    LV_LOG_USER("");
    // bsp_led_RGB_set(0x00, 0x00, 0x00);
    return true;
}

void set_light_warm_cool(void)
{
    if(light_set_conf.light_cck == LIGHT_CCK_COOL){
        lv_obj_set_style_bg_color(ui_Panel100, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Panel100, lv_color_hex(0xFAFAFA), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_Panel100, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(ui_Panel75, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Panel75, lv_color_hex(0xFAFAFA), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_Panel75, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(ui_Panel50, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Panel50, lv_color_hex(0xFAFAFA), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_Panel50, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(ui_Panel25, lv_color_hex(0x282828), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(ui_Panel25, lv_color_hex(0xFAFAFA), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_Panel25, lv_color_hex(0xFAFA82), LV_PART_MAIN | LV_STATE_DEFAULT);

    }
    else{
    lv_obj_set_style_bg_color(ui_Panel100, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel100, lv_color_hex(0xFFFF50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel100, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Panel75, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel75, lv_color_hex(0xFFFF46), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel75, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Panel50, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel50, lv_color_hex(0xFFFF32), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel50, lv_color_hex(0xFAFA96), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Panel25, lv_color_hex(0x282828), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel25, lv_color_hex(0xFFFF02), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel25, lv_color_hex(0xFAFA82), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}
extern void pwm_RGB_start(uint8_t red_percent, uint8_t green_percent, uint8_t blue_percent);
static void light_rgb_layer_timer_cb(lv_timer_t *tmr)
{
    uint32_t RGB_color = 0xFF;

    feed_clock_time();

    if (is_time_out(&time_20ms)) {

        if ((light_set_conf.light_pwm ^ light_xor.light_pwm) || (light_set_conf.light_cck ^ light_xor.light_cck)) {
            light_xor.light_pwm = light_set_conf.light_pwm;
            light_xor.light_cck = light_set_conf.light_cck;

            if (LIGHT_CCK_COOL == light_xor.light_cck) {
                pwm_RGB_start(light_xor.light_pwm, light_xor.light_pwm, light_xor.light_pwm);
                // RGB_color = (0xFF * light_xor.light_pwm / 100) << 16 | (0xFF * light_xor.light_pwm / 100) << 8 | (0xFF * light_xor.light_pwm / 100) << 0;
            } else {
                pwm_RGB_start(light_xor.light_pwm, light_xor.light_pwm, 0);
                // RGB_color = (0xFF * light_xor.light_pwm / 100) << 16 | (0xFF * light_xor.light_pwm / 100) << 8 | (0x33 * light_xor.light_pwm / 100) << 0;
            }

            lv_obj_add_flag(ui_Panel100, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Panel75, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Panel50, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Panel25, LV_OBJ_FLAG_HIDDEN);
            // lv_obj_add_flag(img_light_pwm_0, LV_OBJ_FLAG_HIDDEN);

            if (light_set_conf.light_pwm) {
                lv_label_set_text_fmt(label_pwm_set, "%d%%", light_set_conf.light_pwm);
            } else {
                lv_label_set_text(label_pwm_set, "--");
            }

            uint8_t cck_set = (uint8_t)light_xor.light_cck;

            switch (light_xor.light_pwm) {
            case 100:
                lv_obj_clear_flag(ui_Panel100, LV_OBJ_FLAG_HIDDEN);
            case 75:
                lv_obj_clear_flag(ui_Panel75, LV_OBJ_FLAG_HIDDEN);
            case 50:
                lv_obj_clear_flag(ui_Panel50, LV_OBJ_FLAG_HIDDEN);
            case 25:
                lv_obj_clear_flag(ui_Panel25, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(img_light_bg, light_image.img_bg[cck_set]);
                break;
            case 0:
                // lv_obj_clear_flag(img_light_pwm_0, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(img_light_bg, &light_close_bg);
                break;
            default:
                break;
            }
        }
    }
}
