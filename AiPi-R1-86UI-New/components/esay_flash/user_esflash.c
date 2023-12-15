/**
 * @file user_flash.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2023
 *
*/
#include "stdio.h"
#include "string.h"
#include "easyflash.h"
#include "FreeRTOS.h"

void esay_flash_init(void)
{
    bflb_mtd_init();
    easyflash_init();
}
/**
 * @brief
 *
 * @param key
 * @param value
*/
void flash_erase_set(char* key, char* value)
{
    size_t len = 0;
    int value_len = strlen(value);
    ef_set_and_save_env(key, value);
    // bflb_flash_read(key, flash_data, strlen(value));
    // printf("writer data:%s\r\n", flash_data);
    memset(value, 0, strlen(value));
    ef_get_env_blob(key, value, value_len, &len);
}
/**
 * @brief
 *
 * @param key
 * @return char*
*/
char* flash_get_data(char* key, int len)
{
    static char* flash_data;
    flash_data = pvPortMalloc(len);
    memset(flash_data, 0, len);

    ef_get_env_blob(key, flash_data, len, (size_t)&len);

    return flash_data;
}

int flash_check_key(char* key)
{
    char* flash_data = NULL;
    uint8_t len = 64;

    flash_data = flash_get_data(key, len);
    len = strlen(flash_data);
    vPortFree(flash_data);
    return (int)len;
}
/**
 * @brief flash_erase_data
 *  擦出flash
 * @param key
*/
void flash_erase_data(char* key)
{
    ef_del_and_save_env(key);
}