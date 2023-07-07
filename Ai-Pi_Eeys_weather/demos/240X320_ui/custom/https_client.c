/**
 * @file https_client.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-21
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "custom.h"
#include <sys/socket.h>
#include <lwip/api.h>
#include <lwip/arch.h>
#include <lwip/opt.h>
#include <lwip/inet.h>
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/md5.h"
#include "mbedtls/debug.h"

#define WEB_PORT "443"

extern TaskHandle_t https_Handle;
extern xQueueHandle queue;
static const char* REQUEST = "GET " "%s" " HTTP/1.0\r\n"
"Host: " "%s" "\r\n"
"User-Agent: Eyes \r\n"
"\r\n";

static const uint8_t CERTIFICATE_FILENAME[] = { "-----BEGIN CERTIFICATE-----\r\n"
                                                    "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\r\n"
                                                    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\r\n"
                                                    "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\r\n"
                                                    "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\r\n"
                                                    "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\r\n"
                                                    "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\r\n"
                                                    "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\r\n"
                                                    "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\r\n"
                                                    "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\r\n"
                                                    "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\r\n"
                                                    "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\r\n"
                                                    "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\r\n"
                                                    "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\r\n"
                                                    "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\r\n"
                                                    "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\r\n"
                                                    "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\r\n"
                                                    "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\r\n"
                                                    "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\r\n"
                                                    "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\r\n"
                                                    "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\r\n"
                                                    "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\r\n"
                                                    "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\r\n"
                                                    "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\r\n"
                                                    "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\r\n"
                                                    "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\r\n"
                                                    "-----END CERTIFICATE-----\r\n" };
/**
 * @brief https get request
 *
 * @param host
 * @param https_url https
 * @return char*
*/
char* https_get_request(const char* host, const char* https_url)
{
    static char* buff;
    char* https_request_handle = pvPortMalloc(512);
    int ret, flags, len;
    buff = pvPortMalloc(2*1024);
    memset(buff, 0, 2*1024);
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_x509_crt cacert;
    mbedtls_ssl_config conf;
    mbedtls_net_context server_fd;

    mbedtls_ssl_init(&ssl);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    printf("Seeding the random number generator\r\n");

    mbedtls_ssl_config_init(&conf);

    mbedtls_entropy_init(&entropy);
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        NULL, 0)) != 0)
    {
        printf("mbedtls_ctr_drbg_seed returned %d\r\n", ret);
        abort();
    }

    printf("Loading the CA root certificate...\r\n");

    ret = mbedtls_x509_crt_parse(&cacert,
                                 CERTIFICATE_FILENAME,
                                 strlen((char*)CERTIFICATE_FILENAME) + 1);

    if (ret < 0)
    {
        printf("mbedtls_x509_crt_parse returned -0x%x\n\n", -ret);
        abort();
    }

    printf("Setting hostname for TLS session...\r\n");

    /* Hostname set here should match CN in server certificate */
    if ((ret = mbedtls_ssl_set_hostname(&ssl, host)) != 0)
    {
        printf("mbedtls_ssl_set_hostname returned -0x%x\r\n", -ret);
        abort();
    }

    printf("Setting up the SSL/TLS structure...\r\n");

    if ((ret = mbedtls_ssl_config_defaults(&conf,
        MBEDTLS_SSL_IS_CLIENT,
        MBEDTLS_SSL_TRANSPORT_STREAM,
        MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        printf("mbedtls_ssl_config_defaults returned %d\r\n", ret);
        goto exit;
    }

    /* MBEDTLS_SSL_VERIFY_OPTIONAL is bad for security, in this example it will print
       a warning if CA verification fails but it will continue to connect.

       You should consider using MBEDTLS_SSL_VERIFY_REQUIRED in your own code.
    */
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
    {
        printf("mbedtls_ssl_setup returned -0x%x\r\n", -ret);
        goto exit;
    }

    mbedtls_net_init(&server_fd);

    printf("Connecting to %s:%s...\r\n", host, WEB_PORT);
    printf("");
    if ((ret = mbedtls_net_connect(&server_fd, host,
        WEB_PORT, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        printf("mbedtls_net_connect returned -%x\r\n", -ret);
        goto exit;
    }

    printf("Connected.\r\n");

    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    printf("Performing the SSL/TLS handshake...\r\n");

    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            printf("mbedtls_ssl_handshake returned -0x%x\r\n", -ret);
            goto exit;
        }
    }

    printf("Verifying peer X.509 certificate...\r\n");

    if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0)
    {
        /* In real life, we probably want to close connection if ret != 0 */
        printf("Failed to verify peer certificate!\r\n");
        bzero(buff, sizeof(buff));
        mbedtls_x509_crt_verify_info(buff, sizeof(buff), "  ! ", flags);
        printf("verification info: %s\r\n", buff);

    }
    else
    {
        printf("Certificate verified.\r\n");
    }

    printf("Cipher suite is %s", mbedtls_ssl_get_ciphersuite(&ssl));

    printf("Writing HTTP request:\r\n");
    sprintf(https_request_handle, REQUEST, https_url, host);
    printf("%s\r\n", https_request_handle);
    size_t written_bytes = 0;
    do
    {

        ret = mbedtls_ssl_write(&ssl,
                                (const unsigned char*)https_request_handle + written_bytes,
                                strlen(https_request_handle) - written_bytes);
        if (ret >= 0)
        {
            printf("%d bytes written\r\n", ret);
            written_bytes += ret;
        }
        else if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ)
        {
            printf("mbedtls_ssl_write returned -0x%x\r\n", -ret);
            goto exit;
        }
    } while (written_bytes < strlen(REQUEST));

    printf("Reading HTTP response...\r\n");
    len = 0;
    do
    {
        char* buff_s = pvPortMalloc(32);
        bzero(buff_s, sizeof(buff_s));
        ret = mbedtls_ssl_read(&ssl, (unsigned char*)buff_s, 32);

        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
            continue;

        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
        {
            ret = 0;
            break;
        }

        if (ret < 0)
        {
            printf("mbedtls_ssl_read returned -0x%x\r\n", -ret);
            break;
        }

        if (ret == 0)
        {
            printf("connection closed\r\n");
            break;
        }
        len += ret;
        /* Print response directly to stdout as it is read */
        // for (int i = 0; i < len; i++)
        // {
        //     printf("%c", buff[i]);
        // }
        // strcat(buff, buff_s);
        strncat(buff, buff_s, ret);
        vPortFree(buff_s);
    } while (1);

    mbedtls_ssl_close_notify(&ssl);
    printf("https read len :%d\n\r", len);
