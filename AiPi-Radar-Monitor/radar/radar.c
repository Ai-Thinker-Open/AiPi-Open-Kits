#define __RADAR_TASK_C_
#include "radar.h"

TaskHandle_t radar_task_xhandle;

static lv_point_t line_points0[] = { { 160, 240 }, { 160, 0 } };
static lv_point_t line_points1[] = { { 160, 240 }, { 0, 62 } };
static lv_point_t line_points2[] = { { 160, 240 }, { 320, 62 } };
static lv_point_t line_points3[] = { { 160, 240 }, { 47, 28 } };
static lv_point_t line_points4[] = { { 160, 240 }, { 273, 28 } };
static lv_point_t line_points5[] = { { 160, 240 }, { 102, 7 } };
static lv_point_t line_points6[] = { { 160, 240 }, { 218, 7 } };

static lv_point_t line_points_one[] = { { 160, 160 }, { 160, 150 } };
static lv_point_t line_points_two[] = { { 160, 160 }, { 160, 150 } };
static lv_point_t line_points_three[] = { { 160, 160 }, { 160, 150 } };

lv_obj_t *line_obj_one = NULL;
lv_obj_t *line_obj_two = NULL;
lv_obj_t *line_obj_three = NULL;

lv_obj_t *lab_obj_one_x = NULL;
lv_obj_t *lab_obj_one_y = NULL;
lv_obj_t *lab_obj_one_d = NULL;

lv_obj_t *lab_obj_two_x = NULL;
lv_obj_t *lab_obj_two_y = NULL;
lv_obj_t *lab_obj_two_d = NULL;

lv_obj_t *lab_obj_three_x = NULL;
lv_obj_t *lab_obj_three_y = NULL;
lv_obj_t *lab_obj_three_d = NULL;

void clear_lab_one(void)
{
    if (lab_obj_one_x != NULL) {
        lv_obj_del(lab_obj_one_x);
        lab_obj_one_x = NULL;
    }
    if (lab_obj_one_y != NULL) {
        lv_obj_del(lab_obj_one_y);
        lab_obj_one_y = NULL;
    }
    if (lab_obj_one_d != NULL) {
        lv_obj_del(lab_obj_one_d);
        lab_obj_one_d = NULL;
    }
}
void clear_lab_two(void)
{
    if (lab_obj_two_x != NULL) {
        lv_obj_del(lab_obj_two_x);
        lab_obj_two_x = NULL;
    }
    if (lab_obj_two_y != NULL) {
        lv_obj_del(lab_obj_two_y);
        lab_obj_two_y = NULL;
    }
    if (lab_obj_two_d != NULL) {
        lv_obj_del(lab_obj_two_d);
        lab_obj_two_d = NULL;
    }
}
void clear_lab_three(void)
{
    if (lab_obj_three_x != NULL) {
        lv_obj_del(lab_obj_three_x);
        lab_obj_three_x = NULL;
    }
    if (lab_obj_three_y != NULL) {
        lv_obj_del(lab_obj_three_y);
        lab_obj_three_y = NULL;
    }
    if (lab_obj_three_d != NULL) {
        lv_obj_del(lab_obj_three_d);
        lab_obj_three_d = NULL;
    }
}

void int_to_str(int num, char *str)
{
    // 处理负数
    if (num < 0) {
        *str++ = '-';
        num = -num;
    }

    // 求整数的位数
    int temp = num;
    int digits = 1;
    while (temp /= 10) {
        digits++;
    }

    // 逐位转换
    str += digits;
    *str = '\0';

    do {
        *--str = num % 10 + '0';
        num /= 10;
    } while (num);
}

