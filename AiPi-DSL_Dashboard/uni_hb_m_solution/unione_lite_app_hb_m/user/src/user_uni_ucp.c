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
 * Description : user_uart_ucp.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2020.04.20
 *
 **************************************************************************/
#include "user_uni_ucp.h"
#include "user_config.h"
#include "uni_uart.h"
#include "uni_communication.h"

#define TAG "user_uart_ucp"

#if USE_UNIONE_PROTOCOL

static UartHandle      g_uart_handle  = NULL;
static _user_uni_recv g_user_recv_cb = NULL;

static void _comm_recv_packet_handler(CommPacket *packet) {
  if (g_user_recv_cb) {
    g_user_recv_cb(packet->cmd, packet->payload, packet->payload_len);
  }
}

static void _register_user_recv_hook(_user_uni_recv cb_recv) {
  g_user_recv_cb = cb_recv;
}

static void _unregister_user_recv_hook() {
  g_user_recv_cb = NULL;
}

static int _uart_send(char *buf, int len) {
  return UartWrite(buf, len, g_uart_handle);
}
#endif

int user_uni_ucp_init(_user_uni_recv cb_recv) {
#if USE_UNIONE_PROTOCOL
  UartConfig config;

#if (UNI_UART_DEVICE_NUM == 1)
  config.device = UNI_UART1;
#else
  #error "Support 1 only on HB-M board"
#endif
#if (UNI_UART_PARITY == 0)
  config.parity = UNI_PARITY_NONE;
#elif (UNI_UART_PARITY == 1)
  config.parity = UNI_PARITY_ODD;
#elif (UNI_UART_PARITY == 2)
  config.parity = UNI_PARITY_EVEN;
#else
  #error "Support 0/1/2 only on HB-M board"
#endif
#if (UNI_UART_BAUD_RATE == 4800)
  config.speed = UNI_B_4800;
#elif (UNI_UART_BAUD_RATE == 9600)
  config.speed = UNI_B_9600;
#elif (UNI_UART_BAUD_RATE == 19200)
  config.speed = UNI_B_19200;
#elif (UNI_UART_BAUD_RATE == 38400)
  config.speed = UNI_B_38400;
#elif (UNI_UART_BAUD_RATE == 57600)
  config.speed = UNI_B_57600;
#elif (UNI_UART_BAUD_RATE == 115200)
  config.speed = UNI_B_115200;
#elif (UNI_UART_BAUD_RATE == 921600)
  config.speed = UNI_B_921600;
#else
  #error "Support 4800/9600/19200/38400/57600/115200/921600 only on HB board"
#endif
#if (UNI_UART_PIN_SELECT == 0)
  config.mode = UNI_MODE0;
#elif (UNI_UART_PIN_SELECT == 1)
  config.mode = UNI_MODE1;
#elif (UNI_UART_PIN_SELECT == 2)
  config.mode = UNI_MODE2;
#else
  #error "Support 0/1/2 only on HB-M board"
#endif
#if (UNI_UART_STOP_BIT == 1)
  config.stop = UNI_ONE_STOP_BIT;
#elif (UNI_UART_STOP_BIT == 2)
  config.stop = UNI_TWO_STOP_BIT;
#else
  #error "Support 1/2 only on HB-M board"
#endif
  config.data_bit = UNI_UART_DATA_BIT;

  if (NULL == g_uart_handle) {
    g_uart_handle = UartInitialize(&config, CommProtocolReceiveUartData);
    if (NULL == g_uart_handle) {
      LOGE(TAG, "UartInitialize failed.");
      return -1;
    }
    CommProtocolInit(_uart_send, _comm_recv_packet_handler);
  }
  _register_user_recv_hook(cb_recv);
#endif
  return 0;
}

void user_uni_ucp_final(void) {
#if USE_UNIONE_PROTOCOL
  if (g_uart_handle) {
    UartFinalize(g_uart_handle);
    CommProtocolFinal();
    g_uart_handle = NULL;
  }
  _unregister_user_recv_hook();
#endif
}

int user_uni_ucp_send(uni_u16 cmd, char *payload, int len, uni_bool is_block) {
#if USE_UNIONE_PROTOCOL
  CommAttribute attr;
  if (is_block) {
    attr.reliable = true;
  } else {
    attr.reliable = false;
  }
  if (NULL == g_uart_handle) {
    LOGE(TAG, "Need to call user_uni_ucp_init() at first.");
    return -1;
  }
  int ret = CommProtocolPacketAssembleAndSend(cmd, payload, len, &attr);
  if (0 != ret) {
    return -1;
  }
#endif
  return 0;
}

