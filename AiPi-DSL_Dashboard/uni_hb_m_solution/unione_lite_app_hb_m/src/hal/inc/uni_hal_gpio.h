#ifndef __GPIO_HAL_H__
#define __GPIO_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"

#define     RESTORE_TO_GENERAL_IO     			   			(0xFF)  /**<this mcaro will be used as a para value to restore the multiplex io to general io*/

/**
 * RegIndex for Bank A
 */
#define		GPIO_A_START						(0x00)

#define		GPIO_A_IN							(0x00 + GPIO_A_START)	/**<GPIOA input data register macro*/
#define		GPIO_A_OUT							(0x01 + GPIO_A_START)	/**<GPIOA output data register macro*/
#define     GPIO_A_SET							(0x02 + GPIO_A_START)
#define     GPIO_A_CLR							(0x03 + GPIO_A_START)
#define     GPIO_A_TGL							(0x04 + GPIO_A_START)
#define		GPIO_A_IE							(0x05 + GPIO_A_START)	/**<GPIOA input enable register macro.0: disable, 1: enable*/
#define		GPIO_A_OE							(0x06 + GPIO_A_START)	/**<GPIOA output enable register macro.0: disable, 1: enable*/
#define		GPIO_A_DS							(0x07 + GPIO_A_START)	/**<GPIOA pull-up/pull-down drive strength(when GPIOA input or output) register macro. 0: weak pull(20uA), 1: strong pull(70uA)*/
#define		GPIO_A_PU							(0x08 + GPIO_A_START)	/**<GPIOA pull up register macro.{pu} 1'b1: pull-up,  1'b0: no pull-up*/
#define		GPIO_A_PD							(0x09 + GPIO_A_START)	/**<GPIOA pull down register macro.{pd} 1'b1: pull-down, 1'b0: no pull-down*/
#define		GPIO_A_ANA_EN						(0x0a + GPIO_A_START)
#define		GPIO_A_PULLDOWN						(0x0b + GPIO_A_START)	/**<GPIOA pull down current register macro.0: 0mA pull-down current, 1: 2.4mA pull-down current*/
#define		GPIO_A_CORE_OUT_MASK				(0x0c + GPIO_A_START)
#define		GPIO_A_DMA_OUT_MASK					(0x0d + GPIO_A_START)

#define		GPIO_A_END							GPIO_A_DMA_OUT_MASK

/**
 * RegIndex for Bank B
 */
#define		GPIO_B_START						(GPIO_A_END + 1)

#define		GPIO_B_IN							(0x00 + GPIO_B_START)	/**<GPIOB input data register macro*/
#define		GPIO_B_OUT							(0x01 + GPIO_B_START)	/**<GPIOB output data register macro*/
#define     GPIO_B_SET							(0x02 + GPIO_B_START)
#define     GPIO_B_CLR							(0x03 + GPIO_B_START)
#define     GPIO_B_TGL							(0x04 + GPIO_B_START)
#define		GPIO_B_IE							(0x05 + GPIO_B_START)	/**<GPIOB input enable register macro.0: disable, 1: enable*/
#define		GPIO_B_OE							(0x06 + GPIO_B_START)	/**<GPIOB output enable register macro.0: disable, 1: enable*/
#define		GPIO_B_DS							(0x07 + GPIO_B_START)	/**<GPIOB pull-up/pull-down drive strength(when GPIOA input or output) register macro. 0: weak pull(20uA), 1: strong pull(70uA)*/
#define		GPIO_B_PU							(0x08 + GPIO_B_START)	/**<GPIOB pull up register macro.{pu} 1'b1: pull-up,  1'b0: no pull-up*/
#define		GPIO_B_PD							(0x09 + GPIO_B_START)	/**<GPIOB pull down register macro.{pd} 1'b1: pull-down, 1'b0: no pull-down*/
#define		GPIO_B_ANA_EN						(0x0a + GPIO_B_START)
#define		GPIO_B_PULLDOWN						(0x0b + GPIO_B_START)	/**<GPIOB pull down current register macro.0: 0mA pull-down current, 1: 2.4mA pull-down current*/
#define		GPIO_B_CORE_OUT_MASK				(0x0c + GPIO_B_START)
#define		GPIO_B_DMA_OUT_MASK					(0x0d + GPIO_B_START)

