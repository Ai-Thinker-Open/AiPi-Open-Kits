#ifndef __AIIO_LOG_H__
#define __AIIO_LOG_H__

#include "elog.h"
// #include "aiio_autoconf.h"


//#define aiio_log_init             elog_init
//#define aiio_log_set_fmt          elog_set_fmt
//#define aiio_log_start            elog_start

int32_t aiio_log_init(void);
int32_t aiio_manufacturer_info(void);

#define aiio_log_a                  log_a
#define aiio_log_e                  log_e
#define aiio_log_w                  log_w
#define aiio_log_i                  log_i
#define aiio_log_d                  log_d
#define aiio_log_v                  log_v

#if (CONFIG_LOG_DEFAULT_LEVEL == ELOG_LVL_ASSERT)
#define aiio_assert(EXPR)			ELOG_ASSERT(EXPR)
#else
#define aiio_assert(EXPR)			((void)0);
#endif
#endif
