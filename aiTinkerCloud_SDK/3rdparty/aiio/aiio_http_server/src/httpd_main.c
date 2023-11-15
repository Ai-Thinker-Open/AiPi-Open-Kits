#include <string.h>
// #include <sys/socket.h>
#include "aiio_chip_spec.h"
#include <sys/param.h>
#include <errno.h>
#include <aiio_log.h>
#include <aiio_err.h>
#include <assert.h>

#include "aiio_http_server.h"
#include "aiio_httpd_priv.h"
#include "ctrl_sock.h"

typedef struct {
    fd_set *fdset;
    struct aiio_httpd_data *hd;
} process_session_context_t;

static aiio_err_t httpd_accept_conn(struct aiio_httpd_data *hd, int listen_fd)
{
    /* If no space is available for new session, close the least recently used one */
    if (hd->config.lru_purge_enable == true) {
        if (!aiio_httpd_is_sess_available(hd)) {
            /* Queue asynchronous closure of the least recently used session */
            return aiio_httpd_sess_close_lru(hd);
            /* Returning from this allowes the main server thread to process
             * the queued asynchronous control message for closing LRU session.
             * Since connection request hasn't been addressed yet using accept()
             * therefore httpd_accept_conn() will be called again, but this time
             * with space available for one session
             */
        }
    }

    struct sockaddr_in addr_from;
    socklen_t addr_from_len = sizeof(addr_from);
    int new_fd = accept(listen_fd, (struct sockaddr *)&addr_from, &addr_from_len);
    if (new_fd < 0) {
        aiio_log_w("error in accept (%d)", errno);
        return AIIO_FAIL;
    }
    aiio_log_d("newfd = %d", new_fd);

    struct timeval tv;
    /* Set recv timeout of this fd as per config */
    tv.tv_sec = hd->config.recv_wait_timeout;
    tv.tv_usec = 0;
    setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

    /* Set send timeout of this fd as per config */
    tv.tv_sec = hd->config.send_wait_timeout;
    tv.tv_usec = 0;
    setsockopt(new_fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv));

    if (AIIO_OK != aiio_httpd_sess_new(hd, new_fd)) {
        aiio_log_w("session creation failed");
        close(new_fd);
        return AIIO_FAIL;
    }
    aiio_log_d("complete");
    return AIIO_OK;
}

struct httpd_ctrl_data {
    enum httpd_ctrl_msg {
        HTTPD_CTRL_SHUTDOWN,
        HTTPD_CTRL_WORK,
    } hc_msg;
    aiio_httpd_work_fn_t hc_work;
    void *hc_work_arg;
};