#define		GPIO_B_END							GPIO_B_DMA_OUT_MASK

/**
 * RegIndex for others
 */
#define		GPIO_MISC_START						(GPIO_B_END + 1)

//#define		GPIO_OUT_MASK						(0x00 + GPIO_MISC_START)
#define		GPIO_B8_ANA_EN						(0x00 + GPIO_MISC_START)
#define		GPIO_A18_OUTDS						(0x01 + GPIO_MISC_START) /**<RegIndex for GPIO A18 output driving strength */

#define		GPIO_MISC_END						GPIO_A18_OUTDS

#define		GPIO_GLB_INTE						(GPIO_MISC_END + 7)
/**
 * RegIndex for separate interrupt of GPIO Bank A
 */
#define		GPIO_A_INT_START					(GPIO_MISC_END + 8)

#define		GPIO_A_SEP_INTE						(0x00 + GPIO_A_INT_START)/**<GPIOA interrupt inte register macro, using it with functions(GPIO_INTEnable, GPIO_INTDisable, GPIO_ClearINTFlag, GPIO_GetINTFlag)as the first para*/
#define		GPIO_A_SEP_NTRIG					(0x01 + GPIO_A_INT_START)
#define		GPIO_A_SEP_PTRIG					(0x02 + GPIO_A_INT_START)
#define		GPIO_A_SEP_INTC						(0x03 + GPIO_A_INT_START)
#define		GPIO_A_SEP_INTS						(0x04 + GPIO_A_INT_START)

#define		GPIO_A_INT_END						GPIO_A_SEP_INTS

/**
 * RegIndex for separate interrupt of GPIO Bank B
 */
#define		GPIO_B_INT_START					(GPIO_A_INT_END + 1)

#define		GPIO_B_SEP_INTE						(0x00 + GPIO_B_INT_START)	/**<GPIOB interrupt inte register macro, using it with functions(GPIO_INTEnable, GPIO_INTDisable, GPIO_ClearINTFlag, GPIO_GetINTFlag)as the first para*/
#define		GPIO_B_SEP_NTRIG					(0x01 + GPIO_B_INT_START)
#define		GPIO_B_SEP_PTRIG					(0x02 + GPIO_B_INT_START)
#define		GPIO_B_SEP_INTC						(0x03 + GPIO_B_INT_START)
#define		GPIO_B_SEP_INTS						(0x04 + GPIO_B_INT_START)

#define		GPIO_B_INT_END						GPIO_B_SEP_INTS


#define	GPIO_A_INT	GPIO_A_SEP_INTE
#define	GPIO_B_INT	GPIO_B_SEP_INTE

/**
 * Define GPIO interrupt trigger type
 */
typedef 	uint8_t								GpioTriggerType;

#define     GPIO_NEG_EDGE_TRIGGER				(0)	/**<GPIO interrupt mode:negedge trigger, you can use it with function GPIO_INTEnable as the third para*/
#define     GPIO_POS_EDGE_TRIGGER				(1)	/**<GPIO interrupt mode:posedge trigger, you can use it with function GPIO_INTEnable as the third para*/
#define     GPIO_EDGE_TRIGGER                   (2)/**<GPIO interrupt mode:edge trigger, you can use it with function GPIO_INTEnable as the third para*/


/**
 * Define GPIO output Driving Strength
 */
typedef		uint8_t								GpioOutDsType;

#define		GPIO_OUT_DS_8mA						(0)
#define		GPIO_OUT_DS_24mA					(1)

typedef		uint8_t								GpioOutMask;

#define		CORE_MASK							(1 << 0)
#define		DMA_MASK							(1 << 1)

#define		GPIO_A_OUT_MASK						GPIO_A_CORE_OUT_MASK
#define		GPIO_B_OUT_MASK						GPIO_B_CORE_OUT_MASK

