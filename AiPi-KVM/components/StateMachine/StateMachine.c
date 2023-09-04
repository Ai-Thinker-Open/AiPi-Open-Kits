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
#include "kvm_dev.h"

#define DBG_TAG "STATEMACHINE"
#define ULONG_MAX (0xffffffff)

void* StateMachine_Handle = NULL;

static void StateMachineTaskHandle(void* arg);
/**
 * @brief
 *
*/
void StateMachineTask_start(void)
{
    xTaskCreate(StateMachineTaskHandle, "StateMachineTask", 1024, NULL, 1, (xTaskHandle*)&StateMachine_Handle);
}
/**
 * @brief
 *
 * @param arg
*/
static void StateMachineTaskHandle(void* arg)
{
    uint32_t State;
    while (1) {
        xTaskNotifyWait(ULONG_MAX, 0, &State, portMAX_DELAY);
        aipi_kvm_set_HDIM_channel(State);
        switch (State) {
            case KVM_HDMI_CH_1:
                aipi_kvm_set_usb_channel(KVM_USB_CH_1);
                LOG_I("KVM device set: KVM_HDMI_CH%d", State+1);
                break;
            case KVM_HDMI_CH_2:
                aipi_kvm_set_usb_channel(KVM_USB_CH_2);
                LOG_I("KVM device set: KVM_HDMI_CH%d", State+1);
                break;
            case KVM_HDMI_CH_3:
                aipi_kvm_set_usb_channel(KVM_USB_CH_3);
                LOG_I("KVM device set: KVM_HDMI_CH%d", State+1);
                break;
        }
    }
}

/**
 * @brief
 *
 * @param key
 * @param value
*/
void flash_erase_set(char* key, char* value)
{
    size_t len = 0;
    int value_len = strlen(value);
    ef_set_and_save_env(key, value);
    // bflb_flash_read(key, flash_data, strlen(value));
    // printf("writer data:%s\r\n", flash_data);
    memset(value, 0, strlen(value));
    ef_get_env_blob(key, value, value_len, &len);
}
/**
 * @brief
 *
 * @param key
 * @return char*
*/
char* flash_get_data(char* key, uint32_t len)
{
    static char* flash_data;
    flash_data = pvPortMalloc(len);
    memset(flash_data, 0, len);

    ef_get_env_blob(key, flash_data, len, (size_t)&len);

    return flash_data;
}
