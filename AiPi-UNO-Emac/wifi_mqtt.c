#include "FreeRTOS_POSIX.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <lwip/errno.h>
#include <netdb.h>

#include "utils_getopt.h"

#include "mqtt.h"
#include "shell.h"

#include "bflb_rtc.h"

#include "easyflash.h"
#include "mqtt_user.h"
// #include "mbedtls_sockets.h"

uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

extern shell_sig_func_ptr abort_exec;
TaskHandle_t client_daemon;
int test_sockfd;
const char* addr;
struct mqtt_client client;

extern MqttItem_t MqttItem;

const char EMQX_ca_file[]="-----BEGIN CERTIFICATE-----\r\n\
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\r\n\
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n\
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n\
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\r\n\
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n\
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\r\n\
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\
-----END CERTIFICATE-----";
/*
    A template for opening a non-blocking POSIX socket.
*/
#if 1
static int open_nb_socket(const char* addr, const char* port);

static int open_nb_socket(const char* addr, const char* port) {
    struct addrinfo hints = {0};

    hints.ai_family = AF_UNSPEC; /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Must be TCP */
    int sockfd = -1;
    int rv;
    struct addrinfo *p, *servinfo;

    /* get address information */
    rv = getaddrinfo(addr, port, &hints, &servinfo);
    if(rv != 0) {
        printf("Failed to open socket (getaddrinfo): %d\r\n", rv);
        return -1;
    }

    /* open the first possible socket */
    for(p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        /* connect to server */
        rv = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if(rv == -1) {
          close(sockfd);
          sockfd = -1;
          continue;
        }
        break;
    }

    /* free servinfo */
    freeaddrinfo(servinfo);

    /* make non-blocking */
    if (sockfd != -1) {
        int iMode = 1;
        ioctlsocket(sockfd, FIONBIO, &iMode);
    }

    return sockfd;
}
#endif
/**
 * @brief The function will be called whenever a PUBLISH message is received.
 */
static void publish_callback_1(void** unused, struct mqtt_response_publish *published);

/**
 * @brief The client's refresher. This function triggers back-end routines to
 *        handle ingress/egress traffic to the broker.
 *
 * @note All this function needs to do is call \ref __mqtt_recv and
 *       \ref __mqtt_send every so often. I've picked 100 ms meaning that
 *       client ingress/egress traffic will be handled every 100 ms.
 */
static void client_refresher(void* client);

/**
 * @brief Safelty closes the \p sockfd and cancels the \p client_daemon before \c exit.
 */
static void test_close(int sig)
{
    if (test_sockfd)
    {
        close(test_sockfd);
    }
    printf("mqtt_pub stop publish to %s\r\n", addr);

    abort_exec(sig);
    if(client_daemon != NULL){
    vTaskDelete(client_daemon);
    }

}

int MqttConnect(char *addr, char *port)
{
    // const char* ca_file;
    // struct mbedtls_context ctx;
    /* open the non-blocking TCP socket (connecting to the broker) */
    test_sockfd = open_nb_socket(addr, port);
    // open_nb_socket(&ctx, addr, port, ca_file);
    // test_sockfd = &ctx.ssl_ctx;

    if (test_sockfd < 0) {
        printf("Failed to open socket: %d\r\n", test_sockfd);
        if (test_sockfd)
        {
            close(test_sockfd);
        }
        printf("client_daemon:%d\r\n",client_daemon);
        if(client_daemon != NULL){
            vTaskDelete(client_daemon);
        }
        return -1;
    }

    /* setup a client */
    // struct mqtt_client client;

    mqtt_init(&client, test_sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback_1);
    /* Create an anonymous session */
    const char* client_id = NULL;
    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    int ret = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

    if (ret != MQTT_OK)
    {
        printf("fail \r\n");
    }
    /* check that we don't have any errors */
    if (client.error != MQTT_OK) {
        printf("error: %s\r\n", mqtt_error_str(client.error));
        if (test_sockfd)
        {
            close(test_sockfd);
        }
        if(client_daemon != NULL){
            vTaskDelete(client_daemon);
        }
        return -2;
    }

    
    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    xTaskCreate(client_refresher, (char*)"client_ref", 1024,  &client, 10, &client_daemon);
    printf("mqtt connect success!\r\n");
    return 0;
}

