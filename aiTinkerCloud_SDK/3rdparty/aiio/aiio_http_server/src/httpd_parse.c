#include <stdlib.h>
#include <sys/param.h>
#include <aiio_log.h>
#include <aiio_err.h>
#include <http_parser.h>

#include "aiio_http_server.h"
#include "aiio_httpd_priv.h"
#include "osal.h"

typedef struct {
    /* Parser settings for http_parser_execute() */
    http_parser_settings settings;

    /* Request being parsed */
    struct aiio_httpd_req *req;

    /* Status of the parser describes the part of the
     * HTTP request packet being processed at any moment.
     */
    enum {
        PARSING_IDLE = 0,
        PARSING_URL,
        PARSING_HDR_FIELD,
        PARSING_HDR_VALUE,
        PARSING_BODY,
        PARSING_COMPLETE,
        PARSING_FAILED
    } status;

    /* Response error code in case of PARSING_FAILED */
    aiio_httpd_err_code_t error;

    /* For storing last callback parameters */
    struct {
        const char *at;
        size_t      length;
    } last;

    /* State variables */
    bool   paused;          /*!< Parser is paused */
    size_t pre_parsed;      /*!< Length of data to be skipped while parsing */
    size_t raw_datalen;     /*!< Full length of the raw data in scratch buffer */
} parser_data_t;

static aiio_err_t verify_url (http_parser *parser)
{
    parser_data_t *parser_data  = (parser_data_t *) parser->data;
    struct aiio_httpd_req *r         = parser_data->req;
    struct aiio_httpd_req_aux *ra    = r->aux;
    struct http_parser_url *res = &ra->url_parse_res;

    /* Get previous values of the parser callback arguments */
    const char *at = parser_data->last.at;
    size_t  length = parser_data->last.length;

    if ((r->method = parser->method) < 0) {
        aiio_log_w("HTTP Operation not supported");
        parser_data->error = HTTPD_501_METHOD_NOT_IMPLEMENTED;
        return AIIO_FAIL;
    }

    if (sizeof(r->uri) < (length + 1)) {
        aiio_log_w("URI length (%d) greater than supported (%d)",
                 length, sizeof(r->uri));
        parser_data->error = HTTPD_414_URI_TOO_LONG;
        return AIIO_FAIL;
    }

    /* Keep URI with terminating null character. Note URI string pointed
     * by 'at' is not NULL terminated, therefore use length provided by
     * parser while copying the URI to buffer */
    strlcpy((char *)r->uri, at, (length + 1));
    aiio_log_d("received URI = %s", r->uri);

    /* Make sure version is HTTP/1.1 */
    if ((parser->http_major != 1) && (parser->http_minor != 1)) {
        aiio_log_w("unsupported HTTP version = %d.%d",
                 parser->http_major, parser->http_minor);
        parser_data->error = HTTPD_505_VERSION_NOT_SUPPORTED;
        return AIIO_FAIL;
    }

    /* Parse URL and keep result for later */
    http_parser_url_init(res);
    if (http_parser_parse_url(r->uri, strlen(r->uri),
                              r->method == HTTP_CONNECT, res)) {
        aiio_log_w("http_parser_parse_url failed with errno = %d",
                              parser->http_errno);
        parser_data->error = HTTPD_400_BAD_REQUEST;
        return AIIO_FAIL;
    }
    return AIIO_OK;
}

/* http_parser callback on finding url in HTTP request
 * Will be invoked ATLEAST once every packet
 */
static aiio_err_t cb_url(http_parser *parser,
                        const char *at, size_t length)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;

    if (parser_data->status == PARSING_IDLE) {
        aiio_log_d("message begin");

        /* Store current values of the parser callback arguments */
        parser_data->last.at     = at;
        parser_data->last.length = 0;
        parser_data->status      = PARSING_URL;
    } else if (parser_data->status != PARSING_URL) {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    aiio_log_d("processing url = %.*s", length, at);

    /* Update length of URL string */
    if ((parser_data->last.length += length) > HTTPD_MAX_URI_LEN) {
        aiio_log_w("URI length (%d) greater than supported (%d)",
                 parser_data->last.length, HTTPD_MAX_URI_LEN);
        parser_data->error = HTTPD_414_URI_TOO_LONG;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }
    return AIIO_OK;
}

