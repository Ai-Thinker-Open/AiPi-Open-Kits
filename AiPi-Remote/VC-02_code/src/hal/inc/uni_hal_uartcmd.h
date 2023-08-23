#ifndef _UNI_HAL_UARTCMD_H_
#define _UNI_HAL_UARTCMD_H_  /* 需要有 define 保护 */

#define UNISOUND_UART_CMD_DEBUG 1

#ifdef UNISOUND_UART_CMD_DEBUG


#ifndef NULL
#define NULL (void *)0
#endif
#define TOTAL_CMD_BUF_LENGTH   	256
#define MAX_EACH_CMD_LENGTH		32	
#define MAX_SPACE_NUMBER		(TOTAL_CMD_BUF_LENGTH / MAX_EACH_CMD_LENGTH)

typedef int (*cli_cmd_handler_t)(int len, char (*param)[MAX_EACH_CMD_LENGTH]);

typedef struct cmd_s cmd_t;

struct cmd_s {
    char                *cmd;       /**< Command string. */
    char                *help;      /**< Type '?' to retrieve help message. */
    cli_cmd_handler_t   fptr;       /**< The function pointer to call when an input string matches with the command in MiniCLI engine. */
    cmd_t               *sub;       /**< Sub-commands. */
};



extern int uni_hal_uartcmdinit(void *pvParameters);
extern int  _is_factory_mode(void);

#endif
#endif  /*_UNI_HAL_UARTCMD_H_*/

