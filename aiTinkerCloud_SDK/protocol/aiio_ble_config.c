/**
 * @brief   Define the application interface of ble distribution network
 * 
 * @file    aiio_ble_config.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        Achieve logic function of ble distribution network by the protocol interface of it
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> The application interface of ble distribute network
 */
#include "aiio_ble_protocol.h"
#include "aiio_ble_config.h"
#include "aiio_protocol_code.h"
#include "aiio_error.h"
#include "aiio_wifi.h"
#include "aiio_ble.h"
#include "aiio_log.h"
#include "platform_bl.h"
#include "aiio_stdio.h"
#include "aiio_os_port.h"




#define BLE_MIN_INTV    200
#define BLE_MAX_INTV    220
#define BLE_LATENCY     0
#define BLE_TIMEOUT     400

#define BLE_ADV_MIN    60
#define BLE_ADV_MAX    60


#define ADV_FLAG_LEN_BYTES                  (1)
#define ADV_FLAG_TYPE_BYTES                 (1)
#define ADV_FLAG_VALUE_BYTES                (1)
#define ADV_NAME_LEN_BYTES                  (1)
#define ADV_NAME_TYPE_BYTES                 (1)


#define ADV_DATA_MAX_LEN                    (31)

#define QUEUE_MAX_SIZE                      (5)


typedef enum
{
    ADV_NAME_LEN_POSITIONM = 0,
    ADV_NAME_TYPE_POSITION,
    ADV_NAME_VALUE_POSITION
}aiio_adv_data_structure_t;

typedef enum 
{
    QUEUE_INVAILD_EVENT = 0,
    QUEUE_REV_BLE_DISCONNECTED,
    QUEUE_REV_BLE_DATA_EVENT
}aiio_queue_event_t;

static aiio_ble_default_handle_t srv_handle;

static aiio_ble_config_t ble_config = {
    .conn_intv = {
        .min_interval = BLE_MIN_INTV,
        .max_interval = BLE_MAX_INTV,
        .latency = BLE_LATENCY,
        .timeout = BLE_TIMEOUT,
    },
};

static aiio_ble_default_server_t ble_server = {
    .server_uuid[0] = AIIO_BLE_UUID_DECLARE_16(0xff, 0xff),
    .tx_char_uuid[0] = AIIO_BLE_UUID_DECLARE_16(0xff, 0x02),
    .rx_char_uuid[0] = AIIO_BLE_UUID_DECLARE_16(0xff, 0x01),

    .server_uuid[1] = AIIO_BLE_UUID_DECLARE_16(0xb0, 0x01),
    .tx_char_uuid[1] = AIIO_BLE_UUID_DECLARE_16(0xb0, 0x02),
    .rx_char_uuid[1] = AIIO_BLE_UUID_DECLARE_16(0xb0, 0x03),
};



static aiio_ble_adv_param_t param = {
    .conn_mode = AIIO_BLE_CONN_MODE_UND,
    /* BL602 unsupport */
    .disc_mode = AIIO_BLE_DISC_MODE_GEN,
    .interval_min = BLE_ADV_MIN,
    .interval_max = BLE_ADV_MAX,
};

typedef struct 
{
    char *queue_data;
    uint16_t queue_data_len;
    aiio_queue_event_t evt;
}aiio_ble_config_queue_t;




static uint8_t adv_structure_remain_len = (ADV_NAME_LEN_BYTES +  ADV_NAME_TYPE_BYTES);
static aiio_ble_iot_config_t  ble_iot_config = {0};
static char *ble_adv_name = NULL;
static uint8_t last_frame = 0xff;
static aiio_ble_data_t  ble_config_data = {0};
static aiio_entrypt_type_t entrypt_type = PROTOCOL_ENTRYPT_UNKNOW;
static char             *entrypt_key = NULL;
static aiio_ble_conn_t  ble_conn = NULL;
static aiio_os_thread_handle_t            ble_config_task_handle;
static aiio_os_queue_handle_t             ble_config_queue_handle = NULL;
static bool         ble_config_init = false;
static bool         ble_config_task_run = false;

static int aiio_ble_start_adv_name(char *adv_data, uint16_t adv_data_len);
static int aiio_ble_config_response_ack(int8_t error_code);
static void aiio_ble_data_parse(aiio_ble_protocol_event_t *event);
static void ble_salve_init(void);
static void aiio_ble_iot_destroy(void);