#define     GPIO_INDEX0          				(1 << 0)	/**<GPIO bit0 macro*/
#define     GPIO_INDEX1          				(1 << 1)	/**<GPIO bit1 macro*/
#define     GPIO_INDEX2          				(1 << 2)	/**<GPIO bit2 macro*/
#define     GPIO_INDEX3          				(1 << 3)	/**<GPIO bit3 macro*/
#define     GPIO_INDEX4          				(1 << 4)	/**<GPIO bit4 macro*/
#define     GPIO_INDEX5         	 			(1 << 5)	/**<GPIO bit5 macro*/
#define     GPIO_INDEX6          				(1 << 6)	/**<GPIO bit6 macro*/
#define     GPIO_INDEX7          				(1 << 7)	/**<GPIO bit7 macro*/
#define     GPIO_INDEX8          				(1 << 8)	/**<GPIO bit8 macro*/
#define     GPIO_INDEX9          				(1 << 9)	/**<GPIO bit9 macro*/
#define     GPIO_INDEX10         				(1 << 10)	/**<GPIO bit10 macro*/
#define     GPIO_INDEX11         				(1 << 11)	/**<GPIO bit11 macro*/
#define     GPIO_INDEX12         				(1 << 12)	/**<GPIO bit12 macro*/
#define     GPIO_INDEX13         				(1 << 13)	/**<GPIO bit13 macro*/
#define     GPIO_INDEX14         				(1 << 14)	/**<GPIO bit14 macro*/
#define     GPIO_INDEX15         				(1 << 15)	/**<GPIO bit15 macro*/
#define     GPIO_INDEX16         				(1 << 16)	/**<GPIO bit16 macro*/
#define     GPIO_INDEX17         				(1 << 17)	/**<GPIO bit17 macro*/
#define     GPIO_INDEX18         				(1 << 18)	/**<GPIO bit18 macro*/
#define     GPIO_INDEX19         				(1 << 19)	/**<GPIO bit19 macro*/
#define     GPIO_INDEX20         				(1 << 20)	/**<GPIO bit20 macro*/
#define     GPIO_INDEX21         				(1 << 21)	/**<GPIO bit21 macro*/
#define     GPIO_INDEX22         				(1 << 22)	/**<GPIO bit22 macro*/
#define     GPIO_INDEX23         				(1 << 23)	/**<GPIO bit23 macro*/
#define     GPIO_INDEX24         				(1 << 24)	/**<GPIO bit24 macro*/
#define     GPIO_INDEX25         				(1 << 25)	/**<GPIO bit25 macro*/
#define     GPIO_INDEX26         				(1 << 26)	/**<GPIO bit26 macro*/
#define     GPIO_INDEX27         				(1 << 27)	/**<GPIO bit27 macro*/
#define     GPIO_INDEX28         				(1 << 28)	/**<GPIO bit28 macro*/
#define     GPIO_INDEX29        				(1 << 29)	/**<GPIO bit29 macro*/
#define     GPIO_INDEX30        				(1 << 30)	/**<GPIO bit30 macro*/
#define     GPIO_INDEX31         				(1 << 31)	/**<GPIO bit31 macro*/


