#include "stdio.h"
#include "string.h"
#include "aiio_error.h"
#include "aiio_os_port.h"
#include "mem.h"

#define AIIO_OS_TIMER_OPT_BLOCK_MS_MAX		10*1000

#define AIIO_OS_WAIT_FOREVER				(0xFFFFFFFF)
#define AIIO_OS_NO_WAIT 					(0)
#define AIIO_OS_WAIT_MS2TICK(ms)			\
	( ((ms) == AIIO_OS_WAIT_FOREVER) ? AIIO_OS_WAIT_FOREVER : (((ms)/portTICK_PERIOD_MS)+((((ms)%portTICK_PERIOD_MS)+portTICK_PERIOD_MS-1)/portTICK_PERIOD_MS)) )

int32_t aiio_os_is_inisr(void)
{
    return (int32_t)xPortIsInsideInterrupt();
}

int32_t aiio_os_tick_count(uint32_t *tick)
{
    if (tick == NULL)
    {
        return AIIO_ERROR;
    }

    if (aiio_os_is_inisr())
    {
        *tick = xTaskGetTickCountFromISR();
    }
    else
    {
        *tick = xTaskGetTickCount();
    }

    return AIIO_OK;
}

int32_t aiio_os_thread_self(aiio_os_thread_handle_t *pthread)
{
    *pthread = xTaskGetCurrentTaskHandle();

    if (*pthread)
    {
        return AIIO_OK;
    }
    else
    {
        return AIIO_ERROR;
    }
}

int32_t aiio_os_thread_self_name(char *pthread_name)
{
    if (pthread_name == NULL)
    {
        return AIIO_ERROR;
    }

    memcpy(pthread_name, pcTaskGetName(xTaskGetCurrentTaskHandle()), strlen(pcTaskGetName(xTaskGetCurrentTaskHandle())));
    return AIIO_OK;
}

int32_t aiio_os_tick_dealy(uint32_t tick)
{
    vTaskDelay(tick);
    return AIIO_OK;
}

int32_t aiio_os_mutex_create(aiio_os_mutex_handle_t *phandle)
{
    *phandle = xSemaphoreCreateMutex();

    if (*phandle)
    {
        return AIIO_OK;
    }
    else
    {
        return AIIO_ERROR;
    }
}

int32_t aiio_os_mutex_delete(aiio_os_mutex_handle_t handle)
{
    vSemaphoreDelete(handle);
    return AIIO_OK;
}

int32_t aiio_os_mutex_put(aiio_os_mutex_handle_t handle)
{
    int ret;

    if (aiio_os_is_inisr())
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xSemaphoreGiveFromISR(handle, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xSemaphoreGive(handle);
    }

    return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_mutex_get(aiio_os_mutex_handle_t handle, uint32_t wait_ms)
{
    int ret;

    if (aiio_os_is_inisr())
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xSemaphoreTakeFromISR(handle, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xSemaphoreTake(handle, wait_ms);
    }

    return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_thread_create(aiio_os_thread_handle_t *pthread, const char *name, aiio_os_function_return_t (*function)(void *arg), uint32_t stack_size, void *arg, int priority)
{
    if (pdPASS == xTaskCreate((pdTASK_CODE)function, (const char *const)name, (unsigned short)(stack_size / sizeof(portSTACK_TYPE)),
                              arg, priority, pthread))
    {
        return AIIO_OK;
    }

    return AIIO_ERROR;
}

int32_t aiio_os_thread_delete(aiio_os_thread_handle_t thread)
{
    vTaskDelete(thread);
    return AIIO_OK;
}

int32_t aiio_os_thread_start_scheduler()
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        vTaskStartScheduler();
    }

    return AIIO_OK;
}

uint32_t aiio_os_ms2tick(uint32_t ms)
{
    return ((ms) / portTICK_PERIOD_MS);
}

uint32_t aiio_os_tick2ms(uint32_t tick)
{
    return ((tick)*portTICK_PERIOD_MS);
}

