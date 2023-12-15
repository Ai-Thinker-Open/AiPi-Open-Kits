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
#include "timers.h"
#include "custom.h"
#include <sys/socket.h>
#include <lwip/api.h>
#include <lwip/arch.h>
#include <lwip/opt.h>
#include <lwip/inet.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/tcp.h>

#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/md5.h"
#include "mbedtls/debug.h"
#include "log.h"
#include "cJSON.h"
#include "https_client.h"
#include "device_ctrl.h"
// #define REQUEST_HTTPS
#define REQUEST_HTTP

extern TimerHandle_t http_timers;

int src_home_digital_date_mont_value = 12, \
src_home_digital_date_wday_value = 5, \
src_home_digital_date_day_value = 15, \
src_home_digital_date_yesr_value = 2023, \
src_home_digital_clock_1_hour_value = 16, \
src_home_digital_clock_1_min_value = 42, \
src_home_digital_clock_1_sec_value = 0;

#ifdef REQUEST_HTTPS

#define DBG_TAG "HTTPS"
#define WEB_PORT "443"

static const uint8_t* CERTIFICATE_FILENAME = { "-----BEGIN CERTIFICATE-----\r\n"
                                                    "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n"
                                                    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
                                                    "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n"
                                                    "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n"
                                                    "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n"
                                                    "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n"
                                                    "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n"
                                                    "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n"
                                                    "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n"
                                                    "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n"
                                                    "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n"
                                                    "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n"
                                                    "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n"
                                                    "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n"
                                                    "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n"
                                                    "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n"
                                                    "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n"
                                                    "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n"
                                                    "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n"
                                                    "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n"
                                                    "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n"
                                                    "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n"
                                                    "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n"
                                                    "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n"
                                                    "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n"
                                                    "-----END CERTIFICATE-----\r\n"
};

#elif defined REQUEST_HTTP

#define DBG_TAG "HTTP"
#define WEB_PORT "80"
int sock_client = -1;
static struct sockaddr_in dest;
#endif

static const char* REQUEST = "GET " "/%s" " HTTP/1.0\r\n"
"Host: " "%s" ":" WEB_PORT "\r\n"
"User-Agent: AiPi-DSL_Dashboard\r\n"
"\r\n";

void* https_Handle;
// extern xQueueHandle queue;
/**
 * @brief https get request
 *
 * @param host
 * @param https_url https path /free/week?unescape=1&appid=17769781&appsecret=5IbudTJx
 * @return char*
*/
char* https_get_request(const char* host, const char* https_url)
{
    static char* buff;
    char* https_request_handle = pvPortMalloc(512);
    int ret, flags, len;
    buff = pvPortMalloc(1024);
    memset(buff, 0, 1024);

#ifdef REQUEST_HTTPS

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_x509_crt cacert;
    mbedtls_ssl_config conf;
    mbedtls_net_context server_fd;

    mbedtls_ssl_init(&ssl);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    LOG_I("Seeding the random number generator");

    mbedtls_ssl_config_init(&conf);

    mbedtls_entropy_init(&entropy);
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        NULL, 0)) != 0)
    {
        LOG_I("mbedtls_ctr_drbg_seed returned %d", ret);
        abort();
    }

    LOG_I("Loading the CA root certificate...");

    ret = mbedtls_x509_crt_parse(&cacert,
                                 CERTIFICATE_FILENAME,
                                 strlen((char*)CERTIFICATE_FILENAME) + 1);

    if (ret < 0)
    {
        LOG_E("mbedtls_x509_crt_parse returned -0x%x\n\n", -ret);
        abort();
    }

    LOG_I("Setting hostname for TLS session...");

    /* Hostname set here should match CN in server certificate */
    if ((ret = mbedtls_ssl_set_hostname(&ssl, host)) != 0)
    {
        LOG_E("mbedtls_ssl_set_hostname returned -0x%x", -ret);
        abort();
    }

    LOG_I("Setting up the SSL/TLS structure...");

    if ((ret = mbedtls_ssl_config_defaults(&conf,
        MBEDTLS_SSL_IS_CLIENT,
        MBEDTLS_SSL_TRANSPORT_STREAM,
        MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        LOG_E("mbedtls_ssl_config_defaults returned %d", ret);
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
        LOG_E("mbedtls_ssl_setup returned -0x%x", -ret);
        goto exit;
    }

    mbedtls_net_init(&server_fd);

    LOG_I("Connecting to %s:%s...", host, WEB_PORT);
    LOG_I("");
    if ((ret = mbedtls_net_connect(&server_fd, host,
        WEB_PORT, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        LOG_E("mbedtls_net_connect returned -%x", -ret);
        goto exit;
    }

    LOG_I("Connected.");

    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    LOG_I("Performing the SSL/TLS handshake...");

    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            LOG_E("mbedtls_ssl_handshake returned -0x%x", -ret);
            goto exit;
        }
    }

    LOG_I("Verifying peer X.509 certificate...");

    if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0)
    {
        /* In real life, we probably want to close connection if ret != 0 */
        LOG_E("Failed to verify peer certificate!");
        bzero(buff, sizeof(buff));
        mbedtls_x509_crt_verify_info(buff, sizeof(buff), "  ! ", flags);
        LOG_E("verification info: %s", buff);

    }
    else
    {
        LOG_I("Certificate verified.");
    }

    LOG_I("Cipher suite is %s", mbedtls_ssl_get_ciphersuite(&ssl));

    LOG_I("Writing HTTP request:");
    sprintf(https_request_handle, REQUEST, https_url, host);
    LOG_I("%s", https_request_handle);
    size_t written_bytes = 0;
    do
    {

        ret = mbedtls_ssl_write(&ssl,
                                (const unsigned char*)https_request_handle + written_bytes,
                                strlen(https_request_handle) - written_bytes);
        if (ret >= 0)
        {
            LOG_I("%d bytes written", ret);
            written_bytes += ret;
        }
        else if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ)
        {
            LOG_E("mbedtls_ssl_write returned -0x%x", -ret);
            goto exit;
        }
    } while (written_bytes < strlen(REQUEST));

    LOG_I("Reading HTTP response...");
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
            LOG_E("mbedtls_ssl_read returned -0x%x", -ret);
            break;
        }

        if (ret == 0)
        {
            LOG_I("connection closed");
            break;
        }
        len += ret;
        /* Print response directly to stdout as it is read */
        // for (int i = 0; i < len; i++)
        // {
        //     LOG_I("%c", buff[i]);
        // }
        // strcat(buff, buff_s);
        strncat(buff, buff_s, ret);
        vPortFree(buff_s);
    } while (1);

    mbedtls_ssl_close_notify(&ssl);
    LOG_I("https read len :%d", len);
