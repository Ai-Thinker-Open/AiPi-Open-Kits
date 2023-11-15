#ifndef __AIIO_CHIP_SPEC_H__
#define __AIIO_CHIP_SPEC_H__


/* INCN SPEC */

/* TYPE SPEC */
#ifdef      __AIIO_DEFAULT_DATE_TYPE

#endif

#ifdef      __AIIO_DATA_TYPE

#endif

/* RTOS SPEC */
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "task.h"
#include <lwip/netdb.h>
#include <sys/socket.h>
#ifdef configMAX_PRIORITIES
/**< The current OS maximum priority is configMAX_PRIORITIES, and the user-available range is (configMAX_PRIORITIES -1)*/
#define     __AIIO_OS_PRIORITY_DEFAULT  (configMAX_PRIORITIES/2-1)
#define     __AIIO_OS_PRIORITY_FREE  (__AIIO_OS_PRIORITY_DEFAULT-1)
#else
#define     __AIIO_OS_PRIORITY_DEFAULT  (15)
#define     __AIIO_OS_PRIORITY_FREE  (__AIIO_OS_PRIORITY_DEFAULT-1)
#endif

/* GPIO SPEC */
#define     __AIIO_GPIO_P_ENABLE        (0)

#if(__AIIO_GPIO_P_ENABLE == 1)
    #define     __AIIO_GPIO_A
    #define     __AIIO_GPIO_B
#else
    #define     __AIIO_GPIO_A               (0)
    #define     __AIIO_GPIO_B               (0)
#endif

#define     __AIIO_GPIO_NUM_0           (0)     /*!< GPIO0, input and output */
#define     __AIIO_GPIO_NUM_1           (1)     /*!< GPIO1, input and output */
#define     __AIIO_GPIO_NUM_2           (2)     /*!< GPIO2, input and output */
#define     __AIIO_GPIO_NUM_3           (3)     /*!< GPIO3, input and output */
#define     __AIIO_GPIO_NUM_4           (4)     /*!< GPIO4, input and output */
#define     __AIIO_GPIO_NUM_5           (5)     /*!< GPIO5, input and output */
#define     __AIIO_GPIO_NUM_6           (6)     /*!< GPIO6, input and output */

#define     __AIIO_GPIO_NUM_7           (7)     /*!< GPIO7, input and output */
#define     __AIIO_GPIO_NUM_8           (8)     /*!< GPIO8, input and output */
#define     __AIIO_GPIO_NUM_9           (9)     /*!< GPIO9, input and output */


#define     __AIIO_GPIO_NUM_10          (10)   /*!< GPIO10, input and output */
#define     __AIIO_GPIO_NUM_11          (11)   /*!< GPIO11, input and output */
#define     __AIIO_GPIO_NUM_12          (12)   /*!< GPIO12, input and output */
#define     __AIIO_GPIO_NUM_13          (13)   /*!< GPIO13, input and output */

#define     __AIIO_GPIO_NUM_14          (14)   /*!< GPIO14, input and output */
#define     __AIIO_GPIO_NUM_15          (15)   /*!< GPIO15, input and output */

#define     __AIIO_GPIO_NUM_16          (16)   /*!< GPIO16, input and output */
#define     __AIIO_GPIO_NUM_17          (17)   /*!< GPIO17, input and output */
#define     __AIIO_GPIO_NUM_18          (18)   /*!< GPIO18, input and output */
#define     __AIIO_GPIO_NUM_19          (19)   /*!< GPIO19, input and output */

#define     __AIIO_GPIO_NUM_20          (20)   /*!< GPIO20, input and output */
#define     __AIIO_GPIO_NUM_21          (21)   /*!< GPIO21, input and output */
#define     __AIIO_GPIO_NUM_22          (22)   /*!< GPIO22, input and output */

/* UART SPEC */
#define __AIIO_UART0    0
#define __AIIO_UART1    1

#define     __AIIO_UART0_TX_P
#define     __AIIO_UART0_TX             (21)
#define     __AIIO_UART0_RX_P
#define     __AIIO_UART0_RX             (22)

#define     __AIIO_UART1_TX_P
#define     __AIIO_UART1_TX             (16)
#define     __AIIO_UART1_RX_P
#define     __AIIO_UART1_RX             (17)


/* RTOS SPEC */

#define     __MAIN_ENTRY()          void main(void)

#endif
