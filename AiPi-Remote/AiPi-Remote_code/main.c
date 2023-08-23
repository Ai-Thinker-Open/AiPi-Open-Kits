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


#define DBG_TAG "MAIN"
#include "log.h"


#include "lv_conf.h"
#include "lvgl.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "demos/lv_demos.h"

#include "lcd.h"
#include "portable.h"
  //#include "fhost.h"
#include "gui_guider.h"
#include "custom.h"
#include "easyflash.h"
#include "bflb_mtd.h"


#include "button_task.h"
#include "irsend_task.h"
#include "battery_task.h"
#include "power_task.h"
#include "voice_task.h"
#include "app_task.h"

extern xQueueHandle queue;
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WIFI_STACK_SIZE     (1536)
#define TASK_PRIORITY_FW    (16)

 /****************************************************************************
  * Private Types
  ****************************************************************************/

  /****************************************************************************
   * Private Data
   ****************************************************************************/

static struct bflb_device_s* uart0;
static int bl61x_get_heap_size(void);
#if 1
static TaskHandle_t wifi_fw_task;

static wifi_conf_t conf =
{
    .country_code = "CN",
};

static uint32_t sta_ConnectStatus = 0;

extern void shell_init_with_task(struct bflb_device_s* shell);
extern int es8388_voice_init(void);
extern int audio_init(void);
extern int audio_pcm_init(void);
extern void play_voice_open(void);
extern void play_voice_close(void);

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

 /****************************************************************************
  * Functions
  ****************************************************************************/

int wifi_start_firmware_task(void)
{
    LOG_I("Starting wifi ...\r\n");

    /* enable wifi clock */

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    /* set ble controller EM Size */

    GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

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

void wifi_event_handler(uint32_t code)
{

    sta_ConnectStatus = code;
    switch (code) {
        case CODE_WIFI_ON_INIT_DONE:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE\r\n", __func__);
            wifi_mgmr_init(&conf);
        }
        break;
        case CODE_WIFI_ON_MGMR_DONE:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_MGMR_DONE\r\n", __func__);
        }
        break;
        case CODE_WIFI_ON_SCAN_DONE:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_SCAN_DONE\r\n", __func__);
            wifi_mgmr_sta_scanlist();
        }
        break;
        case CODE_WIFI_ON_CONNECTED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_CONNECTED\r\n", __func__);
            void mm_sec_keydump();
            mm_sec_keydump();
        }
        break;
        case CODE_WIFI_ON_GOT_IP:
        {

            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_GOT_IP\r\n", __func__);
        }
        break;
        case CODE_WIFI_ON_DISCONNECT:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_DISCONNECT\r\n", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STARTED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STARTED\r\n", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STOPPED:
        {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STOPPED\r\n", __func__);
        }
        break;
        case CODE_WIFI_ON_AP_STA_ADD:
        {
            LOG_I("[APP] [EVT] [AP] [ADD] %lld\r\n", xTaskGetTickCount());
        }
        break;
        case CODE_WIFI_ON_AP_STA_DEL:
        {
            LOG_I("[APP] [EVT] [AP] [DEL] %lld\r\n", xTaskGetTickCount());
        }
        break;
        default:
        {
            LOG_I("[APP] [EVT] Unknown code %u \r\n", code);
        }
    }
}
#endif
/* lvgl log cb */
void lv_log_print_g_cb(const char* buf)
{
    printf("[LVGL] %s", buf);
}

static TaskHandle_t lvgl_TaskHandle;
#define LVGL_STACK_SIZE 1024*2
#define LVGL_TASK_PRIORITY 10

void lvgl_task(void* param)
{

    while (1)
    {
        lv_task_handler();
        vTaskDelay(1);
        // bflb_mtimer_delay_ms(1);
    }
}

void io2_set(uint8_t value)
{
    struct bflb_device_s* gpio;
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    if (value)
    {
        bflb_gpio_set(gpio, GPIO_PIN_2);
    }
    else
    {
        bflb_gpio_reset(gpio, GPIO_PIN_2);
    }

}

// uint8_t wifi_connect(char* ssid, char* passwd)
// {
//     int ret = 255;
//     // struct fhost_vif_ip_addr_cfg ip_cfg = { 0 };
//     uint32_t ipv4_addr = 0;
//     static char* queue_buff;
//     queue_buff = pvPortMalloc(128);
//     memset(queue_buff, 0, 128);
//     if (NULL==ssid || 0==strlen(ssid)) {
//         return 1;
//     }