// static void aiio_HexPrint(char *str, uint8_t *data, uint16_t data_len)
// {
//     if(str)
//     {
//         aiio_log_d("\n%s: \r\n", str);
//     }
//     for(uint16_t n = 1; n <= data_len; n++)
//     {
//         printf("%02x ", data[n-1]);
//         if(n  % 16 == 0)
//         {
//             printf("\r\n");
//         }
//     }
//     printf("\r\n");
//     printf("\r\n");
// }



static void aiio_ble_config_task(void *pv)
{
    aiio_ble_config_queue_t ble_config_queue_data = {0};

    ble_config_task_run = true;
    while (ble_config_task_run)
    {
        if(aiio_os_queue_recv(ble_config_queue_handle, &ble_config_queue_data, 100) != AIIO_OK)
        {
            aiio_os_tick_dealy(aiio_os_ms2tick(50));
            continue;
        }

        switch (ble_config_queue_data.evt)
        {
            case QUEUE_INVAILD_EVENT:
            {
                aiio_log_d("QUEUE_INVAILD_EVENT \r\n");
            }
            break;

            case QUEUE_REV_BLE_DISCONNECTED:
            {
                aiio_log_d("QUEUE_REV_BLE_DISCONNECTED \r\n");
                aiio_ble_iot_data_t config_iot_data = {
                    .event = AIIO_BLE_DISCONNECED,
                };
                if(ble_iot_config.aiio_ble_iot_config_cb)
                {
                    ble_iot_config.aiio_ble_iot_config_cb(&config_iot_data);
                }

            }
            break;

            case QUEUE_REV_BLE_DATA_EVENT:
            {
                aiio_log_d("QUEUE_REV_BLE_DATA_EVENT \r\n");
                if(ble_config_queue_data.queue_data)
                {
                    aiio_log_d("queue_data = %s \r\n", ble_config_queue_data.queue_data);
                    // aiio_HexPrint("receive queue data", (uint8_t *)ble_config_queue_data.queue_data, ble_config_queue_data.queue_data_len);
                    aiio_log_d("queue_data_len = %d \r\n", ble_config_queue_data.queue_data_len);
                    aiio_ParseBleProtocolData((uint8_t *)ble_config_queue_data.queue_data, ble_config_queue_data.queue_data_len, aiio_ble_data_parse);
                    aiio_os_free(ble_config_queue_data.queue_data);
                    aiio_memset(&ble_config_queue_data, 0, sizeof(aiio_ble_config_queue_t));
                }
            }
            break;
            
            default:
                aiio_log_e("not find evt \r\n");
                break;
        }
    }


    aiio_os_queue_delete(ble_config_queue_handle);
    ble_config_queue_handle = NULL;

    aiio_log_d("delete ble config task \r\n");
    aiio_os_thread_delete(ble_config_task_handle);
}