exit:
    mbedtls_ssl_session_reset(&ssl);
    mbedtls_net_free(&server_fd);

    if (ret != 0)
    {
        mbedtls_strerror(ret, buff, 100);
        printf("Last error was: -0x%x - %s\r\n", -ret, buff);
        return NULL;
    }

    printf("\r\n"); // JSON output doesn't have a newline at end

    static int request_count;
    printf("Completed %d requests\r\n", ++request_count);
    vPortFree(https_request_handle);
    return buff;
}
/**
 * @brief Get the https date object
 *  Date: Sun, 25 Jun 2023 02:37:04 GMT
 * @param date
*/
static void get_https_date(char* date)
{
    if (date==NULL) {
        printf("DATE data is NULL \r\n");
        return;
    }

    char* date_data = date + 6;
    char* date_wday = strtok(date_data, ",");
    if (strcmp(date_wday, "Sun")==0)cont_4_lable_1_wday = 0;
    if (strcmp(date_wday, "Mon")==0)cont_4_lable_1_wday = 1;
    if (strcmp(date_wday, "Tue")==0)cont_4_lable_1_wday = 2;
    if (strcmp(date_wday, "Wed")==0)cont_4_lable_1_wday = 3;
    if (strcmp(date_wday, "Thu")==0)cont_4_lable_1_wday = 4;
    if (strcmp(date_wday, "Fri")==0)cont_4_lable_1_wday = 5;
    if (strcmp(date_wday, "Sat")==0)cont_4_lable_1_wday = 6;
    date_data += 5;

    char* date_day = strtok(date_data, " ");
    cont_4_lable_1_day = atoi(date_day);
    int i = 0;
    while (date_day!=NULL) {
        date_day = strtok(NULL, " ");
        if (i==0) {
            if (strcmp(date_day, "Jan")==0)cont_4_lable_1_month = 1;
            if (strcmp(date_day, "Feb")==0)cont_4_lable_1_month = 2;
            if (strcmp(date_day, "Mar")==0)cont_4_lable_1_month = 3;
            if (strcmp(date_day, "Apr")==0)cont_4_lable_1_month = 4;
            if (strcmp(date_day, "May")==0)cont_4_lable_1_month = 5;
            if (strcmp(date_day, "Jun")==0)cont_4_lable_1_month = 6;
            if (strcmp(date_day, "Jul")==0)cont_4_lable_1_month = 7;
            if (strcmp(date_day, "Aug")==0)cont_4_lable_1_month = 8;
            if (strcmp(date_day, "Sept")==0)cont_4_lable_1_month = 9;
            if (strcmp(date_day, "Oct")==0)cont_4_lable_1_month = 10;
            if (strcmp(date_day, "Nov")==0)cont_4_lable_1_month = 11;
            if (strcmp(date_day, "Dec")==0)cont_4_lable_1_month = 12;
        }
        if (i==1) cont_4_lable_1_yers = atoi(date_day);

        if (i==2) {
            char* date_s = strtok(date_day, ":");
            cont_4_digital_clock_1_hour_value = atoi(date_s)+8;
            for (size_t j = 0; j < 2; j++)
            {
                date_s = strtok(NULL, ":");
                if (j==0) cont_4_digital_clock_1_min_value = atoi(date_s);
                else cont_4_digital_clock_1_sec_value = atoi(date_s);
            }
        }
        i++;
    }
}

