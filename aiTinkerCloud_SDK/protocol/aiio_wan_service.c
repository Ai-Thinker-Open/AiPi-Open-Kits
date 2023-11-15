/**
 * @brief   Define the interface of Cloud communication
 * 
 * @file    aiio_wan_service.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note       Define the protocol interface for commnunicating with the cloud
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The protocol interface of Cloud communication 
 */
#include "aiio_wan_service.h"
#include "aiio_protocol_packet.h"
#include "aiio_protocol_parse.h"
#include "aiio_rtc_timer.h"
#include "aiio_log.h"
#include "aiio_error.h"
#include "platform_bl.h"
#include "aiio_wifi.h"
#include "aiio_os_port.h"
#include "aiio_stdio.h"




#ifdef SERVICE_DISABLE_LOCKS
# define SERVICE_LOCK(c)
# define SERVUCE_UNLOCK(c)
#else
# define SERVICE_LOCK(c)          aiio_os_semaphore_get(c->handle->service_lock, portMAX_DELAY)
# define SERVICE_UNLOCK(c)        aiio_os_semaphore_put(c->handle->service_lock)
#endif /* SERVICE_LOCKS */


#define SERVICE_REPEAT_INTERVAL_TIME                        (2 * 1000)
#define SERVICE_REPEAT_INTERVAL_TIME_REQ_UTC                (5 * 60 * 60 * 1000)

typedef enum
{
    AIIO_NS_VAILD = 0,
    AIIO_NS_CONTROL,
    AIIO_NS_QUERY,
    AIIO_NS_REPORT,
    AIIO_NS_CONFIG,
    AIIO_NS_UPGRADE,
    AIIO_NS_INFO,
    AIIO_NS_UTC
}aiio_service_ns_event_t;


typedef enum
{
    AIIO_SERVICE_STATE_INVAILD_EVENT = 0,
    AIIO_SERVICE_STATE_INIT_EVENT,
    AIIO_SERVICE_STATE_MQTT_CONNECTD,
    AIIO_SERVICE_STATE_SUBSCRIBED_EVENT,
    AIIO_SERVICE_STATE_REQ_UTC_EVENT,
    AIIO_SERVICE_STATE_REP_ACTIVITY_EVENT,
    AIIO_SERVICE_STATE_REP_ONLINE_EVENT,
    AIIO_SERVICE_STATE_ONLINE_CHECK,
}aiio_service_state_event_t;


typedef enum 
{
    AIIO_SUBSCRIBE_INVAILD_EVENT = 0,
    AIIO_SUBSCRIBE_UTC_ACK_EVENT,
    AIIO_SUBSCRIBE_INFO_EVENT,
    AIIO_SUBSCRIBE_CONTROL_EVENT,
    AIIO_SUBSCRIBE_QUERY_EVENT,
    AIIO_SUBSCRIBE_UPGRADE_EVENT,
    AIIO_SUBSCRIBE_CONFIG_EVENT
}aiio_subscribe_event_t;


struct aiio_service
{
    int                             cur_sub_msg_id;
    int                             last_sub_msg_id;
    aiio_subscribe_event_t          cur_sub_event;
    aiio_subscribe_event_t          last_sub_event;
    int                             service_state;
    bool                            task_run;
    bool                            utc_req;
    aiio_os_thread_handle_t         task_handle;
    aiio_os_semaphore_handle_t      service_lock;
    aiio_os_event_handle_t          status_bits;
};


const static int SUBSCRIBED_BIT = (1 << 0);
const static int CONNECTED_BIT = (1 << 1);
const static int DISCONNECT_BIT = (1 << 2);

static aiio_service_t service = {0};
static bool utc_is_req = false;
static int  utc_repeat_count = 0;

uint8_t  lan_pro_key[65] = "1234567812345678";
uint8_t  pro_iv[65] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};

static void aiio_protocol_payload(aiio_head_protocol_t *head, char *payload, uint32_t payload_len);
static void aiio_mqtt_service_event(aiio_mqtt_service_event_t *service_event);
static int aiio_service_destroy(aiio_service_t *service_handle);
static int aiio_mqtt_service_config(aiio_service_t *service_handle, aiio_protocol_payload_cb payload_cb, aiio_mqtt_service_event_cb event_cb);
static void aiio_service_task(void *pv);
static int aiio_sev_handle_destroy(aiio_service_handle_t handle);
static int aiio_service_subscribe_topic(aiio_service_t *service_handle, char *topic, int qos);
static int aiio_service_publish_msg(aiio_service_t *service_handle, char *topic, int qos, int retain, char *data, int len);
static int aiio_service_request_utc(aiio_service_t *service_handle);
static int aiio_service_publish_device_info(aiio_service_t *service_handle, char *msgMid, char *from, bool token_status);
static int aiio_service_publish_online(aiio_service_t *service_handle);
static int aiio_service_publish_ota_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_reboot_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_restore_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_control_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_config_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_unkown_ack(aiio_service_t *service_handle, char *topic, char *ns, char *name, char *msgMid, char *from, int32_t code);
static int aiio_service_publish_upgrade_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code);


