/**
 * @file m62_rgb.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "timers.h"
#include "log.h"
#include "bflb_mtimer.h"
#include "bflb_pwm_v2.h"
#include "bflb_clock.h"
#include "board.h"
#include "bflb_gpio.h"

#define DBG_TAG "RGB"

// #define Ai_M62

#ifdef Ai_M62
#define RGB_RED GPIO_PIN_1
#define RGB_G GPIO_PIN_30
#define RGB_B GPIO_PIN_0

#define RGB_RED_PWM_CH PWM_CH1
#define RGB_GREEN_PWM_CH PWM_CH2
#define RGB_BLUE_PWM_CH PWM_CH0
#else
#define RGB_RED GPIO_PIN_12
#define RGB_G GPIO_PIN_14
#define RGB_B GPIO_PIN_15
#define RGB_RED_PWM_CH PWM_CH0
#define RGB_GREEN_PWM_CH PWM_CH2
#define RGB_BLUE_PWM_CH PWM_CH3

#endif

struct bflb_device_s* pwm;

unsigned short int color_red = 31;
unsigned short int color_green = 31;
unsigned short int color_blue = 31;
bool switch_status;
void dev_rgb_init(void)
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, RGB_RED, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, RGB_G, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, RGB_B, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);

    pwm = bflb_device_get_by_name("pwm_v2_0");

    /* period = .XCLK / .clk_div / .period = 40MHz / 40 / 1000 = 1KHz */
    struct bflb_pwm_v2_config_s cfg = {
        .clk_source = BFLB_SYSTEM_XCLK,
        .clk_div = 40,
        .period = 1000,
    };

    bflb_pwm_v2_init(pwm, &cfg);
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_RED_PWM_CH, 0, 0); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_GREEN_PWM_CH, 0, 500); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_BLUE_PWM_CH, 0, 600); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_channel_positive_start(pwm, RGB_RED_PWM_CH);
    bflb_pwm_v2_channel_positive_start(pwm, RGB_GREEN_PWM_CH);
    bflb_pwm_v2_channel_positive_start(pwm, RGB_BLUE_PWM_CH);

    bflb_pwm_v2_start(pwm);
}
/**
 * @brief  dev_rgb_ch_color
 *
 * @param red
 * @param green
 * @param blue
*/
void dev_rgb_ch_color(unsigned char red, unsigned char green, unsigned char blue)
{
    uint16_t color_red_period = 1000.0*((float)red/255.0);
    uint16_t color_green_period = 1000.0*((float)green/255.0);
    uint16_t color_blue_period = 1000.0*((float)blue/255.0);

    bflb_pwm_v2_stop(pwm);
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_RED_PWM_CH, 0, color_red_period); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_GREEN_PWM_CH, 0, color_green_period); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_channel_set_threshold(pwm, RGB_BLUE_PWM_CH, 0, color_blue_period); /* duty = (500-100)/1000 = 40% */
    bflb_pwm_v2_start(pwm);
}