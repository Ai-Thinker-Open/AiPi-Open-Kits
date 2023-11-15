
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
// #include <sys/socket.h>
#include <netdb.h>
#include "aiio_chip_spec.h"
#include <http_parser.h>
#include "aiio_tls.h"
#include "aiio_tls_error_capture_internal.h"
#include "aiio_err.h"
#include <FreeRTOS.h>
#include <task.h>

static const char *TAG = "aiio-tls";

#ifdef CONFIG_TLS_USING_MBEDTLS
#include "aiio_tls_mbedtls.h"
#elif CONFIG_TLS_USING_WOLFSSL
#include "aiio_tls_wolfssl.h"
#endif

#include <aiio_log.h>

#ifdef CONFIG_TLS_USING_MBEDTLS
#define _create_ssl_handle              aiio_create_mbedtls_handle
#define _tls_handshake                  aiio_mbedtls_handshake
#define _tls_read                       aiio_mbedtls_read
#define _tls_write                      aiio_mbedtls_write
#define _tls_conn_delete                aiio_mbedtls_conn_delete
#define _tls_net_init                   aiio_mbedtls_net_init
#define _tls_get_client_session         aiio_mbedtls_get_client_session
#ifdef CONFIG_TLS_SERVER
#define _tls_server_session_create      aiio_mbedtls_server_session_create
#define _tls_server_session_delete      aiio_mbedtls_server_session_delete
#define _tls_server_session_ticket_ctx_init    aiio_mbedtls_server_session_ticket_ctx_init
#define _tls_server_session_ticket_ctx_free    aiio_mbedtls_server_session_ticket_ctx_free
#endif  /* CONFIG_TLS_SERVER */
#define _tls_get_bytes_avail            aiio_mbedtls_get_bytes_avail
#define _tls_init_global_ca_store       aiio_mbedtls_init_global_ca_store
#define _tls_set_global_ca_store        aiio_mbedtls_set_global_ca_store                 /*!< Callback function for setting global CA store data for TLS/SSL */
#define _tls_get_global_ca_store        aiio_mbedtls_get_global_ca_store
#define _tls_free_global_ca_store       aiio_mbedtls_free_global_ca_store                /*!< Callback function for freeing global ca store for TLS/SSL */
#elif CONFIG_TLS_USING_WOLFSSL /* CONFIG_TLS_USING_MBEDTLS */
#define _create_ssl_handle              aiio_create_wolfssl_handle
#define _tls_handshake                  aiio_wolfssl_handshake
#define _tls_read                       aiio_wolfssl_read
#define _tls_write                      aiio_wolfssl_write
#define _tls_conn_delete                aiio_wolfssl_conn_delete
#define _tls_net_init                   aiio_wolfssl_net_init
#ifdef CONFIG_TLS_SERVER
#define _tls_server_session_create      aiio_wolfssl_server_session_create
#define _tls_server_session_delete      aiio_wolfssl_server_session_delete
#endif  /* CONFIG_TLS_SERVER */
#define _tls_get_bytes_avail            aiio_wolfssl_get_bytes_avail
#define _tls_init_global_ca_store       aiio_wolfssl_init_global_ca_store
#define _tls_set_global_ca_store        aiio_wolfssl_set_global_ca_store                 /*!< Callback function for setting global CA store data for TLS/SSL */
#define _tls_free_global_ca_store       aiio_wolfssl_free_global_ca_store                /*!< Callback function for freeing global ca store for TLS/SSL */
#else   /* AIIO_TLS_USING_WOLFSSL */
#error "No TLS stack configured"
#endif

static int create_ssl_handle(const char *hostname, size_t hostlen, const void *cfg, aiio_tls_t *tls)
{
    return _create_ssl_handle(hostname, hostlen, cfg, tls);
}

static int aiio_tls_handshake(aiio_tls_t *tls, const aiio_tls_cfg_t *cfg)
{
    return _tls_handshake(tls, cfg);
}

static ssize_t tcp_read(aiio_tls_t *tls, char *data, size_t datalen)
{
    return recv(tls->sockfd, data, datalen, 0);
}

