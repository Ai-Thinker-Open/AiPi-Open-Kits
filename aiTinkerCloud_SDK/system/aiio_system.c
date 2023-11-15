/** @brief      System-on-Chip Interface.
 *
 *  @file       aiio_system.c
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       System-on-Chip Interface
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/02/27      <td>1.0.0       <td>yanch       <td>Define the first version of the system interface
 *  </table>
 *
 */
#include "aiio_system.h"
#include "bl616_glb.h"


void aiio_restart(void)
{
    GLB_SW_POR_Reset();
}

int aiio_random(void)
{
   return rand();
}