int aiio_service_init(aiio_service_config_t *service_config)
{
    int ret = -1;

    if(service_config == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(service.service_init)
    {
        aiio_log_e("service already init \r\n");
        return AIIO_MQTT_MUL_INIT;
    }

    service.config.rev_service_data_cb = service_config->rev_service_data_cb;

    if(service_config->service_data.deviceid)
    {
        service.config.service_data.deviceid = aiio_os_malloc(aiio_strlen(service_config->service_data.deviceid) + 1);
        AIIO_MEM_CHECK(service.config.service_data.deviceid, goto service_init_fail);
        aiio_memset(service.config.service_data.deviceid, 0, aiio_strlen(service_config->service_data.deviceid) + 1);
        aiio_strncpy(service.config.service_data.deviceid, service_config->service_data.deviceid, aiio_strlen(service_config->service_data.deviceid));
        aiio_log_i("deviceid : %s \r\n", service.config.service_data.deviceid);
    }
    else
    {
        aiio_log_i("deviceid is NULL \r\n");
        goto service_init_fail;
    }

    if(service_config->service_data.productid)
    {
        service.config.service_data.productid = aiio_os_malloc(aiio_strlen(service_config->service_data.productid) + 1);
        AIIO_MEM_CHECK(service.config.service_data.productid, goto service_init_fail);
        aiio_memset(service.config.service_data.productid, 0, aiio_strlen(service_config->service_data.productid) + 1);
        aiio_strncpy(service.config.service_data.productid, service_config->service_data.productid, aiio_strlen(service_config->service_data.productid));
        aiio_log_i("productid : %s \r\n", service.config.service_data.productid);
    }
    else
    {
        aiio_log_i("productid is NULL \r\n");
        goto service_init_fail;
    }

    if(service_config->service_data.user_name)
    {
        service.config.service_data.user_name = aiio_os_malloc(aiio_strlen(service_config->service_data.user_name) + 1);
        AIIO_MEM_CHECK(service.config.service_data.user_name, goto service_init_fail);
        aiio_memset(service.config.service_data.user_name, 0, aiio_strlen(service_config->service_data.user_name) + 1);
        aiio_strncpy(service.config.service_data.user_name, service_config->service_data.user_name, aiio_strlen(service_config->service_data.user_name));
        aiio_log_i("user_name : %s \r\n", service.config.service_data.user_name);
    }
    else
    {
        aiio_log_i("user_name is NULL \r\n");
        goto service_init_fail;
    }

    if(service_config->service_data.passwd)
    {
        service.config.service_data.passwd = aiio_os_malloc(aiio_strlen(service_config->service_data.passwd) + 1);
        AIIO_MEM_CHECK(service.config.service_data.passwd, goto service_init_fail);
        aiio_memset(service.config.service_data.passwd, 0, aiio_strlen(service_config->service_data.passwd) + 1);
        aiio_strncpy(service.config.service_data.passwd, service_config->service_data.passwd, aiio_strlen(service_config->service_data.passwd));
        aiio_log_i("passwd : %s \r\n", service.config.service_data.passwd);
    }
    else
    {
        aiio_log_i("passwd is NULL \r\n");
        goto service_init_fail;
    }

    if(service_config->dev_info.fwVer)
    {
        service.config.dev_info.fwVer = aiio_os_malloc(aiio_strlen(service_config->dev_info.fwVer) + 1);
        AIIO_MEM_CHECK(service.config.dev_info.fwVer, goto service_init_fail);
        aiio_memset(service.config.dev_info.fwVer, 0, aiio_strlen(service_config->dev_info.fwVer) + 1);
        aiio_strncpy(service.config.dev_info.fwVer, service_config->dev_info.fwVer, aiio_strlen(service_config->dev_info.fwVer));
        aiio_log_i("fwVer : %s \r\n", service.config.dev_info.fwVer);
    }
    else
    {
        aiio_log_i("fwVer is NULL \r\n");
    }

    if(service_config->dev_info.mcuVer)
    {
        service.config.dev_info.mcuVer = aiio_os_malloc(aiio_strlen(service_config->dev_info.mcuVer) + 1);
        AIIO_MEM_CHECK(service.config.dev_info.mcuVer, goto service_init_fail);
        aiio_memset(service.config.dev_info.mcuVer, 0, aiio_strlen(service_config->dev_info.mcuVer) + 1);
        aiio_strncpy(service.config.dev_info.mcuVer, service_config->dev_info.mcuVer, aiio_strlen(service_config->dev_info.mcuVer));
        aiio_log_i("mcuVer : %s \r\n", service.config.dev_info.mcuVer);
    }
    else
    {
        aiio_log_i("mcuVer is NULL \r\n");
    }

    if(service_config->dev_info.devType)
    {
        service.config.dev_info.devType = aiio_os_malloc(aiio_strlen(service_config->dev_info.devType) + 1);
        AIIO_MEM_CHECK(service.config.dev_info.devType, goto service_init_fail);
        aiio_memset(service.config.dev_info.devType, 0, aiio_strlen(service_config->dev_info.devType) + 1);
        aiio_strncpy(service.config.dev_info.devType, service_config->dev_info.devType, aiio_strlen(service_config->dev_info.devType));
        aiio_log_i("devType : %s \r\n", service.config.dev_info.devType);
    }
    else
    {
        aiio_log_i("devType is NULL \r\n");
    }

    if(service_config->dev_info.mac)
    {
        service.config.dev_info.mac = aiio_os_malloc(aiio_strlen(service_config->dev_info.mac) + 1);
        AIIO_MEM_CHECK(service.config.dev_info.mac, goto service_init_fail);
        aiio_memset(service.config.dev_info.mac, 0, aiio_strlen(service_config->dev_info.mac) + 1);
        aiio_strncpy(service.config.dev_info.mac, service_config->dev_info.mac, aiio_strlen(service_config->dev_info.mac));
        aiio_log_i("mac : %s \r\n", service.config.dev_info.mac);
    }
    else
    {
        aiio_log_i("mac is NULL \r\n");
    }

    if(service_config->token)
    {
        service.config.token = aiio_os_malloc(aiio_strlen(service_config->token) + 1);
        AIIO_MEM_CHECK(service.config.token, goto service_init_fail);
        aiio_memset(service.config.token, 0, aiio_strlen(service_config->token) + 1);
        aiio_strncpy(service.config.token, service_config->token, aiio_strlen(service_config->token));
        aiio_log_i("token : %s \r\n", service.config.token);
    }
    else
    {
        aiio_log_i("token is NULL \r\n");
        // goto service_init_fail;
    }

    if(service_config->wifi_info.ssid)
    {
        service.config.wifi_info.ssid = aiio_os_malloc(aiio_strlen(service_config->wifi_info.ssid) + 1);
        AIIO_MEM_CHECK(service.config.wifi_info.ssid, goto service_init_fail);
        aiio_memset(service.config.wifi_info.ssid, 0, aiio_strlen(service_config->wifi_info.ssid) + 1);
        aiio_strncpy(service.config.wifi_info.ssid, service_config->wifi_info.ssid, aiio_strlen(service_config->wifi_info.ssid));
        aiio_log_i("ssid : %s \r\n", service.config.wifi_info.ssid);
    }
    else
    {
        aiio_log_i("ssid is NULL \r\n");
    }

    if(service_config->wifi_info.bssid)
    {
        service.config.wifi_info.bssid = aiio_os_malloc(aiio_strlen(service_config->wifi_info.bssid) + 1);
        AIIO_MEM_CHECK(service.config.wifi_info.bssid, goto service_init_fail);
        aiio_memset(service.config.wifi_info.bssid, 0, aiio_strlen(service_config->wifi_info.bssid) + 1);
        aiio_strncpy(service.config.wifi_info.bssid, service_config->wifi_info.bssid, aiio_strlen(service_config->wifi_info.bssid));
        aiio_log_i("bssid : %s \r\n", service.config.wifi_info.bssid);
    }
    else
    {
        aiio_log_i("bssid is NULL \r\n");
    }

    if(service_config->wifi_info.ip)
    {
        service.config.wifi_info.ip = aiio_os_malloc(aiio_strlen(service_config->wifi_info.ip) + 1);
        AIIO_MEM_CHECK(service.config.wifi_info.ip, goto service_init_fail);
        aiio_memset(service.config.wifi_info.ip, 0, aiio_strlen(service_config->wifi_info.ip) + 1);
        aiio_strncpy(service.config.wifi_info.ip, service_config->wifi_info.ip, aiio_strlen(service_config->wifi_info.ip));
        aiio_log_i("ip : %s \r\n", service.config.wifi_info.ip);
    }
    else
    {
        aiio_log_i("ip is NULL \r\n");
    }

    if(service_config->wifi_info.mask)
    {
        service.config.wifi_info.mask = aiio_os_malloc(aiio_strlen(service_config->wifi_info.mask) + 1);
        AIIO_MEM_CHECK(service.config.wifi_info.mask, goto service_init_fail);
        aiio_memset(service.config.wifi_info.mask, 0, aiio_strlen(service_config->wifi_info.mask) + 1);
        aiio_strncpy(service.config.wifi_info.mask, service_config->wifi_info.mask, aiio_strlen(service_config->wifi_info.mask));
        aiio_log_i("mask : %s \r\n", service.config.wifi_info.mask);
    }
    else
    {
        aiio_log_i("mask is NULL \r\n");
    }


    if(service_config->wifi_info.gw)
    {
        service.config.wifi_info.gw = aiio_os_malloc(aiio_strlen(service_config->wifi_info.gw) + 1);
        AIIO_MEM_CHECK(service.config.wifi_info.gw, goto service_init_fail);
        aiio_memset(service.config.wifi_info.gw, 0, aiio_strlen(service_config->wifi_info.gw) + 1);
        aiio_strncpy(service.config.wifi_info.gw, service_config->wifi_info.gw, aiio_strlen(service_config->wifi_info.gw));
        aiio_log_i("gw : %s \r\n", service.config.wifi_info.gw);
    }
    else
    {
        aiio_log_i("mask is NULL \r\n");
    }

    if(service_config->service_netif.host)
    {
        service.config.service_netif.host = aiio_os_malloc(aiio_strlen(service_config->service_netif.host) + 1);
        AIIO_MEM_CHECK(service.config.service_netif.host, goto service_init_fail);
        aiio_memset(service.config.service_netif.host, 0, aiio_strlen(service_config->service_netif.host) + 1);
        aiio_strncpy(service.config.service_netif.host, service_config->service_netif.host, aiio_strlen(service_config->service_netif.host));
        aiio_log_i("host : %s \r\n", service.config.service_netif.host);
    }
    else if(service_config->service_netif.url)
    {
        service.config.service_netif.url = aiio_os_malloc(aiio_strlen(service_config->service_netif.url) + 1);
        AIIO_MEM_CHECK(service.config.service_netif.url, goto service_init_fail);
        aiio_memset(service.config.service_netif.url, 0, aiio_strlen(service_config->service_netif.url) + 1);
        aiio_strncpy(service.config.service_netif.url, service_config->service_netif.url, aiio_strlen(service_config->service_netif.url));
        aiio_log_i("url : %s \r\n", service.config.service_netif.url);
    }
    else
    {
        aiio_log_i("host and url is NULL \r\n");
        goto service_init_fail;

    }

    if(service_config->service_netif.cert_pem)
    {
        service.config.service_netif.cert_pem = aiio_os_malloc(aiio_strlen(service_config->service_netif.cert_pem) + 1);
        AIIO_MEM_CHECK(service.config.service_netif.cert_pem, goto service_init_fail);
        aiio_memset(service.config.service_netif.cert_pem, 0, aiio_strlen(service_config->service_netif.cert_pem) + 1);
        aiio_strncpy(service.config.service_netif.cert_pem, service_config->service_netif.cert_pem, aiio_strlen(service_config->service_netif.cert_pem));
        service.config.service_netif.cert_len = service_config->service_netif.cert_len;
        aiio_log_i("get cert_perm \r\n");
    }
    else
    {
        aiio_log_i("cert_pem is NULL \r\n");
    }

    aiio_service_handle_t handle = (aiio_service_handle_t)aiio_os_malloc(sizeof(struct aiio_service));
    AIIO_MEM_CHECK(handle, goto service_init_fail);
    aiio_memset(handle, 0, sizeof(struct aiio_service));

    int pa = sizeof(struct aiio_service);
    int pb = sizeof(aiio_service_handle_t);
    aiio_log_i("pa = %d \r\n", pa);
    aiio_log_i("pb = %d \r\n", pb);
    aiio_log_i("handle = %p \r\n", handle);

    service.handle = handle;
    // service.handle->status_bits = xEventGroupCreate();
    aiio_os_event_create(&service.handle->status_bits);
    AIIO_MEM_CHECK(service.handle->status_bits, goto service_init_fail);


    service.handle->service_lock = xSemaphoreCreateRecursiveMutex();
    AIIO_MEM_CHECK(service.handle->service_lock, goto service_init_fail);

    service.config.service_netif.keepalive = service_config->service_netif.keepalive;
    service.config.service_netif.port = service_config->service_netif.port;
    service.config.service_netif.transport = service_config->service_netif.transport;
    service.config.activity = service_config->activity;


    service.config.task_prio = service_config->task_prio;
    if (service.config.task_prio <= 0) {
        service.config.task_prio = SERVICE_TASK_PRIORITY;
    }

    service.config.task_stack = service_config->task_stack;
    if (service.config.task_stack <= 0) {
        service.config.task_stack = SERVICE_TASK_STACK;
    }

    aiio_log_i("create service task \r\n");
    if (aiio_os_thread_create( &service.handle->task_handle, "service_task", aiio_service_task,  service.config.task_stack, NULL, service.config.task_prio) != AIIO_OK) 
    {
        aiio_log_e( "Error create mqtt task");
        goto service_init_fail;
    }

    service.service_init = true;

    ret = aiio_mqtt_service_config(&service, aiio_protocol_payload, aiio_mqtt_service_event);
    if(ret != AIIO_CODE_OK)
    {
        aiio_log_i("mqtt service init fail\r\n");
        goto service_init_fail;
    }

    return AIIO_CODE_OK;


service_init_fail:
    aiio_service_destroy(&service);
    return AIIO_CLOUD_INIT_FAIL;
}


static int aiio_service_destroy(aiio_service_t *service_handle)
{
    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(service_handle->config.service_data.deviceid)
    {
        aiio_os_free(service_handle->config.service_data.deviceid);
        service_handle->config.service_data.deviceid = NULL;
    }

    if(service_handle->config.service_data.productid)
    {
        aiio_os_free(service_handle->config.service_data.productid);
        service_handle->config.service_data.productid = NULL;
    }

    if(service_handle->config.service_data.user_name)
    {
        aiio_os_free(service_handle->config.service_data.user_name);
        service_handle->config.service_data.user_name = NULL;
    }

    if(service_handle->config.service_data.passwd)
    {
        aiio_os_free(service_handle->config.service_data.passwd);
        service_handle->config.service_data.passwd = NULL;
    }

    if(service_handle->config.dev_info.fwVer)
    {
        aiio_os_free(service_handle->config.dev_info.fwVer);
        service_handle->config.dev_info.fwVer = NULL;
    }

    if(service_handle->config.dev_info.mcuVer)
    {
        aiio_os_free(service_handle->config.dev_info.mcuVer);
        service_handle->config.dev_info.mcuVer = NULL;
    }

    if(service_handle->config.dev_info.devType)
    {
        aiio_os_free(service_handle->config.dev_info.devType);
        service_handle->config.dev_info.devType = NULL;
    }

    if(service_handle->config.dev_info.mac)
    {
        aiio_os_free(service_handle->config.dev_info.mac);
        service_handle->config.dev_info.mac = NULL;
    }

    if(service_handle->config.token)
    {
        aiio_os_free(service_handle->config.token);
        service_handle->config.token = NULL;
    }

    if(service_handle->config.wifi_info.ssid)
    {
        aiio_os_free(service_handle->config.wifi_info.ssid);
        service_handle->config.wifi_info.ssid = NULL;
    }

    if(service_handle->config.wifi_info.bssid)
    {
        aiio_os_free(service_handle->config.wifi_info.bssid);
        service_handle->config.wifi_info.bssid = NULL;
    }

    if(service_handle->config.wifi_info.ip)
    {
        aiio_os_free(service_handle->config.wifi_info.ip);
        service_handle->config.wifi_info.ip = NULL;
    }

    if(service_handle->config.wifi_info.mask)
    {
        aiio_os_free(service_handle->config.wifi_info.mask);
        service_handle->config.wifi_info.mask = NULL;
    }

    if(service_handle->config.service_netif.host)
    {
        aiio_os_free(service_handle->config.service_netif.host);
        service_handle->config.service_netif.host = NULL;
        aiio_log_e("########### host ######\r\n");
    }

    if(service_handle->config.service_netif.url)
    {
        aiio_os_free(service_handle->config.service_netif.url);
        service_handle->config.service_netif.url = NULL;
        aiio_log_e("########### url ######\r\n");
    }

    if(service_handle->config.service_netif.cert_pem)
    {
        aiio_os_free(service_handle->config.service_netif.cert_pem);
        service_handle->config.service_netif.cert_pem = NULL;
        aiio_log_e("########### cert ######\r\n");
    }

    if(service_handle->handle)
    {
        if(service_handle->handle->status_bits)
        {
            // vEventGroupDelete(service_handle->handle->status_bits);
            aiio_os_event_delete(service_handle->handle->status_bits);
            service_handle->handle->status_bits = NULL;
        }

        if(service_handle->handle->service_lock)
        {
            aiio_os_semaphore_delete(service_handle->handle->service_lock);
            service_handle->handle->service_lock = NULL;
        }

        if(service_handle->handle->task_handle)
        {
            // aiio_os_thread_delete(service_handle->handle->task_handle);
            service_handle->handle->task_handle = NULL;
        }

        aiio_os_free(service_handle->handle);
        service_handle->handle = NULL;
    }

    aiio_memset(service_handle, 0, sizeof(aiio_service_t));

    aiio_log_d("service destroy ok \r\n");

    return AIIO_CODE_OK;
}


int aiio_service_deinit(void)
{
    if(!service.service_init)
    {
        aiio_log_e("service already deinit \r\n");
        return AIIO_MQTT_MUL_DEINIT;
    }

    if(service.handle)
    {
        service.handle->task_run = false;
    }
    aiio_mqtt_service_deinit();

    service.service_init = false;

    aiio_log_d("service deinit ok \r\n");

    return AIIO_CODE_OK;
}


int aiio_service_report_device_info(char *msgMid, char *from)
{
    return aiio_service_publish_device_info(&service, msgMid, from, false);
}


int aiio_service_report_ota_rate(aiio_ota_info_t *ota_info)
{
    char *packet_data = NULL;
    int err = 0;

    if(ota_info == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN * 2);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN * 2);

    err = aiio_PacketReportOtaRateData(packet_data, AIIO_SERVICE_MAX_MSG_LEN * 2, NULL, NULL, aiio_get_current_time_stamp(), ota_info);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(&service, TOPIC_STR_UPGRADE"/"TOPIC_STR_REPORT, 0, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


int aiio_service_report_config_ack(char *msgMid, char *from, int32_t code)
{
    return aiio_service_publish_control_ack(&service, msgMid, from, code);
}


int aiio_service_report_control_ack(char *msgMid, char *from, int32_t code)
{
    return aiio_service_publish_control_ack(&service, msgMid, from, code);
}


int aiio_service_report_restore_ack(char *msgMid, char *from, int32_t code)
{
    return aiio_service_publish_control_ack(&service, msgMid, from, code);
}


int aiio_service_report_reboot_ack(char *msgMid, char *from, int32_t code)
{
    return aiio_service_publish_control_ack(&service, msgMid, from, code);
}


int aiio_service_report_ota_ack(char *msgMid, char *from, int32_t code)
{
    return aiio_service_publish_upgrade_ack(&service, msgMid, from, code);
}


static int aiio_service_publish_upgrade_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketOtaAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_UPGRADE"/"TOPIC_STR_ACK, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}

int aiio_service_report_attribute(char *msgMid, char *from, char *device_att)
{
    char *packet_data = NULL;
    int err = 0;
    int att_len = 0;

    if(device_att == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    att_len = aiio_strlen(device_att);
    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN + att_len);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN + att_len);

    err = aiio_PacketReportAttrbutesData(packet_data, AIIO_SERVICE_MAX_MSG_LEN + att_len, msgMid, from, aiio_get_current_time_stamp(), device_att);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(&service, TOPIC_STR_REPORT, 0, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}



static int aiio_service_publish_config_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketConfigAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_CONFIG"/"TOPIC_STR_ACK, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_control_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketControlAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_CONTROL"/"TOPIC_STR_ACK, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_unkown_ack(aiio_service_t *service_handle, char *topic, char *ns, char *name, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL || topic == NULL || ns == NULL || name == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketAckData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, ns, name, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, topic, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_restore_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketRestoreAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_CONTROL"/"TOPIC_STR_ACK, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_reboot_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketRebootAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_CONTROL"/"TOPIC_STR_ACK, 1, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_ota_ack(aiio_service_t *service_handle, char *msgMid, char *from, int32_t code)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketOtaAckeData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, msgMid, from, aiio_get_current_time_stamp(), code);
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_UPGRADE"/"TOPIC_STR_ACK, 0, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}





