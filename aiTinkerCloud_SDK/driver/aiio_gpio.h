/** @brief GPIO HAL layer header file
 *
 *  @file        aiio_gpio.h
 *  @copyright   Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note
 *  @par         Change Logs:
 *  <table>
 *  <tr><th>Date                <th>Version         <th>Author          <th>Notes
 *  <tr><th>2023/02/06          <td>V1.0.0          <td>hewm            <td>Define the GPIO header API
 *  <table>
 *
 */

#ifndef __AIIO_HAL_GPIO_H__
#define __AIIO_HAL_GPIO_H__

#include "stdint.h"
#include "aiio_type.h"
#include "aiio_chip_spec.h"
#include "aiio_errcode.h"

#ifdef __cplusplus
extern "C" {
#endif


#define     AIIO_GPIO_P_ENABLE      __AIIO_GPIO_P_ENABLE
#define     AIIO_GPIO_A             __AIIO_GPIO_A
#define     AIIO_GPIO_B             __AIIO_GPIO_B

#define     AIIO_GPIO_NUM_0         __AIIO_GPIO_NUM_0
#define     AIIO_GPIO_NUM_1         __AIIO_GPIO_NUM_1
#define     AIIO_GPIO_NUM_2         __AIIO_GPIO_NUM_2
#define     AIIO_GPIO_NUM_3         __AIIO_GPIO_NUM_3
#define     AIIO_GPIO_NUM_4         __AIIO_GPIO_NUM_4
#define     AIIO_GPIO_NUM_5         __AIIO_GPIO_NUM_5
#define     AIIO_GPIO_NUM_6         __AIIO_GPIO_NUM_6
#define     AIIO_GPIO_NUM_7         __AIIO_GPIO_NUM_7
#define     AIIO_GPIO_NUM_8         __AIIO_GPIO_NUM_8
#define     AIIO_GPIO_NUM_9         __AIIO_GPIO_NUM_9
#define     AIIO_GPIO_NUM_10        __AIIO_GPIO_NUM_10
#define     AIIO_GPIO_NUM_11        __AIIO_GPIO_NUM_11
#define     AIIO_GPIO_NUM_12        __AIIO_GPIO_NUM_12
#define     AIIO_GPIO_NUM_13        __AIIO_GPIO_NUM_13
#define     AIIO_GPIO_NUM_14        __AIIO_GPIO_NUM_14
#define     AIIO_GPIO_NUM_15        __AIIO_GPIO_NUM_15
#define     AIIO_GPIO_NUM_16        __AIIO_GPIO_NUM_16
#define     AIIO_GPIO_NUM_17        __AIIO_GPIO_NUM_17
#define     AIIO_GPIO_NUM_18        __AIIO_GPIO_NUM_18
#define     AIIO_GPIO_NUM_19        __AIIO_GPIO_NUM_19
#define     AIIO_GPIO_NUM_20        __AIIO_GPIO_NUM_20
#define     AIIO_GPIO_NUM_21        __AIIO_GPIO_NUM_21
#define     AIIO_GPIO_NUM_22        __AIIO_GPIO_NUM_22
#define     AIIO_GPIO_NUM_23        __AIIO_GPIO_NUM_23
#define     AIIO_GPIO_NUM_24        __AIIO_GPIO_NUM_24
#define     AIIO_GPIO_NUM_25        __AIIO_GPIO_NUM_25
#define     AIIO_GPIO_NUM_26        __AIIO_GPIO_NUM_26
#define     AIIO_GPIO_NUM_27        __AIIO_GPIO_NUM_27
#define     AIIO_GPIO_NUM_28        __AIIO_GPIO_NUM_28
#define     AIIO_GPIO_NUM_29        __AIIO_GPIO_NUM_29
#define     AIIO_GPIO_NUM_30        __AIIO_GPIO_NUM_30
#define     AIIO_GPIO_NUM_31        __AIIO_GPIO_NUM_31
#define     AIIO_GPIO_NUM_32        __AIIO_GPIO_NUM_32
#define     AIIO_GPIO_NUM_33        __AIIO_GPIO_NUM_33
#define     AIIO_GPIO_NUM_34        __AIIO_GPIO_NUM_34
#define     AIIO_GPIO_NUM_35        __AIIO_GPIO_NUM_35
#define     AIIO_GPIO_NUM_36        __AIIO_GPIO_NUM_36
#define     AIIO_GPIO_NUM_37        __AIIO_GPIO_NUM_37
#define     AIIO_GPIO_NUM_38        __AIIO_GPIO_NUM_38
#define     AIIO_GPIO_NUM_39        __AIIO_GPIO_NUM_39
#define     AIIO_GPIO_NUM_40        __AIIO_GPIO_NUM_40

/**
 * @brief Enumerate GPIO interrupt types
 */
typedef enum
{
    AGPIO_INT_DISABLE = 0,    /**< Interrupts are disabled by default*/
    AGPIO_INT_RISING,         /**< Rising edge trigger*/
    AGPIO_INT_FALLING,        /**< Falling edge trigger*/
    AGPIO_INT_RISING_FALLING, /**< Bilateral edge trigger*/
} aiio_gpio_int_t;
typedef enum
{
    AIIO_GPIO_INPUT = 0,
    AIIO_GPIO_OUTPUT = 1,
} aiio_gpio_direction_t;

typedef enum
{
    AIIO_GPIO_PULL_DOWN = 0,
    AIIO_GPIO_PULL_UP = 1,
} aiio_gpio_pull_t;
/**
 * @brief IO Interrupt callback functions
 */
typedef void(*aiio_hal_gpio_int_cb_t)(void *param);

/**
 * @brief API to init specific GPIO
 * @param  port The IO port to be initialized
 * @param  pin  The IO pins to be initialized
 * @param  cfg  The configuration information of the IO to be initialized
 * @return Initialization returns the device handle successfully, and the failure returns the 0xffff
 */
CHIP_API aiio_ret_t aiio_hal_gpio_init(uint32_t port, uint16_t pin);

/**
 * @brief API to deinit specific GPIO
 * @param  port The IO port to be initialized
 * @param  pin  The IO pins to be initialized
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E
 */
CHIP_API aiio_ret_t aiio_hal_gpio_deinit(uint32_t port, uint16_t pin);

/**
 * @brief API to set the GPIO output direction
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @param  dir output direction @see aiio_gpio_direction_t
 * @return Status  - 0: Succeed, else: fail
 */
CHIP_API int aiio_hal_gpio_pin_direction_set(uint32_t port, uint16_t pin, aiio_gpio_direction_t dir);

/**
 * @brief API to set the GPIO pull-up mode
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @param  pull pull-up mode @see aiio_gpio_pull_t
 * @return Status  - 0: Succeed, else: fail
 */
CHIP_API int aiio_hal_gpio_pin_pull_set(uint32_t port, uint16_t pin, aiio_gpio_pull_t pull);

/**
 * @brief API to set a GPIO to specified mode
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @param  OnOff Set GPIO status 0 or 1
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E
 */
CHIP_API aiio_ret_t aiio_hal_gpio_set(uint32_t port, uint16_t pin, uint8_t OnOff);

/**
 * @brief API to get a GPIO level
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E 
 */
CHIP_API uint8_t aiio_hal_gpio_get(uint32_t port, uint16_t pin);

/**
 * @brief API to enable a GPIO Interrupt
 * @param  handle Device handle
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E
 */

/**
 * @brief API to enable a GPIO Interrupt
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @param  en Enable GPIO interrupts
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E 
 */
CHIP_API aiio_ret_t aiio_hal_gpio_int_enable(uint32_t port, uint16_t pin, uint8_t en);

/**
 * @brief API to register an interrupt callback function
 * @param  port The IO port to be initialized
 * @param  pin The IO pins to be initialized
 * @param  cb Interrupt callback functions @see aiio_hal_gpio_int_cb_t
 * @return Successfully returned AIIO_SUCC, Other failed. @see AIIO_ERR_E
 */
CHIP_API aiio_ret_t aiio_hal_gpio_int_register(uint32_t port, uint16_t pin, aiio_hal_gpio_int_cb_t cb);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__AIIO_HAL_GPIO_H__
