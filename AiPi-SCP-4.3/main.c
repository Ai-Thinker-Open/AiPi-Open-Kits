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
#include "shell.h"

#define DBG_TAG "MAIN"
#include "log.h"


#include "lv_conf.h"
#include "lvgl.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"


#include "lcd.h"
#include "portable.h"
// #include "fhost.h"

// #include "gui_guider.h"
#include "ui.h"

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

static struct bflb_device_s *uart0;
static struct bflb_device_s *gpio;
#if 1
static TaskHandle_t wifi_fw_task;

static wifi_conf_t conf =
{
    .country_code = "CN",
};

static uint32_t sta_ConnectStatus = 0;

extern void shell_init_with_task(struct bflb_device_s *shell);
extern int es8388_voice_init(void);
extern int audio_init(void);
extern int audio_pcm_init(void);
extern void play_voice_open(void);
extern void play_voice_close(void);
extern int es8388test_init(void);

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

    xTaskCreate(wifi_main, (char *)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

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
void lv_log_print_g_cb(const char *buf)
{
    printf("[LVGL] %s", buf);
}

static TaskHandle_t lvgl_TaskHandle;
#define LVGL_STACK_SIZE 1024*2
#define LVGL_TASK_PRIORITY 15

void lvgl_task(void *param)
{
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_18, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_gpio_set(gpio, GPIO_PIN_18);
    while(1)
    {
        lv_task_handler();
        vTaskDelay(3);
    }
}


void io2_set(uint8_t value)
{
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    if(value)
    {
        bflb_gpio_set(gpio, GPIO_PIN_2);
    }
    else
    {
        bflb_gpio_reset(gpio, GPIO_PIN_2);
    }
    
}

uint8_t wifi_connect(char *ssid, char *passwd)
{
    int ret = 255;
	// struct fhost_vif_ip_addr_cfg ip_cfg = {0};

	if(NULL==ssid || 0==strlen(ssid)){
		return 1;
	}

	if(wifi_mgmr_sta_state_get() == 1){
		wifi_sta_disconnect();
	}
	if(wifi_sta_connect(ssid, passwd, NULL, NULL, 0, 0, 0, 1)){
		return 4;
	}

	//等待连接成功
	sta_ConnectStatus = 0;
	for(int i=0;i<10*30;i++){
		vTaskDelay(100);
		switch(sta_ConnectStatus){
			case CODE_WIFI_ON_CONNECTED:	//连接成功(表示wifi sta状态的时候表示同时获取IP(DHCP)成功，或者使用静态IP)
				return 0;
			case CODE_WIFI_ON_DISCONNECT:	//连接失败（超过了重连次数还没有连接成功的状态）
				wifi_sta_disconnect();
				return 4;
			case CODE_WIFI_ON_GOT_IP:	
				return 0;
			default:
				//等待连接成功
				break;
		}
	}

    return 14; //连接超时
}

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

extern void ui_event_BTN_BG1(lv_event_t * e);
extern void ui_event_BTN_BG2(lv_event_t * e);
extern void ui_event_BTN_BG3(lv_event_t * e);
#define button_PROCESS_STACK_SIZE  (1024)
#define button_PROCESS_PRIORITY (14)
static TaskHandle_t button_process_task_hd;
void button_process_task(void *param)
{
    static uint32_t press_10ms_cnt = 0;
    uint32_t press_mode = 0;
    static bool led_ctrl = 0;
    static bool button = 1;
    static uint32_t btn_cnt = 0;
    uint8_t *pBuf = NULL;
    uint32_t len;
    uint8_t *pStr2 = NULL;
    uint32_t len2;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_init(gpio, GPIO_PIN_16, GPIO_OUTPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_reset(gpio, GPIO_PIN_16);
    while(1){
        if(bflb_gpio_read(gpio, GPIO_PIN_2) == 1) {
            press_10ms_cnt++;
        }
        else if(bflb_gpio_read(gpio, GPIO_PIN_2) == 0){
            if(press_10ms_cnt > 6 && press_10ms_cnt < 100){
                press_mode = 1;
            }
            else if(press_10ms_cnt > 100){
                press_mode = 2;
            }
            press_10ms_cnt = 0;
        }

        if(1 == press_mode){
            press_mode = 0;
            printf("1 == press_mode\r\n");
            btn_cnt++;
            switch(btn_cnt%3)
            {
                case 0: ui_CLICKED_BTN_BG1();
                    break;
                case 1: ui_CLICKED_BTN_BG2();
                    break;
                case 2: ui_CLICKED_BTN_BG3();
                    break;
                default: ui_CLICKED_BTN_BG1();
                    break;
            }
        }
        else if(2 == press_mode){
            press_mode = 0;
            led_ctrl = !led_ctrl;
            printf("led_ctrl:%d\r\n", led_ctrl);
            // if(led_ctrl){
            //     bflb_gpio_set(gpio, GPIO_PIN_16);
            // }
            // else{
            //     bflb_gpio_reset(gpio, GPIO_PIN_16);
            // }
        }

        vTaskDelay(10);
    }
}

// lv_ui guider_ui;
int main(void)
{
    board_init();

    uart0 = bflb_device_get_by_name("uart0");
    shell_init_with_task(uart0);

    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    lwip_sntp_init();


    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    // lv_port_indev_init();

    // printf("lv_demo_benchmark\r\n");
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_widgets();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    ui_init();
    // setup_ui(&guider_ui);
    // custom_init(&guider_ui);
    
    printf("lv_task_handler\r\n");
    // lv_task_handler();

    printf("lvgl success\r\n");
    xTaskCreate(lvgl_task, (char *)"lvgl", LVGL_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, &lvgl_TaskHandle);
    xTaskCreate(button_process_task, (char *)"button_proc_task", button_PROCESS_STACK_SIZE, NULL, button_PROCESS_PRIORITY, &button_process_task_hd);
    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    vTaskStartScheduler();
    // while (1) {
    //     lv_task_handler();
    //     bflb_mtimer_delay_ms(1);
    // }

    while (1) {
    }
}