char str_x_one[10], str_y_one[10];
void disp_lab_one(lv_obj_t *radar, int32_t pos_x, int32_t pos_y)
{
    int_to_str(pos_x, str_x_one);
    int_to_str(pos_y, str_y_one);
    // pos_d = sqrt(pos_x*pos_x + pos_y*pos_y);
    // snprintf(str_x, sizeof(str_x), "%d", pos_x / 10);
    // snprintf(str_y, sizeof(str_y), "%d", pos_y / 10);
    // snprintf(str_d, sizeof(str_d), "%d", pos_d/10);

    // static lv_style_t point_font_style_one;
    // lv_style_init(&point_font_style_one);
    // lv_style_set_text_font(&point_font_style_one, &lv_font_montserrat_12);

    // lab_obj_one_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_x, 230);
    // lv_obj_set_y(lab_obj_one_x, 225);
    lv_label_set_text(lab_obj_one_x, str_x_one);
    // lv_obj_add_style(lab_obj_one_x, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_one_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_y, 265);
    // lv_obj_set_y(lab_obj_one_y, 225);
    lv_label_set_text(lab_obj_one_y, str_y_one);
    // lv_obj_add_style(lab_obj_one_y, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_one_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_d, 300);
    // lv_obj_set_y(lab_obj_one_d, 225);
    // lv_label_set_text(lab_obj_one_d, "");
    // lv_obj_add_style(lab_obj_one_d, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_d, LV_ALIGN_TOP_LEFT);
}

void disp_lab_two(lv_obj_t *radar, int32_t pos_x, int32_t pos_y)
{
    char str_x[10], str_y[10], str_d[10];
    int32_t pos_d;

    int_to_str(pos_x, str_x);
    int_to_str(pos_y, str_y);
    // pos_d = sqrt(pos_x*pos_x + pos_y*pos_y);
    // snprintf(str_x, sizeof(str_x), "%d", pos_x / 10);
    // snprintf(str_y, sizeof(str_y), "%d", pos_y / 10);
    // snprintf(str_d, sizeof(str_d), "%d", pos_d/10);
    // static lv_style_t point_font_style_two;
    // lv_style_init(&point_font_style_two);
    // lv_style_set_text_font(&point_font_style_two, &lv_font_montserrat_12);

    // lab_obj_two_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_x, 230);
    // lv_obj_set_y(lab_obj_two_x, 210);
    lv_label_set_text(lab_obj_two_x, str_x);
    // lv_obj_add_style(lab_obj_two_x, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_two_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_y, 265);
    // lv_obj_set_y(lab_obj_two_y, 210);
    lv_label_set_text(lab_obj_two_y, str_y);
    // lv_obj_add_style(lab_obj_two_y, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_two_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_d, 300);
    // lv_obj_set_y(lab_obj_two_d, 210);
    // lv_label_set_text(lab_obj_two_d, "");
    // lv_obj_add_style(lab_obj_two_d, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_d, LV_ALIGN_TOP_LEFT);
}

void disp_lab_three(lv_obj_t *radar, int32_t pos_x, int32_t pos_y)
{
    char str_x[10], str_y[10], str_d[10];
    int32_t pos_d;

    int_to_str(pos_x, str_x);
    int_to_str(pos_y, str_y);
    // pos_d = sqrt(pos_x*pos_x + pos_y*pos_y);
    // snprintf(str_x, sizeof(str_x), "%d", pos_x / 10);
    // snprintf(str_y, sizeof(str_y), "%d", pos_y / 10);
    // snprintf(str_d, sizeof(str_d), "%d", pos_d/10);
    // static lv_style_t point_font_style_three;
    // lv_style_init(&point_font_style_three);
    // lv_style_set_text_font(&point_font_style_three, &lv_font_montserrat_12);

    // lab_obj_three_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_x, 230);
    // lv_obj_set_y(lab_obj_three_x, 195);
    lv_label_set_text(lab_obj_three_x, str_x);
    // lv_obj_add_style(lab_obj_three_x, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_three_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_y, 265);
    // lv_obj_set_y(lab_obj_three_y, 195);
    lv_label_set_text(lab_obj_three_y, str_y);
    // lv_obj_add_style(lab_obj_three_y, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_three_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_d, 300);
    // lv_obj_set_y(lab_obj_three_d, 195);
    // lv_label_set_text(lab_obj_three_d, "");
    // lv_obj_add_style(lab_obj_three_d, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_d, LV_ALIGN_TOP_LEFT);
}

typedef struct
{
    int32_t pos_x;
    int32_t pos_y;
    int32_t speed;
    int32_t resolution;
} radar_type_t;

typedef struct
{
    radar_type_t radar_one;
    radar_type_t radar_two;
    radar_type_t radar_three;
} radar_three_type_t;

