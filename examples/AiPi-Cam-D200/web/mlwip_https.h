#ifndef __MLWIP_HTTPS_H_
#define __MLWIP_HTTPS_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <string.h>
#include "task.h"
#include "semphr.h"
#include <sys/socket.h>

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t
typedef void *at_os_mutex_t;

typedef struct myparm
{
    int sc;
    u8 *buf;
}MYPARM;

extern char *datajpeg_buf;
extern uint32_t datajpeg_len;

void mhttp_server_init();
void CamDataProcess(uint8_t *data, uint32_t len);

#endif
