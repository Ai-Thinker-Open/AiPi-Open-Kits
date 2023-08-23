#define __POWER_TASK_C_
#include "power_task.h"

TaskHandle_t power_task_xhandle;
struct bflb_device_s *adc;

struct bflb_adc_channel_s chan[] = {
    {.pos_chan = ADC_CHANNEL_7,
     .neg_chan = ADC_CHANNEL_GND},
};

volatile uint32_t raw_data[1];
volatile uint8_t read_count = 0;

uint8_t batt_status;

uint8_t arc_flag = 0;
uint8_t arc_count = 0;

void adc_isr(int irq, void *arg)
{
    uint32_t intstatus = bflb_adc_get_intstatus(adc);
    if (intstatus & ADC_INTSTS_ADC_READY)
    {
        bflb_adc_int_clear(adc, ADC_INTCLR_ADC_READY);
        uint8_t count = bflb_adc_get_count(adc);
        for (size_t i = 0; i < count; i++)
        {
            raw_data[read_count] = bflb_adc_read_raw(adc);
            read_count++;
        }
    }
}

void power_task_process(void *msg)
{
    // board_adc_gpio_init();
    uint16_t value = 0xA6;
    struct bflb_device_s *gpio;

    gpio = bflb_device_get_by_name("gpio");

    /* ADC_CH7 */
    bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_ANALOG | GPIO_SMT_EN | GPIO_DRV_0);

    adc = bflb_device_get_by_name("adc");

    /* adc clock = XCLK / 2 / 32 */
    struct bflb_adc_config_s cfg;
    cfg.clk_div = ADC_CLK_DIV_32;
    cfg.scan_conv_mode = true;
    cfg.continuous_conv_mode = false;
    cfg.differential_mode = false;
    cfg.resolution = ADC_RESOLUTION_16B;
    cfg.vref = ADC_VREF_3P2V;

    bflb_adc_init(adc, &cfg);
    bflb_adc_channel_config(adc, chan, 1);
    bflb_adc_rxint_mask(adc, false);
    bflb_irq_attach(adc->irq_num, adc_isr, NULL);
    bflb_irq_enable(adc->irq_num);

    while (1)
    {
        read_count = 0;
        bflb_adc_start_conversion(adc);

        while (read_count < 1)
        {
            bflb_mtimer_delay_ms(1);
        }
        for (size_t j = 0; j < 1; j++)
        {
            struct bflb_adc_result_s result;
            // printf("raw data:%08x\r\n", raw_data[j]);
            bflb_adc_parse_result(adc, (uint32_t *)&raw_data[j], &result, 1); // 转换函数
            if(result.millivolt < 1900)
            {
                batt_status = 0;
            }
            else if(result.millivolt < 2000)
            {
                batt_status = 1;
            }
            else 
            {
                batt_status = 2;
            }
            // printf("pos chan %d,%d mv \r\n", result.pos_chan, result.millivolt);
        }
        bflb_adc_stop_conversion(adc);

        if(arc_flag)
        {
            arc_count ++;
            if(arc_count > 2)
            {
                arc_count = 0;
                arc_flag = 0;
                xQueueSend(button_queue, &value, 0);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
