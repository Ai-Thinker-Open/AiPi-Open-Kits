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
 * Description : uni_uart.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_uart.h"
#include "uni_iot.h"
#include "uni_log.h"
#include "uni_hal_uart.h"
#include "uni_databuf.h"

#define UART_TAG           "uart"
#define DATA_BUF_LEN       (256)

typedef struct {
  RecvUartDataHandler on_recv_uart_data;
  UART_PORT_T         device;
  DataBufHandle       databuf;
  int                 running;
  uni_sem_t           sem_exit;
}uart_t;

static uart_t* g_uarts[UNI_UART_CNT] = {NULL};
static int     g_current_cnt         = 0;

static void _register_uart_recv_hook(uart_t *uart,
                                     RecvUartDataHandler handler) {
  uart->on_recv_uart_data = handler;
}

static DataBufHandle _get_databuf_by_device(UART_PORT_T port) {
  int i;
  for (i = 0; i < g_current_cnt; i++) {
    if (g_uarts[i]->device == port) {
      return g_uarts[i]->databuf;
    }
  }
  return NULL;
}

static void _recv_uart_data_handler(UART_PORT_T port) {
  char c = 0;
  DataBufHandle databuf;
  if (TRUE == uni_hal_uart_recvbyte(port, (uint8_t *)&c)) {
    databuf = _get_databuf_by_device(port);
    if (databuf && (DataBufferGetFreeSize(databuf) > 0)) {
      DataBufferWrite(databuf, &c, sizeof(c));
    }
  }
}

static uint32_t _get_baudrate(UartSpeed speed) {
  if (UNI_B_1200   == speed) return 1200;
  if (UNI_B_2400   == speed) return 2400;
  if (UNI_B_4800   == speed) return 4800;
  if (UNI_B_9600   == speed) return 9600;
  if (UNI_B_14400  == speed) return 14400;
  if (UNI_B_19200  == speed) return 19200;
  if (UNI_B_38400  == speed) return 38400;
  if (UNI_B_57600  == speed) return 57600;
  if (UNI_B_115200 == speed) return 115200;
  if (UNI_B_921600 == speed) return 921600;
  LOGE(UART_TAG, "speed[%d] invalid", speed);
  return -1;
}

static UART_PORT_T _get_uart_device(UartDeviceName device) {
  if (UNI_UART1 == device) return UART_PORT1;
  LOGE(UART_TAG, "device[%d] invalid", device);
  return -1;
}

static uint32_t _get_parity(UartParity parity) {
  if (UNI_PARITY_NONE == parity) return 0;
  if (UNI_PARITY_ODD  == parity) return 1;
  if (UNI_PARITY_EVEN == parity) return 2;
  LOGE(UART_TAG, "parity[%d] invalid", parity);
  return -1;
}

static uint32_t _get_stop_bit(UartStop stop) {
  if (UNI_ONE_STOP_BIT == stop) return 1;
  if (UNI_TWO_STOP_BIT == stop) return 2;
  LOGE(UART_TAG, "stop bit[%d] invalid", stop);
  return -1;
}

static uint32_t _get_data_bit(int len) {
  if (len < 5 || len > 8) {
    LOGE(UART_TAG, "data bit[%d] invalid", len);
    return -1;
  }
  return len;
}

static uint32_t _get_pin_mode(UartPinMode mode) {
  if (UNI_MODE0 == mode) return 0;
  if (UNI_MODE1 == mode) return 1;
  if (UNI_MODE2 == mode) return 2;
  LOGE(UART_TAG, "pin mode[%d] invalid", mode);
  return -1;
}

