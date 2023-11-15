
#include <string.h>

/* #include "aiio_system.h" */
#include "aiio_log.h"
#include "aiio_check.h"
#include "http_header.h"
#include "aiio_transport.h"
#include "aiio_transport_tcp.h"
#include "http_utils.h"
#include "http_auth.h"
#include "aiio_err.h"
#include "aiio_http_client.h"
#include "errno.h"

#ifdef CONFIG_HTTP_CLIENT_ENABLE_HTTPS
#include "aiio_transport_ssl.h"
#endif

// Adapter for aiio_random()
// #include <bl_sec.h>
// #define aiio_random bl_rand

static const char *TAG = "HTTP_CLIENT";
/**
 * HTTP Buffer
 */
typedef struct {
    char *data;         /*!< The HTTP data received from the server */
    int len;            /*!< The HTTP data len received from the server */
    char *raw_data;     /*!< The HTTP data after decoding */
    char *orig_raw_data;/*!< The Original pointer to HTTP data after decoding */
    int raw_len;        /*!< The HTTP data len after decoding */
    char *output_ptr;   /*!< The destination address of the data to be copied to after decoding */
} aiio_http_buffer_t;

/**
 * private HTTP Data structure
 */
typedef struct {
    http_header_handle_t headers;       /*!< http header */
    aiio_http_buffer_t   *buffer;        /*!< data buffer as linked list */
    int                 status_code;    /*!< status code (integer) */
    int64_t             content_length; /*!< data length */
    int                 chunk_length;   /*!< chunk length */
    int                 data_offset;    /*!< offset to http data (Skip header) */
    int64_t             data_process;   /*!< data processed */
    int                 method;         /*!< http method */
    bool                is_chunked;
} aiio_http_data_t;

typedef struct {
    char                         *url;
    char                         *scheme;
    char                         *host;
    int                          port;
    char                         *username;
    char                         *password;
    char                         *path;
    char                         *query;
    char                         *cert_pem;
    aiio_http_client_method_t     method;
    aiio_http_client_auth_type_t  auth_type;
    aiio_http_client_transport_t  transport_type;
    int                          max_store_header_size;
} connection_info_t;

typedef enum {
    HTTP_STATE_UNINIT = 0,
    HTTP_STATE_INIT,
    HTTP_STATE_CONNECTED,
    HTTP_STATE_REQ_COMPLETE_HEADER,
    HTTP_STATE_REQ_COMPLETE_DATA,
    HTTP_STATE_RES_COMPLETE_HEADER,
    HTTP_STATE_RES_ON_DATA_START,
    HTTP_STATE_RES_COMPLETE_DATA,
    HTTP_STATE_CLOSE
} aiio_http_state_t;
/**
 * HTTP client class
 */
struct aiio_http_client {
    int                         redirect_counter;
    int                         max_redirection_count;
    int                         max_authorization_retries;
    int                         process_again;
    struct http_parser          *parser;
    struct http_parser_settings *parser_settings;
    aiio_transport_list_handle_t     transport_list;
    aiio_transport_handle_t          transport;
    aiio_http_data_t                 *request;
    aiio_http_data_t                 *response;
    void                        *user_data;
    aiio_http_auth_data_t        *auth_data;
    char                        *post_data;
    char                        *location;
    char                        *auth_header;
    char                        *current_header_key;
    char                        *current_header_value;
    int                         post_len;
    connection_info_t           connection_info;
    bool                        is_chunk_complete;
    aiio_http_state_t            state;
    http_event_handle_cb        event_handler;
    int                         timeout_ms;
    int                         buffer_size_rx;
    int                         buffer_size_tx;
    bool                        disable_auto_redirect;
    aiio_http_client_event_t     event;
    int                         data_written_index;
    int                         data_write_left;
    bool                        first_line_prepared;
    int                         header_index;
    bool                        is_async;
    aiio_transport_keep_alive_t  keep_alive_cfg;
    struct ifreq                *if_name;
    unsigned                    cache_data_in_fetch_hdr: 1;
};

typedef struct aiio_http_client aiio_http_client_t;

static int _clear_connection_info(aiio_http_client_handle_t client);
/**
 * Default settings
 */
#define DEFAULT_HTTP_PORT (80)
#define DEFAULT_HTTPS_PORT (443)

#define ASYNC_TRANS_CONNECT_FAIL -1
#define ASYNC_TRANS_CONNECTING 0
#define ASYNC_TRANS_CONNECT_PASS 1

static const char *DEFAULT_HTTP_USER_AGENT = "BL60X HTTP Client/1.0";
static const char *DEFAULT_HTTP_PROTOCOL = "HTTP/1.1";
static const char *DEFAULT_HTTP_PATH = "/";
static const int DEFAULT_MAX_REDIRECT = 10;
static const int DEFAULT_MAX_AUTH_RETRIES = 10;
static const int DEFAULT_TIMEOUT_MS = 5000;
static const int DEFAULT_KEEP_ALIVE_IDLE = 5;
static const int DEFAULT_KEEP_ALIVE_INTERVAL= 5;
static const int DEFAULT_KEEP_ALIVE_COUNT= 3;

static const char *HTTP_METHOD_MAPPING[] = {
    "GET",
    "POST",
    "PUT",
    "PATCH",
    "DELETE",
    "HEAD",
    "NOTIFY",
    "SUBSCRIBE",
    "UNSUBSCRIBE",
    "OPTIONS",
    "COPY",
    "MOVE",
    "LOCK",
    "UNLOCK",
    "PROPFIND",
    "PROPPATCH",
    "MKCOL"
};

static int aiio_http_client_request_send(aiio_http_client_handle_t client, int write_len);
static int aiio_http_client_connect(aiio_http_client_handle_t client);
static int aiio_http_client_send_post_data(aiio_http_client_handle_t client);

static int http_dispatch_event(aiio_http_client_t *client, aiio_http_client_event_id_t event_id, void *data, int len)
{
    aiio_http_client_event_t *event = &client->event;

    if (client->event_handler) {
        event->event_id = event_id;
        event->user_data = client->user_data;
        event->data = data;
        event->data_len = len;
        return client->event_handler(event);
    }
    return AIIO_OK;
}

static int http_on_message_begin(http_parser *parser)
{
    aiio_http_client_t *client = parser->data;
    aiio_log_d( "on_message_begin");

    client->response->is_chunked = false;
    client->is_chunk_complete = false;
    return 0;
}

static int http_on_url(http_parser *parser, const char *at, size_t length)
{
    aiio_log_d( "http_on_url");
    return 0;
}

static int http_on_status(http_parser *parser, const char *at, size_t length)
{
    return 0;
}

static int http_on_header_event(aiio_http_client_handle_t client)
{
    if (client->current_header_key != NULL && client->current_header_value != NULL) {
        aiio_log_d( "HEADER=%s:%s", client->current_header_key, client->current_header_value);
        client->event.header_key = client->current_header_key;
        client->event.header_value = client->current_header_value;
        http_dispatch_event(client, HTTP_EVENT_ON_HEADER, NULL, 0);
        free(client->current_header_key);
        free(client->current_header_value);
        client->current_header_key = NULL;
        client->current_header_value = NULL;
    }
    return 0;
}

static int http_on_header_field(http_parser *parser, const char *at, size_t length)
{
    aiio_http_client_t *client = parser->data;
    http_on_header_event(client);
    http_utils_append_string(&client->current_header_key, at, length);

    return 0;
}