static int example_mqtt(int argc, const char *argv[])
{
    MqttItem_t MqttItem_env = {0};
    const char* port;
    const char* topic;
    const char* ca_file;

    // struct mbedtls_context ctx;
    // mqtt_pal_socket_handle sockfd;

    int ret = 0;
    // int argc = 0;

    abort_exec = shell_signal(1, test_close);

    // struct bflb_device_s *rtc;

    // rtc = bflb_device_get_by_name("rtc");
    // bflb_rtc_set_time(rtc, 0);
    // /* our build utc time 1686873600 2023-06-16 00:00:00 */
    // time_t time_stamp = 1686873600;

    /* get address (argv[1] if present) */
    if (argc > 1) {
        addr = argv[1];
    } else {
        addr = "test.mosquitto.org";
    }

    /* get port number (argv[2] if present) */
    if (argc > 2) {
        port = argv[2];
    } else {
        port = "1883";
    }

    /* get the topic name to publish */
    if (argc > 3) {
        topic = argv[3];
    } else {
        topic = "bl618";
    }

    /* open the non-blocking TCP socket (connecting to the broker) */
    test_sockfd = open_nb_socket(addr, port);
    // open_nb_socket(&ctx, addr, port, ca_file);
    // test_sockfd = &ctx.ssl_ctx;

    if (test_sockfd < 0) {
        printf("Failed to open socket: %d\r\n", test_sockfd);
        test_close(SHELL_SIGINT);
    }

    /* setup a client */
    // struct mqtt_client client;

    mqtt_init(&client, test_sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback_1);
    /* Create an anonymous session */
    const char* client_id = NULL;
    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    ret = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

    if (ret != MQTT_OK)
    {
        printf("fail \r\n");
    }
    /* check that we don't have any errors */
    if (client.error != MQTT_OK) {
        printf("error: %s\r\n", mqtt_error_str(client.error));
        test_close(SHELL_SIGINT);
    }

    
    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    xTaskCreate(client_refresher, (char*)"client_ref", 1024,  &client, 10, &client_daemon);
    printf("mqtt connect success!\r\n");
    printf("save mqtt info\r\n");
    memcpy(MqttItem_env.s_addr, addr, strlen(addr));
    memcpy(MqttItem_env.s_port, port, strlen(port));
    memcpy(MqttItem_env.s_topic, topic, strlen(topic));

    // ef_set_and_save_env(MQTT_ENV_ADDR, MqttItem_env.s_addr);
    // ef_set_and_save_env(MQTT_ENV_PORT, MqttItem_env.s_port);
    ef_set_env_blob(MQTT_ENV_ADDR, MqttItem_env.s_addr, sizeof(MqttItem_env.s_addr));
    ef_set_env_blob(MQTT_ENV_PORT, MqttItem_env.s_port, sizeof(MqttItem_env.s_port));
    // ef_set_env_blob(MQTT_ENV_INFO, &MqttItem_env, sizeof(MqttItem_env));
    ef_save_env();

    /* start publishing the time */
    // printf("%s is ready to begin publishing the time.\r\n", argv[0]);
    // printf("Press ENTER to publish the current time.\r\n");
    // printf("Press CTRL-C to exit.\r\n");

    /* block wait CTRL-C exit */
    // while(1) {
    //     /* get the current time */
    //     time_t time = (BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc)) + time_stamp);
    //     struct tm *gmt = gmtime(&time);
    //     char time_buf[26];
    //     strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", gmt);
    //     // printf("%s\r\n", time_buf);

    //     /* print a message */
    //     char application_message[256] = {"{\"hello mqtt by bl616/8 !\"}\r\n"};
    //     sprintf(&application_message[strlen(application_message)], "Now time is %s", time_buf);
    //     printf("%s published : \"%s\"\r\n", argv[0], application_message);

    //     /* publish the time */
    //     mqtt_publish(&client, topic, application_message, strlen(application_message) + 1, MQTT_PUBLISH_QOS_0);

    //     /* check for errors */
    //     if (client.error != MQTT_OK) {
    //         printf("error: %s\r\n", mqtt_error_str(client.error));
    //         test_close(SHELL_SIGINT);
    //     }
    //     vTaskDelay(3000);
    // }

    /* disconnect */
    /* exit */
    // test_close(SHELL_SIGINT);

}
static int example_mqtt_publisher(int argc, const char *argv[])
{
    char *topic;
    uint8_t qos;
    char *payload;


    if(argc != 4){
        printf("para number error\r\n");
        return;
    }
    topic = argv[1];
    qos = atoi(argv[2]);
    payload = argv[3];

    mqtt_publish(&client, topic, payload, strlen(payload) + 1, ((qos << 1) & 0x06));
    if (client.error != MQTT_OK) {
        printf("publish error: %s\r\n", mqtt_error_str(client.error));
        if (test_sockfd)
        {
            close(test_sockfd);
        }
        printf("mqtt_pub stop publish to %s\r\n", addr);
        if(client_daemon != NULL){
            vTaskDelete(client_daemon);
        }
    }

    printf("mqtt_publish ok\r\n");
}
static int example_mqtt_subscribe(int argc, const char *argv[])
{
    char *topic;
    uint8_t qos;


    if(argc != 3){
        printf("para number error\r\n");
        return;
    }
    topic = argv[1];
    qos = atoi(argv[2]);

    mqtt_subscribe(&client, topic, qos);
    if (client.error != MQTT_OK) {
        printf("subscribe error: %s\r\n", mqtt_error_str(client.error));
        
    }
    
    printf("mqtt_subscribe ok\r\n");
}

