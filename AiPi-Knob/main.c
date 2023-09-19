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
#include "bflb_pwm_v2.h"
#include "bflb_clock.h"
#include "bflb_ir.h"

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

#include "lv_example_pub.h"
#include "ir_send.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WIFI_STACK_SIZE     (1536)
#define TASK_PRIORITY_FW    (16)

#define LVGL_STACK_SIZE 1024*4
#define LVGL_TASK_PRIORITY 3
static TaskHandle_t lvgl_TaskHandle;


#define button_PROCESS_STACK_SIZE  (1024)
#define button_PROCESS_PRIORITY (8)
static TaskHandle_t button_process_task_hd;

#define IR_TX_STACK_SIZE  (1024)
#define IR_TX_PRIORITY (9)
// static TaskHandle_t ir_tx_task_hd;

#define IR_RX_STACK_SIZE  (1024)
#define IR_RX_PRIORITY (10)
static TaskHandle_t ir_recive_nec_task_hd;
/****************************************************************************
 * Private Types
 ****************************************************************************/


 /****************************************************************************
  * Private Data
  ****************************************************************************/

static struct bflb_device_s* uart0;
struct bflb_device_s* gpio;
#if 1
static TaskHandle_t wifi_fw_task;

static wifi_conf_t conf =
{
    .country_code = "CN",
};

static uint32_t sta_ConnectStatus = 0;



/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
extern void shell_init_with_task(struct bflb_device_s* shell);
extern int es8388_voice_init(void);
extern int audio_init(void);
extern int audio_pcm_init(void);
extern void play_voice_open(void);
extern void play_voice_close(void);
extern int es8388test_init(void);

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


void lvgl_task(void* param)
{
    // uint8_t *pBuf;

    // pBuf = malloc(1024 * 2000);
    // printf("pBuf:0x%x\r\n", pBuf);
    while (1)
    {
        lv_task_handler();
        vTaskDelay(2);
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
    if (wifi_sta_connect(ssid, passwd, NULL, NULL, 0, 0, 0, 1)) {
        return 4;
    }

    //等待连接成功
    sta_ConnectStatus = 0;
    for (int i = 0;i<10*30;i++) {
        vTaskDelay(100);
        switch (sta_ConnectStatus) {
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
void gpio26_isr(int irq, void* arg)
{
    static int i = 0;
    bool intstatus = bflb_gpio_get_intstatus(gpio, GPIO_PIN_26);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, GPIO_PIN_26);
        printf("%d\r\n", i++);
    }
}
void gpio28_isr(int irq, void* arg)
{
    static int i = 0;
    bool intstatus = bflb_gpio_get_intstatus(gpio, GPIO_PIN_28);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, GPIO_PIN_28);
        printf("%d\r\n", i++);
    }
}

