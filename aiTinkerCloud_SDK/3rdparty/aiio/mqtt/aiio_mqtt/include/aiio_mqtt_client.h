/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 * Tuan PM <tuanpm at live dot com>
 */

#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "aiio_err.h"
#include "aiio_bit_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_SUPPORTED_FEATURE_WS_SUBPROTOCOL
#define MQTT_SUPPORTED_FEATURE_PSK_AUTHENTICATION
#define MQTT_SUPPORTED_FEATURE_ALPN
#define MQTT_SUPPORTED_FEATURE_CLIENT_KEY_PASSWORD


#ifndef AIIO_EVENT_DECLARE_BASE
// Define event loop types if macros not available
typedef void *aiio_event_loop_handle_t;
typedef void *aiio_event_handler_t;
#endif

typedef const char*  aiio_event_base_t; /**< unique pointer to a subsystem that exposes events */


typedef struct aiio_mqtt_client *aiio_mqtt_client_handle_t;

/**
 * @brief MQTT event types.
 *
 * User event handler receives context data in `aiio_mqtt_event_t` structure with
 *  - `user_context` - user data from `aiio_mqtt_client_config_t`
 *  - `client` - mqtt client handle
 *  - various other data depending on event type
 *
 */
typedef enum {
    MQTT_EVENT_ANY = -1,
    MQTT_EVENT_ERROR = 0,          /*!< on error event, additional context: connection return code, error handle from aiio_tls (if supported) */
    MQTT_EVENT_CONNECTED,          /*!< connected event, additional context: session_present flag */
    MQTT_EVENT_DISCONNECTED,       /*!< disconnected event */
    MQTT_EVENT_SUBSCRIBED,         /*!< subscribed event, additional context:
                                        - msg_id               message id
                                        - data                 pointer to the received data
                                        - data_len             length of the data for this event
                                        */
    MQTT_EVENT_UNSUBSCRIBED,       /*!< unsubscribed event */
    MQTT_EVENT_PUBLISHED,          /*!< published event, additional context:  msg_id */
    MQTT_EVENT_DATA,               /*!< data event, additional context:
                                        - msg_id               message id
                                        - topic                pointer to the received topic
                                        - topic_len            length of the topic
                                        - data                 pointer to the received data
                                        - data_len             length of the data for this event
                                        - current_data_offset  offset of the current data for this event
                                        - total_data_len       total length of the data received
                                        - retain               retain flag of the message
                                        - qos                  qos level of the message
                                        - dup                  dup flag of the message
                                        Note: Multiple MQTT_EVENT_DATA could be fired for one message, if it is
                                        longer than internal buffer. In that case only first event contains topic
                                        pointer and length, other contain data only with current data length
                                        and current data offset updating.
                                         */
    MQTT_EVENT_BEFORE_CONNECT,     /*!< The event occurs before connecting */
    MQTT_EVENT_DELETED,            /*!< Notification on delete of one message from the internal outbox,
                                        if the message couldn't have been sent and acknowledged before expiring
                                        defined in OUTBOX_EXPIRED_TIMEOUT_MS.
                                        (events are not posted upon deletion of successfully acknowledged messages)
                                        - This event id is posted only if MQTT_REPORT_DELETED_MESSAGES==1
                                        - Additional context: msg_id (id of the deleted message).
                                        */
} aiio_mqtt_event_id_t;

/**
 * MQTT connection error codes propagated via ERROR event
 */
typedef enum {
    MQTT_CONNECTION_ACCEPTED = 0,                   /*!< Connection accepted  */
    MQTT_CONNECTION_REFUSE_PROTOCOL,                /*!< MQTT connection refused reason: Wrong protocol */
    MQTT_CONNECTION_REFUSE_ID_REJECTED,             /*!< MQTT connection refused reason: ID rejected */
    MQTT_CONNECTION_REFUSE_SERVER_UNAVAILABLE,      /*!< MQTT connection refused reason: Server unavailable */
    MQTT_CONNECTION_REFUSE_BAD_USERNAME,            /*!< MQTT connection refused reason: Wrong user */
    MQTT_CONNECTION_REFUSE_NOT_AUTHORIZED           /*!< MQTT connection refused reason: Wrong username or password */
} aiio_mqtt_connect_return_code_t;

