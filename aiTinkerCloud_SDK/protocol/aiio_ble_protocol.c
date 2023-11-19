/**
 * @brief   Define the packet protocol interface of ble distribution network, and the parse protocol of it.
 *
 * @file    aiio_ble_protocol.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is mainly for describing the ble distribution network protocol, for defining packet protocol interface and parse protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Describe the protocol interface about the ble distribution network
 */
#include "aiio_ble_protocol.h"
#include "aiio_protocol_code.h"
#include "aiio_log.h"
#include "aiio_aes.h"
#include "aiio_crclib.h"
#include "cJSON.h"
#include "aiio_stdio.h"
#include "aiio_os_port.h"
#include "platform_bl.h"
#include "aiio_error.h"


#define AIIO_PROTOCOL_HEAD_FLAG                         (0xab)
#define AIIO_PROTOCOL_HEAD_DIVIDE_FLAG                  (0xac)


#define AIIO_PROTOCOL_HEAD_LEN                          (4)
#define AIIO_DATA_PROTOCOL_HEAD_LEN                     (4)
#define AIIO_PROTOCOL_CRC_LEN                           (2)





typedef enum
{
    PROTOCOL_HEAD_POSITION = 0,
    PROTOCOL_FRAME_POSITION,
    PROTOCOL_DIVIDE_FLAG_POSITION,
    PROTOCOL_DATA_LEN_POSITION,
    PROTOCOL_DATA_POSITION,
}aiio_divide_data_structure_t;


typedef enum
{
    DATA_VERSION_POSITION = 0,
    DATA_TOTAL_DATA_LEN_POSITION,
    DATA_ENTRYPT_TYPE_POSITION = 3,
    DATA_ENTRYPT_KEY_POSITION,
    DATA_POSITION = 4,
    DATA_CRC_POSITION = 4
}aiio_data_structure_t;

typedef enum
{
    ACK_PROTOCOL_HEAD_POSITION = 0,
    ACK_PROTOCOL_FRAM_POSITION,
    ACK_PROTOCOL_DATA_LEN_POSITION,
    ACK_PROTOCOL_DATA_POSITION,
    ACK_PROTOCOL_CRC_POSITION
}aiio_divide_ack_structure_t;



static uint8_t IV[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6 };
static uint8_t last_frame = 0xff;
static uint8_t* data_info = NULL;
static aiio_ble_protocol_event_t protocol_event = { 0 };
static aiio_entrypt_type_t       protocol_entrypt_type = PROTOCOL_ENTRYPT_UNKNOW;
static uint16_t data_info_len = 0;



static bool aiio_ProtocolCrcCheck(uint8_t* data, uint16_t data_len)
{
    uint16_t crc = 0;
    uint16_t crcx = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    crc = ((data[data_len - 2] << 8) | data[data_len - 1]);

    aiio_log_i("crc = 0x%02x  \r\n", crc);
    if (crc == (crcx = aiio_crc16_xmodem(data, data_len-2)))          // crc verification
    {
        return true;
    }

    return false;
}


static bool aiio_ProtocolHeadCheck(uint8_t* data, uint16_t data_len)
{
    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (data[PROTOCOL_HEAD_POSITION] == AIIO_PROTOCOL_HEAD_FLAG)                 // head protocol verification
    {
        return true;
    }

    return false;
}


static uint8_t aiio_GetDividePacketNumber(uint8_t* data, uint16_t data_len)
{
    uint8_t num = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    num = (data[PROTOCOL_DIVIDE_FLAG_POSITION] & 0x0f);

    return num;
}


static uint8_t aiio_GetDividePacketSequence(uint8_t* data, uint16_t data_len)
{
    uint8_t num = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    num = ((data[PROTOCOL_DIVIDE_FLAG_POSITION] & 0xf0) >> 4);

    return num;
}



static uint8_t aiio_GetProtocolFrame(uint8_t* data, uint16_t data_len)
{
    uint8_t frame = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    frame = data[PROTOCOL_FRAME_POSITION];

    return frame;
}


static bool aiio_ProtocolVersionCheck(uint8_t* data, uint16_t data_len)
{
    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (data[PROTOCOL_DATA_POSITION + DATA_VERSION_POSITION] == AIIO_BLE_PROCOTOL_VERSION)                 // head protocol verification
    {
        return true;
    }

    return false;
}


static uint8_t aiio_GetProtocolDataLen(uint8_t* data, uint16_t data_len)
{
    uint8_t len = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    len = data[PROTOCOL_DATA_LEN_POSITION];

    return len;
}



static uint16_t aiio_GetProtocolDataTotalLen(uint8_t* data, uint16_t data_len)
{
    uint16_t len = 0;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    len = ((data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION] << 8) | data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION + 1]);         // Get the total lenght

    return len;
}