int encoder_gpio_init(void)
{
    gpio = bflb_device_get_by_name("gpio");

    // bflb_gpio_int_init(gpio, GPIO_PIN_26, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE | GPIO_PULLUP);
    bflb_gpio_init(gpio, GPIO_PIN_26, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_int_mask(gpio, GPIO_PIN_26, false);

    // bflb_irq_attach(gpio->irq_num, gpio26_isr, gpio);
    // bflb_irq_enable(gpio->irq_num);

    // bflb_gpio_int_init(gpio, GPIO_PIN_28, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE | GPIO_PULLUP);
    bflb_gpio_init(gpio, GPIO_PIN_28, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_int_mask(gpio, GPIO_PIN_28, false);

    // bflb_irq_attach(gpio->irq_num, gpio28_isr, gpio);
    // bflb_irq_enable(gpio->irq_num);

    bflb_gpio_init(gpio, GPIO_PIN_24, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
}
extern lv_obj_t* get_current_layer_obj(void);
extern lv_group_t* get_encoder_group(void);
void button_process_task(void* param)
{
    static uint32_t io26_press_ms_cnt = 0;
    static uint32_t io28_press_ms_cnt = 0;
    static uint32_t btn_press_ms_cnt = 0;
    uint8_t press_mode = 0;
    uint8_t btn_press_mode = 0;
    static bool led_ctrl = 0;
    static bool button = 1;
    uint32_t frames = 1;
    uint8_t* pBuf = NULL;
    uint32_t len;
    uint8_t* pStr2 = NULL;
    uint32_t len2;
    bool long_press_flag = 0;


    gpio = bflb_device_get_by_name("gpio");
    // bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
    // bflb_gpio_init(gpio, GPIO_PIN_16, GPIO_OUTPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_reset(gpio, GPIO_PIN_16);

    while (1) {
        if (bflb_gpio_read(gpio, GPIO_PIN_26) == 0) {
            io26_press_ms_cnt++;
            if (bflb_gpio_read(gpio, GPIO_PIN_28) == 0 && io26_press_ms_cnt > 20) {
                press_mode = 1;
                io26_press_ms_cnt = 0;
            }
        }
        else if (bflb_gpio_read(gpio, GPIO_PIN_26) == 1) {
            // if(press_10ms_cnt > 10 && press_10ms_cnt < 100){
            //     press_mode = 1;
            // }
            // else if(press_10ms_cnt > 100){
            //     press_mode = 2;
            // }
            io26_press_ms_cnt = 0;
        }

        if (bflb_gpio_read(gpio, GPIO_PIN_28) == 0) {
            io28_press_ms_cnt++;
            if (bflb_gpio_read(gpio, GPIO_PIN_26) == 0 && io28_press_ms_cnt > 20) {
                press_mode = 2;
                io28_press_ms_cnt = 0;
            }
        }
        else if (bflb_gpio_read(gpio, GPIO_PIN_28) == 1) {
            io28_press_ms_cnt = 0;
        }
        //button
        if (bflb_gpio_read(gpio, GPIO_PIN_24) == 0) {
            btn_press_ms_cnt++;
            if (btn_press_ms_cnt > 500  && 0 == long_press_flag) {
                press_mode = 4;
                btn_press_ms_cnt = 0;
                long_press_flag = 1;
                printf("4 == press_mode0\r\n");
                // lv_obj_t *now_obj = get_current_layer_obj();
                lv_obj_t* now_obj = lv_group_get_focused(get_encoder_group());

                lv_event_send(now_obj, LV_EVENT_LONG_PRESSED, NULL);
            }
        }
        else if (bflb_gpio_read(gpio, GPIO_PIN_24) == 1) {

            if (btn_press_ms_cnt > 80 && btn_press_ms_cnt < 500 && 0 == long_press_flag) {
                press_mode = 3;
                printf("3 == press_mode\r\n");
            }
            if (1 == long_press_flag) {
                long_press_flag = 0;
            }
            // else if(btn_press_ms_cnt > 800){

            //     press_mode = 4;
            //     printf("4 == press_mode\r\n");
            //     // lv_obj_t *now_obj = get_current_layer_obj();
            //     lv_obj_t *now_obj = lv_group_get_focused(get_encoder_group());
            //     printf("now_obj:0x%x, light_2color:0x%x\r\n", now_obj, get_obj_light_2color());

            //     lv_event_send(now_obj, LV_EVENT_LONG_PRESSED, NULL);
            // }
            btn_press_ms_cnt = 0;
        }


        if (1 == press_mode) {
            encoder_handler(press_mode);
            printf("1 == press_mode\r\n");
        }
        else if (2 == press_mode) {
            encoder_handler(press_mode);
            printf("2 == press_mode\r\n");
        }
        else {
            encoder_handler(press_mode);
        }
        press_mode = 0;
        vTaskDelay(1);
    }
}
//pwm RGB led
void RGB_led_init(void)
{
    gpio = bflb_device_get_by_name("gpio");

    bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_29, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_30, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLDOWN | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_init(gpio, GPIO_PIN_29, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    // bflb_gpio_init(gpio, GPIO_PIN_30, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_25, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_set(gpio, GPIO_PIN_25);
    // bflb_gpio_reset(gpio, GPIO_PIN_27);
    // bflb_gpio_set(gpio, GPIO_PIN_29);
    // bflb_gpio_set(gpio, GPIO_PIN_30);
}
struct bflb_device_s* pwm;
//ch1--green, ch2--red, ch3--blue
int pwm_init(void)
{

    pwm = bflb_device_get_by_name("pwm_v2_0");

    /* period = .PBCLK / .clk_div / .period = 80MHz / 80 / 1000 = 1KHz */
    struct bflb_pwm_v2_config_s cfg = {
        .clk_source = BFLB_SYSTEM_PBCLK,
        .clk_div = 80,
        .period = 1000,
    };

    bflb_pwm_v2_init(pwm, &cfg);
    // bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH0, 0, 100); /* duty = (500-100)/1000 = 40% */
    // bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH1, 0, 500); /* duty = (400-200)/1000 = 20% */
    // bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, 0, 000); /* duty = (999-100)/1000 = 89.9% */
    // bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH3, 0, 300); /* duty = (500-0)/1000 = 50% */
    // bflb_pwm_v2_channel_positive_start(pwm, PWM_CH0);
    // // bflb_pwm_v2_channel_negative_start(pwm, PWM_CH0);
    // bflb_pwm_v2_channel_positive_start(pwm, PWM_CH1);
    // // bflb_pwm_v2_channel_negative_start(pwm, PWM_CH1);
    // bflb_pwm_v2_channel_positive_start(pwm, PWM_CH2);
    // // bflb_pwm_v2_channel_negative_start(pwm, PWM_CH2);
    // bflb_pwm_v2_channel_positive_start(pwm, PWM_CH3);
    // // bflb_pwm_v2_channel_negative_start(pwm, PWM_CH3);
    // bflb_pwm_v2_start(pwm);
}
void pwm_RGB_start(uint8_t red_percent, uint8_t green_percent, uint8_t blue_percent)
{

    bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, 0, red_percent * 10);
    bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH1, 0, green_percent * 10);
    bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH3, 0, blue_percent * 10);
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH2);
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH1);
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH3);
    bflb_pwm_v2_start(pwm);
}

