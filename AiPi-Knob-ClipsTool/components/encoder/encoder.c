/**
 * @file encoder.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-29
 *
 * @copyright Copyright (c) 2023
 *
*/

// #ifdef ENCODER_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bflb_gpio.h"
#include "touch.h"
#include "ble_hid_dev.h"
#include "custom.h"
#include "log.h"
#define DBG_TAG "ENCODER"

#define ENCODER_PUSH GPIO_PIN_24
#define ENCODER_B GPIO_PIN_28
#define ENCODER_A GPIO_PIN_26
static struct bflb_device_s* gpio;
static xTimerHandle ENCODER_1_timer;
static xTimerHandle ENCODER_2_timer;
static xTimerHandle ENCODER_3_timer;

static bool encoder_diff_enable = true;
void* encoder_task;

lv_obj_t* get_current_layer_obj(void);
lv_group_t* get_encoder_group(void);

/**
 * @brief encoder_timer_cb
 *   旋钮单独使用一个中断来识别会导致旋转方向识别不稳定，大概率出现误触，
 *   使用的定时器以1ms为单位计算另外一个输入的电平时间可以有效减小误触
 *   顺时针方向波形：
 * A:  _______      _______
 *            |____|       |_____
 * B:  __________      _______
 *               |____|       |____
 *
 * 逆时针方向波形：
 * A: ________      _______
 *            |____|       |_____
 * B:_______      ______
 *          |____|      |________
 *
 * @param xTimer
*/
static void encoder_timer_cb(TimerHandle_t xTimer)
{
    static int timer_cont;
    int timer_id = pvTimerGetTimerID(xTimer);

    switch (timer_id) {
        case 0: //顺时针方向定时器
            if (bflb_gpio_read(gpio, ENCODER_B)&&bflb_gpio_read(gpio, ENCODER_A)==0) timer_cont++; //当 A和B都满足顺时针状态时，开始增加及时
            else {
                xTimerStop(ENCODER_1_timer, portMAX_DELAY);
                // LOG_I("encoder B io Higt timer=%d ms", timer_cont);
                if (timer_cont>4)xTaskNotify(encoder_task, 1, eSetValueWithOverwrite); //当时B的高电平时间超过4ms,则方向为顺时针
                timer_cont = 0;
            }
            break;
        case 1: //逆时针方向定时器
            if (bflb_gpio_read(gpio, ENCODER_B)==0&&bflb_gpio_read(gpio, ENCODER_A)==0) timer_cont++; //当 A和B都满足逆时针状态时，开始增加及时
            else {
                xTimerStop(ENCODER_2_timer, portMAX_DELAY);
                // LOG_I("encoder B  io low timer=%d ms", timer_cont);
                if (timer_cont>4)xTaskNotify(encoder_task, 2, eSetValueWithOverwrite);//当时B的低电平时间超过4ms,则方向为顺时针
                timer_cont = 0;
            }
            break;
        case 2://按钮定时器
            if (bflb_gpio_read(gpio, ENCODER_PUSH)==0) {
                timer_cont++;
                if (timer_cont>300) {
                    timer_cont = 0;
                    xTimerStop(ENCODER_3_timer, portMAX_DELAY);
                    xTaskNotify(encoder_task, 4, eSetValueWithOverwrite);
                }
            }
            else {
                if (timer_cont< 250) {

                    xTimerStop(ENCODER_3_timer, portMAX_DELAY);
                    xTaskNotify(encoder_task, 3, eSetValueWithOverwrite);
                }
                timer_cont = 0;
            }

            break;
    }

}