typedef enum
{
	GPIOA0 = (1 << 0),	/** GpioMode
						 * 			1:i2s0_mclk_in_0(i)
						 * 			2:i2s0_mclk_out_0(o)
						 * 			3:osc32k_out_0(o)
						 */

	GPIOA1 = (1 << 1),	/** GpioMode
						 *			1:spis_cs_0(i)
						 *			2:i2s0_lrck_0(i)/i2s1_lrck_1(o)
						 *			3:i2s0_lrck_0(o)/i2s1_lrck_1(i)
						 *			4:i2s0_lrck_0(io)
						 *			5:i2s1_lrck_1(io)
						 */

	GPIOA2 = (1 << 2),	/** GpioMode
						 *			1:sd0_dat_0(io)
						 *			2:i2s0_bclk_0(i)/i2s1_bclk_1(o)
						 *			3:i2s0_bclk_0(o)/i2s1_bclk_1(i)
						 *			4:i2s0_bclk_0(io)
						 *			5:i2s1_bclk_1(io)
						 *			6:spis_miso_0(o)
						 */

	GPIOA3 = (1 << 3),	/** GpioMode
						 *			1:spis_clk_0(i)
						 *			2:sd0_clk_0(o)
						 *			3:i2s0_do_0(o)
						 */

	GPIOA4 = (1 << 4),	/** GpioMode
						 *			1:spis_mosi_0(i)
						 *			2:sd0_cmd_0(io)
						 *			3:i2s0_din_0(i)
						 */

	GPIOA5 = (1 << 5),	/** GpioMode
						 *			1:spim_miso_0(i)
						 *			2:sd1_dat(io)
						 */

	GPIOA6 = (1 << 6),	/** GpioMode
						 *			1:spim_clk_0(o)
						 *			2:sd1_clk(o)
						 */

	GPIOA7 = (1 << 7),	/** GpioMode
						 *			1:sd1_cmd(io)
						 *			2:spim_mosi_0(o)
						 */

	GPIOA8 = (1 << 8),	/** GpioMode
						 *			1:i2s1_mclk_in_0(i)
						 *			2:i2s1_mclk_out_0(o)
						 */

	GPIOA9 = (1 << 9),	/** GpioMode
						 *			1:i2s1_lrck_0(io)
						 *			2:mdac_test[0](io)
						 *			3:sadc_test[0](o)
						 *			4:mcu_deepsleep(o)
						 *			5:pll_div_clk(o)
						 */

	GPIOA10 = (1 << 10),/** GpioMode
						 *			1:sd0_dat_1(io)
						 *			2:i2s1_bclk_0(io)
						 *			3:mdac_test[1](io)
						 *			4:sadc_test[1](o)
						 *			5:clk_out(o)
						 *			6:rc_clk(o)
						 */

	GPIOA11 = (1 << 11),/** GpioMode
						 *			1:mdac_test[2](io)
						 *			2:sd0_clk_1(o)
						 *			3:i2s1_do_0(o)
						 *			4:sadc_test[2](o)
						 *			5:bist_end(o)
						 */

	GPIOA12 = (1 << 12),/** GpioMode
						 *			1:sd0_cmd_1(io)
						 *			2:i2s1_din_0(i)
						 *			3:mdac_test[3](io)
						 *			4:sadc_test[3](o)
						 */

	GPIOA13 = (1 << 13),/** GpioMode
						 *			1:mdac_test[4](io)
						 *			2:uart0_txd(o)
						 *			3:sadc_test[4](o)
						 */

	GPIOA14 = (1 << 14),/** GpioMode
						 *			1:uart0_rxd(i)
						 *			2:mdac_test[5](io)
						 *			3:sadc_test[5](o)
						 */

	GPIOA15 = (1 << 15),/** GpioMode
						 *			1:mdac_test[6](io)
						 *			2:uart0_rts(o)
						 */

	GPIOA16 = (1 << 16),/** GpioMode
						 *			1:uart0_cts(i)
						 *			2:mdac_test[7](io)
						 *			3:osc32k_out_1(o)
						 *			4:hosc_out(o)
						 */

	GPIOA17 = (1 << 17),/** GpioMode
						 *			1:psram_sio3(io)
						 *			2:fshc_hold_0(io)
						 *			3:AD0/MICDET
						 */

	GPIOA18 = (1 << 18),/** GpioMode
						 *			1:psram_clk(o)
						 *			2:fshc_sck_0(o)
						 *			3:AD1
						 */

	GPIOA19 = (1 << 19),/** GpioMode
						 *			1:psram_sio0(io)
						 *			2:fshc_mosi_0(io)
						 */

	GPIOA20 = (1 << 20),/** GpioMode
						 *			1:psram_sio2(io)
						 *			2:fshc_wp_0(io)
						 */

	GPIOA21 = (1 << 21),/** GpioMode
						 *			1:psram_sio1(io)
						 *			2:fshc_miso_0(io)
						 *			3:MIC_DET_1(i)
						 */

	GPIOA22 = (1 << 22),/** GpioMode
						 *			1:psram_ce(o)
						 *			2:fshc_cs_0(o)
						 */

	GPIOA23 = (1 << 23),/** GpioMode
						 *			1:tim3_pwc_i_0(i)
						 *			2:mdac_test[8](io)
						 *			3:asdm_test[0](io)
						 *			4:phy_vpo(i)
						 *			5:tim3_pwm_0(o)
						 *			6:sadc_test[6](o)
						 *			7:AD2
						 */

	GPIOA24 = (1 << 24),/** GpioMode
						 *			1:i2s0_mclk_in_1(i)
						 *			2:tim4_pwc_i_0(i)
						 *			3:mdac_test[9](io)
						 *			4:asdm_test[1](io)
						 *			5:phy_vmo(i)
						 *			6:i2s0_mclk_out_1(o)
						 *		    7:tim4_pwm_0(o)
						 *		    8:sadc_test[7](o)
						 *		    9:AD3
						 */

	GPIOA25 = (1 << 25),/** GpioMode
						 *			1:spim_miso_1(i)
						 *			2:i2s0_lrck_1(i)/i2s1_lrck_2(o)
						 *			3:i2s0_lrck_1(o)/i2s1_lrck_2(i)
						 *			4:i2s0_lrck_1(io)
						 *			5:i2s1_lrck_2(io)
						 *			6:dmic0_dat(i)
						 *			7:mdac_test[10](io)
						 *			8:asdm_test[2](io)
						 *			9:phy_oen(i)
						 *			10:spis_miso_1(o)
						 *			11:sadc_test[8](o)
						 *			12:SPDIF_AI_0/AD4
						 */

	GPIOA26 = (1 << 26),/** GpioMode
						 *			1:spis_clk_1(i)
						 *			2:i2s0_bclk_1(i)/i2s1_bclk_2(o)
						 *			3:i2s0_bclk_1(o)/i2s1_bclk_2(i)
						 *			4:i2s0_bclk_1(io)
						 *			5:i2s1_bclk_2(io)
						 *			6:mdac_test[11](io)
						 *			7:asdm_test[3](io)
						 *			8:spim_clk_1(o)
						 *			9:dmic0_clk(o)
						 *			10:sadc_test[9](o)
						 *			11:phy_vp(o)
						 *			12:SPDIF_AI_2/AD5
						 */

	GPIOA27 = (1 << 27),/** GpioMode
						 *			1:spis_mosi_1(i)
						 *			2:tim3_pwc_i_1(i)
						 *			11:mdac_test[12](io)
						 *			4:asdm_test[4](io)
						 *			5:spim_mosi_1(o)
						 *			6:i2s0_do_1(o)
						 *			7:dmic1_clk(o)
						 *			8:spdif_do(o)
						 *			9:tim3_pwm_1(o)
						 *			10:sadc_test[10](o)
						 *			11:phy_vm(o)
						 *			12:SPDIF_AI_3/AD6
						 */

	GPIOA28 = (1 << 28),/** GpioMode
						 *			1:spis_cs_1(i)
						 *			2:i2s0_din_1(i)
						 *			3:dmic1_dat(i)
						 *			4:spdif_di(i)
						 *			5:tim4_pwc_i_1(i)
						 *			6:mdac_test[13](io)
						 *			7:asdm_test[5](io)
						 *			8:tim4_pwm_1(o)
						 *			9:sadc_test[11](o)
						 *			10:phy_rcv(o)
						 *			11:SPDIF_AI_1/AD7
						 */

	GPIOA29 = (1 << 29),/** GpioMode
						 *			1:spim_miso_2(i)
						 *			2:mdac_test[14](io)
						 *			3:asdm_test[6](io)
						 *			4:phy_suspend(i)
						 *			5:sadc_test[12](o)
						 *			6:AD8
						 */

	GPIOA30 = (1 << 30),/** GpioMode
						 *			1:uart1_cts_0(i)
						 *			2:mdac_test[15](io)
						 *			3:asdm_test[7](io)
						 *			4:sadc_clk(io)
						 *			5:spim_clk_2(o)
						 *			6:uart7816_clk(o)
						 *			7:AD9
						 */

	GPIOA31 = (1 << 31),/** GpioMode
						 *			1:uart7816_dat(io)
						 *			2:mdac_test[16](io)
						 *			3:asdm_test[8](io)
						 *			4:sadc_soc(io)
						 *			5:spim_mosi_2(o)
						 *			6:uart1_rts_0(o)
						 *			7:AD10
						 */
}GPIO_PortA;

