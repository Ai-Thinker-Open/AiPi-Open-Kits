/**
 * @brief   Declare the interface of mqtt communication
 * 
 * @file    aiio_mqtt_service.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note     Declare the interface of  mqtt configuration by mqtt library 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The interface of mqtt configuration
 */
#ifndef __AIIO_MQTT_SERVICE_H_
#define __AIIO_MQTT_SERVICE_H_
#include "aiio_protocol_code.h"
#include "aiio_protocol_packet.h"
#include "aiio_protocol_parse.h"
#include "aiio_mqtt_client.h"



/**
 * @brief   The event structure is receive from cloud by mqtt
 */
typedef struct 
{
    aiio_mqtt_event_id_t event_id;       /*!< MQTT event type */
    char *topic;                        /*!< Topic associated with this event */
    int topic_len;                      /*!< Length of the topic for this event associated with this event */
    int msg_id;                         /*!< MQTT messaged id of message */
    int qos;                            /*!< qos of the messages associated with this event */
}aiio_mqtt_service_event_t;



/**
 * @brief   The callback function is receive data from cloud by mqtt
 */
typedef void (*aiio_mqtt_service_event_cb)(aiio_mqtt_service_event_t *service_event);


/**
 * @brief The mqtt communication configuration structure 
*/
typedef struct 
{
    aiio_protocol_payload_cb  rev_payload_cb;           /*!< receive payload data handle*/
    aiio_mqtt_service_event_cb     rev_event_cb;             /*!< receive data event handle*/
    aiio_mqtt_client_config_t   *mqtt_config;                /*!< mqtt configuration structure, set mqtt configuration*/
}aiio_mqtt_service_t;


/**
 * @brief   Initialize mqtt commnunication of cloud 
 * @note   1. if the callback function that the rev_event_cb and the rev_payload_cb in the configuration data is NULL， 
 *            we have to configurate this callback function that the event_cb in the configuration data, it will recelve data from cloud by mqtt, which isn't parse.
 *         2. if the callback function that the rev_event_cb and the rev_payload_cb in the configuration data isn't NULL， which will receive data from by mqtt, which is parse.
 * 
 * @param[in]   config      The configuration of data
 * 
 * @return  int             Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_mqtt_service_init(aiio_mqtt_service_t *config);


/**
 * @brief   Deinitialize mqtt commnunicatio, and release memory resources
 * 
 * @return  int             Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_mqtt_service_deinit(void);


/**
 * @brief   subscribe mqtt topic 
 * 
 * @param[in]   topic           The mqtt topic to be subscribed
 * @param[in]   qos             The message quality for the mqtt topic
 * 
 * @return  int             Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_mqtt_service_subscribe(char *topic, int qos);



/**
 * @brief   Pubish message to cloud according to the topic by mqtt
 * 
 * @param[in]   topic               The mqtt topic to be subscribed
 * @param[in]   data                The message data to be publish
 * @param[in]   len                 The length of the message data
 * @param[in]   qos                 The message quality for the mqtt topic
 * @param[in]   retain              The message data to be publish whether is retain message
 * 
 * @return  int             Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_mqtt_service_publish(char *topic, char *data, int len, int qos, int retain);


#endif