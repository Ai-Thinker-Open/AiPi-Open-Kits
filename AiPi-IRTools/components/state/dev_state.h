/**
 * @file dev_state.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef DEV_STATE_H
#define DEV_STATE_H

#define IR_TYPE_T "IRTYPE"

typedef enum {
    DEV_STATE_DEF = 0,
    DEV_STATE_ENCODER_ROTATION_CLOCKWISE,
    DEV_STATE_ENCODER_ROTATION_ANTICLOCKWISE,
    DEV_STATE_ENCODER_BUTTON_CLICK,
    DEV_STATE_ENCODER_BUTTON_LONGPRESS,
    DEV_STATE_IR_RX_DONE,
    DEV_STATE_IR_TYPE_CHANGE,
    DEV_STATE_IR_SAVE_CODE,
    DEV_STATE_LV_EVENT_TETS,

}dev_state_t;

void dev_state_init(void* user_data);
void dev_state_send_notify(dev_state_t state);
#endif