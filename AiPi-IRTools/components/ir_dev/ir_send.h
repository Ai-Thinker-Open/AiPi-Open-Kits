#ifndef __IR_SEND_H_
#define __IR_SEND_H_
// #include "chip_include.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#ifndef __IR_SEND_C_
#define IR_SEND_EXT extern
#else
#define IR_SEND_EXT
#endif

IR_SEND_EXT TaskHandle_t ir_send_system_task_xhandle;

IR_SEND_EXT void ir_send_system_task(void* p_arg);
void irTxInitConfig(void);
void irTx_send_code(uint64_t coder);
#endif
