/**
 * @file CST816D_i2c.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include "touch.h"

#ifdef TOUCH_I2C_CST816D
#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_i2c.h"
#include "CST816D_i2c.h"

static struct bflb_device_s* touch_cst816d_i2c = NULL;

typedef struct {
    uint8_t fresh;
    uint16_t x;
    uint16_t y;
    uint16_t size;
} POINT;

POINT point[CST816D_MAX_TOUCH_POINT];

static struct sw_touch_panel_platform_data* _touch_panel_data = NULL;
struct sw_touch_panel_platform_data touch_panel_data;

static void cst816d_i2c_gpio_init(void)
{
    struct bflb_device_s* ft63x6_i2c_gpio = NULL;
    ft63x6_i2c_gpio = bflb_device_get_by_name("gpio");

    /* I2C0_SCL */
    bflb_gpio_init(ft63x6_i2c_gpio, TOUCH_I2C_SCL_PIN, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(ft63x6_i2c_gpio, TOUCH_I2C_SDA_PIN, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

static int cst816d_i2c_peripheral_init(void)
{
    touch_cst816d_i2c = bflb_device_get_by_name("i2c0");

    if (touch_cst816d_i2c) {
        // printf("cst816d i2c gpio init\r\n");
        /* init i2c gpio */
        cst816d_i2c_gpio_init();
        /* init i2c 200k */
        bflb_i2c_init(touch_cst816d_i2c, 200000);
    }
    else {
        printf("i2c device get fail\r\n");
        return -1;
    }

    return 0;
}

static int cst816d_i2c_write_byte(uint8_t register_addr, uint8_t* data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = CST816D_IIC_ADDR;
    msg[0].flags = 0;
    msg[0].buffer = &register_addr;
    msg[0].length = 1;

    msg[1].addr = CST816D_IIC_ADDR;
    msg[1].flags = 0;
    msg[1].buffer = data_buf;
    msg[1].length = len;

    uint8_t ret = bflb_i2c_transfer(touch_cst816d_i2c, msg, 2);
    printf("i2c_write_ret:%d\r\n", ret);
    return 0;
}

static int cst816d_i2c_read_byte(uint8_t register_addr, uint8_t* data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = CST816D_IIC_ADDR;
    msg[0].flags = 0;
    msg[0].buffer = &register_addr;
    msg[0].length = 1;

    msg[1].addr = CST816D_IIC_ADDR;
    msg[1].flags = I2C_M_READ;
    msg[1].buffer = data_buf;
    msg[1].length = len;
    bflb_i2c_transfer(touch_cst816d_i2c, msg, 2);

    return 0;
}

int cst816d_get_gesture_id()
{
    uint8_t data_buf = 0;

    if (cst816d_i2c_read_byte(CST816D_REG_VENDOR_ID_ADDR, &data_buf, 1)) {
        return -1;
    }

    return data_buf;
}

int cst816d_i2c_init(touch_coord_t* max_value)
{
    uint8_t data_buf[3] = { 0 };
    printf("cst816d i2c init\r\n");

    _touch_panel_data = &touch_panel_data;
    cst816d_i2c_peripheral_init();

    if (cst816d_i2c_read_byte(CST816D_REG_VENDOR_ID_ADDR, &data_buf, 1)) {
        return -1;
    }
    printf("Touch Device ID: 0x%02x\r\n", data_buf[0]);

    // if (cst816d_i2c_read_byte(CST816D_REG_IC_YTPE_ADDR, &data_buf, 1)) {
    //     return -1;
    // }
    // printf("Touch Chip ID: 0x%02x\r\n", data_buf);

    data_buf[0] = 0x60;
    cst816d_i2c_write_byte(0xFA, data_buf, 1);


    return 0;
}

int cst816d_i2c_read(uint8_t* point_num, touch_coord_t* touch_coord, uint8_t max_num)
{
    uint8_t point_data[10] = { 0 };
    uint8_t touch_num = 0, touch_type, touch_info;
    static uint8_t fresh_comm = 0;
    uint16_t xpos, ypos, touchSize;
    int i;

    *point_num = 0;
    point_data[1] = 0xff;

    if (point_num == NULL || touch_coord == NULL || max_num == 0) {
        return -1;
    }

    /* Get the first point */
    cst816d_i2c_read_byte(CST816D_REG_READ_COOR_ADDR, point_data, 7);
    touch_num = point_data[2];
    xpos = ((uint16_t)(point_data[3] & 0X0F)<<8) + (uint16_t)point_data[4];
    ypos = ((uint16_t)(point_data[5] & 0X0F)<<8) + (uint16_t)point_data[6];

    if (point_data[1] == 0x00) {  //point mode
        // printf("\n touch:(x=%d, y=%d), mode:%x, touch_type:%d\r\n", xpos, ypos, point_data[1], touch_type);
        touch_type = point_data[3] >> 4;    //status
        if (touch_type == 0x00) {

        }
        else if (touch_type == 0x04) {

        }
        touch_coord[0].coord_x = xpos;
        touch_coord[0].coord_y = ypos;
        *point_num = 1;
        return 0;
    }
    if (point_data[1]==0x0c)
    {
        //TP_type = TP_LONG_EVENT;
    }

    else if (point_data[1]==0x02)
    {
        //TP_type = TP_UP_EVENT;
    }
    else if (point_data[1]==0x01)
    {
        //TP_type = TP_DOWN_EVENT;

    }
    else if (point_data[1]==0x04)
    {
        //TP_type = TP_LEFT_EVENT;

    }
    else if (point_data[1]==0x03)
    {
        //TP_type = TP_RIGHT_EVENT;

    }
    else if (point_data[1]==0x0e)
    {
        //TP_type = KU_MODE;

    }
    else if (point_data[1]==0x0d)
    {
        //TP_type = KLU_MODE;

    }

    /* Get the second point */

    return -5;
}

#endif
