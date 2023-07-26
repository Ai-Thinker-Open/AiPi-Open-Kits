#ifndef __UART_HAL_H__
#define __UART_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"

typedef enum
{
	UART_PORT0 = 0,
	UART_PORT1,
}UART_PORT_T;

typedef void (*dma_rx_cb)(uint8_t* RecvBuf, uint16_t BufLen);
typedef void  (*uart_cb)(UART_PORT_T port);

/**********************************************************************************************************************************
//Uart RX复用:ModeSel(0-2)
//Uart0_0 RX-GPIOA14
//Uart1_0 RX-GPIOB6
//Uart1_1 RX-GPIOB1
//Uart1_2 RX-GPIOB3

//Uart TX复用:ModeSel(0-2)
//Uart0_0 TX-GPIOA13
//Uart1_0 TX-GPIOB7
//Uart1_1 TX-GPIOB0
//Uart1_2 TX-GPIOB2

***********************************************************************************************************************************/

extern int32_t uni_hal_uart_init(UART_PORT_T port,uint32_t BaudRate, uint8_t DataBits, uint8_t Parity, uint8_t StopBits,uint8_t ModeSel,void *cb);
extern uint32_t uni_hal_uart_send(UART_PORT_T port,uint8_t* SendBuf, uint32_t BufLen,uint32_t TimeOut);
extern uint32_t uni_hal_uart_recv(UART_PORT_T port,uint8_t* RecvBuf, uint32_t BufLen,uint32_t TimeOut);
extern bool uni_hal_uart_recvbyte(UART_PORT_T port,uint8_t* Val);
extern int32_t uni_hal_uart_dma_init(UART_PORT_T port,uint32_t BaudRate, uint8_t DataBits, uint8_t Parity, uint8_t StopBits,uint8_t ModeSel, dma_rx_cb cb);
#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__UART_HAL_H__