/**
 * MQTT connection error codes propagated via ERROR event
 */
typedef enum {
    MQTT_ERROR_TYPE_NONE = 0,
    MQTT_ERROR_TYPE_TCP_TRANSPORT,
    MQTT_ERROR_TYPE_CONNECTION_REFUSED,
} aiio_mqtt_error_type_t;

/**
 * MQTT_ERROR_TYPE_TCP_TRANSPORT error type hold all sorts of transport layer errors,
 * including AIIO-TLS error, but in the past only the errors from MQTT_ERROR_TYPE_AIIO_TLS layer
 * were reported, so the AIIO-TLS error type is re-defined here for backward compatibility
 */
#define MQTT_ERROR_TYPE_AIIO_TLS MQTT_ERROR_TYPE_TCP_TRANSPORT

typedef enum {
    MQTT_TRANSPORT_UNKNOWN = 0x0,
    MQTT_TRANSPORT_OVER_TCP,      /*!< MQTT over TCP, using scheme: ``mqtt`` */
    MQTT_TRANSPORT_OVER_SSL,      /*!< MQTT over SSL, using scheme: ``mqtts`` */
    MQTT_TRANSPORT_OVER_WS,       /*!< MQTT over Websocket, using scheme:: ``ws`` */
    MQTT_TRANSPORT_OVER_WSS       /*!< MQTT over Websocket Secure, using scheme: ``wss`` */
} aiio_mqtt_transport_t;

/**
 *  MQTT protocol version used for connection
 */
typedef enum {
    MQTT_PROTOCOL_UNDEFINED = 0,
    MQTT_PROTOCOL_V_3_1,
    MQTT_PROTOCOL_V_3_1_1
} aiio_mqtt_protocol_ver_t;

/**
 * @brief MQTT error code structure to be passed as a contextual information into ERROR event
 *
 * Important: This structure extends `aiio_tls_last_error` error structure and is backward compatible with it
 * (so might be down-casted and treated as `aiio_tls_last_error` error, but recommended to update applications if used this way previously)
 *
 * Use this structure directly checking error_type first and then appropriate error code depending on the source of the error:
 *
 * | error_type | related member variables | note |
 * | MQTT_ERROR_TYPE_TCP_TRANSPORT | aiio_tls_last_aiio_err, aiio_tls_stack_err, aiio_tls_cert_verify_flags, sock_errno | Error reported from tcp_transport/esp-tls |
 * | MQTT_ERROR_TYPE_CONNECTION_REFUSED | connect_return_code | Internal error reported from MQTT broker on connection |
 */
typedef struct aiio_mqtt_error_codes {
    /* compatible portion of the struct corresponding to struct aiio_tls_last_error */
    aiio_err_t aiio_tls_last_aiio_err;              /*!< last aiio_err code reported from esp-tls component */
    int       aiio_tls_stack_err;                 /*!< tls specific error code reported from underlying tls stack */
    int       aiio_tls_cert_verify_flags;         /*!< tls flags reported from underlying tls stack during certificate verification */
    /* esp-mqtt specific structure extension */
    aiio_mqtt_error_type_t error_type;            /*!< error type referring to the source of the error */
    aiio_mqtt_connect_return_code_t connect_return_code; /*!< connection refused error code reported from MQTT broker on connection */
    /* tcp_transport extension */
    int       aiio_transport_sock_errno;         /*!< errno from the underlying socket */

} aiio_mqtt_error_codes_t;

/**
 * MQTT event configuration structure
 */
typedef struct {
    aiio_mqtt_event_id_t event_id;       /*!< MQTT event type */
    aiio_mqtt_client_handle_t client;    /*!< MQTT client handle for this event */
    void *user_context;                 /*!< User context passed from MQTT client config */
    char *data;                         /*!< Data associated with this event */
    int data_len;                       /*!< Length of the data for this event */
    int total_data_len;                 /*!< Total length of the data (longer data are supplied with multiple events) */
    int current_data_offset;            /*!< Actual offset for the data associated with this event */
    char *topic;                        /*!< Topic associated with this event */
    int topic_len;                      /*!< Length of the topic for this event associated with this event */
    int msg_id;                         /*!< MQTT messaged id of message */
    int session_present;                /*!< MQTT session_present flag for connection event */
    aiio_mqtt_error_codes_t *error_handle; /*!< esp-mqtt error handle including esp-tls errors as well as internal mqtt errors */
    bool retain;                        /*!< Retained flag of the message associated with this event */
    int qos;                            /*!< qos of the messages associated with this event */
    bool dup;                           /*!< dup flag of the message associated with this event */
} aiio_mqtt_event_t;

