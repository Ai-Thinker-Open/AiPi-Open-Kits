
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <lwip/api.h>
#include <lwip/arch.h>
#include <lwip/opt.h>
#include <lwip/inet.h>
#include <lwip/errno.h>
#include <netdb.h>

#include "shell.h"
#include "utils_getopt.h"
#include "bflb_mtimer.h"

#include "ring_buffer.h"


#define MAX_SOCKET_ITEM_NUMBER 5
#define SOCKET_PACKAGE_SIZE 1024
#define SOCKET_THREAD_SIZE	(1024 + SOCKET_PACKAGE_SIZE)
#define SOCKET_THREAD_PRIORITY	(3)
#define SOCKET_MAX_LISTINE_COUNT	(5)	//创建socket server时的最大监听数量

shell_sig_func_ptr abort_exec;

typedef void (*TaskFunction_t)( void * );

//SOCKET类型
typedef enum {
    SOCKET_NO_USE = 0,
	SOCKET_UDP_SERVER=1,	//UDP服务端
	SOCKET_UDP_CLIENT=2,	//UDP客户端
	SOCKET_TCP_SERVER=3,	//TCP服务端
	SOCKET_TCP_CLIENT=4,	//TCP客户端
	SOCKET_TCP_SEED=5,	//TCP seed(模组创建TCP服务端后如果有客户端和模组连接就会产生一个seed，无法主动创建)
} SocketType_e;

typedef struct
{
	TaskFunction_t threadFunc;	//任务函数
	char *threadName;	//任务名称(任务名称长度受系统配置影响，超出部分可能无效)
	void *userParam;	//需要传到任务中的参数地址(指针)
	uint32_t stackSize;	//任务堆栈大小(注意单位，这里是字节，FreeRTOS在32位平台上1单位一般是4字节)
	uint8_t priority;	//优先级 0~N (N具体到多少要看系统支持多少，一般不要超过32)数值越大优先级越高(和FreeRTOS相同,和UCOS相反)
	TaskHandle_t TaskHandle;	//如果创建成功这个返回的就是对应的任务句柄(函数内会完成该变量的赋值，这个需要函数内部开辟空间实现)否则返回NULL
} TaskParams_t;

typedef struct
{
	SocketType_e socketType;	//Socke类型
	char remoteHost[30];	//访问域名或者IP字符串指针(这个空间一般是malloc出来的，使用完毕记得free)
	int remotePort;	//访问的远程端口
	int localPort;	//使用或者监听的本地端口
	int keeplive;	//心跳间隔
	int sockfd;		//socket句柄
    int sock_TcpServerConnected;
	uint32_t conID;	//
	struct sockaddr_in socketObj;	//socket 连接对象
    struct sockaddr_in udpServerReciveObj;	//该变量仅限udpserver有效 ，这个是用来记录upd server接收到数据之后获取到的客户端的连接对象
    struct sockaddr_in udpClientReciveObj;
    uint8_t TTMode;


    TaskParams_t socketThreadParam;
    TaskParams_t TTmodeThreadParam;
} SocketItem_t;

static SocketItem_t SocketItem[MAX_SOCKET_ITEM_NUMBER] = {0};

Ring_Buffer_Type TTmode_rb;
uint8_t TTmode_send_buf[SOCKET_PACKAGE_SIZE] = {0};
extern SemaphoreHandle_t sem_shell;

int8_t Socket_FindFreeCon(void)
{
    for(int i=0; i<MAX_SOCKET_ITEM_NUMBER; i++){
        if(SocketItem[i].socketType == 0){
            return i;
        }
    }
    return -1;
}

static int SocketItemTask_Delete(SocketItem_t *pSocketItem)
{
    if (pSocketItem->sock_TcpServerConnected >= 0 && pSocketItem->socketType == SOCKET_TCP_SERVER) {
        closesocket(pSocketItem->sock_TcpServerConnected);
        pSocketItem->sock_TcpServerConnected = -1;
    }
    if(pSocketItem->sockfd >= 0){
        closesocket(pSocketItem->sockfd);
        pSocketItem->sockfd = -1;
    }
    pSocketItem->socketType = SOCKET_NO_USE;
    // memset(pSocketItem, 0, sizeof(SocketItem_t));
    pSocketItem->TTMode = 0;
    if(pSocketItem->TTmodeThreadParam.TaskHandle){
        vTaskDelete( pSocketItem->TTmodeThreadParam.TaskHandle );
    }
    if(pSocketItem->socketThreadParam.TaskHandle){
        vTaskDelete( pSocketItem->socketThreadParam.TaskHandle );
    }
    
    
}

