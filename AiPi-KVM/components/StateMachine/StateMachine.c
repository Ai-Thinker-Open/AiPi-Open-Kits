/**
 * @file StateMachine.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"

#define DBG_TAG "STATEMACHINE"
#define ULONG_MAX (0xffffffff)

TaskHandle_t StateMachine_Handle = NULL;

static void StateMachineTaskHandle(void* arg);

void StateMachineTask_start(void)
{
    xTaskCreate(StateMachineTaskHandle, "StateMachineTask", 1024, NULL, 1, &StateMachine_Handle);
}

static void StateMachineTaskHandle(void* arg)
{
    uint32_t State;
    while (1) {
        xTaskNotifyWait(ULONG_MAX, 0, &State, portMAX_DELAY);
        LOG_F("GET Notift=%d", State);
    }
}