//     if (wifi_mgmr_sta_state_get() == 1) {
//         wifi_sta_disconnect();
//     }
//     if (wifi_sta_connect(ssid, passwd, NULL, NULL, 0, 0, 0, 1)) {
//         return 4;
//     }

//     //等待连接成功
//     sta_ConnectStatus = 0;
//     for (int i = 0;i<10*30;i++) {
//         vTaskDelay(100);
//         switch (sta_ConnectStatus) {
//             case CODE_WIFI_ON_CONNECTED:	//连接成功(表示wifi sta状态的时候表示同时获取IP(DHCP)成功，或者使用静态IP)

//                 break;;
//             case CODE_WIFI_ON_DISCONNECT:	//连接失败（超过了重连次数还没有连接成功的状态）
//                 wifi_sta_disconnect();
//                 return 4;
//             case CODE_WIFI_ON_GOT_IP:
//                 wifi_sta_ip4_addr_get(&ipv4_addr, NULL, NULL, NULL);

//                 printf("wifi connened %s,IP:%s\r\n", ssid, inet_ntoa(ipv4_addr));
//                 sprintf(queue_buff, "{\"ip\":{\"IP\":\"%s\"}}", inet_ntoa(ipv4_addr));

//                 // flash_erase_set(SSID_KEY, ssid);
//                 // flash_erase_set(PASS_KEY, passwd);
//                 xQueueSend(queue, queue_buff, portMAX_DELAY);
//                 return 0;
//             default:
//                 //等待连接成功
//                 break;
//         }
//     }
//     vPortFree(queue_buff);
//     return 14; //连接超时
// }

void lwip_sntp_init(void)
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // sntp_setserver();
    // sntp_setservername(0, "pool.ntp.org");
    sntp_setservername(0, "ntp1.aliyun.com");
    // sntp_setservername(0, "114.67.237.130");
    sntp_init();

    printf("getservername:%s\r\n", sntp_getservername(0));
}

static void bl61x_show_heap_size_task(void* arg)
{

    while (1) {
        LOG_F("[%s:%d]heap_size-------> %d\r\n", DBG_TAG, __LINE__, bl61x_get_heap_size());
        vTaskDelay(3000/portTICK_PERIOD_MS);
    }
}

lv_ui guider_ui;

int main(void)
{

    board_init();
    // audio_init();
    // audio_pcm_init();
    // es8388_voice_init();

    // uart0 = bflb_device_get_by_name("uart0");
    // shell_init_with_task(uart0);

    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    // tcpip_init(NULL, NULL);
    // wifi_start_firmware_task();
    // lwip_sntp_init();
    //init easyflash
    bflb_mtd_init();
    easyflash_init();
    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    setup_ui(&guider_ui);

    printf("lv_task_handler\r\n");
    // lv_task_handler();
    custom_init(&guider_ui);
    printf("lvgl success\r\n");
    xTaskCreate(lvgl_task, (char*)"lvgl", LVGL_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, &lvgl_TaskHandle);
    xTaskCreate(bl61x_show_heap_size_task, (char*)"heap", 1024, NULL, 2, NULL);
    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    button_queue = xQueueCreate(10, sizeof(uint32_t));
    lvgl_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(irsend_task_process, (char *)"irsend_task_process", 1024, NULL, 20, &irsend_task_xhandle);
    xTaskCreate(button_task_process, (char *)"button_task_process", 1024, NULL, 16, &button_task_xhandle);
    xTaskCreate(battery_task_process, (char *)"battery_task_process", 1024, NULL, 17, &battery_task_xhandle);
    xTaskCreate(power_task_process, (char *)"power_task_process", 1024, NULL, 17, &power_task_xhandle);
    xTaskCreate(voice_task_process, (char *)"voice_task_process", 1024, NULL, 18, &voice_task_xhandle);
    xTaskCreate(app_task_process, (char *)"app_task_process", 1024, NULL, 18, &app_task_xhandle);
    
    vTaskStartScheduler();

    // while (1) {
    //     lv_task_handler();
    //     bflb_mtimer_delay_ms(1);
    // }

    while (1) {
        // printf("\r\n[%s] heap size ------------------- %d\r\n", __func__, xPortGetFreeHeapSize());
        vTaskDelay(5000/portTICK_RATE_MS);
    }
}
static int bl61x_get_heap_size(void)
{
    struct meminfo info1 = { 0 };
    struct meminfo info2 = { 0 };
    uint32_t total_free_size = 0;
    // return xPortGetFreeHeapSize();

    bflb_mem_usage(KMEM_HEAP, &info1);
    bflb_mem_usage(PMEM_HEAP, &info2);

    total_free_size = info1.free_size + info2.free_size;

    return total_free_size;
}