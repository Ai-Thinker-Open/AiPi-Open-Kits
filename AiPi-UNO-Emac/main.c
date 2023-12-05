/****************************************************************************
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

 /****************************************************************************
  * Included Files
  ****************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "mem.h"

#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>

  // #include "bl_fw_api.h"
  // #include "wifi_mgmr_ext.h"
  // #include "wifi_mgmr.h"

#include "bflb_irq.h"
#include "bflb_uart.h"

#include "bl616_glb.h"
// #include "rfparam_adapter.h"

#include "board.h"
#include "shell.h"
#include "bflb_mtimer.h"

#define DBG_TAG "MAIN"
#include "log.h"

#include "easyflash.h"
#include "mqtt.h"
#include "mqtt_user.h"

#include "bflb_emac.h"
#include "ethernet_phy.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "ethernetif.h"
#include "lwip/dns.h"

struct bflb_device_s* gpio;

//user config


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WIFI_STACK_SIZE  (1536)
#define TASK_PRIORITY_FW (16)
#define WIFI_SSID_KEY   "wifi.ssid"
#define WIFI_PASSWD_KEY "wifi.passwd"


 /****************************************************************************
  * Private Types
  ****************************************************************************/

  /****************************************************************************
   * Private Data
   ****************************************************************************/

static struct bflb_device_s* uart0;

// static TaskHandle_t wifi_fw_task;

// static wifi_conf_t conf = {
//     .country_code = "CN",
// };



MqttItem_t MqttItem = { 0 };
TaskHandle_t mqttClient_taskHD;
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
int save_wifi_ssid_pwd(char* ssid, char* pwd);

extern void shell_init_with_task(struct bflb_device_s* shell);

/****************************************************************************
 * Functions
 ****************************************************************************/
#if 0
int wifi_start_firmware_task(void)
{
    LOG_I("Starting wifi ...\r\n");

    /* enable wifi clock */

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    if (0 != rfparam_init(0, NULL, 0)) {
        LOG_I("PHY RF init failed!\r\n");
        return 0;
    }

    LOG_I("PHY RF init success!\r\n");

    /* Enable wifi irq */

    extern void interrupt0_handler(void);
    bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    bflb_irq_enable(WIFI_IRQn);

    xTaskCreate(wifi_main, (char*)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

    return 0;
}

static wifi_mgmr_connect_ind_stat_info_t connection_info;
volatile uint32_t wifi_state = 0;
uint8_t wifi_save_state = 0;
void wifi_event_handler(uint32_t code)
{
    switch (code) {
        case CODE_WIFI_ON_INIT_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE\r\n", __func__);
            wifi_mgmr_init(&conf);
        } break;
        case CODE_WIFI_ON_MGMR_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_MGMR_DONE\r\n", __func__);
        } break;
        case CODE_WIFI_ON_SCAN_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_SCAN_DONE\r\n", __func__);
            wifi_mgmr_sta_scanlist();
        } break;
        case CODE_WIFI_ON_CONNECTED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_CONNECTED\r\n", __func__);
            void mm_sec_keydump();
            mm_sec_keydump();
        } break;
        case CODE_WIFI_ON_GOT_IP: {
            wifi_state = 1;
            wifi_save_state = 1;
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_GOT_IP\r\n", __func__);
            LOG_I("[SYS] Memory left is %d Bytes\r\n", kfree_size());

        } break;
        case CODE_WIFI_ON_DISCONNECT: {
            wifi_state = 0;
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_DISCONNECT\r\n", __func__);
        } break;
        case CODE_WIFI_ON_AP_STARTED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STARTED\r\n", __func__);
        } break;
        case CODE_WIFI_ON_AP_STOPPED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STOPPED\r\n", __func__);
        } break;
        case CODE_WIFI_ON_AP_STA_ADD: {
            LOG_I("[APP] [EVT] [AP] [ADD] %lld\r\n", xTaskGetTickCount());
        } break;
        case CODE_WIFI_ON_AP_STA_DEL: {
            LOG_I("[APP] [EVT] [AP] [DEL] %lld\r\n", xTaskGetTickCount());
        } break;
        default: {
            LOG_I("[APP] [EVT] Unknown code %u \r\n", code);
        }
    }
}


