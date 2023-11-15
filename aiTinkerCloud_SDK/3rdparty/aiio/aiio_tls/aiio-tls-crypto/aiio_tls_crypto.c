
#include "aiio_tls_config.h"

#include "aiio_tls_crypto.h"
#include "aiio_log.h"
#include "aiio_err.h"
static const char *TAG = "aiio_crypto";
#ifdef CONFIG_TLS_USING_MBEDTLS
#include "mbedtls/sha1.h"
#include "mbedtls/base64.h"
#define _crypto_sha1 aiio_crypto_sha1_mbedtls
#define _crypto_base64_encode aiio_crypto_bas64_encode_mbedtls
#elif  CONFIG_TLS_USING_WOLFSSL
#include "wolfssl/ssl.h" /* SHA functions are listed in wolfssl/ssl.h */
#include "wolfssl/wolfcrypt/coding.h"
#define _crypto_sha1 aiio_crypto_sha1_wolfSSL
#define _crypto_base64_encode aiio_crypto_base64_encode_woflSSL
#endif

#ifdef CONFIG_TLS_USING_MBEDTLS
static int aiio_crypto_sha1_mbedtls( const unsigned char *input,
                                    size_t ilen,
                                    unsigned char output[20])
{
    int ret = mbedtls_sha1_ret(input, ilen, output);
    if (ret != 0) {
        aiio_log_e( "Error in calculating sha1 sum , Returned 0x%02X", ret);
    }
    return ret;
}

static int aiio_crypto_bas64_encode_mbedtls( unsigned char *dst, size_t dlen,
        size_t *olen, const unsigned char *src,
        size_t slen)
{
    return mbedtls_base64_encode(dst, dlen, olen, src, slen);
}

#elif CONFIG_TLS_USING_WOLFSSL
static int aiio_crypto_sha1_wolfSSL( const unsigned char *input,
                                    size_t ilen,
                                    unsigned char output[20])
{
    unsigned char *ret = wolfSSL_SHA1(input, ilen, output);
    if (ret == NULL) {
        aiio_log_e( "Error in calculating sha1 sum");
        return -1;
    }
    return 0;
}

static int aiio_crypto_base64_encode_woflSSL(unsigned char *dst, size_t dlen, size_t *olen,
        const unsigned char *src, size_t slen)
{
    *olen = dlen;
    return Base64_Encode_NoNl((const byte *) src, (word32) slen, (byte *) dst, (word32 *) olen);
}

#else
#error "No TLS/SSL Stack selected"
#endif

int aiio_crypto_sha1( const unsigned char *input,
                     size_t ilen,
                     unsigned char output[20])
{
    return _crypto_sha1(input, ilen, output);
}

int aiio_crypto_base64_encode(unsigned char *dst, size_t dlen, size_t *olen,
                             const unsigned char *src, size_t slen )
{
    return _crypto_base64_encode(dst, dlen, olen, src, slen);
}