typedef aiio_mqtt_event_t *aiio_mqtt_event_handle_t;

typedef aiio_err_t (* mqtt_event_callback_t)(aiio_mqtt_event_handle_t event);

/**
 * MQTT client configuration structure
 */
typedef struct {
    mqtt_event_callback_t event_handle;     /*!< handle for MQTT events as a callback in legacy mode */
    aiio_event_loop_handle_t event_loop_handle; /*!< handle for MQTT event loop library */
    const char *host;                       /*!< MQTT server domain (ipv4 as string) */
    const char *uri;                        /*!< Complete MQTT broker URI */
    uint32_t port;                          /*!< MQTT server port */
    bool set_null_client_id;                /*!< Selects a NULL client id */
    const char *client_id;                  /*!< Set client id.
                                                 Ignored if set_null_client_id == true
                                                 If NULL set the default client id.
                                                 Default client id is ``AIIO_%CHIPID%`` where %CHIPID% are last 3 bytes of MAC address in hex format */
    const char *username;                   /*!< MQTT username */
    const char *password;                   /*!< MQTT password */
    const char *lwt_topic;                  /*!< LWT (Last Will and Testament) message topic (NULL by default) */
    const char *lwt_msg;                    /*!< LWT message (NULL by default) */
    int lwt_qos;                            /*!< LWT message qos */
    int lwt_retain;                         /*!< LWT retained message flag */
    int lwt_msg_len;                        /*!< LWT message length */
    int disable_clean_session;              /*!< mqtt clean session, default clean_session is true */
    int keepalive;                          /*!< mqtt keepalive, default is 120 seconds */
    bool disable_auto_reconnect;            /*!< this mqtt client will reconnect to server (when errors/disconnect). Set disable_auto_reconnect=true to disable */
    void *user_context;                     /*!< pass user context to this option, then can receive that context in ``event->user_context`` */
    int task_prio;                          /*!< MQTT task priority, default is 5, can be changed in ``make menuconfig`` */
    int task_stack;                         /*!< MQTT task stack size, default is 6144 bytes, can be changed in ``make menuconfig`` */
    int buffer_size;                        /*!< size of MQTT send/receive buffer, default is 1024 (only receive buffer size if ``out_buffer_size`` defined) */
    const char *cert_pem;                   /*!< Pointer to certificate data in PEM or DER format for server verify (with SSL), default is NULL, not required to verify the server. PEM-format must have a terminating NULL-character. DER-format requires the length to be passed in cert_len. */
    size_t cert_len;                        /*!< Length of the buffer pointed to by cert_pem. May be 0 for null-terminated pem */
    const char *client_cert_pem;            /*!< Pointer to certificate data in PEM or DER format for SSL mutual authentication, default is NULL, not required if mutual authentication is not needed. If it is not NULL, also `client_key_pem` has to be provided. PEM-format must have a terminating NULL-character. DER-format requires the length to be passed in client_cert_len. */
    size_t client_cert_len;                 /*!< Length of the buffer pointed to by client_cert_pem. May be 0 for null-terminated pem */
    const char *client_key_pem;             /*!< Pointer to private key data in PEM or DER format for SSL mutual authentication, default is NULL, not required if mutual authentication is not needed. If it is not NULL, also `client_cert_pem` has to be provided. PEM-format must have a terminating NULL-character. DER-format requires the length to be passed in client_key_len */
    size_t client_key_len;                  /*!< Length of the buffer pointed to by client_key_pem. May be 0 for null-terminated pem */
    aiio_mqtt_transport_t transport;         /*!< overrides URI transport */
    int refresh_connection_after_ms;        /*!< Refresh connection after this value (in milliseconds) */
    const struct psk_key_hint *psk_hint_key;     /*!< Pointer to PSK struct defined in aiio_tls.h to enable PSK authentication (as alternative to certificate verification). If not NULL and server/client certificates are NULL, PSK is enabled */
    bool          use_global_ca_store;      /*!< Use a global ca_store for all the connections in which this bool is set. */
    aiio_err_t (*crt_bundle_attach)(void *conf); /*!< Pointer to x509 Certificate Bundle attach function for the usage of certification bundles in mqtts */
    int reconnect_timeout_ms;               /*!< Reconnect to the broker after this value in miliseconds if auto reconnect is not disabled (defaults to 10s) */
    const char **alpn_protos;               /*!< NULL-terminated list of supported application protocols to be used for ALPN */
    const char *clientkey_password;         /*!< Client key decryption password string */
    int clientkey_password_len;             /*!< String length of the password pointed to by clientkey_password */
    aiio_mqtt_protocol_ver_t protocol_ver;   /*!< MQTT protocol version used for connection, defaults to value from menuconfig*/
    int out_buffer_size;                    /*!< size of MQTT output buffer. If not defined, both output and input buffers have the same size defined as ``buffer_size`` */
    bool skip_cert_common_name_check;       /*!< Skip any validation of server certificate CN field, this reduces the security of TLS and makes the mqtt client susceptible to MITM attacks  */
    bool use_secure_element;                /*!< enable secure element for enabling SSL connection */
    void *ds_data;                          /*!< carrier of handle for digital signature parameters */
    int network_timeout_ms;                 /*!< Abort network operation if it is not completed after this value, in milliseconds (defaults to 10s) */
    bool disable_keepalive;                 /*!< Set disable_keepalive=true to turn off keep-alive mechanism, false by default (keepalive is active by default). Note: setting the config value `keepalive` to `0` doesn't disable keepalive feature, but uses a default keepalive period */
    const char *path;                       /*!< Path in the URI*/
    int message_retransmit_timeout;         /*!< timeout for retansmit of failded packet */
} aiio_mqtt_client_config_t;

