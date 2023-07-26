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
 * Description : hb_uart_sucp.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.06.09
 *
 **************************************************************************/
#include "user_uni_sucp.h"

#define TAG "hb_sucp"

/* 接收到串口数据后，该接口将被触发回调 */
static void _hb_sucp_recv(sucp_pack_t *pack) {
  LOGT(TAG, "recv sucp packet, cmd_code=0x%02x, reserve[0] = 0x%02x, reserve[1] = 0x%02x",
            pack->cmd_code, pack->reserve[0], pack->reserve[1]);
}

static void _hb_sucp_send_task(void *args) {
  char buf[2] = {0x11, 0x22};
  int ret;

  while (1) {
    LOGT(TAG, "hb uart sucp send heartbeat: 0x11 - 0x22");
    /* 蜂鸟系列支持最多150条命令词，这里cmd_code建议从150开始 */
    ret = user_uni_sucp_send(150, (uint8_t *)buf);
    LOGT(TAG, "hb uart send done, ret=%d", ret);
    uni_msleep(2000);
  }
}

int hb_uart_sucp(void) {
  //将宏USE_SAMPLE_PROTOCOL打开，设置为1
  int ret = user_uni_sucp_init(_hb_sucp_recv);
  if (ret != 0) {
    LOGE(TAG, "hb uart sucp init failed");
    return -1;
  }

  //启动一个线程，模拟发送串口消息任务
  uni_pthread_t pid;
  thread_param param;
  param.stack_size = STACK_SMALL_SIZE;
  param.priority   = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "hb-sucp", sizeof(param.task_name) - 1);
  uni_pthread_create(&pid, &param, _hb_sucp_send_task, NULL);
  return 0;
}
