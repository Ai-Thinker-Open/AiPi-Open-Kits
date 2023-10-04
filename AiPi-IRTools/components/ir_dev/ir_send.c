#define __IR_SEND_C_
#include "ir_send.h"
#include "module_wireless.h"
#include "bflb_gpio.h"
#include "dev_ir.h"

TaskHandle_t ir_send_system_task_xhandle;

module_wireless_send_type_t module_wireless_send;
module_wireless_send_config_t module_wireless_send_config;
module_wireless_dat_t module_wireless_send_nec_dat_one;
module_wireless_dat_t module_wireless_send_nec_dat_zero;
module_wireless_dat_t module_wireless_send_nec_start;
module_wireless_dat_t module_wireless_send_nec_end;

static struct bflb_device_s* gpio;


void module_wireless_send_config_delay(void)
{
    arch_delay_us(5);
    // vTaskDelay(2 / portTICK_RATE_MS);

}
void module_wireless_send_config_38k_delay(void)
{
    arch_delay_us(12);
}
void module_wireless_send_config_write(uint8_t status)
{
    if (status)
    {
        bflb_gpio_reset(gpio, IR_TX_IO);
    }
    else
    {
        bflb_gpio_set(gpio, IR_TX_IO);
    }
}

extern int8_t module_wireless_send_38k_low_write_end(struct module_wireless_send_type* this, module_wireless_dat_t* module_wireless_send_dat);

// void ir_send_system_task(void* p_arg)
// {
//     printf("ir_send_system_task\r\n");

//     // gpio = bflb_device_get_by_name("gpio");
//     // bflb_gpio_init(gpio, GPIO_PIN_13, GPIO_OUTPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0); //tx io13
//     // bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);  //rx io11

//     uint8_t addr = 0x55;
//     uint8_t commxx = 0x88;
//     while (1)
//     {
//         module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);
//         module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, addr);
//         module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~addr);
//         module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, commxx);
//         module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~commxx);
//         module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_start);
//         vTaskDelay(1000 / portTICK_RATE_MS);

//         // bflb_gpio_set(gpio, GPIO_PIN_13);
//         // vTaskDelay(3000 / portTICK_RATE_MS);
//         // bflb_gpio_reset(gpio, GPIO_PIN_13);
//         // vTaskDelay(3000 / portTICK_RATE_MS);
//     }
// }

void irTxInitConfig(void)
{
    module_wireless_send_config.module_wireless_send_config_38k_delay = module_wireless_send_config_38k_delay;
    module_wireless_send_config.module_wireless_send_config_delay = module_wireless_send_config_delay;
    module_wireless_send_config.module_wireless_send_config_write = module_wireless_send_config_write;
    module_wireless_send_create(&module_wireless_send, &module_wireless_send_config);

    module_wireless_send_nec_dat_one.encoding_low = 65;
    module_wireless_send_nec_dat_one.encoding_high = 100;
    module_wireless_send_nec_dat_one.direction = 0;

    module_wireless_send_nec_dat_zero.encoding_low = 22;
    module_wireless_send_nec_dat_zero.encoding_high = 100;
    module_wireless_send_nec_dat_zero.direction = 0;

    module_wireless_send_nec_start.encoding_low = 172;
    module_wireless_send_nec_start.encoding_high = 1600;
    module_wireless_send_nec_start.direction = 0;

    module_wireless_send_nec_end.encoding_low = 22;
    module_wireless_send_nec_end.encoding_high = 800;
    module_wireless_send_nec_end.direction = 0;

    gpio = bflb_device_get_by_name("gpio");
}

void irTx_send_code(uint64_t coder)
{
    uint8_t ir_ircode[16] = { 0 };
    size_t i = 0;
    printf("[%s:%d]", __func__, __LINE__);
    for (; coder>0; i++)
    {
        ir_ircode[i] += coder&0xff;
        coder >>= 8;
        printf("0x%02X ", ir_ircode[i]);
    }
    printf("\r\n");
    module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0xb2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0xb2);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0x7b);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0x7b);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0xe0);
    module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0xe0);
    module_wireless_send.module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_end);

    // module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0xb2);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0xb2);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0x7b);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0x7b);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, 0xe0);
    // module_wireless_send.module_wireless_send_38k_low_write_high_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~0xe0);
    // module_wireless_send.module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_end);
    // module_wireless_send.module_wireless_send_38k_low_write_start(&module_wireless_send, &module_wireless_send_nec_start);

    // for (size_t j = 0; j < i; j++)
    // {
    //     module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ir_ircode[j]);
    //     module_wireless_send.module_wireless_send_38k_low_write_byte(&module_wireless_send, &module_wireless_send_nec_dat_zero, &module_wireless_send_nec_dat_one, ~ir_ircode[j]);
    // }
    // module_wireless_send_38k_low_write_end(&module_wireless_send, &module_wireless_send_nec_start);
}