static int aiio_mqtt_service_config(aiio_service_t *service_handle, aiio_protocol_payload_cb payload_cb, aiio_mqtt_service_event_cb event_cb)
{
    char *service_lwt_topic = NULL;
    char  *service_lwt_msg = NULL;
    int  err = 0;
    aiio_mqtt_service_t mqtt_service = {0};

    if(service_handle == NULL || payload_cb == NULL || event_cb == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    service_lwt_topic = aiio_os_malloc(AIIO_SERVICE_MAX_TOPIC_LEN);
    AIIO_MEM_CHECK(service_lwt_topic, goto config);
    aiio_memset(service_lwt_topic, 0, AIIO_SERVICE_MAX_TOPIC_LEN);
    aiio_snprintf(service_lwt_topic, AIIO_SERVICE_MAX_TOPIC_LEN, "%s/%s/online",service_handle->config.service_data.productid, service_handle->config.service_data.deviceid);
    aiio_log_i("service_lwt_topic = %s \r\n", service_lwt_topic);

    service_lwt_msg = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(service_lwt_msg, goto config);
    aiio_memset(service_lwt_msg, 0, AIIO_SERVICE_MAX_MSG_LEN);
    aiio_PacketOfflineData(service_lwt_msg, AIIO_SERVICE_MAX_MSG_LEN, NULL, NULL, aiio_get_current_time_stamp());
    aiio_log_i("service_lwt_msg = %s \r\n", service_lwt_msg);

    aiio_mqtt_client_config_t mqtt_cfg = {
        .client_id = service_handle->config.service_data.deviceid,
        .username = service_handle->config.service_data.user_name,
        .password = service_handle->config.service_data.passwd,
        .lwt_topic = service_lwt_topic,
        .lwt_msg = service_lwt_msg,
        .lwt_qos = 0,
        .lwt_retain = 1,
        .lwt_msg_len = aiio_strlen(service_lwt_msg),
        .keepalive = service_handle->config.service_netif.keepalive,
        .disable_clean_session = 0,
        .uri = service_handle->config.service_netif.url,
        .host = service_handle->config.service_netif.host,
        .port = service_handle->config.service_netif.port,
        .cert_pem = service_handle->config.service_netif.cert_pem,
        .cert_len = service_handle->config.service_netif.cert_len,
        .transport = service_handle->config.service_netif.transport,
    };

    mqtt_service.rev_payload_cb = payload_cb;
    mqtt_service.rev_event_cb = event_cb;
    mqtt_service.mqtt_config = &mqtt_cfg;

    err = aiio_mqtt_service_init(&mqtt_service);
    if(err != AIIO_CODE_OK)
    {
        aiio_log_e("mqtt service init err \r\n");
        goto config;
    }

    aiio_service_rev_event_t event = {0};
    event.service_event = AIIO_SERVICE_MQTT_CONNECTING;
    if(service.config.rev_service_data_cb)
    {
        service.config.rev_service_data_cb(&event);
    }

    aiio_os_free(service_lwt_topic);
    aiio_os_free(service_lwt_msg);
    return err;

config:
    if(service_lwt_topic)
    {
        aiio_os_free(service_lwt_topic);
    }
    if(service_lwt_msg)
    {
        aiio_os_free(service_lwt_msg);
    }

    return AIIO_FAIL;
}



static aiio_service_ns_event_t aiio_protocol_ns_parse(char *str_ns)
{
    if(str_ns == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_NS_VAILD;
    }

    if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_CONTROL))
    {
        return AIIO_NS_CONTROL;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_QUERY))
    {
        return AIIO_NS_QUERY;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_REPORT))
    {
        return AIIO_NS_REPORT;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_CONFIG))
    {
        return AIIO_NS_CONFIG;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_UPGRADE))
    {
        return AIIO_NS_UPGRADE;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_INFO))
    {
        return AIIO_NS_INFO;
    }
    else if(ARGV_CMDEQUAL(str_ns, STR_NS_IOT_DEVICE_UTC))
    {
        return AIIO_NS_UTC;
    }
    else
    {
        return AIIO_NS_VAILD;
    }
}



