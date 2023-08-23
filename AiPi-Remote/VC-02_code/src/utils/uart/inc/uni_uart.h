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
 * Description : uni_uart.h
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#ifndef UTILS_UART_INC_UNI_UART_H_
#define UTILS_UART_INC_UNI_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * device type
 */
typedef enum {
  UNI_UART1,
  UNI_UART_CNT
} UartDeviceName;

/**
 * pin mode
 */
typedef enum {
  UNI_MODE0,      /* B7/B6 */
  UNI_MODE1,      /* B2/B3 */
  UNI_MODE2       /* B0/B1 */
} UartPinMode;

/**
 * baud rate
 */
typedef enum {
  UNI_B_1200,
  UNI_B_2400,
  UNI_B_4800,
  UNI_B_9600,
  UNI_B_14400,
  UNI_B_19200,
  UNI_B_38400,
  UNI_B_57600,
  UNI_B_115200,
  UNI_B_921600,
} UartSpeed;

/**
 *  parity check
 */
typedef enum {
  UNI_PARITY_NONE,
  UNI_PARITY_ODD,
  UNI_PARITY_EVEN
} UartParity;

/**
 * stop bit
 */
typedef enum {
  UNI_ONE_STOP_BIT,
  UNI_TWO_STOP_BIT,
} UartStop;

/**
 * uart init configure parameter
 */
typedef struct {
  UartDeviceName device;   /* device name */
  UartPinMode    mode;     /* pin select mode */
  UartSpeed      speed;    /* baudrate */
  UartParity     parity;   /* parity check */
  UartStop       stop;     /* stop bit */
  int            data_bit; /* data bit */
} UartConfig;

typedef void (*RecvUartDataHandler)(unsigned char *buf, int len);
typedef void* UartHandle;

/**
 * @brief uart init
 * @param config uart configure parameter
 * @param handler handle uart receive data hook
 * @return handle of uart, NULL failed
 */
UartHandle UartInitialize(UartConfig *config, RecvUartDataHandler handler);

/**
 * @brief uart finalize
 * @param handle uart handler
 * @return void
 */
void UartFinalize(UartHandle handle);

/**
 * @brief write data by UART, multi-thread unsafe, please write in sync mode
 * @param buf the data buffer to write
 * @param len the data length
 * @return the actual write length by UART
 */
int UartWrite(char *buf, int len, UartHandle handle);

#ifdef __cplusplus
}
#endif
#endif  // UTILS_UART_INC_UNI_UART_H_