void TTmode_task(void *para)
{
    int SendLen = 0;
    int SendRet = 0;
    int i = 0;
    socklen_t addr_len;
    SocketItem_t *pSocketItem = (SocketItem_t *)para;
    
    Ring_Buffer_Init(&TTmode_rb, TTmode_send_buf, sizeof(TTmode_send_buf), NULL, NULL);
    while (1) {
        // if (xSemaphoreTake(sem_shell, portMAX_DELAY) == pdTRUE) {
            vTaskDelay(50);
            SendLen = Ring_Buffer_Get_Length(&TTmode_rb);
            if(SendLen > 0){
                //quit TTmode and close socket
                // printf("+Get_Length:%d\r\n",SendLen);
                Ring_Buffer_Read(&TTmode_rb, TTmode_send_buf, SendLen);
                
                if(strstr(TTmode_send_buf, "+++") != NULL){
                    printf("quit TTmode and close socket\r\n");
                    if(pSocketItem->socketThreadParam.TaskHandle){
                        vTaskDelete( pSocketItem->socketThreadParam.TaskHandle );
                    }
                    SocketItemTask_Delete(pSocketItem);
                }
                switch(pSocketItem->socketType)
                {
                    case SOCKET_TCP_CLIENT:
                        SendRet=send(pSocketItem->sockfd,TTmode_send_buf,SendLen,0);
                        if(SendRet!=SendLen){
                            printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                        }
                    break;
                    case SOCKET_TCP_SERVER:
                        SendRet=send(pSocketItem->sock_TcpServerConnected,TTmode_send_buf,SendLen,0);
                        if(SendRet!=SendLen){
                            printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                        }
                    break;
                    case SOCKET_UDP_SERVER:
                        if(!pSocketItem->udpServerReciveObj.sin_addr.s_addr){
                            printf("\r\nudpServerReciveObj is null\r\nERROR\r\n");
                        }
                        SendRet=sendto(pSocketItem->sockfd, TTmode_send_buf, SendLen, 0, &pSocketItem->udpServerReciveObj, sizeof(struct sockaddr_in));
                        if(SendRet!=SendLen){
                            printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                        }
                    break;
                    case SOCKET_UDP_CLIENT:
                        SendRet=sendto(pSocketItem->sockfd,TTmode_send_buf,SendLen,0, &pSocketItem->socketObj, sizeof(struct sockaddr_in));
                        if(SendRet!=SendLen){
                            printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                        }
                    break;

                    default:
                        printf("para socket Type error\r\n");
                    break;
                }
                memset(TTmode_send_buf, 0, sizeof TTmode_send_buf);
            }
        // }
    }
}
uint8_t is_TTmode(void)
{
    for(int i=0; i<MAX_SOCKET_ITEM_NUMBER; i++){
        if(SocketItem[i].TTMode){
            return 1;
        }
    }
    // if(SocketItem[0].TTMode == 1){
    //     printf("T");
    //     return 1;
    // }
    return 0;
}
void TcpClient_task(void *para)
{
    uint8_t recv_data[SOCKET_PACKAGE_SIZE];
    int recv_data_len = 0;
    int i = 0;
    SocketItem_t *pSocketItem = (SocketItem_t *)para;
    
    
    while(1)
    {
        vTaskDelay(2);
        recv_data_len = recv((pSocketItem->sockfd), recv_data, SOCKET_PACKAGE_SIZE, 0);
            if (recv_data_len == 0) {
                printf("close socket conID:%d\r\n", pSocketItem->conID);
                SocketItemTask_Delete(pSocketItem);
                return;
            }
            if (recv_data_len < 0) {
                continue;
            }
            // printf("recv %lld len data\r\n", recv_data_cnt);
            // if (write(sock, recv_data, recv_data_cnt) < 0) {
            //     printf("write falied!\r\n");
            //     break;
            // }
            printf("Tcp Client recv_data_len:%d\r\n", recv_data_len);
            printf("HEX DATA:");
            for(i=0; i<recv_data_len; i++){
                printf("%02x ", recv_data[i]);
            }
            printf("\r\n");
            
    }
}
void TcpServer_task(void *para)
{
    uint8_t recv_data[SOCKET_PACKAGE_SIZE];
    int recv_data_len = 0;
    int i = 0, connected = 0;
    SocketItem_t *pSocketItem = (SocketItem_t *)para;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    
    connected = accept(pSocketItem->sockfd, (struct sockaddr *)&pSocketItem->socketObj, &sin_size);
    pSocketItem->sock_TcpServerConnected = connected;
    printf("new client connected from (%s, %d)\r\n", inet_ntoa(pSocketItem->socketObj.sin_addr), ntohs(pSocketItem->socketObj.sin_port));
    printf("sockfd:%d, connected:%d\r\n", pSocketItem->sockfd, connected);
    // int flag = 1;
    // setsockopt(connected, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));
    pSocketItem->TTMode = 1;
    printf("TTmode_task creat:%d\r\n",xTaskCreate(TTmode_task, (char *)"TTmode_task", SOCKET_THREAD_SIZE, pSocketItem, SOCKET_THREAD_PRIORITY, &pSocketItem->TTmodeThreadParam.TaskHandle));

    while(1)
    {
        vTaskDelay(2);
        recv_data_len = recv(connected, recv_data, SOCKET_PACKAGE_SIZE, 0);
            if (recv_data_len == 0) {
                printf("close socket\r\n");
                SocketItemTask_Delete(pSocketItem);
                break;
            }
            if (recv_data_len < 0) {
                continue;
            }
            // printf("recv %lld len data\r\n", recv_data_cnt);
            // if (write(sock, recv_data, recv_data_cnt) < 0) {
            //     printf("write falied!\r\n");
            //     break;
            // }
            printf("Tcp Server recv_data_len:%d\r\n", recv_data_len);
            printf("HEX DATA:");
            for(i=0; i<recv_data_len; i++){
                printf("%02x ", recv_data[i]);
            }
            printf("\r\n");
    }
}
void UdpServer_task(void *para)
{
    uint8_t recv_data[SOCKET_PACKAGE_SIZE] = {0};
    int recv_data_len = 0;
    int i = 0;
    socklen_t addr_len;
    SocketItem_t *pSocketItem = (SocketItem_t *)para;
    
    
    while(1)
    {
        vTaskDelay(2);
        recv_data_len = recvfrom((pSocketItem->sockfd), recv_data, SOCKET_PACKAGE_SIZE, 0, (struct sockaddr*)&pSocketItem->udpServerReciveObj, &addr_len);
            // if (recv_data_len == 0) {
            //     printf("close socket\r\n");
            //     SocketItemTask_Delete(pSocketItem);
            //     break;
            // }
            if (recv_data_len <= 0) {
                continue;
            }
            // printf("recv %lld len data\r\n", recv_data_cnt);
            // if (write(sock, recv_data, recv_data_cnt) < 0) {
            //     printf("write falied!\r\n");
            //     break;
            // }
            printf("Udp Server recv_data_len:%d\r\n", recv_data_len);
            printf("HEX DATA:");
            for(i=0; i<recv_data_len; i++){
                printf("%02x ", recv_data[i]);
            }
            printf("\r\n");
    }
}
void UdpClient_task(void *para)
{
    uint8_t recv_data[SOCKET_PACKAGE_SIZE] = {0};
    int recv_data_len = 0;
    int i = 0;
    socklen_t addr_len;
    SocketItem_t *pSocketItem = (SocketItem_t *)para;
    
    
    while(1)
    {
        vTaskDelay(2);
        recv_data_len = recvfrom((pSocketItem->sockfd), recv_data, SOCKET_PACKAGE_SIZE, 0, (struct sockaddr*)&pSocketItem->udpClientReciveObj, &addr_len);
            // if (recv_data_len == 0) {
            //     printf("close socket\r\n");
            //     SocketItemTask_Delete(pSocketItem);
            //     break;
            // }
            if (recv_data_len <= 0) {
                continue;
            }
            // printf("recv %lld len data\r\n", recv_data_cnt);
            // if (write(sock, recv_data, recv_data_cnt) < 0) {
            //     printf("write falied!\r\n");
            //     break;
            // }
            printf("Udp Server recv_data_len:%d\r\n", recv_data_len);
            printf("HEX DATA:");
            for(i=0; i<recv_data_len; i++){
                printf("%02x ", recv_data[i]);
            }
            printf("\r\n");
    }
}


