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
 * Description : user_uart.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.14
 *
 **************************************************************************/
#include "user_uart.h"
#include "user_config.h"
#include "uni_uart.h"

#define TAG "user_uart"

static UartHandle g_uart_handle = NULL;

int user_uart_init(_user_uart_recv cb_recv) {
  UartConfig config;
#if USE_UNIONE_PROTOCOL || USE_SAMPLE_PROTOCOL
#if (USER_UART_DEVICE_NUM == UNI_UART_DEVICE_NUM)
  LOGE(TAG, "HB chip have UART1, USER_UART_DEVICE_NUM and UNI_UART_DEVICE_NUM cannot be same device.");
  return -1;
#endif
#endif
#if (USER_UART_DEVICE_NUM == 1)
  config.device = UNI_UART1;
#else
  #error "Support 1 only on HB-M board"
#endif
#if (USER_UART_PIN_SELECT == 0)
  config.mode = UNI_MODE0;
#elif (USER_UART_PIN_SELECT == 1)
  config.mode = UNI_MODE1;
#elif (USER_UART_PIN_SELECT == 2)
  config.mode = UNI_MODE2;
#else
  #error "Support 0/1/2 only on HB-M board"
#endif
#if (USER_UART_PARITY == 0)
  config.parity = UNI_PARITY_NONE;
#elif (USER_UART_PARITY == 1)
  config.parity = UNI_PARITY_ODD;
#elif (USER_UART_PARITY == 2)
  config.parity = UNI_PARITY_EVEN;
#else
  #error "Support 0/1/2 only on HB-M board"
#endif
#if (USER_UART_BAUD_RATE == 4800)
  config.speed = UNI_B_4800;
#elif (USER_UART_BAUD_RATE == 9600)
  config.speed = UNI_B_9600;
#elif (USER_UART_BAUD_RATE == 19200)
  config.speed = UNI_B_19200;
#elif (USER_UART_BAUD_RATE == 38400)
  config.speed = UNI_B_38400;
#elif (USER_UART_BAUD_RATE == 57600)
  config.speed = UNI_B_57600;
#elif (USER_UART_BAUD_RATE == 115200)
  config.speed = UNI_B_115200;
#else
  #error "Support 4800/9600/19200/38400/57600/115200 only on HB-M board"
#endif
#if (USER_UART_STOP_BIT == 1)
  config.stop = UNI_ONE_STOP_BIT;
#elif (USER_UART_STOP_BIT == 2)
  config.stop = UNI_TWO_STOP_BIT;
#else
  #error "Support 1/2 only on HB-M board"
#endif
  config.data_bit = USER_UART_DATA_BIT;
  if (NULL == (g_uart_handle = UartInitialize(&config, (RecvUartDataHandler)cb_recv))) {
    LOGE(TAG, "UartInitialize failed.");
    return -1;
  }
  return 0;
}

void user_uart_final(void) {
  UartFinalize(g_uart_handle);
}

int user_uart_send(const char *buf, int len) {
  if (NULL == g_uart_handle) {
    LOGE(TAG, "g_uart_handle is NULL.");
    return -1;
  }
  return UartWrite((char *)buf, len, g_uart_handle);
}