static int http_on_header_value(http_parser *parser, const char *at, size_t length)
{
    aiio_http_client_handle_t client = parser->data;
    if (client->current_header_key == NULL) {
        return 0;
    }
    if (strcasecmp(client->current_header_key, "Location") == 0) {
        http_utils_append_string(&client->location, at, length);
    } else if (strcasecmp(client->current_header_key, "Transfer-Encoding") == 0
               && memcmp(at, "chunked", length) == 0) {
        client->response->is_chunked = true;
    } else if (strcasecmp(client->current_header_key, "WWW-Authenticate") == 0) {
        http_utils_append_string(&client->auth_header, at, length);
    }
    http_utils_append_string(&client->current_header_value, at, length);
    return 0;
}

static int http_on_headers_complete(http_parser *parser)
{
    aiio_http_client_handle_t client = parser->data;
    http_on_header_event(client);
    client->response->status_code = parser->status_code;
    client->response->data_offset = parser->nread;
    client->response->content_length = parser->content_length;
    client->response->data_process = 0;
    aiio_log_d( "http_on_headers_complete, status=%d, offset=%d, nread=%d", parser->status_code, client->response->data_offset, parser->nread);
    client->state = HTTP_STATE_RES_COMPLETE_HEADER;
    if (client->connection_info.method == HTTP_METHOD_HEAD) {
        /* In a HTTP_RESPONSE parser returning '1' from on_headers_complete will tell the
           parser that it should not expect a body. This is used when receiving a response
           to a HEAD request which may contain 'Content-Length' or 'Transfer-Encoding: chunked'
           headers that indicate the presence of a body.*/
        return 1;
    }
    return 0;
}

static int http_on_body(http_parser *parser, const char *at, size_t length)
{
    aiio_http_client_t *client = parser->data;
    aiio_log_d( "http_on_body %d", length);

    if (client->response->buffer->output_ptr) {
        memcpy(client->response->buffer->output_ptr, (char *)at, length);
        client->response->buffer->output_ptr += length;
    } else {
        /* Do not cache body when http_on_body is called from aiio_http_client_perform */
        if (client->state < HTTP_STATE_RES_ON_DATA_START && client->cache_data_in_fetch_hdr) {
            aiio_log_i( "Body received in fetch header state, %p, %d", at, length);
            aiio_http_buffer_t *res_buffer = client->response->buffer;
            assert(res_buffer->orig_raw_data == res_buffer->raw_data);
            res_buffer->orig_raw_data = (char *)realloc(res_buffer->orig_raw_data, res_buffer->raw_len + length);
            if (!res_buffer->orig_raw_data) {
                aiio_log_e( "Failed to allocate memory for storing decoded data");
                return -1;
            }
            memcpy(res_buffer->orig_raw_data + res_buffer->raw_len, at, length);
            res_buffer->raw_data = res_buffer->orig_raw_data;
        }
    }

    client->response->data_process += length;
    client->response->buffer->raw_len += length;
    http_dispatch_event(client, HTTP_EVENT_ON_DATA, (void *)at, length);
    return 0;
}

static int http_on_message_complete(http_parser *parser)
{
    aiio_log_d( "http_on_message_complete, parser=%x", (int)parser);
    aiio_http_client_handle_t client = parser->data;
    client->is_chunk_complete = true;
    return 0;
}

static int http_on_chunk_complete(http_parser *parser)
{
    aiio_log_d( "http_on_chunk_complete");
    return 0;
}

static int http_on_chunk_header(http_parser *parser)
{
    aiio_http_client_handle_t client = parser->data;
    client->response->chunk_length = parser->content_length;
    aiio_log_d( "http_on_chunk_header, chunk_length");
    return 0;
}

int aiio_http_client_set_header(aiio_http_client_handle_t client, const char *key, const char *value)
{
    return http_header_set(client->request->headers, key, value);
}

int aiio_http_client_get_header(aiio_http_client_handle_t client, const char *key, char **value)
{
    return http_header_get(client->request->headers, key, value);
}

int aiio_http_client_delete_header(aiio_http_client_handle_t client, const char *key)
{
    return http_header_delete(client->request->headers, key);
}

