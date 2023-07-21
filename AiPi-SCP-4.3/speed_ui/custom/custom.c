/**
 * @file custom.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/
#define CHART1_POINTS 100
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static int32_t speed = 0;
static bool is_increase = true;

static int16_t spd_chart[CHART1_POINTS] = {0};
static bool is_up = true;

/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

void speed_meter_timer_cb(lv_timer_t * t)
{
    lv_ui * gui = t->user_data;

    if(speed >= 110) is_increase = false;
    if(speed <= 70) is_increase = true;

    lv_meter_set_indicator_value(gui->speed_meter_board, gui->speed_meter_board_scale_1_ndline_0, speed);
    lv_label_set_text_fmt(gui->speed_label_digit, "%"LV_PRId32, speed);
    if(is_increase)
    {
        speed ++;
    }else
    {
        speed --;
    }
}

void record_chart_timer_cb(lv_timer_t * t)
{
    lv_obj_t * obj = t->user_data;

    lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
    lv_coord_t * ser_array = lv_chart_get_y_array(obj, ser);


    for(int i = 0; i < CHART1_POINTS - 1; i++)
    {
        spd_chart[i] = spd_chart[i+1];
        ser_array[i] = spd_chart[i];
    }

    if(spd_chart[CHART1_POINTS - 1] > 110) is_up = false;
    if(spd_chart[CHART1_POINTS - 1] < 70) is_up = true;

    if(is_up)
    {
        spd_chart[CHART1_POINTS - 1] += lv_rand(0, 5);
    }else
    {
        spd_chart[CHART1_POINTS - 1] -= lv_rand(0, 5);
    }
    ser_array[CHART1_POINTS - 1] = spd_chart[CHART1_POINTS - 1];
    lv_chart_refresh(obj);
}
