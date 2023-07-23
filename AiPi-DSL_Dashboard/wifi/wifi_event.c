/**
 * @file wifi_event.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2023
 *
*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include "sntp.h"

#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "bflb_irq.h"
#include "bflb_uart.h"
#include "bflb_l1c.h"
#include "bflb_mtimer.h"

#include "bl616_glb.h"
#include "rfparam_adapter.h"

#include "board.h"
#include "log.h"
#include "custom.h"

#define DBG_TAG "WIFI EVENT"

#define WIFI_STACK_SIZE     (1024*4)
#define TASK_PRIORITY_FW    (16)

static wifi_conf_t conf =
{
    .country_code = "CN",
};
static TaskHandle_t wifi_fw_task;
static uint32_t sta_ConnectStatus = 0;

extern xQueueHandle queue;
extern TaskHandle_t https_Handle;
/**
 * @brief WiFi 任务
 *
 * @return int
*/
int wifi_start_firmware_task(void)
{
    LOG_I("Starting wifi ...");

    /* enable wifi clock */

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    /* set ble controller EM Size */

    GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

    if (0 != rfparam_init(0, NULL, 0)) {
        LOG_I("PHY RF init failed!");
        return 0;
    }

    LOG_I("PHY RF init success!");

    /* Enable wifi irq */

    extern void interrupt0_handler(void);
    bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    bflb_irq_enable(WIFI_IRQn);

    xTaskCreate(wifi_main, (char*)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

    return 0;
}
/**
 * @brief wifi event handler
 *      WiFi 事件回调
 *
 * @param code
*/


void wifi_event_handler(uint32_t code)
{

    sta_ConnectStatus = code;

    switch (code) {
        case CODE_WIFI_ON_INIT_DONE:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE", __func__);
            wifi_mgmr_init(&conf);
        }
        break;
        case CODE_WIFI_ON_MGMR_DONE:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_MGMR_DONE", __func__);

        }
        break;
        case CODE_WIFI_ON_SCAN_DONE:
        {
            char* scan_msg = pvPortMalloc(128);
            wifi_mgmr_sta_scanlist();
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_SCAN_DONE SSID numbles:%d", __func__, wifi_mgmr_sta_scanlist_nums_get());
            sprintf(scan_msg, "{\"wifi_scan\":{\"status\":0}}");
            // xQueueSend(queue, scan_msg, );
            xQueueSendFromISR(queue, scan_msg, pdTRUE);
            vPortFree(scan_msg);
        }
        break;
        case CODE_WIFI_ON_CONNECTED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_CONNECTED", __func__);
            void mm_sec_keydump();
            mm_sec_keydump();
        }
        break;
        case CODE_WIFI_ON_GOT_IP:
        {

            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_GOT_IP", __func__);
        }
        break;
        case CODE_WIFI_ON_DISCONNECT:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_DISCONNECT", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STARTED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STARTED", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STOPPED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STOPPED", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STA_ADD:
        {
            LOG_I("[APP] [EVT] [AP] [ADD] %lld", xTaskGetTickCount());
        }
        break;
        case CODE_WIFI_ON_AP_STA_DEL:
        {
            LOG_I("[APP] [EVT] [AP] [DEL] %lld", xTaskGetTickCount());
        }
        break;
        default:
        {
            LOG_I("[APP] [EVT] Unknown code %u ", code);
        }
    }
}

uint8_t wifi_connect(char* ssid, char* passwd)
{
    int ret = 255;
    // struct fhost_vif_ip_addr_cfg ip_cfg = { 0 };
    uint32_t ipv4_addr = 0;
    char* queue_buff = pvPortMalloc(128);
    memset(queue_buff, 0, 128);
    if (NULL==ssid || 0==strlen(ssid)) {
        return 1;
    }

    if (wifi_mgmr_sta_state_get() == 1) {
        wifi_sta_disconnect();
    }
    if (wifi_sta_connect(ssid, passwd, NULL, NULL, 0, 0, 0, 1)) {
        return 4;
    }
    LOG_I("Wating wifi connet");
    //等待连接成功
    sta_ConnectStatus = 0;
    for (int i = 0;i<10*30;i++) {

        vTaskDelay(100/portTICK_PERIOD_MS);
        switch (sta_ConnectStatus) {
            case CODE_WIFI_ON_MGMR_DONE:
                // vTaskDelay(2000);
                // LOG_I("wifi_mgmr_sta_scan:%d", wifi_mgmr_sta_scan(wifi_scan_config));
                vPortFree(queue_buff);
                return 3;
            case CODE_WIFI_ON_SCAN_DONE:

                // LOG_I("WIFI STA SCAN DONE %s", wifi_scan_config[0].ssid_array);

                vPortFree(queue_buff);
                return 2;
            case CODE_WIFI_ON_DISCONNECT:	//连接失败（超过了重连次数还没有连接成功的状态）

                wifi_sta_disconnect();
                guider_ui.wifi_stayus = false;
                vPortFree(queue_buff);
                return 4;
            case CODE_WIFI_ON_CONNECTED:	//连接成功(表示wifi sta状态的时候表示同时获取IP(DHCP)成功，或者使用静态IP)
                // LOG_I("Wating wifi connet OK");
                break;
            case CODE_WIFI_ON_GOT_IP:
                wifi_sta_ip4_addr_get(&ipv4_addr, NULL, NULL, NULL);
                LOG_I("wifi connened %s,IP:%s", ssid, inet_ntoa(ipv4_addr));
                sprintf(queue_buff, "{\"ip\":{\"IP\":\"%s\"}}", inet_ntoa(ipv4_addr));
                flash_erase_set(SSID_KEY, ssid);
                flash_erase_set(PASS_KEY, passwd);
                guider_ui.wifi_stayus = true;
                xQueueSend(queue, queue_buff, portMAX_DELAY);
                LOG_I("Wating wifi connet OK and get ip OK");
                vPortFree(queue_buff);
                return 0;
            default:
                //等待连接成功
                break;
        }

    }
    vPortFree(queue_buff);


    return 14; //连接超时
}
