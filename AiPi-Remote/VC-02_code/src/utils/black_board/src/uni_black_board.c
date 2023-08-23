/**********************************************************************
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
 **********************************************************************
 *
 * Description : black_board.c
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.12.28
 *
 **********************************************************************/

#include "uni_black_board.h"
#include "uni_log.h"

#define BB_TAG "black_board"

typedef struct {
  BLACK_BOARD_KEY key;
  int             value;
}BB_KEY_VALUE;

static struct {
  BB_KEY_VALUE bb[BB_KEY_MAX];
  uni_mutex_t mutex;
} g_bb;

int BbWrite(BLACK_BOARD_KEY key, int value) {
  int ret = -1;
  if ((key < BB_KEY_IS_WAKEUP) || (key >= BB_KEY_MAX)) {
    LOGE(BB_TAG, "cannot find key: %d", key);
    return -1;
  }
  uni_pthread_mutex_lock(g_bb.mutex);
  g_bb.bb[key].key = key;
  g_bb.bb[key].value = value;
  uni_pthread_mutex_unlock(g_bb.mutex);
  return ret;
}

int BbRead(BLACK_BOARD_KEY key, int *value) {
  int ret = -1;
  if ((key < BB_KEY_IS_WAKEUP) || (key >= BB_KEY_MAX)) {
    LOGE(BB_TAG, "cannot find key: %d", key);
    return -1;
  }
  uni_pthread_mutex_lock(g_bb.mutex);
  if (g_bb.bb[key].key == key) {
    *value = g_bb.bb[key].value;
    ret = 0;
  } else {
    ret = -1;
  }
  uni_pthread_mutex_unlock(g_bb.mutex);
  return ret;
}

int BbInit(void) {
  uni_memset(g_bb.bb, 0, sizeof(g_bb.bb));
  uni_pthread_mutex_init(&g_bb.mutex);
  return 0;
}

void BbFinal(void) {
  uni_pthread_mutex_destroy(g_bb.mutex);
}
