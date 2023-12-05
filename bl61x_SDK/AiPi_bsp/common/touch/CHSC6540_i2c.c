/**
 * @file CHSC6540_i2c.c
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

#ifdef TOUCH_I2C_CHSC6540
#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_i2c.h"
#include "CHSC6540_i2c.h"

static struct bflb_device_s *touch_ft6x36_i2c = NULL;

typedef struct {
    uint8_t fresh;
    uint16_t x;
    uint16_t y;
    uint16_t size;
} POINT;

POINT point[TLSC6X_MAX_TOUCH_POINT];

static struct sw_touch_panel_platform_data *_touch_panel_data = NULL;
struct sw_touch_panel_platform_data touch_panel_data;

static void ft6x36_i2c_gpio_init(void)
{
    struct bflb_device_s *ft63x6_i2c_gpio = NULL;
    ft63x6_i2c_gpio = bflb_device_get_by_name("gpio");

    /* I2C0_SCL */
    bflb_gpio_init(ft63x6_i2c_gpio, TOUCH_I2C_SCL_PIN, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(ft63x6_i2c_gpio, TOUCH_I2C_SDA_PIN, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

static int ft6x36_i2c_peripheral_init(void)
{
    touch_ft6x36_i2c = bflb_device_get_by_name("i2c0");

    if (touch_ft6x36_i2c) {
        // printf("ft6x36 i2c gpio init\r\n");
        /* init i2c gpio */
        ft6x36_i2c_gpio_init();
        /* init i2c 200k */
        bflb_i2c_init(touch_ft6x36_i2c, 200000);
    } else {
        printf("i2c device get fail\r\n");
        return -1;
    }

    return 0;
}

static int ft6x36_i2c_read_byte(uint8_t register_addr, uint8_t *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = TLSC6X_IIC_ADDR;
    msg[0].flags = 0;
    msg[0].buffer = &register_addr;
    msg[0].length = 1;

    msg[1].addr = TLSC6X_IIC_ADDR;
    msg[1].flags = I2C_M_READ;
    msg[1].buffer = data_buf;
    msg[1].length = len;
    bflb_i2c_transfer(touch_ft6x36_i2c, msg, 2);

    return 0;
}

int chsc6540_get_gesture_id()
{
    uint8_t data_buf = 0;

    if (ft6x36_i2c_read_byte(TLSC6X_REG_VENDOR_ID_ADDR, &data_buf, 1)) {
        return -1;
    }

    return data_buf;
}

int chsc6540_i2c_init(touch_coord_t *max_value)
{
    uint8_t data_buf = 0;
    printf("ft6x36 i2c init\r\n");

    _touch_panel_data = &touch_panel_data;
    ft6x36_i2c_peripheral_init();

    if (ft6x36_i2c_read_byte(TLSC6X_REG_VENDOR_ID_ADDR, &data_buf, 1)) {
        return -1;
    }
    printf("Touch Device ID: 0x%02x\r\n", data_buf);

    if (ft6x36_i2c_read_byte(TLSC6X_REG_IC_YTPE_ADDR, &data_buf, 1)) {
        return -1;
    }
    printf("Touch Chip ID: 0x%02x\r\n", data_buf);


    return 0;
}

int chsc6540_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num)
{
    uint8_t point_data[TLSC6X_MAX_TOUCH_POINT * TLSC6X_DATA_CNT_FOR_POINT + 1] = {0};
    uint8_t touch_num = 0, tid, touch_info;
    static uint8_t fresh_comm = 0;
    uint16_t xpos, ypos, touchSize;
    int i;

    *point_num = 0;
    
    if (point_num == NULL || touch_coord == NULL || max_num == 0) {
        return -1;
    }

    /* Get the first point */
    ft6x36_i2c_read_byte(TLSC6X_REG_READ_COOR_ADDR, point_data, (TLSC6X_MAX_TOUCH_POINT * TLSC6X_DATA_CNT_FOR_POINT + 1));
    // for (i = 0; i < (TLSC6X_MAX_TOUCH_POINT * TLSC6X_DATA_CNT_FOR_POINT + 1); i++) {
    //     printf("TLSC6X TS 0x%02X : 0x%02X\n", (TLSC6X_REG_READ_COOR_ADDR + i), point_data[i]);
    // }
    if(0x10 != point_data[8]){
		//  printf("error touch size:%d \n",point_data[8]);
		  return -1;
	}
    touch_info = (point_data[2] & 0x0f);
    if(touch_info < 1 || touch_info > 2){
		return -2;
    }
    if (touch_info) {
        touch_num = touch_info;

        if (touch_num > max_num) {
           
            touch_num = max_num;
        }

        fresh_comm++;
         /* x =(point_data[1]<<8)|((point_data[3]>>4)&0x0f) 
            y =(point_data[2]<<8)|(point_data[3]&0x0f)*/
        for (i = 0; i < touch_num; i++) {
           // coor_data = &point_data[i * TLSC6X_DATA_CNT_FOR_POINT + 1];

         //   touch_status = point_data[i] & 0x0f;// touch_status = coor_data[0] >> 6;

			tid = i;//(point_data[i]&0xf0) >> 8; //触点id号
           // tid = coor_data[2] >> 4;

		    
			xpos = point_data[4]; //x的低8bit
			xpos = xpos | ((point_data[3] & 0x0f)<<8);
       
            ypos = point_data[6]; //y的低8bit
			ypos = ypos | ((point_data[5] & 0x0f)<<8);
			
            touchSize = point_data[8] ;//touchSize = coor_data[4];

            // if (_touch_panel_data->_DEBUGP){
            //     printf("touch id %d\n", tid);
            // // printf("touch status 0x%02X\n", touch_status);
			// }
            // if (tid > _touch_panel_data->_MAX_POINT - 1) {
            //     break;
            // }

          

            point[tid].fresh = fresh_comm;
            // point[tid].y = ypos;
            // point[tid].x = xpos;
            point[tid].size = touchSize;

            if (_touch_panel_data->_X2Y_EN) {
                point[tid].x = ypos;
                point[tid].y = xpos;
            } else {
                point[tid].y = ypos;
                point[tid].x = xpos;
            }

            // if (_touch_panel_data->_X_MIRRORING) {
            //     point[tid].x = _touch_panel_data->_MAX_X - 1 - point[tid].x;
            // }

            // if (_touch_panel_data->_Y_MIRRORING) {
            //     point[tid].y = _touch_panel_data->_MAX_Y - 1 - point[tid].y;
            // } else {

            // }

            // if (_touch_panel_data->_DEBUGP) {
            //     printf("\n touch_panel:(%d)(x=%d, y=%d)[%d]\n", tid, point[tid].x, point[tid].y, point[tid].size);
            // }

            touch_coord[i].coord_x = point[tid].x;
            touch_coord[i].coord_y = point[tid].y;
            *point_num += 1;
            
        }
        return 0;
    }

    /* Get the second point */
    // if (data_buf[0] > 1 && max_num > 1) {
    //     ft6x36_i2c_read_byte(FT6X36_P2_XH_REG, &data_buf[1], 4);
    //     touch_coord[1].coord_x = (data_buf[1] & FT6X36_MSB_MASK) << 8 | (data_buf[2] & FT6X36_LSB_MASK);
    //     touch_coord[1].coord_y = (data_buf[3] & FT6X36_MSB_MASK) << 8 | (data_buf[4] & FT6X36_LSB_MASK);
    //     *point_num = 2;
    // }
    return -5;
}

#endif
