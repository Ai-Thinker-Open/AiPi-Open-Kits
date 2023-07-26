#ifndef __RESET_HAL_H__
#define __RESET_HAL_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

typedef enum wakeup_type_t{
	WAKEUP_BY_POWERON = 0,
	WAKEUP_BY_RESET_PIN,
	WAKEUP_BY_WATCHDOG,
	WAKEUP_BY_LVD,
	WAKEUP_BY_SYSTEM_RESET,
	WAKEUP_BY_UNKNOWN
}wakeup_type;

extern void uni_hal_reset_system(void);
extern wakeup_type uni_hal_get_boot_status(void);
extern void uni_hal_clear_boot_status(void);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__RESET_HAL_H__