int read_MqttInfo(MqttItem_t* pBuf)
{
    int ret = 0;

    ret = ef_get_env_blob(MQTT_ENV_ADDR, pBuf->s_addr, sizeof(pBuf->s_addr), NULL);
    printf("get env MQTT addr:%s\r\n", pBuf->s_addr);
    ret = ef_get_env_blob(MQTT_ENV_PORT, pBuf->s_port, sizeof(pBuf->s_port), NULL);
    printf("get env MQTT PORT:%s\r\n", pBuf->s_port);
    // if (ef_get_env(MQTT_ENV_INFO) != NULL) {
    //     int ret = ef_get_env_blob(MQTT_ENV_INFO, pBuf, sizeof(MqttItem_t), NULL);
    //     // int ret = ef_get_env_blob(MQTT_ENV_INFO, pBuf->s_addr, 32, NULL);
    //     printf("get env ret:%d, s_addr:%s\r\n",ret, pBuf->s_addr);
    //     return ret;
    // }
    // else{
    //     printf("get env MQTT_ENV_INFO is NULL\r\n");
    // }

    return ret;
}

uint8_t wifi_connect(char* ssid, char* passwd)
{
    int ret = 255;
    // struct fhost_vif_ip_addr_cfg ip_cfg = {0};

    if (NULL==ssid || 0==strlen(ssid)) {
        return 1;
    }

    if (wifi_mgmr_sta_state_get() == 1) {
        wifi_sta_disconnect();
    }
    wifi_state = 0;
    if (wifi_sta_connect(ssid, passwd, NULL, NULL, 0, 0, 0, 1)) {
        return 4;
    }

    //等待连接成功
    for (int i = 0;i<10*30;i++) {
        vTaskDelay(100);
        if (wifi_state == 1) {
            return 0;
        }
        // switch(sta_ConnectStatus){
        // 	case CODE_WIFI_ON_CONNECTED:	//连接成功(表示wifi sta状态的时候表示同时获取IP(DHCP)成功，或者使用静态IP)
        // 		return 0;
        // 	case CODE_WIFI_ON_DISCONNECT:	//连接失败（超过了重连次数还没有连接成功的状态）
        // 		wifi_sta_disconnect();
        // 		return 4;
        // 	case CODE_WIFI_ON_GOT_IP:	
        // 		return 0;
        // 	default:
        // 		//等待连接成功
        // 		break;
        // }
    }

    return 14; //连接超时
}
int save_wifi_ssid_pwd(char* ssid, char* pwd)
{
    ef_set_and_save_env(WIFI_SSID_KEY, (const char*)ssid);
    ef_set_and_save_env(WIFI_PASSWD_KEY, (const char*)pwd);
    // ef_save_env();

    return 0;
}



int WifiSta_AutoConnect(void)
{
    char ssid[33] = { 0 };
    char pwd[65] = { 0 };
    int ret = 0;

    if (ef_get_env(WIFI_SSID_KEY) == NULL) {
        return -1;
    }
    if (ef_get_env(WIFI_PASSWD_KEY) == NULL) {
        return -2;
    }

    ret = ef_get_env_blob(WIFI_SSID_KEY, ssid, sizeof(ssid), NULL);
    ret = ef_get_env_blob(WIFI_PASSWD_KEY, pwd, sizeof(pwd), NULL);

    printf("get wifi env ok, now connecting ssid:%s, pwd:%d\r\n", ssid, pwd);
    return wifi_connect(ssid, pwd);
    // wifi_connect("zifeng", "123456789");
}
void mqttClient_task(void* para)
{

    vTaskDelay(2000);
    if (WifiSta_AutoConnect() == 0) {
        if (read_MqttInfo(&MqttItem) > 0) {
            if (MqttConnect(MqttItem.s_addr, MqttItem.s_port) == 0) {

            }
        }
        // printf("WifiSta_AutoConnect fail!\r\n");
        // goto EXIT;
    }
    // if(read_MqttInfo(&MqttItem) <= 0){
    //     printf("read_MqttInfo fail!\r\n");
    //     goto EXIT;
    // }
    // if(MqttConnect(MqttItem.s_addr, MqttItem.s_port) != 0){
    //     printf("Mqtt Connect fail!\r\n");
    //     goto EXIT;
    // }

    while (1)
    {
        if (wifi_save_state == 1) {
            wifi_save_state = 0;
            if (!wifi_mgmr_sta_connect_ind_stat_get(&connection_info)) {
                save_wifi_ssid_pwd(connection_info.ssid, connection_info.passphr);
                printf("save wifi ssid:%s , pwd:%s\r\n", connection_info.ssid, connection_info.passphr);
            }
            else {
                printf("get wiifi sta connect info error!!\r\n");
            }
        }
        vTaskDelay(200);
    }

EXIT:
    vTaskDelete(NULL);
}
#endif

