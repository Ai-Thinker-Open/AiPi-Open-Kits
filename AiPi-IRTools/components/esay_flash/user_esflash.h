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
/**
 * @brief
 *
 * @param key
 * @param value
*/
void flash_erase_set(char* key, char* value);
/**
 * @brief
 *
 * @param key
 * @param len
 * @return char*
*/
char* flash_get_data(char* key, int len);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true save ok
 * @return false save error
*/
bool ef_set_u32(const char* key, uint32_t value);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true
 * @return false
*/
bool ef_get_u32(const char* key, uint32_t* value);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true
 * @return false
*/
bool ef_set_int(const char* key, int value);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true
 * @return false
*/
bool ef_get_int(const char* key, int* value);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true
 * @return false
*/
bool ef_set_u64(const char* key, uint64_t value);
/**
 * @brief
 *
 * @param key
 * @param value
 * @return true
 * @return false
*/
bool ef_get_u64(const char* key, uint64_t* value);
#endif