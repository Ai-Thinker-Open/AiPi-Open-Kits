/**********************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************
 *
 * Description : uni_float2string.c
 * Author      : shangjinlong@unisound.com
 * Date        : 2019.12.16
 *
 **********************************************************************/
#include <uni_float2string.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** Number on countu **/
static int __n_tu(int number, int count) {
  int result = 1;
  while (count-- > 0) result *= number;
  return result;
}

/*** Convert float to string ***/
static void __float_to_string(float f, char r[], int len) {
  long long int length, length2, i, number, position, sign;
  float number2;

  sign = -1;   // -1 == positive number
  if (f < 0) {
    sign = '-';
    f *= -1;
  }

  number2 = f;
  number = f;
  length = 0;  // Size of decimal part
  length2 = 0; // Size of tenth

  /* Calculate length2 tenth part */
  while((number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0)) {
    number2 = f * (__n_tu(10.0, length2 + 1));
    number = number2;
    length2++;
  }

  /* Calculate length decimal part */
  for (length = (f > 1) ? 0 : 1; f > 1; length++) f /= 10;
  position = length;
  length = length + 1 + length2;
  number = number2;
  if (sign == '-') {
    length++;
    position++;
  }

  for (i = length; i >= 0 ; i--) {
    if (i == (length) && i < len) r[i] = '\0';
    else if(i == (position)) r[i] = '.';
    else if(sign == '-' && i == 0) r[i] = '-';
    else {
      if (i < len) {
        r[i] = (number % 10) + '0';
        number /=10;
      }
    }
  }

  r[len - 1] = '\0';
}

/* Thread unsafe 线程不安全函数，有效精度8位（包含整数和小数部分），超过不能保证准确性 */
static char floatBuf[32] = {0};

const char* Float2String(float f) {
  __float_to_string(f + 0.00001, floatBuf, sizeof(floatBuf));
  return floatBuf;
}

const char* PFloat2String(float *pf) {
  float f = *pf;
  __float_to_string(f + 0.00001, floatBuf, sizeof(floatBuf));
  return floatBuf;
}