#if 1
// Local IP address define
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0 (uint8_t)192
#define IP_ADDR1 (uint8_t)168
#define IP_ADDR2 (uint8_t)2
#define IP_ADDR3 (uint8_t)150

// Local NET Mask address define
/*NETMASK*/
#define NETMASK_ADDR0 (uint8_t)255
#define NETMASK_ADDR1 (uint8_t)255
#define NETMASK_ADDR2 (uint8_t)255
#define NETMASK_ADDR3 (uint8_t)0

// Local Gateway address define
/*Gateway Address*/
#define GW_ADDR0 (uint8_t)192
#define GW_ADDR1 (uint8_t)168
#define GW_ADDR2 (uint8_t)2
#define GW_ADDR3 (uint8_t)1

/* global network interface struct define */
struct netif gnetif;

/* For emac tx and rx buffer,we put here to make controlling it's size easy */
#define ETH_RXBUFNB 5
#define ETH_TXBUFNB 5
ATTR_NOCACHE_NOINIT_RAM_SECTION __attribute__((aligned(4))) uint8_t ethRxBuff[ETH_RXBUFNB][ETH_RX_BUFFER_SIZE]; /* Ethernet Receive Buffers */
ATTR_NOCACHE_NOINIT_RAM_SECTION __attribute__((aligned(4))) uint8_t ethTxBuff[ETH_TXBUFNB][ETH_TX_BUFFER_SIZE]; /* Ethernet Transmit Buffers */

/**
  * @brief  Setup the network interface
  * @param  None
  * @retval None
  */
static void netif_config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    /* if use DHCP */
#if LWIP_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else

    /* IP address default setting */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

#endif

    /* add the network interface */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /*  Registers the default network interface */
    netif_set_default(&gnetif);
    ethernet_link_status_updated(&gnetif);
#if LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif

#if LWIP_DHCP
    // struct netif *tmpnetif;
    // tmpnetif = netif_find("bl");
    // while (tmpnetif->state != 3) {
    while (gnetif.state != 3) {
        printf("wait DHCP get ip...\r\n");
        bflb_mtimer_delay_ms(500);
    }
#endif

    //dns
    ip_addr_t dns_ipaddr;
    dns_ipaddr.addr = 0x08080808;
    printf("dns_ipaddr:::%04x \r\n", dns_ipaddr.addr);
    dns_setserver(0, &dns_ipaddr);
}

static void ethernet_lwip_init()
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_30, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_16, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_reset(gpio, GPIO_PIN_30);
    bflb_gpio_reset(gpio, GPIO_PIN_16);
    bflb_mtimer_delay_ms(10);
    bflb_gpio_set(gpio, GPIO_PIN_16);


    printf("lwip statck init\r\n");

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    printf("netif config\r\n");
    /* Configure the Network interface */
    netif_config();

}

void emac_init_txrx_buffer(struct bflb_device_s* emac)
{
    bflb_emac_bd_init(emac, (uint8_t*)ethTxBuff, ETH_TXBUFNB, (uint8_t*)ethRxBuff, ETH_RXBUFNB);
}
#endif

int main(void)
{
    board_init();
    /* emac gpio init */
    board_emac_gpio_init();
    ethernet_lwip_init();

    bflb_mtd_init();
    easyflash_init();

    uart0 = bflb_device_get_by_name("uart0");
    shell_init_with_task(uart0);

    // tcpip_init(NULL, NULL);
    // wifi_start_firmware_task();


    // xTaskCreate(mqttClient_task, (char*)"mqttClient_task", 1024*2,  NULL, 3, &mqttClient_taskHD);
    vTaskStartScheduler();

    while (1) {
    }
}
