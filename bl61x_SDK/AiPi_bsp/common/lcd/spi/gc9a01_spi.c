/**
 * @file gc9a01_spi.c
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

#include "../lcd.h"

#if defined(LCD_SPI_GC9A01)

#if (LCD_SPI_INTERFACE_TYPE == 1)
#include "bl_spi_hard_4.h"

#define lcd_spi_init                          lcd_spi_hard_4_init
#define lcd_spi_isbusy                        lcd_spi_hard_4_is_busy

#define lcd_spi_transmit_cmd_para             lcd_spi_hard_4_transmit_cmd_para
#define lcd_spi_transmit_cmd_pixel_sync       lcd_spi_hard_4_transmit_cmd_pixel_sync
#define lcd_spi_transmit_cmd_pixel_fill_sync  lcd_spi_hard_4_transmit_cmd_pixel_fill_sync

#define lcd_spi_sync_callback_enable          lcd_spi_hard_4_async_callback_enable
#define lcd_spi_async_callback_register       lcd_spi_hard_4_async_callback_register
#define lcd_spi_transmit_cmd_pixel_async      lcd_spi_hard_4_transmit_cmd_pixel_async
#define lcd_spi_transmit_cmd_pixel_fill_async lcd_spi_hard_4_transmit_cmd_pixel_fill_async

static lcd_spi_hard_4_init_t spi_para = {
    .clock_freq = 40 * 1000 * 1000,
#if (GC9A01_SPI_PIXEL_FORMAT == 1)
    .pixel_format = LCD_SPI_LCD_PIXEL_FORMAT_RGB565,
#elif (GC9A01_SPI_PIXEL_FORMAT == 2)
    .pixel_format = LCD_SPI_LCD_PIXEL_FORMAT_NRGB8888,
#endif
};

#else

#error "Configuration error"

#endif
const gc9a01_spi_init_cmd_t gc9a01_spi_init_cmds[] = {

    { 0xFE, NULL, 0 },
    { 0xEF, NULL, 0 },
    { 0xEB, "\x14", 1 },
    { 0x84, "\x60", 1 },
    { 0x85, "\xFF", 1 },
    { 0x86, "\xFF", 1 },
    { 0x87, "\xFF", 1 },
    { 0x8E, "\xFF", 1 },
    { 0x8F, "\xFF", 1 },
    { 0x88, "\x0A", 1 },
    { 0x89, "\x23", 1 },
    { 0x8A, "\x00", 1 },
    { 0x8B, "\x80", 1 },
    { 0x8C, "\x01", 1 },
    { 0x8D, "\x03", 1 },
    { 0xB5, "\x08\x09\x14\x08", 4 },
    { 0xB6, "\x00\x00", 2 },
    { 0x36, "\x48", 1 },
    { 0x3A, "\x05", 1 },
    { 0x90, "\x08\x08\x08\x08", 4 },
    { 0xBD, "\x06", 1 },
    { 0xBC, "\x00", 1 },
    { 0xFF, "\x60\x01\x04", 3 },
    { 0xC3, "\x1D", 1 },
    { 0xC4, "\x1D", 1 },
    { 0xC9, "\x25", 1 },
    { 0xBE, "\x11", 1 },
    { 0xE1, "\x10\x0E", 2 },
    { 0xDF, "\x21\x0C\x02", 3 },
    { 0xF0, "\x45\x09\x08\x08\x26\x2A", 6 },
    { 0xF1, "\x43\x70\x72\x36\x37\x6F", 6 },
    { 0xF2, "\x45\x09\x08\x08\x26\x2A", 6 },
    { 0xF3, "\x43\x70\x72\x36\x37\x6F", 6 },
    { 0xED, "\x1B\x0B", 2 },
    { 0xAE, "\x77", 1 },
    { 0xCD, "\x63", 1 },
    { 0x70, "\x07\x07\x04\x0E\x10\x09\x07\x08\x03", 9 },
    { 0xE8, "\x44", 1 },//34x  14x

    { 0x60, "\x38\x0B\x6D\x6D\x39\xF0\x6D\x6D", 8 },
    { 0x61, "\x38\xF4\x6D\x6D\x38\xF7\x6D\x6D", 8 },
    { 0x62, "\x38\x0D\x71\xED\x70\x70\x38\x0F\x71\xEF\x70\x70", 12 },
    { 0x63, "\x38\x11\x71\xF1\x70\x70\x38\x13\x71\xF3\x70\x70", 12 },
    { 0x64, "\x28\x29\xF1\x01\xF1\x00\x07", 7 },
    { 0x66, "\x3C\x00\xCD\x67\x45\x45\x10\x00\x00\x00", 10 },
    { 0x67, "\x00\x3C\x00\x00\x00\x01\x54\x10\x32\x98", 10 },
    { 0x74, "\x10\x85\x80\x00\x00\x4E\x00", 7 },
    { 0x98, "\x3E\x07", 2 },
    { 0x99, "\x3E\x07", 2 },
    // { 0xC1, "\x02", 1 },
    { 0x35, "\x00", 1 },
    { 0x21, NULL, 0 },
    { 0xFF, NULL, 120 },

    { 0x11, NULL, 0 },
    { 0xFF, NULL, 120 },
    { 0x29, NULL, 0 },
    { 0xFF, NULL, 120 },
    { 0x2A, "\x00\x00\x00\xEF", 4 },
    { 0x2B, "\x00\x00\x00\xEF", 4 },
    { 0x2C, NULL, 0 },
};

/**
 * @brief gc9a01_spi_async_callback_enable
 *
 * @return
 */
