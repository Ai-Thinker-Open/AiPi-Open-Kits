#include <stdlib.h>
#include <string.h>
// #include <sys/random.h>
#include <aiio_log.h>
#include <aiio_err.h>
#include <mbedtls/sha1.h>
#include <mbedtls/base64.h>

#include "aiio_http_server.h"
#include "aiio_httpd_priv.h"
// #include "freertos/event_groups.h"

#ifdef CONFIG_HTTPD_WS_SUPPORT

#define WS_SEND_OK      (1 << 0)
#define WS_SEND_FAILED  (1 << 1)

typedef struct {
    aiio_httpd_ws_frame_t frame;
    aiio_httpd_handle_t handle;
    int socket;
    aiio_transfer_complete_cb callback;
    void *arg;
    bool blocking;
    EventGroupHandle_t transfer_done;
} async_transfer_t;

/*
 * Bit masks for WebSocket frames.
 * Please refer to RFC6455 Section 5.2 for more details.
 */
#define HTTPD_WS_CONTINUE       0x00U
#define HTTPD_WS_FIN_BIT        0x80U
#define HTTPD_WS_OPCODE_BITS    0x0fU
#define HTTPD_WS_MASK_BIT       0x80U
#define HTTPD_WS_LENGTH_BITS    0x7fU

/*
 * The magic GUID string used for handshake
 * Please refer to RFC6455 Section 1.3 for more details.
 */
static const char ws_magic_uuid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

/* Checks if any subprotocols from the comma seperated list matches the supported one
 *
 * Returns true if the response should contain a protocol field
*/

/**
 * @brief Checks if any subprotocols from the comma seperated list matches the supported one
 *
 * @param supported_subprotocol[in] The subprotocol supported by the URI
 * @param subprotocol[in],  [in]: A comma seperate list of subprotocols requested
 * @param buf_len Length of the buffer
 * @return true: found a matching subprotocol
 * @return false
 */
static bool httpd_ws_get_response_subprotocol(const char *supported_subprotocol, char *subprotocol, size_t buf_len)
{
    /* Request didnt contain any subprotocols */
    if (strnlen(subprotocol, buf_len) == 0) {
        return false;
    }

    if (supported_subprotocol == NULL) {
        aiio_log_w("Sec-WebSocket-Protocol %s not supported, URI do not support any subprotocols", subprotocol);
        return false;
    }

    /* Get first subprotocol from comma seperated list */
    char *rest = NULL;
    char *s = strtok_r(subprotocol, ", ", &rest);
    do {
        if (strncmp(s, supported_subprotocol, sizeof(subprotocol)) == 0) {
            aiio_log_d("Requested subprotocol supported: %s", s);
            return true;
        }
    } while ((s = strtok_r(NULL, ", ", &rest)) != NULL);

    aiio_log_w("Sec-WebSocket-Protocol %s not supported, supported subprotocol is %s", subprotocol, supported_subprotocol);

    /* No matches */
    return false;

}