/**
 * @brief Creates mqtt client handle based on the configuration
 *
 * @param config    mqtt configuration structure
 *
 * @return mqtt_client_handle if successfully created, NULL on error
 */
aiio_mqtt_client_handle_t aiio_mqtt_client_init(const aiio_mqtt_client_config_t *config);

/**
 * @brief Sets mqtt connection URI. This API is usually used to overrides the URI
 * configured in aiio_mqtt_client_init
 *
 * @param client    mqtt client handle
 * @param uri
 *
 * @return AIIO_FAIL if URI parse error, AIIO_OK on success
 */
aiio_err_t aiio_mqtt_client_set_uri(aiio_mqtt_client_handle_t client, const char *uri);

/**
 * @brief Starts mqtt client with already created client handle
 *
 * @param client    mqtt client handle
 *
 * @return AIIO_OK on success
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 *         AIIO_FAIL on other error
 */
aiio_err_t aiio_mqtt_client_start(aiio_mqtt_client_handle_t client);

/**
 * @brief This api is typically used to force reconnection upon a specific event
 *
 * @param client    mqtt client handle
 *
 * @return AIIO_OK on success
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 *         AIIO_FAIL if client is in invalid state
 */
aiio_err_t aiio_mqtt_client_reconnect(aiio_mqtt_client_handle_t client);

/**
 * @brief This api is typically used to force disconnection from the broker
 *
 * @param client    mqtt client handle
 *
 * @return AIIO_OK on success
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 */
aiio_err_t aiio_mqtt_client_disconnect(aiio_mqtt_client_handle_t client);

/**
 * @brief Stops mqtt client tasks
 *
 *  * Notes:
 *  - Cannot be called from the mqtt event handler
 *
 * @param client    mqtt client handle
 *
 * @return AIIO_OK on success
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 *         AIIO_FAIL if client is in invalid state
 */
aiio_err_t aiio_mqtt_client_stop(aiio_mqtt_client_handle_t client);

/**
 * @brief Subscribe the client to defined topic with defined qos
 *
 * Notes:
 * - Client must be connected to send subscribe message
 * - This API is could be executed from a user task or
 * from a mqtt event callback i.e. internal mqtt task
 * (API is protected by internal mutex, so it might block
 * if a longer data receive operation is in progress.
 *
 * @param client    mqtt client handle
 * @param topic
 * @param qos
 *
 * @return message_id of the subscribe message on success
 *         -1 on failure
 */