static ssize_t tcp_write(aiio_tls_t *tls, const char *data, size_t datalen)
{
    return send(tls->sockfd, data, datalen, 0);
}

/**
 * @brief      Close the TLS connection and free any allocated resources.
 */
int aiio_tls_conn_destroy(aiio_tls_t *tls)
{
    if (tls != NULL) {
        int ret = 0;
        _tls_conn_delete(tls);
        if (tls->sockfd >= 0) {
            ret = close(tls->sockfd);
        }
        aiio_tls_internal_event_tracker_destroy(tls->error_handle);
        free(tls);
        return ret;
    }
    return -1; // invalid argument
}

aiio_tls_t *aiio_tls_init(void)
{
    aiio_tls_t *tls = (aiio_tls_t *)calloc(1, sizeof(aiio_tls_t));
    if (!tls) {
        return NULL;
    }
    tls->error_handle = aiio_tls_internal_event_tracker_create();
    if (!tls->error_handle) {
        free(tls);
        return NULL;
    }
    _tls_net_init(tls);
    tls->sockfd = -1;
    return tls;
}

static int aiio_tls_hostname_to_fd(const char *host, size_t hostlen, int port, struct sockaddr_storage *address, int* fd)
{
    struct addrinfo *address_info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char *use_host = strndup(host, hostlen);
    if (!use_host) {
        return AIIO_ERR_NO_MEM;
    }

    aiio_log_d( "host:%s: strlen %lu", use_host, (unsigned long)hostlen);
    int res = getaddrinfo(use_host, NULL, &hints, &address_info);
    if (res != 0 || address_info == NULL) {
        aiio_log_e( "couldn't get hostname for :%s: "
                      "getaddrinfo() returns %d, addrinfo=%p", use_host, res, address_info);
        free(use_host);
        return AIIO_ERR_TLS_CANNOT_RESOLVE_HOSTNAME;
    }
    free(use_host);
    *fd = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
    if (*fd < 0) {
        aiio_log_e( "Failed to create socket (family %d socktype %d protocol %d)", address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
        freeaddrinfo(address_info);
        return AIIO_ERR_TLS_CANNOT_CREATE_SOCKET;
    }

    if (address_info->ai_family == AF_INET) {
        struct sockaddr_in *p = (struct sockaddr_in *)address_info->ai_addr;
        p->sin_port = htons(port);
        aiio_log_d( "[sock=%d] Resolved IPv4 address: %s", *fd, ipaddr_ntoa((const ip_addr_t*)&p->sin_addr.s_addr));
        memcpy(address, p, sizeof(struct sockaddr ));
    }
#if CONFIG_LWIP_IPV6
    else if (address_info->ai_family == AF_INET6) {
        struct sockaddr_in6 *p = (struct sockaddr_in6 *)address_info->ai_addr;
        p->sin6_port = htons(port);
        p->sin6_family = AF_INET6;
        aiio_log_d( "[sock=%d] Resolved IPv6 address: %s", *fd, ip6addr_ntoa((const ip6_addr_t*)&p->sin6_addr));
        memcpy(address, p, sizeof(struct sockaddr_in6 ));
    }
#endif
    else {
        aiio_log_e( "Unsupported protocol family %d", address_info->ai_family);
        close(*fd);
        freeaddrinfo(address_info);
        return AIIO_ERR_TLS_UNSUPPORTED_PROTOCOL_FAMILY;
    }

    freeaddrinfo(address_info);
    return AIIO_OK;
}

static void ms_to_timeval(int timeout_ms, struct timeval *tv)
{
    tv->tv_sec = timeout_ms / 1000;
    tv->tv_usec = (timeout_ms % 1000) * 1000;
}

static int aiio_tls_set_socket_options(int fd, const aiio_tls_cfg_t *cfg)
{
    if (cfg) {
        if (cfg->timeout_ms >= 0) {
            struct timeval tv;
            ms_to_timeval(cfg->timeout_ms, &tv);
            if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
                aiio_log_e( "Fail to setsockopt SO_RCVTIMEO");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
            if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != 0) {
                aiio_log_e( "Fail to setsockopt SO_SNDTIMEO");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
        }
        if (cfg->keep_alive_cfg && cfg->keep_alive_cfg->keep_alive_enable) {
            int keep_alive_enable = 1;
            int keep_alive_idle = cfg->keep_alive_cfg->keep_alive_idle;
            int keep_alive_interval = cfg->keep_alive_cfg->keep_alive_interval;
            int keep_alive_count = cfg->keep_alive_cfg->keep_alive_count;

            aiio_log_d( "Enable TCP keep alive. idle: %d, interval: %d, count: %d", keep_alive_idle, keep_alive_interval, keep_alive_count);
            if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keep_alive_enable, sizeof(keep_alive_enable)) != 0) {
                aiio_log_e( "Fail to setsockopt SO_KEEPALIVE");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
            if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_alive_idle, sizeof(keep_alive_idle)) != 0) {
                aiio_log_e( "Fail to setsockopt TCP_KEEPIDLE");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
            if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_alive_interval, sizeof(keep_alive_interval)) != 0) {
                aiio_log_e( "Fail to setsockopt TCP_KEEPINTVL");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
            if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_alive_count, sizeof(keep_alive_count)) != 0) {
                aiio_log_e( "Fail to setsockopt TCP_KEEPCNT");
                return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
            }
        }
        if (cfg->if_name) {
            if (cfg->if_name->ifr_name[0] != 0) {
                aiio_log_d( "Bind [sock=%d] to interface %s", fd, cfg->if_name->ifr_name);
                if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,  cfg->if_name, sizeof(struct ifreq)) != 0) {
                    aiio_log_e( "Bind [sock=%d] to interface %s fail", fd, cfg->if_name->ifr_name);
                    return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
                }
            }
        }
    }
    return AIIO_OK;
}

