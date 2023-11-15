/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include <elog.h>

#include "aiio_uart.h"
#include "aiio_os_port.h"
// #include "aiio_autoconf.h"

aiio_os_mutex_handle_t aiio_os_mutex_handle = NULL;

#define CONFIG_LOG_BAUDRATE   2000000

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void)
{
    ElogErrCode result = ELOG_NO_ERR;
    /* add your code here */
    aiio_uart_config_t uart =
    {
#if CONFIG_AIIO_LOG_CUSTOM
        .uart_num = AIIO_UART0,
        .uart_tx_pin = CONFIG_AIIO_CUSTOM_LOG_TX,
        .uart_rx_pin = CONFIG_AIIO_CUSTOM_LOG_RX,
#else
#if (CONFIG_LOG_UART == __AIIO_UART0)
        .uart_num = AIIO_UART0,
        .uart_tx_pin = AIIO_UART0_TX,
        .uart_rx_pin = AIIO_UART0_RX,
#elif (CONFIG_LOG_UART == __AIIO_UART1)
        .uart_num = AIIO_UART1,
        .uart_tx_pin = AIIO_UART1_TX,
        .uart_rx_pin = AIIO_UART1_RX,
#else
#error "The log serial port does not exist"
#endif
#endif
        .baud_rate = CONFIG_LOG_BAUDRATE
    };
    aiio_uart_log_init(uart);
    aiio_os_mutex_create(&aiio_os_mutex_handle);
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void)
{
    /* add your code here */
    aiio_os_mutex_delete(aiio_os_mutex_handle);
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size)
{
    /* add your code here */
    aiio_uart_log_send("%.*s", size, log);
}

/**
 * output lock
 */
void elog_port_output_lock(void)
{
    /* add your code here */
    aiio_os_mutex_get(aiio_os_mutex_handle, portMAX_DELAY);
}

/**
 * output unlock
 */
void elog_port_output_unlock(void)
{
    /* add your code here */
    aiio_os_mutex_put(aiio_os_mutex_handle);
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void)
{
    /* add your code here */
    uint32_t tick;
    static char cur_system_time[16] = "";
    aiio_os_tick_count(&tick);
    snprintf(cur_system_time, 16, "%lu", tick);
    return cur_system_time;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void)
{
    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void)
{
    /* add your code here */
    static char cur_thread_name[32] = "";
    memset(cur_thread_name,0,sizeof(cur_thread_name));
    aiio_os_thread_self_name(cur_thread_name);
    return cur_thread_name;
}