static aiio_entrypt_type_t aiio_GetProtocolEntryptType(uint8_t* data, uint16_t data_len)
{
    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (data[PROTOCOL_DATA_POSITION + DATA_ENTRYPT_TYPE_POSITION] == PROTOCOL_ENTRYPT_NONE)
    {
        return PROTOCOL_ENTRYPT_NONE;
    }
    else if (data[PROTOCOL_DATA_POSITION + DATA_ENTRYPT_TYPE_POSITION] == PROTOCOL_ENTRYPT_AES_CBC_128)
    {
        return PROTOCOL_ENTRYPT_AES_CBC_128;
    }
    else
    {
        return PROTOCOL_ENTRYPT_UNKNOW;
    }
}


static int aiio_GetProtocolData(uint8_t* source_data, uint16_t data_len, aiio_entrypt_type_t entrypt_type, uint8_t data_position, uint8_t* destination, uint16_t destination_len)
{
    uint8_t len = 0;

    if (source_data == NULL || destination == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (data_position == PROTOCOL_DATA_POSITION)
    {
        if (destination_len >  (data_len - data_position - AIIO_PROTOCOL_CRC_LEN))              /*it is avoid the memory out of bounds when taking data */
        {
            len = data_len - data_position - AIIO_PROTOCOL_CRC_LEN;
            aiio_strncpy((char*)destination, (char*)&source_data[data_position], len);             /* Gets the data in the data area of protocol format*/
        }
        else
        {
            len = destination_len;
            aiio_strncpy((char*)destination, (char*)&source_data[data_position], len);             /* Gets the data in the data area of protocol format*/
        }
    }
    else
    {
        if (entrypt_type == PROTOCOL_ENTRYPT_NONE)
        {
            if (destination_len >  (data_len - data_position - AIIO_PROTOCOL_CRC_LEN))              /* it is avoid the memory out of bounds when taking data*/
            {
                len = data_len - data_position - AIIO_PROTOCOL_CRC_LEN;
                aiio_strncpy((char*)destination, (char*)&source_data[data_position], len);             /* Gets the data in the data area of protocol format*/
            }
            else
            {
                len = destination_len;
                aiio_strncpy((char*)destination, (char*)&source_data[data_position], len);             /* Gets the data in the data area of protocol format*/
            }
        }
        else if (entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)
        {
            if (destination_len >  (data_len - data_position - AIIO_AES_CRC_128_ENTRYPT_KEY_LEN - AIIO_PROTOCOL_CRC_LEN))              /* it is avoid the memory out of bounds when taking data*/
            {
                len = data_len - data_position - AIIO_AES_CRC_128_ENTRYPT_KEY_LEN - AIIO_PROTOCOL_CRC_LEN;
                aiio_strncpy((char*)destination, (char*)&source_data[data_position + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN], len);             /* Gets the data in the data area of protocol format*/
            }
            else
            {
                len = destination_len;
                aiio_strncpy((char*)destination, (char*)&source_data[data_position + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN], len);             /* Gets the data in the data area of protocol format*/
            }
        }
        else
        {
            aiio_log_e("entrypt type err \r\n");
            return entrypt_type;
        }
    }

    return len;
}


// static void aiio_HexPrint(char *str, uint8_t *data, uint16_t data_len)
// {
//     if(str)
//     {
//         aiio_log_d("\n%s: \r\n", str);
//     }
//     for(uint16_t n = 1; n <= data_len; n++)
//     {
//         aiio_log_d("%02x ", data[n-1]);
//         if(n  % 16 == 0)
//         {
//             aiio_log_d("\r\n");
//         }
//     }
//     aiio_log_d("\r\n");
//     aiio_log_d("\r\n");
// }


static int aiio_DestroyBleEvent(aiio_ble_protocol_event_t* event)
{
    if (event == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (event->ble_data)
    {
        if (event->ble_data->mqttip)
        {
            aiio_os_free(event->ble_data->mqttip);
        }

        if (event->ble_data->passwd)
        {
            aiio_os_free(event->ble_data->passwd);
        }

        if (event->ble_data->ssid)
        {
            aiio_os_free(event->ble_data->ssid);
        }

        if (event->ble_data->token)
        {
            aiio_os_free(event->ble_data->token);
        }

        if (event->ble_data->tz)
        {
            aiio_os_free(event->ble_data->tz);
        }

        if (event->ble_data->wificc)
        {
            aiio_os_free(event->ble_data->wificc);
        }

        aiio_os_free(event->ble_data);
    }

    if (event->entrypt_key)
    {
        aiio_os_free(event->entrypt_key);
    }

    aiio_memset((char*)event, 0, sizeof(aiio_ble_protocol_event_t));

    return AIIO_BLE_CODE_OK;
}


static int aiio_GetProtocolEntryptKey(uint8_t* data, uint16_t data_len, aiio_entrypt_type_t entrypt_type, aiio_ble_protocol_event_t* event)
{
    if (data == NULL || event == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)
    {
        event->entrypt_key = (char*)aiio_os_malloc(AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + 1);
        if (event->entrypt_key == NULL)
        {
            aiio_log_e("aiio_os_malloc err \r\n");
            return AIIO_MALLOC_FAIL;
        }
        aiio_memset(event->entrypt_key, 0, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + 1);

        aiio_strncpy((char*)event->entrypt_key, (char*)&data[PROTOCOL_DATA_POSITION + DATA_ENTRYPT_KEY_POSITION], AIIO_AES_CRC_128_ENTRYPT_KEY_LEN);
    }

    return AIIO_BLE_CODE_OK;
}


static void aiio_DestroyBleData(void)
{
    aiio_DestroyBleEvent(&protocol_event);
    if (data_info)
    {
        aiio_os_free(data_info);
        data_info = NULL;
    }
    data_info_len = 0;
    last_frame = 0xff;
    protocol_entrypt_type = PROTOCOL_ENTRYPT_UNKNOW;
}


static void aiio_destroy_ble_data(aiio_ble_data_t* ble_data)
{
    if (ble_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return;
    }

    if (ble_data->ssid)
    {
        aiio_os_free(ble_data->ssid);
    }

    if (ble_data->passwd)
    {
        aiio_os_free(ble_data->passwd);
    }

    if (ble_data->wificc)
    {
        aiio_os_free(ble_data->wificc);
    }

    if (ble_data->mqttip)
    {
        aiio_os_free(ble_data->mqttip);
    }

    if (ble_data->token)
    {
        aiio_os_free(ble_data->token);
    }

    if (ble_data->tz)
    {
        aiio_os_free(ble_data->tz);
    }
}


static int aiio_JsonProcotolDataParse(uint8_t* data, uint16_t data_len, aiio_ble_protocol_event_t* event)
{
    cJSON* json_root = NULL;
    cJSON* json_value = NULL;
    char* json_str = NULL;

    if (data == NULL || event == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_root = cJSON_Parse((char*)data);
    if (!json_root)
    {
        aiio_log_e("cjson parse fail \r\n");
        return AIIO_JSON_PARSE_FAIL;
    }

    event->ble_data = (aiio_ble_data_t*)aiio_os_malloc(sizeof(aiio_ble_data_t));
    AIIO_MEM_CHECK(event->ble_data, goto end);
    aiio_memset((char*)event->ble_data, 0, sizeof(aiio_ble_data_t));

    // ssid
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_SSID);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    AIIO_MEM_CHECK(json_str, goto end);
    event->ble_data->ssid = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
    AIIO_MEM_CHECK(event->ble_data->ssid, goto end);
    aiio_memset(event->ble_data->ssid, 0, aiio_strlen(json_str) + 1);
    aiio_strncpy((char*)event->ble_data->ssid, json_str, aiio_strlen(json_str));
    aiio_log_i("ssid = %s \r\n", event->ble_data->ssid);

    // passwd
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_PASSWD);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    if (json_str != NULL)
    {
        event->ble_data->passwd = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
        AIIO_MEM_CHECK(event->ble_data->passwd, goto end);
        aiio_memset(event->ble_data->passwd, 0, aiio_strlen(json_str) + 1);
        aiio_strncpy((char*)event->ble_data->passwd, json_str, aiio_strlen(json_str));
        aiio_log_i("passwd = %s \r\n", event->ble_data->passwd);
    }

    // wificc
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_WIFICC);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    AIIO_MEM_CHECK(json_str, goto end);
    event->ble_data->wificc = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
    AIIO_MEM_CHECK(event->ble_data->wificc, goto end);
    aiio_memset(event->ble_data->wificc, 0, aiio_strlen(json_str) + 1);
    aiio_strncpy((char*)event->ble_data->wificc, json_str, aiio_strlen(json_str));
    aiio_log_i("wificc = %s \r\n", event->ble_data->wificc);

    // mqttip
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_MQTTIP);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    AIIO_MEM_CHECK(json_str, goto end);
    event->ble_data->mqttip = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
    AIIO_MEM_CHECK(event->ble_data->mqttip, goto end);
    aiio_memset(event->ble_data->mqttip, 0, aiio_strlen(json_str) + 1);
    aiio_strncpy((char*)event->ble_data->mqttip, json_str, aiio_strlen(json_str));
    aiio_log_i("mqttip = %s \r\n", event->ble_data->mqttip);

    // tz
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_TZ);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    AIIO_MEM_CHECK(json_str, goto end);
    event->ble_data->tz = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
    AIIO_MEM_CHECK(event->ble_data->tz, goto end);
    aiio_memset(event->ble_data->tz, 0, aiio_strlen(json_str) + 1);
    aiio_strncpy(event->ble_data->tz, json_str, aiio_strlen(json_str));
    aiio_log_i("tz = %s \r\n", event->ble_data->tz);

    // ts
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_TS);
    AIIO_MEM_CHECK(json_value, goto end);
    if (cJSON_String == json_value->type)
    {
        json_str = cJSON_GetStringValue(json_value);
        AIIO_MEM_CHECK(json_str, goto end);
        event->ble_data->ts = aiio_atoi(json_str);
        aiio_log_i("ts = %d \r\n", event->ble_data->ts);
    }
    else if (cJSON_Number == json_value->type)
    {
        event->ble_data->ts = (uint32_t)cJSON_GetNumberValue(json_value);
        aiio_log_i("ts = %d \r\n", event->ble_data->ts);
    }

    // token
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_TOKEN);
    AIIO_MEM_CHECK(json_value, goto end);
    json_str = cJSON_GetStringValue(json_value);
    AIIO_MEM_CHECK(json_str, goto end);
    event->ble_data->token = (char*)aiio_os_malloc(aiio_strlen(json_str) + 1);
    AIIO_MEM_CHECK(event->ble_data->token, goto end);
    aiio_memset(event->ble_data->token, 0, aiio_strlen(json_str) + 1);
    aiio_strncpy(event->ble_data->token, json_str, aiio_strlen(json_str));
    aiio_log_i("token = %s \r\n", event->ble_data->token);


    // port
    json_value = cJSON_GetObjectItem(json_root, BLE_DATA_STR_MQTTPORT);
    AIIO_MEM_CHECK(json_value, goto end);
    event->ble_data->port = cJSON_GetNumberValue(json_value);
    aiio_log_i("port = %d \r\n", event->ble_data->port);

    cJSON_Delete(json_root);

    return AIIO_OK;