int aiio_mqtt_client_subscribe(aiio_mqtt_client_handle_t client, const char *topic, int qos);

/**
 * @brief Unsubscribe the client from defined topic
 *
 * Notes:
 * - Client must be connected to send unsubscribe message
 * - It is thread safe, please refer to `aiio_mqtt_client_subscribe` for details
 *
 * @param client    mqtt client handle
 * @param topic
 *
 * @return message_id of the subscribe message on success
 *         -1 on failure
 */
int aiio_mqtt_client_unsubscribe(aiio_mqtt_client_handle_t client, const char *topic);

/**
 * @brief Client to send a publish message to the broker
 *
 * Notes:
 * - This API might block for several seconds, either due to network timeout (10s)
 *   or if publishing payloads longer than internal buffer (due to message
 *   fragmentation)
 * - Client doesn't have to be connected for this API to work, enqueueing the messages
 *   with qos>1 (returning -1 for all the qos=0 messages if disconnected).
 *   If MQTT_SKIP_PUBLISH_IF_DISCONNECTED is enabled, this API will not attempt to publish
 *   when the client is not connected and will always return -1.
 * - It is thread safe, please refer to `aiio_mqtt_client_subscribe` for details
 *
 * @param client    mqtt client handle
 * @param topic     topic string
 * @param data      payload string (set to NULL, sending empty payload message)
 * @param len       data length, if set to 0, length is calculated from payload string
 * @param qos       qos of publish message
 * @param retain    retain flag
 *
 * @return message_id of the publish message (for QoS 0 message_id will always be zero) on success.
 *         -1 on failure.
 */
int aiio_mqtt_client_publish(aiio_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain);

/**
 * @brief Enqueue a message to the outbox, to be sent later. Typically used for messages with qos>0, but could
 * be also used for qos=0 messages if store=true.
 *
 * This API generates and stores the publish message into the internal outbox and the actual sending
 * to the network is performed in the mqtt-task context (in contrast to the aiio_mqtt_client_publish()
 * which sends the publish message immediately in the user task's context).
 * Thus, it could be used as a non blocking version of aiio_mqtt_client_publish().
 *
 * @param client    mqtt client handle
 * @param topic     topic string
 * @param data      payload string (set to NULL, sending empty payload message)
 * @param len       data length, if set to 0, length is calculated from payload string
 * @param qos       qos of publish message
 * @param retain    retain flag
 * @param store     if true, all messages are enqueued; otherwise only qos1 and qos 2 are enqueued
 *
 * @return message_id if queued successfully, -1 otherwise
 */
int aiio_mqtt_client_enqueue(aiio_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain, bool store);

/**
 * @brief Destroys the client handle
 *
 * Notes:
 *  - Cannot be called from the mqtt event handler
 *
 * @param client    mqtt client handle
 *
 * @return AIIO_OK
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 */
aiio_err_t aiio_mqtt_client_destroy(aiio_mqtt_client_handle_t client);

/**
 * @brief Set configuration structure, typically used when updating the config (i.e. on "before_connect" event
 *
 * @param client    mqtt client handle
 *
 * @param config    mqtt configuration structure
 *
 * @return AIIO_ERR_NO_MEM if failed to allocate
 *         AIIO_ERR_INVALID_ARG if conflicts on transport configuration.
 *         AIIO_OK on success
 */
aiio_err_t aiio_mqtt_set_config(aiio_mqtt_client_handle_t client, const aiio_mqtt_client_config_t *config);

/**
 * @brief Registers mqtt event
 *
 * @param client            mqtt client handle
 * @param event             event type
 * @param event_handler     handler callback
 * @param event_handler_arg handlers context
 *
 * @return AIIO_ERR_NO_MEM if failed to allocate
 *         AIIO_ERR_INVALID_ARG on wrong initialization
 *         AIIO_OK on success
 */
aiio_err_t aiio_mqtt_client_register_event(aiio_mqtt_client_handle_t client, aiio_mqtt_event_id_t event, aiio_event_handler_t event_handler, void *event_handler_arg);

/**
 * @brief Get outbox size
 *
 * @param client            mqtt client handle
 * @return outbox size
 *         0 on wrong initialization
 */
int aiio_mqtt_client_get_outbox_size(aiio_mqtt_client_handle_t client);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
