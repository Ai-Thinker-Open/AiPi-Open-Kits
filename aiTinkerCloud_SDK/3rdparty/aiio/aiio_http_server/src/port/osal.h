/** @brief      http server interface.
 *
 *  @file       osal.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       http server interface.
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/06/19      <td>1.0.0       <td>hjz         <td>http server
 *  </table>
 *
 */

#ifndef _OSAL_H_
#define _OSAL_H_

#include <FreeRTOS.h>
#include <task.h>
#include <unistd.h>
#include <stdint.h> 
#include "aiio_os_port.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS_SUCCESS AIIO_OK
#define OS_FAIL    AIIO_FAIL

typedef aiio_os_thread_handle_t othread_t;

static inline int httpd_os_thread_create(othread_t *thread,
                                 const char *name, uint16_t stacksize, int prio,
                                 void (*thread_routine)(void *arg), void *arg,
                                 BaseType_t core_id)
{
    int ret = aiio_os_thread_create(thread, name, thread_routine, stacksize, arg, prio);
    if (ret == AIIO_OK)
        return OS_SUCCESS;
    
    return OS_FAIL;
}

/* Only self delete is supported */
static inline void httpd_os_thread_delete(void)
{
    aiio_os_thread_handle_t thread_handle;
    if (aiio_os_thread_self(&thread_handle) == AIIO_OK)
    {
        aiio_os_thread_delete(thread_handle);
    }
}

static inline void httpd_os_thread_sleep(int msecs)
{
    aiio_os_tick_dealy(aiio_os_ms2tick(msecs));
}

static inline othread_t httpd_os_thread_handle(void)
{
    aiio_os_thread_handle_t thread_handle = NULL;
    aiio_os_thread_self(&thread_handle);
    return thread_handle;
}

#ifdef __cplusplus
}
#endif

#endif /* ! _OSAL_H_ */
