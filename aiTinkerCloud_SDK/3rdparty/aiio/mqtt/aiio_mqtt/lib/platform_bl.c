#include "platform.h"

#include "aiio_log.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "aiio_wifi.h"
#include "aiio_system.h"
#include "platform_bl.h"

static const char *TAG = "PLATFORM";

#define MAX_ID_STRING (32)

char *platform_create_id_string(void)
{
    uint8_t mac[6];
    char *id_string = calloc(1, MAX_ID_STRING);
    AIIO_MEM_CHECK(id_string, return NULL);
    aiio_wifi_sta_mac_get(mac);
    sprintf(id_string, "aiio_%02x%02X%02X", mac[3], mac[4], mac[5]);
    return id_string;
}

int platform_random(int max)
{
    return aiio_random() % max;
}

long long platform_tick_get_ms(void)
{
    uint32_t count;
    aiio_os_tick_count(&count);
    return aiio_os_tick2ms(count);
}

void ms_to_timeval(int timeout_ms, struct timeval *tv)
{
    tv->tv_sec = timeout_ms / 1000;
    tv->tv_usec = (timeout_ms - (tv->tv_sec * 1000)) * 1000;
}