aiio_err_t aiio_httpd_ws_respond_server_handshake(aiio_httpd_req_t *req, const char *supported_subprotocol)
{
    /* Probe if input parameters are valid or not */
    if (!req || !req->aux) {
        aiio_log_w("Argument is invalid");
        return AIIO_ERR_INVALID_ARG;
    }

    /* Detect handshake - reject if handshake was ALREADY performed */
    struct aiio_httpd_req_aux *req_aux = req->aux;
    if (req_aux->sd->ws_handshake_done) {
        aiio_log_w("State is invalid - Handshake has been performed");
        return AIIO_ERR_INVALID_STATE;
    }

    /* Detect WS version existence */
    char version_val[3] = { '\0' };
    if (aiio_httpd_req_get_hdr_value_str(req, "Sec-WebSocket-Version", version_val, sizeof(version_val)) != AIIO_OK) {
        aiio_log_w("\"Sec-WebSocket-Version\" is not found");
        return AIIO_ERR_NOT_FOUND;
    }

    /* Detect if WS version is "13" or not.
     * WS version must be 13 for now. Please refer to RFC6455 Section 4.1, Page 18 for more details. */
    if (strcasecmp(version_val, "13") != 0) {
        aiio_log_w("\"Sec-WebSocket-Version\" is not \"13\", it is: %s", version_val);
        return AIIO_ERR_INVALID_VERSION;
    }

    /* Grab Sec-WebSocket-Key (client key) from the header */
    /* Size of base64 coded string is equal '((input_size * 4) / 3) + (input_size / 96) + 6' including Z-term */
    char sec_key_encoded[28] = { '\0' };
    if (aiio_httpd_req_get_hdr_value_str(req, "Sec-WebSocket-Key", sec_key_encoded, sizeof(sec_key_encoded)) != AIIO_OK) {
        aiio_log_w("Cannot find client key");
        return AIIO_ERR_NOT_FOUND;
    }

    /* Prepare server key (Sec-WebSocket-Accept), concat the string */
    char server_key_encoded[33] = { '\0' };
    uint8_t server_key_hash[20] = { 0 };
    char server_raw_text[sizeof(sec_key_encoded) + sizeof(ws_magic_uuid) + 1] = { '\0' };

    strcpy(server_raw_text, sec_key_encoded);
    strcat(server_raw_text, ws_magic_uuid);

    aiio_log_d("Server key before encoding: %s", server_raw_text);

    /* Generate SHA-1 first and then encode to Base64 */
    size_t key_len = strlen(server_raw_text);
    mbedtls_sha1_ret((uint8_t *)server_raw_text, key_len, server_key_hash);

    size_t encoded_len = 0;
    mbedtls_base64_encode((uint8_t *)server_key_encoded, sizeof(server_key_encoded), &encoded_len,
                          server_key_hash, sizeof(server_key_hash));

    aiio_log_d("Generated server key: %s", server_key_encoded);

    char subprotocol[50] = { '\0' };
    if (aiio_httpd_req_get_hdr_value_str(req, "Sec-WebSocket-Protocol", subprotocol, sizeof(subprotocol) - 1) == AIIO_ERR_HTTPD_RESULT_TRUNC) {
        aiio_log_w("Sec-WebSocket-Protocol length exceeded buffer size of %d, was trunctated", sizeof(subprotocol));
    }


    /* Prepare the Switching Protocol response */
    char tx_buf[192] = { '\0' };
    int fmt_len = snprintf(tx_buf, sizeof(tx_buf),
                           "HTTP/1.1 101 Switching Protocols\r\n"
                           "Upgrade: websocket\r\n"
                           "Connection: Upgrade\r\n"
                           "Sec-WebSocket-Accept: %s\r\n", server_key_encoded);

    if (fmt_len < 0 || fmt_len > sizeof(tx_buf)) {
        aiio_log_w("Failed to prepare Tx buffer");
        return AIIO_FAIL;
    }

    if ( httpd_ws_get_response_subprotocol(supported_subprotocol, subprotocol, sizeof(subprotocol))) {
        aiio_log_d("subprotocol: %s", subprotocol);
        int r = snprintf(tx_buf + fmt_len, sizeof(tx_buf) - fmt_len, "Sec-WebSocket-Protocol: %s\r\n", supported_subprotocol);
        if (r <= 0) {
            aiio_log_e("Error in response generation"
                          "(snprintf of subprotocol returned %d, buffer size: %d", r, sizeof(tx_buf));
            return AIIO_FAIL;
        }

        fmt_len += r;

        if (fmt_len >= sizeof(tx_buf)) {
            aiio_log_e("Error in response generation"
                          "(snprintf of subprotocol returned %d, desired response len: %d, buffer size: %d", r, fmt_len, sizeof(tx_buf));
            return AIIO_FAIL;
        }
    }

    int r = snprintf(tx_buf + fmt_len, sizeof(tx_buf) - fmt_len, "\r\n");
    if (r <= 0) {
        aiio_log_e("Error in response generation"
                        "(snprintf of subprotocol returned %d, buffer size: %d", r, sizeof(tx_buf));
        return AIIO_FAIL;
    }
    fmt_len += r;
    if (fmt_len >= sizeof(tx_buf)) {
        aiio_log_e("Error in response generation"
                       "(snprintf of header terminal returned %d, desired response len: %d, buffer size: %d", r, fmt_len, sizeof(tx_buf));
        return AIIO_FAIL;
    }

    /* Send off the response */
    if (aiio_httpd_send(req, tx_buf, fmt_len) < 0) {
        aiio_log_w("Failed to send the response");
        return AIIO_FAIL;
    }

    return AIIO_OK;
}

