/**
 * @file CST816D_i2c.h
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
#ifndef _CST816D_I2C_H
#define _CST816D_I2C_H

#include "bflb_core.h"
#include "touch.h"


/**IIC ADDR**/
#define CST816D_IIC_ADDR 0x15

/**REG ADDR**/
// #define CST816D_REG_IC_YTPE_ADDR 	0xD0 //0xa0
#define CST816D_REG_VENDOR_ID_ADDR 	0xA7


#define CST816D_REG_READ_COOR_ADDR      0x00//0xD000//0x02


/**OTHER DEFINE**/
// #define TLSC6X_CNT_DOWN        1
// #define TLSC6X_CNT_LONG        20
// #define TLSC6X_CNT_HOLD        0

#define CST816D_MAX_TOUCH_POINT		1
// #define TLSC6X_DATA_CNT_FOR_POINT 	7//6

// #define TLSC6X_TOUCH_STATUS_PRESS 		6 //0
// #define TLSC6X_TOUCH_STATUS_RELEASE 	1
// #define TLSC6X_TOUCH_STATUS_CONTACT 	2
// #define TLSC6X_TOUCH_STATUS_NG 		3

typedef struct touch_point {
    uint8_t  e;
    uint32_t x;
    uint32_t y;
    uint32_t c;
} TOUCH_POINT;

typedef struct touch_panel_points {
    uint32_t point_num;
    TOUCH_POINT  p[5];
} TOUCH_PANEL_POINTS;
typedef struct sw_touch_panel_platform_data {
    uint8_t	 enable;
    char *iic_dev;
    uint32_t  rst_pin;
    uint32_t  int_pin;

    uint32_t  _MAX_POINT;
    uint32_t  _MAX_X;
    uint32_t  _MAX_Y;
    uint8_t 	 _INT_TRIGGER;			//0:int_raising 1:int_falling 2:int_down 3:int_up
    uint8_t 	 _X2Y_EN;
    uint8_t 	 _X_MIRRORING;
    uint8_t 	 _Y_MIRRORING;
    uint8_t 	 _DEBUGP;				//debug law points
    uint8_t 	 _DEBUGE;				//debug touch event

    TOUCH_PANEL_POINTS points;

} SW_TOUCH_PANEL_PLATFORM_DATA;

int cst816d_i2c_init(touch_coord_t *max_value);
int cst816d_get_gesture_id();
int cst816d_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num);

#endif /* __FT6X36_H */