static void aiio_protocol_payload(aiio_head_protocol_t *head, char *payload, uint32_t payload_len)
{
    aiio_service_rev_event_t event = {0};
    char *time_buff = NULL;

    if(head == NULL)
    {
        aiio_log_e("param err \r\n");
        return ;
    }

    if(!service.handle->task_run)
    {
        aiio_log_e("task run err \r\n");
        return ;
    }

    if(payload)
    {
        aiio_log_i("payload : %s \r\n", payload);
    }

    event.data = payload;
    event.data_len = payload_len;
    event.from = head->from;
    event.msgMid = head->mid;
    switch (aiio_protocol_ns_parse(head->ns))
    {
        case AIIO_NS_CONTROL:
        {
            aiio_log_i("receive control cmd \r\n");

            if(ARGV_CMDEQUAL(head->name, STR_NAME_RESTORE))
            {
                event.service_event = AIIO_SERVICE_RESTORE_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_restore_ack(&service, head->mid, head->from, AIIO_CODE_OK);
                }
            }
            else if(ARGV_CMDEQUAL(head->name, STR_NAME_REBOOT))
            {
                event.service_event = AIIO_SERVICE_REBOOT_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_reboot_ack(&service, head->mid, head->from, AIIO_CODE_OK);
                }
            }
            else if(ARGV_CMDEQUAL(head->name, STR_NAME_CONTROL))
            {
                event.service_event = AIIO_SERVICE_CONTROL_EVENT;
                aiio_SetPacketReportAttributesType(CLOUD_CONTROL_TYPE);
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_control_ack(&service, head->mid, head->from, AIIO_CODE_OK);
                }
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_unkown_ack(&service, TOPIC_STR_CONTROL"/"TOPIC_STR_ACK, head->ns, head->name, head->mid, head->from, AIIO_NAME_ERROR);
                }
            }
        }
        break;

        case AIIO_NS_QUERY:
        {
            aiio_log_i("receive query cmd \r\n");

            if(ARGV_CMDEQUAL(head->name, STR_NAME_QUERY))
            {
                event.service_event = AIIO_SERVICE_QUERY_EVENT;
                aiio_SetPacketReportAttributesType(CLOUD_ALL_QUERY_TYPE);
            }
            else if(ARGV_CMDEQUAL(head->name, STR_NAME_QUERY_ALL))
            {
                event.service_event = AIIO_SERVICE_QUERY_EVENT;
                aiio_SetPacketReportAttributesType(CLOUD_ALL_QUERY_TYPE);
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_unkown_ack(&service, TOPIC_STR_QUERY"/"TOPIC_STR_ACK, head->ns, head->name, head->mid, head->from, AIIO_NAME_ERROR);
                }
            }
        }
        break;

        case AIIO_NS_REPORT:
        {
            aiio_log_i("receive report cmd \r\n");
        }
        break;

        case AIIO_NS_CONFIG:
        {
            aiio_log_i("receive config cmd \r\n");

            if(ARGV_CMDEQUAL(head->name, STR_NAME_CONFIG))
            {
                event.service_event = AIIO_SERVICE_CONFIG_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_config_ack(&service, head->mid, head->from, AIIO_CODE_OK);
                }
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_unkown_ack(&service, TOPIC_STR_CONFIG"/"TOPIC_STR_ACK, head->ns, head->name, head->mid, head->from, AIIO_NAME_ERROR);
                }
            }
        }
        break;

        case AIIO_NS_UPGRADE:
        {
            aiio_log_i("receive upgrade cmd \r\n");
            if(ARGV_CMDEQUAL(head->name, STR_NAME_OTA_INFO))
            {
                event.service_event = AIIO_SERVICE_UPGRADE_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_ota_ack(&service, head->mid, head->from, AIIO_CODE_OK);
                }
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_unkown_ack(&service, TOPIC_STR_UPGRADE"/"TOPIC_STR_ACK, head->ns, head->name, head->mid, head->from, AIIO_NAME_ERROR);
                }
            }
        }
        break;

        case AIIO_NS_INFO:
        {
            aiio_log_i("receive info cmd \r\n");
            if(ARGV_CMDEQUAL(head->name, STR_NAME_INFO))
            {
                event.service_event = AIIO_SERVICE_INFO_EVENT;
                aiio_service_publish_device_info(&service, head->mid, head->from, false);
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
                if(!service.config.disenable_auto_ack)
                {
                    aiio_service_publish_unkown_ack(&service, TOPIC_STR_INFO"/"TOPIC_STR_REPORT, head->ns, head->name, head->mid, head->from, AIIO_NAME_ERROR);
                }
            }
        }
        break;

        case AIIO_NS_UTC:
        {
            aiio_log_i("receive utc cmd \r\n");
            if(ARGV_CMDEQUAL(head->name, STR_NAME_UTC))
            {
                event.service_event = AIIO_SERVICE_UTC_EVENT;
                service.handle->utc_req = false;
                time_buff = aiio_os_malloc(AIIO_SERVICE_MAX_TMP_LEN);
                AIIO_MEM_CHECK(time_buff, return );
                aiio_memset(time_buff, 0, AIIO_SERVICE_MAX_TMP_LEN);
                aiio_ParseTimeData(payload, time_buff, AIIO_SERVICE_MAX_TMP_LEN);
                event.data = time_buff;
                event.data_len = aiio_strlen(time_buff);
                aiio_os_free(time_buff);
            }
            else
            {
                event.service_event = AIIO_SERVICE_INVAILD_EVENT;
            }
        }
        break;
        
        default:
            aiio_log_e("ns invaild \r\n");
            break;
    }

    if(service.config.rev_service_data_cb)
    {
        service.config.rev_service_data_cb(&event);
    }

}
static void aiio_mqtt_service_event(aiio_mqtt_service_event_t *service_event)
{

    if(service_event == NULL)
    {
        aiio_log_e("param err \r\n");
        return ;
    }

    aiio_log_i("Event dispatched, event_id=%d\r\n", service_event->event_id);
    aiio_log_i("Event dispatched, topic=%s\r\n", service_event->topic);
    aiio_log_i("Event dispatched, topic_len=%d\r\n", service_event->topic_len);
    aiio_log_i("Event dispatched, msg_id=%d\r\n", service_event->msg_id);
    aiio_log_i("Event dispatched, qos=%d\r\n", service_event->qos);

    if(!service.handle)
    {
        aiio_log_e("task run err \r\n");
        return ;
    }

    if(!service.handle->task_run)
    {
        aiio_log_e("task run err \r\n");
        return ;
    }
    
    switch (service_event->event_id)
    {
        case MQTT_EVENT_CONNECTED:
        {
            aiio_log_i("MQTT_EVENT_CONNECTED\r\n");
            // xEventGroupSetBits(service.handle->status_bits, CONNECTED_BIT);
            aiio_os_event_set_bit(service.handle->status_bits, CONNECTED_BIT);
        }
        break;

        case MQTT_EVENT_DISCONNECTED:
        {
            aiio_log_i("MQTT_EVENT_DISCONNECTED\r\n");
            // xEventGroupSetBits(service.handle->status_bits, DISCONNECT_BIT);
            aiio_os_event_set_bit(service.handle->status_bits, DISCONNECT_BIT);
            service.service_init = false;                                           /* A delete task and resource release event will be sent when the mqtt disconnecte, This variable will prevent repeated call aiio_service_deinit() */
        }
        break;

        case MQTT_EVENT_SUBSCRIBED:
        {
            aiio_log_i("MQTT_EVENT_SUBSCRIBED, msg_id=%d\r\n", service_event->msg_id);
            service.handle->last_sub_msg_id = service_event->msg_id;
            if(service.handle->last_sub_msg_id == service.handle->cur_sub_msg_id)
            {
                service.handle->last_sub_event = service.handle->cur_sub_event;
            }
            else
            {
                service.handle->last_sub_event = AIIO_SUBSCRIBE_INVAILD_EVENT;
            }
            aiio_os_event_set_bit(service.handle->status_bits, SUBSCRIBED_BIT);
        }
        break;

        case MQTT_EVENT_UNSUBSCRIBED:
        {
            aiio_log_i("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d\r\n", service_event->msg_id);
        }
        break;

        case MQTT_EVENT_PUBLISHED:
        {
            aiio_log_i("MQTT_EVENT_PUBLISHED, msg_id=%d\r\n", service_event->msg_id);
        }
        break;

        case MQTT_EVENT_DATA:
        {
            aiio_log_i("MQTT_EVENT_DATA\r\n");
            aiio_log_i("MQTT_EVENT_DATA, msg_id=%d\r\n", service_event->msg_id);
        }
        break;

        case MQTT_EVENT_ERROR:
        {
            aiio_log_i("MQTT_EVENT_ERROR\r\n");
        }
        break;

        default:
        aiio_log_i("Other event id:%d\r\n", service_event->event_id);
        break;
    }
}




