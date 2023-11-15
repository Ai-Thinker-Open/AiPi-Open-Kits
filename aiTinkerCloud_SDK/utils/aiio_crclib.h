/** @brief      crc library interface
 *
 *  @file       aiio_crclib.h
 *  @copyright  Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 *  @note       crc library interface
 *  @par        Change Logs:
 *  <table>
 *  <tr><th>Date            <th>Version     <th>Author      <th>Notes
 *  <tr><td>2023/06/13      <td>1.0.0       <td>zhuolm         <td>crc library
 *  </table>
 *
 */
#ifndef __AIIO_CRCLIB_H_
#define __AIIO_CRCLIB_H_
#include "stdint.h"




uint8_t aiio_crc4_itu(uint8_t *data, uint16_t length);
uint8_t aiio_crc5_epc(uint8_t *data, uint16_t length);
uint8_t aiio_crc5_itu(uint8_t *data, uint16_t length);
uint8_t aiio_crc5_usb(uint8_t *data, uint16_t length);
uint8_t aiio_crc6_itu(uint8_t *data, uint16_t length);
uint8_t aiio_crc7_mmc(uint8_t *data, uint16_t length);
uint8_t aiio_crc8(uint8_t *data, uint16_t length);
uint8_t aiio_crc8_itu(uint8_t *data, uint16_t length);
uint8_t aiio_crc8_rohc(uint8_t *data, uint16_t length);
uint8_t aiio_crc8_maxim(uint8_t *data, uint16_t length);//DS18B20
uint16_t aiio_crc16_ibm(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_maxim(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_usb(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_modbus(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_ccitt(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_ccitt_false(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_x25(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_xmodem(uint8_t *data, uint16_t length);
uint16_t aiio_crc16_dnp(uint8_t *data, uint16_t length);
uint32_t aiio_crc32(uint8_t *data, uint16_t length);
uint32_t aiio_crc32_mpeg_2(uint8_t *data, uint16_t length);



#endif 

