#ifndef __APP_BLE_H_
#define __APP_BLE_H_

#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "queue.h"

#include "bluetooth.h"
#include "hci_driver.h"
#include "hci_core.h"
#include "btble_lib_api.h"
#include "conn.h"
#include "conn_internal.h"
#include "gatt.h"
#include "bl_port.h"
#include "bl616_glb.h"

#include "bas.h"
#include "dis.h"
#include "hog_kb.h"
#ifndef __APP_BLE_C_
#define APP_BLE_EXT extern
#else
#define APP_BLE_EXT
#endif

#define APPBLE_PROCESS_STACK_SIZE  (1024)
#define APPBLE_PROCESS_PRIORITY (15)

APP_BLE_EXT TaskHandle_t app_ble_process_task_hd;

APP_BLE_EXT void app_ble_task(void* msg);
APP_BLE_EXT int btblecontroller_em_config(void);


#endif
