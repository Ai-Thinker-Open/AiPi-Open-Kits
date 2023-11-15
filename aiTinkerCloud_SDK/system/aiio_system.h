/** @brief      System-on-Chip Interface.
 *
 *  @file       aiio_system.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       System-on-Chip Interface
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/02/27      <td>1.0.0       <td>yanch       <td>Define the first version of the system interface
 *  <tr><td>2023/04/13      <td>1.0.1       <td>wusen       <td>add aiio_random
 *  </table>
 *
 */
#ifndef __AIIO_SYSTEM_H__
#define __AIIO_SYSTEM_H__
#include "aiio_log.h"
#include "aiio_type.h"
#include "aiio_error.h"

/**
 * @brief Software reset CPU
 *
 */
CHIP_API void aiio_restart(void);

/**
 * @brief random
 *
 */
CHIP_API int aiio_random(void);

#endif
