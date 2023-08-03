#include "bflb_mtimer.h"
#include "bflb_pwm_v2.h"
#include "bflb_clock.h"
#include "bflb_gpio.h"
#include "board.h"

struct bflb_device_s *pwm;
struct bflb_device_s *gpio;
int main(void)
{
    int i;
    board_init();

    pwm = bflb_device_get_by_name("pwm_v2_0");
    gpio = bflb_device_get_by_name("gpio");

    /*初始化RGB灯，IO15、12、14，对应蓝灯、红灯、绿灯*/
    bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_12, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    
    /* period = .XCLK / .clk_div / .period = 40MHz / 40 / 1000 = 1KHz */
    struct bflb_pwm_v2_config_s cfg = {
        .clk_source = BFLB_SYSTEM_XCLK,
        .clk_div = 40,
        .period = 1000,
    };

    
    /*初始化PWM输出*/
    bflb_pwm_v2_init(pwm, &cfg);
    bflb_pwm_v2_start(pwm);


    /*功能实现，红、绿、蓝灯依次呼吸循环亮灭*/
    while (1) {

    //红灯呼吸亮灭    
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH0);
    for(i=100;i>0;i--)
    {

        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH0, i, 100); 
 
        bflb_mtimer_delay_ms(10);
    }
    for(i=1;i<100;i++)
    {
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH0, i, 100);

        bflb_mtimer_delay_ms(10);
    }
    bflb_pwm_v2_channel_positive_stop(pwm, PWM_CH0);



    //绿灯呼吸亮灭
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH2);
    for(i=100;i>0;i--)
    {
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, i, 100);

        bflb_mtimer_delay_ms(10);
    }    
     for(i=1;i<100;i++)
    {
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, i, 100);

        bflb_mtimer_delay_ms(10);
    }      
    bflb_pwm_v2_channel_positive_stop(pwm, PWM_CH2);


    //蓝灯循环亮灭
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH3);
    for(i=100;i>0;i--)
    {
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH3, i, 100);

        bflb_mtimer_delay_ms(10);
    }  
     for(i=1;i<100;i++)
    {
        bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH3, i, 100);

        bflb_mtimer_delay_ms(10);
    }    
    bflb_pwm_v2_channel_positive_stop(pwm, PWM_CH3);
    }
}