static aiio_err_t httpd_ws_check_req(aiio_httpd_req_t *req)
{
    /* Probe if input parameters are valid or not */
    if (!req || !req->aux) {
        aiio_log_w("Argument is null");
        return AIIO_ERR_INVALID_ARG;
    }

    /* Detect handshake - reject if handshake was NOT YET performed */
    struct aiio_httpd_req_aux *req_aux = req->aux;
    if (!req_aux->sd->ws_handshake_done) {
        aiio_log_w("State is invalid - No handshake performed");
        return AIIO_ERR_INVALID_STATE;
    }

    return AIIO_OK;
}

static aiio_err_t httpd_ws_unmask_payload(uint8_t *payload, size_t len, const uint8_t *mask_key)
{
    if (len < 1 || !payload) {
        aiio_log_w("Invalid payload provided");
        return AIIO_ERR_INVALID_ARG;
    }

    for (size_t idx = 0; idx < len; idx++) {
        payload[idx] = (payload[idx] ^ mask_key[idx % 4]);
    }

    return AIIO_OK;
}

aiio_err_t aiio_httpd_ws_recv_frame(aiio_httpd_req_t *req, aiio_httpd_ws_frame_t *frame, size_t max_len)
{
    aiio_err_t ret = httpd_ws_check_req(req);
    if (ret != AIIO_OK) {
        return ret;
    }

    struct aiio_httpd_req_aux *aux = req->aux;
    if (aux == NULL) {
        aiio_log_w("Invalid Aux pointer");
        return AIIO_ERR_INVALID_ARG;
    }

    if (!frame) {
        aiio_log_w(LOG_FMT("Frame pointer is invalid");
        return AIIO_ERR_INVALID_ARG;
    }
    /* If frame len is 0, will get frame len from req. Otherwise regard frame len already achieved by calling aiio_httpd_ws_recv_frame before */
    if (frame->len == 0) {
        /* Assign the frame info from the previous reading */
        frame->type = aux->ws_type;
        frame->final = aux->ws_final;

        /* Grab the second byte */
        uint8_t second_byte = 0;
        if (aiio_httpd_recv_with_opt(req, (char *)&second_byte, sizeof(second_byte), false) <= 0) {
            aiio_log_w("Failed to receive the second byte");
            return AIIO_FAIL;
        }

        /* Parse the second byte */
        /* Please refer to RFC6455 Section 5.2 for more details */
        bool masked = (second_byte & HTTPD_WS_MASK_BIT) != 0;

        /* Interpret length */
        uint8_t init_len = second_byte & HTTPD_WS_LENGTH_BITS;
        if (init_len < 126) {
            /* Case 1: If length is 0-125, then this length bit is 7 bits */
            frame->len = init_len;
        } else if (init_len == 126) {
            /* Case 2: If length byte is 126, then this frame's length bit is 16 bits */
            uint8_t length_bytes[2] = { 0 };
            if (aiio_httpd_recv_with_opt(req, (char *)length_bytes, sizeof(length_bytes), false) <= 0) {
                aiio_log_w("Failed to receive 2 bytes length");
                return AIIO_FAIL;
            }

            frame->len = ((uint32_t)(length_bytes[0] << 8U) | (length_bytes[1]));
        } else if (init_len == 127) {
            /* Case 3: If length is byte 127, then this frame's length bit is 64 bits */
            uint8_t length_bytes[8] = { 0 };
            if (aiio_httpd_recv_with_opt(req, (char *)length_bytes, sizeof(length_bytes), false) <= 0) {
                aiio_log_w("Failed to receive 2 bytes length");
                return AIIO_FAIL;
            }

            frame->len = (((uint64_t)length_bytes[0] << 56U) |
                    ((uint64_t)length_bytes[1] << 48U) |
                    ((uint64_t)length_bytes[2] << 40U) |
                    ((uint64_t)length_bytes[3] << 32U) |
                    ((uint64_t)length_bytes[4] << 24U) |
                    ((uint64_t)length_bytes[5] << 16U) |
                    ((uint64_t)length_bytes[6] <<  8U) |
                    ((uint64_t)length_bytes[7]));
        }
        /* If this frame is masked, dump the mask as well */
        if (masked) {
            if (aiio_httpd_recv_with_opt(req, (char *)aux->mask_key, sizeof(aux->mask_key), false) <= 0) {
                aiio_log_w("Failed to receive mask key");
                return AIIO_FAIL;
            }
        } else {
            /* If the WS frame from client to server is not masked, it should be rejected.
             * Please refer to RFC6455 Section 5.2 for more details. */
            aiio_log_w("WS frame is not properly masked.");
            return AIIO_ERR_INVALID_STATE;
        }
    }
    /* We only accept the incoming packet length that is smaller than the max_len (or it will overflow the buffer!) */
    /* If max_len is 0, regard it OK for userspace to get frame len */
    if (frame->len > max_len) {
        if (max_len == 0) {
            aiio_log_d("regard max_len == 0 is OK for user to get frame len");
            return AIIO_OK;
        }
        aiio_log_w("WS Message too long");
        return AIIO_ERR_INVALID_SIZE;
    }

    /* Receive buffer */
    /* If there's nothing to receive, return and stop here. */
    if (frame->len == 0) {
        return AIIO_OK;
    }

    if (frame->payload == NULL) {
        aiio_log_w("Payload buffer is null");
        return AIIO_FAIL;
    }

    size_t left_len = frame->len;
    size_t offset = 0;

    while (left_len > 0) {
        int read_len = aiio_httpd_recv_with_opt(req, (char *)frame->payload + offset, left_len, false);
        if (read_len <= 0) {
            aiio_log_w("Failed to receive payload");
            return AIIO_FAIL;
        }
        offset += read_len;
        left_len -= read_len;

        aiio_log_d("Frame length: %d, Bytes Read: %d", frame->len, offset);
    }

    /* Unmask payload */
    httpd_ws_unmask_payload(frame->payload, frame->len, aux->mask_key);

    return AIIO_OK;
}

aiio_err_t aiio_httpd_ws_send_frame(aiio_httpd_req_t *req, aiio_httpd_ws_frame_t *frame)
{
    aiio_err_t ret = httpd_ws_check_req(req);
    if (ret != AIIO_OK) {
        return ret;
    }
    return aiio_httpd_ws_send_frame_async(req->handle, aiio_httpd_req_to_sockfd(req), frame);
}

aiio_err_t aiio_httpd_ws_send_frame_async(aiio_httpd_handle_t hd, int fd, aiio_httpd_ws_frame_t *frame)
{
    if (!frame) {
        aiio_log_w("Argument is invalid");
        return AIIO_ERR_INVALID_ARG;
    }

    /* Prepare Tx buffer - maximum length is 14, which includes 2 bytes header, 8 bytes length, 4 bytes mask key */
    uint8_t tx_len = 0;
    uint8_t header_buf[10] = {0 };
    /* Set the `FIN` bit by default if message is not fragmented. Else, set it as per the `final` field */
    header_buf[0] |= (!frame->fragmented) ? HTTPD_WS_FIN_BIT : (frame->final? HTTPD_WS_FIN_BIT: HTTPD_WS_CONTINUE);
    header_buf[0] |= frame->type; /* Type (opcode): 4 bits */

    if (frame->len <= 125) {
        header_buf[1] = frame->len & 0x7fU; /* Length for 7 bits */
        tx_len = 2;
    } else if (frame->len > 125 && frame->len < UINT16_MAX) {
        header_buf[1] = 126;                /* Length for 16 bits */
        header_buf[2] = (frame->len >> 8U) & 0xffU;
        header_buf[3] = frame->len & 0xffU;
        tx_len = 4;
    } else {
        header_buf[1] = 127;                /* Length for 64 bits */
        uint8_t shift_idx = sizeof(uint64_t) - 1; /* Shift index starts at 7 */
        uint64_t len64 = frame->len; /* Raise variable size to make sure we won't shift by more bits
                                      * than the length has (to avoid undefined behaviour) */
        for (int8_t idx = 2; idx <= 9; idx++) {
            /* Now do shifting (be careful of endianness, i.e. when buffer index is 2, frame length shift index is 7) */
            header_buf[idx] = (len64 >> (shift_idx * 8)) & 0xffU;
            shift_idx--;
        }
        tx_len = 10;
    }

    /* WebSocket server does not required to mask response payload, so leave the MASK bit as 0. */
    header_buf[1] &= (~HTTPD_WS_MASK_BIT);

    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, fd);
    if (!sess) {
        return AIIO_ERR_INVALID_ARG;
    }

    /* Send off header */
    if (sess->send_fn(hd, fd, (const char *)header_buf, tx_len, 0) < 0) {
        aiio_log_w("Failed to send WS header");
        return AIIO_FAIL;
    }

    /* Send off payload */
    if(frame->len > 0 && frame->payload != NULL) {
        if (sess->send_fn(hd, fd, (const char *)frame->payload, frame->len, 0) < 0) {
            aiio_log_w("Failed to send WS payload");
            return AIIO_FAIL;
        }
    }

    return AIIO_OK;
}

