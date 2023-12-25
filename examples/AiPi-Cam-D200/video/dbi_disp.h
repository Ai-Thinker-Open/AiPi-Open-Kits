#ifndef __DBI_DISP_H__
#define __DBI_DISP_H__

#define DISP_EVENT_FREE_QUEUE  (0x01 << 0)
#define DISP_EVENT_ENABLE_DISP (0x01 << 1)

volatile uint32_t g_dbi_disp_fps;

void dbi_disp_free_init(void);
void dbi_disp_init(void);

#endif /* __DVP_MJPEG_H__ */