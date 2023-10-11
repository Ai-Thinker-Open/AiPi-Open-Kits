#ifndef _AUADC_H
#define _AUADC_H

#include <stdint.h>

#define AUADC_SAMPLING_NUM (48 * 1024)
#if CONFIG_SOLUTION_FUNC_AUDIO_AUADC

int audio_record_task_init(void);
#endif

#endif