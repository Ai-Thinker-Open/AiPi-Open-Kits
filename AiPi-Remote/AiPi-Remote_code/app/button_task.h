#ifndef __BUTTON_TASK_H_
#define __BUTTON_TASK_H_
#include "chip_include.h"
#ifndef __BUTTON_TASK_C_
#define BUTTON_TASK_EXT extern
#else
#define BUTTON_TASK_EXT 
#endif

BUTTON_TASK_EXT TaskHandle_t button_task_xhandle;

BUTTON_TASK_EXT void button_task_process(void *msg);
#endif
