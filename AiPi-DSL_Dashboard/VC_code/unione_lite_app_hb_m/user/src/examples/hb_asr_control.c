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
 * Description : user_flash_example.c
 * Author      : yuanshifeng@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_asr.h"

#define TAG "asr_control"

static void _hb_asr_control_task(void *args) {
  while (1) {
    //进入睡眠模式，此时将只能通过唤醒词唤醒，说命令词无效
    user_asr_goto_sleep();
    LOGT(TAG, "Now is into sleep mode, you can awake it");
    uni_sleep(10);//延迟10秒
    //进入唤醒模式，此时可以识别命令词
    user_asr_goto_awakend();
    LOGT(TAG, "Now is into awake mode, you can say cmd word ");
    uni_sleep(10);
    //仍然在唤醒模式，执行关闭语音控制
    user_asr_recognize_disable();
    LOGT(TAG, "asr recognize is disabled, nothing will be recongnized");
    uni_sleep(10);
    //重新打开语音控制
    user_asr_recognize_enable();
    LOGT(TAG, "asr recognize is enabled");
    //延迟10秒
    uni_sleep(10);
  }
}

int hb_asr_control(void) {
  //启动一个线程，执行语音控制切换
  uni_pthread_t pid;
  thread_param param;
  param.stack_size = STACK_SMALL_SIZE;
  param.priority   = OS_PRIORITY_NORMAL;
  strncpy(param.task_name, "hb-asr", sizeof(param.task_name) - 1);
  uni_pthread_create(&pid, &param, _hb_asr_control_task, NULL);
  //禁用一个识别/唤醒词
  user_asr_word_disable("退下");
  user_asr_word_disable("再见");
  return 0;
}
