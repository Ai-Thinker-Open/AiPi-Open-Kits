#include "stdio.h"
#include "stdarg.h"
#include "aiio_stdio.h"

#define AIIO_VSPRINTF_MAX_LEN 512

/**
 * @brief API to fomat str
 * @param  buffer recive data buffer
 * @param  n buffer size
 * @param  format format string
 * @param  ap format param list
 * @return Upon successful return, these functions return the number of characters printed (excluding the null byte used to end output to strings).
 */
int aiio_vsnprintf(char *buffer, size_t n, const char *format, va_list ap)
{
    return vsnprintf(buffer, n, format, ap);
}

/**
 * @brief API to fomat str
 * @param  buffer recive data buffer
 * @param  format format string
 * @param  ap format param list
 * @return Upon successful return, these functions return the number of characters printed (excluding the null byte used to end output to strings).
 */
int aiio_vsprintf(char *buffer, const char *format, va_list ap)
{
    return aiio_vsnprintf(buffer, AIIO_VSPRINTF_MAX_LEN, format, ap);
}

/**
 * @brief API to fomat str
 * @param  buffer recive data buffer
 * @param  n buffer size
 * @param  format format string
 * @param  ap format param list
 * @return Upon successful return, these functions return the number of characters printed (excluding the null byte used to end output to strings).
 */
int aiio_snprintf(char *buffer, size_t n, const char *format, ...)
{
    va_list ap;
    int rv;
    va_start(ap, format);
    rv = aiio_vsnprintf(buffer, n, format, ap);
    va_end(ap);
    return rv;
}

/**
 * @brief API to fomat str
 * @param  buffer recive data buffer
 * @param  format format string
 * @param  ap format param list
 * @return Upon successful return, these functions return the number of characters printed (excluding the null byte used to end output to strings).
 */
int aiio_sprintf(char *buffer, const char *format, ...)
{
    va_list ap;
    int rv;
    va_start(ap, format);
    rv = aiio_vsprintf(buffer, format, ap);
    va_end(ap);
    return rv;
}

/**
 * @brief string copy
 * @param  strDest Destination string address
 * @param  strSrc source string address
 * @return the point of strDest
 */
char *aiio_strcpy(char *strDest, const char *strSrc)
{
    if (strDest == NULL || strSrc == NULL)
        return NULL;
    if (strDest == strSrc)
        return strDest;
    char *tempDest = strDest;
    while ((*strDest++ = *strSrc++) != '\0')
        ;
    return tempDest;
}

/**
 * @brief string copy
 * @param  dest Destination string address
 * @param  src source string address
 * @param count copy length
 * @return the point of strDest
 */
char *aiio_strncpy(char *dest, const char *src, size_t count)
{
    char *tmp = dest;
    if (dest == NULL || src == NULL)
        return NULL;

    while (count--)
    {
        *tmp = *src;
        tmp++;
        src++;
    }
    return dest;
}


/**
 * @brief   Setting value to destination by this function
 * 
 * @param[in]   dest            destination address
 * @param[in]   set_value       The value to be setting
 * @param[in]   count           The count number to be setting
 * 
 * @return  void*               Reture destination address
 */
void* aiio_memset(void* dest, int set_value, size_t count) 
{
    char* tmp = (char*)dest;

    if(tmp == NULL)      return NULL;
    for (size_t i = 0; i < count; ++i) 
    {
        tmp[i] = set_value;
    }

    return dest;
}

/**
 * @brief   Count  the length of the string data
 * 
 * @param[in]   str    The value in string format
 * 
 * @return  size_t   return the length of the string data
 */
size_t aiio_strlen(const char* str)
{
    size_t count = 0;

    if(str == NULL)      return NULL;
    while(*str!='\0')
    {
        count++;
        str++;
    }

    return count;
}



/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int aiio_strcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;

  do
  {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
      {
        return c1 - c2;
      }

    }
  while (c1 == c2);

  return c1 - c2;
}



// A simple atoi() function
int aiio_atoi(char* str)
{
    // Initialize result
    int res = 0;
 
    // Iterate through all characters
    // of input string and update result
    // take ASCII character of corresponding digit and
    // subtract the code from '0' to get numerical
    // value and multiply res by 10 to shuffle
    // digits left to update running total
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
 
    // return result.
    return res;
}

