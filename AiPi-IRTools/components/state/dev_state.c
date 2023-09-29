/**
 * @file dev_state.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "log.h"

#include "dev_state.h"
#include "gui_guider.h"
#include "dev_ir.h"
#include "user_esflash.h"
#define DBG_TAG "DEV STATE"

xTaskHandle devTaskHandle;

static char* code_name[] = { "开关","模式","升温","降温","风向","风速" };

static void dev_state_task(void* arg)
{
    dev_state_t states;
    lv_ui* ui = (lv_ui*)arg;
    while (1) {
        xTaskNotifyWait(0xffffffff, 0, &states, portMAX_DELAY);

        switch (states)
        {
            case DEV_STATE_DEF:
            {

            }
            break;
            case DEV_STATE_ENCODER_ROTATION_CLOCKWISE:
            {
                LOG_I("DEV_STATE_ENCODER_ROTATION_CLOCKWISE");

            }
            break;
            case DEV_STATE_ENCODER_ROTATION_ANTICLOCKWISE:
            {
                LOG_I("DEV_STATE_ENCODER_ROTATION_ANTICLOCKWISE");
            }
            break;
            case DEV_STATE_ENCODER_BUTTON_CLICK:
            {
                LOG_I("DEV_STATE_ENCODER_BUTTON_CLIC");
            }
            break;
            case DEV_STATE_ENCODER_BUTTON_LONGPRESS:
            {
                LOG_I("DEV_STATE_ENCODER_BUTTON_LONGPRESS");
            }
            break;
            case DEV_STATE_IR_RX_DONE:
            {
                LOG_I("DEV_STATE_IR_RX_DONE:0x%llX", deviceIRGetCodeValue());
                if (!ui->Home_del) {
                    lv_label_set_text_fmt(ui->Home_ta_ircmd, "0x%08X", deviceIRGetCodeValue());
                }
            }
            break;
            case DEV_STATE_IR_TYPE_CHANGE:
            {
                LOG_I("DEV_STATE_IR_TYPE_CHANGE value=%d", dev_ir_type);
                device_ir_init(dev_ir_type);
                ef_set_int(IR_TYPE_T, dev_ir_type);
            }
            break;
            case DEV_STATE_LV_EVENT_TETS:
            {
                LOG_I("DEV_STATE_LV_EVENT_TETS");
            }
            break;
            case DEV_STATE_IR_SAVE_CODE:
            {
                LOG_I("DEV_STATE_IR_SAVE_CODE:0x%llX", deviceIRGetCodeValue());
                irListPushBack(irCreateCodeListNode(ir_code_type, code_name[ir_code_type], deviceIRGetCodeValue()));
                irAllNode_log();
            }
            break;
            default:
                break;
        }
    }
}
/**
 * @brief 初始化任务
 *
*/
void dev_state_init(void* user_data)
{

    BaseType_t ret = xTaskCreate(dev_state_task, "state task", 1024*2, user_data, 8, &devTaskHandle);
    if (ret==pdTRUE)LOG_I("state task Create OK");
    else LOG_E("state task Create fail");
}
/**
 * @brief dev_state_send_notify
 *      发送任务通知
 * @param state 通知类型
*/
void dev_state_send_notify(dev_state_t state)
{
    BaseType_t ret;
    if (dev_state_task==NULL) {
        LOG_E("dev_state_init is not running");
        return;
    }
    ret = xTaskNotify(devTaskHandle, state, eSetValueWithoutOverwrite);
    if (ret==pdTRUE) LOG_I("Notify send OK");
    else LOG_E("Notify send fail");
}