aiio_err_t aiio_httpd_queue_work(aiio_httpd_handle_t handle, aiio_httpd_work_fn_t work, void *arg)
{
    if (handle == NULL || work == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    struct httpd_ctrl_data msg = {
        .hc_msg = HTTPD_CTRL_WORK,
        .hc_work = work,
        .hc_work_arg = arg,
    };

    int ret = cs_send_to_ctrl_sock(hd->msg_fd, hd->config.ctrl_port, &msg, sizeof(msg));
    if (ret < 0) {
        aiio_log_w("failed to queue work");
        return AIIO_FAIL;
    }

    return AIIO_OK;
}

aiio_err_t aiio_httpd_get_client_list(aiio_httpd_handle_t handle, size_t *fds, int *client_fds)
{
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    if (hd == NULL || fds == NULL || *fds == 0 || client_fds == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }
    size_t max_fds = *fds;
    *fds = 0;
    for (int i = 0; i < hd->config.max_open_sockets; ++i) {
        if (hd->hd_sd[i].fd != -1) {
            if (*fds < max_fds) {
                client_fds[(*fds)++] = hd->hd_sd[i].fd;
            } else {
                return AIIO_ERR_INVALID_ARG;
            }
        }
    }
    return AIIO_OK;
}

void *aiio_httpd_get_global_user_ctx(aiio_httpd_handle_t handle)
{
    return ((struct aiio_httpd_data *)handle)->config.global_user_ctx;
}

void *aiio_httpd_get_global_transport_ctx(aiio_httpd_handle_t handle)
{
    return ((struct aiio_httpd_data *)handle)->config.global_transport_ctx;
}


static void httpd_process_ctrl_msg(struct aiio_httpd_data *hd)
{
    struct httpd_ctrl_data msg;
    int ret = recv(hd->ctrl_fd, &msg, sizeof(msg), 0);
    if (ret <= 0) {
        aiio_log_w("error in recv (%d)", errno);
        return;
    }
    if (ret != sizeof(msg)) {
        aiio_log_w("incomplete msg");
        return;
    }

    switch (msg.hc_msg) {
    case HTTPD_CTRL_WORK:
        if (msg.hc_work) {
            aiio_log_d("work");
            (*msg.hc_work)(msg.hc_work_arg);
        }
        break;
    case HTTPD_CTRL_SHUTDOWN:
        aiio_log_d("shutdown");
        hd->hd_td.status = THREAD_STOPPING;
        break;
    default:
        break;
    }
}

// Called for each session from httpd_server
static int httpd_process_session(struct aiio_sock_db *session, void *context)
{
    if ((!session) || (!context)) {
        return 0;
    }

    if (session->fd < 0) {
        return 1;
    }

    process_session_context_t *ctx = (process_session_context_t *)context;
    int fd = session->fd;

    if (FD_ISSET(fd, ctx->fdset) || aiio_httpd_sess_pending(ctx->hd, session)) {
        aiio_log_d("processing socket %d", fd);
        if (aiio_httpd_sess_process(ctx->hd, session) != AIIO_OK) {
            aiio_httpd_sess_delete(ctx->hd, session); // Delete session
        }
    }
    return 1;
}

/* Manage in-coming connection or data requests */
static aiio_err_t httpd_server(struct aiio_httpd_data *hd)
{
    fd_set read_set;
    FD_ZERO(&read_set);
    if (hd->config.lru_purge_enable || aiio_httpd_is_sess_available(hd)) {
        /* Only listen for new connections if server has capacity to
         * handle more (or when LRU purge is enabled, in which case
         * older connections will be closed) */
        FD_SET(hd->listen_fd, &read_set);
    }
    FD_SET(hd->ctrl_fd, &read_set);

    int tmp_max_fd;
    aiio_httpd_sess_set_descriptors(hd, &read_set, &tmp_max_fd);
    int maxfd = MAX(hd->listen_fd, tmp_max_fd);
    tmp_max_fd = maxfd;
    maxfd = MAX(hd->ctrl_fd, tmp_max_fd);

    aiio_log_d("doing select maxfd+1 = %d", maxfd + 1);
    int active_cnt = select(maxfd + 1, &read_set, NULL, NULL, NULL);
    if (active_cnt < 0) {
        aiio_log_e("error in select (%d)", errno);
        aiio_httpd_sess_delete_invalid(hd);
        return AIIO_OK;
    }

    /* Case0: Do we have a control message? */
    if (FD_ISSET(hd->ctrl_fd, &read_set)) {
        aiio_log_d("processing ctrl message");
        httpd_process_ctrl_msg(hd);
        if (hd->hd_td.status == THREAD_STOPPING) {
            aiio_log_d("stopping thread");
            return AIIO_FAIL;
        }
    }

    /* Case1: Do we have any activity on the current data
     * sessions? */
    process_session_context_t context = {
        .fdset = &read_set,
        .hd = hd
    };
    aiio_httpd_sess_enum(hd, httpd_process_session, &context);

    /* Case2: Do we have any incoming connection requests to
     * process? */
    if (FD_ISSET(hd->listen_fd, &read_set)) {
        aiio_log_d("processing listen socket %d", hd->listen_fd);
        if (httpd_accept_conn(hd, hd->listen_fd) != AIIO_OK) {
            aiio_log_w("error accepting new connection");
        }
    }
    return AIIO_OK;
}

/* The main HTTPD thread */
static void httpd_thread(void *arg)
{
    int ret;
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) arg;
    hd->hd_td.status = THREAD_RUNNING;

    aiio_log_d("web server started");
    while (1) {
        ret = httpd_server(hd);
        if (ret != AIIO_OK) {
            break;
        }
    }

    aiio_log_d("web server exiting");
    close(hd->msg_fd);
    cs_free_ctrl_sock(hd->ctrl_fd);
    aiio_httpd_sess_close_all(hd);
    close(hd->listen_fd);
    hd->hd_td.status = THREAD_STOPPED;
    httpd_os_thread_delete();
}

