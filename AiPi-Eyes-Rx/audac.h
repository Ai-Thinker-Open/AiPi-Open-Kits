#ifndef _AUDAC_H
#define _AUDAC_H


#define AUDAC_DMA_NUME           "dma0_ch0"
#define AUDAC_DMA_AUTO_DELE_EN   (1)

#define AUDAC_TASK_PRIORITY_MAIN (25)

#if CONFIG_SOLUTION_FUNC_AUDIO_AUDAC

int audio_play_task_init(void);
#endif

#endif
