/**
 * @brief   Declare the interface of Cloud communication
 * 
 * @file    aiio_wan_service.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note       Declare the protocol interface for commnunicating with the cloud
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-14          <td>1.0.0            <td>zhuolm             <td> The protocol interface of Cloud communication 
 */
#ifndef __AIIO_WAN_SERVICE_H_
#define __AIIO_WAN_SERVICE_H_
#include "aiio_mqtt_service.h"





#define     SERVICE_TASK_PRIORITY                           (5)
#define     SERVICE_TASK_STACK                              (4 * 1024)

 
typedef struct aiio_service *aiio_service_handle_t;


/**
 * @brief Receive event command for cloud
*/
typedef enum
{
    AIIO_SERVICE_INVAILD_EVENT = 0,             /*!< invalid event*/
    AIIO_SERVICE_LANCH_FAIL_EVNET,              /*!< Mqtt connected fail event, it will callback the event when the time of mqtt connection is more than keepalive time, and then call aiio_service_deinit()*/
    AIIO_SERVICE_SUBSCRIBE_TIMEOUT,             /*!< After mqtt is connected, it will be subscribed some topic that it have to be subscribed, it wil callback this event when the time of subscribing is more than twice keepalive time, and then call aiio_service_deinit() */
    AIIO_SERVICE_MQTT_DISCONNECT,               /*!< it will callback this event when the mqtt is suddenly disconnected or disconnected by exception, and then call aiio_service_deinit()*/
    AIIO_SERVICE_MQTT_CONNECTING,               /*!< it will callback this event when the mqtt is connecting*/
    AIIO_SERVICE_MQTT_CONNECTED,                /*!< it will callback this event when the mqtt is connected*/
    AIIO_SERVICE_ACTIVITY_EVENT,                /*!< it will callback this event when it publish activity information to cloud. And it have to publish activity information to cloud that the device is in the distribution network state*/
    AIIO_SERVICE_RESTORE_EVENT,                 /*!< it wil callback this event when it is received restore command from cloud. The device have to restore the facory when receives this event, which need to clean all running data in device*/
    AIIO_SERVICE_REBOOT_EVENT,                  /*!< it will callback this event when it is received reboot command from cloud. The device have to reboot when receives this event*/
    AIIO_SERVICE_INFO_EVENT,                    /*!< it will callback this event when it is received request-information command from cloud, which auto to publish device information to cloud*/
    AIIO_SERVICE_CONFIG_EVENT,                  /*!< it will callback this event when received configuration command from cloud. The device have to parse configuration command, and it will be update keepalive time, mqtt connection timeout time and auto to report attribute for cloud. And then the mqtt needs to reconnect by device.*/
    AIIO_SERVICE_UPGRADE_EVENT,                 /*!< it will callback this event when received OTA information command from cloud*/
    AIIO_SERVICE_QUERY_EVENT,                   /*!< it will callback this event when received request-attribute data from cloud, The device have to report all attribute status to cloud*/
    AIIO_SERVICE_CONTROL_EVENT,                 /*!< it will callback this event when received control-attribute status from cloud. The device have to change the status of attribute, and report control-attribute status to cloud*/
    AIIO_SERVICE_UTC_EVENT,                      /*!< it will callback this event when received utc update command from cloud. The device have to update local real-time time by the utc data*/
    AIIO_SERVICE_ONLINE_EVENT
}aiio_service_event_t;



/**
 * @brief   The unique data structure of the device
 * 
 */
typedef struct 
{
    char *deviceid;                         /*!< client ID data*/
    char *productid;                        /*!< product ID data*/
    char *user_name;                        /*!< MQTT user name data*/
    char *passwd;                           /*!< MQTT password data*/
}aiio_service_data_t;


/**
 * @brief   device information structure
 * 
 */
typedef struct
{
    char *fwVer;                        /*!< The device firmware version*/
    char *mcuVer;                       /*!< The third-party device firmware version, such as the MCU communicates with module, the element is the MCU firmware version */
    char *devType;                      /*!< The type of chip platform, for example: bl602 chip, the element is bl602*/
    char *mac;                          /*!< The mac of chip */
}aiio_service_dev_info_t;

/**
 * @brief   device network information structure
 * 
 */
typedef struct 
{
    char *host;                                 /*!< The ip address of the remote server*/
    char *url;                                  /*!< The domain name of the remote server*/
    uint32_t port;                              /*!< Port number of the remote server*/
    int keepalive;                              /*!< The keepalive time of mqtt*/
    char *cert_pem;                             /*!< Certificate data*/
    uint32_t cert_len;                          /*!< The length of Certificate data*/
    aiio_mqtt_transport_t transport;            /*!< The type of mqtt communication，The type is defined in this file that is called aiio_mqtt_client.h, which is defined in this enumeration struction that is called aiio_mqtt_protocol_ver_t*/
}aiio_service_netif_t;


/**
 * @brief   attribute data and event command structure
 * 
 */
