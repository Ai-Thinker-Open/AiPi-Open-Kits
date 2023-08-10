/**
 * @file m62_rgb.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef M62_RGB_H
#define M62_RGB_H

void dev_rgb_init(void);
void dev_rgb_ch_color(unsigned char red, unsigned char green, unsigned char blue);

extern unsigned short int color_red;
extern unsigned short int color_green;
extern unsigned short int color_blue;

#endif