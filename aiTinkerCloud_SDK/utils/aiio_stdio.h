#ifndef __AIIO_STDIO_H__
#define __AIIO_STDIO_H__

#include "aiio_type.h"

CHIP_API int aiio_vsnprintf(char *buffer, size_t n, const char *format, va_list ap);

CHIP_API int aiio_vsprintf(char *buffer, const char *format, va_list ap);

CHIP_API int aiio_snprintf(char *buffer, size_t n, const char *format, ...);

CHIP_API int aiio_sprintf(char *buffer, const char *format, ...);

CHIP_API char* aiio_strcpy(char *strDest, const char *strSrc);

CHIP_API char *aiio_strncpy(char *dest, const char *src, size_t count);

CHIP_API void* aiio_memset(void* dest, int set_value, size_t count);

CHIP_API size_t aiio_strlen(const char* str);

CHIP_API int aiio_strcmp (const char *p1, const char *p2);

CHIP_API int aiio_atoi(char* str);

#endif
