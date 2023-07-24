#include <stdint.h>
#include <stddef.h>
#include "type.h"
#include "rtos_api.h"
//#include "remap.h"
#include "task.h"
#ifdef CFG_APP_CONFIG
#include "app_config.h"
#endif
#include <nds32_intrinsic.h>

extern uint32_t gSramEndAddr;
extern void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions ); //defined by heap_5s.c add for warning by pi
uint32_t GetIPSR( void );
/* Determine whether we are in thread mode or handler mode. */
uint32_t inHandlerMode (void)
{
	//return 0;
	return GetIPSR();
}

bool MessageSend(MessageHandle msgHandle,  MessageContext * msgContext)
{
	portBASE_TYPE taskWoken = pdFALSE;

	if(msgHandle == NULL)
	{
		return  FALSE;
	}
	if (inHandlerMode())
	{
		if (xQueueSendFromISR(msgHandle, msgContext, &taskWoken) != pdTRUE)
		{
			return FALSE;
		}
		portEND_SWITCHING_ISR(taskWoken);
	}
	else
	{
		if (xQueueSend(msgHandle, msgContext, 0) != pdTRUE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void MessageSendx(MessageHandle msgHandle,  MessageContext * msgContext)
{
	if(msgHandle == NULL)
	{
		return ;
	}
	xQueueSend(msgHandle, msgContext, 0xFFFFFFFF);
}


bool MessageRecv(MessageHandle msgHandle, MessageContext * msgContext, uint32_t millisec)
{
	portBASE_TYPE	taskWoken;
	bool			ret = FALSE;

	if (msgHandle == NULL)
	{
		return FALSE;
	}

	taskWoken = pdFALSE;
	msgContext->msgId = MSG_INVAILD;

	if (inHandlerMode())
	{
		if (xQueueReceiveFromISR(msgHandle, msgContext, &taskWoken) == pdTRUE)
		{
			/* We have mail */
			ret = TRUE;
		}
		else
		{
			ret = FALSE;
		}
		portEND_SWITCHING_ISR(taskWoken);
	}
	else 
	{
		if (xQueueReceive(msgHandle, msgContext, millisec) == pdTRUE)
		{
			/* We have mail */
			ret = TRUE;
		}
		else 
		{
			ret = FALSE;
		}
	}

	return ret;
}

void prvInitialiseHeap(void)
{
	extern char _end;
	HeapRegion_t xHeapRegions[2];

	xHeapRegions[0].pucStartAddress = (uint8_t*)&_end;
	xHeapRegions[0].xSizeInBytes = gSramEndAddr-(uint32_t)&_end;

	xHeapRegions[1].pucStartAddress = NULL;
	xHeapRegions[1].xSizeInBytes = 0;

	vPortDefineHeapRegions( (HeapRegion_t *)xHeapRegions );
}

//note: 改函数必须在prvInitialiseHeap()函数调用之后
void osSemaphoreMutexCreate(void)
{
    if(UART0Mutex == NULL)
    {
    	UART0Mutex = xSemaphoreCreateMutex();
    }
	if(UART1Mutex == NULL)
	{
		UART1Mutex = xSemaphoreCreateMutex();
	}
	if(SDIOMutex == NULL)
	{
		SDIOMutex = osMutexCreate();
	}
#ifdef CFG_RES_UDISK_USE
	if(UDiskMutex == NULL)
	{
		UDiskMutex = osMutexCreate();
	}
#endif
#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	if(AudioEffectMutex == NULL)
	{
		AudioEffectMutex = osMutexCreate();
	}
#endif
}

/**
* @brief Allocate a memory block from a memory pool
* @param  osWantedSize Allocate memory word(1word=4byte) size
* @retval  address of the allocated memory block or NULL in case of no memory available.
*/
void *osPortMalloc(uint16_t osWantedSize)
{
	void *ospvReturn = NULL;

	ospvReturn=pvPortMalloc(osWantedSize);
	//DBG("Malloc11:%d->%d, Add=%x\n", (int)osWantedSize, (int)xPortGetFreeHeapSize(), ospvReturn);
	return ospvReturn;
}
void *pvPortMallocFromEnd( size_t xWantedSize );
/**
* @brief Allocate a memory block from a memory pool
* @param  osWantedSize Allocate memory word(1word=4byte) size
* @retval  address of the allocated memory block or NULL in case of no memory available.
*/
void *osPortMallocFromEnd(uint32_t osWantedSize)
{
	void *ospvReturn = NULL;

	//DBG("\nMalloc22:%d, %d\n", xPortGetFreeHeapSize(), osWantedSize);
	ospvReturn = pvPortMallocFromEnd(osWantedSize);
	//DBG("Malloc33:%d->%d, Add=%x\n", (int)osWantedSize, (int)xPortGetFreeHeapSize(), ospvReturn);
	return ospvReturn;
}

int osPortRemainMem(void)
{
	return (int)xPortGetFreeHeapSize();
}

/**
* @brief Free a memory block
* @param address of the allocated memory block.
*/
void osPortFree(void *ospv)
{
	//DBG("Add=%x\n", ospv);
	vPortFree(ospv);
}

void osTaskDelay(uint32_t Cnt)
{
	vTaskDelay(Cnt);
}

//PRIVILEGED_DATA static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;

uint32_t SysemMipsPercent;
static uint32_t UlRunTimeCounterOffset=0;
static uint32_t UlTotalTimeOffset=0;
#define StatsPeriod (60*1000)//ms
void vApplicationIdleHook(void)
{
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	uint32_t ulTotalTime, ulStatsAsPercentage;

	uxArraySize = uxTaskGetNumberOfTasks();

	pxTaskStatusArray = pvPortMalloc( uxTaskGetNumberOfTasks() * sizeof( TaskStatus_t ) );

	if( pxTaskStatusArray != NULL )
	{
		uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalTime );
		ulTotalTime /= 100UL;

		if( ulTotalTime > 0 )
		{
			for( x = 0; x < uxArraySize; x++ )
			{
				ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalTime;
				if(( ulStatsAsPercentage > 0UL ))// && (pxTaskStatusArray[ x ].pcTaskName == 'IDLE'))
				{
					char *P_T = (char *)pxTaskStatusArray[ x ].pcTaskName;
					if((P_T[0] == 'I') && (P_T[1] == 'D') && (P_T[2] == 'L') && (P_T[3] == 'E'))
					{
						if(SysemMipsPercent==0)
						{
							SysemMipsPercent = ulStatsAsPercentage;
						}

						if(ulTotalTime-UlTotalTimeOffset>0)
						{
							ulStatsAsPercentage = (pxTaskStatusArray[ x ].ulRunTimeCounter-UlRunTimeCounterOffset) / (ulTotalTime-UlTotalTimeOffset);
							SysemMipsPercent = ulStatsAsPercentage;
							if(ulTotalTime-UlTotalTimeOffset>(StatsPeriod/100))
							{
								UlRunTimeCounterOffset=pxTaskStatusArray[ x ].ulRunTimeCounter;
								UlTotalTimeOffset=ulTotalTime;
							}
						}
						break;
					}
				}
			}
		}
	}

	vPortFree( pxTaskStatusArray );

#ifdef CFG_GOTO_SLEEP_USE
	//#include <nds32_intrinsic.h>
	if(__nds32__mfsr(NDS32_SR_INT_PEND2))
	{
		__nds32__mtsr(__nds32__mfsr(NDS32_SR_INT_PEND2), NDS32_SR_INT_PEND2);
	}
	__nds32__standby_no_wake_grant();
#endif
}

#ifdef use_MCPS_ANALYSIS
//#include <nds32_intrinsic.h>
typedef struct _TASK_COUT_LOG
{
	uint32_t cout;
	uint8_t name[8];
} TASK_COUT_LOG;

#define TASK_COUT_LOG_NUM  100

TASK_COUT_LOG TaskCountLog[TASK_COUT_LOG_NUM];
uint32_t LogCont=0;

void trace_TASK_SWITCHED_IN(void)
{
	__nds32__mtsr(0, NDS32_SR_PFMC0);
	__nds32__mtsr(1, NDS32_SR_PFM_CTL);
}

void trace_TASK_SWITCHED_OUT(void)
{
	if(LogCont >= TASK_COUT_LOG_NUM)
		LogCont = 0;

	__nds32__mtsr(0, NDS32_SR_PFM_CTL);
	TaskCountLog[LogCont].cout = __nds32__mfsr(NDS32_SR_PFMC0);
	memcpy(&TaskCountLog[LogCont].name[0], (void *)pcTaskGetTaskName(NULL), 7);
	TaskCountLog[LogCont].name[7] = 0;
	LogCont++;
}

void DisplayMcpsInfo(void)
{
	uint32_t i;
	GIE_DISABLE();
	for(i=0;  i<TASK_COUT_LOG_NUM; i++)
	{
		printf("%08s  %d\n", &TaskCountLog[i].name[0], TaskCountLog[i].cout);
	}
	GIE_ENABLE();
}

#endif
