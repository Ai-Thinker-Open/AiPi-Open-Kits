#ifndef __RADAR_TASK_H_
#define __RADAR_TASK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "log.h"
#include "board.h"
#include "mem.h"

//easy flash
#include "easyflash.h"
#include "bflb_mtd.h"
#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
//lvgl
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_user_config.h"
//guiGuider
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "buttom_led.h"
//esay flashg
#include "user_esflash.h"

#include "radar.h"

//voice
#include "dev_8388_pcm.h"
#ifndef __RADAR_TASK_C_
#define RADAR_TASK_EXT extern
#else
#define RADAR_TASK_EXT 
#endif


RADAR_TASK_EXT TaskHandle_t radar_task_xhandle;

RADAR_TASK_EXT void radar_task_process(void *msg);
#endif




