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