/**************************************************************************
 * Copyright (C) 2020-2020  Unisound
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
 * Description : user_gpio.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.24
 *
 **************************************************************************/
#include "user_gpio.h"
#include "uni_hal_gpio.h"
#include "uni_hal_adc.h"
#include "uni_timer.h"
#include "irqn.h"

#define TAG "user_gpio"

#define MAX_PULSE_SUPPORT   10

enum   __ADC_DC_CHANNEL_SEL { 
  ADC_CHANNEL_VIN = 0,
  ADC_CHANNEL_VDDA,
  ADC_CHANNEL_VDD1V2,
  ADC_CHANNEL_GPIOA17,
  ADC_CHANNEL_GPIOA18,
  ADC_CHANNEL_GPIOA23,
  ADC_CHANNEL_GPIOA24,
  ADC_CHANNEL_GPIOA25,
  ADC_CHANNEL_GPIOA26,
  ADC_CHANNEL_GPIOA27,
  ADC_CHANNEL_GPIOA28,
  ADC_CHANNEL_GPIOA29,
  ADC_CHANNEL_GPIOA30,
  ADC_CHANNEL_GPIOA31,
  ADC_CHANNEL_GPIOB6,
  ADC_CHANNEL_GPIOB7,
  ADC_CHANNEL_INVALID
};

typedef struct {
  TimerHandle     timer;
  GPIO_NUMBER     num;
  int             cycle;
  int             times;
  int             def_val;
  uni_bool        is_top;
}gpio_pulse_t;

static gpio_pulse_t *g_gpio_pulse = NULL;

static _gpio_interrupt_cb g_gpio_int_cb[GPIO_NUM_MAX] = {NULL};
static GPIO_MODE g_gpio_mode[GPIO_NUM_MAX] = {GPIO_MODE_MAX};
static const uint8_t g_gpio_mode_map[GPIO_NUM_MAX][GPIO_MODE_MAX] = {
/* IN    OUT   ADC   PWM   UART  I2C   SPI */
  {0x00, 0x00,   12, 0xff, 0xff, 0xff,    1}, //GPIO_NUM_A25
  {0x00, 0x00,   12, 0xff, 0xff, 0xff,    8}, //GPIO_NUM_A26
  {0x00, 0x00,   12,    9, 0xff, 0xff,    5}, //GPIO_NUM_A27
  {0x00, 0x00,   12,    8, 0xff, 0xff, 0xff}, //GPIO_NUM_A28
  {0x00, 0x00, 0xff,    5,    4,    1, 0xff}, //GPIO_NUM_B0
  {0x00, 0x00, 0xff,    5,    1,    2, 0xff}, //GPIO_NUM_B1
  {0x00, 0x00, 0xff,    4,    3,    1, 0xff}, //GPIO_NUM_B2
  {0x00, 0x00, 0xff,    4,    1,    2, 0xff}, //GPIO_NUM_B3
  {0x00, 0x00, 0xff, 0xff,    1,    2, 0xff}, //GPIO_NUM_B6
  {0x00, 0x00, 0xff, 0xff,    3,    1, 0xff}, //GPIO_NUM_B7
  {0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff}  //GPIO_NUM_B8
};
static uni_bool g_is_init = false;

int user_gpio_init(void) {
  if (!g_is_init) {
    SarADC_Init();
    g_gpio_pulse = (gpio_pulse_t *)uni_malloc(sizeof(gpio_pulse_t) * MAX_PULSE_SUPPORT);
    if (NULL == g_gpio_pulse) {
      LOGE(TAG, "malloc failed.");
      return -1;
    }
    uni_memset(g_gpio_pulse, 0, sizeof(gpio_pulse_t) * MAX_PULSE_SUPPORT);
    /* TODO: reserve for pinmux tool */
    g_is_init = true;
  }
  return 0;
}

void user_gpio_final(void) {
  if (g_is_init) {
    if (g_gpio_pulse) {
      uni_free(g_gpio_pulse);
      g_gpio_pulse = NULL;
    }
    /* TODO: reserve for pinmux tool */
  }
}

