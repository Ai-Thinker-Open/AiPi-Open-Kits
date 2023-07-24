#ifndef __I2C_HAL_H__
#define __I2C_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus
#include "type.h"

#define	I2C_WRITE	        0x00    //I2C write command    
#define	I2C_READ	        0x01    //I2C read command

/**
 * i2c define
 */
typedef enum 
{
	ENABLE					= 1,
	DISABLE					= 0
}I2C_CONFIG;

/**
 * i2c mode define
 */
typedef enum 
{
	MASTER					= 1,
	SLAVE					= 0
}I2C_CONFIG_MODE;

/**
 * i2c port define
 */
typedef enum 
{
	I2C_PORT_B0_B1			= 0,
	I2C_PORT_B7_B6			= 1,
	I2C_PORT_B2_B3			= 2
}I2C_PORT_MODE;

/**
 * i2c state define
 */
typedef enum 
{
	I2C_OK       			= 0x00,
	I2C_ERROR    			= 0x01,
	I2C_BUSY     			= 0x02,
	I2C_TIMEOUT  			= 0x03
}I2C_StatusTypeDef;

/**
 * err state define
 */
typedef enum 
{
	ERROR_OK       			= 0x00,
	ERROR_BUSY    			= 0x01,
	ERROR_TIMEOUT    		= 0x02,
	ERROR_NOACK				= 0x03,
	ERROR_UNMATCH			= 0x04,
	ERROR_SLAVEMODE			= 0x05,
	ERROR_DIV_OVERSTEP		= 0x06
}I2C_ErrorState;


/**
 * i2c速率计算公式:
 * PLL模式下: i2c_rate = OSC-PLL/4/(APB_CLK_DIV_NUM+1)/I2C_CLK_DIV
 * RC模式下: i2c_rate = OSC-RC/(APB_CLK_DIV_NUM+1)/I2C_CLK_DIV
 * 默认设置: OSC-PLL = 480M 
 *           OSC-RC = 12M
 *           APB_CLK_DIV_NUM = 7
 * 参考参数:
 *     (1)I2C_CLK_DIV_40(0x26) i2c_rate = 375KHz
 *     (2)I2C_CLK_DIV_72(0x2B) i2c_rate = 208KHz
 *     (3)I2C_CLK_DIV_160(0x30) i2c_rate = 93KHz
 * 注: i2c速率不要超过400K
 * i2c 分频比和设置参数关系表如下:
 ******************************************************************************
 ****** Div | Param **** Div  | Param **** Div | Param **** Div  | Param ******
 ****** 30  | 0x00  **** 288  | 0x10  **** 22  | 0x20  **** 160  | 0x30  ******
 ****** 32  | 0x01  **** 320  | 0x11  **** 24  | 0x21  **** 192  | 0x31  ******
 ****** 36  | 0x02  **** 384  | 0x12  **** 26  | 0x22  **** 224  | 0x32  ******
 ****** 42  | 0x03  **** 480  | 0x13  **** 28  | 0x23  **** 256  | 0x33  ******
 ****** 48  | 0x04  **** 576  | 0x14  **** 32  | 0x24  **** 320  | 0x34  ******
 ****** 52  | 0x05  **** 640  | 0x15  **** 36  | 0x25  **** 384  | 0x35  ******
 ****** 60  | 0x06  **** 768  | 0x16  **** 40  | 0x26  **** 448  | 0x36  ******
 ****** 72  | 0x07  **** 960  | 0x17  **** 44  | 0x27  **** 512  | 0x37  ******
 ****** 80  | 0x08  **** 1152 | 0x18  **** 48  | 0x28  **** 640  | 0x38  ******
 ****** 88  | 0x09  **** 1280 | 0x19  **** 56  | 0x29  **** 768  | 0x39  ******
 ****** 104 | 0x0A  **** 1536 | 0x1A  **** 64  | 0x2A  **** 896  | 0x3A  ******
 ****** 128 | 0x0B  **** 1920 | 0x1B  **** 72  | 0x2B  **** 1024 | 0x3B  ******
 ****** 144 | 0x0C  **** 2304 | 0x1C  **** 80  | 0x2C  **** 1280 | 0x3C  ******
 ****** 160 | 0x0D  **** 2560 | 0x1D  **** 96  | 0x2D  **** 1536 | 0x3D  ******
 ****** 192 | 0x0E  **** 3072 | 0x1E  **** 112 | 0x2E  **** 1792 | 0x3E  ******
 ****** 240 | 0x0F  **** 3840 | 0x1F  **** 128 | 0x2F  **** 2048 | 0x3F  ******
 ******************************************************************************
 */

/**
 * @brief     i2c 初始化
 *
 * @param[in] I2cClkParam 分频参数设置(范围0x00-0x3f)，详见 # i2c div table
 * @param[in] port      i2c使用端口选择，详见 # I2C_PORT_MODE
 * @param[in] OwnAddr   i2c做slave，需要配置自身地址，支持7bit地址模式，有效范围0x08-0x77
 *                      i2c做master，可随意配置
 *
 * @return    state     详见 # I2C_ErrorState
 */
I2C_ErrorState I2C_Init(uint8_t I2cClkParam, I2C_PORT_MODE port, uint8_t OwnAddr);

/**
 * @brief     I2C主模式下发送数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_MasterSendBuffer(uint8_t SlaveAddr, void* SendBuf, uint32_t BufLen, uint32_t timeout);

/**
 * @brief     I2C主模式下发送数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] RegAddr   从设备寄存器地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_MasterSendData(uint8_t SlaveAddr, uint8_t RegAddr, void* SendBuf, uint32_t BufLen, uint32_t timeout);

/**
 * @brief     I2C主模式下接收数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_MasterReceiveBuffer(uint8_t SlaveAddr, void* RecvBuf, uint32_t BufLen, uint32_t timeout);

/**
 * @brief     I2C主模式下接收数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] RegAddr   从设备寄存器地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_MasterReceiveData(uint8_t SlaveAddr, uint8_t RegAddr, void* RecvBuf, uint32_t BufLen, uint32_t timeout);

/**
 * @brief     I2C从模式下发送数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_SlaveSendBuffer(uint8_t *SendBuf, uint32_t BufLen, uint32_t timeout);

/**
 * @brief     I2C从模式下接收数据
 *
 * @param[in] SlaveAddr 从设备地址
 * @param[in] SendBuf   发送数据缓存
 * @param[in] BufLen    数据长度
 * @param[in] timeout   超时时间设置
 *
 * @return    i2c state  详见 I2C_ErrorState
 */
I2C_ErrorState I2C_SlaveReceiveBuffer(uint8_t *RecvBuf, uint32_t BufLen, uint32_t timeout);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__I2C_HAL_H__