static uint32_t diffTimeTick(uint32_t start, uint32_t end)
{
    return ((end >= start) ? (end-start) : ((0xFFFFFFFF-start) + end));
}


static int aiio_service_subscribe_topic(aiio_service_t *service_handle, char *topic, int qos)
{
    char *sub_topic = NULL;
    int  err = 0;
    
    if(service_handle == NULL || topic == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    sub_topic = aiio_os_malloc(AIIO_SERVICE_MAX_TOPIC_LEN);
    AIIO_MEM_CHECK(sub_topic, return AIIO_MALLOC_FAIL);
    aiio_memset(sub_topic, 0, AIIO_SERVICE_MAX_TOPIC_LEN);

    aiio_snprintf(sub_topic, AIIO_SERVICE_MAX_TOPIC_LEN, "%s/%s/%s", service_handle->config.service_data.productid, service_handle->config.service_data.deviceid, topic);

    aiio_log_i("subscribe topic: %s \r\n", sub_topic);
    err = aiio_mqtt_service_subscribe(sub_topic, qos);

    aiio_os_free(sub_topic);

    return err;
}


static int aiio_service_publish_msg(aiio_service_t *service_handle, char *topic, int qos, int retain, char *data, int len)
{
    char *publish_topic = NULL;
    int  err = 0;
    
    if(service_handle == NULL || topic == NULL || data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    publish_topic = aiio_os_malloc(AIIO_SERVICE_MAX_TOPIC_LEN);
    AIIO_MEM_CHECK(publish_topic, return AIIO_MALLOC_FAIL);
    aiio_memset(publish_topic, 0, AIIO_SERVICE_MAX_TOPIC_LEN);

    aiio_snprintf(publish_topic, AIIO_SERVICE_MAX_TOPIC_LEN, "%s/%s/%s", service_handle->config.service_data.productid, service_handle->config.service_data.deviceid, topic);

    aiio_log_i("publish topic: %s \r\n", publish_topic);
    aiio_log_i("publish data: %s \r\n", data);
    err = aiio_mqtt_service_publish(publish_topic, data, len, qos, retain);

    aiio_os_free(publish_topic);

    return err;
}


static int aiio_service_request_utc(aiio_service_t *service_handle)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketRequesUTCData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, NULL, NULL, aiio_get_current_time_stamp());
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_UTC, 0, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static int aiio_service_publish_device_info(aiio_service_t *service_handle, char *msgMid, char *from, bool token_status)
{
    char *packet_data = NULL;
    aiio_device_info_t device_info = {0};
    aiio_wifi_info_t   wifi_info = {0};
    int err = 0;
    int rssi = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    device_info.deviceid = service_handle->config.service_data.deviceid;
    device_info.productid = service_handle->config.service_data.productid;
    device_info.devType = service_handle->config.dev_info.devType;
    device_info.fwVer = service_handle->config.dev_info.fwVer;
    device_info.mcuVer = service_handle->config.dev_info.mcuVer;
    device_info.mac = service_handle->config.dev_info.mac;
    device_info.memfree = aiio_os_get_free_heap_size();
    device_info.secrtkey = (char *)lan_pro_key;
    device_info.token = service_handle->config.token;

    aiio_strncpy(&wifi_info, &service_handle->config.wifi_info, sizeof(aiio_wifi_info_t));
    aiio_wifi_rssi_get(&rssi);
    wifi_info.rssi = rssi;

    err = aiio_PacketDeviceInfoData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, &device_info, &wifi_info, msgMid, from, token_status, aiio_get_current_time_stamp());
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_INFO"/"TOPIC_STR_REPORT, 0, 0, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}



