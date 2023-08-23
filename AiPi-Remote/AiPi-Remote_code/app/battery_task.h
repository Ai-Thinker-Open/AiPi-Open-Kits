#ifndef __BATTERY_TASK_H_
#define __BATTERY_TASK_H_
#include "chip_include.h"
#ifndef __BATTERY_TASK_C_
#define BATTERY_TASK_EXT extern
#else
#define BATTERY_TASK_EXT 
#endif


BATTERY_TASK_EXT TaskHandle_t battery_task_xhandle;

BATTERY_TASK_EXT void battery_task_process(void *msg);
#endif