/**
 * @brief   GPIO intermediate device port adaptation
 * 
 * @file    aiio_gpio.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-27          <td>1.0.0            <td>zhuolm             <td> device port adaptation
 */
#include "stdint.h"
#include "aiio_gpio.h"
#include "bflb_gpio.h"
#include "aiio_log.h"

#define DEF_PIN_ID_MAX (63)

typedef struct
{
    uint8_t inited ;  /**< 0 deinit 1 init*/
    bool    irq_en;
    uint8_t pin;
    uint8_t cfg_in_out;
    uint8_t cfg_pudn;
    uint8_t cfg_smt_en;
    uint8_t cfg_drv;
    struct bflb_device_s *gpio;
    aiio_hal_gpio_int_cb_t aiio_hal_gpio_int_cb;
} aiio_gpio_t;

static aiio_gpio_t io_port[DEF_PIN_ID_MAX + 1] = {0};




aiio_ret_t aiio_hal_gpio_init(uint32_t port, uint16_t pin)
{
    struct bflb_device_s *gpio;
    (void)port;

    if (io_port[pin].inited || pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    gpio = bflb_device_get_by_name("gpio");

    io_port[pin].pin = pin;
    io_port[pin].gpio = gpio;
    io_port[pin].cfg_smt_en = GPIO_SMT_EN;
    io_port[pin].cfg_drv = GPIO_DRV_0;

    bflb_gpio_init(io_port[pin].gpio, io_port[pin].pin, io_port[pin].cfg_in_out | io_port[pin].cfg_pudn | io_port[pin].cfg_smt_en | io_port[pin].cfg_drv);
    io_port[pin].inited = 1;

    return AIIO_SUCC;
}


aiio_ret_t aiio_hal_gpio_deinit(uint32_t port, uint16_t pin)
{
    (void)port;

    if (!io_port[pin].inited || pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    if (io_port[pin].irq_en)
    {
        io_port[pin].irq_en = false;
        bflb_gpio_int_mask(io_port[pin].gpio, pin, io_port[pin].irq_en);
    }

    bflb_gpio_deinit(io_port[pin].gpio, pin);
    io_port[pin].inited = 0;
    return AIIO_SUCC;
}


int aiio_hal_gpio_pin_direction_set(uint32_t port, uint16_t pin, aiio_gpio_direction_t dir)
{
    (void)port;

    if (pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    if (AIIO_GPIO_INPUT == dir)
    {
        io_port[pin].cfg_in_out = GPIO_INPUT;
    }
    else
    {
        io_port[pin].cfg_in_out = GPIO_OUTPUT;
    }

    if(io_port[pin].inited)
    {
        aiio_hal_gpio_deinit(port, pin);
        aiio_hal_gpio_init(port, pin);
    }
    else
    {
        aiio_hal_gpio_init(port, pin);
    }

    return AIIO_SUCC;
}


int aiio_hal_gpio_pin_pull_set(uint32_t port, uint16_t pin, aiio_gpio_pull_t pull)
{
    (void)port;

    if (pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    if (AIIO_GPIO_PULL_DOWN == pull)
    {
        io_port[pin].cfg_pudn = GPIO_PULLDOWN;
    }
    else
    {
        io_port[pin].cfg_pudn = GPIO_PULLUP;
    }

    if(io_port[pin].inited)
    {
        aiio_hal_gpio_deinit(port, pin);
        aiio_hal_gpio_init(port, pin);
    }
    else
    {
        aiio_hal_gpio_init(port, pin);
    }

    return AIIO_SUCC;
}


aiio_ret_t aiio_hal_gpio_set(uint32_t port, uint16_t pin, uint8_t OnOff)
{
    (void)port;
    if (!io_port[pin].inited || pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    if (OnOff)
    {
        bflb_gpio_set(io_port[pin].gpio, io_port[pin].pin);
    }
    else
    {
        bflb_gpio_reset(io_port[pin].gpio, io_port[pin].pin);
    }
    return AIIO_SUCC;
}


uint8_t aiio_hal_gpio_get(uint32_t port, uint16_t pin)
{
    (void)port;
    uint8_t value;
    if (!io_port[pin].inited || pin > DEF_PIN_ID_MAX)
        return AIIO_ERR_INIT_FAIL;

    if(bflb_gpio_read(io_port[pin].gpio, io_port[pin].pin))
    {
        value = 1;
    }
    else
    {
        value = 0;
    }

    return value;
}


aiio_ret_t aiio_hal_gpio_int_enable(uint32_t port, uint16_t pin, uint8_t en)
{
    return AIIO_SUCC;
}


aiio_ret_t aiio_hal_gpio_int_register(uint32_t port, uint16_t pin, aiio_hal_gpio_int_cb_t cb)
{
    return AIIO_SUCC;
}
