#ifndef __IR_RECEIVE_H_
#define __IR_RECEIVE_H_
#include "chip_include.h"
#ifndef __IR_RECEIVE_C_
#define IR_RECEIVE_EXT extern
#else
#define IR_RECEIVE_EXT
#endif

IR_RECEIVE_EXT TaskHandle_t ir_receive_system_task_xhandle;

IR_RECEIVE_EXT void ir_receive_system_task(void *p_arg);

#endif