extern xQueueHandle radar_queue;

void draw_point_one(lv_obj_t *radar, int32_t pos_x, int32_t pos_y, uint32_t color)
{
    line_points_one[0].x = 160 + pos_x;
    line_points_one[0].y = 240 - pos_y;

    line_points_one[1].x = 160 + pos_x;
    line_points_one[1].y = 240 - pos_y + 10;

    if (line_obj_one == NULL) {
        line_obj_one = lv_line_create(radar);
        lv_obj_set_style_line_width(line_obj_one, 10, LV_PART_MAIN);
        lv_obj_set_style_line_color(line_obj_one, lv_color_hex(color), LV_PART_MAIN);
    }
    // if (line_obj_one != NULL) {

    lv_line_set_points(line_obj_one, line_points_one, sizeof(line_points_one) / sizeof(lv_point_t));

    // }
}

void clear_point_one(void)
{
    // if (line_obj_one != NULL) {
    //     lv_obj_del(line_obj_one);
    //     // if(line_obj_one == NULL)
    //     // {
    //     //     printf("line_obj_one is null 1111!\r\n");
    //     // }
    //     line_obj_one = NULL;
    //     // if(line_obj_one == NULL)
    //     // {
    //     //     printf("line_obj_one is null 2222!\r\n");
    //     // }
    // }
}

void draw_point_two(lv_obj_t *radar, int32_t pos_x, int32_t pos_y, uint32_t color)
{
    line_points_two[0].x = 160 + pos_x;
    line_points_two[0].y = 240 - pos_y;

    line_points_two[1].x = 160 + pos_x;
    line_points_two[1].y = 240 - pos_y + 10;

    if (line_obj_two == NULL) {
        line_obj_two = lv_line_create(radar);
        lv_obj_set_style_line_width(line_obj_two, 10, LV_PART_MAIN);
        lv_obj_set_style_line_color(line_obj_two, lv_color_hex(color), LV_PART_MAIN);
    }
    // if (line_obj_two != NULL) {

    lv_line_set_points(line_obj_two, line_points_two, sizeof(line_points_two) / sizeof(lv_point_t));
    // }
}

void clear_point_two(void)
{
    // if (line_obj_two != NULL) {
    //     lv_obj_del(line_obj_two);
    //     line_obj_two = NULL;
    // }
}

void draw_point_three(lv_obj_t *radar, int32_t pos_x, int32_t pos_y, uint32_t color)
{
    line_points_three[0].x = 160 + pos_x;
    line_points_three[0].y = 240 - pos_y;

    line_points_three[1].x = 160 + pos_x;
    line_points_three[1].y = 240 - pos_y + 10;

    if (line_obj_three == NULL) {
        line_obj_three = lv_line_create(radar);
        lv_obj_set_style_line_width(line_obj_three, 10, LV_PART_MAIN);
        lv_obj_set_style_line_color(line_obj_three, lv_color_hex(color), LV_PART_MAIN);
    }
    // if (line_obj_three != NULL) {

    lv_line_set_points(line_obj_three, line_points_three, sizeof(line_points_three) / sizeof(lv_point_t));
    // }
}

void clear_point_three(void)
{
    // if (line_obj_three != NULL) {
    //     lv_obj_del(line_obj_three);
    //     line_obj_three = NULL;
    // }
}

lv_obj_t *ui_Switch2;

extern void multiplayer_uart_send(void);
extern void single_uart_send(void);

static void sw_1_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    switch (code) {
        case LV_EVENT_VALUE_CHANGED: {
            if (lv_obj_has_state(ui_Switch2, LV_STATE_CHECKED)) {
                multiplayer_uart_send();
                printf("State: %s\n", lv_obj_has_state(ui_Switch2, LV_STATE_CHECKED) ? "On" : "Off");
            } else {
                single_uart_send();
                printf("State: %s\n", lv_obj_has_state(ui_Switch2, LV_STATE_CHECKED) ? "On" : "Off");
            }

            // if (state == LV_SWITCH_STATE_OFF) {
            //     printf("Switch State: OFF\n");
            // } else if (state == LV_SWITCH_STATE_ON) {
            //     printf("Switch State: ON\n");
            // }
        } break;
        default:
            break;
    }
}

