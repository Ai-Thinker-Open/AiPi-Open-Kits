/**
 * @brief   Define packing protocol interface of cloud communication
 * 
 * @file    aiio_protocol_packet.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is decribing about the protocol of cloud communication, mainly define packing protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Packing protocol interface of cloud communication
 */
#include "aiio_protocol_packet.h"
#include "aiio_protocol_code.h"
#include "aiio_log.h"
#include "aiio_error.h"
#include "aiio_ctime.h"
#include "aiio_crclib.h"
#include "aiio_aes.h"
#include "aiio_uuid4.h"
#include "cJSON.h"
#include "aiio_stdio.h"
#include "aiio_os_port.h"


static uint8_t  IsQuery = DEVICE_REPORT_TYPE;




static cJSON *aiio_PacketHeadJson(aiio_head_protocol_t *head_protocol)
{
    cJSON  *json_header = NULL;

    if(head_protocol == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    json_header = cJSON_CreateObject();
    if (!json_header)
    {
        aiio_log_e("json create object fail \r\n");
        return NULL;
    }

    aiio_log_i("head_protocol->ts = %d \r\n", head_protocol->ts);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_NS, head_protocol->ns);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_NAME, head_protocol->name);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_MID, head_protocol->mid);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_GID, head_protocol->gid);
    cJSON_AddNumberToObject(json_header, PROTOCOL_STR_TS, head_protocol->ts);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_VER, head_protocol->ver);
    cJSON_AddStringToObject(json_header, PROTOCOL_STR_FROM, head_protocol->from);

    return json_header;
}




static char *aiio_WanPacketData(aiio_head_protocol_t *head_protocol, cJSON *payload)
{
    cJSON  *json_root = NULL;
    cJSON  *json_header = NULL;
    char  *json_str = NULL;

    if(head_protocol == NULL)
    {
        aiio_log_e("param err \r\n");
        if(payload)
        {
            cJSON_Delete(payload);
        }
        return NULL;
    }

    json_root = cJSON_CreateObject();
    if (!json_root)
    {
        aiio_log_e("json create object fail \r\n");
        if(payload)
        {
            cJSON_Delete(payload);
        }
        return NULL;
    }

    json_header = aiio_PacketHeadJson(head_protocol);
    if (!json_header)
    {
        aiio_log_e("json create object fail \r\n");
        cJSON_Delete(json_root);
        if(payload)
        {
            cJSON_Delete(payload);
        }
        return NULL;
    }

    cJSON_AddItemToObject(json_root, PROTOCOL_STR_HEADER, json_header);

    if(payload != NULL)
    {
        cJSON_AddItemToObject(json_root, PROTOCOL_STR_PAYLOAD, payload);
    }

    json_str = cJSON_PrintUnformatted(json_root);
    if(json_str == NULL)
    {
        aiio_log_e("json create str fail \r\n");
        cJSON_Delete(json_root);
        cJSON_Delete(json_header);
        if(payload)
        {
            cJSON_Delete(payload);
        }
        return NULL;
    }
    // aiio_log_i("json_str = %s \r\n", json_str);

    cJSON_Delete(json_root);

    return json_str;
}


static cJSON *aiio_PacketDeviceInfoToJson(aiio_device_info_t *device_info)
{
    cJSON  *json_payload = NULL;

    if(device_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return NULL;
    }

    if(device_info->deviceid)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_DEVICE_ID, device_info->deviceid);
    }
    
    if(device_info->secrtkey)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_SECRTKEY, device_info->secrtkey);
    }
    
    if(device_info->productid)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_PRODUCT_KEY, device_info->productid);
    }

    if(device_info->fwVer)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_FW_VER, device_info->fwVer);
    }

    if(device_info->mcuVer)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_MCU_VER, device_info->mcuVer);
    }

    if(device_info->devType)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_HW_VER, device_info->devType);
    }

    cJSON_AddNumberToObject(json_payload, DEVICE_INFO_STR_MEMFREE, device_info->memfree);

    if(device_info->mac)
    {
        cJSON_AddStringToObject(json_payload, DEVICE_INFO_STR_MAC, device_info->mac);
    }
    
    return json_payload;
}


