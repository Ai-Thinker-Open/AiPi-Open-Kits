/*
 * retarget.c
 *
 *  Created on: Mar 8, 2017
 *      Author: peter
 */

#include <stdio.h>
#include "type.h"

#define SRAM_END_ADDR		0x20038000

extern void (*UARTS_SendByte_MAP[])(uint8_t SendByte);

#define UARTS_SendByte(uartnum, SendByte) (*UARTS_SendByte_MAP[uartnum])(SendByte)

uint8_t DebugPrintPort = 0;
uint32_t gSramEndAddr = SRAM_END_ADDR;

uint8_t IsSwUartActedAsUARTFlag = 0;
void EnableSwUartAsUART(uint8_t EnableFlag)
{
	IsSwUartActedAsUARTFlag = EnableFlag;
}

//This is used as dummy function in case that appilcation dont define this function.
__attribute__((weak))
void SwUartSend(unsigned char* Buf, unsigned int BufLen)
{

}

__attribute__((used))
int putchar(int c)
{
    if(IsSwUartActedAsUARTFlag)
    {
        if((unsigned char)c == '\n')
        {
            const char lfca[2] = "\r\n";
            SwUartSend((unsigned char*)lfca, 2);
        }
        else
        {
            SwUartSend((unsigned char*)&c, 1);
        }
    }
    else
    {
    	if (c == '\n')
    	{
    		UARTS_SendByte(DebugPrintPort, '\r');
    		UARTS_SendByte(DebugPrintPort, '\n');
    	}
    	else
    	{
    		UARTS_SendByte(DebugPrintPort, (uint8_t)c);
    	}
    }

	return c;
}

__attribute__((used))
void nds_write(const unsigned char *buf, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		putchar(buf[i]);
	}
}