end:
    cJSON_Delete(json_root);
    if (event->ble_data)
    {
        aiio_destroy_ble_data(event->ble_data);
        free(event->ble_data);
    }

    return AIIO_ERROR;
}


int aiio_ParseBleProtocolData(uint8_t* data, uint16_t data_len, aiio_ble_data_parse_cb data_parse_cb)
{
    uint8_t  proc_frame = 0;
    uint8_t  packet_num = 0;
    uint8_t  packet_seq = 0;
    uint8_t  len = 0;
    uint16_t total_len = 0;
    int32_t ret = 0;
    struct AES_ctx ctx;

    if (data == NULL)
    {
        aiio_log_e("param err \r\n");
        aiio_DestroyBleData();
        return AIIO_PARAM_ERR;
    }

    // aiio_HexPrint("source data",data, data_len);
    aiio_log_d("data_len = %d \r\n", data_len);

    if (!aiio_ProtocolCrcCheck(data, data_len))
    {
        aiio_log_e("ble data crc check err \r\n");
        protocol_event.event = AIIO_BLE_DATA_CRC_ERR_EVENT;
        if (data_parse_cb)
        {
            data_parse_cb(&protocol_event);
        }
        aiio_DestroyBleData();
        return AIIO_BLE_ACK_CRC_ERR;
    }

    if (!aiio_ProtocolHeadCheck(data, data_len))
    {
        aiio_log_e("ble data protocol format check err \r\n");
        protocol_event.event = AIIO_BLE_DATA_PROTOCOL_ERR_EVENT;
        if (data_parse_cb)
        {
            data_parse_cb(&protocol_event);
        }
        aiio_DestroyBleData();
        return AIIO_BLE_ACK_TYPE_ERR;
    }

    proc_frame = aiio_GetProtocolFrame(data, data_len);
    if (proc_frame != last_frame)
    {
        last_frame = proc_frame;
    }
    else
    {
        aiio_log_e("protocol frame check err \r\n");
        protocol_event.event = AIIO_BLE_DATA_FRAME_ERR_EVENT;
        if (data_parse_cb)
        {
            data_parse_cb(&protocol_event);
        }
        aiio_DestroyBleData();
        return AIIO_BLE_ACK_FRAME_ERR;
    }
    aiio_log_d("proc_frame = %d \r\n", proc_frame);

    protocol_event.frame = proc_frame;
    protocol_event.event = AIIO_BLE_DATA_VERIFICATE_OK;
    if (data_parse_cb)
    {
        data_parse_cb(&protocol_event);
    }

    len = aiio_GetProtocolDataLen(data, data_len);
    packet_num = aiio_GetDividePacketNumber(data, data_len);
    packet_seq = aiio_GetDividePacketSequence(data, data_len);
    if (packet_seq == 0  && data_info != NULL)                       // it will avoid memory leakage when receiving the multi-package shard data while processing the shard data
    {
        aiio_os_free(data_info);
        data_info = NULL;
    }
    aiio_log_d("len = %d \r\n", len);
    aiio_log_d("packet_num = %d \r\n", packet_num);
    aiio_log_d("packet_seq = %d \r\n", packet_seq);

    if (packet_num > 1)
    {
        if (packet_seq == 0)
        {
            if (!aiio_ProtocolVersionCheck(data, data_len))
            {
                aiio_log_e("ble data protocol version check err \r\n");
                protocol_event.event = AIIO_BLE_DATA_PROTOCOL_VER_ERR_EVENT;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_VER_ERR;
            }

            total_len = aiio_GetProtocolDataTotalLen(data, data_len);
            data_info = (uint8_t*)aiio_os_malloc(total_len + 1);
            if (data_info == NULL)
            {
                aiio_log_e("hard err \r\n");
                protocol_event.event = AIIO_BLE_DATA_HARD_ERR_EVENT;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_HARD_ERR;
            }
            aiio_memset((char*)data_info, 0, total_len + 1);

            protocol_entrypt_type = aiio_GetProtocolEntryptType(data, data_len);

            ret = aiio_GetProtocolEntryptKey(data, data_len, protocol_entrypt_type, &protocol_event);
            if (ret < 0)
            {
                aiio_log_e("hard err \r\n");
                protocol_event.event = AIIO_BLE_DATA_HARD_ERR_EVENT;
                protocol_event.entrypt_type = PROTOCOL_ENTRYPT_NONE;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_HARD_ERR;
            }

            if (protocol_event.entrypt_key)
            {
                // aiio_HexPrint("entrypt key",(uint8_t *)protocol_event.entrypt_key, aiio_strlen(protocol_event.entrypt_key));
            }

            ret = aiio_GetProtocolData(data, data_len, protocol_entrypt_type, PROTOCOL_DATA_POSITION + DATA_POSITION, data_info, len);
            if (ret < 0)
            {
                aiio_log_e("entrypt type err \r\n");
                protocol_event.event = AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_HARD_ERR;
            }
            data_info_len += ret;
        }
        else
        {
            if (data_info == NULL)
            {
                aiio_log_e("hard err \r\n");
                protocol_event.event = AIIO_BLE_DATA_HARD_ERR_EVENT;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_HARD_ERR;
            }

            ret = aiio_GetProtocolData(data, data_len, protocol_entrypt_type, PROTOCOL_DATA_POSITION, &data_info[data_info_len], len);
            if (ret < 0)
            {
                aiio_log_e("entrypt type err \r\n");
                protocol_event.event = AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_HARD_ERR;
            }
            data_info_len += len;

            if ((packet_seq + 1) == packet_num)
            {
                if (protocol_entrypt_type == PROTOCOL_ENTRYPT_NONE)
                {
                    aiio_log_i("data_info = %s \r\n", data_info);
                    ret = aiio_JsonProcotolDataParse(data_info, data_info_len, &protocol_event);
                    if (ret < 0)
                    {
                        aiio_log_e("protocol data parse err \r\n");
                        protocol_event.event = AIIO_BLE_DATA_PARSE_FAIL_EVENT;
                        protocol_event.entrypt_type = PROTOCOL_ENTRYPT_NONE;
                        if (data_parse_cb)
                        {
                            data_parse_cb(&protocol_event);
                        }
                        aiio_DestroyBleData();
                        return AIIO_BLE_ACK_TYPE_ERR;
                    }
                    else
                    {
                        aiio_log_e("protocol data parse ok \r\n");
                        protocol_event.event = AIIO_BLE_DATA_PARSE_OK_EVENT;
                        protocol_event.entrypt_type = PROTOCOL_ENTRYPT_NONE;
                        if (data_parse_cb)
                        {
                            data_parse_cb(&protocol_event);
                        }
                        aiio_DestroyBleData();
                        return AIIO_BLE_ACK_OK;
                    }
                }
                else if (protocol_entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)
                {
                    // aiio_HexPrint("entrypt key",(uint8_t *)protocol_event.entrypt_key, aiio_strlen(protocol_event.entrypt_key));
                    AES_init_ctx_iv(&ctx, (uint8_t*)protocol_event.entrypt_key, IV);
                    AES_CBC_decrypt_buffer(&ctx, data_info, data_info_len);   // Start decrypting data
                    aiio_log_i("data_info = %s \r\n", data_info);
                    ret = aiio_JsonProcotolDataParse(data_info, data_info_len, &protocol_event);
                    if (ret < 0)
                    {
                        aiio_log_e("protocol data parse err \r\n");
                        protocol_event.event = AIIO_BLE_DATA_PARSE_FAIL_EVENT;
                        protocol_event.entrypt_type = PROTOCOL_ENTRYPT_AES_CBC_128;
                        if (data_parse_cb)
                        {
                            data_parse_cb(&protocol_event);
                        }
                        aiio_DestroyBleData();
                        return AIIO_BLE_ACK_TYPE_ERR;
                    }
                    else
                    {
                        aiio_log_e("protocol data parse ok \r\n");
                        protocol_event.event = AIIO_BLE_DATA_PARSE_OK_EVENT;
                        protocol_event.entrypt_type = PROTOCOL_ENTRYPT_AES_CBC_128;
                        if (data_parse_cb)
                        {
                            data_parse_cb(&protocol_event);
                        }
                        aiio_DestroyBleData();
                        return AIIO_BLE_ACK_OK;
                    }
                }
                else
                {
                    aiio_log_e("entrypt type err \r\n");
                    protocol_event.event = AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT;
                    protocol_event.entrypt_type = PROTOCOL_ENTRYPT_UNKNOW;
                    if (data_parse_cb)
                    {
                        data_parse_cb(&protocol_event);
                    }
                    aiio_DestroyBleData();
                    return AIIO_BLE_ACK_HARD_ERR;
                }
            }

        }
    }
    else
    {
        if (!aiio_ProtocolVersionCheck(data, data_len))
        {
            aiio_log_e("ble data protocol version check err \r\n");
            protocol_event.event = AIIO_BLE_DATA_PROTOCOL_VER_ERR_EVENT;
            if (data_parse_cb)
            {
                data_parse_cb(&protocol_event);
            }

            return AIIO_BLE_ACK_VER_ERR;
        }

        total_len = aiio_GetProtocolDataTotalLen(data, data_len);
        aiio_log_d("total_len = %d \r\n", total_len);
        data_info = (uint8_t*)aiio_os_malloc(total_len + 1);
        if (data_info == NULL)
        {
            aiio_log_e("hard err \r\n");
            protocol_event.event = AIIO_BLE_DATA_HARD_ERR_EVENT;
            if (data_parse_cb)
            {
                data_parse_cb(&protocol_event);
            }
            aiio_DestroyBleData();
            return AIIO_BLE_ACK_HARD_ERR;
        }
        aiio_memset((char*)data_info, 0, total_len + 1);

        protocol_entrypt_type = aiio_GetProtocolEntryptType(data, data_len);
        aiio_log_d("protocol_entrypt_type = %d \r\n", protocol_entrypt_type);

        ret = aiio_GetProtocolEntryptKey(data, data_len, protocol_entrypt_type, &protocol_event);
        if (ret < 0)
        {
            aiio_log_e("hard err \r\n");
            protocol_event.event = AIIO_BLE_DATA_HARD_ERR_EVENT;
            if (data_parse_cb)
            {
                data_parse_cb(&protocol_event);
            }
            aiio_DestroyBleData();
            return AIIO_BLE_ACK_HARD_ERR;
        }

        if (protocol_event.entrypt_key)
        {
            // aiio_HexPrint("entrypt key",(uint8_t *)protocol_event.entrypt_key, aiio_strlen(protocol_event.entrypt_key));
        }

        ret = aiio_GetProtocolData(data, data_len, protocol_entrypt_type, PROTOCOL_DATA_POSITION + DATA_POSITION, data_info, len);
        if (ret < 0)
        {
            aiio_log_e("entrypt type err \r\n");
            protocol_event.event = AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT;
            if (data_parse_cb)
            {
                data_parse_cb(&protocol_event);
            }
            aiio_DestroyBleData();
            return AIIO_BLE_ACK_HARD_ERR;
        }
        // aiio_HexPrint("data info",(uint8_t *)data_info, len);

        if (protocol_entrypt_type == PROTOCOL_ENTRYPT_NONE)
        {
            aiio_log_i("data_info = %s \r\n", data_info);
            ret = aiio_JsonProcotolDataParse(data_info, total_len, &protocol_event);
            if (ret < 0)
            {
                aiio_log_e("protocol data parse err \r\n");
                protocol_event.event = AIIO_BLE_DATA_PARSE_FAIL_EVENT;
                protocol_event.entrypt_type = PROTOCOL_ENTRYPT_NONE;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_TYPE_ERR;
            }
            else
            {
                aiio_log_e("protocol data parse ok \r\n");
                protocol_event.event = AIIO_BLE_DATA_PARSE_OK_EVENT;
                protocol_event.entrypt_type = PROTOCOL_ENTRYPT_NONE;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_OK;
            }
        }
        else if (protocol_entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)
        {
            // aiio_HexPrint("entrypt key",(uint8_t *)protocol_event.entrypt_key, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN);
            // aiio_HexPrint("IV",(uint8_t *)IV, aiio_strlen((char *)IV));
            AES_init_ctx_iv(&ctx, (uint8_t*)protocol_event.entrypt_key, IV);
            AES_CBC_decrypt_buffer(&ctx, data_info, total_len);   // Start decrypting data
            aiio_log_i("data_info = %s \r\n", data_info);
            ret = aiio_JsonProcotolDataParse(data_info, total_len, &protocol_event);
            if (ret < 0)
            {
                aiio_log_e("protocol data parse err \r\n");
                protocol_event.event = AIIO_BLE_DATA_PARSE_FAIL_EVENT;
                protocol_event.entrypt_type = PROTOCOL_ENTRYPT_AES_CBC_128;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_TYPE_ERR;
            }
            else
            {
                aiio_log_i("protocol data parse ok \r\n");
                protocol_event.event = AIIO_BLE_DATA_PARSE_OK_EVENT;
                protocol_event.entrypt_type = PROTOCOL_ENTRYPT_AES_CBC_128;
                if (data_parse_cb)
                {
                    data_parse_cb(&protocol_event);
                }
                aiio_DestroyBleData();
                return AIIO_BLE_ACK_OK;
            }
        }
        else
        {
            aiio_log_e("entrypt type err \r\n");
            protocol_event.event = AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT;
            protocol_event.entrypt_type = PROTOCOL_ENTRYPT_UNKNOW;
            if (data_parse_cb)
            {
                data_parse_cb(&protocol_event);
            }
            aiio_DestroyBleData();
            return AIIO_BLE_ACK_HARD_ERR;
        }
    }

    return AIIO_BLE_ACK_OK;
}