typedef enum
{
	GPIOB0 = (1 << 0),	/** GpioMode
						 *			1:i2c_scl_0(io)
						 *			2:tim3_pwc_i_2(i)
						 *			3:sw_clk(i)
						 *			4:uart1_txd_1(o)
						 *			5:tim3_pwm_2(o)
						 */

	GPIOB1 = (1 << 1),	/** GpioMode
						 *			1:uart1_rxd_1(i)
						 *			2:i2c_sda_0(io)
						 *			3:tim4_pwc_i_2(i)
						 *			4:sw_d(io)
						 *			5:tim4_pwm_2(o)
						 */

	GPIOB2 = (1 << 2),	/** GpioMode
						 *			1:i2c_scl_2(io)
						 *			2:tim3_pwc_i_3(i)
						 *			3:uart1_txd_2(o)
						 *			4:tim3_pwm_3(o)
						 *			5:DM
						 */

	GPIOB3 = (1 << 3),	/** GpioMode
						 *			1:uart1_rxd_2(i)
						 *			2:i2c_sda_2(io)
						 *			3:tim4_pwc_i_3(i)
						 *			4:tim4_pwm_3(o)
						 *			5:DP
						 */

	GPIOB4 = (1 << 4),	/** GpioMode
						 *			1::HOSC_XI
						 */

	GPIOB5 = (1 << 5),	/** GpioMode
						 *			1:HOSC_XO
						 */

	GPIOB6 = (1 << 6),	/** GpioMode
						 *			1:uart1_rxd_0(i)
						 *			2:i2c_sda_1(io)
						 *			3:mdac_test[17](io)
						 *			4:FMR/AD11
						 */

	GPIOB7 = (1 << 7),	/** GpioMode
						 *			1:i2c_scl_1(io)
						 *			2:mdac_test[18](io)
						 *			3:uart1_txd_0(o)
						 *			4:FML/AD12
						 */

	GPIOB8 = (1 << 8),	/** GpioMode
						 *			1:MIC_DET_0(i)
						 *			2:FUSEVDD25
						 */
}GPIO_PortB;