static aiio_err_t pause_parsing(http_parser *parser, const char* at)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;
    struct aiio_httpd_req *r        = parser_data->req;
    struct aiio_httpd_req_aux *ra   = r->aux;

    /* The length of data that was not parsed due to interruption
     * and hence needs to be read again later for parsing */
    ssize_t unparsed = parser_data->raw_datalen - (at - ra->scratch);
    if (unparsed < 0) {
        aiio_log_e("parsing beyond valid data = %d", -unparsed);
        return AIIO_ERR_INVALID_STATE;
    }

    /* Push back the un-parsed data into pending buffer for
     * receiving again with aiio_httpd_recv_with_opt() later when
     * read_block() executes */
    if (unparsed && (unparsed != aiio_httpd_unrecv(r, at, unparsed))) {
        aiio_log_e("data too large for un-recv = %d", unparsed);
        return AIIO_FAIL;
    }

    /* Signal http_parser to pause execution and save the maximum
     * possible length, of the yet un-parsed data, that may get
     * parsed before http_parser_execute() returns. This pre_parsed
     * length will be updated then to reflect the actual length
     * that got parsed, and must be skipped when parsing resumes */
    parser_data->pre_parsed = unparsed;
    http_parser_pause(parser, 1);
    parser_data->paused = true;
    aiio_log_d("paused");
    return AIIO_OK;
}

static size_t continue_parsing(http_parser *parser, size_t length)
{
    parser_data_t *data = (parser_data_t *) parser->data;

    /* Part of the received data may have been parsed earlier
     * so we must skip that before parsing resumes */
    length = MIN(length, data->pre_parsed);
    data->pre_parsed -= length;
    aiio_log_d("skip pre-parsed data of size = %d", length);

    http_parser_pause(parser, 0);
    data->paused = false;
    aiio_log_d("un-paused");
    return length;
}

/* http_parser callback on header field in HTTP request
 * May be invoked ATLEAST once every header field
 */
static aiio_err_t cb_header_field(http_parser *parser, const char *at, size_t length)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;
    struct aiio_httpd_req *r        = parser_data->req;
    struct aiio_httpd_req_aux *ra   = r->aux;

    /* Check previous status */
    if (parser_data->status == PARSING_URL) {
        if (verify_url(parser) != AIIO_OK) {
            /* verify_url would already have set the
             * error field of parser data, so only setting
             * status to failed */
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }

        aiio_log_d("headers begin");
        /* Last at is set to start of scratch where headers
         * will be received next */
        parser_data->last.at     = ra->scratch;
        parser_data->last.length = 0;
        parser_data->status      = PARSING_HDR_FIELD;

        /* Stop parsing for now and give control to process */
        if (pause_parsing(parser, at) != AIIO_OK) {
            parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }
    } else if (parser_data->status == PARSING_HDR_VALUE) {
        /* Overwrite terminator (CRLFs) following last header
         * (key: value) pair with null characters */
        char *term_start = (char *)parser_data->last.at + parser_data->last.length;
        memset(term_start, '\0', at - term_start);

        /* Store current values of the parser callback arguments */
        parser_data->last.at     = at;
        parser_data->last.length = 0;
        parser_data->status      = PARSING_HDR_FIELD;

        /* Increment header count */
        ra->req_hdrs_count++;
    } else if (parser_data->status != PARSING_HDR_FIELD) {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    aiio_log_d("processing field = %.*s", length, at);

    /* Update length of header string */
    parser_data->last.length += length;
    return AIIO_OK;
}

/* http_parser callback on header value in HTTP request.
 * May be invoked ATLEAST once every header value
 */
static aiio_err_t cb_header_value(http_parser *parser, const char *at, size_t length)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;

    /* Check previous status */
    if (parser_data->status == PARSING_HDR_FIELD) {
        /* Store current values of the parser callback arguments */
        parser_data->last.at     = at;
        parser_data->last.length = 0;
        parser_data->status      = PARSING_HDR_VALUE;

        if (length == 0) {
            /* As per behavior of http_parser, when length > 0,
             * `at` points to the start of CRLF. But, in the
             * case when header value is empty (zero length),
             * then `at` points to the position right after
             * the CRLF. Since for our purpose we need `last.at`
             * to point to exactly where the CRLF starts, it
             * needs to be adjusted by the right offset */
            char *at_adj = (char *)parser_data->last.at;
            /* Find the end of header field string */
            while (*(--at_adj) != ':');
            /* Now skip leading spaces' */
            while (*(++at_adj) == ' ');
            /* Now we are at the right position */
            parser_data->last.at = at_adj;
        }
    } else if (parser_data->status != PARSING_HDR_VALUE) {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    aiio_log_d("processing value = %.*s", length, at);

    /* Update length of header string */
    parser_data->last.length += length;
    return AIIO_OK;
}

/* http_parser callback on completing headers in HTTP request.
 * Will be invoked ONLY once every packet
 */
