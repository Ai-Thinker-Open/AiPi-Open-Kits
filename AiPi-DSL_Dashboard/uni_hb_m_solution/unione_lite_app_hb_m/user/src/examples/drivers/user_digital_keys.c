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
 * Description : user_digital_keys.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.04.25
 *
 **************************************************************************/
#include "user_digital_keys.h"

#define TAG  "key"

typedef struct {
  uni_pthread_t     pid;
  uni_bool          running;
  uni_sem_t         sem_thread_exit_sync;
  uni_bool          trigger[GPIO_NUM_MAX];
  _keys_trigger_cb  keys_cb[GPIO_NUM_MAX];
  GPIO_INT_TYPE     trigger_type;
}keys_context_t;

static keys_context_t g_keys_context = {0};

static void _keys_trigger_process(void *args) {
  int i = 0;
  while (g_keys_context.running) {
    for (i = 0; i < GPIO_NUM_MAX; i++) {
      if (g_keys_context.trigger[i]) {
        g_keys_context.trigger[i] = false;
        if (g_keys_context.keys_cb[i]) {
          g_keys_context.keys_cb[i]((GPIO_NUMBER)i);
        }
      }
    }
    uni_msleep(10);
  }
  uni_sem_post(g_keys_context.sem_thread_exit_sync);
}

static Result _create_keys_trigger_thread(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_LOW;
  uni_strncpy(param.task_name, "gpio_keys", sizeof(param.task_name) - 1);
  if (0 != uni_pthread_create(&g_keys_context.pid, &param,
                              _keys_trigger_process, NULL)) {
    LOGE(TAG, "create thread failed");
    return E_FAILED;
  }
  uni_pthread_detach(g_keys_context.pid);
  return E_OK;
}

int user_digital_keys_init(GPIO_INT_TYPE type) {
  user_gpio_init();
  user_gpio_interrupt_enable();
  uni_sem_init(&g_keys_context.sem_thread_exit_sync, 0);
  uni_memset(g_keys_context.trigger, 0, sizeof(g_keys_context.trigger));
  uni_memset(g_keys_context.keys_cb, 0, sizeof(g_keys_context.keys_cb));
  g_keys_context.trigger_type = type;
  g_keys_context.running = true;
  if (E_OK != _create_keys_trigger_thread()) {
    LOGE(TAG, "_create_keys_trigger_thread failed.");
    return -1;
  }
  return 0;
}

void user_digital_keys_final(void) {
  g_keys_context.running = false;
  uni_sem_wait(g_keys_context.sem_thread_exit_sync);
  uni_sem_destroy(g_keys_context.sem_thread_exit_sync);
  user_gpio_interrupt_disable();
  uni_memset(g_keys_context.trigger, 0, sizeof(g_keys_context.trigger));
  uni_memset(g_keys_context.keys_cb, 0, sizeof(g_keys_context.keys_cb));
}

void _keys_interrupt_cb(GPIO_NUMBER num, uni_bool is_high) {
  g_keys_context.trigger[num] = true;
}

int user_digital_keys_register_key(GPIO_NUMBER num,
                                            _keys_trigger_cb cb) {
  if (num < GPIO_NUM_A25 || num >= GPIO_NUM_MAX) {
    LOGE(TAG, "GPIO num %d is failed.", num);
    return -1;
  }
  if (NULL == cb) {
    LOGE(TAG, "_gpio_interrupt_cb cannot be NULL.");
    return -1;
  }
  g_keys_context.keys_cb[num] = cb;
  user_gpio_set_mode(num, GPIO_MODE_IN);
  user_gpio_set_pull_mode(num, GPIO_PULL_UP_DOWN);
  user_gpio_set_interrupt(num, g_keys_context.trigger_type, _keys_interrupt_cb);
  return 0;
}

int user_digital_keys_cancel_key(GPIO_NUMBER num) {
  if (num < GPIO_NUM_A25 || num >= GPIO_NUM_MAX) {
    LOGE(TAG, "GPIO num %d is failed.", num);
    return -1;
  }
  user_gpio_clear_interrupt(num);
  g_keys_context.keys_cb[num] = NULL;
  return 0;
}

