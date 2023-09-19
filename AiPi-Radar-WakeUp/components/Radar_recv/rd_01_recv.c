/**
 * @file Rd-01_recv.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "log.h"
#include "bflb_gpio.h"
#include "rd_01_recv.h"
#include "custom.h"
#define DBG_TAG "Rd-01"

#define RD01_DET_TIME 1*10      //有人检测计时1S 
#define RD01_NDET_TIME 20*10    //有人检测计时20S 

xTimerHandle rd_01_det_time;
void* rd_01_nodet_time;

int rd_01_detected = 0;

struct bflb_device_s* gpio;

static void gpio_isr(int irq, void* arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static int i = 0;
    bool intstatus = bflb_gpio_get_intstatus(gpio, RD_01_IO);
    if (intstatus) {
        bflb_gpio_int_clear(gpio, RD_01_IO);
        bool Rd_01_status = bflb_gpio_read(gpio, RD_01_IO);
        if (Rd_01_status) {
            LOG_I("Rd-01  detected by someone");
            if (xTimerIsTimerActive(rd_01_nodet_time)!=pdFALSE)
                xTimerStopFromISR(rd_01_nodet_time, &xHigherPriorityTaskWoken);

            if (xTimerIsTimerActive(rd_01_det_time)==pdFALSE)
                xTimerResetFromISR(rd_01_det_time, &xHigherPriorityTaskWoken);
        }
        else {
            LOG_I("Rd-01 No one detected");
            if (xTimerIsTimerActive(rd_01_det_time)!=pdFALSE)
                xTimerStopFromISR(rd_01_det_time, &xHigherPriorityTaskWoken);

            if (xTimerIsTimerActive(rd_01_nodet_time)==pdFALSE)
                xTimerResetFromISR(rd_01_nodet_time, &xHigherPriorityTaskWoken);
            // timer_count_det = 0;
        }
    }
}
/**
 * @brief rd_01_detected_time_cb
 *       软件 定时器回调
 * @param xtimer
*/
static void rd_01_detected_time_cb(xTaskHandle xtimer)
{
    uint8_t timer_id = pvTimerGetTimerID(xtimer);

    if (timer_id) {
        LOG_I("Rd-01  detected by someone set PC Sleep");
        if (!rd_01_detected)
            xTaskNotify(custom_status_task, CUSTOM_STATE_RADAR_NDET, eSetValueWithOverwrite);

    }
    else {
        LOG_I("Rd-01  detected by someone set PC Wake up");
        xTaskNotify(custom_status_task, CUSTOM_STATE_RADAR_DET, eSetValueWithOverwrite);

    }
    rd_01_detected = 0;
}

void Rd_01_recv_init(void)
{
    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_int_init(gpio, RD_01_IO, GPIO_INT_TRIG_MODE_SYNC_FALLING_RISING_EDGE);
    bflb_gpio_int_mask(gpio, RD_01_IO, false);

    rd_01_det_time = xTimerCreate("rd-01det", pdMS_TO_TICKS(RD01_DET_TIME*100), pdFALSE, 0, rd_01_detected_time_cb);
    rd_01_nodet_time = xTimerCreate("rd-01 no det", pdMS_TO_TICKS(guider_ui.timerout*1000), pdFALSE, 1, rd_01_detected_time_cb);

    bflb_irq_attach(gpio->irq_num, gpio_isr, gpio);
    bflb_irq_enable(gpio->irq_num);


}