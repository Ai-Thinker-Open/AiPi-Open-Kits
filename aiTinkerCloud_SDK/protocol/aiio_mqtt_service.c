/**
 * @brief   Define the interface of mqtt communication
 * 
 * @file    aiio_mqtt_service.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note     Define the interface of  mqtt configuration by mqtt library 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The interface of mqtt configuration
 */
#include "aiio_mqtt_service.h"
#include "aiio_log.h"
#include "aiio_stdio.h"



static aiio_mqtt_service_t service_config = {0};
static aiio_mqtt_client_handle_t mqtt_client = NULL;
static aiio_mqtt_client_config_t mqtt_cfg = {0};

static aiio_err_t mqtt_event_cb(aiio_mqtt_event_handle_t event);



int aiio_mqtt_service_init(aiio_mqtt_service_t *config)
{
    if(config == NULL || config->mqtt_config == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(mqtt_client)
    {
        aiio_log_e("mqtt already init \r\n");
        return AIIO_MQTT_MUL_INIT;
    }

    if(config->rev_event_cb == NULL || config->rev_payload_cb == NULL)
    {
        aiio_strncpy(&mqtt_cfg, config->mqtt_config, sizeof(aiio_mqtt_client_config_t));
        mqtt_client = aiio_mqtt_client_init(&mqtt_cfg);
    }
    else
    {
        service_config.rev_event_cb = config->rev_event_cb;
        service_config.rev_payload_cb = config->rev_payload_cb;
        // aiio_strncpy(&mqtt_cfg, config->mqtt_config, sizeof(aiio_mqtt_client_config_t));
        mqtt_cfg.event_handle = mqtt_event_cb;

        if(config->mqtt_config->client_id)
        {
            mqtt_cfg.client_id = config->mqtt_config->client_id;
            aiio_log_i("client_id: %s \r\n", mqtt_cfg.client_id);
        }
        else
        {
            aiio_log_e("client_id is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->username)
        {
            mqtt_cfg.username = config->mqtt_config->username;
            aiio_log_i("username: %s \r\n", mqtt_cfg.username);
        }
        else
        {
            aiio_log_e("username is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->password)
        {
            mqtt_cfg.password = config->mqtt_config->password;
            aiio_log_i("password: %s \r\n", mqtt_cfg.password);
        }
        else
        {
            aiio_log_e("password is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->lwt_topic)
        {
            mqtt_cfg.lwt_topic = config->mqtt_config->lwt_topic;
            aiio_log_i("lwt_topic: %s \r\n", mqtt_cfg.lwt_topic);
        }
        else
        {
            aiio_log_e("lwt_topic is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->lwt_msg)
        {
            mqtt_cfg.lwt_msg = config->mqtt_config->lwt_msg;
            aiio_log_i("lwt_msg: %s \r\n", mqtt_cfg.lwt_msg);
        }
        else
        {
            aiio_log_e("lwt_msg is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->uri)
        {
            mqtt_cfg.uri = config->mqtt_config->uri;
            aiio_log_i("uri: %s \r\n", mqtt_cfg.uri);
        } 
        else if(config->mqtt_config->host)
        {
            mqtt_cfg.host = config->mqtt_config->host;
            aiio_log_i("host: %s \r\n", mqtt_cfg.host);
        }
        else
        {
            aiio_log_e("uri and host is NULL \r\n");
            return AIIO_PARAM_ERR;
        }

        if(config->mqtt_config->cert_pem)
        {
            mqtt_cfg.cert_pem = config->mqtt_config->cert_pem;
        }
        
        mqtt_cfg.lwt_qos = config->mqtt_config->lwt_qos;
        mqtt_cfg.lwt_retain = config->mqtt_config->lwt_retain;
        mqtt_cfg.lwt_msg_len = config->mqtt_config->lwt_msg_len;
        mqtt_cfg.keepalive = config->mqtt_config->keepalive;
        mqtt_cfg.disable_clean_session = config->mqtt_config->disable_clean_session;
        mqtt_cfg.port = config->mqtt_config->port;
        mqtt_cfg.cert_len = config->mqtt_config->cert_len;
        mqtt_cfg.transport = config->mqtt_config->transport;
        
        aiio_log_i("lwt_msg_len: %d \r\n", mqtt_cfg.lwt_msg_len);
        aiio_log_i("keepalive: %d \r\n", mqtt_cfg.keepalive);
        aiio_log_i("port: %d \r\n", mqtt_cfg.port);
        aiio_log_i("transport: %d \r\n", mqtt_cfg.transport);

        mqtt_client = aiio_mqtt_client_init(&mqtt_cfg);
    }

    aiio_mqtt_client_start(mqtt_client);

    return AIIO_CODE_OK;
}


int aiio_mqtt_service_deinit(void)
{
    if(mqtt_client == NULL)
    {
        aiio_log_e("mqtt already deinit \r\n");
        return AIIO_MQTT_MUL_DEINIT;
    }

    aiio_mqtt_client_disconnect(mqtt_client);
    aiio_mqtt_client_stop(mqtt_client);
    aiio_mqtt_client_destroy(mqtt_client);

    mqtt_client = NULL;

    return AIIO_CODE_OK;
}


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        aiio_log_e("Last error %s: 0x%x", message, error_code);
    }
}


static aiio_err_t mqtt_event_cb(aiio_mqtt_event_handle_t event)
{
    int32_t event_id;
    int msg_id;
    aiio_mqtt_service_event_t service_event = {0};

    service_event.msg_id = event->msg_id;
    service_event.event_id = event->event_id;
    service_event.qos = event->qos;
    service_event.topic = event->topic;
    service_event.topic_len = event->topic_len;

    if(service_config.rev_event_cb)
    {
        service_config.rev_event_cb(&service_event);
    }
    event_id = event->event_id;
    aiio_log_i("Event dispatched, event_id=%d \r\n", event_id);
    
    switch ((aiio_mqtt_event_id_t)event_id) 
    {
        case MQTT_EVENT_CONNECTED:
            aiio_log_i("MQTT_EVENT_CONNECTED \r\n");

            break;
        case MQTT_EVENT_DISCONNECTED:
            aiio_log_i("MQTT_EVENT_DISCONNECTED \r\n");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            aiio_log_i("MQTT_EVENT_SUBSCRIBED, msg_id=%d\r\n", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            aiio_log_i("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d\r\n", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            aiio_log_i("MQTT_EVENT_PUBLISHED, msg_id=%d\r\n", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            aiio_log_i("MQTT_EVENT_DATA \r\n");
            aiio_log_i("MQTT_EVENT_DATA, msg_id=%d\r\n", event->msg_id);
            if(service_config.rev_payload_cb)
            {
                aiio_JsonDataParse(event->data, event->data_len, service_config.rev_payload_cb);
            }
            break;
        case MQTT_EVENT_ERROR:
            aiio_log_i("MQTT_EVENT_ERROR \r\n");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) 
            {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->aiio_tls_last_aiio_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->aiio_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->aiio_transport_sock_errno);
                aiio_log_i("Last errno string (%s)", strerror(event->error_handle->aiio_transport_sock_errno));
            }
            break;
        default:
            aiio_log_i("Other event id:%d\r\n", event->event_id);
            break;
    }

    return AIIO_CODE_OK;
}


int aiio_mqtt_service_subscribe(char *topic, int qos)
{
    if(topic == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(mqtt_client == NULL)
    {
        aiio_log_e("mqtt service not init \r\n");
        return AIIO_CLOUD_NOT_INIT;
    }

    return aiio_mqtt_client_subscribe(mqtt_client, topic, qos);
}


int aiio_mqtt_service_publish(char *topic, char *data, int len, int qos, int retain)
{
    if(topic == NULL || data == NULL)
    {
        aiio_log_e("param err \r\n");
        return AIIO_PARAM_ERR;
    }

    if(mqtt_client == NULL)
    {
        aiio_log_e("mqtt service not init \r\n");
        return AIIO_CLOUD_NOT_INIT;
    }

    return aiio_mqtt_client_publish(mqtt_client, topic , data , len, qos, retain);
}

