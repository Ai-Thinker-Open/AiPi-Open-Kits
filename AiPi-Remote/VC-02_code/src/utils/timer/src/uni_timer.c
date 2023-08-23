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
 * Description : uni_timer.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_timer.h"
#include "uni_interruptable.h"
#include "list_head.h"
#include "uni_iot.h"
#include "uni_log.h"

#define TAG     "timer"

static struct {
  uni_list_head      timer_list;
  int                is_running;
  uni_pthread_t      pid;
  uni_mutex_t        mutex;
  uni_sem_t          sem_thread_exit_sync;
  unsigned long long id;
  InterruptHandle    interrupt_handle;
} g_timer;

typedef struct {
  uni_list_head       link;
  long long           period;
  long long           expire;
  unsigned long long  id;
  TimerExpireCallback fct;
  void                *arg;
} Timer;

static long long _relative_now(void) {
  return uni_get_clock_time_ms();
}

static inline void _orderized_insert(Timer *rt) {
  uni_list_head *p = NULL, *t = NULL;
  uni_list_for_each_safe(p, t, &g_timer.timer_list) {
    Timer *c = uni_list_entry(p, Timer, link);
    if (rt->expire < c->expire) {
      uni_list_add_before(&rt->link, p);
      return;
    }
  }
  uni_list_add_tail(&rt->link, &g_timer.timer_list);
}

static unsigned long long _get_id(void) {
  return ++g_timer.id;
}

static inline int _add_timer(Timer *rt, long long interval,
                             TimerType type, TimerExpireCallback fct,
                             unsigned long long id, void *arg) {
  rt->fct = fct;
  rt->arg = arg;
  rt->id = id;
  rt->expire = _relative_now() + interval;
  rt->period = (type == TIMER_TYPE_PERIODICAL ? interval : 0);
  _orderized_insert(rt);
  return 0;
}

static Timer* _is_timer_valid(unsigned long long id) {
  Timer *p = NULL;
  uni_list_for_each_entry(p, &g_timer.timer_list, Timer, link) {
    if (p->id == id) {
      return p;
    }
  }
  return NULL;
}

static inline void _del_timer(Timer *rt) {
  uni_list_del(&rt->link);
}

TimerHandle TimerStart(int interval_msec, TimerType type,
                       TimerExpireCallback fct, void *arg) {
  Timer *rt = (Timer *)uni_malloc(sizeof(Timer));
  unsigned long long id = 0;
  if (NULL == rt) {
    LOGE(TAG, "malloc failed");
    return 0;
  }
  if (interval_msec == 0) {
    interval_msec = 1;
  }
  LOGD(TAG, "TimerStart malloc rt = %p", rt);
  uni_memset(rt, 0, sizeof(Timer));
  uni_list_init(&rt->link);
  uni_pthread_mutex_lock(g_timer.mutex);
  _add_timer(rt, interval_msec, type, fct, id = _get_id(), arg);
  uni_pthread_mutex_unlock(g_timer.mutex);
  InterruptableBreak(g_timer.interrupt_handle);
  return (TimerHandle)id;
}

void TimerStop(TimerHandle handle) {
  unsigned long long id = (unsigned long long)handle;
  Timer *rt = NULL;
  uni_pthread_mutex_lock(g_timer.mutex);
  if (NULL != (rt = _is_timer_valid(id))) {
    _del_timer(rt);
    uni_free(rt);
  }
  uni_pthread_mutex_unlock(g_timer.mutex);
}

static long long _timer_manage(void) {
  uni_list_head *node;
  Timer *timer;
  int ret;
  long long now = _relative_now();
  do {
    uni_pthread_mutex_lock(g_timer.mutex);
    if (NULL == (node = uni_list_get_head(&g_timer.timer_list))) {
      uni_pthread_mutex_unlock(g_timer.mutex);
      break;
    }
    timer = uni_list_entry(node, Timer, link);
    if (now < timer->expire) {
      uni_pthread_mutex_unlock(g_timer.mutex);
      return timer->expire - now;
    }
    _del_timer(timer);
    ret = timer->fct(timer->arg);
    if (0 != timer->period && ret != TIMER_ERRNO_STOP_PERIODICAL) {
      _add_timer(timer, timer->period, TIMER_TYPE_PERIODICAL, timer->fct,
                 timer->id, timer->arg);
      uni_pthread_mutex_unlock(g_timer.mutex);
      continue;
    }
    uni_pthread_mutex_unlock(g_timer.mutex);
    uni_free(timer);
  } while (node != NULL);
  return 1000;
}

static void _free_timer_nodes(void) {
  Timer *p, *t;
  uni_list_for_each_entry_safe(p, t, &g_timer.timer_list, Timer, link) {
    uni_list_del(&p->link);
    uni_free(p);
  }
}

static void _destroy_all(void){
  _free_timer_nodes();
  uni_pthread_mutex_destroy(g_timer.mutex);
  uni_sem_destroy(g_timer.sem_thread_exit_sync);
  InterruptDestroy(g_timer.interrupt_handle);
}

static void _timer_main(void *arg) {
  while (g_timer.is_running) {
    InterruptableSleep(g_timer.interrupt_handle, _timer_manage());
  }
  uni_sem_post(g_timer.sem_thread_exit_sync);
}

static void _woker_thread_create(void) {
  thread_param param;
  uni_memset(&param, 0, sizeof(param));
  param.stack_size = STACK_SMALL_SIZE;
  param.priority = OS_PRIORITY_NORMAL;
  uni_strncpy(param.task_name, "timer", sizeof(param.task_name) - 1);
  g_timer.is_running = 1;
  uni_pthread_create(&g_timer.pid, &param, _timer_main, NULL);
  uni_pthread_detach(g_timer.pid);
}

int TimerInitialize(void) {
  uni_memset(&g_timer, 0, sizeof(g_timer));
  if (NULL == (g_timer.interrupt_handle = InterruptCreate())) {
    return -1;
  }
  uni_list_init(&g_timer.timer_list);
  uni_pthread_mutex_init(&g_timer.mutex);
  uni_sem_init(&g_timer.sem_thread_exit_sync, 0);
  _woker_thread_create();
  return 0;
}

void TimerFinalize(void) {
  g_timer.is_running = 0;
  InterruptableBreak(g_timer.interrupt_handle);
  uni_sem_wait(g_timer.sem_thread_exit_sync);
  _destroy_all();
}