static int aiio_service_publish_online(aiio_service_t *service_handle)
{
    char *packet_data = NULL;
    int err = 0;

    if(service_handle == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    packet_data = aiio_os_malloc(AIIO_SERVICE_MAX_MSG_LEN);
    AIIO_MEM_CHECK(packet_data, return AIIO_MALLOC_FAIL);
    aiio_memset(packet_data, 0, AIIO_SERVICE_MAX_MSG_LEN);

    err = aiio_PacketOnlineData(packet_data, AIIO_SERVICE_MAX_MSG_LEN, NULL, NULL, aiio_get_current_time_stamp());
    if(err < 0)
    {
        aiio_log_e("packet protocol err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    err = aiio_service_publish_msg(service_handle, TOPIC_STR_ONLINE, 0, 1, packet_data, aiio_strlen(packet_data));
    if(err < 0)
    {
        aiio_log_e("publish msg err \r\n");
        aiio_os_free(packet_data);
        return err;
    }

    aiio_os_free(packet_data);
    
    return AIIO_CODE_OK;
}


static void aiio_service_subscribe_event(aiio_subscribe_event_t event)
{
    switch (event)
    {
        case AIIO_SUBSCRIBE_UTC_ACK_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_UTC_ACK_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_UTC"/"TOPIC_STR_ACK, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_UTC_ACK_EVENT;
        }
        break;

        case AIIO_SUBSCRIBE_INFO_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_INFO_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_INFO, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_INFO_EVENT;
        }
        break;

        case AIIO_SUBSCRIBE_CONTROL_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_CONTROL_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_CONTROL, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_CONTROL_EVENT;

        }
        break;

        case AIIO_SUBSCRIBE_QUERY_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_QUERY_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_QUERY, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_QUERY_EVENT;
        }
        break;

        case AIIO_SUBSCRIBE_UPGRADE_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_UPGRADE_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_UPGRADE, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_UPGRADE_EVENT;
        }
        break;

        case AIIO_SUBSCRIBE_CONFIG_EVENT:
        {
            aiio_log_i("AIIO_SUBSCRIBE_CONFIG_EVENT \r\n");
            service.handle->cur_sub_msg_id = aiio_service_subscribe_topic(&service, TOPIC_STR_CONFIG, 0);
            aiio_log_i("subscribe id: %d \r\n", service.handle->cur_sub_msg_id);
            service.handle->cur_sub_event  = AIIO_SUBSCRIBE_CONFIG_EVENT;
        }
        break;
        
        default:
            aiio_log_e("event err \r\n");
            break;
    }
}

