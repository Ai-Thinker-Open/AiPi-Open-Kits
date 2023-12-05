/**
 * @file ue043hv_spi.c
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

#if defined(LCD_SPI_UE043HV)

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
#if (UE043HV_SPI_PIXEL_FORMAT == 1)
    .pixel_format = LCD_SPI_LCD_PIXEL_FORMAT_RGB565,
#elif (UE043HV_SPI_PIXEL_FORMAT == 2)
    .pixel_format = LCD_SPI_LCD_PIXEL_FORMAT_NRGB8888,
#endif
};

#else

#error "Configuration error"

#endif
const ue043hv_spi_init_cmd_t ue043hv_spi_init_cmds[] = {
    //---------Start Initial Code ------//
    {0xff,
    "\xa5",1},
    {0xE7, //TE_output_en
    "\x10",1},
    {0x35, //TE_ interface_en
    "\x00",1},//01
    {0x36,
    "\x00",1},//c0
    {0x3A,
    "\x01",1},//01---565/00---666
    {0x40,
    "\x01",1},//01:IPS/00:TN
    {0x41,
    "\x03",1},//01--8bit//03--16bit
    {0x44, //VBP 
    "\x15",1},

    {0x45, //VFP 
    "\x15",1},
    {0x7d,//vdds_trim[2:0]
    "\x03",1},

    {0xc1,//avdd_clp_en avdd_clp[1:0] avcl_clp_en avcl_clp[1:0]
    "\xbb",1},//0xbb 88 a2
    {0xc2,//vgl_clp_en vgl_clp[2:0]
    "\x05",1},
    {0xc3,//vgl_clp_en vgl_clp[2:0]
    "\x10",1},
    {0xc6,//avdd_ratio_sel avcl_ratio_sel vgh_ratio_sel[1:0] vgl_ratio_sel[1:0]
    "\x3e",1},
    {0xc7,//mv_clk_sel[1:0] avdd_clk_sel[1:0] avcl_clk_sel[1:0]
    "\x25",1},
    {0xc8,// VGL_CLK_sel
    "\x21",1},
    {0x7a,// user_vgsp
    "\x51",1}, //58
    {0x6f,// user_gvdd
    "\x49",1}, //4F 
    {0x78,// user_gvcl
    "\x57",1}, //70 
    {0xc9,
    "\x00",1},
    {0x67,
    "\x11",1},
    //gate_ed
    {0x51,//gate_st_o[7:0]
    "\x0a",1},
    {0x52,//gate_ed_o[7:0]
    "\x7D",1}, //7A
    {0x53,//gate_st_e[7:0]
    "\x0a",1},
    {0x54,//gate_ed_e[7:0]
    "\x7D",1}, //7A
    //sorce 
    {0x46,//fsm_hbp_o[5:0]
    "\x0a",1},
    //NVu NV3041A
    {0x47,//fsm_hfp_o[5:0]
    "\x2a",1},
    {0x48,//fsm_hbp_e[5:0]
    "\x0a",1},
    {0x49,//fsm_hfp_e[5:0]
    "\x1a",1},
    {0x44, //VBP 
    "\x15",1},
    {0x45, //VFP 
    "\x15",1},
    {0x73,
    "\x08",1},
    {0x74,
    "\x10",1}, //0A

    ///test mode
    //{0xf8, 
    //"\x16",1},
    //{0xf9, 
    //"\x20",1},

    {0x56,//src_ld_wd[1:0] src_ld_st[5:0]
    "\x43",1},
    {0x57,//pn_cs_en src_cs_st[5:0]
    "\x42",1},
    {0x58,//src_cs_p_wd[6:0]
    "\x3c",1},
    {0x59,//src_cs_n_wd[6:0]
    "\x64",1},
    {0x5a,//src_pchg_st_o[6:0]
    "\x41",1},
    {0x5b,//src_pchg_wd_o[6:0]
    "\x3C",1},
    {0x5c,//src_pchg_st_e[6:0]
    "\x02",1},
    {0x5d,//src_pchg_wd_e[6:0]
    "\x3c",1},
    {0x5e,//src_pol_sw[7:0]
    "\x1f",1},
    {0x60,//src_op_st_o[7:0]
    "\x80",1},

    {0x61,//src_op_st_e[7:0]
    "\x3f",1},
    {0x62,//src_op_ed_o[9:8] src_op_ed_e[9:8]
    "\x21",1},
    {0x63,//src_op_ed_o[7:0]
    "\x07",1},
    {0x64,//src_op_ed_e[7:0]
    "\xe0",1},
    {0x65,//chopper
    "\x02",1},
    {0xca, //avdd_mux_st_o[7:0]
    "\x20",1},
    {0xcb, //avdd_mux_ed_o[7:0]
    "\x52",1},
    {0xcc, //avdd_mux_st_e[7:0]
    "\x10",1},
    {0xcD, //avdd_mux_ed_e[7:0]
    "\x42",1},
    {0xD0, //avcl_mux_st_o[7:0]
    "\x20",1},
    {0xD1, //avcl_mux_ed_o[7:0]
    "\x52",1},
    {0xD2, //avcl_mux_st_e[7:0]
    "\x10",1},
    {0xD3, //avcl_mux_ed_e[7:0]
    "\x42",1},
    {0xD4, //vgh_mux_st[7:0]
    "\x0a",1},
    {0xD5, //vgh_mux_ed[7:0]
    "\x32",1},
    //gammma boe4.3 
    {0x80, //gam_vrp0
    "\x00",1},
    {0xA0, //gam_VRN0
    "\x00",1},
    {0x81, //gam_vrp1
    "\x06",1},//07
    {0xA1, //gam_VRN1
    "\x08",1},//06

    {0x82, //gam_vrp2
    "\x03",1},//02
    {0xA2, //gam_VRN2
    "\x03",1},//01
    {0x86, //gam_prp0
    "\x14",1}, //11
    {0xA6, //gam_PRN0
    "\x14",1}, //10
    {0x87, //gam_prp1
    "\x2C",1}, //27
    {0xA7, //gam_PRN1
    "\x26",1}, //27
    {0x83, //gam_vrp3
    "\x37",1},
    {0xA3, //gam_VRN3
    "\x37",1},
    {0x84, //gam_vrp4 
    "\x35",1},
    {0xA4, //gam_VRN4
    "\x35",1},
    {0x85, //gam_vrp5 
    "\x3f",1},
    {0xA5, //gam_VRN5
    "\x3f",1},
    {0x88, //gam_pkp0 
    "\x0A",1}, //0b
    {0xA8, //gam_PKN0
    "\x0A",1}, //0b
    {0x89, //gam_pkp1 
    "\x13",1}, //14
    {0xA9, //gam_PKN1
    "\x12",1}, //13
    {0x8a, //gam_pkp2 
    "\x18",1}, //1a
    {0xAa, //gam_PKN2
    "\x19",1}, //1a
    {0x8b, //gam_PKP3 
    "\x0a",1},
    {0xAb, //gam_PKN3
    "\x0a",1},
    {0x8c, //gam_PKP4 
    "\x17",1},//14
    {0xAc, //gam_PKN4
    "\x0B",1},//08
    {0x8d, //gam_PKP5
    "\x1A",1},//17

    {0xAd, //gam_PKN5
    "\x09",1},//07
    {0x8e, //gam_PKP6
    "\x1A",1}, //16 //16
    {0xAe, //gam_PKN6
    "\x08",1}, //06 //13
    {0x8f, //gam_PKP7
    "\x1F",1}, //1B
    {0xAf, //gam_PKN7
    "\x00",1}, //07
    {0x90, //gam_PKP8 
    "\x08",1},//04
    {0xB0, //gam_PKN8
    "\x00",1},//04
    {0x91, //gam_PKP9
    "\x10",1},//0A
    {0xB1, //gam_PKN9
    "\x06",1},//0A
    {0x92, //gam_PKP10
    "\x19",1},//16
    {0xB2, //gam_PKN10
    "\x15",1},//15
    {0xff,
    "\x00",1},
    { 0x11, NULL, 0 },
    { 0xFF, NULL, 120 },
    { 0x29, NULL, 0 },
    { 0xFF, NULL, 20 },

};

/**
 * @brief ue043hv_spi_async_callback_enable
 *
 * @return
 */