static void aiio_ble_data_parse(aiio_ble_protocol_event_t *event)
{
    last_frame = event->frame;
    switch (event->event)
    {
        case AIIO_BLE_DATA_CRC_ERR_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_CRC_ERR_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_CRC_ERR);
        }
        break;

        case AIIO_BLE_DATA_PROTOCOL_ERR_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_PROTOCOL_ERR_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_TYPE_ERR);
        }
        break;

        case AIIO_BLE_DATA_PROTOCOL_VER_ERR_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_PROTOCOL_VER_ERR_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_VER_ERR);
        }
        break;

        case AIIO_BLE_DATA_HARD_ERR_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_HARD_ERR_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_HARD_ERR);
        }
        break;

        case AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_ENTRYPT_TYPE_UNKNOW_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_TYPE_ERR);
        }
        break;

        case AIIO_BLE_DATA_FRAME_ERR_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_FRAME_ERR_EVENT \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_FRAME_ERR);
        }
        break;

        case AIIO_BLE_DATA_VERIFICATE_OK:
        {
            aiio_log_i("AIIO_BLE_DATA_VERIFICATE_OK \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_OK);
        }
        break;

        case AIIO_BLE_DATA_PARSE_FAIL_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_PARSE_FAIL_EVENT \r\n");

            entrypt_type = event->entrypt_type;
            aiio_log_i("entrypt_type = %d \r\n", entrypt_type);

            aiio_ble_iot_data_t config_iot_data = {
                .ble_data = event->ble_data,
                .entrypt_key = event->entrypt_key,
                .event = AIIO_BLE_CONFIG_FAIL,
            };
            if(ble_iot_config.aiio_ble_iot_config_cb)
            {
                ble_iot_config.aiio_ble_iot_config_cb(&config_iot_data);
            }

            aiio_ble_config_response_status(AIIO_BLE_CODE_FAIL);
        }
        break;

        case AIIO_BLE_DATA_PARSE_OK_EVENT:
        {
            aiio_log_i("AIIO_BLE_DATA_PARSE_OK_EVENT \r\n");
            entrypt_type = event->entrypt_type;
            aiio_log_i("entrypt_type = %d \r\n", entrypt_type);

            if(entrypt_type == PROTOCOL_ENTRYPT_AES_CBC_128)                    /* when ble distribution network data is encrypted, the current encryption key needs to be stored for Lan network using*/
            {
                entrypt_key = aiio_os_malloc(AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + 1);
                if(entrypt_key && event->entrypt_key)
                {
                    aiio_memset(entrypt_key, 0, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN + 1);
                    aiio_strncpy(entrypt_key, event->entrypt_key, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN);
                    // aiio_HexPrint("entrypt_key", (uint8_t *)entrypt_key, AIIO_AES_CRC_128_ENTRYPT_KEY_LEN);
                }
                else
                {
                    if(entrypt_key)
                    {
                        aiio_os_free(entrypt_key);
                        entrypt_key = NULL;
                    }
                }
            }
            if(event->ble_data->ssid)
            {
                aiio_log_d("ssid = %s \r\n", event->ble_data->ssid);
            }
            if(event->ble_data->passwd)
            {
                aiio_log_d("passwd = %s \r\n", event->ble_data->passwd);
            }
            if(event->ble_data->mqttip)
            {
                aiio_log_d("mqttip = %s \r\n", event->ble_data->mqttip);
            }
            aiio_ble_iot_data_t config_iot_data = {
                .ble_data = event->ble_data,
                .entrypt_key = event->entrypt_key,
                .event = AIIO_BLE_CONFIG_OK,
            };
            if(ble_iot_config.aiio_ble_iot_config_cb)
            {
                ble_iot_config.aiio_ble_iot_config_cb(&config_iot_data);
            }

            aiio_ble_config_response_status(AIIO_BLE_CODE_DATA_REV_OK);
        }
        break;
        
        default:
            aiio_log_e("not find event \r\n");
            aiio_ble_config_response_ack(AIIO_BLE_ACK_TYPE_ERR);
            break;
    }
}


static void aiio_ble_cb(aiio_ble_evt_t *event)
{
    switch (event->type) 
    {
        case AIIO_BLE_EVT_DISCONNECT:
        {
            aiio_log_w("ble disconn");
            aiio_ble_config_queue_t ble_config_queue_data = {0};
            ble_config_queue_data.evt = QUEUE_REV_BLE_DISCONNECTED;
            if(ble_config_queue_handle && ble_config_task_run)
            {
                if (aiio_os_queue_send(ble_config_queue_handle, &ble_config_queue_data, 0) != AIIO_OK)
                {
                    aiio_log_i("queue send failed\r\n");
                }
            }
            ble_conn = event->disconnect.conn;
            aiio_log_i("ble_conn = %p \r\n", ble_conn);
            ble_conn = NULL;
            // if(ble_adv_name)
            // {
            //     aiio_ble_start_adv_name(ble_adv_name, aiio_strlen(ble_adv_name));
            // }
        }
        break;

        case AIIO_BLE_EVT_CONNECT:
        {
            aiio_log_i("ble conn");
            ble_conn = event->connect.conn;
            aiio_log_i("ble_conn = %p \r\n", ble_conn);
        }
        break;

        case AIIO_BLE_EVT_MTU:
        {
            aiio_log_i("mtu update:%d", event->mtu.mtu);
            ble_conn = event->mtu.conn;
            aiio_log_i("ble_conn = %p \r\n", ble_conn);
        }
        break;

        case AIIO_BLE_EVT_DATA:
        {
            ble_conn = event->data.conn;
            aiio_log_i("ble_conn = %p \r\n", ble_conn);
            aiio_log_i("data down handle:%d len:%d", event->data.handle, event->data.length);
            // aiio_HexPrint("ble receive data", (uint8_t *)event->data.data, event->data.length);
            aiio_ble_config_queue_t ble_config_queue_data = {0};
            ble_config_queue_data.queue_data = aiio_os_malloc(event->data.length + 1);
            if(ble_config_queue_data.queue_data)
            {
                aiio_memset(ble_config_queue_data.queue_data, 0, event->data.length + 1);
                aiio_strncpy(ble_config_queue_data.queue_data, (char *)event->data.data, event->data.length);
                ble_config_queue_data.queue_data_len = event->data.length;
                ble_config_queue_data.evt = QUEUE_REV_BLE_DATA_EVENT;

                // aiio_HexPrint("ble queue data", (uint8_t *)ble_config_queue_data.queue_data, ble_config_queue_data.queue_data_len);
                if(ble_config_queue_handle && ble_config_task_run)
                {
                    if (aiio_os_queue_send(ble_config_queue_handle, &ble_config_queue_data, 0) != AIIO_OK)
                    {
                        aiio_log_i("queue send failed\r\n");
                        aiio_os_free(ble_config_queue_data.queue_data);
                    }
                }
                else
                {
                    aiio_os_free(ble_config_queue_data.queue_data);
                }
            }
        }
        break;

        default:
            aiio_log_w("undef event!");
            break;
    }
}