static void aiio_service_task(void *pv)
{
    int last_service_state = AIIO_SERVICE_STATE_INVAILD_EVENT;
    aiio_service_rev_event_t event = {0};
    int ret = 0;
    uint32_t last_tick = 0;
    uint32_t current_tick = 0;

    service.handle->task_run = true;
    service.handle->service_state = AIIO_SERVICE_STATE_INIT_EVENT;

    while (service.handle->task_run)
    {
        switch (service.handle->service_state)
        {
            case AIIO_SERVICE_STATE_INIT_EVENT:
            {
                if(last_service_state != AIIO_SERVICE_STATE_INIT_EVENT)
                {
                    last_service_state = service.handle->service_state;
                    aiio_os_tick_count(&last_tick);
                    aiio_log_i("AIIO_SERVICE_STATE_INIT_EVENT \r\n");
                }
                
                // if(xEventGroupWaitBits(service.handle->status_bits, CONNECTED_BIT, true, true, 0) & CONNECTED_BIT)
                if(aiio_os_event_wait_bits(service.handle->status_bits, CONNECTED_BIT, true, true, 0) & CONNECTED_BIT)
                {
                    service.handle->service_state = AIIO_SERVICE_STATE_MQTT_CONNECTD;
                }
                else
                {
                    aiio_os_tick_count(&current_tick);
                    if(service.config.mqtt_connect_timeout == 0)
                    {
                        service.config.mqtt_connect_timeout = service.config.service_netif.keepalive;
                    }
                    if(diffTimeTick(last_tick, current_tick) > (service.config.mqtt_connect_timeout * 1000))
                    {
                        aiio_mqtt_service_deinit();
                        service.handle->task_run = false;
                        service.handle->service_state = AIIO_SERVICE_STATE_INVAILD_EVENT;
                        event.service_event = AIIO_SERVICE_LANCH_FAIL_EVNET;
                        if(service.config.rev_service_data_cb)
                        {
                            service.config.rev_service_data_cb(&event);
                        }
                    }
                }
            }
            break;

            case AIIO_SERVICE_STATE_MQTT_CONNECTD:
            {
                if(last_service_state != AIIO_SERVICE_STATE_MQTT_CONNECTD)
                {
                    last_service_state = service.handle->service_state;
                    aiio_log_i("AIIO_SERVICE_STATE_MQTT_CONNECTD \r\n");
                }
                service.handle->service_state = AIIO_SERVICE_STATE_SUBSCRIBED_EVENT;
                event.service_event = AIIO_SERVICE_MQTT_CONNECTED;
                if(service.config.rev_service_data_cb)
                {
                    service.config.rev_service_data_cb(&event);
                }
            }
            break;

            case AIIO_SERVICE_STATE_SUBSCRIBED_EVENT:
            {
                if(last_service_state != AIIO_SERVICE_STATE_SUBSCRIBED_EVENT)
                {
                    last_service_state = service.handle->service_state;
                    aiio_os_tick_count(&last_tick);
                    aiio_log_i("AIIO_SERVICE_STATE_SUBSCRIBED_EVENT \r\n");
                }

                if(aiio_os_event_wait_bits(service.handle->status_bits, SUBSCRIBED_BIT, true, true, 0) & SUBSCRIBED_BIT)
                {
                    if(service.handle->last_sub_event == AIIO_SUBSCRIBE_UTC_ACK_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_INFO_EVENT);
                    }
                    else if(service.handle->last_sub_event == AIIO_SUBSCRIBE_INFO_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_CONTROL_EVENT);
                    }
                    else if(service.handle->last_sub_event == AIIO_SUBSCRIBE_CONTROL_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_QUERY_EVENT);
                    }
                    else if(service.handle->last_sub_event == AIIO_SUBSCRIBE_QUERY_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_UPGRADE_EVENT);
                    }
                    else if(service.handle->last_sub_event == AIIO_SUBSCRIBE_UPGRADE_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_CONFIG_EVENT);
                    }
                    else if(service.handle->last_sub_event == AIIO_SUBSCRIBE_CONFIG_EVENT)
                    {
                        service.handle->cur_sub_event = AIIO_SUBSCRIBE_INVAILD_EVENT;
                        service.handle->last_sub_event = AIIO_SUBSCRIBE_INVAILD_EVENT;
                        service.handle->cur_sub_msg_id = 0;
                        service.handle->last_sub_msg_id = 0;
                        service.handle->service_state = AIIO_SERVICE_STATE_REQ_UTC_EVENT;
                    }
                    else
                    {
                        aiio_service_subscribe_event(service.handle->cur_sub_event);
                    }
                }
                else
                {
                    if(service.handle->cur_sub_event == AIIO_SUBSCRIBE_INVAILD_EVENT)
                    {
                        aiio_service_subscribe_event(AIIO_SUBSCRIBE_UTC_ACK_EVENT);
                    }
                }

                aiio_os_tick_count(&current_tick);
                if(diffTimeTick(last_tick, current_tick) > (service.config.service_netif.keepalive * 2 * 1000))
                {
                    aiio_mqtt_service_deinit();
                    service.handle->task_run = false;
                    service.handle->service_state = AIIO_SERVICE_STATE_INVAILD_EVENT;
                    event.service_event = AIIO_SERVICE_SUBSCRIBE_TIMEOUT;
                    if(service.config.rev_service_data_cb)
                    {
                        service.config.rev_service_data_cb(&event);
                    }
                }
            
            }
            break;

            case AIIO_SERVICE_STATE_REQ_UTC_EVENT:
            {
                if(last_service_state != AIIO_SERVICE_STATE_REQ_UTC_EVENT)
                {
                    last_service_state = service.handle->service_state;
                    utc_is_req = true;
                    aiio_os_tick_count(&last_tick);
                    aiio_log_i("last_tick = %d \r\n", last_tick);
                    aiio_log_i("AIIO_SERVICE_STATE_REQ_UTC_EVENT \r\n");
                }
                
                if(utc_is_req)
                {
                    utc_is_req = false;
                    service.handle->utc_req = true;
                    aiio_service_request_utc(&service);
                }

                if(!service.handle->utc_req)
                {
                    service.handle->service_state = AIIO_SERVICE_STATE_REP_ACTIVITY_EVENT;
                    utc_repeat_count = 0;
                }

                aiio_os_tick_count(&current_tick);
                if(diffTimeTick(last_tick, current_tick) > SERVICE_REPEAT_INTERVAL_TIME)
                {
                    aiio_log_i("last_tick = %d \r\n", last_tick);
                    aiio_log_i("current_tick = %d \r\n", current_tick);
                    utc_repeat_count++;
                    utc_is_req = true;
                    last_tick = current_tick;

                    if(utc_repeat_count > 10)
                    {
                        utc_repeat_count = 0;
                        aiio_mqtt_service_deinit();
                        service.handle->task_run = false;
                        service.handle->service_state = AIIO_SERVICE_STATE_INVAILD_EVENT;
                        event.service_event = AIIO_SERVICE_SUBSCRIBE_TIMEOUT;
                        if(service.config.rev_service_data_cb)
                        {
                            service.config.rev_service_data_cb(&event);
                        }
                    }
                }
                
            }
            break;

            case AIIO_SERVICE_STATE_REP_ACTIVITY_EVENT:
            {
                if(last_service_state != AIIO_SERVICE_STATE_REP_ACTIVITY_EVENT)
                {
                    last_service_state = service.handle->service_state;
                    aiio_log_i("AIIO_SERVICE_STATE_REP_ACTIVITY_EVENT \r\n");
                }
                
                if(!service.config.activity)
                {
                    aiio_log_i("publish activity info \r\n");
                    aiio_service_publish_device_info(&service, NULL, NULL, true);
                    service.config.activity = true;
                    event.service_event = AIIO_SERVICE_ACTIVITY_EVENT;
                    if(service.config.rev_service_data_cb)
                    {
                        service.config.rev_service_data_cb(&event);
                    }
                }
                else
                {
                    aiio_log_i("device already activity \r\n");
                }

                service.handle->service_state = AIIO_SERVICE_STATE_REP_ONLINE_EVENT;
            }
            break;

            case AIIO_SERVICE_STATE_REP_ONLINE_EVENT:
            {
                if(last_service_state != AIIO_SERVICE_STATE_REP_ONLINE_EVENT)
                {
                    last_service_state = service.handle->service_state;
                    aiio_log_i("AIIO_SERVICE_STATE_REP_ONLINE_EVENT \r\n");
                }
                
                aiio_service_publish_online(&service);
                service.handle->service_state = AIIO_SERVICE_STATE_ONLINE_CHECK;
                event.service_event = AIIO_SERVICE_ONLINE_EVENT;
                if(service.config.rev_service_data_cb)
                {
                    service.config.rev_service_data_cb(&event);
                }
            }
            break;

            case AIIO_SERVICE_STATE_ONLINE_CHECK:
            {
                if(last_service_state != AIIO_SERVICE_STATE_ONLINE_CHECK)
                {
                    last_service_state = service.handle->service_state;
                    service.online_status = true;
                    aiio_os_tick_count(&last_tick);
                    aiio_log_i("last_tick = %d \r\n", last_tick);
                    aiio_log_i("AIIO_SERVICE_STATE_ONLINE_CHECK \r\n");
                }
                
                if(utc_is_req)
                {
                    utc_is_req = false;
                    service.handle->utc_req = true;
                    aiio_service_request_utc(&service);
                }

                aiio_os_tick_count(&current_tick);
                if(diffTimeTick(last_tick, current_tick) > SERVICE_REPEAT_INTERVAL_TIME_REQ_UTC)
                {
                    aiio_log_i("last_tick = %d \r\n", last_tick);
                    aiio_log_i("current_tick = %d \r\n", current_tick);
                    utc_is_req = true;
                    last_tick = current_tick;
                }
            }
            break;

            
            default:
                aiio_log_i("Other event id:%d", service.handle->service_state);
                break;
        }

        if(aiio_os_event_wait_bits(service.handle->status_bits, DISCONNECT_BIT, true, true, 0) & DISCONNECT_BIT)
        {
            aiio_mqtt_service_deinit();
            service.handle->task_run = false;
            service.online_status = false;
            service.handle->service_state = AIIO_SERVICE_STATE_INVAILD_EVENT;
            event.service_event = AIIO_SERVICE_MQTT_DISCONNECT;
            if(service.config.rev_service_data_cb)
            {
                service.config.rev_service_data_cb(&event);
            }
        }

        aiio_os_tick_dealy(aiio_os_ms2tick(20));
    }
    
    aiio_service_destroy(&service);
    aiio_os_thread_delete(NULL);
}