aiio_err_t aiio_httpd_ws_get_frame_type(aiio_httpd_req_t *req)
{
    aiio_err_t ret = httpd_ws_check_req(req);
    if (ret != AIIO_OK) {
        return ret;
    }

    struct aiio_httpd_req_aux *aux = req->aux;
    if (aux == NULL) {
        aiio_log_w("Invalid Aux pointer");
        return AIIO_ERR_INVALID_ARG;
    }

    /* Read the first byte from the frame to get the FIN flag and Opcode */
    /* Please refer to RFC6455 Section 5.2 for more details */
    uint8_t first_byte = 0;
    if (aiio_httpd_recv_with_opt(req, (char *)&first_byte, sizeof(first_byte), false) <= 0) {
        /* If the recv() return code is <= 0, then this socket FD is invalid (i.e. a broken connection) */
        /* Here we mark it as a Close message and close it later. */
        aiio_log_w("Failed to read header byte (socket FD invalid), closing socket now");
        aux->ws_final = true;
        aux->ws_type = HTTPD_WS_TYPE_CLOSE;
        return AIIO_OK;
    }

    aiio_log_d("First byte received: 0x%02X", first_byte);

    /* Decode the FIN flag and Opcode from the byte */
    aux->ws_final = (first_byte & HTTPD_WS_FIN_BIT) != 0;
    aux->ws_type = (first_byte & HTTPD_WS_OPCODE_BITS);

    /* Reply to PING. For PONG and CLOSE, it will be handled elsewhere. */
    if(aux->ws_type == HTTPD_WS_TYPE_PING) {
        aiio_log_d("Got a WS PING frame, Replying PONG...");

        /* Read the rest of the PING frame, for PONG to reply back. */
        /* Please refer to RFC6455 Section 5.5.2 for more details */
        aiio_httpd_ws_frame_t frame;
        uint8_t frame_buf[128] = { 0 };
        memset(&frame, 0, sizeof(aiio_httpd_ws_frame_t));
        frame.payload = frame_buf;

        if(aiio_httpd_ws_recv_frame(req, &frame, 126) != AIIO_OK) {
            aiio_log_d("Cannot receive the full PING frame");
            return AIIO_ERR_INVALID_STATE;
        }

        /* Now turn the frame to PONG */
        frame.type = HTTPD_WS_TYPE_PONG;
        return aiio_httpd_ws_send_frame(req, &frame);
    }

    return AIIO_OK;
}

