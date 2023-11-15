
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <mbedtls/md5.h>
#include "aiio_check.h"
/* #include "aiio_netif.h" */
#include "lwip/sockets.h"
/* #include "aiio_rom_md5.h" */
#include "aiio_tls_crypto.h"

/* #include "aiio_system.h" */
#include "aiio_log.h"
#include "aiio_check.h"
#include "http_utils.h"
#include "http_auth.h"

#define MD5_MAX_LEN (33)
#define HTTP_AUTH_BUF_LEN (1024)

static const char *TAG = "HTTP_AUTH";

/**
 * @brief      This function hash a formatted string with MD5 and format the result as ascii characters
 *
 * @param      md         The buffer will hold the ascii result
 * @param[in]  fmt        The format
 *
 * @return     Length of the result
 */
static int md5_printf(char *md, const char *fmt, ...)
{
    unsigned char *buf;
    unsigned char digest[MD5_MAX_LEN];
    int len, i;
    va_list ap;
    va_start(ap, fmt);
    len = vasprintf((char **)&buf, fmt, ap);
    if (buf == NULL) {
        va_end(ap);
        return AIIO_FAIL;
    }

    if (mbedtls_md5_ret(buf, len, digest)) {
        va_end(ap);
        return AIIO_FAIL;
    }

    for (i = 0; i < 16; ++i) {
        sprintf(&md[i * 2], "%02x", (unsigned int)digest[i]);
    }
    va_end(ap);

    free(buf);
    return MD5_MAX_LEN;
}

char *http_auth_digest(const char *username, const char *password, aiio_http_auth_data_t *auth_data)
{
    char *ha1, *ha2 = NULL;
    char *digest = NULL;
    char *auth_str = NULL;
    char *temp_auth_str = NULL;
    int ret = AIIO_OK;

    if (username == NULL ||
        password == NULL ||
        auth_data->nonce == NULL ||
        auth_data->uri == NULL ||
        auth_data->realm == NULL) {
        return NULL;
    }

    ha1 = calloc(1, MD5_MAX_LEN);
    AIIO_GOTO_ON_FALSE(ha1, AIIO_FAIL, _digest_exit, TAG, "Memory exhausted");

    ha2 = calloc(1, MD5_MAX_LEN);
    AIIO_GOTO_ON_FALSE(ha2, AIIO_FAIL, _digest_exit, TAG, "Memory exhausted");

    digest = calloc(1, MD5_MAX_LEN);
    AIIO_GOTO_ON_FALSE(digest, AIIO_FAIL, _digest_exit, TAG, "Memory exhausted");

    if (md5_printf(ha1, "%s:%s:%s", username, auth_data->realm, password) <= 0) {
        goto _digest_exit;
    }

    aiio_log_d( "%s %s %s %s\r\n", "Digest", username, auth_data->realm, password);
    if (strcasecmp(auth_data->algorithm, "md5-sess") == 0) {
        if (md5_printf(ha1, "%s:%s:%016llx", ha1, auth_data->nonce, auth_data->cnonce) <= 0) {
            goto _digest_exit;
        }
    }
    if (md5_printf(ha2, "%s:%s", auth_data->method, auth_data->uri) <= 0) {
        goto _digest_exit;
    }

    //support qop = auth
    if (auth_data->qop && strcasecmp(auth_data->qop, "auth-int") == 0) {
        if (md5_printf(ha2, "%s:%s", ha2, "entity") <= 0) {
            goto _digest_exit;
        }
    }

    if (auth_data->qop) {
        // response=MD5(HA1:nonce:nonceCount:cnonce:qop:HA2)
        if (md5_printf(digest, "%s:%s:%08x:%016llx:%s:%s", ha1, auth_data->nonce, auth_data->nc, auth_data->cnonce, auth_data->qop, ha2) <= 0) {
            goto _digest_exit;
        }
    } else {
        // response=MD5(HA1:nonce:HA2)
        if (md5_printf(digest, "%s:%s:%s", ha1, auth_data->nonce, ha2) <= 0) {
            goto _digest_exit;
        }
    }
    asprintf(&auth_str, "Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", algorithm=\"MD5\", "
             "response=\"%s\", qop=%s, nc=%08x, cnonce=\"%016llx\"",
             username, auth_data->realm, auth_data->nonce, auth_data->uri, digest, auth_data->qop, auth_data->nc, auth_data->cnonce);
    if (auth_data->opaque) {
        asprintf(&temp_auth_str, "%s, opaque=\"%s\"", auth_str, auth_data->opaque);
        free(auth_str);
        auth_str = temp_auth_str;
    }
_digest_exit:
    free(ha1);
    free(ha2);
    free(digest);
    return (ret == AIIO_OK) ? auth_str : NULL;
}

char *http_auth_basic(const char *username, const char *password)
{
    int out;
    char *user_info = NULL;
    char *digest = NULL;
    int ret = AIIO_OK;
    size_t n = 0;
    asprintf(&user_info, "%s:%s", username, password);
    AIIO_RETURN_ON_FALSE(user_info, NULL, TAG, "Memory exhausted");
    aiio_crypto_base64_encode(NULL, 0, &n, (const unsigned char *)user_info, strlen(user_info));
    digest = calloc(1, 6 + n + 1);
    AIIO_GOTO_ON_FALSE(digest, AIIO_FAIL, _basic_exit, TAG, "Memory exhausted");
    strcpy(digest, "Basic ");
    aiio_crypto_base64_encode((unsigned char *)digest + 6, n, (size_t *)&out, (const unsigned char *)user_info, strlen(user_info));
_basic_exit:
    free(user_info);
    return (ret == AIIO_OK) ? digest : NULL;
}
