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
 * Description : user_uart_sucp.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.06.09
 *
 **************************************************************************/
#include "user_uni_sucp.h"
#include "user_config.h"
#include "uni_uart.h"
#include "uni_communication.h"

#define TAG "user_uart_sucp"

#define PACKAGE_START_CODE   0x5A

typedef enum {
  UART_RECV_START_CODE = 0,
  UART_RECV_CMD_CODE,
  UART_RECV_RESERVE1,
  UART_RECV_RESERVE2,
  UART_RECV_CHECKSUM
}UART_RECV_RUN_STATE;

#if USE_SAMPLE_PROTOCOL

static UartHandle g_uart_handle  = NULL;
static _user_uni_sucp_recv g_user_recv_cb = NULL;
static sucp_pack_t g_recv_package = {0};
static UART_RECV_RUN_STATE g_recv_state = UART_RECV_START_CODE;

static uni_u8 _get_checksum_code(sucp_pack_t *pack) {
  uni_u8 *p = (uni_u8 *)pack;
  uni_u8 code = 0x00;
  uni_u8 i = 0;
  /* not include checksum */
  for (i = 0; i < (sizeof(sucp_pack_t) - 1); i++) {
    code += *p++;
  }
  return code & 0xFF;
}

static uni_bool _check_checksum_code(sucp_pack_t *pack) {
  uni_u8 tmp_code = _get_checksum_code(pack);
  if (pack->checksum != tmp_code) {
    LOGE(TAG, "Error chacksum: 0x%02x -- 0x%02x", tmp_code, pack->checksum);
    return false;
  }
  return true;
}

static void _parser_package(sucp_pack_t *pack) {
  if (g_user_recv_cb) {
    g_user_recv_cb(pack);
  }
}

static void _uart_recv_cb(char *buf, int len) {
  int i = 0;
_GOTO_PARSE_RESET:
  switch (g_recv_state) {
    case UART_RECV_START_CODE:
      /* find start code */
      for (; i < len; i++) {
        if (PACKAGE_START_CODE == buf[i]) {
          break;
        }
      }
      if (i >= len) {
        break;
      }
      g_recv_package.start_code = buf[i];
      g_recv_state = UART_RECV_CMD_CODE;
      i++;
    case UART_RECV_CMD_CODE:
      if (i >= len) {
        break;
      }
      g_recv_package.cmd_code = buf[i];
      g_recv_state = UART_RECV_RESERVE1;
      i++;
    case UART_RECV_RESERVE1:
      if (i >= len) {
        break;
      }
      g_recv_package.reserve[0] = buf[i];
      g_recv_state = UART_RECV_RESERVE2;
      i++;
    case UART_RECV_RESERVE2:
      if (i >= len) {
        break;
      }
      g_recv_package.reserve[1] = buf[i];
      g_recv_state = UART_RECV_CHECKSUM;
      i++;
    case UART_RECV_CHECKSUM:
      if (i >= len) {
        break;
      }
      g_recv_package.checksum = buf[i];
      if (!_check_checksum_code(&g_recv_package)) {
        g_recv_state = UART_RECV_START_CODE;
        goto _GOTO_PARSE_RESET;
      }
      _parser_package(&g_recv_package);
      g_recv_state = UART_RECV_START_CODE;
      i++;
      goto _GOTO_PARSE_RESET;
    default:
      break;
  }
}

static int _uart_send_pack(sucp_pack_t *pack) {
  return UartWrite((char *)pack, sizeof(sucp_pack_t), g_uart_handle);
}

#endif

int user_uni_sucp_init(_user_uni_sucp_recv cb_recv) {
#if USE_SAMPLE_PROTOCOL
  UartConfig config;
#if USE_UNIONE_PROTOCOL
  LOGE(TAG, "HB-M chip have UART1 only, cannot use USE_SAMPLE_PROTOCOL if USE_UNIONE_PROTOCOL set to 1.");
  return -1;
#endif
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
#else
  #error "Support 4800/9600/19200/38400/57600/115200 only on HB-M board"
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
    g_uart_handle = UartInitialize(&config, (RecvUartDataHandler)_uart_recv_cb);
    if (NULL == g_uart_handle) {
      LOGE(TAG, "UartInitialize failed.");
      return -1;
    }
  }
  g_user_recv_cb = cb_recv;
#endif
  return 0;
}

void user_uni_sucp_final(void) {
#if USE_SAMPLE_PROTOCOL
  if (g_uart_handle) {
    UartFinalize(g_uart_handle);
    CommProtocolFinal();
    g_uart_handle = NULL;
  }
  g_user_recv_cb = NULL;
#endif
}

int user_uni_sucp_send(uni_u8 cmd_code, uni_u8 reserve[2]) {
#if USE_SAMPLE_PROTOCOL
  sucp_pack_t pack;
  uni_memset(&pack, 0, sizeof(sucp_pack_t));
  pack.start_code = PACKAGE_START_CODE;
  pack.cmd_code = cmd_code;
  if (reserve) {
    pack.reserve[0] = reserve[0];
    pack.reserve[1] = reserve[1];
  }
  pack.checksum = _get_checksum_code(&pack);
  return _uart_send_pack(&pack);
#endif
  return 0;
}

