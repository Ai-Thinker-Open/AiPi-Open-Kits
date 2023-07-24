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
 * Description : hb_player.c
 * Author      : liuwenzheng@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_player.h"

#define TAG "player_demo"

static void _player_test_process(void *args) {
  while (1) {
    uni_sleep(10);

    user_player_set_volume_min();
    LOGT(TAG, "set min vol");
    /*播放9.mp3文件*/
    user_player_play(AUDIO_PLAY_REPLY, "9"); //9.mp3
    uni_sleep(10);

    user_player_set_volume_mid();
    LOGT(TAG, "set mid vol");
    /*播放列表中的101.MP3文件*/
    user_player_reply_list_num("[9,101,102]", 1);
    uni_sleep(10);

    user_player_set_volume_max();
    LOGT(TAG, "set max vol");
    /*随机播放列表中的某个文件*/
    user_player_reply_list_random("[9,101,102]");
    uni_sleep(10);

    /*顺序播放列表中的所有文件*/
    user_player_reply_list_in_order("[9,101,102]");
    uni_sleep(10);
  }
}

static Result _create_player_test_thread(void) {
  thread_param param;
  uni_pthread_t pid;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "player_test", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&pid, &param,
                              _player_test_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(pid);
  return E_OK;
}

int hb_player(void) {
  _create_player_test_thread();
  return 0;
}