static cJSON *aiio_PacketApInfoToJson(aiio_wifi_info_t *wifi_info)
{
    cJSON  *json_ap = NULL;

    if(wifi_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    json_ap = cJSON_CreateObject();
    if (!json_ap)
    {
        aiio_log_e("json create object fail \r\n");
        return NULL;
    }

    if(wifi_info->ssid)
    {
        cJSON_AddStringToObject(json_ap, WIFI_STR_SSID, wifi_info->ssid);
    }

    if(wifi_info->bssid)
    {
        cJSON_AddStringToObject(json_ap, WIFI_STR_BSSID, wifi_info->bssid);
    }
    
    cJSON_AddNumberToObject(json_ap, WIFI_STR_RSSI, wifi_info->rssi);

    return json_ap;
}


static cJSON *aiio_PacketNetifInfoToJson(aiio_wifi_info_t *wifi_info)
{
    cJSON  *json_netif = NULL;

    if(wifi_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    json_netif = cJSON_CreateObject();
    if (!json_netif)
    {
        aiio_log_e("json create object fail \r\n");
        return NULL;
    }

    if(wifi_info->ip)
    {
        cJSON_AddStringToObject(json_netif, WIFI_STR_LOCAL_IP, wifi_info->ip);
    }
    
    if(wifi_info->mask)
    {
        cJSON_AddStringToObject(json_netif, WIFI_STR_MASK, wifi_info->mask);
    }
    
    if(wifi_info->gw)
    {
        cJSON_AddStringToObject(json_netif, WIFI_STR_GW, wifi_info->gw);
    }
    
    return json_netif;
}




int32_t aiio_PacketDeviceInfoData(char *packet_data, uint32_t packet_data_len, aiio_device_info_t *device_info, aiio_wifi_info_t *wifi_info, char *msgMid, char *from, bool token_status, uint32_t current_stamp)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    cJSON  *json_ap = NULL;
    cJSON  *json_netif = NULL;
    char *mid_pt = NULL;


    if(packet_data == NULL || device_info == NULL || wifi_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = aiio_PacketDeviceInfoToJson(device_info);
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }

    json_ap = aiio_PacketApInfoToJson(wifi_info);
    if (!json_ap)
    {
        aiio_log_e("json create object fail \r\n");
        cJSON_Delete(json_payload);
        return AIIO_CREATE_JSON_FAIL;
    }

    json_netif = aiio_PacketNetifInfoToJson(wifi_info);
    if (!json_netif)
    {
        aiio_log_e("json create object fail \r\n");
        cJSON_Delete(json_ap);
        cJSON_Delete(json_payload);
        return AIIO_CREATE_JSON_FAIL;
    }

    if(token_status)
    {
        cJSON_AddStringToObject(json_payload, PROTOCOL_STR_TOKEN, device_info->token);
    }

    cJSON_AddItemToObject(json_payload, WIFI_STR_AP, json_ap);
    cJSON_AddItemToObject(json_payload, WIFI_STR_NETIF, json_netif);

    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_INFO, aiio_strlen(STR_NS_IOT_DEVICE_INFO));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_INFO, aiio_strlen(STR_NAME_INFO));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



int32_t aiio_PacketOfflineData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }

    cJSON_AddStringToObject(json_payload, PROTOCOL_STR_ONLINE_STATUS, PROTOCOL_STR_OFFLINE);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_REPORT, aiio_strlen(STR_NS_IOT_DEVICE_REPORT));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_ONLINE, aiio_strlen(STR_NAME_ONLINE));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



int32_t aiio_PacketOnlineData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddStringToObject(json_payload, PROTOCOL_STR_ONLINE_STATUS, PROTOCOL_STR_ONLINE);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_REPORT, aiio_strlen(STR_NS_IOT_DEVICE_REPORT));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_ONLINE, aiio_strlen(STR_NAME_ONLINE));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}




int32_t aiio_PacketConfigAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_CONFIG, aiio_strlen(STR_NS_IOT_DEVICE_CONFIG));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_CONFIG, aiio_strlen(STR_NAME_CONFIG));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}





int32_t aiio_PacketControlAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);

    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_CONTROL, aiio_strlen(STR_NS_IOT_DEVICE_CONTROL));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_CONTROL, aiio_strlen(STR_NAME_CONTROL));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



int32_t aiio_PacketAckData(char *packet_data, uint32_t packet_data_len, char *ns, char *name, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL || ns == NULL || name == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);

    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, ns, aiio_strlen(ns));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, name, aiio_strlen(name));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



int32_t aiio_PacketRestoreAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_CONTROL, aiio_strlen(STR_NS_IOT_DEVICE_CONTROL));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_RESTORE, aiio_strlen(STR_NAME_RESTORE));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}


int32_t aiio_PacketRebootAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;


    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_CONTROL, aiio_strlen(STR_NS_IOT_DEVICE_CONTROL));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_REBOOT, aiio_strlen(STR_NAME_REBOOT));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}




int32_t aiio_PacketOtaAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code)
{
    cJSON  *json_payload = NULL;
    int32_t data_str_len = 0;
    char *data_str = NULL;
    aiio_head_protocol_t head_protocol = {0};
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }


    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, code);


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_UPGRADE, aiio_strlen(STR_NS_IOT_DEVICE_UPGRADE));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_OTA_INFO, aiio_strlen(STR_NAME_OTA_INFO));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



static cJSON * aiio_PacketOtaRateJson(aiio_ota_info_t *ota_info)
{
    cJSON  *json_payload = NULL;

    if(ota_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return NULL;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return NULL;
    }

    cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_CODE, ota_info->code);

    if(ota_info->rate != 0xffffffff)
    {
        cJSON_AddNumberToObject(json_payload, PROTOCOL_STR_PROGRESS, ota_info->rate);
    }

    if(ota_info->otaStatus != NULL)
    {
        cJSON_AddStringToObject(json_payload, PROTOCOL_STR_OTA_STATUS, ota_info->otaStatus);
    }

    if(ota_info->OtaVer != NULL)
    {
        cJSON_AddStringToObject(json_payload, PROTOCOL_STR_OTA_VER, ota_info->OtaVer);
    }

    if(ota_info->PubId != NULL)
    {
        cJSON_AddStringToObject(json_payload, PROTOCOL_STR_PUBID, ota_info->PubId);
    }

    return json_payload;
}


