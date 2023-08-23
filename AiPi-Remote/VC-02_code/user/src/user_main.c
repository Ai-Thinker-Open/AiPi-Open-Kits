/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : user_main.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.03
 *
 **************************************************************************/
#include "user_main.h"

#define TAG "user_main"

#if USER_RUN_DEMO

extern int hb_uart(void);
extern int hb_uart_ucp(void);
extern int hb_smart_ac(void);
extern int hb_housekeeper(void);
extern int hb_key_wakeup(void);
extern int hb_adc_ctrl(void);
extern int hb_timer_buzzer(void);
extern int hb_pwm_led(void);
extern int hb_flash_example(void);
extern int hb_asr_control(void);
extern int hb_player(void);
extern int hb_sw_timer(void);
extern int hb_uart_sucp(void);
extern int hb_auto_gpio(void);
extern int hb_power_sleep(void);

int user_main(void) {
#if (USER_RUN_DEMO_SELECT == USER_DEMO_SMART_AC)
  return hb_smart_ac();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_HOUSEKEEPER)
  return hb_housekeeper();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_KEY_WAKEUP)
  return hb_key_wakeup();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_ADC_CTRL)
  return hb_adc_ctrl();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_TIMERS)
  return hb_sw_timer();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_BUZZER)
  return hb_timer_buzzer();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_PWM_LED)
  return hb_pwm_led();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_I2C_TIME)
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_SPI_LCD)
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_UART)
  return hb_uart();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_UART_UCP)
  return hb_uart_ucp();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_PLAYER)
  return hb_player();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_FLASH)
  return hb_flash();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_ASR_CONTROL)
  return hb_asr_control();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_UART_SUCP)
  return hb_uart_sucp();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_AUTO_GPIO)
  return hb_auto_gpio();
#elif (USER_RUN_DEMO_SELECT == USER_DEMO_POWER_SLEEP)
  return hb_power_sleep();
#else
  return 0;
#endif
}

void user_exit(void) {
}

#else

extern int user_start_init(void);

int user_main(void) {
  /* add your code on user_start.c to run */
  user_start_init();
  return 0;
}


void user_exit(void) {
  /* TODO: add your code if something need to be free when app exit */
}

#endif