static int _uart_device_init(uart_t *uart, UartConfig *config) {
  UART_PORT_T device = _get_uart_device(config->device);
  if (-1 == device) {
    return -1;
  }

  uart->device = device;

  uint32_t mode = _get_pin_mode(config->mode);
  if (-1 == mode) {
    return -1;
  }

  uint32_t baud_rate = _get_baudrate(config->speed);
  if (-1 == baud_rate) {
    return -1;
  }

  uint32_t parity = _get_parity(config->parity);
  if (-1 == parity) {
    return -1;
  }

  uint32_t stop = _get_stop_bit(config->stop);
  if (-1 == stop) {
    return -1;
  }

  uint32_t data_bit = _get_data_bit(config->data_bit);
  if (-1 == data_bit) {
    return -1;
  }
  LOGT(UART_TAG, "device=%d, baud_rate=%d, parity=%d, stop=%d, data_bit=%d",
                  device, baud_rate, parity, stop, data_bit);
  uni_hal_uart_init(device, baud_rate, data_bit, parity, stop,
                    mode, _recv_uart_data_handler);

  LOGT(UART_TAG, "init uart success");
  return 0;
}

int UartWrite(char *buf, int len, UartHandle handle) {
  /* TODO: need to set TimeOut */
  uart_t *uart = (uart_t *)handle;
  int ret = uni_hal_uart_send(uart->device, (uint8_t *)buf, len, 1000);
  return ret;
}

static void _uart_recv_process(void *args) {
  int data_len = 0;
  static char tmp_buf[32] = {0};
  uart_t *uart = (uart_t *)args;
  while (uart->running) {
    data_len = DataBufferGetDataSize(uart->databuf);
    if (data_len <= 0) {
      uni_msleep(10);
    }

    if (data_len > sizeof(tmp_buf)) {
      data_len = sizeof(tmp_buf);
    }

    data_len = DataBufferRead(tmp_buf, data_len, uart->databuf);
    if (uart->on_recv_uart_data && data_len > 0) {
      uart->on_recv_uart_data((unsigned char *)tmp_buf, data_len);
    }
  }

  uni_sem_post(uart->sem_exit);
}

static int _create_uart_recv_thread(uart_t *uart) {
  thread_param param;
  uni_pthread_t pid = NULL;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_HIGH;
  uni_strncpy(param.task_name, "uart", sizeof(param.task_name) - 1);
  uart->running = true;
  if (0 != uni_pthread_create(&pid, &param, _uart_recv_process, uart)) {
    LOGE(UART_TAG, "create thread failed");
    return -1;
  }

  uni_pthread_detach(pid);
  return 0;
}

UartHandle UartInitialize(UartConfig *config, RecvUartDataHandler handler) {
  int ret;

  if (UNI_UART_CNT == g_current_cnt) {
    LOGE(UART_TAG, "cannot register %d uart at the same time");
    return NULL;
  }

  uart_t *uart = uni_malloc(sizeof(uart_t));
  if (NULL == uart) {
    LOGE(UART_TAG, "alloc memory failed");
    return NULL;
  }

  uni_memset(uart, 0, sizeof(uart_t));

  uni_sem_init(&uart->sem_exit, 0);

  uart->databuf = DataBufferCreate(DATA_BUF_LEN);
  if (NULL == uart->databuf) {
    LOGE(UART_TAG, "alloc memory failed");
    goto L_ERROR;
  }

  ret = _uart_device_init(uart, config);
  if (0 != ret) {
    goto L_ERROR;
  }

  ret = _create_uart_recv_thread(uart);
  if (0 != ret) {
    goto L_ERROR;
  }

  _register_uart_recv_hook(uart, handler);
  g_uarts[g_current_cnt++] = uart;

  return uart;

L_ERROR:
  if (uart->sem_exit) {
    uni_sem_destroy(uart->sem_exit);
  }

  if (uart->databuf) {
    uni_free(uart->databuf);
  }

  uni_free(uart);
  return NULL;
}

void UartFinalize(UartHandle handle) {
  uart_t *uart = (uart_t *)handle;
  if (NULL == uart) {
    return;
  }

  uart->running = false;
  uni_sem_wait(uart->sem_exit);

  if (uart->databuf) {
    uni_free(uart->databuf);
  }

  if (uart->sem_exit) {
    uni_sem_destroy(uart->sem_exit);
  }

  uni_free(uart);

  //donnot handler g_uarts now, not neccessary
  return;
}

