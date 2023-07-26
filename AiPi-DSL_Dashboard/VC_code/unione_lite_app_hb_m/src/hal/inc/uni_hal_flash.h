#ifndef __FLASH_HAL_H__
#define __FLASH_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include "spi_flash.h"

//please note you need first erase ,then you can write flash

extern SPI_FLASH_ERR_CODE uni_hal_flash_read(uint32_t StartAddr, uint8_t* Buffer, uint32_t Length, uint32_t TimeOut);
extern SPI_FLASH_ERR_CODE uni_hal_flash_write(uint32_t	Addr, uint8_t	*Buffer, uint32_t 	Length, uint32_t TimeOut);

/***************************************************************************************************
 *	CHIP_ERASE:		Erase whole chip
 *	SECTOR_ERASE:	Erase One sector
 *  BLOCK_ERASE:	Erase One block		
 
 for SECTOR_ERASE,the erase size and addr must be 4KB aligned
 for BLOCK_ERASE, the erase size and addr must be 64KB aligned
 for CHIP_ERASE type, param 2 & param 3 are NOT cared
***************************************************************************************************/
extern int32_t uni_hal_flash_erase(ERASE_TYPE_ENUM erase_type, uint32_t EraseAddr, uint32_t NumByteToErase);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__FLASH_HAL_H__