static void publish_callback_1(void** unused, struct mqtt_response_publish *published)
{
    /* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
    char* topic_name = (char*) malloc(published->topic_name_size + 1);
    memcpy(topic_name, published->topic_name, published->topic_name_size);
    topic_name[published->topic_name_size] = '\0';

    char* topic_msg = (char*) malloc(published->application_message_size + 1);
    memcpy(topic_msg, published->application_message, published->application_message_size);
    topic_msg[published->application_message_size] = '\0';

    printf("Received publish('%s'): %s\r\n", topic_name, topic_msg);

    free(topic_name);
    free(topic_msg);
}

static void client_refresher(void* client)
{
    while(1)
    {
        mqtt_sync((struct mqtt_client*) client);
        vTaskDelay(100);
    }

}

// #ifdef CONFIG_SHELL
#include <shell.h>

// extern uint32_t wifi_state;
// static int check_wifi_state(void)
// {
//     if (wifi_state == 1)
//     {
//         return 0;
//     } else {
//         return 1;
//     }
// }

int cmd_mqtt_connect(int argc, const char **argv)
{
    uint32_t ret = 0;

    // ret = check_wifi_state();
    // if (ret != 0) {
    //     printf("your wifi not connected!\r\n");
    //     return 0;
    // }

    // xTaskCreate(example_mqtt,(char*)"test_mqtt", 8192, argv, 10, NULL);
    example_mqtt(argc, argv);

    return 0;
}
int cmd_mqtt_publisher(int argc, const char **argv)
{
    uint8_t ret = 0;

    // ret = mqtt_client_is_connected(&client);
    // if (ret != 1) {
    //     printf("mqtt not connected!\r\n");
    //     return 0;
    // }

    // xTaskCreate(example_mqtt,(char*)"test_mqtt", 8192, argv, 10, NULL);
    example_mqtt_publisher(argc, argv);

    return 0;
}
int cmd_mqtt_subscribe(int argc, const char **argv)
{
    uint32_t ret = 0;

    // ret = check_wifi_state();
    // if (ret != 0) {
    //     printf("your wifi not connected!\r\n");
    //     return 0;
    // }

    // xTaskCreate(example_mqtt,(char*)"test_mqtt", 8192, argv, 10, NULL);
    example_mqtt_subscribe(argc, argv);

    return 0;
}


SHELL_CMD_EXPORT_ALIAS(cmd_mqtt_connect, mqtt, mqtt publisher);
SHELL_CMD_EXPORT_ALIAS(cmd_mqtt_publisher, mqtt_pub, mqtt publisher);
SHELL_CMD_EXPORT_ALIAS(cmd_mqtt_subscribe, mqtt_sub, mqtt subscribe);

// #endif
