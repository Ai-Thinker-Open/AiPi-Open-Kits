/** @brief      RTOS application interface.
 *
 *  @file       aiio_os_port.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       RTOS application interface.
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/01/30      <td>1.0.0       <td>chenzf      <td>Define file
 *  <tr><td>2023/02/16      <td>1.0.1       <td>yanch       <td>Add system application memory and release memory interface
 *  </table>
 *
 */
#ifndef __AIIO_OS_PORT_H__
#define __AIIO_OS_PORT_H__

#include "aiio_type.h"
#include "aiio_chip_spec.h"


#define aiio_portMAX_DELAY          (uint32_t)0xffffffffUL


typedef void *aiio_os_mutex_handle_t;
typedef void *aiio_os_thread_handle_t;
typedef void *aiio_os_queue_handle_t;
typedef void *aiio_os_function_return_t;
typedef void *aiio_os_semaphore_handle_t;
typedef void *aiio_os_timer_handle_t;
typedef void *aiio_os_event_handle_t;
#define ARCH_OS_FUNCTION_RETURN(v) ((aiio_os_function_return_t)(v))

CHIP_API int32_t aiio_os_enter_critical(void);
CHIP_API int32_t aiio_os_exit_critical(void);

CHIP_API int32_t aiio_os_is_inisr(void);
CHIP_API int32_t aiio_os_tick_dealy(uint32_t tick);
CHIP_API int32_t aiio_os_tick_count(uint32_t *tick);
CHIP_API int32_t aiio_os_thread_self(aiio_os_thread_handle_t *pthread);
CHIP_API int32_t aiio_os_thread_self_name(char *pthread_name);

CHIP_API int32_t aiio_os_mutex_create(aiio_os_mutex_handle_t *phandle);
CHIP_API int32_t aiio_os_mutex_delete(aiio_os_mutex_handle_t handle);
CHIP_API int32_t aiio_os_mutex_put(aiio_os_mutex_handle_t handle);
CHIP_API int32_t aiio_os_mutex_get(aiio_os_mutex_handle_t handle, uint32_t wait_ms);

CHIP_API int32_t aiio_os_thread_create(aiio_os_thread_handle_t *pthread, const char *name, aiio_os_function_return_t (*function)(void *), uint32_t stack_size, void *arg, int priority);
CHIP_API int32_t aiio_os_thread_delete(aiio_os_thread_handle_t thread);
CHIP_API int32_t aiio_os_thread_start_scheduler();

CHIP_API uint32_t aiio_os_ms2tick(uint32_t ms);
CHIP_API uint32_t aiio_os_tick2ms(uint32_t tick);

CHIP_API uint32_t aiio_os_get_free_heap_size();
CHIP_API uint32_t aiio_os_get_minimum_ever_free_heap_size();

CHIP_API int32_t aiio_os_queue_create(aiio_os_queue_handle_t *pqhandle, uint32_t q_len, uint32_t item_size);
CHIP_API int32_t aiio_os_queue_recv(aiio_os_queue_handle_t qhandle, void *msg, uint32_t wait_ms);
CHIP_API int32_t aiio_os_queue_delete(aiio_os_queue_handle_t qhandle);
CHIP_API int32_t aiio_os_queue_send(aiio_os_queue_handle_t qhandle, const void *msg, uint32_t wait_ms);
CHIP_API int32_t aiio_os_queue_get_msgs_waiting(aiio_os_queue_handle_t qhandle);

CHIP_API int32_t aiio_os_semaphore_create_counting(aiio_os_semaphore_handle_t* pmhandle, uint32_t maxcount, uint32_t initcount);
CHIP_API int32_t aiio_os_semaphore_create(aiio_os_semaphore_handle_t* mhandle);
CHIP_API int32_t aiio_os_semaphore_get(aiio_os_semaphore_handle_t mhandle, uint32_t wait_ms);
CHIP_API int32_t aiio_os_semaphore_put(aiio_os_semaphore_handle_t mhandle);
CHIP_API int32_t aiio_os_semaphore_delete(aiio_os_semaphore_handle_t mhandle);

CHIP_API int32_t aiio_os_event_create(aiio_os_event_handle_t *event);
CHIP_API int32_t aiio_os_event_set_bit(aiio_os_event_handle_t event, const int32_t bits_to_set);
CHIP_API int32_t aiio_os_event_wait_bits(aiio_os_event_handle_t event, const uint32_t bits_to_wait_for, const uint32_t clear_on_exit, const uint32_t wait_for_all_bit, uint32_t wait_ms);
CHIP_API int32_t aiio_os_event_clear_bits(aiio_os_event_handle_t event, const uint32_t bits_to_clear);
CHIP_API int32_t aiio_os_event_clear_bits_from_isr(aiio_os_event_handle_t event, const uint32_t bits_to_clear);
CHIP_API int32_t aiio_os_event_get_bits(aiio_os_event_handle_t event);
CHIP_API int32_t aiio_os_event_get_bits_from_isr(aiio_os_event_handle_t event);
CHIP_API int32_t aiio_os_event_delete(aiio_os_event_handle_t event);

typedef enum os_timer_reload {
	/**
	 * Create one shot timer. Timer will be in the dormant state after
	 * it expires.
	 */
	AIIO_OS_TIMER_ONE_SHOT,
	/**
	 * Create a periodic timer. Timer will auto-reload after it expires.
	 */
	AIIO_OS_TIMER_PERIODIC,
} aiio_os_timer_reload_t;

/**
 * OS Timer Activate Options
 */
typedef enum os_timer_activate {
	/** Start the timer on creation. */
	AIIO_OS_TIMER_AUTO_ACTIVATE,
	/** Do not start the timer on creation. */
	AIIO_OS_TIMER_NO_ACTIVATE,
} aiio_os_timer_activate_t;

CHIP_API int32_t aiio_os_timer_create(aiio_os_timer_handle_t *ptimer, const char *name, uint32_t ms,
		    void (*call_back)(arch_os_timer_handle_t), void *context,
		    aiio_os_timer_reload_t reload,
		    aiio_os_timer_activate_t activate);

CHIP_API int32_t aiio_os_timer_activate(aiio_os_timer_handle_t timer);
CHIP_API int32_t aiio_os_timer_change(aiio_os_timer_handle_t timer, uint32_t ms);
CHIP_API int32_t aiio_os_timer_is_active(aiio_os_timer_handle_t timer);
CHIP_API void *aiio_os_timer_get_context(aiio_os_timer_handle_t timer);
CHIP_API int32_t aiio_os_timer_reset(aiio_os_timer_handle_t timer);
CHIP_API int32_t aiio_os_timer_deactivate(aiio_os_timer_handle_t timer);
CHIP_API int32_t aiio_os_timer_delete(aiio_os_timer_handle_t timer);



/**
 * @brief Apply memory function
 *
 *
 * @param[in] size  memory size
 *
 * @return  Return apply memory status. When the return value is AIIO_OK, is successful.
 * @retval  AIIO_OK         Malloc successful.
 * @retval  AIIO_ERROR      Malloc error.
 *
 * @note
 * @see
 */
CHIP_API void *aiio_os_malloc(size_t size);

/**
 * @brief Free memory function
 *
 * @param[in] prv addr
 *
 * @return  Return free memory status. When the return value is AIIO_OK, is successful.
 * @retval  AIIO_OK         Free successful.
 * @retval  AIIO_ERROR      Free error.
 *
 * @note
 * @see
 */
CHIP_API void aiio_os_free(void *prv);

#endif
