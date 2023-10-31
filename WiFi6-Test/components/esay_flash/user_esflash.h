/**
 * @file user_flash.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef USER_FLASH_H
#define USER_FLASH_H
void esay_flash_init(void);
void flash_erase_set(char* key, char* value);
char* flash_get_data(char* key, int len);
uint8_t flash_get_key(char* key);
#endif