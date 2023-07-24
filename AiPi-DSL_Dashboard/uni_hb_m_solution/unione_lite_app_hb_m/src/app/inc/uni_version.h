#ifndef LIB_VERSION_H
#define LIB_VERSION_H

#include "uni_iot.h"

static inline void uni_print_version(void)
{
    printf("\r\n");
    printf("*|**||*|\\*||**||**/'\\   >>Unione Version : %s \r\n", UNI_CROW_VERSION);
    printf("*\\_//**|*\\||**||**\\_/   >>Build TIME     : %s \r\n", BUILD_TIME);
    printf("\r\n");
}

#endif
