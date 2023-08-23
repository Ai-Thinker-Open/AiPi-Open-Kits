#ifndef __APP_TASK_H_
#define __APP_TASK_H_
#include "chip_include.h"
#ifndef __APP_TASK_C_
#define APP_TASK_EXT extern
#else
#define APP_TASK_EXT 
#endif

APP_TASK_EXT TaskHandle_t app_task_xhandle;

// APP_TASK_EXT xQueueHandle battery_queue;
APP_TASK_EXT xQueueHandle button_queue;
APP_TASK_EXT xQueueHandle lvgl_queue;
// APP_TASK_EXT xQueueHandle irsend_queue;
// APP_TASK_EXT xQueueHandle power_queue;
// APP_TASK_EXT xQueueHandle voice_queue;


APP_TASK_EXT void app_task_process(void *msg);

#endif