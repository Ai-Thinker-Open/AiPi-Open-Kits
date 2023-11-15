/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 * Tuan PM <tuanpm at live dot com>
 */
#pragma once

#include <FreeRTOS.h>
#include <event_groups.h>
#include "aiio_log.h"
#include <sys/time.h>
#include "aiio_err.h"
char *platform_create_id_string(void);
int platform_random(int max);
long long platform_tick_get_ms(void);
void ms_to_timeval(int timeout_ms, struct timeval *tv);

#define AIIO_MEM_CHECK(a, action) if (!(a)) {                                                      \
        aiio_log_e("%s(%d): %s",  __FUNCTION__, __LINE__, "Memory exhausted");                      \
        action;                                                                                         \
        }

#define AIIO_OK_CHECK(a, action) if ((a) != AIIO_OK) {                                                     \
        aiio_log_e("%s(%d): %s", __FUNCTION__, __LINE__, "Failed with non AIIO_OK err code");               \
        action;                                                                                               \
        }