static aiio_err_t cb_headers_complete(http_parser *parser)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;
    struct aiio_httpd_req *r        = parser_data->req;
    struct aiio_httpd_req_aux *ra   = r->aux;

    /* Check previous status */
    if (parser_data->status == PARSING_URL) {
        aiio_log_d("no headers");
        if (verify_url(parser) != AIIO_OK) {
            /* verify_url would already have set the
             * error field of parser data, so only setting
             * status to failed */
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }
    } else if (parser_data->status == PARSING_HDR_VALUE) {
        /* Locate end of last header */
        char *at = (char *)parser_data->last.at + parser_data->last.length;

        /* Check if there is data left to parse. This value should
         * at least be equal to the number of line terminators, i.e. 2 */
        ssize_t remaining_length = parser_data->raw_datalen - (at - ra->scratch);
        if (remaining_length < 2) {
            aiio_log_e("invalid length of data remaining to be parsed");
            parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }

        /* Locate end of headers section by skipping the remaining
         * two line terminators. No assumption is made here about the
         * termination sequence used apart from the necessity that it
         * must end with an LF, because:
         *      1) some clients may send non standard LFs instead of
         *         CRLFs for indicating termination.
         *      2) it is the responsibility of http_parser to check
         *         that the termination is either CRLF or LF and
         *         not any other sequence */
        unsigned short remaining_terminators = 2;
        while (remaining_length-- && remaining_terminators) {
            if (*at == '\n') {
                remaining_terminators--;
            }
            /* Overwrite termination characters with null */
            *(at++) = '\0';
        }
        if (remaining_terminators) {
            aiio_log_e("incomplete termination of headers");
            parser_data->error = HTTPD_400_BAD_REQUEST;
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }

        /* Place the parser ptr right after the end of headers section */
        parser_data->last.at = at;

        /* Increment header count */
        ra->req_hdrs_count++;
    } else {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    /* In absence of body/chunked encoding, http_parser sets content_len to -1 */
    r->content_len = ((int)parser->content_length != -1 ?
                      parser->content_length : 0);

    aiio_log_d("bytes read     = %d",  parser->nread);
    aiio_log_d("content length = %zu", r->content_len);

    /* Handle upgrade requests - only WebSocket is supported for now */
    if (parser->upgrade) {
#ifdef CONFIG_HTTPD_WS_SUPPORT
        aiio_log_d("Got an upgrade request");

        /* If there's no "Upgrade" header field, then it's not WebSocket. */
        char ws_upgrade_hdr_val[] = "websocket";
        if (aiio_httpd_req_get_hdr_value_str(r, "Upgrade", ws_upgrade_hdr_val, sizeof(ws_upgrade_hdr_val)) != AIIO_OK) {
            aiio_log_w("Upgrade header does not match the length of \"websocket\"");
            parser_data->error = HTTPD_400_BAD_REQUEST;
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }

        /* If "Upgrade" field's key is not "websocket", then we should also forget about it. */
        if (strcasecmp("websocket", ws_upgrade_hdr_val) != 0) {
            aiio_log_w("Upgrade header found but it's %s", ws_upgrade_hdr_val);
            parser_data->error = HTTPD_400_BAD_REQUEST;
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }

        /* Now set handshake flag to true */
        ra->ws_handshake_detect = true;
#else
        aiio_log_d("WS functions has been disabled, Upgrade request is not supported.");
        parser_data->error = HTTPD_400_BAD_REQUEST;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
#endif
    }

    parser_data->status = PARSING_BODY;
    ra->remaining_len = r->content_len;
    return AIIO_OK;
}

/* Last http_parser callback if body present in HTTP request.
 * Will be invoked ONLY once every packet
 */