exit:
    mbedtls_ssl_session_reset(&ssl);
    mbedtls_net_free(&server_fd);

    if (ret != 0)
    {
        mbedtls_strerror(ret, buff, 100);
        LOG_E("Last error was: -0x%x - %s", -ret, buff);
        return NULL;
    }

    LOG_I(""); // JSON output doesn't have a newline at end

    static int request_count;
    LOG_I("Completed %d requests", ++request_count);

#elif defined  REQUEST_HTTP
    struct in_addr addr;

#ifdef LWIP_DNS
    netconn_gethostbyname(host, &addr);
    LOG_I("Host:%s, Server ip Address : %s:%s", host, ip_ntoa(&addr), WEB_PORT);
#endif
    //Crate tcp socket
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (ret<0) {
        LOG_E("Failed to allocate socket.");
        goto __exit;
    }
    LOG_I("allocated socket");

    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(WEB_PORT));
    dest.sin_addr = addr;
    //connect http server
    ret = connect(sock_client, (struct sockaddr*)&dest, sizeof(dest));
    if (ret!=0) {
        LOG_E("... socket connect failed errno=%d", errno);
        close(sock_client);
        goto __exit;
    }
    LOG_I("HTTP client connect server success!");

    //send request
    memset(https_request_handle, 0, 256);
    sprintf(https_request_handle, REQUEST, https_url, host);
    ret = write(sock_client, https_request_handle, strlen(https_request_handle));
    if (ret< 0) {
        LOG_E("HTTP send Handler failed error=%d", ret);
        close(sock_client);
        goto __exit;
    }
    LOG_I("request send OK", ret);
    // LOG_F("Handler byte=%d\r\n%s", ret, https_request_handle);

    memset(buff, 0, 1024);
    flags = read(sock_client, buff, 1024);
    // LOG_F("\r\n%s", buff);

    shutdown(sock_client, SHUT_RDWR);
    close(sock_client);

