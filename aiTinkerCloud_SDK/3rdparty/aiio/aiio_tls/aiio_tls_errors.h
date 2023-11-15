

#ifndef __AIIO_TLS_ERRORS_H__
#define __AIIO_TLS_ERRORS_H__

#include "aiio_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AIIO_ERR_TLS_BASE           0x8000             /*!< Starting number of AIIO-TLS error codes */

/* generic aiio-tls error codes */
#define AIIO_ERR_TLS_CANNOT_RESOLVE_HOSTNAME           (AIIO_ERR_TLS_BASE + 0x01)  /*!< Error if hostname couldn't be resolved upon tls connection */
#define AIIO_ERR_TLS_CANNOT_CREATE_SOCKET              (AIIO_ERR_TLS_BASE + 0x02)  /*!< Failed to create socket */
#define AIIO_ERR_TLS_UNSUPPORTED_PROTOCOL_FAMILY       (AIIO_ERR_TLS_BASE + 0x03)  /*!< Unsupported protocol family */
#define AIIO_ERR_TLS_FAILED_CONNECT_TO_HOST            (AIIO_ERR_TLS_BASE + 0x04)  /*!< Failed to connect to host */
#define AIIO_ERR_TLS_SOCKET_SETOPT_FAILED              (AIIO_ERR_TLS_BASE + 0x05)  /*!< failed to set/get socket option */
#define AIIO_ERR_TLS_CONNECTION_TIMEOUT                (AIIO_ERR_TLS_BASE + 0x06)  /*!< new connection in aiio_tls_low_level_conn connection timeouted */
#define AIIO_ERR_TLS_SE_FAILED                         (AIIO_ERR_TLS_BASE + 0x07)  /*< aiio-tls use Secure Element returned failed */
#define AIIO_ERR_TLS_TCP_CLOSED_FIN                    (AIIO_ERR_TLS_BASE + 0x08)  /*< aiio-tls's TPC transport connection has benn closed (in a clean way) */

/* mbedtls specific error codes */
#define AIIO_ERR_MBEDTLS_CERT_PARTLY_OK                    (AIIO_ERR_TLS_BASE + 0x10)  /*!< mbedtls parse certificates was partly successful */
#define AIIO_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED              (AIIO_ERR_TLS_BASE + 0x11)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED           (AIIO_ERR_TLS_BASE + 0x12)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED        (AIIO_ERR_TLS_BASE + 0x13)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED    (AIIO_ERR_TLS_BASE + 0x14)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_X509_CRT_PARSE_FAILED             (AIIO_ERR_TLS_BASE + 0x15)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED          (AIIO_ERR_TLS_BASE + 0x16)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_SETUP_FAILED                  (AIIO_ERR_TLS_BASE + 0x17)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_SSL_WRITE_FAILED                  (AIIO_ERR_TLS_BASE + 0x18)  /*!< mbedtls api returned error */
#define AIIO_ERR_MBEDTLS_PK_PARSE_KEY_FAILED               (AIIO_ERR_TLS_BASE + 0x19)  /*!< mbedtls api returned failed  */
#define AIIO_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED              (AIIO_ERR_TLS_BASE + 0x1A)  /*!< mbedtls api returned failed  */
#define AIIO_ERR_MBEDTLS_SSL_CONF_PSK_FAILED               (AIIO_ERR_TLS_BASE + 0x1B)  /*!< mbedtls api returned failed  */
#define AIIO_ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED           (AIIO_ERR_TLS_BASE + 0x1C)  /*!< mbedtls api returned failed  */

/* wolfssl specific error codes */
#define AIIO_ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED           (AIIO_ERR_TLS_BASE + 0x31)  /*!< wolfSSL api returned error */
#define AIIO_ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED    (AIIO_ERR_TLS_BASE + 0x32)  /*!< wolfSSL api returned error */
#define AIIO_ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED          (AIIO_ERR_TLS_BASE + 0x33)  /*!< wolfSSL api returned error */
#define AIIO_ERR_WOLFSSL_KEY_VERIFY_SETUP_FAILED           (AIIO_ERR_TLS_BASE + 0x34)  /*!< wolfSSL api returned error */
#define AIIO_ERR_WOLFSSL_SSL_HANDSHAKE_FAILED              (AIIO_ERR_TLS_BASE + 0x35)  /*!< wolfSSL api returned failed  */
#define AIIO_ERR_WOLFSSL_CTX_SETUP_FAILED                  (AIIO_ERR_TLS_BASE + 0x36)  /*!< wolfSSL api returned failed */
#define AIIO_ERR_WOLFSSL_SSL_SETUP_FAILED                  (AIIO_ERR_TLS_BASE + 0x37)  /*!< wolfSSL api returned failed */
#define AIIO_ERR_WOLFSSL_SSL_WRITE_FAILED                  (AIIO_ERR_TLS_BASE + 0x38)  /*!< wolfSSL api returned failed */


/**
* Definition of errors reported from IO API (potentially non-blocking) in case of error:
* - aiio_tls_conn_read()
* - aiio_tls_conn_write()
*/
#ifdef CONFIG_TLS_USING_MBEDTLS
#define AIIO_TLS_ERR_SSL_WANT_READ                          MBEDTLS_ERR_SSL_WANT_READ
#define AIIO_TLS_ERR_SSL_WANT_WRITE                         MBEDTLS_ERR_SSL_WANT_WRITE
#define AIIO_TLS_ERR_SSL_TIMEOUT                            MBEDTLS_ERR_SSL_TIMEOUT
#elif CONFIG_TLS_USING_WOLFSSL /* CONFIG_TLS_USING_MBEDTLS */
#define AIIO_TLS_ERR_SSL_WANT_READ                          -0x6900
#define AIIO_TLS_ERR_SSL_WANT_WRITE                         -0x6880
#define AIIO_TLS_ERR_SSL_TIMEOUT                            WOLFSSL_CBIO_ERR_TIMEOUT
#endif /*CONFIG_TLS_USING_WOLFSSL */

/**
* Definition of different types/sources of error codes reported
* from different components
*/
typedef enum {
    AIIO_TLS_ERR_TYPE_UNKNOWN = 0,
    AIIO_TLS_ERR_TYPE_SYSTEM,                /*!< System error -- errno */
    AIIO_TLS_ERR_TYPE_MBEDTLS,               /*!< Error code from mbedTLS library */
    AIIO_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS,    /*!< Certificate flags defined in mbedTLS */
    AIIO_TLS_ERR_TYPE_AIIO,                   /*!< AIIO-IDF error type -- int  */
    AIIO_TLS_ERR_TYPE_WOLFSSL,               /*!< Error code from wolfSSL library */
    AIIO_TLS_ERR_TYPE_WOLFSSL_CERT_FLAGS,    /*!< Certificate flags defined in wolfSSL */
    AIIO_TLS_ERR_TYPE_MAX,                   /*!< Last err type -- invalid entry */
} aiio_tls_error_type_t;

typedef struct aiio_tls_last_error* aiio_tls_error_handle_t;

/**
*  @brief Error structure containing relevant errors in case tls error occurred
*/
typedef struct aiio_tls_last_error {
    int last_error;               /*!< error code (based on AIIO_ERR_TLS_BASE) of the last occurred error */
    int       aiio_tls_error_code;       /*!< aiio_tls error code from last aiio_tls failed api */
    int       aiio_tls_flags;            /*!< last certification verification flags */
} aiio_tls_last_error_t;

#ifdef __cplusplus
}
#endif

#endif //__AIIO_TLS_ERRORS_H__
