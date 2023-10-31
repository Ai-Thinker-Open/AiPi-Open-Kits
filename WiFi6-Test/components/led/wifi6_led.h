/**
 * @file wifi6_led.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef WIFI6_LED_H
#define WIFI6_LED_H

#define LED_CONFIG_NET 14
#define LED_DEV 15

void led_init(void);
void led_indicator_change(int led_state);
void led_dev_open(int* led_state);
void led_dev_cloes(int* led_state);


#endif