/**
 * @file kvm_dev.c
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
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bflb_gpio.h"
#include "bflb_dac.h"
#include "StateMachine.h"
#include "kvm_dev.h"
#include "easyflash.h"
#include "log.h"
#define DBG_TAG "KVM_DEV"
//USB 通道切换IO
#define KVM_USB_EN GPIO_PIN_16
#define KVM_USB_IN0 GPIO_PIN_14
#define KVM_USB_IN1 GPIO_PIN_12

#define KVM_USB_HUB_EN GPIO_PIN_18
//HDMI 通道输入读取 IO
#define KVM_HDMI_IN_CH1 GPIO_PIN_26
#define KVM_HDMI_IN_CH2 GPIO_PIN_28
#define KVM_HDMI_IN_CH3 GPIO_PIN_24
//HDMI 切换按键 IO
#define KVM_HDMI_CTRL_CH1 GPIO_PIN_34
#define KVM_HDMI_CTRL_CH2 GPIO_PIN_33
#define KVM_HDMI_CTRL_CH3 GPIO_PIN_32
//HDMI 通道控制IO
#define KVM_HDIM_CTRL_DAC GPIO_PIN_3

struct bflb_device_s* gpio;
struct bflb_device_s* dac;


static void gpio_isr(int irq, void* arg)
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    bool  intstatus = bflb_gpio_get_intstatus(gpio, KVM_HDMI_CTRL_CH1);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, KVM_HDMI_CTRL_CH1);
        xTaskNotifyFromISR(StateMachine_Handle, KVM_HDMI_CH_1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }

    intstatus = bflb_gpio_get_intstatus(gpio, KVM_HDMI_CTRL_CH2);
    if (intstatus) {

        bflb_gpio_int_clear(gpio, KVM_HDMI_CTRL_CH2);
        xTaskNotifyFromISR(StateMachine_Handle, KVM_HDMI_CH_2, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }

    intstatus = bflb_gpio_get_intstatus(gpio, KVM_HDMI_CTRL_CH3);
    if (intstatus) {

        bflb_gpio_int_clear(gpio, KVM_HDMI_CTRL_CH3);
        xTaskNotifyFromISR(StateMachine_Handle, KVM_HDMI_CH_3, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }

}
void aipi_kvm_dev_init(void)
{
    gpio = bflb_device_get_by_name("gpio");
    dac = bflb_device_get_by_name("dac");
    //USB 切换初始化
    bflb_gpio_init(gpio, KVM_USB_EN, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, KVM_USB_IN0, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, KVM_USB_IN1, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    //USB HUB 控制初始化
    bflb_gpio_init(gpio, KVM_USB_HUB_EN, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    //HDMI 输入识别IO初始化
    bflb_gpio_init(gpio, KVM_HDMI_IN_CH1, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, KVM_HDMI_IN_CH2, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_init(gpio, KVM_HDMI_IN_CH3, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);

    //HDMI 按键识别初始化
    bflb_gpio_int_init(gpio, KVM_HDMI_CTRL_CH1, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_init(gpio, KVM_HDMI_CTRL_CH2, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_init(gpio, KVM_HDMI_CTRL_CH3, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, KVM_HDMI_CTRL_CH1, false);
    bflb_gpio_int_mask(gpio, KVM_HDMI_CTRL_CH2, false);
    bflb_gpio_int_mask(gpio, KVM_HDMI_CTRL_CH3, false);
    bflb_irq_attach(gpio->irq_num, gpio_isr, gpio);
    bflb_irq_enable(gpio->irq_num);
    //HDMI DAC初始化
    bflb_dac_init(dac, DAC_CLK_DIV_16);

    bflb_dac_channel_enable(dac, DAC_CHANNEL_A);

    char* kvm_ch = flash_get_data(KVM_CH, 1);
    LOG_I("KVM start read kvm channel:KVM_HDMI_CH%d", *kvm_ch+1);
    aipi_kvm_set_HDIM_channel(*kvm_ch);
    aipi_kvm_set_usb_channel(*kvm_ch);
    vPortFree(kvm_ch);
}
/**
 * @brief aipi_kvm_set_usb_channel
 *
 * @param kvm_usb_ch
*/
void aipi_kvm_set_usb_channel(kvm_usb_ch_t kvm_usb_ch)
{
    //关闭切换输入
    bflb_gpio_set(gpio, KVM_USB_EN);
    bflb_gpio_set(gpio, KVM_USB_HUB_EN);
    // vTaskDelay(200/portTICK_PERIOD_MS);
    bflb_mtimer_delay_ms(200);
    LOG_I("KVM redy set USB ch%d", kvm_usb_ch+1);
    switch (kvm_usb_ch)
    {
        case KVM_USB_CH_1:
        {
            bflb_gpio_reset(gpio, KVM_USB_IN0);
            bflb_gpio_reset(gpio, KVM_USB_IN1);
        }
        break;

        case KVM_USB_CH_2:
        {
            bflb_gpio_set(gpio, KVM_USB_IN0);
            bflb_gpio_reset(gpio, KVM_USB_IN1);
        }
        break;
        case KVM_USB_CH_3:
        {
            bflb_gpio_reset(gpio, KVM_USB_IN0);
            bflb_gpio_set(gpio, KVM_USB_IN1);
        }
        break;
        default:
            break;
    }
    //使能输入
    bflb_gpio_reset(gpio, KVM_USB_EN);
    bflb_mtimer_delay_ms(200);
    // vTaskDelay(200/portTICK_PERIOD_MS);
    //打开HUB
    bflb_gpio_reset(gpio, KVM_USB_HUB_EN);
}
/**
 * @brief aipi_kvm_set_HDIM_channel
 *
 * @param kvm_ch
*/
void aipi_kvm_set_HDIM_channel(kvm_hdmi_ch_t kvm_ch)
{
    //计算公式： val = (1.8V-0.2V) * digital_val/4095 + 0.2V
    // digital_val= (val-0.2)*4095/(1.8V-0.2V) 
    uint16_t  digital_val = 0;
    uint16_t val = 0;
    LOG_I("KVM redy set ch%d", kvm_ch+1);
    switch (kvm_ch)
    {
        case KVM_HDMI_CH_1:
            val = 2;

            break;
        case KVM_HDMI_CH_2:
        {
            val = 9;
        }
        break;
        case KVM_HDMI_CH_3:
        {
            val = 18;
        }
        break;
    }
    digital_val = (val-2)*4095/(18-2);
    bflb_dac_set_value(dac, DAC_CHANNEL_A, digital_val); //200mV，满值 0.2~1.8V

    flash_erase_set(KVM_CH, &kvm_ch);
}