/**************************************************************************************************
                                 通用GPIO设置或获取状态部分
**************************************************************************************************/

//************************* 1、 只设置或读取 1 bit  *****************************//

/**
 * @brief      只置位 1bit
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 * @param[in]  GpioIndex：GPIO bit位，例如：GPIO_INDEX0
 *
 * @return     无
 * __attribute__((section(".tcm_section")))
 */
void GPIO_RegOneBitSet(uint32_t	RegIndex, uint32_t GpioIndex);

/**
 * @brief      只清零 1bit
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 * @param[in]  GpioIndex：GPIO bit位，例如：GPIO_INDEX0
 *
 * @return     无
 * __attribute__((section(".tcm_section")))
 */
void GPIO_RegOneBitClear(uint8_t RegIndex, uint32_t GpioIndex);

/**
 * @brief      读取 1bit数据
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_IN
 * @param[in]  GpioIndex：GPIO bit位，例如：GPIO_INDEX0
 *
 * @return     1 or 0
 */
bool GPIO_RegOneBitGet(uint8_t RegIndex, uint32_t GpioIndex);


//************************* 2、 设置或读取多 bit  ***************************** //

/**
 * @brief      置位多 bits
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 * @param[in]  GpioIndex：GPIO bit位，例如：(GPIO_INDEX0 | GPIO_INDEX1)
 *
 * @return     无
 * __attribute__((section(".tcm_section")))
 */
void GPIO_RegBitsSet(uint8_t RegIndex, uint32_t GpioIndex);

/**
 * @brief      清除多 bits
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 * @param[in]  GpioIndex：GPIO bit位，例如：(GPIO_INDEX0 | GPIO_INDEX1)
 *
 * @return     无
 * __attribute__((section(".tcm_section")))
 */
void GPIO_RegBitsClear(uint8_t RegIndex, uint32_t GpioIndex);


//************************* 3、 设置或读取一组  ***************************** //

/**
 * @brief      设置一组GPIO数据
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 * @param[in]  GpioIndex：GPIO bit位，例如：(GPIO_INDEX0 | GPIO_INDEX1)
 *
 * @return     无
 */
void GPIO_RegSet(uint8_t RegIndex, uint32_t GpioIndex);