/**
 * @brief   Store the data in a protocol format
 */
static int aiio_PacketProtocolPositionData(char* packet_data, uint32_t packet_data_len, uint8_t position, uint8_t* data, uint32_t data_len)
{
    int len = 0;

    if (packet_data == NULL || data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (position >= packet_data_len)
    {
        aiio_log_e("position overflow err \r\n");
        return AIIO_OVERFLOW_ERR;
    }

    len = packet_data_len - position;
    if (data_len >= len)
    {
        aiio_strncpy(&packet_data[position], (char*)data, len);
    }
    else
    {
        len = data_len;
        aiio_strncpy(&packet_data[position], (char*)data, len);
    }
    // aiio_log_i("len = %d \r\n", len);

    return len;
}



static int aiio_PacketDivideProtocolData(char* packet_data, uint32_t packet_data_len, uint8_t frame, aiio_entrypt_type_t entrypt_type, char* entrypt_key, char* data, uint16_t data_len)
{
    uint16_t crcx = 0;
    uint16_t  len = 0;
    uint8_t  dataBuff = 0;
    int     protocol_data_len = 0;
    int     ret = 0;
    struct  AES_ctx ctx;
    uint8_t* aes_pro_data = NULL;
    uint16_t aes_remain_data_len = 0;
    uint16_t aes_data_len = 0;
    uint16_t aes_total_data_len = 0;

    if (packet_data == NULL || data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if (entrypt_type != PROTOCOL_ENTRYPT_NONE && entrypt_key == NULL)
    {
        aiio_log_e("entrypt key err \r\n");
        return AIIO_PARAM_ERR;
    }

    dataBuff = AIIO_PROTOCOL_HEAD_FLAG;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_HEAD_POSITION, &dataBuff, sizeof(dataBuff));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    protocol_data_len += ret;

    dataBuff = frame;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_FRAME_POSITION, &dataBuff, sizeof(dataBuff));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    protocol_data_len += ret;

    dataBuff = PROTOCOL_DIVIDE_TYPE_NONE;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DIVIDE_FLAG_POSITION, &dataBuff, sizeof(dataBuff));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    protocol_data_len += ret;

    if (entrypt_type == PROTOCOL_ENTRYPT_NONE)
    {
        dataBuff = AIIO_DATA_PROTOCOL_HEAD_LEN + data_len;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_LEN_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;


        dataBuff = AIIO_BLE_PROCOTOL_VERSION;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_VERSION_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        packet_data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION] = (data_len & 0xff00) >> 8;
        packet_data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION + 1] = (data_len & 0xff);
        protocol_data_len += sizeof(data_len);

        dataBuff = PROTOCOL_ENTRYPT_NONE;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_ENTRYPT_TYPE_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_POSITION, (uint8_t*)data, data_len);
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        crcx = aiio_crc16_xmodem((uint8_t*)packet_data, protocol_data_len);
        aiio_log_d("crcx = 0x%02x \r\n", crcx);
        packet_data[PROTOCOL_DATA_POSITION + DATA_CRC_POSITION + data_len] = (crcx & 0xff00) >> 8;
        packet_data[PROTOCOL_DATA_POSITION + DATA_CRC_POSITION + data_len + 1] = (crcx & 0xff);
        protocol_data_len += AIIO_PROTOCOL_CRC_LEN;

    }
    else if (entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)
    {
        aes_remain_data_len = data_len % 16;
        if (aes_remain_data_len != 0)
        {
            aes_data_len = (data_len / 16) * 16 + 16;
            aiio_log_i("aes_data_len = %d \r\n", aes_data_len);
        }
        else
        {
            aes_data_len = (data_len / 16) * 16;
            aiio_log_i("aes_data_len = %d \r\n", aes_data_len);
        }

        aes_pro_data = (uint8_t*)aiio_os_malloc(aes_data_len + 1);
        if (aes_pro_data == NULL)
        {
            aiio_log_e("aiio_os_malloc fail \r\n");
            return AIIO_MALLOC_FAIL;
        }
        aiio_memset((char*)aes_pro_data, 0, aes_data_len + 1);
        aiio_strncpy((char*)aes_pro_data, data, data_len);

        AES_init_ctx_iv(&ctx, (uint8_t*)entrypt_key, IV);
        AES_CBC_encrypt_buffer(&ctx, aes_pro_data, aes_data_len);   // 开始加密数据


        dataBuff = AIIO_DATA_PROTOCOL_HEAD_LEN + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + aes_data_len;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_LEN_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;


        dataBuff = AIIO_BLE_PROCOTOL_VERSION;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_VERSION_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        packet_data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION] = (aes_data_len & 0xff00) >> 8;
        packet_data[PROTOCOL_DATA_POSITION + DATA_TOTAL_DATA_LEN_POSITION + 1] = (aes_data_len & 0xff);
        protocol_data_len += sizeof(aes_data_len);

        dataBuff = PROTOCOL_ENTRYPT_AES_CBC_128;
        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_ENTRYPT_TYPE_POSITION, &dataBuff, sizeof(dataBuff));
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_ENTRYPT_KEY_POSITION, (uint8_t*)entrypt_key, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN);
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;

        ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, PROTOCOL_DATA_POSITION + DATA_POSITION + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN, aes_pro_data, aes_data_len);
        if (ret < 0)
        {
            aiio_log_e("protocol packet err \r\n");
            return ret;
        }
        protocol_data_len += ret;
        // aiio_HexPrint("ble packet data", (uint8_t *)packet_data, protocol_data_len);

        crcx = aiio_crc16_xmodem((uint8_t*)packet_data, protocol_data_len);
        aiio_log_d("crcx = 0x%02x \r\n", crcx);
        packet_data[PROTOCOL_DATA_POSITION + DATA_CRC_POSITION + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + aes_data_len] = (crcx & 0xff00) >> 8;
        packet_data[PROTOCOL_DATA_POSITION + DATA_CRC_POSITION + AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + aes_data_len + 1] = (crcx & 0xff);
        protocol_data_len += AIIO_PROTOCOL_CRC_LEN;
    }
    else
    {
        aiio_log_e("entrypt type  err \r\n");
        return AIIO_PROTOCOL_ERROR;
    }

    aiio_log_i("packet response data[%d] \r\n", protocol_data_len);
    // for(uint8_t i = 0; i < protocol_data_len; i++)
    // {
    //     aiio_log_d("%02x ", packet_data[i]);
    // }
    // aiio_log_d("\r\n");

    if (aes_pro_data)
    {
        aiio_os_free(aes_pro_data);
    }

    return protocol_data_len;
}