/**
 * @brief HTTP解析请求数据
 *
 * @param https_request_data
 * @return char*
*/
static char* https_get_data(const char* https_request_data)
{
    char* request_data = https_request_data;
    static char* https_data;
    https_data = pvPortMalloc(1024*2);
    memset(https_data, 0, 1024*2);
    request_data += 2;
    char* date = pvPortMalloc(64);
    char* request_value = strtok(request_data, "\n");
    for (size_t i = 0; i < 9; i++)
    {
        printf("%s\r\n", request_value);
        if (i==2) strcpy(date, request_value);
        memset(request_value, 0, strlen(request_value));
        request_value = strtok(NULL, "\n");
    }
    get_https_date(date);
    strcpy(https_data, request_value);
    vPortFree(https_request_data);
    vPortFree(date);
    return https_data;
}

void https_get_weather_task(void* arg)
{
    char* queue_buff = pvPortMalloc(1024*2);
    memset(queue_buff, 0, 1024*2);
    char* buff = https_get_data(https_get_request("v0.yiketianqi.com", "https://v0.yiketianqi.com/free/week?unescape=1&appid=17769781&appsecret=5IbudTJx"));
    sprintf(queue_buff, "{\"weather\":%s}", buff);
    xQueueSend(queue, queue_buff, portMAX_DELAY);
    vPortFree(buff);
    vTaskSuspend(https_Handle);
    while (1) {
        char* buff = https_get_data(https_get_request("v0.yiketianqi.com", "https://v0.yiketianqi.com/free/week?unescape=1&appid=17769781&appsecret=5IbudTJx"));
        memset(queue_buff, 0, 1024*2);
        sprintf(queue_buff, "{\"weather\":%s}", buff);

        xQueueSend(queue, queue_buff, portMAX_DELAY);
        vPortFree(buff);
        vTaskSuspend(https_Handle);
        vTaskDelay(50/portTICK_RATE_MS);
    }
}