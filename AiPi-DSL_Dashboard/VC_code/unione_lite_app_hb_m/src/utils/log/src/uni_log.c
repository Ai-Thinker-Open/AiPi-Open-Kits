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
 * Description : uni_log.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.09.10
 *
 **************************************************************************/

#include "osal-log.h"
#include "uni_log.h"
#include "uni_iot.h"
#include <stdarg.h>

#define BUF_DEFAULT_LEN         (255)

typedef enum {
  LOG_WORK_INIT = 0,
  LOG_WORK_IDLE,
  LOG_WORK_RUNNING
}log_work_state;

typedef struct {
  log_work_state  state;
  LogLevel        set_level;
  char            buffer[BUF_DEFAULT_LEN];
  uni_mutex_t     mutex;
} log_context_t;

static log_context_t *g_log_context = NULL;

static const char* _level_tostring(LogLevel level) {
  switch (level) {
    case N_LOG_ERROR: return "[E]";
    case N_LOG_DEBUG: return "[D]";
    case N_LOG_TRACK: return "[T]";
    case N_LOG_WARN:  return "[W]";
    default:          return "[N/A]";
  }
}

static void _get_thread_id_str(char *buf, int len) {
  uni_snprintf(buf, len, "%llu", uni_get_clock_time_ms());
}

static int _fill_log_level(LogLevel level, char *buf, int len) {
  int write_len = 0;
  switch (level) {
    case N_LOG_DEBUG:
      write_len = uni_snprintf(buf, len, "\033[0m\033[47;33m%s\033[0m ",
                               _level_tostring(N_LOG_DEBUG));
      break;
    case N_LOG_TRACK:
      write_len = uni_snprintf(buf, len, "\033[0m\033[42;33m%s\033[0m ",
                               _level_tostring(N_LOG_TRACK));
      break;
    case N_LOG_WARN:
      write_len = uni_snprintf(buf, len, "\033[0m\033[41;33m%s\033[0m ",
                               _level_tostring(N_LOG_WARN));
      break;
    case N_LOG_ERROR:
      write_len = uni_snprintf(buf, len, "\033[0m\033[41;33m%s\033[0m ",
                               _level_tostring(N_LOG_ERROR));
      break;
    default:
      break;
  }
  return uni_max(0, write_len);
}

static int _fill_tag(char *buf, int len, const char *tag) {
  return uni_max(0, uni_snprintf(buf, len, "<%s>", tag));
}

static int _fill_function_line(char *buf, int len,
                                     const char *function, int line) {
  return uni_max(0, uni_snprintf(buf, len, "%s:%d->", function, line));
}

static int _fill_thread_id(char *buf, int len) {
  char thread_id[32] = {0};
  _get_thread_id_str(thread_id, sizeof(thread_id));
  return uni_max(0, uni_snprintf(buf, len, "%s", thread_id));
}

static void _fill_customer_info(char *buf, int len, const char *fmt, va_list args,
                                int append_feed_line) {
  int length, remain_len;
  length = vsnprintf(buf, len, fmt, args);
  length = uni_max(length, 0);
  length = uni_min(length, len);
  remain_len = len - length;
  if (0 == remain_len) {
    if (append_feed_line) {
      buf[len - 2] = '\n';
    }
    buf[len - 1] = '\0';
    return;
  }
  if (1 == remain_len) {
    if (append_feed_line) {
      buf[len - 2] = '\n';
    }
    return;
  }
  if (append_feed_line) {
    uni_strncat(buf, "\n", remain_len);
  }
  return;
}

uni_bool LogLevelValid(LogLevel level) {
  if (!g_log_context) {
    return false;
  }
  return level <= g_log_context->set_level ? true : false;
}

static void _sync_write_process(LogLevel level, const char *tags,
                                const char *function, int line,
                                const char *fmt, va_list args) {
  int len = 0;
  if (!g_log_context) {
    return ;
  }
  if (level != N_LOG_RAW) {
    len += _fill_log_level(level, g_log_context->buffer + len,
                          BUF_DEFAULT_LEN - len);
    len += _fill_thread_id(g_log_context->buffer + len, BUF_DEFAULT_LEN - len);
    len += _fill_tag(g_log_context->buffer + len, BUF_DEFAULT_LEN - len, tags);
    len += _fill_function_line(g_log_context->buffer + len,
                              BUF_DEFAULT_LEN - len, function, line);
  }
  _fill_customer_info(g_log_context->buffer + len, BUF_DEFAULT_LEN - len,
                     fmt, args, level != N_LOG_RAW);
  uni_printf("%s", g_log_context->buffer);
}

//implement aik Log func
void OsalLogDump(OsalLogLevel level, const char* tags, const char* function,
                 uint32_t line, const char* fmt, ...){
  int uni_level;
  if (!g_log_context) {
    return ;
  }
  if (LOG_WORK_RUNNING != g_log_context->state) {
    return ;
  }
  uni_level = 5 - level;
  if (LogLevelValid(uni_level)) {
    uni_pthread_mutex_lock(g_log_context->mutex);
    va_list args;
    va_start(args, fmt);
    _sync_write_process(uni_level, tags, function, line, fmt, args);
    va_end(args);
    uni_pthread_mutex_unlock(g_log_context->mutex);
  }
}

int LogWrite(LogLevel level, const char *tags,
                     const char *function, int line, char *fmt, ...) {
  if (!g_log_context) {
    return -1;
  }
  if (LOG_WORK_RUNNING != g_log_context->state) {
    return -1;
  }
  uni_pthread_mutex_lock(g_log_context->mutex);
  va_list args;
  va_start(args, fmt);
  _sync_write_process(level, tags, function, line, fmt, args);
  va_end(args);
  uni_pthread_mutex_unlock(g_log_context->mutex);
  return 0;
}

int LogLevelSet(LogLevel level) {
  if (!g_log_context) {
    return -1;
  }
  g_log_context->set_level = level;
  return 0;
}

static int _log_mutex_init(void) {
  uni_pthread_mutex_init(&g_log_context->mutex);
  return 0;
}

static void _log_mutex_final(void) {
  uni_pthread_mutex_destroy(g_log_context->mutex);
}

int LogInitialize(void) {
  if (g_log_context) {
    return -1;
  }
  g_log_context = (log_context_t *)uni_malloc(sizeof(log_context_t));
  if (!g_log_context) {
    return -1;
  }
  _log_mutex_init();
  g_log_context->state = LOG_WORK_RUNNING;
  return 0;
}

void LogFinalize(void) {
  if (LOG_WORK_INIT != g_log_context->state) {
    _log_mutex_final();
    g_log_context->state = LOG_WORK_INIT;
  }
  if (g_log_context) {
    uni_free(g_log_context);
    g_log_context = NULL;
  }
}

