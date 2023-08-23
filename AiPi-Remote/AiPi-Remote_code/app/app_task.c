#define __APP_TASK_C_
#include "app_task.h"

#include "gui_guider.h"
#include "custom.h"

TaskHandle_t app_task_xhandle;

// xQueueHandle battery_queue = NULL;
xQueueHandle button_queue = NULL;
xQueueHandle lvgl_queue = NULL;
// xQueueHandle irsend_queue = NULL;
// xQueueHandle power_queue = NULL;
// xQueueHandle voice_queue = NULL;

extern lv_ui guider_ui;

extern uint8_t temp_count;
extern uint8_t mode_count;
extern uint8_t wind_count;
extern uint8_t onoff_status;
extern uint8_t power_status;

void app_task_process(void *msg)
{
    uint16_t app_status;
    while (1)
    {
        if (xQueueReceive(lvgl_queue, &app_status, portMAX_DELAY))
        {
            switch (app_status)
            {
            case 0x01:
                if (onoff_status)
                {
                    lv_obj_add_flag(guider_ui.screen_close_btn, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(guider_ui.screen_open_btn, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_clear_flag(guider_ui.screen_close_btn, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_open_btn, LV_OBJ_FLAG_HIDDEN);
                }
                break;
            case 0x02:
                switch (mode_count)
                {
                case 0x00:
                    lv_label_set_text(guider_ui.screen_mode_status, "制冷");
                    /* code */
                    break;
                case 0x01:
                    /* code */
                    lv_label_set_text(guider_ui.screen_mode_status, "送风");
                    break;
                case 0x02:
                    /* code */
                    lv_label_set_text(guider_ui.screen_mode_status, "自动");
                    break;
                case 0x03:
                    /* code */
                    lv_label_set_text(guider_ui.screen_mode_status, "制热");
                    break;
                default:
                    break;
                }
                break;
            case 0x03:
                switch (wind_count)
                {
                case 0x00:
                    lv_label_set_text(guider_ui.screen_wind_status, "固定风");
                    /* code */
                    break;
                case 0x01:
                    /* code */
                    lv_label_set_text(guider_ui.screen_wind_status, "高风");
                    break;
                case 0x02:
                    /* code */
                    lv_label_set_text(guider_ui.screen_wind_status, "中风");
                    break;
                case 0x03:
                    /* code */
                    lv_label_set_text(guider_ui.screen_wind_status, "低风");
                    break;
                case 0x04:
                    /* code */
                    lv_label_set_text(guider_ui.screen_wind_status, "自动");
                    break;
                default:
                    break;
                }
                break;
            case 0x04:
                switch (temp_count)
                {
                case 0x00:
                    lv_label_set_text(guider_ui.screen_temp_lab, "17");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 0);
                    /* code */
                    break;
                case 0x01:
                    lv_label_set_text(guider_ui.screen_temp_lab, "18");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 21);
                    /* code */
                    break;
                case 0x02:
                    lv_label_set_text(guider_ui.screen_temp_lab, "19");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 42);
                    /* code */
                    break;
                case 0x03:
                    lv_label_set_text(guider_ui.screen_temp_lab, "20");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 63);
                    /* code */
                    break;
                case 0x04:
                    lv_label_set_text(guider_ui.screen_temp_lab, "21");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 83);
                    /* code */
                    break;
                case 0x05:
                    lv_label_set_text(guider_ui.screen_temp_lab, "22");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 104);
                    /* code */
                    break;
                case 0x06:
                    lv_label_set_text(guider_ui.screen_temp_lab, "23");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 125);
                    /* code */
                    break;
                case 0x07:
                    lv_label_set_text(guider_ui.screen_temp_lab, "24");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 145);
                    /* code */
                    break;
                case 0x08:
                    lv_label_set_text(guider_ui.screen_temp_lab, "25");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 166);
                    /* code */
                    break;
                case 0x09:
                    lv_label_set_text(guider_ui.screen_temp_lab, "26");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 187);
                    /* code */
                    break;
                case 0x0A:
                    lv_label_set_text(guider_ui.screen_temp_lab, "27");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 208);
                    /* code */
                    break;
                case 0x0B:
                    lv_label_set_text(guider_ui.screen_temp_lab, "28");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 229);
                    /* code */
                    break;
                case 0x0C:
                    lv_label_set_text(guider_ui.screen_temp_lab, "29");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 250);
                    /* code */
                    break;
                case 0x0D:
                    lv_label_set_text(guider_ui.screen_temp_lab, "30");
                    lv_arc_set_angles(guider_ui.screen_arc_1, 0, 270);
                    /* code */
                    break;
                default:
                    break;
                }
                break;
            case 0x05:
                switch (power_status)
                {
                case 0x00:
                    lv_obj_clear_flag(guider_ui.screen_lowpower, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_normalcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_fullcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_Charging, LV_OBJ_FLAG_HIDDEN);
                    /* code */
                    break;
                case 0x01:
                    lv_obj_clear_flag(guider_ui.screen_normalcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_lowpower, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_fullcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_Charging, LV_OBJ_FLAG_HIDDEN);
                    /* code */
                    break;
                case 0x02:
                    lv_obj_clear_flag(guider_ui.screen_fullcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_normalcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_lowpower, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_Charging, LV_OBJ_FLAG_HIDDEN);
                    /* code */
                    break;
                case 0x03:
                    lv_obj_clear_flag(guider_ui.screen_Charging, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_lowpower, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_normalcharge, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(guider_ui.screen_fullcharge, LV_OBJ_FLAG_HIDDEN);
                    /* code */
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}