static aiio_err_t httpd_server_init(struct aiio_httpd_data *hd)
{
#if CONFIG_LWIP_IPV6
    int fd = socket(PF_INET6, SOCK_STREAM, 0);
#else
    int fd = socket(PF_INET, SOCK_STREAM, 0);
#endif
    if (fd < 0) {
        aiio_log_e("error in socket (%d)", errno);
        return AIIO_FAIL;
    }
#if CONFIG_LWIP_IPV6
    struct in6_addr inaddr_any = IN6ADDR_ANY_INIT;
    struct sockaddr_in6 serv_addr = {
        .sin6_family  = PF_INET6,
        .sin6_addr    = inaddr_any,
        .sin6_port    = htons(hd->config.server_port)
    };
#else
    struct sockaddr_in serv_addr = {
        .sin_family   = PF_INET,
        .sin_addr     = {
            .s_addr = htonl(INADDR_ANY)
        },
        .sin_port     = htons(hd->config.server_port)
    };
#endif
    /* Enable SO_REUSEADDR to allow binding to the same
     * address and port when restarting the server */
    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        /* This will fail if CONFIG_LWIP_SO_REUSE is not enabled. But
         * it does not affect the normal working of the HTTP Server */
        aiio_log_w("error enabling SO_REUSEADDR (%d)", errno);
    }

    int ret = bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        aiio_log_e("error in bind (%d)", errno);
        close(fd);
        return AIIO_FAIL;
    }

    ret = listen(fd, hd->config.backlog_conn);
    if (ret < 0) {
        aiio_log_e("error in listen (%d)", errno);
        close(fd);
        return AIIO_FAIL;
    }

    int ctrl_fd = cs_create_ctrl_sock(hd->config.ctrl_port);
    if (ctrl_fd < 0) {
        aiio_log_e("error in creating ctrl socket (%d)", errno);
        close(fd);
        return AIIO_FAIL;
    }

    int msg_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (msg_fd < 0) {
        aiio_log_e("error in creating msg socket (%d)", errno);
        close(fd);
        close(ctrl_fd);
        return AIIO_FAIL;
    }

    hd->listen_fd = fd;
    hd->ctrl_fd = ctrl_fd;
    hd->msg_fd  = msg_fd;
    return AIIO_OK;
}

static struct aiio_httpd_data *httpd_create(const aiio_httpd_config_t *config)
{
    /* Allocate memory for httpd instance data */
    struct aiio_httpd_data *hd = calloc(1, sizeof(struct aiio_httpd_data));
    if (!hd) {
        aiio_log_e("Failed to allocate memory for HTTP server instance");
        return NULL;
    }
    hd->hd_calls = calloc(config->max_uri_handlers, sizeof(aiio_httpd_uri_t *));
    if (!hd->hd_calls) {
        aiio_log_e("Failed to allocate memory for HTTP URI handlers");
        free(hd);
        return NULL;
    }
    hd->hd_sd = calloc(config->max_open_sockets, sizeof(struct aiio_sock_db));
    if (!hd->hd_sd) {
        aiio_log_e("Failed to allocate memory for HTTP session data");
        free(hd->hd_calls);
        free(hd);
        return NULL;
    }
    struct aiio_httpd_req_aux *ra = &hd->hd_req_aux;
    ra->resp_hdrs = calloc(config->max_resp_headers, sizeof(struct resp_hdr));
    if (!ra->resp_hdrs) {
        aiio_log_e("Failed to allocate memory for HTTP response headers");
        free(hd->hd_sd);
        free(hd->hd_calls);
        free(hd);
        return NULL;
    }
    hd->err_handler_fns = calloc(HTTPD_ERR_CODE_MAX, sizeof(aiio_httpd_err_handler_func_t));
    if (!hd->err_handler_fns) {
        aiio_log_e("Failed to allocate memory for HTTP error handlers");
        free(ra->resp_hdrs);
        free(hd->hd_sd);
        free(hd->hd_calls);
        free(hd);
        return NULL;
    }
    /* Save the configuration for this instance */
    hd->config = *config;
    return hd;
}