int aiio_http_client_get_username(aiio_http_client_handle_t client, char **value)
{
    if (client == NULL || value == NULL) {
        aiio_log_e( "client or value must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    *value = client->connection_info.username;
    return AIIO_OK;
}

int aiio_http_client_set_username(aiio_http_client_handle_t client, const char *username)
{
    if (client == NULL) {
        aiio_log_e( "client must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    if (client->connection_info.username != NULL) {
        free(client->connection_info.username);
    }
    client->connection_info.username = username ? strdup(username) : NULL;
    return AIIO_OK;
}

int aiio_http_client_get_password(aiio_http_client_handle_t client, char **value)
{
    if (client == NULL || value == NULL) {
        aiio_log_e( "client or value must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    *value = client->connection_info.password;
    return AIIO_OK;
}

int aiio_http_client_set_password(aiio_http_client_handle_t client, const char *password)
{
    if (client == NULL) {
        aiio_log_e( "client must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    if (client->connection_info.password != NULL) {
        memset(client->connection_info.password, 0, strlen(client->connection_info.password));
        free(client->connection_info.password);
    }
    client->connection_info.password = password ? strdup(password) : NULL;
    return AIIO_OK;
}

int aiio_http_client_set_authtype(aiio_http_client_handle_t client, aiio_http_client_auth_type_t auth_type)
{
    if (client == NULL) {
        aiio_log_e( "client must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    client->connection_info.auth_type = auth_type;
    return AIIO_OK;
}

static int _set_config(aiio_http_client_handle_t client, const aiio_http_client_config_t *config)
{
    int ret = AIIO_OK;
    client->connection_info.method = config->method;
    client->connection_info.port = config->port;
    client->connection_info.auth_type = config->auth_type;
    client->event_handler = config->event_handler;
    client->timeout_ms = config->timeout_ms;
    client->max_redirection_count = config->max_redirection_count;
    client->max_authorization_retries = config->max_authorization_retries;
    client->user_data = config->user_data;
    client->buffer_size_rx = config->buffer_size;
    client->buffer_size_tx = config->buffer_size_tx;
    client->disable_auto_redirect = config->disable_auto_redirect;

    if (config->buffer_size == 0) {
        client->buffer_size_rx = DEFAULT_HTTP_BUF_SIZE;
    }

    if (config->buffer_size_tx == 0) {
        client->buffer_size_tx = DEFAULT_HTTP_BUF_SIZE;
    }

    if (client->max_redirection_count == 0) {
        client->max_redirection_count = DEFAULT_MAX_REDIRECT;
    }

    if (client->max_authorization_retries == 0) {
        client->max_authorization_retries = DEFAULT_MAX_AUTH_RETRIES;
    } else if (client->max_authorization_retries == -1) {
        client->max_authorization_retries = 0;
    }

    if (config->path) {
        client->connection_info.path = strdup(config->path);
    } else {
        client->connection_info.path = strdup(DEFAULT_HTTP_PATH);
    }
    AIIO_RETURN_ON_FALSE(client->connection_info.path, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");

    if (config->host) {
        client->connection_info.host = strdup(config->host);

        AIIO_GOTO_ON_FALSE(client->connection_info.host, AIIO_ERR_NO_MEM, error, TAG, "Memory exhausted");
    }

    if (config->query) {
        client->connection_info.query = strdup(config->query);
        AIIO_GOTO_ON_FALSE(client->connection_info.query, AIIO_ERR_NO_MEM, error, TAG, "Memory exhausted");
    }

    if (config->username) {
        client->connection_info.username = strdup(config->username);
        AIIO_GOTO_ON_FALSE(client->connection_info.username, AIIO_ERR_NO_MEM, error, TAG, "Memory exhausted");
    }

    if (config->password) {
        client->connection_info.password = strdup(config->password);
        AIIO_GOTO_ON_FALSE(client->connection_info.password, AIIO_ERR_NO_MEM, error, TAG, "Memory exhausted");
    }

    if (config->transport_type == HTTP_TRANSPORT_OVER_SSL) {
        http_utils_assign_string(&client->connection_info.scheme, "https", -1);
        if (client->connection_info.port == 0) {
            client->connection_info.port = DEFAULT_HTTPS_PORT;
        }
    } else {
        http_utils_assign_string(&client->connection_info.scheme, "http", -1);
        if (client->connection_info.port == 0) {
            client->connection_info.port = DEFAULT_HTTP_PORT;
        }
    }
    if (client->timeout_ms == 0) {
        client->timeout_ms = DEFAULT_TIMEOUT_MS;
    }
    if (config->is_async) {
        client->is_async = true;
    }

    return ret;

error:
    _clear_connection_info(client);
    return ret;
}

static int _clear_connection_info(aiio_http_client_handle_t client)
{
    free(client->connection_info.path);
    free(client->connection_info.host);
    free(client->connection_info.query);
    free(client->connection_info.username);
    if (client->connection_info.password) {
        memset(client->connection_info.password, 0, strlen(client->connection_info.password));
        free(client->connection_info.password);
    }
    free(client->connection_info.scheme);
    free(client->connection_info.url);
    memset(&client->connection_info, 0, sizeof(connection_info_t));
    return AIIO_OK;
}

static int _clear_auth_data(aiio_http_client_handle_t client)
{
    if (client->auth_data == NULL) {
        return AIIO_FAIL;
    }

    free(client->auth_data->method);
    free(client->auth_data->realm);
    free(client->auth_data->algorithm);
    free(client->auth_data->qop);
    free(client->auth_data->nonce);
    free(client->auth_data->opaque);
    memset(client->auth_data, 0, sizeof(aiio_http_auth_data_t));
    return AIIO_OK;
}

static int aiio_http_client_prepare(aiio_http_client_handle_t client)
{
    client->process_again = 0;
    client->response->data_process = 0;
    client->first_line_prepared = false;
    /**
     * Clear location field before making a new HTTP request. Location
     * field should not be cleared in http_on_header* callbacks because
     * callbacks can be invoked multiple times for same header, and
     * hence can lead to data corruption.
     */
    if (client->location != NULL) {
        free(client->location);
        client->location = NULL;
    }
    http_parser_init(client->parser, HTTP_RESPONSE);
    if (client->connection_info.username) {
        char *auth_response = NULL;

        if (client->connection_info.auth_type == HTTP_AUTH_TYPE_BASIC) {
            auth_response = http_auth_basic(client->connection_info.username, client->connection_info.password);
#ifdef CONFIG_HTTP_CLIENT_ENABLE_DIGEST_AUTH
        } else if (client->connection_info.auth_type == HTTP_AUTH_TYPE_DIGEST && client->auth_data) {
            client->auth_data->uri = client->connection_info.path;
            client->auth_data->cnonce = ((uint64_t)aiio_random() << 32) + aiio_random();
            auth_response = http_auth_digest(client->connection_info.username, client->connection_info.password, client->auth_data);
            client->auth_data->nc ++;
#endif
        }

        if (auth_response) {
            aiio_log_d( "auth_response=%s", auth_response);
            aiio_http_client_set_header(client, "Authorization", auth_response);
            free(auth_response);
        }
    }
    return AIIO_OK;
}

static char *_get_host_header(char *host, int port)
{
    int err = 0;
    char *host_name;
    if (port != DEFAULT_HTTP_PORT && port != DEFAULT_HTTPS_PORT) {
        err = asprintf(&host_name, "%s:%d", host, port);
    } else {
        err = asprintf(&host_name, "%s", host);
    }
    if (err == -1) {
        return NULL;
    }
    return host_name;
}

aiio_http_client_handle_t aiio_http_client_init(const aiio_http_client_config_t *config)
{

    aiio_http_client_handle_t client;
    int ret = AIIO_OK;
    aiio_transport_handle_t tcp = NULL;
    char *host_name;
    bool _success;

    _success = (
                   (client                         = calloc(1, sizeof(aiio_http_client_t)))           &&
                   (client->parser                 = calloc(1, sizeof(struct http_parser)))          &&
                   (client->parser_settings        = calloc(1, sizeof(struct http_parser_settings))) &&
                   (client->auth_data              = calloc(1, sizeof(aiio_http_auth_data_t)))        &&
                   (client->request                = calloc(1, sizeof(aiio_http_data_t)))             &&
                   (client->request->headers       = http_header_init())                             &&
                   (client->request->buffer        = calloc(1, sizeof(aiio_http_buffer_t)))           &&
                   (client->response               = calloc(1, sizeof(aiio_http_data_t)))             &&
                   (client->response->headers      = http_header_init())                             &&
                   (client->response->buffer       = calloc(1, sizeof(aiio_http_buffer_t)))
               );

    if (!_success) {
        aiio_log_e( "Error allocate memory");
        goto error;
    }

    _success = (
                   (client->transport_list = aiio_transport_list_init()) &&
                   (tcp = aiio_transport_tcp_init()) &&
                   (aiio_transport_set_default_port(tcp, DEFAULT_HTTP_PORT) == AIIO_OK) &&
                   (aiio_transport_list_add(client->transport_list, tcp, "http") == AIIO_OK)
               );
    if (!_success) {
        aiio_log_e( "Error initialize transport");
        goto error;
    }

    if (config->keep_alive_enable == true) {
        client->keep_alive_cfg.keep_alive_enable = true;
        client->keep_alive_cfg.keep_alive_idle = (config->keep_alive_idle == 0) ? DEFAULT_KEEP_ALIVE_IDLE : config->keep_alive_idle;
        client->keep_alive_cfg.keep_alive_interval = (config->keep_alive_interval == 0) ? DEFAULT_KEEP_ALIVE_INTERVAL : config->keep_alive_interval;
        client->keep_alive_cfg.keep_alive_count =  (config->keep_alive_count == 0) ? DEFAULT_KEEP_ALIVE_COUNT : config->keep_alive_count;
        aiio_transport_tcp_set_keep_alive(tcp, &client->keep_alive_cfg);
    }

    if (config->if_name) {
        client->if_name = calloc(1, sizeof(struct ifreq) + 1);
        AIIO_GOTO_ON_FALSE(client->if_name, AIIO_FAIL, error, TAG, "Memory exhausted");
        memcpy(client->if_name, config->if_name, sizeof(struct ifreq));
        aiio_transport_tcp_set_interface_name(tcp, client->if_name);
    }

#ifdef CONFIG_HTTP_CLIENT_ENABLE_HTTPS
    aiio_transport_handle_t ssl = NULL;
    _success = (
                   (ssl = aiio_transport_ssl_init()) &&
                   (aiio_transport_set_default_port(ssl, DEFAULT_HTTPS_PORT) == AIIO_OK) &&
                   (aiio_transport_list_add(client->transport_list, ssl, "https") == AIIO_OK)
               );

    if (!_success) {
        aiio_log_e( "Error initialize SSL Transport");
        goto error;
    }

    if (config->crt_bundle_attach != NULL) {
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
        aiio_transport_ssl_crt_bundle_attach(ssl, config->crt_bundle_attach);
#else //CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
        aiio_log_e( "use_crt_bundle configured but not enabled in menuconfig: Please enable MBEDTLS_CERTIFICATE_BUNDLE option");
#endif
    } else if (config->use_global_ca_store == true) {
        aiio_transport_ssl_enable_global_ca_store(ssl);
    } else if (config->cert_pem) {
        if (!config->cert_len) {
            aiio_transport_ssl_set_cert_data(ssl, config->cert_pem, strlen(config->cert_pem));
        } else {
            aiio_transport_ssl_set_cert_data_der(ssl, config->cert_pem, config->cert_len);
        }
    }

    if (config->client_cert_pem) {
        if (!config->client_cert_len) {
            aiio_transport_ssl_set_client_cert_data(ssl, config->client_cert_pem, strlen(config->client_cert_pem));
        } else {
            aiio_transport_ssl_set_client_cert_data_der(ssl, config->client_cert_pem, config->client_cert_len);
        }
    }

    if (config->client_key_pem) {
        if (!config->client_key_len) {
            aiio_transport_ssl_set_client_key_data(ssl, config->client_key_pem, strlen(config->client_key_pem));
        } else {
            aiio_transport_ssl_set_client_key_data_der(ssl, config->client_key_pem, config->client_key_len);
        }
    }

    if (config->client_key_password && config->client_key_password_len > 0) {
        aiio_transport_ssl_set_client_key_password(ssl, config->client_key_password, config->client_key_password_len);
    }

    if (config->skip_cert_common_name_check) {
        aiio_transport_ssl_skip_common_name_check(ssl);
    }
#endif

    if (_set_config(client, config) != AIIO_OK) {
        aiio_log_e( "Error set configurations");
        goto error;
    }
    _success = (
                   (client->request->buffer->data  = malloc(client->buffer_size_tx))  &&
                   (client->response->buffer->data = malloc(client->buffer_size_rx))
               );

    if (!_success) {
        aiio_log_e( "Allocation failed");
        goto error;
    }

    const char *user_agent = config->user_agent == NULL ? DEFAULT_HTTP_USER_AGENT : config->user_agent;

    if (config->host != NULL && config->path != NULL) {
        if (client->connection_info.host == NULL) {
            aiio_log_e( "invalid host");
            goto error;
        }
        host_name = _get_host_header(client->connection_info.host, client->connection_info.port);
        if (host_name == NULL) {
            aiio_log_e( "Failed to allocate memory for host header");
            goto error;
        }
        _success = (
            (aiio_http_client_set_header(client, "User-Agent", user_agent) == AIIO_OK) &&
            (aiio_http_client_set_header(client, "Host", host_name) == AIIO_OK)
        );
        free(host_name);
        if (!_success) {
            aiio_log_e( "Error while setting default configurations");
            goto error;
        }
    } else if (config->url != NULL) {
        if (aiio_http_client_set_url(client, config->url) != AIIO_OK) {
            aiio_log_e( "Failed to set URL");
            goto error;
        }
        if (client->connection_info.host == NULL) {
            aiio_log_e( "invalid host");
            goto error;
        }
        host_name = _get_host_header(client->connection_info.host, client->connection_info.port);
        if (host_name == NULL) {
            aiio_log_e( "Failed to allocate memory for host header");
            goto error;
        }

        _success = (
                    (aiio_http_client_set_header(client, "User-Agent", user_agent) == AIIO_OK) &&
                    (aiio_http_client_set_header(client, "Host", host_name) == AIIO_OK)
                );

        free(host_name);
        if (!_success) {
            aiio_log_e( "Error while setting default configurations");
            goto error;
        }
    } else {
        aiio_log_e( "config should have either URL or host & path");
        goto error;
    }

    /* As default behavior, cache data received in fetch header state. This will be
     * used in aiio_http_client_read API only. For aiio_http_perform we shall disable
     * this as data will be processed by event handler */
    client->cache_data_in_fetch_hdr = 1;

    client->parser_settings->on_message_begin = http_on_message_begin;
    client->parser_settings->on_url = http_on_url;
    client->parser_settings->on_status = http_on_status;
    client->parser_settings->on_header_field = http_on_header_field;
    client->parser_settings->on_header_value = http_on_header_value;
    client->parser_settings->on_headers_complete = http_on_headers_complete;
    client->parser_settings->on_body = http_on_body;
    client->parser_settings->on_message_complete = http_on_message_complete;
    client->parser_settings->on_chunk_complete = http_on_chunk_complete;
    client->parser_settings->on_chunk_header = http_on_chunk_header;
    client->parser->data = client;
    client->event.client = client;

    client->state = HTTP_STATE_INIT;

    if (ret == AIIO_OK) {
        return client;
    }

error:
    aiio_http_client_cleanup(client);
    return NULL;
}

int aiio_http_client_cleanup(aiio_http_client_handle_t client)
{
    if (client == NULL) {
        return AIIO_FAIL;
    }
    aiio_http_client_close(client);
    if (client->transport_list) {
        aiio_transport_list_destroy(client->transport_list);
    }
    if (client->request) {
        http_header_destroy(client->request->headers);
        if (client->request->buffer) {
            free(client->request->buffer->data);
        }
        free(client->request->buffer);
        free(client->request);
    }
    if (client->response) {
        http_header_destroy(client->response->headers);
        if (client->response->buffer) {
            free(client->response->buffer->data);
            if (client->response->buffer->orig_raw_data) {
                free(client->response->buffer->orig_raw_data);
                client->response->buffer->orig_raw_data = NULL;
                client->response->buffer->raw_data = NULL;
            }
        }
        free(client->response->buffer);
        free(client->response);
    }
    if (client->if_name) {
        free(client->if_name);
    }
    free(client->parser);
    free(client->parser_settings);
    _clear_connection_info(client);
    _clear_auth_data(client);
    free(client->auth_data);
    free(client->current_header_key);
    free(client->location);
    free(client->auth_header);
    free(client);
    return AIIO_OK;
}

int aiio_http_client_set_redirection(aiio_http_client_handle_t client)
{
    if (client == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    if (client->location == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    aiio_log_d( "Redirect to %s", client->location);
    return aiio_http_client_set_url(client, client->location);
}

static int aiio_http_check_response(aiio_http_client_handle_t client)
{
    if (client->response->status_code >= HttpStatus_Ok && client->response->status_code < HttpStatus_MultipleChoices) {
        return AIIO_OK;
    }
    if (client->redirect_counter >= client->max_redirection_count) {
        aiio_log_e( "Error, reach max_redirection_count count=%d", client->redirect_counter);
        return AIIO_ERR_HTTP_MAX_REDIRECT;
    }
    switch (client->response->status_code) {
        case HttpStatus_MovedPermanently:
        case HttpStatus_Found:
        case HttpStatus_SeeOther:
        case HttpStatus_TemporaryRedirect:
        case HttpStatus_PermanentRedirect:
            if (client->disable_auto_redirect) {
                http_dispatch_event(client, HTTP_EVENT_REDIRECT, NULL, 0);
            }
            aiio_http_client_set_redirection(client);
            client->redirect_counter ++;
            client->process_again = 1;
            break;
        case HttpStatus_Unauthorized:
            aiio_http_client_add_auth(client);
    }
    return AIIO_OK;
}

int aiio_http_client_set_url(aiio_http_client_handle_t client, const char *url)
{
    int ret = AIIO_OK;
    char *old_host = NULL;
    struct http_parser_url purl;
    int old_port;

    if (client == NULL || url == NULL) {
        aiio_log_e( "client or url must not NULL");
        return AIIO_ERR_INVALID_ARG;
    }

    http_parser_url_init(&purl);

    int parser_status = http_parser_parse_url(url, strlen(url), 0, &purl);

    if (parser_status != 0) {
        aiio_log_e( "Error parse url %s", url);
        return AIIO_ERR_INVALID_ARG;
    }
    if (client->connection_info.host) {
        old_host = strdup(client->connection_info.host);
    }
    old_port = client->connection_info.port;

    if (purl.field_data[UF_HOST].len) {
        http_utils_assign_string(&client->connection_info.host, url + purl.field_data[UF_HOST].off, purl.field_data[UF_HOST].len);
        AIIO_GOTO_ON_FALSE(client->connection_info.host, AIIO_ERR_NO_MEM, error, TAG, "Memory exhausted");
    }
    // Close the connection if host was changed
    if (old_host && client->connection_info.host
            && strcasecmp(old_host, (const void *)client->connection_info.host) != 0) {
        aiio_log_d( "New host assign = %s", client->connection_info.host);
        if (aiio_http_client_set_header(client, "Host", client->connection_info.host) != AIIO_OK) {
            free(old_host);
            return AIIO_ERR_NO_MEM;
        }
        aiio_http_client_close(client);
    }

    if (old_host) {
        free(old_host);
        old_host = NULL;
    }

    if (purl.field_data[UF_SCHEMA].len) {
        http_utils_assign_string(&client->connection_info.scheme, url + purl.field_data[UF_SCHEMA].off, purl.field_data[UF_SCHEMA].len);
        AIIO_RETURN_ON_FALSE(client->connection_info.scheme, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");

        if (strcasecmp(client->connection_info.scheme, "http") == 0) {
            client->connection_info.port = DEFAULT_HTTP_PORT;
        } else if (strcasecmp(client->connection_info.scheme, "https") == 0) {
            client->connection_info.port = DEFAULT_HTTPS_PORT;
        }
    }

    if (purl.field_data[UF_PORT].len) {
        client->connection_info.port = strtol((const char*)(url + purl.field_data[UF_PORT].off), NULL, 10);
    }

    if (old_port != client->connection_info.port) {
        aiio_http_client_close(client);
    }

    if (purl.field_data[UF_USERINFO].len) {
        char *user_info = NULL;
        http_utils_assign_string(&user_info, url + purl.field_data[UF_USERINFO].off, purl.field_data[UF_USERINFO].len);
        if (user_info) {
            char *username = user_info;
            char *password = strchr(user_info, ':');
            if (password) {
                *password = 0;
                password ++;
                http_utils_assign_string(&client->connection_info.password, password, -1);
                AIIO_RETURN_ON_FALSE(client->connection_info.password, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");
            }
            http_utils_assign_string(&client->connection_info.username, username, -1);
            AIIO_RETURN_ON_FALSE(client->connection_info.username, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");
            free(user_info);
        } else {
            return AIIO_ERR_NO_MEM;
        }
    }

    //Reset path and query if there are no information
    if (purl.field_data[UF_PATH].len) {
        http_utils_assign_string(&client->connection_info.path, url + purl.field_data[UF_PATH].off, purl.field_data[UF_PATH].len);
    } else {
        http_utils_assign_string(&client->connection_info.path, "/", -1);
    }
    AIIO_RETURN_ON_FALSE(client->connection_info.path, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");

    if (purl.field_data[UF_QUERY].len) {
        http_utils_assign_string(&client->connection_info.query, url + purl.field_data[UF_QUERY].off, purl.field_data[UF_QUERY].len);
        AIIO_RETURN_ON_FALSE(client->connection_info.query, AIIO_ERR_NO_MEM, TAG, "Memory exhausted");
    } else if (client->connection_info.query) {
        free(client->connection_info.query);
        client->connection_info.query = NULL;
    }

    return ret;

error:
    free(old_host);
    return ret;
}

int aiio_http_client_get_errno(aiio_http_client_handle_t client)
{
    if (!client) {
        aiio_log_e( "Invalid client handle");
        return -1;
    }
    return aiio_transport_get_errno(client->transport);
}

int aiio_http_client_set_method(aiio_http_client_handle_t client, aiio_http_client_method_t method)
{
    client->connection_info.method = method;
    return AIIO_OK;
}

int aiio_http_client_set_timeout_ms(aiio_http_client_handle_t client, int timeout_ms)
{
    if (client == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    client->timeout_ms = timeout_ms;
    return AIIO_OK;
}

static int aiio_http_client_get_data(aiio_http_client_handle_t client)
{
    if (client->state < HTTP_STATE_RES_ON_DATA_START) {
        return AIIO_FAIL;
    }

    if (client->connection_info.method == HTTP_METHOD_HEAD) {
        return 0;
    }

    aiio_http_buffer_t *res_buffer = client->response->buffer;

    aiio_log_d( "data_process=%lld, content_length=%lld", client->response->data_process, client->response->content_length);

    int rlen = aiio_transport_read(client->transport, res_buffer->data, client->buffer_size_rx, client->timeout_ms);
    if (rlen >= 0) {
        http_parser_execute(client->parser, client->parser_settings, res_buffer->data, rlen);
    }
    return rlen;
}

bool aiio_http_client_is_complete_data_received(aiio_http_client_handle_t client)
{
    if (client->response->is_chunked) {
        if (!client->is_chunk_complete) {
            aiio_log_d( "Chunks were not completely read");
            return false;
        }
    } else {
        if (client->response->data_process != client->response->content_length) {
            aiio_log_d( "Data processed %lld != Data specified in content length %lld", client->response->data_process, client->response->content_length);
            return false;
        }
    }
    return true;
}

int aiio_http_client_read(aiio_http_client_handle_t client, char *buffer, int len)
{
    aiio_http_buffer_t *res_buffer = client->response->buffer;

    int rlen = AIIO_FAIL, ridx = 0;
    if (res_buffer->raw_len) {
        int remain_len = client->response->buffer->raw_len;
        if (remain_len > len) {
            remain_len = len;
        }
        memcpy(buffer, res_buffer->raw_data, remain_len);
        res_buffer->raw_len -= remain_len;
        res_buffer->raw_data += remain_len;
        ridx = remain_len;
        if (res_buffer->raw_len == 0) {
            free(res_buffer->orig_raw_data);
            res_buffer->orig_raw_data = NULL;
            res_buffer->raw_data = NULL;
        }
    }
    int need_read = len - ridx;
    bool is_data_remain = true;
    while (need_read > 0 && is_data_remain) {
        if (client->response->is_chunked) {
            is_data_remain = !client->is_chunk_complete;
        } else {
            is_data_remain = client->response->data_process < client->response->content_length;
        }
        aiio_log_d( "is_data_remain=%d, is_chunked=%d, content_length=%lld", is_data_remain, client->response->is_chunked, client->response->content_length);
        if (!is_data_remain) {
            break;
        }
        int byte_to_read = need_read;
        if (byte_to_read > client->buffer_size_rx) {
            byte_to_read = client->buffer_size_rx;
        }
        errno = 0;
        rlen = aiio_transport_read(client->transport, res_buffer->data, byte_to_read, client->timeout_ms);
        aiio_log_d( "need_read=%d, byte_to_read=%d, rlen=%d, ridx=%d", need_read, byte_to_read, rlen, ridx);

        if (rlen <= 0) {
            if (errno != 0) {
                /* aiio_log_level_t sev = AIIO_LOG_WARN; */
                /* On connection close from server, recv should ideally return 0 but we have error conversion
                 * in `tcp_transport` SSL layer which translates it `-1` and hence below additional checks */
                if (rlen == -1 && errno == ENOTCONN && client->response->is_chunked) {
                    /* Explicit call to parser for invoking `message_complete` callback */
                    http_parser_execute(client->parser, client->parser_settings, res_buffer->data, 0);
                    /* ...and lowering the message severity, as closed connection from server side is expected in chunked transport */
                    /* sev = AIIO_LOG_DEBUG; */
                }
                aiio_log_w( "aiio_transport_read returned:%d and errno:%d ", rlen, errno);
            }
#ifdef CONFIG_HTTP_CLIENT_ENABLE_HTTPS
            if (rlen == AIIO_TLS_ERR_SSL_WANT_READ || errno == EAGAIN) {
#else
            if (errno == EAGAIN) {
#endif
                aiio_log_d( "Received EAGAIN! rlen = %d, errno %d", rlen, errno);
                return ridx;
            }
            if (rlen < 0 && ridx == 0 && !aiio_http_client_is_complete_data_received(client)) {
                http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                return AIIO_FAIL;
            }
            return ridx;
        }
        res_buffer->output_ptr = buffer + ridx;
        http_parser_execute(client->parser, client->parser_settings, res_buffer->data, rlen);
        ridx += res_buffer->raw_len;
        need_read -= res_buffer->raw_len;

        res_buffer->raw_len = 0; //clear
        res_buffer->output_ptr = NULL;
    }

    return ridx;
}

int aiio_http_client_perform(aiio_http_client_handle_t client)
{
    int err;
    do {
        if (client->process_again) {
            aiio_http_client_prepare(client);
        }
        switch (client->state) {
        /* In case of blocking aiio_http_client_perform(), the following states will fall through one after the after;
           in case of non-blocking aiio_http_client_perform(), if there is an error condition, like EINPROGRESS or EAGAIN,
           then the aiio_http_client_perform() API will return AIIO_ERR_HTTP_EAGAIN error. The user may call
           aiio_http_client_perform API again, and for this reason, we maintain the states */
            case HTTP_STATE_INIT:
                if ((err = aiio_http_client_connect(client)) != AIIO_OK) {
                    if (client->is_async && err == AIIO_ERR_HTTP_CONNECTING) {
                        return AIIO_ERR_HTTP_EAGAIN;
                    }
                    http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                    return err;
                }
                /* falls through */
            case HTTP_STATE_CONNECTED:
                if ((err = aiio_http_client_request_send(client, client->post_len)) != AIIO_OK) {
                    if (client->is_async && errno == EAGAIN) {
                        return AIIO_ERR_HTTP_EAGAIN;
                    }
                    http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                    return err;
                }
                /* falls through */
            case HTTP_STATE_REQ_COMPLETE_HEADER:
                if ((err = aiio_http_client_send_post_data(client)) != AIIO_OK) {
                    if (client->is_async && errno == EAGAIN) {
                        return AIIO_ERR_HTTP_EAGAIN;
                    }
                    http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                    return err;
                }
                /* falls through */
            case HTTP_STATE_REQ_COMPLETE_DATA:
                /* Disable caching response body, as data should
                 * be handled by application event handler */
                client->cache_data_in_fetch_hdr = 0;
                if (aiio_http_client_fetch_headers(client) < 0) {
                    if (client->is_async && errno == EAGAIN) {
                        return AIIO_ERR_HTTP_EAGAIN;
                    }
                    /* Enable caching after error condition because next
                     * request could be performed using native APIs */
                    client->cache_data_in_fetch_hdr = 1;
                    if (aiio_transport_get_errno(client->transport) == ENOTCONN) {
                        aiio_log_w( "Close connection due to FIN received");
                        aiio_http_client_close(client);
                        http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                        return AIIO_ERR_HTTP_CONNECTION_CLOSED;
                    }
                    http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                    return AIIO_ERR_HTTP_FETCH_HEADER;
                }
                /* falls through */
            case HTTP_STATE_RES_ON_DATA_START:
                /* Enable caching after fetch headers state because next
                 * request could be performed using native APIs */
                client->cache_data_in_fetch_hdr = 1;
                if ((err = aiio_http_check_response(client)) != AIIO_OK) {
                    aiio_log_e( "Error response");
                    http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
                    return err;
                }
                while (client->response->is_chunked && !client->is_chunk_complete) {
                    if (aiio_http_client_get_data(client) <= 0) {
                        if (client->is_async && errno == EAGAIN) {
                            return AIIO_ERR_HTTP_EAGAIN;
                        }
                        aiio_log_d( "Read finish or server requests close");
                        break;
                    }
                }
                while (client->response->data_process < client->response->content_length) {
                    if (aiio_http_client_get_data(client) <= 0) {
                        if (client->is_async && errno == EAGAIN) {
                            return AIIO_ERR_HTTP_EAGAIN;
                        }
                        aiio_log_d( "Read finish or server requests close");
                        break;
                    }
                }
                http_dispatch_event(client, HTTP_EVENT_ON_FINISH, NULL, 0);

                client->response->buffer->raw_len = 0;
                if (!http_should_keep_alive(client->parser)) {
                    aiio_log_d( "Close connection");
                    aiio_http_client_close(client);
                } else {
                    if (client->state > HTTP_STATE_CONNECTED) {
                        client->state = HTTP_STATE_CONNECTED;
                        client->first_line_prepared = false;
                    }
                }
                break;
                default:
                break;
        }
    } while (client->process_again);
    return AIIO_OK;
}

int64_t aiio_http_client_fetch_headers(aiio_http_client_handle_t client)
{
    if (client->state < HTTP_STATE_REQ_COMPLETE_HEADER) {
        return AIIO_FAIL;
    }

    client->state = HTTP_STATE_REQ_COMPLETE_DATA;
    aiio_http_buffer_t *buffer = client->response->buffer;
    client->response->status_code = -1;

    while (client->state < HTTP_STATE_RES_COMPLETE_HEADER) {
        buffer->len = aiio_transport_read(client->transport, buffer->data, client->buffer_size_rx, client->timeout_ms);
        if (buffer->len <= 0) {
            return AIIO_FAIL;
        }
        http_parser_execute(client->parser, client->parser_settings, buffer->data, buffer->len);
    }
    client->state = HTTP_STATE_RES_ON_DATA_START;
    aiio_log_d( "content_length = %lld", client->response->content_length);
    if (client->response->content_length <= 0) {
        client->response->is_chunked = true;
        return 0;
    }
    return client->response->content_length;
}

static int aiio_http_client_connect(aiio_http_client_handle_t client)
{
    int err;

    if (client->state == HTTP_STATE_UNINIT) {
        aiio_log_e( "Client has not been initialized");
        return AIIO_ERR_INVALID_STATE;
    }

    if ((err = aiio_http_client_prepare(client)) != AIIO_OK) {
        aiio_log_e( "Failed to initialize request data");
        aiio_http_client_close(client);
        return err;
    }

    if (client->state < HTTP_STATE_CONNECTED) {
        aiio_log_d( "Begin connect to: %s://%s:%d", client->connection_info.scheme, client->connection_info.host, client->connection_info.port);
        client->transport = aiio_transport_list_get_transport(client->transport_list, client->connection_info.scheme);
        if (client->transport == NULL) {
            aiio_log_e( "No transport found");
#ifndef CONFIG_HTTP_CLIENT_ENABLE_HTTPS
            if (strcasecmp(client->connection_info.scheme, "https") == 0) {
                aiio_log_e( "Please enable HTTPS at menuconfig to allow requesting via https");
            }
#endif
            return AIIO_ERR_HTTP_INVALID_TRANSPORT;
        }
        if (!client->is_async) {
            if (aiio_transport_connect(client->transport, client->connection_info.host, client->connection_info.port, client->timeout_ms) < 0) {
                aiio_log_e( "Connection failed, sock < 0");
                return AIIO_ERR_HTTP_CONNECT;
            }
        } else {
            int ret = aiio_transport_connect_async(client->transport, client->connection_info.host, client->connection_info.port, client->timeout_ms);
            if (ret == ASYNC_TRANS_CONNECT_FAIL) {
                aiio_log_e( "Connection failed");
                if (strcasecmp(client->connection_info.scheme, "http") == 0) {
                    aiio_log_e( "Asynchronous mode doesn't work for HTTP based connection");
                    return AIIO_ERR_INVALID_ARG;
                }
                return AIIO_ERR_HTTP_CONNECT;
            } else if (ret == ASYNC_TRANS_CONNECTING) {
                aiio_log_d( "Connection not yet established");
                return AIIO_ERR_HTTP_CONNECTING;
            }
        }
        client->state = HTTP_STATE_CONNECTED;
        http_dispatch_event(client, HTTP_EVENT_ON_CONNECTED, NULL, 0);
    }
    return AIIO_OK;
}

static int http_client_prepare_first_line(aiio_http_client_handle_t client, int write_len)
{
    if (write_len >= 0) {
        http_header_set_format(client->request->headers, "Content-Length", "%d", write_len);
    } else {
        aiio_http_client_set_header(client, "Transfer-Encoding", "chunked");
    }

    const char *method = HTTP_METHOD_MAPPING[client->connection_info.method];

    int first_line_len = snprintf(client->request->buffer->data,
                                  client->buffer_size_tx, "%s %s",
                                  method,
                                  client->connection_info.path);
    if (first_line_len >= client->buffer_size_tx) {
        aiio_log_e( "Out of buffer");
        return -1;
    }

    if (client->connection_info.query) {
        first_line_len += snprintf(client->request->buffer->data + first_line_len,
                                   client->buffer_size_tx - first_line_len, "?%s", client->connection_info.query);
        if (first_line_len >= client->buffer_size_tx) {
            aiio_log_e( "Out of buffer");
            return -1;

        }
    }
    first_line_len += snprintf(client->request->buffer->data + first_line_len,
                               client->buffer_size_tx - first_line_len, " %s\r\n", DEFAULT_HTTP_PROTOCOL);
    if (first_line_len >= client->buffer_size_tx) {
        aiio_log_e( "Out of buffer");
        return -1;
    }
    return first_line_len;
}

static int aiio_http_client_request_send(aiio_http_client_handle_t client, int write_len)
{
    int first_line_len = 0;
    if (!client->first_line_prepared) {
        if ((first_line_len = http_client_prepare_first_line(client, write_len)) < 0) {
            return first_line_len;
        }
        client->first_line_prepared = true;
        client->header_index = 0;
        client->data_written_index = 0;
        client->data_write_left = 0;
    }

    if (client->data_write_left > 0) {
        /* sending leftover data from previous call to aiio_http_client_request_send() API */
        int wret = 0;
        if (((wret = aiio_http_client_write(client, client->request->buffer->data + client->data_written_index, client->data_write_left)) < 0)) {
            aiio_log_e( "Error write request");
            return AIIO_ERR_HTTP_WRITE_DATA;
        }
        client->data_write_left -= wret;
        client->data_written_index += wret;
        if (client->is_async && client->data_write_left > 0) {
            return AIIO_ERR_HTTP_WRITE_DATA;      /* In case of EAGAIN error, we return AIIO_ERR_HTTP_WRITE_DATA,
                                                 and the handling of EAGAIN should be done in the higher level APIs. */
        }
    }

    int wlen = client->buffer_size_tx - first_line_len;
    while ((client->header_index = http_header_generate_string(client->request->headers, client->header_index, client->request->buffer->data + first_line_len, &wlen))) {
        if (wlen <= 0) {
            break;
        }
        if (first_line_len) {
            wlen += first_line_len;
            first_line_len = 0;
        }
        client->request->buffer->data[wlen] = 0;
        aiio_log_d( "Write header[%d]: %s", client->header_index, client->request->buffer->data);

        client->data_write_left = wlen;
        client->data_written_index = 0;
        while (client->data_write_left > 0) {
            int wret = aiio_transport_write(client->transport, client->request->buffer->data + client->data_written_index, client->data_write_left, client->timeout_ms);
            if (wret <= 0) {
                aiio_log_e( "Error write request");
                aiio_http_client_close(client);
                return AIIO_ERR_HTTP_WRITE_DATA;
            }
            client->data_write_left -= wret;
            client->data_written_index += wret;
        }
        wlen = client->buffer_size_tx;
    }

    client->data_written_index = 0;
    client->data_write_left = client->post_len;
    http_dispatch_event(client, HTTP_EVENT_HEADERS_SENT, NULL, 0);
    client->state = HTTP_STATE_REQ_COMPLETE_HEADER;
    return AIIO_OK;
}

static int aiio_http_client_send_post_data(aiio_http_client_handle_t client)
{
    if (client->state != HTTP_STATE_REQ_COMPLETE_HEADER) {
        aiio_log_e( "Invalid state");
        return AIIO_ERR_INVALID_STATE;
    }
    if (!(client->post_data && client->post_len)) {
        goto success;
    }

    int wret = aiio_http_client_write(client, client->post_data + client->data_written_index, client->data_write_left);
    if (wret < 0) {
        return wret;
    }
    client->data_write_left -= wret;
    client->data_written_index += wret;

    if (client->data_write_left <= 0) {
        goto success;
    } else {
        return AIIO_ERR_HTTP_WRITE_DATA;
    }

success:
    client->state = HTTP_STATE_REQ_COMPLETE_DATA;
    return AIIO_OK;
}

int aiio_http_client_open(aiio_http_client_handle_t client, int write_len)
{
    client->post_len = write_len;
    int err;
    if ((err = aiio_http_client_connect(client)) != AIIO_OK) {
        http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
        return err;
    }
    if ((err = aiio_http_client_request_send(client, write_len)) != AIIO_OK) {
        http_dispatch_event(client, HTTP_EVENT_ERROR, aiio_transport_get_error_handle(client->transport), 0);
        return err;
    }
    return AIIO_OK;
}

int aiio_http_client_write(aiio_http_client_handle_t client, const char *buffer, int len)
{
    if (client->state < HTTP_STATE_REQ_COMPLETE_HEADER) {
        return AIIO_FAIL;
    }

    int wlen = 0, widx = 0;
    while (len > 0) {
        wlen = aiio_transport_write(client->transport, buffer + widx, len, client->timeout_ms);
        /* client->async_block is initialised in case of non-blocking IO, and in this case we return how
           much ever data was written by the aiio_transport_write() API. */
        if (client->is_async || wlen <= 0) {
            return wlen;
        }
        widx += wlen;
        len -= wlen;
    }
    return widx;
}

int aiio_http_client_close(aiio_http_client_handle_t client)
{
    if (client->state >= HTTP_STATE_INIT) {
        http_dispatch_event(client, HTTP_EVENT_DISCONNECTED, aiio_transport_get_error_handle(client->transport), 0);
        client->state = HTTP_STATE_INIT;
        return aiio_transport_close(client->transport);
    }
    return AIIO_OK;
}

int aiio_http_client_set_post_field(aiio_http_client_handle_t client, const char *data, int len)
{
    int err = AIIO_OK;
    client->post_data = (char *)data;
    client->post_len = len;
    aiio_log_d( "set post file length = %d", len);
    if (client->post_data) {
        char *value = NULL;
        if ((err = aiio_http_client_get_header(client, "Content-Type", &value)) != AIIO_OK) {
            return err;
        }
        if (value == NULL) {
            err = aiio_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
        }
    } else {
        client->post_len = 0;
        err = aiio_http_client_set_header(client, "Content-Type", NULL);
    }
    return err;
}

int aiio_http_client_get_post_field(aiio_http_client_handle_t client, char **data)
{
    if (client->post_data) {
        *data = client->post_data;
        return client->post_len;
    }
    return 0;
}

int aiio_http_client_get_status_code(aiio_http_client_handle_t client)
{
    return client->response->status_code;
}

int64_t aiio_http_client_get_content_length(aiio_http_client_handle_t client)
{
    return client->response->content_length;
}

bool aiio_http_client_is_chunked_response(aiio_http_client_handle_t client)
{
    return client->response->is_chunked;
}

aiio_http_client_transport_t aiio_http_client_get_transport_type(aiio_http_client_handle_t client)
{
    if (!strcasecmp(client->connection_info.scheme, "https") ) {
        return HTTP_TRANSPORT_OVER_SSL;
    } else if (!strcasecmp(client->connection_info.scheme, "http")) {
        return HTTP_TRANSPORT_OVER_TCP;
    } else {
        return HTTP_TRANSPORT_UNKNOWN;
    }
}

void aiio_http_client_add_auth(aiio_http_client_handle_t client)
{
    if (client == NULL) {
        return;
    }
    if (client->state != HTTP_STATE_RES_ON_DATA_START) {
        return;
    }
    if (client->redirect_counter >= client->max_authorization_retries) {
        aiio_log_e( "Error, reached max_authorization_retries count=%d", client->redirect_counter);
        return;
    }

    char *auth_header = client->auth_header;
    if (auth_header) {
        http_utils_trim_whitespace(&auth_header);
        aiio_log_d( "UNAUTHORIZED: %s", auth_header);
        client->redirect_counter++;
#ifdef CONFIG_HTTP_CLIENT_ENABLE_DIGEST_AUTH
        if (http_utils_str_starts_with(auth_header, "Digest") == 0) {
            aiio_log_d( "type = Digest");
            client->connection_info.auth_type = HTTP_AUTH_TYPE_DIGEST;
        } else {
#endif
#ifdef CONFIG_HTTP_CLIENT_ENABLE_BASIC_AUTH
        if (http_utils_str_starts_with(auth_header, "Basic") == 0) {
            aiio_log_d( "type = Basic");
            client->connection_info.auth_type = HTTP_AUTH_TYPE_BASIC;
        } else {
#endif
            client->connection_info.auth_type = HTTP_AUTH_TYPE_NONE;
            aiio_log_e( "This authentication method is not supported: %s", auth_header);
            return;
#ifdef CONFIG_HTTP_CLIENT_ENABLE_BASIC_AUTH
        }
#endif
#ifdef CONFIG_HTTP_CLIENT_ENABLE_DIGEST_AUTH
        }
#endif

        _clear_auth_data(client);

        client->auth_data->method = strdup(HTTP_METHOD_MAPPING[client->connection_info.method]);

        client->auth_data->nc = 1;
        client->auth_data->realm = http_utils_get_string_between(auth_header, "realm=\"", "\"");
        client->auth_data->algorithm = http_utils_get_string_between(auth_header, "algorithm=", ",");
        if (client->auth_data->algorithm == NULL) {
            client->auth_data->algorithm = strdup("MD5");
        }
        client->auth_data->qop = http_utils_get_string_between(auth_header, "qop=\"", "\"");
        client->auth_data->nonce = http_utils_get_string_between(auth_header, "nonce=\"", "\"");
        client->auth_data->opaque = http_utils_get_string_between(auth_header, "opaque=\"", "\"");
        client->process_again = 1;
    } else {
        client->connection_info.auth_type = HTTP_AUTH_TYPE_NONE;
        aiio_log_w( "This request requires authentication, but does not provide header information for that");
    }
}

int aiio_http_client_read_response(aiio_http_client_handle_t client, char *buffer, int len)
{
    int read_len = 0;
    while (read_len < len) {
        int data_read = aiio_http_client_read(client, buffer + read_len, len - read_len);
        if (data_read <= 0) {
            return read_len;
        }
        read_len += data_read;
    }
    return read_len;
}

int aiio_http_client_flush_response(aiio_http_client_handle_t client, int *len)
{
    if (client == NULL) {
        aiio_log_e( "client must not be NULL");
        return AIIO_ERR_INVALID_ARG;
    }
    int read_len = 0;
    while (!aiio_http_client_is_complete_data_received(client)) {
        int data_read = aiio_http_client_get_data(client);
        if (data_read < 0) {
            return AIIO_FAIL;
        }
        read_len += data_read;
    }
    if (len) {
        *len = read_len;
    }
    return AIIO_OK;
}

int aiio_http_client_get_url(aiio_http_client_handle_t client, char *url, const int len)
{
    if (client == NULL || url == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    if (client->connection_info.host && client->connection_info.scheme && client->connection_info.path) {
        snprintf(url, len, "%s://%s%s", client->connection_info.scheme, client->connection_info.host, client->connection_info.path);
        return AIIO_OK;
    } else {
        aiio_log_e( "Failed to get URL");
    }
    return AIIO_FAIL;
}

int aiio_http_client_get_chunk_length(aiio_http_client_handle_t client, int *len)
{
    if (client == NULL || len == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    if (aiio_http_client_is_chunked_response(client)) {
        *len = client->response->chunk_length;
    } else {
        aiio_log_e( "Response is not chunked");
        return AIIO_FAIL;
    }
    return AIIO_OK;
}
