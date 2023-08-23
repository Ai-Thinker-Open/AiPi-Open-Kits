#ifndef __VOICE_TASK_H_
#define __VOICE_TASK_H_
#include "chip_include.h"
#ifndef __VOICE_TASK_C_
#define VOICE_TASK_EXT extern
#else
#define VOICE_TASK_EXT 
#endif


VOICE_TASK_EXT TaskHandle_t voice_task_xhandle;

VOICE_TASK_EXT void voice_task_process(void *msg);
#endif
