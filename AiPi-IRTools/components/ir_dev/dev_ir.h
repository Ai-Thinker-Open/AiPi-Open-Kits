/**
 * @file dev_ir.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef DEV_IR_H
#define DEV_IR_H

#define CODE_U64 "CODEU64"

typedef enum {
    DEVICE_IR_TPYE_NEC = 0,
    DEVICE_IR_TPYE_RC5,
    DEVICE_IR_TPYE_SWM
}device_ir_type_t;

typedef enum {
    IR_CODE_SWITCH = 0,
    IR_CODE_MODE,
    IR_CODE_TEMP_UP,
    IR_CODE_TEMP_DOWN,
    IR_CODE_FAN_DIR,
    IR_CODE_FAN_SPEED,
}ir_code_t;

typedef struct dev_ir
{
    char* code_name;
    uint64_t code;
    ir_code_t code_type;
    struct dev_ir* next;
}dev_ir_t;


extern  device_ir_type_t dev_ir_type;
extern ir_code_t ir_code_type;
extern dev_ir_t* pIRList;
void device_ir_init(device_ir_type_t ir_type);
void device_ir_deinit(void);
uint64_t deviceIRGetCodeValue(void);
dev_ir_t* irCreateCodeListNode(ir_code_t code_type, char* name, uint64_t code_numble);
void irListPushBack(dev_ir_t* listcode);
void irChangeCode(ir_code_t code_type, uint32_t codeNuble);
void irAllNode_log(void);
#endif