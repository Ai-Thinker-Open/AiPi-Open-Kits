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

static bool ef_set_bytes(char* key, uint8_t* value, int len)
{
    int ret = ef_set_env_blob(key, value, len);
    return ret == EF_NO_ERR ? true : false;
}

static int ef_get_bytes(const char* key, uint8_t* value, int len) {
    size_t read_len = 0;
    int result = ef_get_env_blob(key, value, len, &read_len);
    return result;
}

bool ef_set_u32(const char* key, uint32_t value)
{
    uint8_t data[4];
    *(uint32_t*)data = value;
    return ef_set_bytes(key, data, sizeof(data));
}

bool ef_get_u32(const char* key, uint32_t* value)
{
    uint8_t data[4];
    if (ef_get_bytes(key, data, sizeof(data))) {
        memcpy(value, data, 4);
        return true;
    }
    else {
        return false;
    }
}

bool ef_set_int(const char* key, int value)
{
    uint8_t data[4];
    *(int*)data = value;
    return ef_set_bytes(key, data, sizeof(data));
}

bool ef_get_int(const char* key, int* value)
{
    uint8_t data[4];
    if (ef_get_bytes(key, data, sizeof(data))) {
        memcpy(value, data, 4);
        return true;
    }
    else {
        return false;
    }
}

bool ef_set_u64(const char* key, uint64_t value)
{
    uint8_t data[8];
    *(uint64_t*)data = value;
    return ef_set_bytes(key, data, sizeof(data));
}

bool ef_get_u64(const char* key, uint64_t* value)
{
    uint8_t data[8];
    if (ef_get_bytes(key, data, sizeof(data))) {
        memcpy(value, data, sizeof(data));
        return true;
    }
    else {
        return false;
    }
}