int aiio_PacketBleResponseDivideData(char* packet_data, uint32_t packet_data_len, uint8_t frame, aiio_entrypt_type_t entrypt_type, char* entrypt_key, char* deviceid, int8_t error_code)
{
    cJSON* json_root = NULL;
    cJSON* json_data = NULL;
    char* json_str = NULL;
    int     err = 0;

    if (packet_data == NULL || deviceid == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }


    json_root = cJSON_CreateObject();
    if (!json_root)
    {
        aiio_log_e("json create object fail \r\n");
        return -1;
    }

    json_data = cJSON_CreateObject();
    if (!json_data)
    {
        aiio_log_e("json create object fail \r\n");
        cJSON_Delete(json_root);
        return -1;
    }

    aiio_log_i("error_code = %d \r\n", error_code);
    cJSON_AddNumberToObject(json_root, PROTOCOL_STR_CODE, error_code);
    cJSON_AddStringToObject(json_data, DEVICE_INFO_STR_DEVICE_ID, deviceid);
    cJSON_AddItemToObject(json_root, DEIVCE_INFO_STR_DATA, json_data);

    json_str = cJSON_PrintUnformatted(json_root);
    aiio_log_i("json_str = %s \r\n", json_str);

    err = aiio_PacketDivideProtocolData(packet_data, packet_data_len, frame, entrypt_type, entrypt_key, json_str, aiio_strlen(json_str));

    cJSON_Delete(json_root);
    cJSON_free(json_str);

    return err;
}