static void httpd_delete(struct aiio_httpd_data *hd)
{
    struct aiio_httpd_req_aux *ra = &hd->hd_req_aux;
    /* Free memory of httpd instance data */
    free(hd->err_handler_fns);
    free(ra->resp_hdrs);
    free(hd->hd_sd);

    /* Free registered URI handlers */
    aiio_httpd_unregister_all_uri_handlers(hd);
    free(hd->hd_calls);
    free(hd);
}

aiio_err_t aiio_httpd_start(aiio_httpd_handle_t *handle, const aiio_httpd_config_t *config)
{
    if (  config == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    /* Sanity check about whether LWIP is configured for providing the
     * maximum number of open sockets sufficient for the server. Though,
     * this check doesn't guarantee that many sockets will actually be
     * available at runtime as other processes may use up some sockets.
     * Note that server also uses 3 sockets for its internal use :
     *     1) listening for new TCP connections
     *     2) for sending control messages over UDP
     *     3) for receiving control messages over UDP
     * So the total number of required sockets is max_open_sockets + 3
     */
    if (CONFIG_LWIP_MAX_SOCKETS < config->max_open_sockets + 3) {
        aiio_log_e("Configuration option max_open_sockets is too large (max allowed %d)\n\t"
                 "Either decrease this or configure LWIP_MAX_SOCKETS to a larger value",
                 CONFIG_LWIP_MAX_SOCKETS - 3);
        return AIIO_ERR_INVALID_ARG;
    }

    struct aiio_httpd_data *hd = httpd_create(config);
    if (hd == NULL) {
        /* Failed to allocate memory */
        return AIIO_ERR_HTTPD_ALLOC_MEM;
    }

    if (httpd_server_init(hd) != AIIO_OK) {
        httpd_delete(hd);
        return AIIO_FAIL;
    }

    aiio_httpd_sess_init(hd);
    if (httpd_os_thread_create(&hd->hd_td.handle, "httpd",
                               hd->config.stack_size,
                               hd->config.task_priority,
                               httpd_thread, hd,
                               hd->config.core_id) != AIIO_OK) {
        /* Failed to launch task */
        httpd_delete(hd);
        return AIIO_ERR_HTTPD_TASK;
    }

    *handle = (aiio_httpd_handle_t *)hd;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_stop(aiio_httpd_handle_t handle)
{
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    if (hd == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    struct httpd_ctrl_data msg;
    memset(&msg, 0, sizeof(msg));
    msg.hc_msg = HTTPD_CTRL_SHUTDOWN;
    cs_send_to_ctrl_sock(hd->msg_fd, hd->config.ctrl_port, &msg, sizeof(msg));

    aiio_log_d("sent control msg to stop server");
    while (hd->hd_td.status != THREAD_STOPPED) {
        httpd_os_thread_sleep(100);
    }

    /* Release global user context, if not NULL */
    if (hd->config.global_user_ctx) {
        if (hd->config.global_user_ctx_free_fn) {
            hd->config.global_user_ctx_free_fn(hd->config.global_user_ctx);
        } else {
            free(hd->config.global_user_ctx);
        }
        hd->config.global_user_ctx = NULL;
    }

    /* Release global transport context, if not NULL */
    if (hd->config.global_transport_ctx) {
        if (hd->config.global_transport_ctx_free_fn) {
            hd->config.global_transport_ctx_free_fn(hd->config.global_transport_ctx);
        } else {
            free(hd->config.global_transport_ctx);
        }
        hd->config.global_transport_ctx = NULL;
    }

    aiio_log_d("server stopped");
    httpd_delete(hd);
    return AIIO_OK;
}