int32_t aiio_PacketReportOtaRateData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, aiio_ota_info_t *ota_info)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    cJSON  *json_payload = NULL;
    char *mid_pt = NULL;


    if(packet_data == NULL || ota_info == NULL)
    {
        aiio_log_e("get profile info err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_payload = aiio_PacketOtaRateJson(ota_info);
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }


    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_UPGRADE, aiio_strlen(STR_NS_IOT_DEVICE_UPGRADE));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_OTA_PROGRESS, aiio_strlen(STR_NAME_OTA_PROGRESS));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



int32_t aiio_PacketRequesUTCData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    aiio_head_protocol_t head_protocol = {0};
    char *mid_pt = NULL;

    if(packet_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_UTC, aiio_strlen(STR_NS_IOT_DEVICE_UTC));
    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));
    aiio_strncpy(head_protocol.name, STR_NAME_UTC, aiio_strlen(STR_NAME_UTC));
    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    aiio_log_i("head_protocol->ts = %d \r\n", head_protocol.ts);
    aiio_log_i("current_stamp = %d \r\n", current_stamp);
    data_str = aiio_WanPacketData(&head_protocol, NULL);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}



void aiio_SetPacketReportAttributesType(uint8_t type)
{
    IsQuery = type;
}




int32_t aiio_PacketReportAttrbutesData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, char *device_att)
{
    char *data_str = NULL;
    int32_t data_str_len = 0;
    cJSON  *json_payload = NULL;
    cJSON  *json_device = NULL;
    aiio_head_protocol_t head_protocol = {0};
    char *mid_pt = NULL;

    if(packet_data == NULL || device_att == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    json_device = cJSON_Parse(device_att);
    if(json_device == NULL)
    {
        aiio_log_e("json parse err \r\n");
        return AIIO_JSON_PARSE_FAIL;
    }

    json_payload = cJSON_CreateObject();
    if (!json_payload)
    {
        aiio_log_e("json create object fail \r\n");
        return AIIO_CREATE_JSON_FAIL;
    }

    cJSON_AddItemToObject(json_payload, PROTOCOL_STR_DEVICE, json_device);

    if(msgMid == NULL)
    {
        mid_pt = aiio_GetMessageId();
        aiio_strncpy(head_protocol.mid, mid_pt, aiio_strlen(mid_pt));
    }
    else
    {
        aiio_strncpy(head_protocol.mid, msgMid, aiio_strlen(msgMid));
    }

    if(IsQuery == CLOUD_ALL_QUERY_TYPE)
    {
        aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_REPORT, aiio_strlen(STR_NS_IOT_DEVICE_REPORT));
        aiio_strncpy(head_protocol.name, STR_NAME_QUERY, aiio_strlen(STR_NAME_QUERY));
        IsQuery = DEVICE_REPORT_TYPE;
    }
    else if(IsQuery == CLOUD_CONTROL_TYPE)
    {
        aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_REPORT, aiio_strlen(STR_NS_IOT_DEVICE_REPORT));
        aiio_strncpy(head_protocol.name, STR_NAME_CONTROL, aiio_strlen(STR_NAME_CONTROL));
        IsQuery = DEVICE_REPORT_TYPE;
    }
    else
    {
        aiio_strncpy(head_protocol.ns, STR_NS_IOT_DEVICE_REPORT, aiio_strlen(STR_NS_IOT_DEVICE_REPORT));
        aiio_strncpy(head_protocol.name, STR_NAME_REPORT, aiio_strlen(STR_NAME_REPORT));
    }

    aiio_strncpy(head_protocol.ver, AIIO_DEVICE_IOT_VERSION, aiio_strlen(AIIO_DEVICE_IOT_VERSION));

    if(from)
    {
        aiio_strncpy(head_protocol.from, from, aiio_strlen(from));
    }
    else
    {
        aiio_strncpy(head_protocol.from, PROTOCOL_STR_DEVICE, aiio_strlen(PROTOCOL_STR_DEVICE));
    }
    head_protocol.ts = current_stamp;
    data_str = aiio_WanPacketData(&head_protocol, json_payload);
    if(data_str == NULL)
    {
        aiio_log_e("packet err \r\n");
        return AIIO_PROTOCOL_ERROR;
    }
    aiio_log_i("data_str = %s \r\n", data_str);
    data_str_len = aiio_strlen(data_str);
    if(data_str_len >= packet_data_len)
    {
        data_str_len = packet_data_len;
        aiio_strncpy(packet_data, data_str, data_str_len);
    }
    else
    {
        aiio_strncpy(packet_data, data_str, data_str_len);
    }

    cJSON_free(data_str);

    return data_str_len;
}