static void ble_print_mac(void)
{
    uint8_t mac[6];

    aiio_ble_get_mac(mac);
    aiio_log_i("ble mac:%02x:%02x:%02x:%02x:%02x:%02x",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}



static int aiio_ble_config_response_ack(int8_t error_code)
{
    char *packet_data = NULL;
    int err = 0;
    aiio_ble_send_t ble_send_data = {0};

    if(ble_conn == NULL)
    {
        aiio_log_e("ble_conn is NULL \r\n");
        return AIIO_ERROR;
    }

    packet_data = aiio_os_malloc(AIIO_BLE_PROTOCOL_PACKET_MAX_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_ERROR);
    aiio_memset(packet_data, 0, AIIO_BLE_PROTOCOL_PACKET_MAX_LEN);

    last_frame += 1;
    err = aiio_PacketBleResponseDivideAckData(packet_data, AIIO_BLE_PROTOCOL_PACKET_MAX_LEN, last_frame, error_code);
    if(err < 0)
    {
        aiio_log_e("packet ble protocol fail \r\n");
        aiio_os_free(packet_data);
        return AIIO_ERROR;
    }
    aiio_log_i("packet data len = %d \r\n", err);
    // aiio_HexPrint("ble response ack data", (uint8_t *)packet_data, err);

    ble_send_data.type = AIIO_BLE_NOTIFY;
    ble_send_data.data = (uint8_t *)packet_data;
    ble_send_data.length = err;
    ble_send_data.handle = srv_handle.tx_char_handle[0];
    ble_send_data.conn = ble_conn;

    err = aiio_ble_send(&ble_send_data);
    if(err != AIIO_OK)
    {
        aiio_log_e("ble send data fail \r\n");
        aiio_os_free(packet_data);
        return AIIO_ERROR;
    }

    aiio_os_free(packet_data);

    return AIIO_OK;
}


int aiio_ble_config_response_status(int8_t error_code)
{
    char *packet_data = NULL;
    int err = 0;
    aiio_ble_send_t ble_send_data = {0};

    if(ble_conn == NULL || ble_iot_config.deviceid == NULL)
    {
        aiio_log_e("ble_conn is NULL, or device is NULL \r\n");
        return AIIO_ERROR;
    }

    packet_data = aiio_os_malloc(AIIO_BLE_PROTOCOL_PACKET_MAX_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_ERROR);
    aiio_memset(packet_data, 0, AIIO_BLE_PROTOCOL_PACKET_MAX_LEN);

    last_frame += 1;
    err = aiio_PacketBleResponseDivideData(packet_data, AIIO_BLE_PROTOCOL_PACKET_MAX_LEN, last_frame, entrypt_type, entrypt_key, ble_iot_config.deviceid, error_code);
    if(err < 0)
    {
        aiio_log_e("packet ble protocol fail \r\n");
        aiio_os_free(packet_data);
        return AIIO_ERROR;
    }
    aiio_log_i("packet data len = %d \r\n", err);
    // aiio_HexPrint("ble response divide data", (uint8_t *)packet_data, err);

    ble_send_data.type = AIIO_BLE_NOTIFY;
    ble_send_data.data = (uint8_t *)packet_data;
    ble_send_data.length = err;
    ble_send_data.handle = srv_handle.tx_char_handle[0];
    ble_send_data.conn = ble_conn;

    err = aiio_ble_send(&ble_send_data);
    if(err != AIIO_OK)
    {
        aiio_log_e("ble send data fail \r\n");
        aiio_os_free(packet_data);
        return AIIO_ERROR;
    }

    aiio_os_free(packet_data);

    return AIIO_OK;
}


static int aiio_ble_start_adv_name(char *adv_data_name, uint16_t adv_data_len)
{
    char *adv = NULL;
    

    if(adv_data_name == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    aiio_log_i("adv_data_name = %s\r\n", adv_data_name);
    aiio_log_i("adv_data_len = %d\r\n", adv_data_len);

    adv = aiio_os_malloc(adv_data_len + adv_structure_remain_len + 1);
    if(adv == NULL)
    {
        aiio_log_e("aiio_os_malloc err \r\n");
        return AIIO_MALLOC_FAIL;
    }
    aiio_memset(adv, 0, adv_data_len + adv_structure_remain_len + 1);

    /* Name */
    adv[ADV_NAME_LEN_POSITIONM] = adv_data_len + ADV_NAME_TYPE_BYTES;        /* length */
    adv[ADV_NAME_TYPE_POSITION] = 0x09;         /* type="Complete name" */
    aiio_strncpy(&adv[ADV_NAME_VALUE_POSITION], adv_data_name, adv_data_len);

    // aiio_HexPrint("adv data", (uint8_t *)adv, adv_data_len + adv_structure_remain_len);
    aiio_log_i("adv = %s\r\n", adv);
    aiio_ble_adv_start(&param, (uint8_t *)adv, adv_data_len + adv_structure_remain_len, NULL, 0);
    aiio_os_free(adv);

    return AIIO_OK;
}


static void ble_salve_init(void)
{
    aiio_ble_init(AIIO_BLE_MODE_PERIPHERAL, &ble_config);
    ble_gatts_add_default_svcs(&ble_server, &srv_handle);

    aiio_ble_register_event_cb(aiio_ble_cb);

    ble_print_mac();
}


static void aiio_ble_iot_destroy(void)
{
    aiio_ble_config_queue_t ble_config_queue_data = {0};

    if(ble_adv_name)
    {
        aiio_os_free(ble_adv_name);
        ble_adv_name = NULL;
    }

    if(ble_iot_config.deviceid)
    {
        aiio_os_free(ble_iot_config.deviceid);
    }

    if(ble_iot_config.productid)
    {
        aiio_os_free(ble_iot_config.productid);
    }

    if(ble_iot_config.wifi_flag)
    {
        aiio_os_free(ble_iot_config.wifi_flag);
    }

    if(entrypt_key)
    {
        aiio_os_free(entrypt_key);
        entrypt_key = NULL;
    }

    aiio_memset(&ble_iot_config, 0, sizeof(ble_iot_config));
}



int aiio_ble_config_init(aiio_ble_iot_config_t *ble_congfig)
{
    uint8_t mac[6];
    char macstr[7]; 
    int err = 0;

    if(ble_config_init)
    {
        aiio_log_e("ble config already init \r\n");
        return AIIO_ERROR;
    }

    AIIO_MEM_CHECK(ble_congfig, goto config_fail);
    AIIO_MEM_CHECK(ble_congfig->deviceid, goto config_fail);
    AIIO_MEM_CHECK(ble_congfig->productid, goto config_fail);
    AIIO_MEM_CHECK(ble_congfig->wifi_flag, goto config_fail);
    AIIO_MEM_CHECK(ble_congfig->aiio_ble_iot_config_cb, goto config_fail);

    ble_iot_config.deviceid = aiio_os_malloc(aiio_strlen(ble_congfig->deviceid) + 1);
    AIIO_MEM_CHECK(ble_iot_config.deviceid, goto config_fail);
    aiio_memset(ble_iot_config.deviceid, 0, aiio_strlen(ble_congfig->deviceid) + 1);
    aiio_strncpy(ble_iot_config.deviceid, ble_congfig->deviceid, aiio_strlen(ble_congfig->deviceid));
    aiio_log_d("deviceid = %s \r\n", ble_iot_config.deviceid);

    ble_iot_config.productid = aiio_os_malloc(aiio_strlen(ble_congfig->productid) + 1);
    AIIO_MEM_CHECK(ble_iot_config.productid, goto config_fail);
    aiio_memset(ble_iot_config.productid, 0, aiio_strlen(ble_congfig->productid) + 1);
    aiio_strncpy(ble_iot_config.productid, ble_congfig->productid, aiio_strlen(ble_congfig->productid));
    aiio_log_d("productid = %s \r\n", ble_iot_config.productid);

    ble_iot_config.wifi_flag = aiio_os_malloc(aiio_strlen(ble_congfig->wifi_flag) + 1);
    AIIO_MEM_CHECK(ble_iot_config.wifi_flag, goto config_fail);
    aiio_memset(ble_iot_config.wifi_flag, 0, aiio_strlen(ble_congfig->wifi_flag) + 1);
    aiio_strncpy(ble_iot_config.wifi_flag, ble_congfig->wifi_flag, aiio_strlen(ble_congfig->wifi_flag));
    aiio_log_d("wifi_flag = %s \r\n", ble_iot_config.wifi_flag);

    ble_iot_config.aiio_ble_iot_config_cb = ble_congfig->aiio_ble_iot_config_cb;

    ble_adv_name = aiio_os_malloc(ADV_DATA_MAX_LEN + 1);
    AIIO_MEM_CHECK(ble_adv_name, goto config_fail);

    // ble_config_queue_handle = xQueueCreate(QUEUE_MAX_SIZE, sizeof(aiio_ble_config_queue_t));
    aiio_os_queue_create(&ble_config_queue_handle, QUEUE_MAX_SIZE, sizeof(aiio_ble_config_queue_t));
    if (!ble_config_queue_handle)
    {
        aiio_log_e("create queue fail \r\n");
        goto config_fail;
    }


    ble_iot_config.task_prio = ble_congfig->task_prio;
    if (ble_iot_config.task_prio <= 0) {
        ble_iot_config.task_prio = AIIO_IOT_BLE_CONFIG_TASK_PRIORITY;
    }

    ble_iot_config.task_stack = ble_congfig->task_stack;
    if (ble_iot_config.task_stack <= 0) {
        ble_iot_config.task_stack = AIIO_IOT_BLE_CONFIG_TASK_STACK;
    }

    aiio_log_i("create ble config task \r\n");
    if (aiio_os_thread_create(&ble_config_task_handle, "ble_config_task", aiio_ble_config_task, ble_iot_config.task_stack, NULL, ble_iot_config.task_prio) != AIIO_OK) 
    {
        aiio_log_e("create ble config task fail \r\n");
        goto config_fail;
    }

    ble_salve_init();

    aiio_wifi_sta_mac_get(mac);
    aiio_log_i("ble mac:%02x:%02x:%02x:%02x:%02x:%02x",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    aiio_memset(macstr, 0, sizeof(macstr));
    aiio_snprintf(macstr, sizeof(macstr), "%02X%02X%02X", mac[3], mac[4], mac[5]);
    aiio_log_i("macstr = %s \r\n", macstr);

    aiio_snprintf(ble_adv_name, ADV_DATA_MAX_LEN, "axy_%s-%s_p1_%s", ble_iot_config.wifi_flag, ble_iot_config.productid, macstr);

    err = aiio_ble_start_adv_name(ble_adv_name, aiio_strlen(ble_adv_name));
    if(err != AIIO_OK)
    {
        aiio_log_e("ble adv start fail \r\n");
        aiio_ble_deinit();
        goto config_fail;
    }

    ble_config_init = true;

    return AIIO_OK;

config_fail:
    aiio_ble_iot_destroy();
    return AIIO_ERROR;
}



int aiio_ble_config_deinit(void)
{
    if(!ble_config_init)
    {
        aiio_log_e("ble config already deinit \r\n");
        return AIIO_ERROR;
    }

    aiio_ble_iot_destroy();
    aiio_ble_deinit();

    ble_config_init = false;
    ble_config_task_run = false;

    return AIIO_OK;
}

