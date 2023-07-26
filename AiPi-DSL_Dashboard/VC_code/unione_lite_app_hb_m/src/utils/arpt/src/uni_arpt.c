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
 * Description : Uniapp uni_arpt.c
 * Author      : wufangfang@unisound.com 
 * Date        : 2019.12.09
 *
 **********************************************************************/
#include "uni_arpt.h"
#include "uni_config.h"

static uni_bool g_is_enbale = false;

int ArptInit(void) {
  uni_s64 tmp = 0;
  if (0 == ConfigReadItemNumber("log.arpt_enable", &tmp)) {
    g_is_enbale = (tmp != 0);
  }
  return 0;
}

void ArptFinal(void) {
  g_is_enbale = false;
}

int ArptWrite(const char *content, uni_bool is_more) {
  if (g_is_enbale) {
    uni_printf("%s", content);
  }
  return 0;
}
