#include "stdio.h"
#include "aiio_log.h"
#include "aiio_type.h"
#include "aiio_error.h"
#include "aiio_uart.h"
// #include "aiio_autoconf.h"

#define CONFIG_LOG_DEFAULT_LEVEL  4

int32_t aiio_log_init(void)
{
    /* close printf buffer */
    setbuf(stdout, NULL);
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);
    /* start EasyLogger */
    elog_start();
    elog_set_filter_lvl(CONFIG_LOG_DEFAULT_LEVEL);
    return AIIO_OK;
}

int32_t aiio_manufacturer_info(void)
{
    aiio_uart_log_send("\r\n\r\n");
    aiio_uart_log_send(" .----------------.  .----------------.  .----------------.  .----------------.   .----------------.  .----------------.  .----------------. \r\n");
    aiio_uart_log_send("| .--------------. || .--------------. || .--------------. || .--------------. | | .--------------. || .--------------. || .--------------. |\r\n");
    aiio_uart_log_send("| |      __      | || |     _____    | || |     _____    | || |     ____     | | | |    _______   | || |  ________    | || |  ___  ____   | |\r\n");
    aiio_uart_log_send("| |     /  \\     | || |    |_   _|   | || |    |_   _|   | || |   .'    `.   | | | |   /  ___  |  | || | |_   ___ `.  | || | |_  ||_  _|  | |\r\n");
    aiio_uart_log_send("| |    / /\\ \\    | || |      | |     | || |      | |     | || |  /  .--.  \\  | | | |  |  (__ \\_|  | || |   | |   `. \\ | || |   | |_/ /    | |\r\n");
    aiio_uart_log_send("| |   / ____ \\   | || |      | |     | || |      | |     | || |  | |    | |  | | | |   '.___`-.   | || |   | |    | | | || |   |  __'.    | |\r\n");
    aiio_uart_log_send("| | _/ /    \\ \\_ | || |     _| |_    | || |     _| |_    | || |  \\  `--'  /  | | | |  |`\\____) |  | || |  _| |___.' / | || |  _| |  \\ \\_  | |\r\n");
    aiio_uart_log_send("| ||____|  |____|| || |    |_____|   | || |    |_____|   | || |   `.____.'   | | | |  |_______.'  | || | |________.'  | || | |____||____| | |\r\n");
    aiio_uart_log_send("| |              | || |              | || |              | || |              | | | |              | || |              | || |              | |\r\n");
    aiio_uart_log_send("| '--------------' || '--------------' || '--------------' || '--------------' | | '--------------' || '--------------' || '--------------' |\r\n");
    aiio_uart_log_send(" '----------------'  '----------------'  '----------------'  '----------------'   '----------------'  '----------------'  '----------------' \r\n");
    aiio_uart_log_send("\r\n");
    return AIIO_OK;
}