static aiio_err_t cb_on_body(http_parser *parser, const char *at, size_t length)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;

    /* Check previous status */
    if (parser_data->status != PARSING_BODY) {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    /* Pause parsing so that if part of another packet
     * is in queue then it doesn't get parsed, which
     * may reset the parser state and cause current
     * request packet to be lost */
    if (pause_parsing(parser, at) != AIIO_OK) {
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    parser_data->last.at     = 0;
    parser_data->last.length = 0;
    parser_data->status      = PARSING_COMPLETE;
    aiio_log_d("body begins");
    return AIIO_OK;
}

/* Last http_parser callback if body absent in HTTP request.
 * Will be invoked ONLY once every packet
 */
static aiio_err_t cb_no_body(http_parser *parser)
{
    parser_data_t *parser_data = (parser_data_t *) parser->data;

    /* Check previous status */
    if (parser_data->status == PARSING_URL) {
        aiio_log_d("no headers");
        if (verify_url(parser) != AIIO_OK) {
            /* verify_url would already have set the
             * error field of parser data, so only setting
             * status to failed */
            parser_data->status = PARSING_FAILED;
            return AIIO_FAIL;
        }
    } else if (parser_data->status != PARSING_BODY) {
        aiio_log_e("unexpected state transition");
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    /* Pause parsing so that if part of another packet
     * is in queue then it doesn't get parsed, which
     * may reset the parser state and cause current
     * request packet to be lost */
    if (pause_parsing(parser, parser_data->last.at) != AIIO_OK) {
        parser_data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
        parser_data->status = PARSING_FAILED;
        return AIIO_FAIL;
    }

    parser_data->last.at     = 0;
    parser_data->last.length = 0;
    parser_data->status      = PARSING_COMPLETE;
    aiio_log_d("message complete");
    return AIIO_OK;
}

static int read_block(aiio_httpd_req_t *req, size_t offset, size_t length)
{
    struct aiio_httpd_req_aux *raux  = req->aux;

    /* Limits the read to scratch buffer size */
    ssize_t buf_len = MIN(length, (sizeof(raux->scratch) - offset));
    if (buf_len <= 0) {
        return 0;
    }

    /* Receive data into buffer. If data is pending (from unrecv) then return
     * immediately after receiving pending data, as pending data may just complete
     * this request packet. */
    int nbytes = aiio_httpd_recv_with_opt(req, raux->scratch + offset, buf_len, true);
    if (nbytes < 0) {
        aiio_log_d("error in httpd_recv");
        /* If timeout occurred allow the
         * situation to be handled */
        if (nbytes == HTTPD_SOCK_ERR_TIMEOUT) {
            /* Invoke error handler which may return AIIO_OK
             * to signal for retrying call to recv(), else it may
             * return AIIO_FAIL to signal for closure of socket */
            return (aiio_httpd_req_handle_err(req, HTTPD_408_REQ_TIMEOUT) == AIIO_OK) ?
                    HTTPD_SOCK_ERR_TIMEOUT : HTTPD_SOCK_ERR_FAIL;
        }
        /* Some socket error occurred. Return failure
         * to force closure of underlying socket.
         * Error message is not sent as socket may not
         * be valid anymore */
        return HTTPD_SOCK_ERR_FAIL;
    } else if (nbytes == 0) {
        aiio_log_d("connection closed");
        /* Connection closed by client so no
         * need to send error response */
        return HTTPD_SOCK_ERR_FAIL;
    }

    aiio_log_d("received HTTP request block size = %d", nbytes);
    return nbytes;
}

static int parse_block(http_parser *parser, size_t offset, size_t length)
{
    parser_data_t        *data  = (parser_data_t *)(parser->data);
    aiio_httpd_req_t          *req   = data->req;
    struct aiio_httpd_req_aux *raux  = req->aux;
    size_t nparsed = 0;

    if (!length) {
        /* Parsing is still happening but nothing to
         * parse means no more space left on buffer,
         * therefore it can be inferred that the
         * request URI/header must be too long */
        aiio_log_w("request URI/header too long");
        switch (data->status) {
            case PARSING_URL:
                data->error = HTTPD_414_URI_TOO_LONG;
                break;
            case PARSING_HDR_FIELD:
            case PARSING_HDR_VALUE:
                data->error = HTTPD_431_REQ_HDR_FIELDS_TOO_LARGE;
                break;
            default:
                aiio_log_e("unexpected state");
                data->error = HTTPD_500_INTERNAL_SERVER_ERROR;
                break;
        }
        data->status = PARSING_FAILED;
        return -1;
    }

    /* Un-pause the parsing if paused */
    if (data->paused) {
        nparsed = continue_parsing(parser, length);
        length -= nparsed;
        offset += nparsed;
        if (!length) {
            return nparsed;
        }
    }

    /* Execute http_parser */
    nparsed = http_parser_execute(parser, &data->settings,
                                  raux->scratch + offset, length);

    /* Check state */
    if (data->status == PARSING_FAILED) {
        /* It is expected that the error field of
         * parser data should have been set by now */
        aiio_log_w("parsing failed");
        return -1;
    } else if (data->paused) {
        /* Update the value of pre_parsed which was set when
         * pause_parsing() was called. (length - nparsed) is
         * the length of the data that will need to be parsed
         * again later and hence must be deducted from the
         * pre_parsed length */
        data->pre_parsed -= (length - nparsed);
        return 0;
    } else if (nparsed != length) {
        /* http_parser error */
        data->error  = HTTPD_400_BAD_REQUEST;
        data->status = PARSING_FAILED;
        aiio_log_w("incomplete (%d/%d) with parser error = %d",
                 nparsed, length, parser->http_errno);
        return -1;
    }

    /* Return with the total length of the request packet
     * that has been parsed till now */
    aiio_log_d("parsed block size = %d", offset + nparsed);
    return offset + nparsed;
}

static void parse_init(aiio_httpd_req_t *r, http_parser *parser, parser_data_t *data)
{
    /* Initialize parser data */
    memset(data, 0, sizeof(parser_data_t));
    data->req = r;

    /* Initialize parser */
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = (void *)data;

    /* Initialize parser settings */
    http_parser_settings_init(&data->settings);

    /* Set parser callbacks */
    data->settings.on_url              = cb_url;
    data->settings.on_header_field     = cb_header_field;
    data->settings.on_header_value     = cb_header_value;
    data->settings.on_headers_complete = cb_headers_complete;
    data->settings.on_body             = cb_on_body;
    data->settings.on_message_complete = cb_no_body;
}

/* Function that receives TCP data and runs parser on it
 */
static aiio_err_t httpd_parse_req(struct aiio_httpd_data *hd)
{
    aiio_httpd_req_t *r = &hd->hd_req;
    int blk_len,  offset;
    http_parser   parser;
    parser_data_t parser_data;

    /* Initialize parser */
    parse_init(r, &parser, &parser_data);

    /* Set offset to start of scratch buffer */
    offset = 0;
    do {
        /* Read block into scratch buffer */
        if ((blk_len = read_block(r, offset, PARSER_BLOCK_SIZE)) < 0) {
            if (blk_len == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry read in case of non-fatal timeout error.
                 * read_block() ensures that the timeout error is
                 * handled properly so that this doesn't get stuck
                 * in an infinite loop */
                continue;
            }
            /* If not HTTPD_SOCK_ERR_TIMEOUT, returned error must
             * be HTTPD_SOCK_ERR_FAIL which means we need to return
             * failure and thereby close the underlying socket */
            return AIIO_FAIL;
        }

        /* This is used by the callbacks to track
         * data usage of the buffer */
        parser_data.raw_datalen = blk_len + offset;

        /* Parse data block from buffer */
        if ((offset = parse_block(&parser, offset, blk_len)) < 0) {
            /* HTTP error occurred.
             * Send error code as response status and
             * invoke error handler */
            return aiio_httpd_req_handle_err(r, parser_data.error);
        }
    } while (parser_data.status != PARSING_COMPLETE);

    aiio_log_d("parsing complete");
    return aiio_httpd_uri(hd);
}

static void init_req(aiio_httpd_req_t *r, aiio_httpd_config_t *config)
{
    r->handle = 0;
    r->method = 0;
    memset((char*)r->uri, 0, sizeof(r->uri));
    r->content_len = 0;
    r->aux = 0;
    r->user_ctx = 0;
    r->sess_ctx = 0;
    r->free_ctx = 0;
    r->ignore_sess_ctx_changes = 0;
}

static void init_req_aux(struct aiio_httpd_req_aux *ra, aiio_httpd_config_t *config)
{
    ra->sd = 0;
    memset(ra->scratch, 0, sizeof(ra->scratch));
    ra->remaining_len = 0;
    ra->status = 0;
    ra->content_type = 0;
    ra->first_chunk_sent = 0;
    ra->req_hdrs_count = 0;
    ra->resp_hdrs_count = 0;
#if CONFIG_HTTPD_WS_SUPPORT
    ra->ws_handshake_detect = false;
#endif
    memset(ra->resp_hdrs, 0, config->max_resp_headers * sizeof(struct resp_hdr));
}

static void httpd_req_cleanup(aiio_httpd_req_t *r)
{
    struct aiio_httpd_req_aux *ra = r->aux;

    /* Check if the context has changed and needs to be cleared */
    if ((r->ignore_sess_ctx_changes == false) && (ra->sd->ctx != r->sess_ctx)) {
        aiio_httpd_sess_free_ctx(ra->sd->ctx, ra->sd->free_ctx);
    }

#if CONFIG_HTTPD_WS_SUPPORT
    /* Close the socket when a WebSocket Close request is received */
    if (ra->sd->ws_close) {
        aiio_log_d("Try closing WS connection at FD: %d", ra->sd->fd);
        aiio_httpd_sess_trigger_close(r->handle, ra->sd->fd);
    }
#endif

    /* Retrieve session info from the request into the socket database. */
    ra->sd->ctx = r->sess_ctx;
    ra->sd->free_ctx = r->free_ctx;
    ra->sd->ignore_sess_ctx_changes = r->ignore_sess_ctx_changes;

    /* Clear out the request and request_aux structures */
    ra->sd = NULL;
    r->handle = NULL;
    r->aux = NULL;
    r->user_ctx = NULL;
}

/* Function that processes incoming TCP data and
 * updates the http request data aiio_httpd_req_t
 */
aiio_err_t aiio_httpd_req_new(struct aiio_httpd_data *hd, struct aiio_sock_db *sd)
{
    aiio_httpd_req_t *r = &hd->hd_req;
    init_req(r, &hd->config);
    init_req_aux(&hd->hd_req_aux, &hd->config);
    r->handle = hd;
    r->aux = &hd->hd_req_aux;

    /* Associate the request to the socket */
    struct aiio_httpd_req_aux *ra = r->aux;
    ra->sd = sd;

    /* Set defaults */
    ra->status = (char *)HTTPD_200;
    ra->content_type = (char *)HTTPD_TYPE_TEXT;
    ra->first_chunk_sent = false;

    /* Copy session info to the request */
    r->sess_ctx = sd->ctx;
    r->free_ctx = sd->free_ctx;
    r->ignore_sess_ctx_changes = sd->ignore_sess_ctx_changes;

    aiio_err_t ret;

#ifdef CONFIG_HTTPD_WS_SUPPORT
    /* Copy user_ctx to the request */
    r->user_ctx = sd->ws_user_ctx;
    /* Handle WebSocket */
    aiio_log_d("New request, has WS? %s, sd->ws_handler valid? %s, sd->ws_close? %s",
             sd->ws_handshake_done ? "Yes" : "No",
             sd->ws_handler != NULL ? "Yes" : "No",
             sd->ws_close ? "Yes" : "No");
    if (sd->ws_handshake_done && sd->ws_handler != NULL) {
        ret = aiio_httpd_ws_get_frame_type(r);
        aiio_log_d("New WS request from existing socket, ws_type=%d", ra->ws_type);

        if (ra->ws_type == HTTPD_WS_TYPE_CLOSE) {
            /*  Only mark ws_close to true if it's a CLOSE frame */
            sd->ws_close = true;
        } else if (ra->ws_type == HTTPD_WS_TYPE_PONG) {
            /* Pass the PONG frames to the handler as well, as user app might send PINGs */
            aiio_log_d("Received PONG frame");
        }

        /* Call handler if it's a non-control frame (or if handler requests control frames, as well) */
        if (ret == AIIO_OK &&
            (ra->ws_type < HTTPD_WS_TYPE_CLOSE || sd->ws_control_frames)) {
            ret = sd->ws_handler(r);
        }

        if (ret != AIIO_OK) {
            httpd_req_cleanup(r);
        }
        return ret;
    }
#endif

    /* Parse request */
    ret = httpd_parse_req(hd);
    if (ret != AIIO_OK) {
        httpd_req_cleanup(r);
    }
    return ret;
}

/* Function that resets the http request data
 */
aiio_err_t aiio_httpd_req_delete(struct aiio_httpd_data *hd)
{
    aiio_httpd_req_t *r = &hd->hd_req;
    struct aiio_httpd_req_aux *ra = r->aux;

    /* Finish off reading any pending/leftover data */
    while (ra->remaining_len) {
        /* Any length small enough not to overload the stack, but large
         * enough to finish off the buffers fast */
        char dummy[CONFIG_HTTPD_PURGE_BUF_LEN];
        int recv_len = MIN(sizeof(dummy), ra->remaining_len);
        recv_len = aiio_httpd_req_recv(r, dummy, recv_len);
        if (recv_len <= 0) {
            httpd_req_cleanup(r);
            return AIIO_FAIL;
        }

        aiio_log_d("purging data size : %d bytes", recv_len);

#ifdef CONFIG_HTTPD_LOG_PURGE_DATA
        /* Enabling this will log discarded binary HTTP content data at
         * Debug level. For large content data this may not be desirable
         * as it will clutter the log */
        aiio_log_d("================= PURGED DATA =================");
        // ESP_LOG_BUFFER_HEX_LEVEL(TAG, dummy, recv_len, ESP_LOG_DEBUG);
        aiio_log_d("===============================================");
#endif
    }

    httpd_req_cleanup(r);
    return AIIO_OK;
}

/* Validates the request to prevent users from calling APIs, that are to
 * be called only inside URI handler, outside the handler context
 */
bool aiio_httpd_validate_req_ptr(aiio_httpd_req_t *r)
{
    if (r) {
        struct aiio_httpd_data *hd = (struct aiio_httpd_data *) r->handle;
        if (hd) {
            /* Check if this function is running in the context of
             * the correct httpd server thread */
            if (httpd_os_thread_handle() == hd->hd_td.handle) {
                return true;
            }
        }
    }
    return false;
}

/* Helper function to get a URL query tag from a query string of the type param1=val1&param2=val2 */
aiio_err_t aiio_httpd_query_key_value(const char *qry_str, const char *key, char *val, size_t val_size)
{
    if (qry_str == NULL || key == NULL || val == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    const char   *qry_ptr = qry_str;
    const size_t  buf_len = val_size;

    while (strlen(qry_ptr)) {
        /* Search for the '=' character. Else, it would mean
         * that the parameter is invalid */
        const char *val_ptr = strchr(qry_ptr, '=');
        if (!val_ptr) {
            break;
        }
        size_t offset = val_ptr - qry_ptr;

        /* If the key, does not match, continue searching.
         * Compare lengths first as key from url is not
         * null terminated (has '=' in the end) */
        if ((offset != strlen(key)) ||
            (strncasecmp(qry_ptr, key, offset))) {
            /* Get the name=val string. Multiple name=value pairs
             * are separated by '&' */
            qry_ptr = strchr(val_ptr, '&');
            if (!qry_ptr) {
                break;
            }
            qry_ptr++;
            continue;
        }

        /* Locate start of next query */
        qry_ptr = strchr(++val_ptr, '&');
        /* Or this could be the last query, in which
         * case get to the end of query string */
        if (!qry_ptr) {
            qry_ptr = val_ptr + strlen(val_ptr);
        }

        /* Update value length, including one byte for null */
        val_size = qry_ptr - val_ptr + 1;

        /* Copy value to the caller's buffer. */
        strlcpy(val, val_ptr, MIN(val_size, buf_len));

        /* If buffer length is smaller than needed, return truncation error */
        if (buf_len < val_size) {
            return AIIO_ERR_HTTPD_RESULT_TRUNC;
        }
        return AIIO_OK;
    }
    aiio_log_d("key %s not found", key);
    return AIIO_ERR_NOT_FOUND;
}

size_t aiio_httpd_req_get_url_query_len(aiio_httpd_req_t *r)
{
    if (r == NULL) {
        return 0;
    }

    if (!aiio_httpd_valid_req(r)) {
        return 0;
    }

    struct aiio_httpd_req_aux   *ra  = r->aux;
    struct http_parser_url *res = &ra->url_parse_res;

    /* Check if query field is present in the URL */
    if (res->field_set & (1 << UF_QUERY)) {
        return res->field_data[UF_QUERY].len;
    }
    return 0;
}

aiio_err_t aiio_httpd_req_get_url_query_str(aiio_httpd_req_t *r, char *buf, size_t buf_len)
{
    if (r == NULL || buf == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux   *ra  = r->aux;
    struct http_parser_url *res = &ra->url_parse_res;

    /* Check if query field is present in the URL */
    if (res->field_set & (1 << UF_QUERY)) {
        const char *qry = r->uri + res->field_data[UF_QUERY].off;

        /* Minimum required buffer len for keeping
         * null terminated query string */
        size_t min_buf_len = res->field_data[UF_QUERY].len + 1;

        strlcpy(buf, qry, MIN(buf_len, min_buf_len));
        if (buf_len < min_buf_len) {
            return AIIO_ERR_HTTPD_RESULT_TRUNC;
        }
        return AIIO_OK;
    }
    return AIIO_ERR_NOT_FOUND;
}

/* Get the length of the value string of a header request field */
size_t aiio_httpd_req_get_hdr_value_len(aiio_httpd_req_t *r, const char *field)
{
    if (r == NULL || field == NULL) {
        return 0;
    }

    if (!aiio_httpd_valid_req(r)) {
        return 0;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    const char   *hdr_ptr = ra->scratch;         /*!< Request headers are kept in scratch buffer */
    unsigned      count   = ra->req_hdrs_count;  /*!< Count set during parsing  */

    while (count--) {
        /* Search for the ':' character. Else, it would mean
         * that the field is invalid
         */
        const char *val_ptr = strchr(hdr_ptr, ':');
        if (!val_ptr) {
            break;
        }

        /* If the field, does not match, continue searching.
         * Compare lengths first as field from header is not
         * null terminated (has ':' in the end).
         */
        if ((val_ptr - hdr_ptr != strlen(field)) ||
            (strncasecmp(hdr_ptr, field, strlen(field)))) {
            if (count) {
                /* Jump to end of header field-value string */
                hdr_ptr = 1 + strchr(hdr_ptr, '\0');

                /* Skip all null characters (with which the line
                 * terminators had been overwritten) */
                while (*hdr_ptr == '\0') {
                    hdr_ptr++;
                }
            }
            continue;
        }

        /* Skip ':' */
        val_ptr++;

        /* Skip preceding space */
        while ((*val_ptr != '\0') && (*val_ptr == ' ')) {
            val_ptr++;
        }
        return strlen(val_ptr);
    }
    return 0;
}

/* Get the value of a field from the request headers */
aiio_err_t aiio_httpd_req_get_hdr_value_str(aiio_httpd_req_t *r, const char *field, char *val, size_t val_size)
{
    if (r == NULL || field == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    if (!aiio_httpd_valid_req(r)) {
        return AIIO_ERR_HTTPD_INVALID_REQ;
    }

    struct aiio_httpd_req_aux *ra = r->aux;
    const char   *hdr_ptr = ra->scratch;         /*!< Request headers are kept in scratch buffer */
    unsigned     count    = ra->req_hdrs_count;  /*!< Count set during parsing  */
    const size_t buf_len  = val_size;

    while (count--) {
        /* Search for the ':' character. Else, it would mean
         * that the field is invalid
         */
        const char *val_ptr = strchr(hdr_ptr, ':');
        if (!val_ptr) {
            break;
        }

        /* If the field, does not match, continue searching.
         * Compare lengths first as field from header is not
         * null terminated (has ':' in the end).
         */
        if ((val_ptr - hdr_ptr != strlen(field)) ||
            (strncasecmp(hdr_ptr, field, strlen(field)))) {
            if (count) {
                /* Jump to end of header field-value string */
                hdr_ptr = 1 + strchr(hdr_ptr, '\0');

                /* Skip all null characters (with which the line
                 * terminators had been overwritten) */
                while (*hdr_ptr == '\0') {
                    hdr_ptr++;
                }
            }
            continue;
        }

        /* Skip ':' */
        val_ptr++;

        /* Skip preceding space */
        while ((*val_ptr != '\0') && (*val_ptr == ' ')) {
            val_ptr++;
        }

        /* Get the NULL terminated value and copy it to the caller's buffer. */
        strlcpy(val, val_ptr, buf_len);

        /* Update value length, including one byte for null */
        val_size = strlen(val_ptr) + 1;

        /* If buffer length is smaller than needed, return truncation error */
        if (buf_len < val_size) {
            return AIIO_ERR_HTTPD_RESULT_TRUNC;
        }
        return AIIO_OK;
    }
    return AIIO_ERR_NOT_FOUND;
}

/* Helper function to get a cookie value from a cookie string of the type "cookie1=val1; cookie2=val2" */
aiio_err_t static httpd_cookie_key_value(const char *cookie_str, const char *key, char *val, size_t *val_size)
{
    if (cookie_str == NULL || key == NULL || val == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    const char *cookie_ptr = cookie_str;
    const size_t buf_len = *val_size;
    size_t _val_size = *val_size;

    while (strlen(cookie_ptr)) {
        /* Search for the '=' character. Else, it would mean
         * that the parameter is invalid */
        const char *val_ptr = strchr(cookie_ptr, '=');
        if (!val_ptr) {
            break;
        }
        size_t offset = val_ptr - cookie_ptr;

        /* If the key, does not match, continue searching.
         * Compare lengths first as key from cookie string is not
         * null terminated (has '=' in the end) */
        if ((offset != strlen(key)) || (strncasecmp(cookie_ptr, key, offset) != 0)) {
            /* Get the name=val string. Multiple name=value pairs
             * are separated by '; ' */
            cookie_ptr = strchr(val_ptr, ' ');
            if (!cookie_ptr) {
                break;
            }
            cookie_ptr++;
            continue;
        }

        /* Locate start of next query */
        cookie_ptr = strchr(++val_ptr, ';');
        /* Or this could be the last query, in which
         * case get to the end of query string */
        if (!cookie_ptr) {
            cookie_ptr = val_ptr + strlen(val_ptr);
        }

        /* Update value length, including one byte for null */
        _val_size = cookie_ptr - val_ptr + 1;

        /* Copy value to the caller's buffer. */
        strlcpy(val, val_ptr, MIN(_val_size, buf_len));

        /* If buffer length is smaller than needed, return truncation error */
        if (buf_len < _val_size) {
            *val_size = _val_size;
            return AIIO_ERR_HTTPD_RESULT_TRUNC;
        }
        /* Save amount of bytes copied to caller's buffer */
        *val_size = MIN(_val_size, buf_len);
        return AIIO_OK;
    }
    aiio_log_d("cookie %s not found", key);
    return AIIO_ERR_NOT_FOUND;
}

/* Get the value of a cookie from the request headers */
aiio_err_t aiio_httpd_req_get_cookie_val(aiio_httpd_req_t *req, const char *cookie_name, char *val, size_t *val_size)
{
    aiio_err_t ret;
    size_t hdr_len_cookie = aiio_httpd_req_get_hdr_value_len(req, "Cookie");
    char *cookie_str = NULL;

    if (hdr_len_cookie <= 0) {
        return AIIO_ERR_NOT_FOUND;
    }
    cookie_str = malloc(hdr_len_cookie + 1);
    if (cookie_str == NULL) {
        aiio_log_e("Failed to allocate memory for cookie string");
        return AIIO_ERR_NO_MEM;
    }

    if (aiio_httpd_req_get_hdr_value_str(req, "Cookie", cookie_str, hdr_len_cookie + 1) != AIIO_OK) {
        aiio_log_w("Cookie not found in header uri:[%s]", req->uri);
        free(cookie_str);
        return AIIO_ERR_NOT_FOUND;
    }

    ret = httpd_cookie_key_value(cookie_str, cookie_name, val, val_size);
    free(cookie_str);
    return ret;

}
