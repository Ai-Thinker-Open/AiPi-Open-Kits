/**
 * @brief
 *
 * @file    aiio_common.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-07-14          <td>1.0.0            <td>zhuolm             <td>
 */

#include "aiio_common.h"


static bool aiio_buffer_is_empty_check(char* buffer, uint32_t buffer_len)
{
    if (buffer == NULL)
    {
        aiio_log_e("param err \r\n");
        return false;
    }

    for (uint32_t num = 0; num < buffer_len; num++)
    {
        if (buffer[num] != 0xff)
        {
            return false;
        }
    }

    return true;
}


static bool aiio_wifi_config_data_is_empty_check(wifi_config_data_t* config_data)
{
    if (config_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return true;
    }

    if (aiio_buffer_is_empty_check(config_data->ssid, sizeof(config_data->ssid)))               return true;
    if (aiio_buffer_is_empty_check(config_data->passwd, sizeof(config_data->passwd)))             return true;
    if (aiio_buffer_is_empty_check(config_data->wificc, sizeof(config_data->wificc)))             return true;
    if (aiio_buffer_is_empty_check(config_data->mqttip, sizeof(config_data->mqttip)))             return true;
    if (aiio_buffer_is_empty_check(config_data->token, sizeof(config_data->token)))              return true;
    if (aiio_buffer_is_empty_check(config_data->tz, sizeof(config_data->tz)))                 return true;
    if (config_data->port == 0xff)                                   return true;


    if (aiio_strlen(config_data->ssid) == 0  ||
        aiio_strlen(config_data->wificc) == 0 || aiio_strlen(config_data->mqttip) == 0 ||
        aiio_strlen(config_data->token) == 0 || aiio_strlen(config_data->tz) == 0 || config_data->port == 0)
    {
        return true;
    }

    return false;
}


int aiio_flash_get_wifi_config_data(wifi_config_data_t* config_data)
{
    uint16_t wifi_config_data_len = 0;
    int32_t ret = 0;

    // if (config_data == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return AIIO_ERROR;
    // }

    // aiio_log_d("aiio_flash_read \r\n");
    // wifi_config_data_len = sizeof(wifi_config_data_t);
    // aiio_flash_read(FLASH_RW_WIFI_DATA_ADDR, config_data, &wifi_config_data_len);

    // aiio_log_d("aiio_wifi_config_data_is_empty_check \r\n");
    // if (aiio_wifi_config_data_is_empty_check(config_data))
    // {
    //     aiio_log_e("Failed to get the data from the flash \r\n");
    //     return 0;
    // }

    // aiio_log_d("ssid = %s \r\n", config_data->ssid);
    // aiio_log_d("passwd = %s \r\n", config_data->passwd);
    // aiio_log_d("wificc = %s \r\n", config_data->wificc);
    // aiio_log_d("mqttip = %s \r\n", config_data->mqttip);
    // aiio_log_d("token = %s \r\n", config_data->token);
    // aiio_log_d("tz = %s \r\n", config_data->tz);
    // aiio_log_d("ts = %d \r\n", config_data->ts);
    // aiio_log_d("port = %d \r\n", config_data->port);

    // aiio_log_d("wifi_config_data_len = %d \r\n", wifi_config_data_len);

    return wifi_config_data_len;
}

void aiio_flash_save_wifi_config_data(wifi_config_data_t* config_data)
{
    // aiio_flash_erase(FLASH_RW_WIFI_DATA_ADDR, sizeof(wifi_config_data_t));
    // aiio_flash_write(FLASH_RW_WIFI_DATA_ADDR, config_data, sizeof(wifi_config_data_t));
    // aiio_log_d("wifi config data save ok \r\n");
}


void aiio_flash_clear_config_data(void)
{
    int32_t ret = 0;
    wifi_config_data_t config_data = { 0 };

    // aiio_flash_erase(FLASH_RW_WIFI_DATA_ADDR, sizeof(wifi_config_data_t));
    // aiio_flash_write(FLASH_RW_WIFI_DATA_ADDR, &config_data, sizeof(wifi_config_data_t));
    // aiio_log_d("nvs set data ok \r\n");

}





static bool aiio_device_info_is_empty_check(aiio_device_info* device_info)
{
    if (device_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return true;
    }

    if (aiio_buffer_is_empty_check(device_info->userName, sizeof(device_info->userName)))               return true;
    if (aiio_buffer_is_empty_check(device_info->userPasswd, sizeof(device_info->userPasswd)))             return true;
    if (aiio_buffer_is_empty_check(device_info->deviceId, sizeof(device_info->deviceId)))             return true;

    if (aiio_strlen(device_info->userName) == 0 || aiio_strlen(device_info->userPasswd) == 0 ||
        aiio_strlen(device_info->deviceId) == 0)
    {
        return true;
    }

    return false;
}