/**
 * @brief      读取一组GPIO数据
 *
 * @param[in]  RegIndex: GPIO地址索引，例如： GPIO_A_OUT
 *
 * @return     GPIO值
 */
uint32_t GPIO_RegGet(uint8_t RegIndex);


//*************************** 4、  GPIO中断  ******************************* //

/**
 * @brief      使能GPIO某一bit的中断功能
 *
 * @param[in]  RegIndex: GPIO中断地址索引，例如： GPIO_A_INT
 * @param[in]  GpioIndex: bit位，例如GPIO_INDEX0
 * @param[in]  TrigType: 触发极性，例如GPIO_NEG_EDGE_TRIGGER触发
 *
 * @return     无
 */
void GPIO_INTEnable(uint8_t RegIndex, uint32_t GpioIndex, GpioTriggerType TrigType);

/**
 * @brief      禁能GPIO某一bit的中断功能
 *
 * @param[in]  RegIndex: GPIO中断地址索引，例如： GPIO_A_INT
 * @param[in]  GpioIndex: bit位，例如GPIO_INDEX0
 *
 * @return     无
 */
void GPIO_INTDisable(uint8_t RegIndex, uint32_t GpioIndex);

/**
 * @brief      获取GPIO的中断状态
 *
 * @param[in]  RegIndex: GPIO中断地址索引，例如： GPIO_A_INT
 *
 * @return     一组GPIO的中断状态
 */
uint32_t GPIO_INTFlagGet(uint8_t RegIndex);

/**
 * @brief      清除某一位GPIO的中断状态
 *
 * @param[in]  RegIndex: GPIO中断地址索引，例如： GPIO_A_INT
 * @param[in]  GpioIndex: GPIO bit位，例如GPIO_INDEX0
 * @return     无
 */
void GPIO_INTFlagClear(uint8_t RegIndex, uint32_t GpioIndex);

//*************************** 5、  GPIO bit mask  ******************************* //

bool GPIO_OutMaskSet(uint8_t regIndex, GpioOutMask outMask, uint32_t maskBits);

bool GPIO_OutMaskClear(uint8_t regIndex, GpioOutMask outMask);

/**************************************************************************************************
                                 GPIO与其他功能的复用部分
**************************************************************************************************/

/**
 * @brief      设置GPIOA端口复用关系
 *
 * @param[in]  port: GPIO port，例如： GPIOA10
 * @param[in]  GpioMode: GPIO 模式， 0 代表使用为普通GPIO口，其他值参考 GPIO_PortA 注释。
 *
 * @return     无
 */
void GPIO_PortAModeSet(GPIO_PortA port, uint8_t GpioMode);

/**
 * @brief      设置GPIOB端口复用关系
 *
 * @param[in]  port: GPIO port，例如： GPIOB0
 * @param[in]  GpioMode: GPIO 模式， 0 代表使用为普通GPIO口，其他值参考 GPIO_PortB 注释。
 *
 * @return     无
 */
void GPIO_PortBModeSet(GPIO_PortB port, uint8_t GpioMode);

/**
 * @brief      进入DeepSleep状态的GPIO端口的模式配置
 *
 * @param      无
 * @return     无
 */
void GPIO_DeepSleepModeConfig(void);

/**
 * @brief      获取GPIOA端口复用关系
 *
 * @param[in]  port: GPIO port，例如： GPIOA10
 *
 * @return     GPIO端口模式
 */
uint8_t  GPIO_PortAModeGet(GPIO_PortA port);

/**
 * @brief      获取GPIOB端口复用关系
 *
 * @param[in]  port: GPIO port，例如： GPIOB0
 *
 * @return     GPIO端口模式
 */
uint8_t GPIO_PortBModeGet(GPIO_PortB port);

/**
 * @brief      获取GPIOA18输出强驱动力24mA
 *
 * @param[in]  无
 *
 * @return     无
 */
void GPIO_RegA18OutdsSet24mA(void);

/**
 * @brief      获取GPIOA18输出弱驱动力8mA
 *
 * @param[in]  无
 *
 * @return     无
 */
void GPIO_RegA18OutdsSet8mA(void);


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__GPIO_HAL_H__