static void test_close(int sig)
{
    // if (sock_client) {
    //     closesocket(sock_client);
    // }
    // abort_exec(sig);
    // if (total_cnt > 0) {
    //     printf("Total send data=%lld\r\n", total_cnt);
    // }
}

#define PING_USAGE                    \
    "wifi_tcp_client [ip] [port]\r\n"   \
    "\t ip: dest ip or server ip\r\n" \
    "\t port: dest server listen port\r\n"

static void wifi_tcp_client_init(int argc, char **argv)
{
    abort_exec = shell_signal(SHELL_SIGINT, test_close);
    printf("tcp client task start ...\r\n");

    char *addr;
    char *port;
    struct sockaddr_in remote_addr;
    int sock_client;
    int8_t conID = 0;

    if (argc < 3) {
        printf("%s", PING_USAGE);
        return;
    }

    /* get address (argv[1] if present) */
    addr = argv[1];

    /* get port number (argv[2] if present) */
    port = argv[2];

    conID = Socket_FindFreeCon();
    if(conID < 0){
        printf("Socket Creat error conID:%d\r\n", conID);
        return;
    }
    while (1) {
        if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("TCP Client create socket error\r\n");
            return;
        }
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port = htons(atoi(port));
        remote_addr.sin_addr.s_addr = inet_addr(addr);
        memset(&(remote_addr.sin_zero), 0, sizeof(remote_addr.sin_zero));

        printf("Server ip Address : %s:%s\r\n", addr, port);

        if (connect(sock_client, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) != 0) {
            printf("TCP client connect server falied!\r\n");
            closesocket(sock_client);
            return;
        }
        SocketItem[conID].sockfd = sock_client;
        SocketItem[conID].remotePort = atoi(port);
        memcpy(SocketItem[conID].remoteHost, addr, strlen(addr));
        SocketItem[conID].socketType = SOCKET_TCP_CLIENT;
        SocketItem[conID].conID = conID;

        // SocketItem[conID].socketThreadParam.priority = SOCKET_THREAD_PRIORITY;
        // SocketItem[conID].socketThreadParam.stackSize = SOCKET_THREAD_SIZE;
        xTaskCreate(TcpClient_task, (char *)"TcpClient_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].socketThreadParam.TaskHandle);
        printf("conID:%d\r\nok\r\n", conID);
        SocketItem[conID].TTMode = 1;
        printf("TTmode_task creat:%d\r\n",xTaskCreate(TTmode_task, (char *)"TTmode_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].TTmodeThreadParam.TaskHandle));

        // printf("TCP client connect server success!\r\n");
        // printf("Press CTRL-C to exit.\r\n");
        // total_cnt = 0;
        // while (1) {
        //     if (write(sock_client, send_buf, sizeof(send_buf)) < 0) {
        //         printf("write falied!\r\n");
        //         break;
        //     }
        //     total_cnt += sizeof(send_buf);
        //     vTaskDelay(500);
        // }
        // closesocket(sock_client);
        return;
    }
}
#define SERVER_USAGE                  \
    "wifi_tcp_server [port]\r\n" \
    "\t port: local server listen port\r\n"
static void wifi_tcp_server_init(int argc, char **argv)
{
    abort_exec = shell_signal(SHELL_SIGINT, test_close);
    printf("tcp server task start ...\r\n");

    char *port;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    int sock_server;
    int8_t conID = 0;

    if (argc < 2) {
        printf("%s", SERVER_USAGE);
        return;
    }
    conID = Socket_FindFreeCon();
    if(conID < 0){
        printf("Socket Creat error conID:%d\r\n", conID);
        return;
    }
    /* get listen port (argv[1] if present) */
    port = argv[1];

    /* create socket */
    if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("TCP server create socket error\r\n");
        return;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* bind socket */
    if (bind(sock_server, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        printf("TCP server socket bind fail!\r\n");
        return;
    }

    /* listen port */
    if (listen(sock_server, 5) == -1) {
        printf("TCP server listen error!\r\n ");
        return;
    }

    SocketItem[conID].sockfd = sock_server;
    SocketItem[conID].localPort = atoi(port);
    SocketItem[conID].socketType = SOCKET_TCP_SERVER;
    // SocketItem[conID].socketThreadParam.priority = SOCKET_THREAD_PRIORITY;
    // SocketItem[conID].socketThreadParam.stackSize = SOCKET_THREAD_SIZE;
    xTaskCreate(TcpServer_task, (char *)"TcpServer_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].socketThreadParam.TaskHandle);
    printf("conID:%d\r\nok\r\n", conID);

    printf("TCP Server listen start: localhost:%s\r\n", port);
    printf("please connet ...\r\n");
    
}
#define PING_USAGE             \
    "wifi_udp_server [port]\r\n" \
    "\t port: local listen port, default port 5001\r\n"

static void wifi_udp_server_init(int argc, char **argv)
{
    abort_exec = shell_signal(SHELL_SIGINT, test_close);
    printf("udp server task start ...\r\n");

    char *port;
    struct sockaddr_in udp_addr, remote_addr;
    socklen_t addr_len;
    int sock;
    int8_t conID = 0;


    if (argc < 2) {
        printf("%s", PING_USAGE);
        return;
    }

    /* get port number (argv[1] if present) */
    if (argc > 1) {
        port = argv[1];
    } else {
        port = "5001";
    }
    conID = Socket_FindFreeCon();
    if(conID < 0){
        printf("Socket Creat error conID:%d\r\n", conID);
        return;
    }

    while (1) {
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            printf("udp create socket error\r\n");
            return;
        }
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_port = htons(atoi(port));
        udp_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(udp_addr.sin_zero), 0, sizeof(udp_addr.sin_zero));

        printf("Server ip Address : %s:%s\r\n", udp_addr.sin_addr.s_addr, port);

        if (bind(sock, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr)) != 0) {
            printf("udp bind falied!\r\n");
            closesocket(sock);
            return;
        }
        SocketItem[conID].sockfd = sock;
        SocketItem[conID].localPort = atoi(port);
        SocketItem[conID].socketType = SOCKET_UDP_SERVER;
        // SocketItem[conID].socketThreadParam.priority = SOCKET_THREAD_PRIORITY;
        // SocketItem[conID].socketThreadParam.stackSize = SOCKET_THREAD_SIZE;
        xTaskCreate(UdpServer_task, (char *)"UdpServer_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].socketThreadParam.TaskHandle);
        printf("conID:%d\r\nok\r\n", conID);
        // printf("udp bind port success!\r\n");
        SocketItem[conID].TTMode = 1;
        xTaskCreate(TTmode_task, (char *)"TTmode_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].TTmodeThreadParam.TaskHandle);


        return;
    }
}
#define PING_USAGE             \
    "wifi_udp_client [ip] [port]\r\n"   \
    "\t ip: dest ip or server ip\r\n" \
    "\t port: dest server listen port\r\n"

static void cmd_wifi_udp_client_init(int argc, char **argv)
{
    abort_exec = shell_signal(SHELL_SIGINT, test_close);
    printf("udp server task start ...\r\n");

    char *port;
    char *addr;
    struct sockaddr_in udp_addr, remote_addr;
    socklen_t addr_len;
    int sock;
    int8_t conID = 0;

    
    
    if (argc < 2) {
        printf("%s", PING_USAGE);
        return;
    }
    /* get address (argv[1] if present) */
    addr = argv[1];
    /* get port number (argv[1] if present) */
    if (argc > 1) {
        port = argv[2];
    } else {
        port = "5001";
    }
    conID = Socket_FindFreeCon();
    if(conID < 0){
        printf("Socket Creat error conID:%d\r\n", conID);
        return;
    }

    while (1) {
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            printf("udp create socket error\r\n");
            return;
        }
        SocketItem[conID].socketObj.sin_family = AF_INET;
        SocketItem[conID].socketObj.sin_addr.s_addr = inet_addr(addr);
        SocketItem[conID].socketObj.sin_port = htons(atoi(port));
        memset(&(SocketItem[conID].socketObj.sin_zero), 0, sizeof(SocketItem[conID].socketObj.sin_zero));
        
        SocketItem[conID].sockfd = sock;
        SocketItem[conID].remotePort = atoi(port);
        SocketItem[conID].socketType = SOCKET_UDP_CLIENT;
        // SocketItem[conID].socketThreadParam.priority = SOCKET_THREAD_PRIORITY;
        // SocketItem[conID].socketThreadParam.stackSize = SOCKET_THREAD_SIZE;
        xTaskCreate(UdpClient_task, (char *)"UdpClient_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].socketThreadParam.TaskHandle);
        SocketItem[conID].TTMode = 1;
        xTaskCreate(TTmode_task, (char *)"TTmode_task", SOCKET_THREAD_SIZE, &SocketItem[conID], SOCKET_THREAD_PRIORITY, &SocketItem[conID].TTmodeThreadParam.TaskHandle);
        

        printf("conID:%d\r\nok\r\n", conID);

        return;
    }
}

int cmd_wifi_socket_send(int argc, char **argv)
{
    uint8_t conID;
    int SendLen;
    int SendRet;
    char *SendData = NULL;

    if (argc != 4) {
        printf("para number error\r\n");
        return;
    }
    conID = atoi(argv[1]);
    SendLen = atoi(argv[2]);
    if(conID > MAX_SOCKET_ITEM_NUMBER-1){
        printf("para conID error\r\n");
        return;
    }
    if(SendLen > SOCKET_PACKAGE_SIZE){

    }
    switch(SocketItem[conID].socketType)
    {
        case SOCKET_TCP_CLIENT:
            SendRet=send(SocketItem[conID].sockfd,argv[3],SendLen,0);
            if(SendRet!=SendLen){
                printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                return;
            }
            printf("send success,send length=%d\r\n",SendRet);
            printf("\r\nOK\r\n");
        break;
        case SOCKET_TCP_SERVER:
            SendRet=send(SocketItem[conID].sock_TcpServerConnected,argv[3],SendLen,0);
            if(SendRet!=SendLen){
                printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                return;
            }
            printf("send success,send length=%d\r\n",SendRet);
            printf("\r\nOK\r\n");
            return;
        break;
        case SOCKET_UDP_SERVER:
            if(!SocketItem[conID].udpServerReciveObj.sin_addr.s_addr){
                printf("\r\nudpServerReciveObj is null\r\nERROR\r\n");
                return;
            }
            SendRet=sendto(SocketItem[conID].sockfd, argv[3], SendLen, 0, &SocketItem[conID].udpServerReciveObj, sizeof(struct sockaddr_in));
            if(SendRet!=SendLen){
                printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                return;
            }
            printf("send success,send length=%d\r\n",SendRet);
            printf("\r\nOK\r\n");
        break;
        case SOCKET_UDP_CLIENT:
            SendRet=sendto(SocketItem[conID].sockfd,argv[3],SendLen,0, &SocketItem[conID].socketObj, sizeof(struct sockaddr_in));
            if(SendRet!=SendLen){
                printf("\r\n+SOCKETSEND:%d\r\nERROR\r\n",SendRet);
                return;
            }
            printf("send success,send length=%d\r\n",SendRet);
            printf("\r\nOK\r\n");
        break;

        default:
            printf("para socket Type error\r\n");
            return;
        break;
    }
    

    return 0;
}
int cmd_wifi_socket_close(int argc, char **argv)
{
    uint8_t conID;

    if (argc !=2) {
        printf("para number error\r\n");
        return;
    }
    conID = atoi(argv[1]);
    if(conID > MAX_SOCKET_ITEM_NUMBER-1){
        printf("para conID error\r\n");
        return;
    }

    if(SocketItem[conID].socketType == SOCKET_NO_USE){
        printf("conID is not use\r\n");
        return;
    }
    SocketItemTask_Delete(&SocketItem[conID]);
    printf("conID:%d close socket ok\r\n", conID);
    return;

}


// #ifdef CONFIG_SHELL
#include <shell.h>

int cmd_wifi_tcp_client(int argc, char **argv)
{
    wifi_tcp_client_init(argc, argv);

    return 0;
}
int cmd_wifi_tcp_server(int argc, char **argv)
{
    wifi_tcp_server_init(argc, argv);

    return 0;
}
int cmd_wifi_udp_server(int argc, char **argv)
{
    wifi_udp_server_init(argc, argv);

    return 0;
}
int cmd_wifi_udp_client(int argc, char **argv)
{
    cmd_wifi_udp_client_init(argc, argv);

    return 0;
}
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_tcp_client, wifi_tcp_client, wifi tcp client);
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_tcp_server, wifi_tcp_server, wifi tcp server);
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_udp_server, wifi_udp_server, wifi udp server);
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_udp_client, wifi_udp_client, wifi udp client);
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_socket_send, socket_send, socket send);
SHELL_CMD_EXPORT_ALIAS(cmd_wifi_socket_close, socket_close, socket close);

// #endif