#ifdef IR_RX_NEC
struct bflb_device_s* irrx;
#endif

void ir_recive_nec_task(void* param)
{
    printf("IR NEC RX task\r\n");
#ifdef IR_RX_NEC
    board_ir_gpio_init();

    uint64_t rx_data;
    uint8_t rx_len;
    struct bflb_ir_rx_config_s rx_cfg;

    irrx = bflb_device_get_by_name("irrx");

    /* RX init */
    rx_cfg.rx_mode = IR_RX_NEC;
    rx_cfg.input_inverse = true;
    rx_cfg.deglitch_enable = false;
    bflb_ir_rx_init(irrx, &rx_cfg);

    /* Enable rx, wait for sending */
    bflb_ir_rx_enable(irrx, true);
#endif

    while (1) {
#ifdef IR_RX_NEC
        /* Receive */
        rx_len = bflb_ir_receive(irrx, &rx_data);
        if (rx_len > 0) {
            printf("Receive bit: %d, value: 0x%016lx\r\n", rx_len, rx_data);

        }

#endif

        vTaskDelay(1);
    }
}

int main(void)
{
    board_init();
    // audio_init();
    audio_pcm_init();
    // es8388_voice_init();
    // es8388test_init();
    encoder_gpio_init();
    RGB_led_init();
    pwm_init();

    uart0 = bflb_device_get_by_name("uart0");
    shell_init_with_task(uart0);

    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    tcpip_init(NULL, NULL);
    wifi_start_firmware_task();
    // lwip_sntp_init();


    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    // printf("lv_demo_benchmark\r\n");
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_widgets();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // ui_init();
    ui_obj_to_encoder_init();
    lv_create_home(&boot_Layer);
    // lv_create_clock(&clock_screen_layer, TIME_ENTER_CLOCK_2MIN);
    lv_create_clock(&clock_screen_layer, 10000);

    printf("lvgl success\r\n");
    xTaskCreate(lvgl_task, (char*)"lvgl", LVGL_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, &lvgl_TaskHandle);
    xTaskCreate(button_process_task, (char*)"button_proc_task", button_PROCESS_STACK_SIZE, NULL, button_PROCESS_PRIORITY, &button_process_task_hd);
    // xTaskCreate(ir_send_system_task, (char *)"ir_tx_task", IR_TX_STACK_SIZE, NULL, IR_TX_PRIORITY, &ir_send_system_task_xhandle);
    // xTaskCreate(ir_recive_nec_task, (char *)"ir_recive_nec_task", IR_RX_STACK_SIZE, NULL, IR_RX_PRIORITY, &ir_recive_nec_task_hd);
    // printf("HeapSize:%d\r\n",xPortGetFreeHeapSize());
    vTaskStartScheduler();


    while (1) {
    }
}