void ue043hv_spi_async_callback_enable(bool enable)
{
    lcd_spi_sync_callback_enable(enable);
}

/**
 * @brief ue043hv_spi_async_callback_register
 *
 * @return
 */
void ue043hv_spi_async_callback_register(void (*callback)(void))
{
    lcd_spi_async_callback_register(callback);
}

/**
 * @brief ue043hv_spi_draw_is_busy, After the call ue043hv_spi_draw_picture_dma must check this,
 *         if ue043hv_spi_draw_is_busy() == 1, Don't allow other draw !!
 *         can run in the DMA interrupt callback function.
 *
 * @return int 0:draw end; 1:Being draw
 */
int ue043hv_spi_draw_is_busy(void)
{
    return lcd_spi_isbusy();
}

/**
 * @brief ue043hv_spi_init
 *
 * @return int
 */
int ue043hv_spi_init()
{
    lcd_spi_init(&spi_para);

    for (uint16_t i = 0; i < (sizeof(ue043hv_spi_init_cmds) / sizeof(ue043hv_spi_init_cmds[0])); i++) {
        if (ue043hv_spi_init_cmds[i].cmd == 0xFF && ue043hv_spi_init_cmds[i].data == NULL && ue043hv_spi_init_cmds[i].databytes) {
            bflb_mtimer_delay_ms(ue043hv_spi_init_cmds[i].databytes);
        }
        else {
            lcd_spi_transmit_cmd_para(ue043hv_spi_init_cmds[i].cmd, (void*)(ue043hv_spi_init_cmds[i].data), ue043hv_spi_init_cmds[i].databytes);
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
int ue043hv_spi_set_dir(uint8_t dir, uint8_t mir_flag)
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
 * @brief ue043hv_spi_set_draw_window
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void ue043hv_spi_set_draw_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if UE043HV_SPI_OFFSET_X
    x1 += UE043HV_SPI_OFFSET_X;
    x2 += UE043HV_SPI_OFFSET_X;
#endif
#if UE043HV_SPI_OFFSET_Y
    y1 += UE043HV_SPI_OFFSET_Y;
    y2 += UE043HV_SPI_OFFSET_Y;
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
 * @brief ue043hv_spi_draw_point
 *
 * @param x
 * @param y
 * @param color
 */
void ue043hv_spi_draw_point(uint16_t x, uint16_t y, ue043hv_spi_color_t color)
{
    /* set window */
    ue043hv_spi_set_draw_window(x, y, x, y);

    lcd_spi_transmit_cmd_pixel_sync(0x2C, (void*)&color, 1);
}

/**
 * @brief ue043hv_draw_area
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void ue043hv_spi_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ue043hv_spi_color_t color)
{
    uint32_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    ue043hv_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_fill_sync(0x2C, (uint32_t)color, pixel_cnt);
}

/**
 * @brief ue043hv_draw_picture_dma, Non-blocking! Using DMA acceleration, Not waiting for the draw end
 *  After the call, No other operations are allowed until (ue043hv_draw_is_busy()==0)
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void ue043hv_spi_draw_picture_nonblocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ue043hv_spi_color_t* picture)
{
    size_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    ue043hv_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_async(0x2C, (void*)picture, pixel_cnt);
}

/**
 * @brief ue043hv_draw_picture,Blocking,Using DMA acceleration,Waiting for the draw end
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void ue043hv_spi_draw_picture_blocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ue043hv_spi_color_t* picture)
{
    size_t pixel_cnt = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    ue043hv_spi_set_draw_window(x1, y1, x2, y2);

    lcd_spi_transmit_cmd_pixel_sync(0x2C, (void*)picture, pixel_cnt);
}

#endif