aiio_httpd_ws_client_info_t aiio_httpd_ws_get_fd_info(aiio_httpd_handle_t hd, int fd)
{
    struct aiio_sock_db *sess = aiio_httpd_sess_get(hd, fd);

    if (sess == NULL) {
        return HTTPD_WS_CLIENT_INVALID;
    }
    bool is_active_ws = sess->ws_handshake_done && (!sess->ws_close);
    return is_active_ws ? HTTPD_WS_CLIENT_WEBSOCKET : HTTPD_WS_CLIENT_HTTP;
}

static void httpd_ws_send_cb(void *arg)
{
    async_transfer_t *trans = arg;

    aiio_err_t err = aiio_httpd_ws_send_frame_async(trans->handle, trans->socket, &trans->frame);

    if (trans->blocking) {
        xEventGroupSetBits(trans->transfer_done, err ? WS_SEND_FAILED : WS_SEND_OK);
    } else if (trans->callback) {
        trans->callback(err, trans->socket, trans->arg);
    }

    free(trans);
}

aiio_err_t aiio_httpd_ws_send_data(aiio_httpd_handle_t handle, int socket, aiio_httpd_ws_frame_t *frame)
{
    async_transfer_t *transfer = calloc(1, sizeof(async_transfer_t));
    if (transfer == NULL) {
        return AIIO_ERR_NO_MEM;
    }

    EventGroupHandle_t transfer_done = xEventGroupCreate();
    if (!transfer_done) {
        free(transfer);
        return AIIO_ERR_NO_MEM;
    }

    transfer->blocking = true;
    transfer->handle = handle;
    transfer->socket = socket;
    transfer->transfer_done = transfer_done;
    memcpy(&transfer->frame, frame, sizeof(aiio_httpd_ws_frame_t));

    aiio_err_t err = aiio_httpd_queue_work(handle, httpd_ws_send_cb, transfer);
    if (err != AIIO_OK) {
        vEventGroupDelete(transfer_done);
        free(transfer);
        return err;
    }

    EventBits_t status = xEventGroupWaitBits(transfer_done, WS_SEND_OK | WS_SEND_FAILED,
                                             pdTRUE, pdFALSE, portMAX_DELAY);

    vEventGroupDelete(transfer_done);

    return (status & WS_SEND_OK) ? AIIO_OK : AIIO_FAIL;
}

aiio_err_t aiio_httpd_ws_send_data_async(aiio_httpd_handle_t handle, int socket, aiio_httpd_ws_frame_t *frame,
                                   aiio_transfer_complete_cb callback, void *arg)
{
    async_transfer_t *transfer = calloc(1, sizeof(async_transfer_t));
    if (transfer == NULL) {
        return AIIO_ERR_NO_MEM;
    }

    transfer->arg = arg;
    transfer->callback = callback;
    transfer->handle = handle;
    transfer->socket = socket;
    memcpy(&transfer->frame, frame, sizeof(aiio_httpd_ws_frame_t));

    aiio_err_t err = aiio_httpd_queue_work(handle, httpd_ws_send_cb, transfer);

    if (err) {
        free(transfer);
        return err;
    }

    return AIIO_OK;
}

#endif /* CONFIG_HTTPD_WS_SUPPORT */