static void gpio_isr(int irq, void* arg)
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    bool  intstatus = bflb_gpio_get_intstatus(gpio, ENCODER_PUSH);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, ENCODER_PUSH);
        xTimerStartFromISR(ENCODER_3_timer, &xHigherPriorityTaskWoken);
        encoder_diff_enable = false;
    }

    intstatus = bflb_gpio_get_intstatus(gpio, ENCODER_A);
    if (intstatus) {
        if (encoder_diff_enable) {
            if (bflb_gpio_read(gpio, ENCODER_B))
                xTimerStartFromISR(ENCODER_1_timer, &xHigherPriorityTaskWoken);
            else xTimerStartFromISR(ENCODER_2_timer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        bflb_gpio_int_clear(gpio, ENCODER_A);
    }

}


void button_process_task(void* param)
{
    static uint32_t io26_press_ms_cnt = 0;
    static uint32_t io28_press_ms_cnt = 0;
    static uint32_t btn_press_ms_cnt = 0;
    uint8_t press_mode = 0;
    uint8_t btn_press_mode = 0;
    static bool led_ctrl = 0;
    static bool button = 1;
    uint32_t frames = 1;
    uint8_t* pBuf = NULL;
    uint32_t len;
    uint8_t* pStr2 = NULL;
    uint32_t len2;
    bool long_press_flag = 0;

    lv_ui* ui = (lv_ui*)param;
    gpio = bflb_device_get_by_name("gpio");

    //创建定时器
    ENCODER_1_timer = xTimerCreate("encoder_b_timer", pdMS_TO_TICKS(1), pdTRUE, 0, encoder_timer_cb);//顺时针识别滤波定时器
    ENCODER_2_timer = xTimerCreate("encoder_BL_timer", pdMS_TO_TICKS(1), pdTRUE, 1, encoder_timer_cb);//逆时针识别滤波定时器
    ENCODER_3_timer = xTimerCreate("encoder_push_timer", pdMS_TO_TICKS(10), pdTRUE, 2, encoder_timer_cb);//长按识别


    bflb_gpio_init(gpio, ENCODER_PUSH, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, ENCODER_B, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, ENCODER_A, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE| GPIO_PULLUP);

    bflb_gpio_int_mask(gpio, ENCODER_PUSH, false);
    bflb_gpio_int_mask(gpio, ENCODER_A, false);

    bflb_irq_attach(gpio->irq_num, gpio_isr, gpio);
    bflb_irq_enable(gpio->irq_num);

    while (1) {
        xTaskNotifyWait(0xffffffff, 0x00, &press_mode, portMAX_DELAY);
        bflb_irq_disable(gpio->irq_num);
        LOG_F("encode read=%d", press_mode);
        vTaskDelay(50/portTICK_PERIOD_MS);
        bflb_irq_enable(gpio->irq_num);

        switch (press_mode) {
            case 1:
                if (ui->screen_type==0) {
                    lv_obj_set_tile_id(ui->screen_tileview_1, 1, 0, LV_DIR_LEFT);
                }
                else if (ui->screen_type>0) {
                    xTaskNotify((xTaskHandle)hid_task, HID_KEY_NUMBLE_ARROW_RIGHT, eSetValueWithOverwrite);
                }
                break;
            case 2:
                if (ui->screen_type==0) {
                    lv_obj_set_tile_id(ui->screen_tileview_1, 0, 0, LV_DIR_RIGHT);
                }
                else if (ui->screen_type>0) {
                    xTaskNotify((xTaskHandle)hid_task, HID_KEY_NUMBLE_ARROW_LEFT, eSetValueWithOverwrite);
                }
                break;
            case 3:
                if (ui->screen_type==0) {
                    lv_obj_t* tile_act = lv_tileview_get_tile_act(ui->screen_tileview_1);
                    if (tile_act==ui->tile_Pr) {
                        // printf("ls Pr tile\r\n");
                        lv_event_send(ui->screen_imgbtn_Pr, LV_EVENT_CLICKED, NULL);
                    }
                    else if (tile_act==ui->tile_JY) {
                        lv_event_send(ui->screen_imgbtn_JY, LV_EVENT_CLICKED, NULL);
                    }
                }
                encoder_diff_enable = true;
                break;
            case 4:
                if (ui->screen_type==2) {
                    lv_event_send(ui->screen_JY_img_jy, LV_EVENT_LONG_PRESSED, NULL);
                }
                else if (ui->screen_type==1) {
                    lv_event_send(ui->screen_Pr_img_pr, LV_EVENT_LONG_PRESSED, NULL);
                }
                encoder_diff_enable = true;
                break;
        }
    }
}