void draw_radar(lv_obj_t *radar, uint32_t color)
{
    //关闭滚动条
    lv_obj_clear_flag(radar, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *line0 = lv_line_create(radar);
    lv_obj_t *line1 = lv_line_create(radar);
    lv_obj_t *line2 = lv_line_create(radar);
    lv_obj_t *line3 = lv_line_create(radar);
    lv_obj_t *line4 = lv_line_create(radar);
    lv_obj_t *line5 = lv_line_create(radar);
    lv_obj_t *line6 = lv_line_create(radar);

    lv_obj_set_style_line_width(line0, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line0, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line1, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line1, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line2, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line2, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line3, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line3, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line4, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line4, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line5, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line5, lv_color_hex(color), LV_PART_MAIN);

    lv_obj_set_style_line_width(line6, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line6, lv_color_hex(color), LV_PART_MAIN);

    lv_line_set_points(line0, line_points0, sizeof(line_points0) / sizeof(lv_point_t));
    lv_line_set_points(line1, line_points1, sizeof(line_points1) / sizeof(lv_point_t));
    lv_line_set_points(line2, line_points2, sizeof(line_points2) / sizeof(lv_point_t));
    lv_line_set_points(line3, line_points3, sizeof(line_points3) / sizeof(lv_point_t));
    lv_line_set_points(line4, line_points4, sizeof(line_points4) / sizeof(lv_point_t));
    lv_line_set_points(line5, line_points5, sizeof(line_points5) / sizeof(lv_point_t));
    lv_line_set_points(line6, line_points6, sizeof(line_points6) / sizeof(lv_point_t));

    lv_obj_t *arc0 = lv_arc_create(radar);
    lv_obj_set_width(arc0, 480);
    lv_obj_set_height(arc0, 480);
    lv_obj_set_x(arc0, 0);
    lv_obj_set_y(arc0, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc0, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc0, 228, 312);
    lv_arc_set_bg_angles(arc0, 228, 312);
    // 清除点
    lv_obj_remove_style(arc0, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc0, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc0, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc0, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc0, LV_ALIGN_CENTER);

    lv_obj_t *arc1 = lv_arc_create(radar);
    lv_obj_set_width(arc1, 400);
    lv_obj_set_height(arc1, 400);
    lv_obj_set_x(arc1, 0);
    lv_obj_set_y(arc1, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc1, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc1, 228, 312);
    lv_arc_set_bg_angles(arc1, 228, 312);
    // 清除点
    lv_obj_remove_style(arc1, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc1, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc1, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc1, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc1, LV_ALIGN_CENTER);

    lv_obj_t *arc2 = lv_arc_create(radar);
    lv_obj_set_width(arc2, 320);
    lv_obj_set_height(arc2, 320);
    lv_obj_set_x(arc2, 0);
    lv_obj_set_y(arc2, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc2, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc2, 228, 312);
    lv_arc_set_bg_angles(arc2, 228, 312);
    // 清除点
    lv_obj_remove_style(arc2, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc2, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc2, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc2, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc2, LV_ALIGN_CENTER);

    lv_obj_t *arc3 = lv_arc_create(radar);
    lv_obj_set_width(arc3, 240);
    lv_obj_set_height(arc3, 240);
    lv_obj_set_x(arc3, 0);
    lv_obj_set_y(arc3, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc3, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc3, 228, 312);
    lv_arc_set_bg_angles(arc3, 228, 312);
    // 清除点
    lv_obj_remove_style(arc3, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc3, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc3, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc3, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc3, LV_ALIGN_CENTER);

    lv_obj_t *arc4 = lv_arc_create(radar);
    lv_obj_set_width(arc4, 160);
    lv_obj_set_height(arc4, 160);
    lv_obj_set_x(arc4, 0);
    lv_obj_set_y(arc4, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc4, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc4, 228, 312);
    lv_arc_set_bg_angles(arc4, 228, 312);
    // 清除点
    lv_obj_remove_style(arc4, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc4, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc4, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc4, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc4, LV_ALIGN_CENTER);

    lv_obj_t *arc5 = lv_arc_create(radar);
    lv_obj_set_width(arc5, 80);
    lv_obj_set_height(arc5, 80);
    lv_obj_set_x(arc5, 0);
    lv_obj_set_y(arc5, 120);

    // 修改圆弧前景色
    lv_obj_set_style_arc_color(arc5, lv_color_hex(color), LV_PART_INDICATOR);
    // 设置圆弧的起始位置和终止位置
    lv_arc_set_angles(arc5, 228, 312);
    lv_arc_set_bg_angles(arc5, 228, 312);
    // 清除点
    lv_obj_remove_style(arc5, NULL, LV_PART_KNOB);
    //静止点击调值
    lv_obj_clear_flag(arc5, LV_OBJ_FLAG_CLICKABLE);
    //修改圆弧宽度
    lv_obj_set_style_arc_width(arc5, 0, LV_PART_MAIN);      //背景弧度
    lv_obj_set_style_arc_width(arc5, 2, LV_PART_INDICATOR); //前景弧度

    lv_obj_set_align(arc5, LV_ALIGN_CENTER);

    static lv_style_t font_style;
    lv_style_init(&font_style);
    lv_style_set_text_font(&font_style, &lv_font_montserrat_8);

    lv_obj_t *lab_125 = lv_label_create(radar);
    lv_obj_set_width(lab_125, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_125, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_125, -42);
    lv_obj_set_y(lab_125, 92);
    lv_label_set_text(lab_125, "1.25");
    lv_obj_add_style(lab_125, &font_style, 0);
    lv_obj_set_align(lab_125, LV_ALIGN_CENTER);

    lv_obj_t *lab_250 = lv_label_create(radar);
    lv_obj_set_width(lab_250, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_250, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_250, -70);
    lv_obj_set_y(lab_250, 65);
    lv_label_set_text(lab_250, "2.50");
    lv_obj_add_style(lab_250, &font_style, 0);
    lv_obj_set_align(lab_250, LV_ALIGN_CENTER);

    lv_obj_t *lab_375 = lv_label_create(radar);
    lv_obj_set_width(lab_375, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_375, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_375, -93);
    lv_obj_set_y(lab_375, 36);
    lv_label_set_text(lab_375, "3.75");
    lv_obj_add_style(lab_375, &font_style, 0);
    lv_obj_set_align(lab_375, LV_ALIGN_CENTER);

    lv_obj_t *lab_500 = lv_label_create(radar);
    lv_obj_set_width(lab_500, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_500, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_500, -122);
    lv_obj_set_y(lab_500, 4);
    lv_label_set_text(lab_500, "5.00");
    lv_obj_add_style(lab_500, &font_style, 0);
    lv_obj_set_align(lab_500, LV_ALIGN_CENTER);

    lv_obj_t *lab_625 = lv_label_create(radar);
    lv_obj_set_width(lab_625, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_625, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_625, -150);
    lv_obj_set_y(lab_625, -28);
    lv_label_set_text(lab_625, "6.25");
    lv_obj_add_style(lab_625, &font_style, 0);
    lv_obj_set_align(lab_625, LV_ALIGN_CENTER);

    // lv_obj_t *lab_750 = lv_label_create(radar);
    // lv_obj_set_width(lab_750, LV_SIZE_CONTENT);   /// 1
    // lv_obj_set_height(lab_750, LV_SIZE_CONTENT);    /// 1
    // lv_obj_set_x(lab_750, 0);
    // lv_obj_set_y(lab_750, -110);
    // lv_label_set_text(lab_750,"7.50m");
    // lv_obj_add_style(lab_750,&font_style,0);
    // lv_obj_set_align(lab_750, LV_ALIGN_CENTER);

    // static lv_style_t point_font_style;
    // lv_style_init(&point_font_style);
    // lv_style_set_text_font(&point_font_style, &lv_font_montserrat_12);

    // lv_obj_t *lab_point1 = lv_label_create(radar);
    // lv_obj_set_width(lab_point1, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point1, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point1, 200);
    // lv_obj_set_y(lab_point1, 225);
    // lv_label_set_text(lab_point1, "P1");
    // lv_obj_add_style(lab_point1, &point_font_style, 0);
    // lv_obj_set_align(lab_point1, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point2 = lv_label_create(radar);
    // lv_obj_set_width(lab_point2, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point2, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point2, 200);
    // lv_obj_set_y(lab_point2, 210);
    // lv_label_set_text(lab_point2, "P2");
    // lv_obj_add_style(lab_point2, &point_font_style, 0);
    // lv_obj_set_align(lab_point2, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point3 = lv_label_create(radar);
    // lv_obj_set_width(lab_point3, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point3, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point3, 200);
    // lv_obj_set_y(lab_point3, 195);
    // lv_label_set_text(lab_point3, "P3");
    // lv_obj_add_style(lab_point3, &point_font_style, 0);
    // lv_obj_set_align(lab_point3, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point1x = lv_label_create(radar);
    // lv_obj_set_width(lab_point1x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point1x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point1x, 215);
    // lv_obj_set_y(lab_point1x, 225);
    // lv_label_set_text(lab_point1x, "-x:");
    // lv_obj_add_style(lab_point1x, &point_font_style, 0);
    // lv_obj_set_align(lab_point1x, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point2x = lv_label_create(radar);
    // lv_obj_set_width(lab_point2x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point2x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point2x, 215);
    // lv_obj_set_y(lab_point2x, 210);
    // lv_label_set_text(lab_point2x, "-x:");
    // lv_obj_add_style(lab_point2x, &point_font_style, 0);
    // lv_obj_set_align(lab_point2x, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point3x = lv_label_create(radar);
    // lv_obj_set_width(lab_point3x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point3x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point3x, 215);
    // lv_obj_set_y(lab_point3x, 195);
    // lv_label_set_text(lab_point3x, "-x:");
    // lv_obj_add_style(lab_point3x, &point_font_style, 0);
    // lv_obj_set_align(lab_point3x, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point1y = lv_label_create(radar);
    // lv_obj_set_width(lab_point1y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point1y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point1y, 250);
    // lv_obj_set_y(lab_point1y, 225);
    // lv_label_set_text(lab_point1y, "y:");
    // lv_obj_add_style(lab_point1y, &point_font_style, 0);
    // lv_obj_set_align(lab_point1y, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point2y = lv_label_create(radar);
    // lv_obj_set_width(lab_point2y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point2y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point2y, 250);
    // lv_obj_set_y(lab_point2y, 210);
    // lv_label_set_text(lab_point2y, "y:");
    // lv_obj_add_style(lab_point2y, &point_font_style, 0);
    // lv_obj_set_align(lab_point2y, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point3y = lv_label_create(radar);
    // lv_obj_set_width(lab_point3y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point3y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point3y, 250);
    // lv_obj_set_y(lab_point3y, 195);
    // lv_label_set_text(lab_point3y, "y:");
    // lv_obj_add_style(lab_point3y, &point_font_style, 0);
    // lv_obj_set_align(lab_point3y, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point1d = lv_label_create(radar);
    // lv_obj_set_width(lab_point1d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point1d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point1d, 285);
    // lv_obj_set_y(lab_point1d, 225);
    // lv_label_set_text(lab_point1d, "d:");
    // lv_obj_add_style(lab_point1d, &point_font_style, 0);
    // lv_obj_set_align(lab_point1d, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point2d = lv_label_create(radar);
    // lv_obj_set_width(lab_point2d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point2d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point2d, 285);
    // lv_obj_set_y(lab_point2d, 210);
    // lv_label_set_text(lab_point2d, "d:");
    // lv_obj_add_style(lab_point2d, &point_font_style, 0);
    // lv_obj_set_align(lab_point2d, LV_ALIGN_TOP_LEFT);

    // lv_obj_t *lab_point3d = lv_label_create(radar);
    // lv_obj_set_width(lab_point3d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_point3d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_point3d, 285);
    // lv_obj_set_y(lab_point3d, 195);
    // lv_label_set_text(lab_point3d, "d:");
    // lv_obj_add_style(lab_point3d, &point_font_style, 0);
    // lv_obj_set_align(lab_point3d, LV_ALIGN_TOP_LEFT);

    // static lv_style_t point_font_style_one;
    // lv_style_init(&point_font_style_one);
    // lv_style_set_text_font(&point_font_style_one, &lv_font_montserrat_12);

    // lab_obj_one_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_x, 230);
    // lv_obj_set_y(lab_obj_one_x, 225);
    // lv_label_set_text(lab_obj_one_x, " ");
    // lv_obj_add_style(lab_obj_one_x, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_one_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_y, 265);
    // lv_obj_set_y(lab_obj_one_y, 225);
    // lv_label_set_text(lab_obj_one_y, " ");
    // lv_obj_add_style(lab_obj_one_y, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_one_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_one_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_one_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_one_d, 300);
    // lv_obj_set_y(lab_obj_one_d, 225);
    // lv_label_set_text(lab_obj_one_d, " ");
    // lv_obj_add_style(lab_obj_one_d, &point_font_style_one, 0);
    // lv_obj_set_align(lab_obj_one_d, LV_ALIGN_TOP_LEFT);

    // static lv_style_t point_font_style_two;
    // lv_style_init(&point_font_style_two);
    // lv_style_set_text_font(&point_font_style_two, &lv_font_montserrat_12);

    // lab_obj_two_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_x, 230);
    // lv_obj_set_y(lab_obj_two_x, 210);
    // lv_label_set_text(lab_obj_two_x, " ");
    // lv_obj_add_style(lab_obj_two_x, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_two_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_y, 265);
    // lv_obj_set_y(lab_obj_two_y, 210);
    // lv_label_set_text(lab_obj_two_y, " ");
    // lv_obj_add_style(lab_obj_two_y, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_two_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_two_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_two_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_two_d, 300);
    // lv_obj_set_y(lab_obj_two_d, 210);
    // lv_label_set_text(lab_obj_two_d, " ");
    // lv_obj_add_style(lab_obj_two_d, &point_font_style_two, 0);
    // lv_obj_set_align(lab_obj_two_d, LV_ALIGN_TOP_LEFT);

    // static lv_style_t point_font_style_three;
    // lv_style_init(&point_font_style_three);
    // lv_style_set_text_font(&point_font_style_three, &lv_font_montserrat_12);

    // lab_obj_three_x = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_x, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_x, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_x, 230);
    // lv_obj_set_y(lab_obj_three_x, 195);
    // lv_label_set_text(lab_obj_three_x, " ");
    // lv_obj_add_style(lab_obj_three_x, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_x, LV_ALIGN_TOP_LEFT);

    // lab_obj_three_y = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_y, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_y, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_y, 265);
    // lv_obj_set_y(lab_obj_three_y, 195);
    // lv_label_set_text(lab_obj_three_y, " ");
    // lv_obj_add_style(lab_obj_three_y, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_y, LV_ALIGN_TOP_LEFT);

    // lab_obj_three_d = lv_label_create(radar);
    // lv_obj_set_width(lab_obj_three_d, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(lab_obj_three_d, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(lab_obj_three_d, 300);
    // lv_obj_set_y(lab_obj_three_d, 195);
    // lv_label_set_text(lab_obj_three_d, " ");
    // lv_obj_add_style(lab_obj_three_d, &point_font_style_three, 0);
    // lv_obj_set_align(lab_obj_three_d, LV_ALIGN_TOP_LEFT);

    static lv_style_t font_style111;
    lv_style_init(&font_style111);
    lv_style_set_text_font(&font_style111, &lv_font_montserrat_12);

    lv_obj_t *lab_sin = lv_label_create(radar);
    lv_obj_set_width(lab_sin, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_sin, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_sin, 0);
    lv_obj_set_y(lab_sin, 220);
    lv_label_set_text(lab_sin, "Single");
    lv_obj_add_style(lab_sin, &font_style111, 0);
    lv_obj_set_align(lab_sin, LV_ALIGN_TOP_LEFT);

    lv_obj_t *lab_mul = lv_label_create(radar);
    lv_obj_set_width(lab_mul, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(lab_mul, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(lab_mul, 82);
    lv_obj_set_y(lab_mul, 220);
    lv_label_set_text(lab_mul, "Multiple");
    lv_obj_add_style(lab_mul, &font_style111, 0);
    lv_obj_set_align(lab_mul, LV_ALIGN_TOP_LEFT);

    ui_Switch2 = lv_switch_create(radar);
    lv_obj_set_width(ui_Switch2, 40);
    lv_obj_set_height(ui_Switch2, 15);
    lv_obj_set_x(ui_Switch2, 40);
    lv_obj_set_y(ui_Switch2, 220);
    lv_obj_set_align(ui_Switch2, LV_ALIGN_TOP_LEFT);

    lv_obj_add_event_cb(ui_Switch2, sw_1_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

void radar_task_process(void *msg)
{
    radar_three_type_t radar_value;
    int32_t one_x = 0;
    int32_t one_y = 0;
    int32_t two_x = 0;
    int32_t two_y = 0;
    int32_t three_x = 0;
    int32_t three_y = 0;

    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x00547F), LV_PART_MAIN | LV_STATE_DEFAULT);
    draw_radar(scr, 0x008800);

    uint8_t *p_test = (uint8_t *)malloc(4);

    if (p_test == NULL) {
        printf("ptest is null11111!\r\n");
    }

    free(p_test);

    if (p_test == NULL) {
        printf("ptest is null22222!\r\n");
    }

    // draw_point_one(scr, -50, 130, 0xFFFFFF);
    // draw_point_two(scr, 70, 170, 0xFFFFFF);
    // draw_point_three(scr, 90, 200, 0xFFFFFF);

    // draw_point_one(scr, 90, 200, 0xFFFFFF);
    // draw_point_one(scr, 70, 200, 0xFFFFFF);
    // draw_point_one(scr, 90, 170, 0xFFFFFF);
    // clear_point_one();

    while (1) {
        if (xQueueReceive(radar_queue, &radar_value, portMAX_DELAY)) {
            if (radar_value.radar_one.pos_x & 0x8000) {
                one_x = (radar_value.radar_one.pos_x & 0x7FFF) / 50;
            } else {
                one_x = (0 - (radar_value.radar_one.pos_x & 0x7FFF)) / 50;
            }
            if (radar_value.radar_one.pos_y & 0x8000) {
                one_y = (radar_value.radar_one.pos_y & 0x7FFF) / 34;
            } else {
                one_y = (0 - (radar_value.radar_one.pos_y & 0x7FFF)) / 34;
            }

            if (radar_value.radar_two.pos_x & 0x8000) {
                two_x = (radar_value.radar_two.pos_x & 0x7FFF) / 50;
            } else {
                two_x = (0 - (radar_value.radar_two.pos_x & 0x7FFF)) / 50;
            }
            if (radar_value.radar_two.pos_y & 0x8000) {
                two_y = (radar_value.radar_two.pos_y & 0x7FFF) / 34;
            } else {
                two_y = (0 - radar_value.radar_two.pos_y & 0x7FFF) / 34;
            }

            if (radar_value.radar_three.pos_x & 0x8000) {
                three_x = (radar_value.radar_three.pos_x & 0x7FFF) / 50;
            } else {
                three_x = (0 - (radar_value.radar_three.pos_x & 0x7FFF)) / 50;
            }
            if (radar_value.radar_three.pos_y & 0x8000) {
                three_y = (radar_value.radar_three.pos_y & 0x7FFF) / 34;
            } else {
                three_y = (0 - (radar_value.radar_three.pos_y & 0x7FFF)) / 34;
            }

            clear_point_one();
            clear_point_two();
            clear_point_three();
            // clear_lab_one();
            // clear_lab_two();
            // clear_lab_three();
            draw_point_one(scr, one_x, one_y, 0xFFFFFF);
            draw_point_two(scr, two_x, two_y, 0xFFFFFF);
            draw_point_three(scr, three_x, three_y, 0xFFFFFF);
            // disp_lab_one(scr, one_x, one_y);
            // disp_lab_two(scr, two_x, two_y);
            // disp_lab_three(scr, three_x, three_y);

            printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", one_x, one_y, radar_value.radar_one.speed, radar_value.radar_one.resolution);
            printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", two_x, two_y, radar_value.radar_two.speed, radar_value.radar_two.resolution);
            printf("pos_x:%d pos_y:%d speed:%d resolution:%d\r\n", three_x, three_y, radar_value.radar_three.speed, radar_value.radar_three.resolution);
        }
    }
}
