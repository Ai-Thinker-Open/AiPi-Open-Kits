#ifndef __MODULE_BUTTON_H_
#define __MODULE_BUTTON_H_
#include "chip_include.h"
#ifndef __MODULE_BUTTON_C_
#define MODULE_BUTTON_EXT extern
#else
#define MODULE_BUTTON_EXT
#endif

/*
        按键配置规则
    1.如果不需要双击，请把module_button_contimer_value设置为空。
    2.单击、双击、长按使用的比例是：1 2 3
    3.按键值基准：每一个按键按下所输入的值是按键的基准。比如：按下按键输入3，那么这个按键的基准是三
    4.支持组合按键
*/
typedef struct
{
    uint8_t module_button_current_status;    // 不要适配，按键接口当前状态
    uint8_t module_button_last_status;       // 不要适配，按键接口上一个状态
    uint8_t module_button_process_flag;      // 不要适配，按键接口进程
    uint8_t module_button_pres_flag;         // 不要适配，当前按键状态标志
    uint16_t module_button_contimer_count;   // 不要适配，连续按下计数器
    uint16_t module_button_longtimer_count;  // 不要适配，长按时间计数器
    uint16_t *module_button_contimer_value;  // 需要适配，短按与双击的连续时间
    uint16_t *module_button_longtimer_value; // 需要适配，长按时间值
    uint16_t module_button_number;           // 需要适配，按键数量
    uint8_t (*module_button_statusin)(void); // 需要适配，按键输入引脚
} module_button_config_t;

// 功能
typedef struct module_button_type
{
    module_button_config_t *module_button_config;

    /*
        返回值：-1 参数错误  0 没有检测到按键     1 检测到按键
        获取按键值的函数要执行没10ms扫描一次，为按键提供一个时间片
        如果采用实时操作系统，那么按键处理的优先级要更高，是按键的功能达到稳定状态。
    */
    int8_t (*module_button_get_buttonvalue)(struct module_button_type *this, uint16_t *value);
} module_button_type_t;

MODULE_BUTTON_EXT int8_t module_button_create(module_button_type_t *this, module_button_config_t *module_button_config);

#endif
