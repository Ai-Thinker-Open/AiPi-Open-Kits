/**
 * @file buttom_led.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-08
 *
 * @copyright Copyright (c) 2023
 *
*/

#ifndef BUTTOM_LED_H
#define BUTTOM_LED_H

#define LED_RED_IO 3
#define LED_GREEN_IO 23
#define LED_BLUE_IO 24

#define BUTTOM_RED_IO  25
#define BUTTOM_GREEN_IO  26
#define BUTTOM_BLUE_IO  28
#define BUTTOM_ALL_ON_IO 27
#define BUTTOM_ALL_OFF_IO 29


void aipi_scp_buttom_led_init(void);
#endif