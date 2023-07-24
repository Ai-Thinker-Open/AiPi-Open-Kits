/**
 * @file voice_uart.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-24
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <timers.h>
#include <task.h>
#include "custom.h"
#include "bflb_uart.h"

#define GBD_TAG "UART"