static int _get_port_group_num(GPIO_NUMBER num,
                                     GPIO_PortA *portA, GPIO_PortB *portB) {
  switch (num) {
    case GPIO_NUM_A25:
      *portA = GPIOA25;
      return 0;
    case GPIO_NUM_A26:
      *portA = GPIOA26;
      return 0;
    case GPIO_NUM_A27:
      *portA = GPIOA27;
      return 0;
    case GPIO_NUM_A28:
      *portA = GPIOA28;
      return 0;
    case GPIO_NUM_B0:
      *portB = GPIOB0;
      return 1;
    case GPIO_NUM_B1:
      *portB = GPIOB1;
      return 1;
    case GPIO_NUM_B2:
      *portB = GPIOB2;
      return 1;
    case GPIO_NUM_B3:
      *portB = GPIOB3;
      return 1;
    case GPIO_NUM_B6:
      LOGW(TAG, "control B6 is not allowed! it used by aec input");
      return -1;
    case GPIO_NUM_B7:
      *portB = GPIOB7;
      return 1;
    case GPIO_NUM_B8:
      *portB = GPIOB8;
      return 1;
    default:
      return -1;
  }
}

int user_gpio_set_mode(GPIO_NUMBER num, GPIO_MODE mode) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t dir_out_reg = 0x00;
  uint32_t dir_in_reg = 0x00;
  uint32_t data_reg = 0x00;
  uint32_t gpio_idx = 0x00;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (mode < GPIO_MODE_IN || mode >= GPIO_MODE_MAX) {
    LOGE(TAG, "error GPIO_MODE: %d", mode);
    return -1;
  }
  if (0xff == g_gpio_mode_map[num][mode]) {
    LOGE(TAG, "unsupport mode %d on GPIO %d", mode, num);
    return -1;
  }
  g_gpio_mode[num] = mode;
  if (group) {
    /* port B */
    GPIO_PortBModeSet(portB, g_gpio_mode_map[num][mode]);
    dir_out_reg = GPIO_B_OE;
    dir_in_reg = GPIO_B_IE;
    data_reg = GPIO_B_OUT;
    gpio_idx = portB;
  } else {
    /* port A */
    GPIO_PortAModeSet(portA, g_gpio_mode_map[num][mode]);
    dir_out_reg = GPIO_A_OE;
    dir_in_reg = GPIO_A_IE;
    data_reg = GPIO_A_OUT;
    gpio_idx = portA;
  }
  if (GPIO_MODE_IN == mode) {
    GPIO_RegOneBitClear(dir_out_reg, gpio_idx);
    GPIO_RegOneBitSet(dir_in_reg, gpio_idx);
  } else if (GPIO_MODE_OUT == mode) {
#if GPIO_OUT_DEF_VAL
    GPIO_RegOneBitSet(data_reg, gpio_idx);    //default is high
#else
    GPIO_RegOneBitClear(data_reg, gpio_idx);  //default is low
#endif
    GPIO_RegOneBitClear(dir_in_reg, gpio_idx);
    GPIO_RegOneBitSet(dir_out_reg, gpio_idx);
  }
  return 0;
}

int user_gpio_set_pull_mode(GPIO_NUMBER num, GPIO_PULL_MODE mode) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t pull_up_reg = 0x00;
  uint32_t pull_down_reg = 0x00;
  uint32_t gpio_idx = 0x00;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (group) {
    pull_up_reg = GPIO_B_PU;
    pull_down_reg = GPIO_B_PD;
    gpio_idx = portB;
  } else {
    pull_up_reg = GPIO_A_PU;
    pull_down_reg = GPIO_A_PD;
    gpio_idx = portA;
  }
  switch (mode) {
    case GPIO_PULL_UP:
      GPIO_RegOneBitClear(pull_down_reg, gpio_idx);
      GPIO_RegOneBitSet(pull_up_reg, gpio_idx);
      break;
    case GPIO_PULL_DOWN:
      GPIO_RegOneBitClear(pull_up_reg, gpio_idx);
      GPIO_RegOneBitSet(pull_down_reg, gpio_idx);
      break;
    case GPIO_PULL_UP_DOWN:
      GPIO_RegOneBitSet(pull_up_reg, gpio_idx);
      GPIO_RegOneBitSet(pull_down_reg, gpio_idx);
      break;
    default:
      LOGE(TAG, "error GPIO_PULL_MODE: %d", mode);
      return -1;
  }
  return 0;
}

int user_gpio_set_value(GPIO_NUMBER num, int val) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t data_reg = 0x00;
  uint32_t gpio_idx = 0x00;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (GPIO_MODE_OUT == g_gpio_mode[num]) {
    if (group) {
      gpio_idx = portB;
      data_reg = GPIO_B_OUT;
    } else {
      gpio_idx = portA;
      data_reg = GPIO_A_OUT;
    }
    if (val) {
      GPIO_RegOneBitSet(data_reg, gpio_idx);
    } else {
      GPIO_RegOneBitClear(data_reg, gpio_idx);
    }
  }
  return 0;
}