int aiio_PacketBleResponseDivideAckData(char* packet_data, uint32_t packet_data_len, uint8_t frame, int8_t error_code)
{
    uint16_t crcx = 0;
    uint8_t  data = 0;
    int     data_len = 0;
    int     ret = 0;

    if (packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }


    data = AIIO_PROTOCOL_HEAD_DIVIDE_FLAG;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, ACK_PROTOCOL_HEAD_POSITION, &data, sizeof(data));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    data_len += ret;

    data = frame;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, ACK_PROTOCOL_FRAM_POSITION, &data, sizeof(data));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    data_len += ret;

    data = sizeof(error_code);
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, ACK_PROTOCOL_DATA_LEN_POSITION, &data, sizeof(data));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    data_len += ret;

    data = error_code;
    ret = aiio_PacketProtocolPositionData(packet_data, packet_data_len, ACK_PROTOCOL_DATA_POSITION, &data, sizeof(data));
    if (ret < 0)
    {
        aiio_log_e("protocol packet err \r\n");
        return ret;
    }
    data_len += ret;

    crcx = aiio_crc16_xmodem((uint8_t*)packet_data, data_len);
    aiio_log_d("crcx = 0x%02x \r\n", crcx);
    packet_data[ACK_PROTOCOL_CRC_POSITION] = (crcx & 0xff00) >> 8;
    packet_data[ACK_PROTOCOL_CRC_POSITION + 1] = (crcx & 0xff);
    data_len += AIIO_PROTOCOL_CRC_LEN;

    // aiio_HexPrint("packet ack data", (uint8_t *)packet_data, data_len);

    return data_len;
}