typedef struct 
{
    char *data;                                 /*!< attribute data*/
    char *msgMid;                               /*!< The message ID of the attribute data or command that is received from cloud, which is different every time */
    char *from;                                 /*!< The source of attribute data or command, which is come from device, app, or cloud*/
    uint32_t data_len;                          /*!< The length of attribute data*/
    aiio_service_event_t service_event;         /*!< The type of event, which is defined in structure that called aiio_service_event_t in this file*/
}aiio_service_rev_event_t;


/**
 * @brief  data configuration structure
 * 
 */
typedef struct 
{
    aiio_service_data_t         service_data;                                   /*!< The unique data structure of the device */
    aiio_service_dev_info_t     dev_info;                                       /*!< device information structure，it will be report to cloud*/
    aiio_wifi_info_t            wifi_info;                                      /*!< The information of WiFi, it will be report to cloud inform the routing information that is connected by device. */
    aiio_service_netif_t        service_netif;                                  /*!< The network information, which the device needs to connect.*/
    char                        *token;                                         /*!< The Activation information, which is given by the APP distribution network. it needs to be reported to the cloud for activate the device that is online*/
    int                         task_prio;                                      /*!< Priority of the thread task*/
    int                         task_stack;                                     /*!< The stack for the thread task*/
    int                         mqtt_connect_timeout;                           /*!< The mqtt connection timeout time, if the element is not set, the default timeout is the keepalive time of mqtt, and the unit of the element is second*/
    bool                        disenable_auto_ack;                             /*!< if the element is set to true, the device needs to reply command ack when is received event commed from cloud. Otherwise set to false, device don't need to reply command ack, and the element is set to false by default */          
    bool                        activity;                                       /*!< if the element must be set to false when the deivce is entering distribution network state, and then it will callback this event that is called AIIO_SERVICE_ACTIVITY_EVENT when the device reports the activation information to cloud.
                                                                                    And if the element must be set to true when the device is reconnectting to cloud, which don't report the activation information to cloud again  */
    void (*rev_service_data_cb)(aiio_service_rev_event_t *rev_event);           /*!< The callback function of attribute data and event command*/
}aiio_service_config_t;


/**
 * @brief  cloud connection configuration structure
 * 
 */
typedef struct 
{
    bool                        service_init;                           /*!< The initialize-status*/
    bool                        online_status;                          /*!< The status of device connect to cloud*/
    aiio_service_handle_t       handle;                                 /*!< The handle of process data*/
    aiio_service_config_t       config;                                  /*!< data configuration structure*/
}aiio_service_t;



/**
 * @brief   Initialize cloud communicaion
 * 
 * @param[in]   service_config          data configuration of cloud
 * 
 * @return  int             Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_init(aiio_service_config_t *service_config);

/**
 * @brief   Deinitialize cloud communication, and release memory resource
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_deinit(void);

/**
 * @brief   report device info to cloud
 * @note    1. it must be called when Lan network received AIIO_LAN_INFO_EVENT
 *          2. when Wan network configuration, disenable_auto_ack is setting true, it must be called while receive AIIO_SERVICE_INFO_EVENT
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_device_info(char *msgMid, char *from);

/**
 * @brief   Report the download progress of ota firmware and the status of processing ota firmware 
 * 
 * @param[in]   ota_info   Report ota information to cloud
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_ota_rate(aiio_ota_info_t *ota_info);

/**
 * @brief   Report the status of attribute data to cloud.
 * @note    Attribute data is defined by the demand of function. 
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   device_att          Attribute data in the json string format
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_attribute(char *msgMid, char *from, char *device_att);

/**
 * @brief   It needs to response ack by this function to cloud when is received configuration information from cloud.
 * 
 * @note    The device must call this function to reponse ack to cloud when the disenable_auto_ack in the aiio_service_config_t structure is set to true.
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   code            The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_config_ack(char *msgMid, char *from, int32_t code);

/**
 * @brief   It needs to response ack by this function to cloud when is received control information from cloud.
 * 
 * @note    The device must call this function to reponse ack to cloud when the disenable_auto_ack in the aiio_service_config_t structure is set to true.
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   code            The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_control_ack(char *msgMid, char *from, int32_t code);

/**
 * @brief   It needs to response ack by this function to cloud when is received restore information from cloud.
 * 
 * @note    The device must call this function to reponse ack to cloud when the disenable_auto_ack in the aiio_service_config_t structure is set to true.
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   code            The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_restore_ack(char *msgMid, char *from, int32_t code);

/**
 * @brief   It needs to response ack by this function to cloud when is received reboot information from cloud.
 * 
 * @note    The device must call this function to reponse ack to cloud when the disenable_auto_ack in the aiio_service_config_t structure is set to true.
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   code            The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_reboot_ack(char *msgMid, char *from, int32_t code);

/**
 * @brief   It needs to response ack by this function to cloud when is received ota information from cloud.
 * 
 * @note    The device must call this function to reponse ack to cloud when the disenable_auto_ack in the aiio_service_config_t structure is set to true.
 * 
 * @param[in]   msgMid          The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from            The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   code            The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int aiio_service_report_ota_ack(char *msgMid, char *from, int32_t code);


#endif