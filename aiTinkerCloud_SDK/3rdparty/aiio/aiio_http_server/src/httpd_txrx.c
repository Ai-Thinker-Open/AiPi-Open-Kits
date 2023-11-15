#include <errno.h>
#include <aiio_log.h>
#include <aiio_err.h>

#include "aiio_http_server.h"
#include "aiio_httpd_priv.h"

aiio_err_t aiio_httpd_sess_set_send_override(aiio_httpd_handle_t hd, int sockfd, aiio_httpd_send_func_t send_func)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, sockfd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }
    sess->send_fn = send_func;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_sess_set_recv_override(aiio_httpd_handle_t hd, int sockfd, aiio_httpd_recv_func_t recv_func)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, sockfd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }
    sess->recv_fn = recv_func;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_sess_set_pending_override(aiio_httpd_handle_t hd, int sockfd, aiio_httpd_pending_func_t pending_func)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, sockfd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }
    sess->pending_fn = pending_func;
    return AIIO_OK;
}

int aiio_httpd_send(aiio_httpd_req_t *r, const char *buf, size_t buf_len)
{
    if (r == NULL || buf == NULL) {
        return HTTPD_SOCK_ERR_INVALID;
    }

    if (!aiio_httpd_valid_req(r)) {
        return HTTPD_SOCK_ERR_INVALID;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    int ret = ra->sd->send_fn(ra->sd->handle, ra->sd->fd, buf, buf_len, 0);
    if (ret < 0) {
        aiio_log_d("error in send_fn");
        return ret;
    }
    return ret;
}

static aiio_err_t httpd_send_all(aiio_httpd_req_t *r, const char *buf, size_t buf_len)
{
    struct aiio_httpd_req_aux *ra = r->aux;
    int ret;

    while (buf_len > 0) {
        ret = ra->sd->send_fn(ra->sd->handle, ra->sd->fd, buf, buf_len, 0);
        if (ret < 0) {
            aiio_log_d("error in send_fn");
            return AIIO_FAIL;
        }
        aiio_log_d("sent = %d", ret);
        buf     += ret;
        buf_len -= ret;
    }
    return AIIO_OK;
}

static size_t httpd_recv_pending(aiio_httpd_req_t *r, char *buf, size_t buf_len)
{
    struct aiio_httpd_req_aux *ra = r->aux;
    size_t offset = sizeof(ra->sd->pending_data) - ra->sd->pending_len;

    /* buf_len must not be greater than remaining_len */
    buf_len = MIN(ra->sd->pending_len, buf_len);
    memcpy(buf, ra->sd->pending_data + offset, buf_len);

    ra->sd->pending_len -= buf_len;
    return buf_len;
}

int aiio_httpd_recv_with_opt(aiio_httpd_req_t *r, char *buf, size_t buf_len, bool halt_after_pending)
{
    aiio_log_d("requested length = %d", buf_len);

    size_t pending_len = 0;
    struct aiio_httpd_req_aux *ra = r->aux;

    /* First fetch pending data from local buffer */
    if (ra->sd->pending_len > 0) {
        aiio_log_d("pending length = %d", ra->sd->pending_len);
        pending_len = httpd_recv_pending(r, buf, buf_len);
        buf     += pending_len;
        buf_len -= pending_len;

        /* If buffer filled then no need to recv.
         * If asked to halt after receiving pending data then
         * return with received length */
        if (!buf_len || halt_after_pending) {
            return pending_len;
        }
    }

    /* Receive data of remaining length */
    int ret = ra->sd->recv_fn(ra->sd->handle, ra->sd->fd, buf, buf_len, 0);
    if (ret < 0) {
        aiio_log_d("error in recv_fn");
        if ((ret == HTTPD_SOCK_ERR_TIMEOUT) && (pending_len != 0)) {
            /* If recv() timeout occurred, but pending data is
             * present, return length of pending data.
             * This behavior is similar to that of socket recv()
             * function, which, in case has only partially read the
             * requested length, due to timeout, returns with read
             * length, rather than error */
            return pending_len;
        }
        return ret;
    }

    aiio_log_d("received length = %d", ret + pending_len);
    return ret + pending_len;
}

int httpd_recv(aiio_httpd_req_t *r, char *buf, size_t buf_len)
{
    return aiio_httpd_recv_with_opt(r, buf, buf_len, false);
}

size_t aiio_httpd_unrecv(struct aiio_httpd_req *r, const char *buf, size_t buf_len)
{
    struct aiio_httpd_req_aux *ra = r->aux;
    /* Truncate if external buf_len is greater than pending_data buffer size */
    ra->sd->pending_len = MIN(sizeof(ra->sd->pending_data), buf_len);

    /* Copy data into internal pending_data buffer with the exact offset
     * such that it is right aligned inside the buffer */
    size_t offset = sizeof(ra->sd->pending_data) - ra->sd->pending_len;
    memcpy(ra->sd->pending_data + offset, buf, ra->sd->pending_len);
    aiio_log_d("length = %d", ra->sd->pending_len);
    return ra->sd->pending_len;
}

/**
 * This API appends an additional header field-value pair in the HTTP response.
 * But the header isn't sent out until any of the send APIs is executed.
 */
aiio_err_t aiio_httpd_resp_set_hdr(aiio_httpd_req_t *r, const char *field, const char *value)
{
    if (r == NULL || field == NULL || value == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) r->handle;

    /* Number of additional headers is limited */
    if (ra->resp_hdrs_count >= hd->config.max_resp_headers) {
        return AIIO_ERR_HTTPD_RESP_HDR;
    }

    /* Assign header field-value pair */
    ra->resp_hdrs[ra->resp_hdrs_count].field = field;
    ra->resp_hdrs[ra->resp_hdrs_count].value = value;
    ra->resp_hdrs_count++;

    aiio_log_d("new header = %s: %s", field, value);
    return AIIO_OK;
}

/**
 * This API sets the status of the HTTP response to the value specified.
 * But the status isn't sent out until any of the send APIs is executed.
 */
aiio_err_t aiio_httpd_resp_set_status(aiio_httpd_req_t *r, const char *status)
{
    if (r == NULL || status == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    ra->status = (char *)status;
    return AIIO_OK;
}

/**
 * This API sets the method/type of the HTTP response to the value specified.
 * But the method isn't sent out until any of the send APIs is executed.
 */
aiio_err_t aiio_httpd_resp_set_type(aiio_httpd_req_t *r, const char *type)
{
    if (r == NULL || type == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    ra->content_type = (char *)type;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_resp_send(aiio_httpd_req_t *r, const char *buf, ssize_t buf_len)
{
    if (r == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    const char *httpd_hdr_str = "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n";
    const char *colon_separator = ": ";
    const char *cr_lf_seperator = "\r\n";

    if (buf_len == HTTPD_RESP_USE_STRLEN) {
        buf_len = strlen(buf);
    }

    /* Request headers are no longer available */
    ra->req_hdrs_count = 0;

    /* Size of essential headers is limited by scratch buffer size */
    if (snprintf(ra->scratch, sizeof(ra->scratch), httpd_hdr_str,
                 ra->status, ra->content_type, buf_len) >= sizeof(ra->scratch)) {
        return AIIO_ERR_HTTPD_RESP_HDR;
    }

    /* Sending essential headers */
    if (httpd_send_all(r, ra->scratch, strlen(ra->scratch)) != AIIO_OK) {
        return AIIO_ERR_HTTPD_RESP_SEND;
    }

    /* Sending additional headers based on set_header */
    for (unsigned i = 0; i < ra->resp_hdrs_count; i++) {
        /* Send header field */
        if (httpd_send_all(r, ra->resp_hdrs[i].field, strlen(ra->resp_hdrs[i].field)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
        /* Send ': ' */
        if (httpd_send_all(r, colon_separator, strlen(colon_separator)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
        /* Send header value */
        if (httpd_send_all(r, ra->resp_hdrs[i].value, strlen(ra->resp_hdrs[i].value)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
        /* Send CR + LF */
        if (httpd_send_all(r, cr_lf_seperator, strlen(cr_lf_seperator)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
    }

    /* End header section */
    if (httpd_send_all(r, cr_lf_seperator, strlen(cr_lf_seperator)) != AIIO_OK) {
        return AIIO_ERR_HTTPD_RESP_SEND;
    }

    /* Sending content */
    if (buf && buf_len) {
        if (httpd_send_all(r, buf, buf_len) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
    }
    return AIIO_OK;
}

aiio_err_t aiio_httpd_resp_send_chunk(aiio_httpd_req_t *r, const char *buf, ssize_t buf_len)
{
    if (r == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    if (buf_len == HTTPD_RESP_USE_STRLEN) {
        buf_len = strlen(buf);
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    const char *httpd_chunked_hdr_str = "HTTP/1.1 %s\r\nContent-Type: %s\r\nTransfer-Encoding: chunked\r\n";
    const char *colon_separator = ": ";
    const char *cr_lf_seperator = "\r\n";

    /* Request headers are no longer available */
    ra->req_hdrs_count = 0;

    if (!ra->first_chunk_sent) {
        /* Size of essential headers is limited by scratch buffer size */
        if (snprintf(ra->scratch, sizeof(ra->scratch), httpd_chunked_hdr_str,
                     ra->status, ra->content_type) >= sizeof(ra->scratch)) {
            return AIIO_ERR_HTTPD_RESP_HDR;
        }

        /* Sending essential headers */
        if (httpd_send_all(r, ra->scratch, strlen(ra->scratch)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }

        /* Sending additional headers based on set_header */
        for (unsigned i = 0; i < ra->resp_hdrs_count; i++) {
            /* Send header field */
            if (httpd_send_all(r, ra->resp_hdrs[i].field, strlen(ra->resp_hdrs[i].field)) != AIIO_OK) {
                return AIIO_ERR_HTTPD_RESP_SEND;
            }
            /* Send ': ' */
            if (httpd_send_all(r, colon_separator, strlen(colon_separator)) != AIIO_OK) {
                return AIIO_ERR_HTTPD_RESP_SEND;
            }
            /* Send header value */
            if (httpd_send_all(r, ra->resp_hdrs[i].value, strlen(ra->resp_hdrs[i].value)) != AIIO_OK) {
                return AIIO_ERR_HTTPD_RESP_SEND;
            }
            /* Send CR + LF */
            if (httpd_send_all(r, cr_lf_seperator, strlen(cr_lf_seperator)) != AIIO_OK) {
                return AIIO_ERR_HTTPD_RESP_SEND;
            }
        }

        /* End header section */
        if (httpd_send_all(r, cr_lf_seperator, strlen(cr_lf_seperator)) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
        ra->first_chunk_sent = true;
    }

    /* Sending chunked content */
    char len_str[10];
    snprintf(len_str, sizeof(len_str), "%x\r\n", buf_len);
    if (httpd_send_all(r, len_str, strlen(len_str)) != AIIO_OK) {
        return AIIO_ERR_HTTPD_RESP_SEND;
    }

    if (buf) {
        if (httpd_send_all(r, buf, (size_t) buf_len) != AIIO_OK) {
            return AIIO_ERR_HTTPD_RESP_SEND;
        }
    }

    /* Indicate end of chunk */
    if (httpd_send_all(r, "\r\n", strlen("\r\n")) != AIIO_OK) {
        return AIIO_ERR_HTTPD_RESP_SEND;
    }
    return AIIO_OK;
}

aiio_err_t aiio_httpd_resp_send_err(aiio_httpd_req_t *req, aiio_httpd_err_code_t error, const char *usr_msg)
{
    aiio_err_t ret;
    const char *msg;
    const char *status;

    switch (error) {
        case HTTPD_501_METHOD_NOT_IMPLEMENTED:
            status = "501 Method Not Implemented";
            msg    = "Request method is not supported by server";
            break;
        case HTTPD_505_VERSION_NOT_SUPPORTED:
            status = "505 Version Not Supported";
            msg    = "HTTP version not supported by server";
            break;
        case HTTPD_400_BAD_REQUEST:
            status = "400 Bad Request";
            msg    = "Server unable to understand request due to invalid syntax";
            break;
        case HTTPD_401_UNAUTHORIZED:
            status = "401 Unauthorized";
            msg    = "Server known the client's identify and it must authenticate itself to get he requested response";
            break;
        case HTTPD_403_FORBIDDEN:
            status = "403 Forbidden";
            msg    = "Server is refusing to give the requested resource to the client";
            break;
        case HTTPD_404_NOT_FOUND:
            status = "404 Not Found";
            msg    = "This URI does not exist";
            break;
        case HTTPD_405_METHOD_NOT_ALLOWED:
            status = "405 Method Not Allowed";
            msg    = "Request method for this URI is not handled by server";
            break;
        case HTTPD_408_REQ_TIMEOUT:
            status = "408 Request Timeout";
            msg    = "Server closed this connection";
            break;
        case HTTPD_414_URI_TOO_LONG:
            status = "414 URI Too Long";
            msg    = "URI is too long for server to interpret";
            break;
        case HTTPD_411_LENGTH_REQUIRED:
            status = "411 Length Required";
            msg    = "Chunked encoding not supported by server";
            break;
        case HTTPD_431_REQ_HDR_FIELDS_TOO_LARGE:
            status = "431 Request Header Fields Too Large";
            msg    = "Header fields are too long for server to interpret";
            break;
        case HTTPD_500_INTERNAL_SERVER_ERROR:
        default:
            status = "500 Internal Server Error";
            msg    = "Server has encountered an unexpected error";
    }

    /* If user has provided custom message, override default message */
    msg = usr_msg ? usr_msg : msg;
    aiio_log_w("%s - %s", status, msg);

    /* Set error code in HTTP response */
    aiio_httpd_resp_set_status(req, status);
    aiio_httpd_resp_set_type(req, HTTPD_TYPE_TEXT);

#ifdef CONFIG_HTTPD_ERR_RESP_NO_DELAY
    /* Use TCP_NODELAY option to force socket to send data in buffer
     * This ensures that the error message is sent before the socket
     * is closed */
    struct aiio_httpd_req_aux *ra = req->aux;
    int nodelay = 1;
    if (setsockopt(ra->sd->fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) < 0) {
        /* If failed to turn on TCP_NODELAY, throw warning and continue */
        aiio_log_w("error calling setsockopt : %d", errno);
        nodelay = 0;
    }
#endif

    /* Send HTTP error message */
    ret = aiio_httpd_resp_send(req, msg, HTTPD_RESP_USE_STRLEN);

#ifdef CONFIG_HTTPD_ERR_RESP_NO_DELAY
    /* If TCP_NODELAY was set successfully above, time to disable it */
    if (nodelay == 1) {
        nodelay = 0;
        if (setsockopt(ra->sd->fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) < 0) {
            /* If failed to turn off TCP_NODELAY, throw error and
             * return failure to signal for socket closure */
            aiio_log_e("error calling setsockopt : %d", errno);
            return AIIO_ERR_INVALID_STATE;
        }
    }
#endif

    return ret;
}

aiio_err_t aiio_httpd_register_err_handler(aiio_httpd_handle_t handle,
                                     aiio_httpd_err_code_t error,
                                     aiio_httpd_err_handler_func_t err_handler_fn)
{
    if (handle == NULL || error >= HTTPD_ERR_CODE_MAX) {
        return AIIO_ERR_INVALID_ARG;
    }

    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    hd->err_handler_fns[error] = err_handler_fn;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_req_handle_err(aiio_httpd_req_t *req, aiio_httpd_err_code_t error)
{
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) req->handle;
    aiio_err_t ret;

    /* Invoke custom error handler if configured */
    if (hd->err_handler_fns[error]) {
        ret = hd->err_handler_fns[error](req, error);

        /* If error code is 500, force return failure
         * irrespective of the handler's return value */
        ret = (error == HTTPD_500_INTERNAL_SERVER_ERROR ? AIIO_FAIL : ret);
    } else {
        /* If no handler is registered for this error default
         * behavior is to send the HTTP error response and
         * return failure for closure of underlying socket */
        aiio_httpd_resp_send_err(req, error, NULL);
        ret = AIIO_FAIL;
    }
    return ret;
}

int aiio_httpd_req_recv(aiio_httpd_req_t *r, char *buf, size_t buf_len)
{
    if (r == NULL || buf == NULL) {
        return HTTPD_SOCK_ERR_INVALID;
    }

    if (!aiio_httpd_valid_req(r)) {
        aiio_log_w("invalid request");
        return HTTPD_SOCK_ERR_INVALID;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    aiio_log_d("remaining length = %d", ra->remaining_len);

    if (buf_len > ra->remaining_len) {
        buf_len = ra->remaining_len;
    }
    if (buf_len == 0) {
        return buf_len;
    }

    int ret = httpd_recv(r, buf, buf_len);
    if (ret < 0) {
        aiio_log_d("error in httpd_recv");
        return ret;
    }
    ra->remaining_len -= ret;
    aiio_log_d("received length = %d", ret);
    return ret;
}

int aiio_httpd_req_to_sockfd(aiio_httpd_req_t *r)
{
    if (r == NULL) {
        return -1;
    }

    if (!aiio_httpd_valid_req(r)) {
        aiio_log_w("invalid request");
        return -1;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    return ra->sd->fd;
}

static int httpd_sock_err(const char *ctx, int sockfd)
{
    int errval;
    aiio_log_w("error in %s : %d", ctx, errno);

    switch(errno) {
    case EAGAIN:
    case EINTR:
        errval = HTTPD_SOCK_ERR_TIMEOUT;
        break;
    case EINVAL:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        errval = HTTPD_SOCK_ERR_INVALID;
        break;
    default:
        errval = HTTPD_SOCK_ERR_FAIL;
    }
    return errval;
}

int aiio_httpd_default_send(aiio_httpd_handle_t hd, int sockfd, const char *buf, size_t buf_len, int flags)
{
    (void)hd;
    if (buf == NULL) {
        return HTTPD_SOCK_ERR_INVALID;
    }

    int ret = send(sockfd, buf, buf_len, flags);
    if (ret < 0) {
        return httpd_sock_err("send", sockfd);
    }
    return ret;
}

int aiio_httpd_default_recv(aiio_httpd_handle_t hd, int sockfd, char *buf, size_t buf_len, int flags)
{
    (void)hd;
    if (buf == NULL) {
        return HTTPD_SOCK_ERR_INVALID;
    }

    int ret = recv(sockfd, buf, buf_len, flags);
    if (ret < 0) {
        return httpd_sock_err("recv", sockfd);
    }
    return ret;
}

int aiio_httpd_socket_send(aiio_httpd_handle_t hd, int sockfd, const char *buf, size_t buf_len, int flags)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, sockfd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }
    if (!sess->send_fn) {
        return AIIO_ERR_INVALID_STATE;
    }
    return sess->send_fn(hd, sockfd, buf, buf_len, flags);
}

int aiio_httpd_socket_recv(aiio_httpd_handle_t hd, int sockfd, char *buf, size_t buf_len, int flags)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, sockfd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }
    if (!sess->recv_fn) {
        return AIIO_ERR_INVALID_STATE;
    }
    return sess->recv_fn(hd, sockfd, buf, buf_len, flags);
}