void gc9a01_spi_async_callback_enable(bool enable)
{
    lcd_spi_sync_callback_enable(enable);
}

/**
 * @brief gc9a01_spi_async_callback_register
 *
 * @return
 */
void gc9a01_spi_async_callback_register(void (*callback)(void))
{
    lcd_spi_async_callback_register(callback);
}

/**
 * @brief gc9a01_spi_draw_is_busy, After the call gc9a01_spi_draw_picture_dma must check this,
 *         if gc9a01_spi_draw_is_busy() == 1, Don't allow other draw !!
 *         can run in the DMA interrupt callback function.
 *
 * @return int 0:draw end; 1:Being draw
 */
int gc9a01_spi_draw_is_busy(void)
{
    return lcd_spi_isbusy();
}

/**
 * @brief gc9a01_spi_init
 *
 * @return int
 */
int gc9a01_spi_init()
{
    lcd_spi_init(&spi_para);

    for (uint16_t i = 0; i < (sizeof(gc9a01_spi_init_cmds) / sizeof(gc9a01_spi_init_cmds[0])); i++) {
        if (gc9a01_spi_init_cmds[i].cmd == 0xFF && gc9a01_spi_init_cmds[i].data == NULL && gc9a01_spi_init_cmds[i].databytes) {
            bflb_mtimer_delay_ms(gc9a01_spi_init_cmds[i].databytes);
        }
        else {
            lcd_spi_transmit_cmd_para(gc9a01_spi_init_cmds[i].cmd, (void*)(gc9a01_spi_init_cmds[i].data), gc9a01_spi_init_cmds[i].databytes);
        }
    }

    return 0;
}

/**
 * @brief
 *
 * @param dir
 * @param mir_flag
 */
int gc9a01_spi_set_dir(uint8_t dir, uint8_t mir_flag)
{
    uint8_t param;

    switch (dir) {
        case 0:
            if (!mir_flag)
                param = 0x08;
            else
                param = 0x48;
            break;
        case 1:
            if (!mir_flag)
                param = 0x28;
            else
                param = 0xA8;
            break;
        case 2:
            if (!mir_flag)
                param = 0x88;
            else
                param = 0xC8;
            break;
        case 3:
            if (!mir_flag)
                param = 0xE8;
            else
                param = 0x68;

            break;
        default:
            return -1;
            break;
    }

    lcd_spi_transmit_cmd_para(0x36, (void*)&param, 1);

    return dir;
}

/**
 * @brief gc9a01_spi_set_draw_window
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void gc9a01_spi_set_draw_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if GC9A01_SPI_OFFSET_X
    x1 += GC9A01_SPI_OFFSET_X;
    x2 += GC9A01_SPI_OFFSET_X;
#endif
#if GC9A01_SPI_OFFSET_Y
    y1 += GC9A01_SPI_OFFSET_Y;
    y2 += GC9A01_SPI_OFFSET_Y;
#endif

    int8_t param[4];

    param[0] = (x1 >> 8) & 0xFF;
    param[1] = x1 & 0xFF;
    param[2] = (x2 >> 8) & 0xFF;
    param[3] = x2 & 0xFF;

    lcd_spi_transmit_cmd_para(0x2A, (void*)param, 4);

    param[0] = (y1 >> 8) & 0xFF;
    param[1] = y1 & 0xFF;
    param[2] = (y2 >> 8) & 0xFF;
    param[3] = y2 & 0xFF;

    lcd_spi_transmit_cmd_para(0x2B, (void*)param, 4);
}

/**
 * @brief gc9a01_spi_draw_point
 *
 * @param x
 * @param y
 * @param color
 */
void gc9a01_spi_draw_point(uint16_t x, uint16_t y, gc9a01_spi_color_t color)
{
    /* set window */
    gc9a01_spi_set_draw_window(x, y, x, y);

    lcd_spi_transmit_cmd_pixel_sync(0x2C, (void*)&color, 1);
}

/**
 * @brief gc9a01_draw_area
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void gc9a01_spi_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, gc9a01_spi_color_t color)
{
    uint32_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    gc9a01_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_fill_sync(0x2C, (uint32_t)color, pixel_cnt);
}

/**
 * @brief gc9a01_draw_picture_dma, Non-blocking! Using DMA acceleration, Not waiting for the draw end
 *  After the call, No other operations are allowed until (gc9a01_draw_is_busy()==0)
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void gc9a01_spi_draw_picture_nonblocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, gc9a01_spi_color_t* picture)
{
    size_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    gc9a01_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_async(0x2C, (void*)picture, pixel_cnt);
}

/**
 * @brief gc9a01_draw_picture,Blocking,Using DMA acceleration,Waiting for the draw end
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void gc9a01_spi_draw_picture_blocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, gc9a01_spi_color_t* picture)
{
    size_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    gc9a01_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_sync(0x2C, (void*)picture, pixel_cnt);
}

#endif
