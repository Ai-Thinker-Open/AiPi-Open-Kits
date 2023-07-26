#ifndef __WATCHDOG_HAL_H__
#define __WATCHDOG_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

/**
 *  定义看门狗的复位周期
 */
typedef enum _WDG_STEP_SEL
{
    WDG_STEP_1S = 0,/**<看门狗的复位周期:1S*/
    WDG_STEP_3S = 1,/**<看门狗的复位周期:3S*/
    WDG_STEP_4S = 2 /**<看门狗的复位周期:4S*/
} WDG_STEP_SEL;

extern void uni_hal_watchdog_enable(WDG_STEP_SEL Mode);
extern void uni_hal_watchdog_disable(void);
extern void uni_hal_watchdog_feed(void);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__WATCHDOG_HAL_H__