#endif
__exit:

    vPortFree(https_request_handle);
    return buff;
}
/**
 * @brief Get the https date object
 *  Date: Sun, 25 Jun 2023 02:37:04 GMT
 * @param date
*/
#if 1
static void get_https_date(char* date)
{
    if (date==NULL) {
        LOG_E("DATE data is NULL ");
        return;
    }

    char* date_data = date + 6;
    char* date_wday = strtok(date_data, ",");
    if (strcmp(date_wday, "Sun")==0)src_home_digital_date_wday_value = 0;
    if (strcmp(date_wday, "Mon")==0)src_home_digital_date_wday_value = 1;
    if (strcmp(date_wday, "Tue")==0)src_home_digital_date_wday_value = 2;
    if (strcmp(date_wday, "Wed")==0)src_home_digital_date_wday_value = 3;
    if (strcmp(date_wday, "Thu")==0)src_home_digital_date_wday_value = 4;
    if (strcmp(date_wday, "Fri")==0)src_home_digital_date_wday_value = 5;
    if (strcmp(date_wday, "Sat")==0)src_home_digital_date_wday_value = 6;
    date_data += 5;

    char* date_day = strtok(date_data, " ");
    src_home_digital_date_day_value = atoi(date_day);
    int i = 0;
    while (date_day!=NULL) {
        date_day = strtok(NULL, " ");
        if (i==0) {
            if (strcmp(date_day, "Jan")==0)src_home_digital_date_mont_value = 1;
            if (strcmp(date_day, "Feb")==0)src_home_digital_date_mont_value = 2;
            if (strcmp(date_day, "Mar")==0)src_home_digital_date_mont_value = 3;
            if (strcmp(date_day, "Apr")==0)src_home_digital_date_mont_value = 4;
            if (strcmp(date_day, "May")==0)src_home_digital_date_mont_value = 5;
            if (strcmp(date_day, "Jun")==0)src_home_digital_date_mont_value = 6;
            if (strcmp(date_day, "Jul")==0)src_home_digital_date_mont_value = 7;
            if (strcmp(date_day, "Aug")==0)src_home_digital_date_mont_value = 8;
            if (strcmp(date_day, "Sept")==0)src_home_digital_date_mont_value = 9;
            if (strcmp(date_day, "Oct")==0)src_home_digital_date_mont_value = 10;
            if (strcmp(date_day, "Nov")==0)src_home_digital_date_mont_value = 11;
            if (strcmp(date_day, "Dec")==0)src_home_digital_date_mont_value = 12;
        }
        if (i==1) src_home_digital_date_yesr_value = atoi(date_day);

        if (i==2) {
            char* date_s = strtok(date_day, ":");
            src_home_digital_clock_1_hour_value = atoi(date_s)+8;
            for (size_t j = 0; j < 2; j++)
            {
                date_s = strtok(NULL, ":");
                if (j==0) src_home_digital_clock_1_min_value = atoi(date_s);
                else src_home_digital_clock_1_sec_value = atoi(date_s);
            }
        }
        i++;
    }
}
# endif
/**
 * @brief HTTP解析请求数据
 *
 * @param https_request_data
 * @return char*
*/
static char* https_get_data(const char* https_request_data)
{
    if (https_get_data==NULL) return NULL;

    char* request_data = https_request_data;
    static char* https_data;
    https_data = pvPortMalloc(1024*2);
    memset(https_data, 0, 1024*2);
    request_data += 2;
    char* date = pvPortMalloc(64);
    char* request_value = strtok(request_data, "\n");
    for (size_t i = 0; i < 9; i++)
    {
        // LOG_I("%s", request_value);
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

static void https_get_weather_data(char* json)
{
    if (json==NULL) {
        LOG_E("json is NULL");
        return;
    }

    printf("%s\r\n", json);

    // printf("\r\n");
    cJSON* root = cJSON_Parse(json);

    if (root==NULL) {
        LOG_E("data is no json");
        return;
    }

    cJSON* cjson_city = cJSON_GetObjectItem(root, "city");
    cJSON* cjson_wea = cJSON_GetObjectItem(root, "wea");
    cJSON* cjson_tem_day = cJSON_GetObjectItem(root, "tem_day");
    cJSON* cjson_tem_night = cJSON_GetObjectItem(root, "tem_night");
    cJSON* cjson_win = cJSON_GetObjectItem(root, "win");
    cJSON* cjson_air = cJSON_GetObjectItem(root, "air");
    cJSON* cjson_pressure = cJSON_GetObjectItem(root, "pressure");
    cJSON* cjson_humidity = cJSON_GetObjectItem(root, "humidity");

    strcpy(city, cjson_city->valuestring);
    strcpy(wea, cjson_wea->valuestring);
    strcpy(tem_day, cjson_tem_day->valuestring);
    strcpy(tem_tem_night, cjson_tem_night->valuestring);
    strcpy(win, cjson_win->valuestring);
    strcpy(air, cjson_air->valuestring);
    strcpy(pressure, cjson_pressure->valuestring);
    strcpy(humidity, cjson_humidity->valuestring);

    cJSON_Delete(root);
}

void https_get_weather_task(void* arg)
{
    // char* queue_buff = NULL;
    // queue_buff = pvPortMalloc(1024*2);
    // memset(queue_buff, 0, 1024*2);
    //
    // char* buff = NULL;
    // = https_get_data(https_get_request(HTTP_HOST, HTTP_PATH));
    // sprintf(queue_buff, "{\"weather\":%s}", buff);
    // xQueueSend(queue, queue_buff, portMAX_DELAY);

    // vPortFree(buff);
// vPortFree(queue_buff);
// xTimerStart(http_timers, portMAX_DELAY);
// LOG_I("Time start 1 hour times ....");
    vTaskSuspend(https_Handle);

    while (1) {

        // queue_buff = pvPortMalloc(1024*2);
        //请求一次错误的响应，只获取时间
        char* buff = https_get_data(https_get_request(HTTP_HOST, HTTP_PATH));
        // memset(queue_buff, 0, 1024*2);
        // sprintf(queue_buff, "{\"weather\":%s}", buff);
        // xQueueSend(queue, queue_buff, portMAX_DELAY);
        // LOG_I("HTTP:%s", buff);
        https_get_weather_data(buff);
        device_send_state_notify(DEVICE_STATE_HTTP_GET_WEATHER, 0);
        vPortFree(buff);
        // vPortFree(queue_buff);
        vTaskSuspend(https_Handle);
        vTaskDelay(50/portTICK_RATE_MS);
    }
}