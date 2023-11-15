/**
 * @brief   Define parsing protocol interface of cloud communication
 * 
 * @file    aiio_protocol_packet.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is decribing about the protocol of cloud communication, mainly declare parsing protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Parsing protocol interface of cloud communication
 */
#include "aiio_protocol_parse.h"
#include "aiio_protocol_code.h"
#include "aiio_log.h"
#include "aiio_ctime.h"
#include "aiio_crclib.h"
#include "aiio_aes.h"
#include "aiio_uuid4.h"
#include "cJSON.h"
#include "aiio_stdio.h"
#include "aiio_os_port.h"




static char *aiio_ParseStringJson(cJSON *cjson_root, char *json_key)
{
    cJSON *cjson_object = NULL;
    char  *cjson_value = NULL;

    if(cjson_root == NULL || json_key == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    cjson_object = cJSON_GetObjectItem(cjson_root, json_key);
    if(cjson_object != NULL)
    {
        cjson_value = cJSON_GetStringValue(cjson_object);
        if(cjson_value)
        {
            aiio_log_i("cjson_value = %s \r\n", cjson_value);
            return cjson_value;
        }
        else
        {
            aiio_log_e("cjson_value is NULL \r\n");
            return NULL;
        }
    }
    else
    {
        aiio_log_e("cjson_object is NULL \r\n");
        return NULL;
    }
}



static int aiio_ParseIntJson(cJSON *cjson_root, char *json_key)
{
    cJSON *cjson_object = NULL;
    int   cjson_value = NULL;

    if(cjson_root == NULL || json_key == NULL)
    {
        aiio_log_e("param err \r\n");
        return -1;
    }

    cjson_object = cJSON_GetObjectItem(cjson_root, json_key);
    if(cjson_object != NULL)
    {
        cjson_value = (int)cJSON_GetNumberValue(cjson_object);
        if(cjson_value)
        {
            aiio_log_i("cjson_value = %d \r\n", cjson_value);
            return cjson_value;
        }
        else
        {
            aiio_log_e("cjson_value is NULL \r\n");
            return -1;
        }
    }
    else
    {
        aiio_log_e("cjson_object is NULL \r\n");
        return -1;
    }
}


int32_t aiio_JsonDataParse(char *data, uint16_t data_len, aiio_protocol_payload_cb payload_cb)
{
    cJSON *cjson_root   = NULL;
    cJSON *cjson_header = NULL;
    cJSON *cjson_payload = NULL;

    char *str_value = NULL;
    char *str_payload = NULL;
    int   int_value = 0;
    aiio_head_protocol_t head_protocol = {0};

    if(data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    aiio_log_i("data = %s \r\n", data);
    aiio_log_i("data_len = %d \r\n", data_len);

    cjson_root = cJSON_Parse(data);
    if(cjson_root == NULL)
    {
        aiio_log_e("json parse err \r\n");
        return AIIO_JSON_PARSE_FAIL;
    }

    cjson_header = cJSON_GetObjectItem(cjson_root, PROTOCOL_STR_HEADER);
    if(cjson_header == NULL)
    {
        aiio_log_e("cjson_header is NULL \r\n");
        cJSON_Delete(cjson_root);
        return AIIO_JSON_PARSE_FAIL;
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_NS);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.ns, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.ns = %s \r\n", head_protocol.ns);
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_NAME);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.name, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.name = %s \r\n", head_protocol.name);
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_MID);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.mid, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.mid = %s \r\n", head_protocol.mid);
    }

    int_value = aiio_ParseIntJson(cjson_header, PROTOCOL_STR_TS);
    if(int_value != -1)
    {
        head_protocol.ts = int_value;
        aiio_log_i("head_protocol.ts = %d \r\n", head_protocol.ts);
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_VER);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.ver, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.ver = %s \r\n", head_protocol.ver);
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_GID);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.gid, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.gid = %s \r\n", head_protocol.gid);
    }

    str_value = aiio_ParseStringJson(cjson_header, PROTOCOL_STR_FROM);
    if(str_value != NULL)
    {
        aiio_strncpy(head_protocol.from, str_value, aiio_strlen(str_value));
        aiio_log_i("head_protocol.from = %s \r\n", head_protocol.gid);
    }

    cjson_payload = cJSON_GetObjectItem(cjson_root, PROTOCOL_STR_PAYLOAD);
    if(cjson_payload)
    {
        str_payload = cJSON_PrintUnformatted(cjson_payload);
        if(str_payload)
        {
            aiio_log_i("payload = %s \r\n", str_payload);
            if(payload_cb)
            {
                payload_cb(&head_protocol, str_payload, aiio_strlen(str_payload));
            }
        }
        else
        {
            aiio_log_i("payload is NULL \r\n");
            if(payload_cb)
            {
                payload_cb(&head_protocol, NULL, 0);
            }
        }
    }
    else
    {
        aiio_log_i("cjson_payload is NULL \r\n");
        if(payload_cb)
        {
            payload_cb(&head_protocol, NULL, 0);
        }
    }

    if(str_payload)
    {
        cJSON_free(str_payload);
    }
    cJSON_Delete(cjson_root);

    return AIIO_CODE_OK;
}




int32_t aiio_ParseTimeData(char *data, char *time_buff, uint32_t time_buff_len)
{
    cJSON *cjson_root   = NULL;
    char  *str_value = NULL;
    uint32_t len = 0;

    if(data == NULL || time_buff == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    cjson_root = cJSON_Parse(data);
    if(cjson_root == NULL)
    {
        aiio_log_e("json parse err \r\n");
        return AIIO_JSON_PARSE_FAIL;
    }

    str_value = aiio_ParseStringJson(cjson_root, PROTOCOL_STR_DATE);
    if(str_value == NULL)
    {
        aiio_log_e("get value err \r\n");
        cJSON_Delete(cjson_root);
        return AIIO_JSON_PARSE_FAIL;
    }

    aiio_log_i("str_value = %s \r\n", str_value);
    len = aiio_strlen(str_value);
    if(len >= time_buff_len)
    {
        len = time_buff_len - 1;
        aiio_strncpy(time_buff, str_value, len);
    }
    else
    {
        aiio_strncpy(time_buff, str_value, len);
    }

    cJSON_Delete(cjson_root);

    return AIIO_CODE_OK;
}

