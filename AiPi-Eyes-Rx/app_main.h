/**
 * @file        app_main.h
 * @brief
 * @author      Jimmy (ccolacat@163.com)
 * @version     v1.0.0
 * @date        2023-08-30
 * 
 * @copyright   Copyright (c) 2020-2023 Shenzhen Ai-Thinker Technology Co., Ltd. All Rights Reserved.
 * 
 * Disclaimer:  This program is for informational purposes only, please keep author information and source code license.
 * 
 */

#pragma once

#include "stdint.h"
#include "bflb_romfs.h"
extern romfs_file_t img_file;

#define DAC_ADC_BUF_MAX (32*1024)
#define DEF_AUDIO_ANXINKE_PCM   (0x400000)

#define DEF_NXP_EN         (1)
#define DEF_USER_ES8388_EN (1)
/**< 
 * If you use the ADC/DAC function, 
 * you need to turn the corresponding macro on and turn off ES8388, 
 * and vice versa 
 * CONFIG_SOLUTION_FUNC_AUDIO_AUADC CONFIG_SOLUTION_FUNC_AUDIO_AUDAC 
 * */

#define CONFIG_SOLUTION_QUEUE_YUYV  (0)
#define CONFIG_SOLUTION_QUEUE_MJPEG  (0)

#if DEF_USER_ES8388_EN
#define DEF_BOARD_INFO    (1)
#define DEF_BOARD_VERSION "Release-V1.1.1"
//#define DEF_BOARD_VERSION "Debug-V1.1.1"
void es8388_paly_en(void);


#else
#define DEF_BOARD_INFO                   (2)
#define DEF_BOARD_VERSION                "V1.0.0"

#define CONFIG_SOLUTION_QUEUE_AUDIO_IN   (1)
#define CONFIG_SOLUTION_QUEUE_AUDIO_OUT  (1)
#define CONFIG_SOLUTION_FUNC_AUDIO_AUADC (1)
#define CONFIG_SOLUTION_FUNC_AUDIO_AUDAC (1)

int audio_play_task_init(void);
void record_play_on(void);
void record_play_off(void);


#endif

/************************ Copyright (c) 2023 Shenzhen Ai-Thinker Technology Co., Ltd. ************************/
