// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: 240_320

#include "../ui.h"

void ui_Screen6_screen_init(void)
{
    ui_Screen6 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5 = lv_obj_create(ui_Screen6);
    lv_obj_set_width(ui_Panel5, 241);
    lv_obj_set_height(ui_Panel5, 28);
    lv_obj_set_x(ui_Panel5, 0);
    lv_obj_set_y(ui_Panel5, -146);
    lv_obj_set_align(ui_Panel5, LV_ALIGN_LEFT_MID);
    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel5, lv_color_hex(0x0E1618), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button17 = lv_btn_create(ui_Screen6);
    lv_obj_set_width(ui_Button17, 29);
    lv_obj_set_height(ui_Button17, 23);
    lv_obj_set_x(ui_Button17, -104);
    lv_obj_set_y(ui_Button17, -145);
    lv_obj_set_align(ui_Button17, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button17, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button17, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button17, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button17, lv_color_hex(0x0E1618), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Button17, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Button17, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label29 = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_Label29, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label29, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label29, -105);
    lv_obj_set_y(ui_Label29, -144);
    lv_obj_set_align(ui_Label29, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label29, "<");
    lv_obj_set_style_text_color(ui_Label29, lv_color_hex(0xB9B9B9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label29, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel12 = lv_obj_create(ui_Screen6);
    lv_obj_set_width(ui_Panel12, 228);
    lv_obj_set_height(ui_Panel12, 137);
    lv_obj_set_x(ui_Panel12, -2);
    lv_obj_set_y(ui_Panel12, -50);
    lv_obj_set_align(ui_Panel12, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel12, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel12, lv_color_hex(0x686868), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TextArea_ssid = lv_textarea_create(ui_Screen6);
    lv_obj_set_width(ui_TextArea_ssid, 150);
    lv_obj_set_height(ui_TextArea_ssid, LV_SIZE_CONTENT);    /// 34
    lv_obj_set_x(ui_TextArea_ssid, -27);
    lv_obj_set_y(ui_TextArea_ssid, -63);
    lv_obj_set_align(ui_TextArea_ssid, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TextArea_ssid, "ssid...");
    lv_textarea_set_one_line(ui_TextArea_ssid, true);

    ui_TextArea_password = lv_textarea_create(ui_Screen6);
    lv_obj_set_width(ui_TextArea_password, 150);
    lv_obj_set_height(ui_TextArea_password, LV_SIZE_CONTENT);    /// 35
    lv_obj_set_x(ui_TextArea_password, -27);
    lv_obj_set_y(ui_TextArea_password, -20);
    lv_obj_set_align(ui_TextArea_password, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TextArea_password, "password...");
    lv_textarea_set_one_line(ui_TextArea_password, true);

    ui_Keyboard2 = lv_keyboard_create(ui_Screen6);
    lv_obj_set_width(ui_Keyboard2, 239);
    lv_obj_set_height(ui_Keyboard2, 108);
    lv_obj_set_x(ui_Keyboard2, 0);
    lv_obj_set_y(ui_Keyboard2, 85);
    lv_obj_set_align(ui_Keyboard2, LV_ALIGN_CENTER);

    ui_Label30 = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_Label30, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label30, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label30, -48);
    lv_obj_set_y(ui_Label30, -144);
    lv_obj_set_align(ui_Label30, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label30, "wifi Setting");
    lv_obj_set_style_text_color(ui_Label30, lv_color_hex(0xB9B9B9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button18 = lv_btn_create(ui_Screen6);
    lv_obj_set_width(ui_Button18, 46);
    lv_obj_set_height(ui_Button18, 30);
    lv_obj_set_x(ui_Button18, 79);
    lv_obj_set_y(ui_Button18, -22);
    lv_obj_set_align(ui_Button18, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button18, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button18, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button18, lv_color_hex(0xB7C1C1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label31 = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_Label31, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label31, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label31, 79);
    lv_obj_set_y(ui_Label31, -24);
    lv_obj_set_align(ui_Label31, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label31, "conect");
    lv_obj_set_style_text_font(ui_Label31, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label32 = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_Label32, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label32, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label32, -73);
    lv_obj_set_y(ui_Label32, -102);
    lv_obj_set_align(ui_Label32, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label32, "status:");
    lv_label_set_recolor(ui_Label32, "true");

    lv_obj_add_event_cb(ui_Button17, ui_event_Button17, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TextArea_ssid, ui_event_TextArea_ssid, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TextArea_password, ui_event_TextArea_password, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(ui_Keyboard2, ui_TextArea_ssid);


    lv_obj_add_event_cb(ui_Button18, ui_event_Button18, LV_EVENT_ALL, NULL);
}
