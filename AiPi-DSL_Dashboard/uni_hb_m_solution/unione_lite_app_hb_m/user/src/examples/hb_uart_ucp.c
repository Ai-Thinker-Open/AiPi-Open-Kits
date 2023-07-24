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
 * Description : hb_uart_ucp.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_uni_ucp.h"

#define TAG "hb_ucp"

/* 接收到串口数据后，该接口将被触发回调 */
static void _hb_ucp_recv(uni_u16 cmd, char *payload, int len) {
  LOGT(TAG, "recv ucp packet, cmd=%d, len=%d", cmd, len);
  int i;
  LOGR(TAG, "payload: ");
  for (i = 0; i < len; i++) {
    LOGR(TAG, "02x ", payload[i]);
  }
  LOGR(TAG, "\n");
}

//消息唯一索引码，不可重叠
//规定[1, 10000]消息号预留给客户扩展；[10001, 65535]云知声内部使用
enum {
  e_MSG_HEARTBEAT = 10001,
  e_MSG_DEMO,
};

static void _hb_ucp_send_task(void *args) {
  char buf[4] = {'p', 'i', 'n', 'g'};
  int ret;

  while (1) {
    LOGT(TAG, "hb uart ucp send heartbeat: ping");
    //is_block = 1指可靠传输，必须接收方ack确认（TCP模式），0 UDP模式
    ret = user_uni_ucp_send(e_MSG_HEARTBEAT, buf, sizeof(buf), 1);
    LOGT(TAG, "hb uart send done, ret=%d", ret);
    uni_msleep(2000);
  }
}

int hb_uart_ucp(void) {
  //将宏USE_UNIONE_PROTOCOL打开，设置为1
  int ret = user_uni_ucp_init(_hb_ucp_recv);
  if (ret != 0) {
    LOGE(TAG, "hb uart ucp init failed");
    return -1;
  }

  //启动一个线程，模拟发送串口消息任务
  uni_pthread_t pid;
  thread_param param;
  param.stack_size = STACK_SMALL_SIZE;
  param.priority   = OS_PRIORITY_NORMAL;
  strncpy(param.task_name, "hb-ucp", sizeof(param.task_name) - 1);
  uni_pthread_create(&pid, &param, _hb_ucp_send_task, NULL);
  return 0;
}
