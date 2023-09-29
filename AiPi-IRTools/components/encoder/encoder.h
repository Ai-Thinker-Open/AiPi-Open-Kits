/**
 * @file encode.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-29
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef ENCODER_H
#define ENCODER_H

#include "bflb_core.h"
#include "touch.h"

extern void* encoder_task;

void button_process_task(void* param);
void encoderDeviceInit(signed int* encoder_diff, int* encoder_state);
// int indev_encoder_init(void);
#endif
