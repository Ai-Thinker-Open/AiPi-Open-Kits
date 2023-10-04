/**
 * @file dev_ir.c
 * @author your name (you@domain.com)
 * @brief BLE618  IR TX only support GPIO 11,RX support GPIO 11 ~ GPIO 13
 * @version 0.1
 * @date 2023-09-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "log.h"
#include "bflb_ir.h"
#include "bflb_gpio.h"
#include "dev_ir.h"
#include "hardware/ir_reg.h"
#include "dev_state.h"
#include "user_esflash.h"


#define DBG_TAG "DEV IR"



struct bflb_device_s* irrx;

struct bflb_device_s* gpio;
struct bflb_ir_rx_config_s rx_cfg;
device_ir_type_t dev_ir_type;
ir_code_t ir_code_type;
static xTaskHandle ir_task;

dev_ir_t* pIRList = NULL;

static uint64_t ir_data = 0;

uint8_t __user_ir_receive(struct bflb_device_s* dev, uint64_t* data)
{
    uint32_t reg_base;
    uint32_t regval;

    bflb_ir_rxint_clear(dev);

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    regval |= IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    while ((bflb_ir_get_rxint_status(dev) & IR_RX_INTSTS_END) == 0) {
        /* Waiting for receiving */
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    regval &= ~IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    bflb_ir_rxint_clear(dev);

    regval = getreg32(reg_base + IRRX_DATA_COUNT_OFFSET) & IR_STS_IRRX_DATA_CNT_MASK;
    if (regval <= 32) {
        *data = getreg32(reg_base + IRRX_DATA_WORD0_OFFSET);
    }
    else {
        *data = getreg32(reg_base + IRRX_DATA_WORD0_OFFSET) | (uint64_t)getreg32(reg_base + IRRX_DATA_WORD1_OFFSET) << 32;
    }

    return regval;
}

static void ir_rx_task(void* arg)
{
    uint8_t rx_len;
    static uint64_t data = 0;
    while (1) {
        rx_len = __user_ir_receive(irrx, &data);
        if (rx_len>0&&data>0xffff) {
            //
            ir_data = data;
            LOG_I("ir  data len=%d:0x%llX,%llu", rx_len, ir_data, ir_data);
            char* code_str = pvPortMalloc(32);
            memset(code_str, 0, 32);
            sprintf(code_str, "%llu", ir_data);
            flash_erase_set(CODE_U64, code_str);
            vPortFree(code_str);
            dev_state_send_notify(DEV_STATE_IR_RX_DONE);

        }
        // vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint64_t deviceIRGetCodeValue(void)
{

    return (uint64_t)atoll(flash_get_data(CODE_U64, 16));
}

void device_ir_init(device_ir_type_t ir_type)
{
    if (rx_cfg.rx_mode==ir_type&& ir_task!=NULL) return;
    else if (rx_cfg.rx_mode!=ir_type&& ir_task!=NULL) {
        device_ir_deinit();
    }

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, IR_TX_IO, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    bflb_gpio_init(gpio, IR_RX_IO, GPIO_INPUT | GPIO_SMT_EN | GPIO_DRV_0);
    GLB_IR_RX_GPIO_Sel(IR_RX_IO);

    irrx = bflb_device_get_by_name("irrx");
    if (irrx==NULL) {
        LOG_E("IR RX init error");
        return;
    }


    switch (ir_type)
    {
        case DEVICE_IR_TPYE_NEC:
            rx_cfg.rx_mode = IR_RX_NEC;
            break;
        case DEVICE_IR_TPYE_RC5:
            rx_cfg.rx_mode = IR_RX_RC5;
        break;case DEVICE_IR_TPYE_SWM:
            rx_cfg.rx_mode = IR_RX_SWM;
            break;
    }
    dev_ir_type = ir_type;
    rx_cfg.input_inverse = true;
    rx_cfg.deglitch_enable = true;
    rx_cfg.end_threshold = 32;

    bflb_ir_rx_init(irrx, &rx_cfg);
    bflb_ir_rx_enable(irrx, true);
    // bflb_ir_rxint_mask(irrx, IR_RX_INTEN_END|IR_RX_INTEN_FER|IR_RX_INTEN_FIFO, false);
    // bflb_irq_attach(irrx->irq_num, irrx_isr, NULL);
    // bflb_irq_enable(irrx->irq_num);
    xTaskCreate(ir_rx_task, "rx task", 1024, NULL, 8, &ir_task);

    LOG_I("IR enable susccse");
}

void device_ir_deinit(void)
{

    bflb_ir_rx_enable(irrx, false);
    bflb_irq_disable(irrx->irq_num);
    bflb_gpio_deinit(gpio, IR_RX_IO);
    bflb_gpio_deinit(gpio, IR_TX_IO);
    memset(&rx_cfg, 0, sizeof(rx_cfg));
    // bflb_ir_rx_init(irrx, &rx_cfg);
    vTaskDelete(ir_task);
}

void device_ir_rea_enable(int enable)
{
    bflb_ir_rx_enable(irrx, enable);
}

/**
 * @brief 创建一个红外码节点
 *
 * @param code
 * @return dev_ir_t*
*/
dev_ir_t* irCreateCodeListNode(ir_code_t code_type, char* name, uint64_t code_numble)
{
    dev_ir_t* typeList = pIRList;
    //检查是否存在该节点
    if (typeList!=NULL) {
        while (typeList!=NULL) {
            if (typeList->code_type==code_type) {
                LOG_E("The node already exists:node type=%d", code_type);
                return NULL;
            }
            typeList = typeList->next;
        }
    }
    dev_ir_t* newNode = (dev_ir_t*)pvPortMalloc(sizeof(dev_ir_t));
    newNode->code_name = name;
    newNode->code = code_numble;
    newNode->code_type = code_type;
    newNode->next = NULL;
    return newNode;
}

uint64_t irCodeListGetCode(ir_code_t code_type)
{
    dev_ir_t* pCode = pIRList;
    if (pCode==NULL) {
        LOG_E("The Code List is NULL");
        return 0;
    }
    while (pCode!=NULL) {

    }
}

void irListPushBack(dev_ir_t* listcode)
{
    if (listcode==NULL) {
        LOG_I("list code is NULL");
        return;
    }
    if (pIRList==NULL)pIRList = listcode;
    else {
        dev_ir_t* newCode = pIRList;
        while (newCode->next!=NULL)
            newCode = newCode->next;

        newCode->next = listcode;
    }
}

void irChangeCode(ir_code_t code_type, uint32_t codeNuble)
{
    if (pIRList==NULL) {
        LOG_E("pIRList is NULL");
        return;
    }
    dev_ir_t* typeNode = pIRList;

    while (typeNode->code_type!=code_type)
        typeNode = typeNode->next;

    typeNode->code = codeNuble;
}

void irAllNode_log(void)
{
    if (pIRList==NULL) {
        LOG_E("pIRList is NULL");
        return;
    }
    dev_ir_t* typeNode = pIRList;

    printf("%s-%d:", __func__, __LINE__);
    while (typeNode!=NULL) {
        printf("[%d]%s:0x%llX ", typeNode->code_type, typeNode->code_name, typeNode->code);
        typeNode = typeNode->next;
    }
    printf("\r\n");
}