uint32_t aiio_os_get_free_heap_size()
{
    struct meminfo info1 = {0};
    struct meminfo info2 = {0};
    uint32_t total_free_size = 0;
    // return xPortGetFreeHeapSize();

    bflb_mem_usage(KMEM_HEAP,&info1);
    bflb_mem_usage(PMEM_HEAP,&info2);

    total_free_size = info1.free_size + info2.free_size;

    return total_free_size;
}

uint32_t aiio_os_get_minimum_ever_free_heap_size()
{
    return xPortGetMinimumEverFreeHeapSize();
}

int32_t aiio_os_queue_create(aiio_os_queue_handle_t *pqhandle, uint32_t q_len, uint32_t item_size)
{
    *pqhandle = xQueueCreate(q_len, item_size);

    if (*pqhandle)
    {
        return AIIO_OK;
    }
    else
    {
        return AIIO_ERROR;
    }
}

int32_t aiio_os_queue_recv(aiio_os_queue_handle_t qhandle, void *msg, uint32_t wait_ms)
{
    int ret;

    if (aiio_os_is_inisr())
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xQueueReceiveFromISR(qhandle, msg, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xQueueReceive(qhandle, msg, aiio_os_ms2tick(wait_ms));
    }

    return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_queue_delete(aiio_os_queue_handle_t qhandle)
{
    vQueueDelete(qhandle);
    return AIIO_OK;
}

int32_t aiio_os_queue_send(aiio_os_queue_handle_t qhandle, const void *msg, uint32_t wait_ms)
{
    int ret;

    if (aiio_os_is_inisr())
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xQueueSendToBackFromISR(qhandle, msg, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xQueueSendToBack(qhandle, msg, aiio_os_ms2tick(wait_ms));
    }

    return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_queue_get_msgs_waiting(aiio_os_queue_handle_t qhandle)
{
    int nmsg = 0;
    nmsg = uxQueueMessagesWaiting(qhandle);
    return nmsg;
}

int32_t aiio_os_enter_critical(void)
{
    portENTER_CRITICAL();
    return AIIO_OK;
}

int32_t aiio_os_exit_critical(void)
{
    portEXIT_CRITICAL();
    return AIIO_OK;
}

void *aiio_os_malloc(size_t size)
{
    return pvPortMalloc(size);
}

void aiio_os_free(void *prv)
{
    if (prv)
    {
        vPortFree(prv);
    }
}

int32_t aiio_os_semaphore_create_counting(aiio_os_semaphore_handle_t* pmhandle, uint32_t maxcount, uint32_t initcount)
{
	*pmhandle = xSemaphoreCreateCounting(maxcount, initcount);
	if (*pmhandle) {
		return AIIO_OK;
	}
	else {
		return AIIO_ERROR;
	}
}

int32_t aiio_os_semaphore_get(aiio_os_semaphore_handle_t mhandle, uint32_t wait_ms)
{
	int ret;
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreTakeFromISR(mhandle, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreTake(mhandle, AIIO_OS_WAIT_MS2TICK(wait_ms));
	}

	return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_semaphore_put(aiio_os_semaphore_handle_t mhandle)
{
	int ret;
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreGiveFromISR(mhandle, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreGive(mhandle);
	}

	return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_semaphore_getcount(aiio_os_semaphore_handle_t mhandle)
{
	return uxQueueMessagesWaiting(mhandle);
}


int32_t aiio_os_semaphore_delete(aiio_os_semaphore_handle_t mhandle)
{
	vSemaphoreDelete(mhandle);
	return AIIO_OK;
}

int32_t aiio_os_timer_create(aiio_os_timer_handle_t* ptimer, const char *name, uint32_t ms,
		    void (*call_back)(aiio_os_timer_handle_t), void *context,
		    aiio_os_timer_reload_t reload,
		    aiio_os_timer_activate_t activate)
{
	int auto_reload = (reload == AIIO_OS_TIMER_ONE_SHOT) ? pdFALSE : pdTRUE;
	uint32_t ticks = AIIO_OS_WAIT_MS2TICK(ms);

	*ptimer = xTimerCreate((const char * const)name, ticks,
				      auto_reload, context, call_back);
	if (*ptimer == NULL)
		return AIIO_ERROR;

	if (activate == AIIO_OS_TIMER_AUTO_ACTIVATE){
		if(pdTRUE != xTimerStart(*ptimer, ticks)){
			xTimerDelete(*ptimer, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
			*ptimer = NULL;
			return AIIO_ERROR;
		}
	}

	return AIIO_OK;
}


int32_t aiio_os_timer_activate(aiio_os_timer_handle_t timer)
{
	int ret;
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xTimerStartFromISR(timer, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xTimerStart(timer, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
	}

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}


int32_t aiio_os_timer_change(aiio_os_timer_handle_t timer, uint32_t ms)
{
	int ret;
	uint32_t ticks = AIIO_OS_WAIT_MS2TICK(ms);
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xTimerChangePeriodFromISR(timer, ticks, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {

		ret = xTimerChangePeriod(timer, ticks, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
	}

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}


int32_t aiio_os_timer_is_active(aiio_os_timer_handle_t timer)
{
	int ret;

	ret = xTimerIsTimerActive(timer);

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}

void *aiio_os_timer_get_context(aiio_os_timer_handle_t timer)
{
	return (void*)pvTimerGetTimerID(timer);
}

int32_t aiio_os_timer_reset(aiio_os_timer_handle_t timer)
{
	int ret;
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xTimerResetFromISR(timer, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xTimerReset(timer, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
	}

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_timer_deactivate(aiio_os_timer_handle_t timer)
{
	int ret;
	if (aiio_os_is_inisr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xTimerStopFromISR(timer, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xTimerStop(timer, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
	}

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}


int32_t aiio_os_timer_delete(aiio_os_timer_handle_t timer)
{
	int ret;
	ret = xTimerDelete(timer, aiio_os_ms2tick(AIIO_OS_TIMER_OPT_BLOCK_MS_MAX));
	if(pdPASS != ret){
		//TODO
	}

	return ret == pdPASS ? AIIO_OK : AIIO_ERROR;
}


int32_t aiio_os_event_create(aiio_os_event_handle_t *event)
{
    *event = xEventGroupCreate();

    if (*event)
    {
        return AIIO_OK;
    }
    else
    {
        return AIIO_ERROR;
    }
}

int32_t aiio_os_event_set_bit(aiio_os_event_handle_t event, const int32_t bits_to_set)
{
    int ret;

    if (aiio_os_is_inisr())
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        ret = xEventGroupSetBitsFromISR(event, bits_to_set, &xHigherPriorityTaskWoken);
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        ret = xEventGroupSetBits(event, bits_to_set);
    }

    return ret == pdTRUE ? AIIO_OK : AIIO_ERROR;
}

int32_t aiio_os_event_wait_bits(aiio_os_event_handle_t event, const uint32_t bits_to_wait_for, const uint32_t clear_on_exit, const uint32_t wait_for_all_bit, uint32_t wait_ms)
{
    return xEventGroupWaitBits(event, bits_to_wait_for, clear_on_exit, wait_for_all_bit, wait_ms);
}

int32_t aiio_os_event_clear_bits(aiio_os_event_handle_t event, const uint32_t bits_to_clear)
{
    return xEventGroupClearBits(event, bits_to_clear);
}

int32_t aiio_os_event_clear_bits_from_isr(aiio_os_event_handle_t event, const uint32_t bits_to_clear)
{
    return xEventGroupClearBitsFromISR(event, bits_to_clear);
}

int32_t aiio_os_event_get_bits(aiio_os_event_handle_t event)
{
    return xEventGroupGetBits(event);
}

int32_t aiio_os_event_get_bits_from_isr(aiio_os_event_handle_t event)
{
    return xEventGroupGetBitsFromISR(event);
}

int32_t aiio_os_event_delete(aiio_os_event_handle_t event)
{
    vEventGroupDelete(event);

    return AIIO_OK;
}

