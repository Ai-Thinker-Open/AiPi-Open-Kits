#ifndef __IRSEND_TASK_H_
#define __IRSEND_TASK_H_
#include "chip_include.h"
#ifndef __IRSEND_TASK_C_
#define IRSEND_TASK_EXT extern
#else
#define IRSEND_TASK_EXT 
#endif


IRSEND_TASK_EXT TaskHandle_t irsend_task_xhandle;

IRSEND_TASK_EXT void irsend_task_process(void *msg);
#endif
