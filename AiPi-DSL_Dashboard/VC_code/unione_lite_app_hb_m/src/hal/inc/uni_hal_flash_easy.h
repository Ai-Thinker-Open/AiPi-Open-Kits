#ifndef __FLASH_EASY_HAL_H__
#define __FLASH_EASY_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* EasyFlash error code */
typedef enum {
    EF_NO_ERR,
    EF_ERASE_ERR,
    EF_READ_ERR,
    EF_WRITE_ERR,
    EF_ENV_NAME_ERR,
    EF_ENV_NAME_EXIST,
    EF_ENV_FULL,
    EF_ENV_INIT_FAILED,
} EfErrCode;

extern EfErrCode uni_hal_flash_easy_init(void);
extern size_t uni_hal_flash_easy_get_env_blob(const char *key, void *value_buf, size_t buf_len, size_t *save_value_len);
extern EfErrCode uni_hal_flash_easy_set_env_blob(const char *key, const void *value_buf, size_t buf_len);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__FLASH_EASY_HAL_H__