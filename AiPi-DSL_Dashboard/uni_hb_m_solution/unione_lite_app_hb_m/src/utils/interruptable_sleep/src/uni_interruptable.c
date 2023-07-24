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
 * Description : uni_interruptable.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.12
 *
 **************************************************************************/
#include "uni_interruptable.h"
#include "uni_iot.h"
#include "uni_log.h"

#define INTERRUPTABLE_TAG "interruptable_tag"

typedef struct {
  uni_sem_t   sem;
} Interruptable;

InterruptHandle InterruptCreate(void) {
  Interruptable *interrupter = NULL;
  interrupter = (Interruptable *)uni_malloc(sizeof(Interruptable));
  if (NULL == interrupter) {
    return NULL;
  }
  if (0 != uni_sem_init(&interrupter->sem, 0)) {
    uni_free(interrupter);
    return NULL;
  }
  return (InterruptHandle)interrupter;
}

int InterruptDestroy(InterruptHandle handle) {
  Interruptable *interrupter = (Interruptable *)handle;
  if (NULL == interrupter) {
    return -1;
  }
  uni_sem_destroy(interrupter->sem);
  uni_free(interrupter);
  return 0;
}

int InterruptableSleep(InterruptHandle handle, uni_u32 sleep_msec) {
  Interruptable *interrupter = (Interruptable *)handle;
  if (NULL == interrupter) {
    return -1;
  }
  if (sleep_msec <= 0) {
    sleep_msec = 0;
  }
  if (sleep_msec > portMAX_DELAY) {
    sleep_msec = portMAX_DELAY;
  }
  uni_sem_wait_ms(interrupter->sem, sleep_msec);
  return 0;
}

int InterruptableBreak(InterruptHandle handle) {
  Interruptable *interrupter = (Interruptable *)handle;
  if (NULL == interrupter) {
    return -1;
  }
  uni_sem_post(interrupter->sem);
  return 0;
}

