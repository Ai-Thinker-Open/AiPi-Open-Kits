/**
 * @file StateMachine.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define KVM_CH "kvm_ch"

extern void* StateMachine_Handle;

void StateMachineTask_start(void);
void flash_erase_set(char* key, char* value);
char* flash_get_data(char* key, uint32_t len);
#endif