int aiio_flash_get_device_info(aiio_device_info* device_info)
{
    uint16_t data_len = 0;

    // if(device_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return AIIO_ERROR;
    // }

    // aiio_log_d("aiio_flash_read \r\n");
    // data_len = sizeof(aiio_device_info);
    // aiio_flash_read(FLASH_RW_DEVICE_INFO_ADDR, device_info, &data_len);

    // aiio_log_d("aiio_device_info_is_empty_check \r\n");
    // if(aiio_device_info_is_empty_check(device_info))
    // {
    //     aiio_log_e("Failed to get the data from the flash \r\n");
    //     return AIIO_ERROR;
    // }

    // aiio_log_d("device_info.userName = %s \r\n", device_info->userName);
    // aiio_log_d("device_info.userPasswd = %s \r\n", device_info->userPasswd);
    // aiio_log_d("device_info.deviceId = %s \r\n", device_info->deviceId);


    return data_len;
}

int aiio_flash_clear_device_info(aiio_device_info* device_info)
{
    // if(device_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return AIIO_ERROR;
    // }

    // memset(device_info, 0, sizeof(aiio_device_info));
    // aiio_flash_erase(FLASH_RW_DEVICE_INFO_ADDR, sizeof(aiio_device_info));
    // aiio_flash_write(FLASH_RW_DEVICE_INFO_ADDR, device_info, sizeof(aiio_device_info));
    // aiio_log_d("nvs set data ok \r\n");

    return AIIO_OK;
}

int aiio_flash_save_device_info(aiio_device_info* device_info)
{
    // if(device_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return 0;
    // }

    // aiio_flash_erase(FLASH_RW_DEVICE_INFO_ADDR, sizeof(aiio_device_info));
    // aiio_flash_write(FLASH_RW_DEVICE_INFO_ADDR, device_info, sizeof(aiio_device_info));
    // aiio_log_d("device info save ok \r\n");

    return AIIO_OK;
}



static bool aiio_ota_info_is_empty_check(aiio_ota_msg_t* ota_info)
{
    if (ota_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return true;
    }

    if (aiio_buffer_is_empty_check(ota_info->otaVer, sizeof(ota_info->otaVer)))                           return true;
    if (aiio_buffer_is_empty_check(ota_info->otaType, sizeof(ota_info->otaType)))                         return true;
    if (aiio_buffer_is_empty_check(ota_info->download_url, sizeof(ota_info->download_url)))               return true;
    if (aiio_buffer_is_empty_check(ota_info->PubId, sizeof(ota_info->PubId)))                             return true;
    if (aiio_buffer_is_empty_check(ota_info->md5, sizeof(ota_info->md5)))                                 return true;


    if (aiio_strlen(ota_info->otaVer) == 0 || aiio_strlen(ota_info->otaType) == 0 || aiio_strlen(ota_info->download_url) == 0 ||
        aiio_strlen(ota_info->PubId) == 0 ||  aiio_strlen(ota_info->md5) == 0 || ota_info->channel == 0)
    {
        return true;
    }

    return false;
}


int aiio_flash_get_ota_info(aiio_ota_msg_t* ota_info)
{
    uint16_t data_len = 0;

    // if(ota_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return AIIO_ERROR;
    // }

    // aiio_log_d("aiio_flash_read \r\n");
    // data_len = sizeof(aiio_ota_msg_t);
    // aiio_flash_read(FLASH_RW_OTA_INFO_ADDR, ota_info, &data_len);

    // aiio_log_d("aiio_ota_info_is_empty_check \r\n");
    // if(aiio_ota_info_is_empty_check(ota_info))
    // {
    //     aiio_log_e("Failed to get the data from the flash \r\n");
    //     return AIIO_ERROR;
    // }

    // aiio_log_d("ota_info.otaVer = %s \r\n", ota_info->otaVer);
    // aiio_log_d("ota_info.otaType = %s \r\n", ota_info->otaType);
    // aiio_log_d("ota_info.download_url = %s \r\n", ota_info->download_url);
    // aiio_log_d("ota_info.PubId = %s \r\n", ota_info->PubId);
    // aiio_log_d("ota_info.md5 = %s \r\n", ota_info->md5);
    // aiio_log_d("ota_info.channel = %d \r\n", ota_info->channel);


    return data_len;
}



int aiio_flash_clear_ota_info(aiio_ota_msg_t* ota_info)
{
    // if(ota_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return AIIO_ERROR;
    // }

    // memset(ota_info, 0, sizeof(aiio_ota_msg_t));
    // aiio_flash_erase(FLASH_RW_OTA_INFO_ADDR, sizeof(aiio_ota_msg_t));
    // aiio_flash_write(FLASH_RW_OTA_INFO_ADDR, ota_info, sizeof(aiio_ota_msg_t));
    // aiio_log_d("nvs clear ota info ok \r\n");

    return AIIO_OK;
}

int aiio_flash_save_ota_info(aiio_ota_msg_t* ota_info)
{
    // if(ota_info == NULL)
    // {
    //     aiio_log_e("param err \r\n");
    //     return 0;
    // }

    // aiio_flash_erase(FLASH_RW_OTA_INFO_ADDR, sizeof(aiio_ota_msg_t));
    // aiio_flash_write(FLASH_RW_OTA_INFO_ADDR, ota_info, sizeof(aiio_ota_msg_t));
    // aiio_log_d("ota info save ok \r\n");

    return AIIO_OK;
}