static int aiio_tls_set_socket_non_blocking(int fd, bool non_blocking)
{
    int flags;
    if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
        aiio_log_e( "[sock=%d] get file flags error: %s", fd, strerror(errno));
        return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
    }

    if (non_blocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(fd, F_SETFL, flags) < 0) {
        aiio_log_e( "[sock=%d] set blocking/nonblocking error: %s", fd, strerror(errno));
        return AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
    }
    return AIIO_OK;
}

static inline int tcp_connect(const char *host, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_error_handle_t error_handle, int *sockfd)
{
    struct sockaddr_storage address;
    int fd;
    int ret = aiio_tls_hostname_to_fd(host, hostlen, port, &address, &fd);
    if (ret != AIIO_OK) {
        AIIO_INT_EVENT_TRACKER_CAPTURE(error_handle, AIIO_TLS_ERR_TYPE_SYSTEM, errno);
        return ret;
    }

    // Set timeout options, keep-alive options and bind device options if configured
    ret = aiio_tls_set_socket_options(fd, cfg);
    if (ret != AIIO_OK) {
        goto err;
    }

    // Set to non block before connecting to better control connection timeout
    ret = aiio_tls_set_socket_non_blocking(fd, true);
    if (ret != AIIO_OK) {
        goto err;
    }

    ret = AIIO_ERR_TLS_FAILED_CONNECT_TO_HOST;
    aiio_log_d( "[sock=%d] Connecting to server. HOST: %s, Port: %d", fd, host, port);
    if (connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr)) < 0) {
        if (errno == EINPROGRESS) {
            fd_set fdset;
            struct timeval tv = { .tv_usec = 0, .tv_sec = 10 }; // Default connection timeout is 10 s

            if (cfg && cfg->non_block) {
                // Non-blocking mode -> just return successfully at this stage
                *sockfd = fd;
                return AIIO_OK;
            }

            if ( cfg && cfg->timeout_ms > 0 ) {
                ms_to_timeval(cfg->timeout_ms, &tv);
            }
            FD_ZERO(&fdset);
            FD_SET(fd, &fdset);

            int res = select(fd+1, NULL, &fdset, NULL, &tv);
            if (res < 0) {
                aiio_log_e( "[sock=%d] select() error: %s", fd, strerror(errno));
                AIIO_INT_EVENT_TRACKER_CAPTURE(error_handle, AIIO_TLS_ERR_TYPE_SYSTEM, errno);
                goto err;
            }
            else if (res == 0) {
                aiio_log_e( "[sock=%d] select() timeout", fd);
                ret = AIIO_ERR_TLS_CONNECTION_TIMEOUT;
                goto err;
            } else {
                int sockerr;
                socklen_t len = (socklen_t)sizeof(int);

                if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&sockerr), &len) < 0) {
                    aiio_log_e( "[sock=%d] getsockopt() error: %s", fd, strerror(errno));
                    ret = AIIO_ERR_TLS_SOCKET_SETOPT_FAILED;
                    goto err;
                }
                else if (sockerr) {
                    AIIO_INT_EVENT_TRACKER_CAPTURE(error_handle, AIIO_TLS_ERR_TYPE_SYSTEM, sockerr);
                    aiio_log_e( "[sock=%d] delayed connect error: %s", fd, strerror(sockerr));
                    goto err;
                }
            }
        } else {
            aiio_log_e( "[sock=%d] connect() error: %s", fd, strerror(errno));
            goto err;
        }
    }

    if (cfg && cfg->non_block == false) {
        // reset back to blocking mode (unless non_block configured)
        ret = aiio_tls_set_socket_non_blocking(fd, false);
        if (ret != AIIO_OK) {
            goto err;
        }
    }

    *sockfd = fd;
    return AIIO_OK;

