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
 * Description : hb_uart.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_uart.h"

#define TAG "hb_uart"

/* 接收到串口数据后，该接口将被触发回调 */
static void _hb_uart_recv(char *buf, int len) {
  int i;
  for (i = 0; i < len; i++) {
    LOGR(TAG, "%02x ", (uint8_t)buf[i]);
  }
}

static void _hb_uart_send_task(void *args) {
  char buf[6] = {1, 2, 3, 4, 5, 6};
  int ret;

  /* 每隔2秒发送一次 */
  while (1) {
    LOGT(TAG, "hb uart send[1, 2, 3, 4, 5, 6]");
    ret = user_uart_send(buf, sizeof(buf));
    LOGT(TAG, "hb uart send done, ret=%d", ret);
    uni_msleep(2000);
  }
}

int hb_uart(void) {
  int ret = user_uart_init(_hb_uart_recv);
  if (ret != 0) {
    LOGE(TAG, "hb uart init failed");
    return -1;
  }

  //启动一个线程，模拟发送串口消息任务
  uni_pthread_t pid;
  thread_param param;
  param.stack_size = STACK_SMALL_SIZE;
  param.priority   = OS_PRIORITY_NORMAL;
  strncpy(param.task_name, "hb-uart", sizeof(param.task_name) - 1);
  uni_pthread_create(&pid, &param, _hb_uart_send_task, NULL);
  return 0;
}
