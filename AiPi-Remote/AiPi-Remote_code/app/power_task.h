#ifndef __POWER_TASK_H_
#define __POWER_TASK_H_
#include "chip_include.h"
#ifndef __POWER_TASK_C_
#define POWER_TASK_EXT extern
#else
#define POWER_TASK_EXT 
#endif


POWER_TASK_EXT TaskHandle_t power_task_xhandle;

POWER_TASK_EXT void power_task_process(void *msg);
#endif