err:
    close(fd);
    return ret;
}

static int aiio_tls_low_level_conn(const char *hostname, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    if (!tls) {
        aiio_log_e( "empty aiio_tls parameter");
        return -1;
    }
    int aiio_ret;
    /* These states are used to keep a tab on connection progress in case of non-blocking connect,
    and in case of blocking connect these cases will get executed one after the other */
    switch (tls->conn_state) {
    case AIIO_TLS_INIT:
        tls->sockfd = -1;
        if (cfg != NULL && cfg->is_plain_tcp == false) {
            _tls_net_init(tls);
            tls->is_tls = true;
        }
        if ((aiio_ret = tcp_connect(hostname, hostlen, port, cfg, tls->error_handle, &tls->sockfd)) != AIIO_OK) {
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, aiio_ret);
            return -1;
        }
        if (tls->is_tls == false) {
            tls->read_ = tcp_read;
            tls->write_ = tcp_write;
            aiio_log_d( "non-tls connection established");
            return 1;
        }
        if (cfg && cfg->non_block) {
            FD_ZERO(&tls->rset);
            FD_SET(tls->sockfd, &tls->rset);
            tls->wset = tls->rset;
        }
        tls->conn_state = AIIO_TLS_CONNECTING;
        __attribute__((fallthrough));
    /* falls through */
    case AIIO_TLS_CONNECTING:
        if (cfg && cfg->non_block) {
            aiio_log_d( "connecting...");
            struct timeval tv;
            ms_to_timeval(cfg->timeout_ms, &tv);

            /* In case of non-blocking I/O, we use the select() API to check whether
               connection has been established or not*/
            if (select(tls->sockfd + 1, &tls->rset, &tls->wset, NULL,
                       cfg->timeout_ms>0 ? &tv : NULL) == 0) {
                aiio_log_d( "select() timed out");
                return 0;
            }
            if (FD_ISSET(tls->sockfd, &tls->rset) || FD_ISSET(tls->sockfd, &tls->wset)) {
                int error;
                socklen_t len = sizeof(error);
                /* pending error check */
                if (getsockopt(tls->sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
                    aiio_log_d( "Non blocking connect failed");
                    AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_SYSTEM, errno);
                    AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_TLS_SOCKET_SETOPT_FAILED);
                    tls->conn_state = AIIO_TLS_FAIL;
                    return -1;
                }
            }
        }
        /* By now, the connection has been established */
        aiio_ret = create_ssl_handle(hostname, hostlen, cfg, tls);
        if (aiio_ret != AIIO_OK) {
            aiio_log_e( "create_ssl_handle failed");
            AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, aiio_ret);
            tls->conn_state = AIIO_TLS_FAIL;
            return -1;
        }
        tls->read_ = _tls_read;
        tls->write_ = _tls_write;
        tls->conn_state = AIIO_TLS_HANDSHAKE;
        __attribute__((fallthrough));
    /* falls through */
    case AIIO_TLS_HANDSHAKE:
        aiio_log_d( "handshake in progress...");
        return aiio_tls_handshake(tls, cfg);
        break;
    case AIIO_TLS_FAIL:
        aiio_log_e( "failed to open a new connection");;
        break;
    default:
        aiio_log_e( "invalid aiio-tls state");
        break;
    }
    return -1;
}