static uint32_t _get_adc_channel_num(GPIO_NUMBER num) {
  switch (num) {
    case GPIO_NUM_A25:
      return ADC_CHANNEL_GPIOA25;
    case GPIO_NUM_A26:
      return ADC_CHANNEL_GPIOA26;
    case GPIO_NUM_A27:
      return ADC_CHANNEL_GPIOA27;
    case GPIO_NUM_B6:
      return ADC_CHANNEL_GPIOB6;
    case GPIO_NUM_B7:
      return ADC_CHANNEL_GPIOB7;
    default:
      break;
  }
  return ADC_CHANNEL_INVALID;
}

int user_gpio_get_value(GPIO_NUMBER num) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t data_reg = 0x00;
  uint32_t gpio_idx = 0x00;
  uint32_t adc_ch = ADC_CHANNEL_INVALID;
  int val = 0;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (GPIO_MODE_IN == g_gpio_mode[num]) {
    if (group) {
      gpio_idx = portB;
      data_reg = GPIO_B_IN;
    } else {
      gpio_idx = portA;
      data_reg = GPIO_A_IN;
    }
    val = GPIO_RegOneBitGet(data_reg, gpio_idx);
  } else if (GPIO_MODE_ADC == g_gpio_mode[num]) {
    adc_ch = _get_adc_channel_num(num);
    if (ADC_CHANNEL_INVALID != adc_ch) {
      val = ADC_SingleModeDataGet(adc_ch);
    }
  } else {
    LOGE(TAG, "GPIO %d is in %d mode, cannot read value.", g_gpio_mode[num]);
    return -1;
  }
  return val;
}

int user_gpio_interrupt_enable(void) {
  NVIC_EnableIRQ(Gpio_IRQn);
  return 0;
}

int user_gpio_interrupt_disable(void) {
  NVIC_DisableIRQ(Gpio_IRQn);
  return 0;
}

static GpioTriggerType _get_gpio_int_type(GPIO_INT_TYPE type) {
  GpioTriggerType trig_type = GPIO_NEG_EDGE_TRIGGER;
  switch (type) {
    case GPIO_INT_NEG_EDGE:
      trig_type = GPIO_NEG_EDGE_TRIGGER;
      break;
    case GPIO_INT_POS_EDGE:
      trig_type = GPIO_POS_EDGE_TRIGGER;
      break;
    case GPIO_INT_BOTH_EDGE:
      trig_type = GPIO_EDGE_TRIGGER;
      break;
    default:
      break;
  }
  return trig_type;
}

int user_gpio_set_interrupt(GPIO_NUMBER num, GPIO_INT_TYPE type,
                                    _gpio_interrupt_cb cb) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t gpio_idx = 0x00;
  uint32_t reg = 0x00;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (type < GPIO_INT_NEG_EDGE || type > GPIO_INT_BOTH_EDGE) {
    LOGE(TAG, "unknown interrupt type %d", type);
    return -1;
  }
  if (NULL == cb) {
    LOGE(TAG, "caalback function for GPIO interrupt cannot be NULL");
    return -1;
  }
  if (GPIO_MODE_IN == g_gpio_mode[num]) {
    if (group) {
      gpio_idx = portB;
      reg = GPIO_B_INT;
    } else {
      gpio_idx = portA;
      reg = GPIO_A_INT;
    }
    GPIO_INTEnable(reg, gpio_idx, _get_gpio_int_type(type));
    g_gpio_int_cb[num] = cb;
  } else {
    LOGE(TAG, "GPIO %d is not GPIO_MODE_IN mode.", num);
    return -1;
  }
  return 0;
}

int user_gpio_clear_interrupt(GPIO_NUMBER num) {
  GPIO_PortA portA;
  GPIO_PortB portB;
  uint32_t gpio_idx = 0x00;
  uint32_t reg = 0x00;
  int group = _get_port_group_num(num, &portA, &portB);
  if (group < 0) {
    LOGE(TAG, "error GPIO_NUMBER: %d", num);
    return -1;
  }
  if (GPIO_MODE_IN == g_gpio_mode[num]) {
    if (group) {
      gpio_idx = portB;
      reg = GPIO_B_INT;
    } else {
      gpio_idx = portA;
      reg = GPIO_A_INT;
    }
    GPIO_INTDisable(reg, gpio_idx);
    g_gpio_int_cb[num] = NULL;
  } else {
    LOGE(TAG, "GPIO %d is not GPIO_MODE_IN mode.", num);
    return -1;
  }
  return 0;
}