/**
 * @brief Create a new plain TCP connection
 */
int aiio_tls_plain_tcp_connect(const char *host, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_error_handle_t error_handle, int *sockfd)
{
    if (sockfd == NULL || error_handle == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    return tcp_connect(host, hostlen, port, cfg, error_handle, sockfd);
}

int aiio_tls_conn_new_sync(const char *hostname, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    size_t start = xTaskGetTickCount();
    while (1) {
        int ret = aiio_tls_low_level_conn(hostname, hostlen, port, cfg, tls);
        if (ret == 1) {
            return ret;
        } else if (ret == -1) {
            aiio_log_e( "Failed to open new connection");
            return -1;
        } else if (ret == 0 && cfg->timeout_ms >= 0) {
            size_t timeout_ticks = pdMS_TO_TICKS(cfg->timeout_ms);
            uint32_t expired = xTaskGetTickCount() - start;
            if (expired >= timeout_ticks) {
                aiio_log_w( "Failed to open new connection in specified timeout");
                AIIO_INT_EVENT_TRACKER_CAPTURE(tls->error_handle, AIIO_TLS_ERR_TYPE_AIIO, AIIO_ERR_TLS_CONNECTION_TIMEOUT);
                return 0;
            }
        }
    }
    return 0;
}

/*
 * @brief      Create a new TLS/SSL non-blocking connection
 */
int aiio_tls_conn_new_async(const char *hostname, int hostlen, int port, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    return aiio_tls_low_level_conn(hostname, hostlen, port, cfg, tls);
}

static int get_port(const char *url, struct http_parser_url *u)
{
    if (u->field_data[UF_PORT].len) {
        return strtol(&url[u->field_data[UF_PORT].off], NULL, 10);
    } else {
        if (strncasecmp(&url[u->field_data[UF_SCHEMA].off], "http", u->field_data[UF_SCHEMA].len) == 0) {
            return 80;
        } else if (strncasecmp(&url[u->field_data[UF_SCHEMA].off], "https", u->field_data[UF_SCHEMA].len) == 0) {
            return 443;
        }
    }
    return 0;
}

aiio_tls_t *aiio_tls_conn_http_new(const char *url, const aiio_tls_cfg_t *cfg)
{
    /* Parse URI */
    struct http_parser_url u;
    http_parser_url_init(&u);
    http_parser_parse_url(url, strlen(url), 0, &u);
    aiio_tls_t *tls = aiio_tls_init();
    if (!tls) {
        return NULL;
    }
    /* Connect to host */
    if (aiio_tls_conn_new_sync(&url[u.field_data[UF_HOST].off], u.field_data[UF_HOST].len,
                              get_port(url, &u), cfg, tls) == 1) {
        return tls;
    }
    aiio_tls_conn_destroy(tls);
    return NULL;
}

/**
 * @brief      Create a new TLS/SSL connection with a given "HTTP" url
 */
int aiio_tls_conn_http_new_sync(const char *url, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    /* Parse URI */
    struct http_parser_url u;
    http_parser_url_init(&u);
    http_parser_parse_url(url, strlen(url), 0, &u);

    /* Connect to host */
    return aiio_tls_conn_new_sync(&url[u.field_data[UF_HOST].off], u.field_data[UF_HOST].len,
                                  get_port(url, &u), cfg, tls);
}

/**
 * @brief      Create a new non-blocking TLS/SSL connection with a given "HTTP" url
 */
int aiio_tls_conn_http_new_async(const char *url, const aiio_tls_cfg_t *cfg, aiio_tls_t *tls)
{
    /* Parse URI */
    struct http_parser_url u;
    http_parser_url_init(&u);
    http_parser_parse_url(url, strlen(url), 0, &u);

    /* Connect to host */
    return aiio_tls_conn_new_async(&url[u.field_data[UF_HOST].off], u.field_data[UF_HOST].len,
                                  get_port(url, &u), cfg, tls);
}

#ifdef CONFIG_TLS_USING_MBEDTLS

mbedtls_x509_crt *aiio_tls_get_global_ca_store(void)
{
    return _tls_get_global_ca_store();
}

#endif /* CONFIG_TLS_USING_MBEDTLS */

#ifdef CONFIG_TLS_CLIENT_SESSION_TICKETS
aiio_tls_client_session_t *aiio_tls_get_client_session(aiio_tls_t *tls)
{
    return _tls_get_client_session(tls);
}
#endif /* CONFIG_TLS_CLIENT_SESSION_TICKETS */


#ifdef CONFIG_TLS_SERVER
int aiio_tls_cfg_server_session_tickets_init(aiio_tls_cfg_server_t *cfg)
{
#if defined(CONFIG_TLS_SERVER_SESSION_TICKETS)
    if (!cfg || cfg->ticket_ctx) {
        return AIIO_ERR_INVALID_ARG;
    }
    cfg->ticket_ctx = calloc(1, sizeof(aiio_tls_server_session_ticket_ctx_t));
    if (!cfg->ticket_ctx) {
        return AIIO_ERR_NO_MEM;
    }
    int ret =  _tls_server_session_ticket_ctx_init(cfg->ticket_ctx);
    if (ret != AIIO_OK) {
        free(cfg->ticket_ctx);
    }
    return ret;
#else
    return AIIO_ERR_NOT_SUPPORTED;
#endif
}

void aiio_tls_cfg_server_session_tickets_free(aiio_tls_cfg_server_t *cfg)
{
#if defined(CONFIG_TLS_SERVER_SESSION_TICKETS)
    if (cfg && cfg->ticket_ctx) {
        _tls_server_session_ticket_ctx_free(cfg->ticket_ctx);
    }
#endif
}

/**
 * @brief      Create a server side TLS/SSL connection
 */
int aiio_tls_server_session_create(aiio_tls_cfg_server_t *cfg, int sockfd, aiio_tls_t *tls)
{
    return _tls_server_session_create(cfg, sockfd, tls);
}
/**
 * @brief      Close the server side TLS/SSL connection and free any allocated resources.
 */
void aiio_tls_server_session_delete(aiio_tls_t *tls)
{
    return _tls_server_session_delete(tls);
}
#endif /* CONFIG_TLS_SERVER */

ssize_t aiio_tls_get_bytes_avail(aiio_tls_t *tls)
{
    return _tls_get_bytes_avail(tls);
}

int aiio_tls_get_conn_sockfd(aiio_tls_t *tls, int *sockfd)
{
    if (!tls || !sockfd) {
        aiio_log_e( "Invalid arguments passed");
        return AIIO_ERR_INVALID_ARG;
    }
    *sockfd = tls->sockfd;
    return AIIO_OK;
}

int aiio_tls_get_and_clear_last_error(aiio_tls_error_handle_t h, int *aiio_tls_code, int *aiio_tls_flags)
{
    if (!h) {
        return AIIO_ERR_INVALID_STATE;
    }
    int last_err = h->last_error;
    if (aiio_tls_code) {
        *aiio_tls_code = h->aiio_tls_error_code;
    }
    if (aiio_tls_flags) {
        *aiio_tls_flags = h->aiio_tls_flags;
    }
    memset(h, 0, sizeof(aiio_tls_last_error_t));
    return last_err;
}

int aiio_tls_init_global_ca_store(void)
{
    return _tls_init_global_ca_store();
}

int aiio_tls_set_global_ca_store(const unsigned char *cacert_pem_buf, const unsigned int cacert_pem_bytes)
{
    return _tls_set_global_ca_store(cacert_pem_buf, cacert_pem_bytes);
}

void aiio_tls_free_global_ca_store(void)
{
    return _tls_free_global_ca_store();
}