static GPIO_NUMBER _get_interrupt_gpio_num(bool is_portA, int index) {
  GPIO_NUMBER num = GPIO_NUM_MAX;
  if (is_portA) {
    switch (index) {
      case 25:
        num = GPIO_NUM_A25;
        break;
      case 26:
        num = GPIO_NUM_A26;
        break;
      case 27:
        num = GPIO_NUM_A27;
        break;
      case 28:
        num = GPIO_NUM_A28;
        break;
      default:
        break;
    }
  } else {
    switch (index) {
      case 0:
        num = GPIO_NUM_B0;
        break;
      case 1:
        num = GPIO_NUM_B1;
        break;
      case 2:
        num = GPIO_NUM_B2;
        break;
      case 3:
        num = GPIO_NUM_B3;
        break;
      case 6:
        num = GPIO_NUM_B6;
        break;
      case 7:
        num = GPIO_NUM_B7;
        break;
      case 8:
        num = GPIO_NUM_B8;
        break;
      default:
        break;
    }
  }
  return num;
}

__attribute__((section(".driver.isr"))) void GpioInterrupt(void)
{
  uint32_t val;
  GPIO_NUMBER num;
  int gpio_val = 0;
  int i = 0;
  val = GPIO_INTFlagGet(GPIO_A_INT);
  if(val) {
    GPIO_INTFlagClear(GPIO_A_INT,val);
    for (i = 25; i < 29; i++) {         //there is 25/26/27/28 only on HB-M
      if (val>>i & 0x01) {
        num = _get_interrupt_gpio_num(TRUE, i);
        if (num >= GPIO_NUM_A25 && num < GPIO_NUM_MAX) {
          if (g_gpio_int_cb[num]) {
            gpio_val = GPIO_RegOneBitGet(GPIO_A_IN, val);
            g_gpio_int_cb[num](num, !!(gpio_val));
          }
        }
      }
    }
  }
  val = GPIO_INTFlagGet(GPIO_B_INT);
  if(val) {
    GPIO_INTFlagClear(GPIO_B_INT,val);
    for (i = 0; i < 9; i++) {         //there is 0/1/2/3/6/7/8 only on HB-M
      if (val>>i & 0x01) {
        num = _get_interrupt_gpio_num(FALSE, i);
        if (num >= GPIO_NUM_A25 && num < GPIO_NUM_MAX) {
          if (g_gpio_int_cb[num]) {
            gpio_val = GPIO_RegOneBitGet(GPIO_B_IN, val);
            g_gpio_int_cb[num](num, !!(gpio_val));
          }
        }
      }
    }
  }
}

int _usr_gpio_pulse_callback(void *arg) {
  gpio_pulse_t *pulse = (gpio_pulse_t *)arg;
  if (pulse->is_top) {
    if (pulse->def_val) {
      user_gpio_set_value(pulse->num, 0);
    } else {
      user_gpio_set_value(pulse->num, 1);
    }
    pulse->is_top = false;
  } else {
    if (pulse->def_val) {
      user_gpio_set_value(pulse->num, 1);
    } else {
      user_gpio_set_value(pulse->num, 0);
    }
    pulse->is_top = true;
    pulse->times--;
    if (pulse->times <= 0) {
      pulse->timer = 0;
      return TIMER_ERRNO_STOP_PERIODICAL;
    }
  }
  return 0;
}

int user_sw_timer_gpio_pulse(GPIO_NUMBER num, int cycle, int times,
                                     int def_val) {
  uint32_t msec = 0;
  TimerHandle timer = 0;
  gpio_pulse_t *p = NULL;
  int i = 0;
  if (NULL == g_gpio_pulse) {
    LOGE(TAG, "must to be inited at first.");
    return -1;
  }
  if (cycle <= 0) {
    cycle = 10;
  }
  if (times <= 0) {
    times = 1;
  }
  msec = (cycle/10 + ((cycle%10)?1:0)) * 10 / 2;
  for (i = 0; i < MAX_PULSE_SUPPORT; i++) {
    p = g_gpio_pulse + i;
    if (p->timer <= 0) {
      p->num = num;
      p->cycle = cycle;
      p->times = times;
      p->def_val = def_val;
      timer = TimerStart(msec, TIMER_TYPE_PERIODICAL, _usr_gpio_pulse_callback,
                        p);
      p->timer = timer;
      p->is_top = true;
      user_gpio_set_value(num, def_val);
      break;
    }
  }
  if (MAX_PULSE_SUPPORT == i) {
    LOGE(TAG, "No more timer use for gpio pulse.");
    return -1;
  }
  return 0;
}

