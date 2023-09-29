#ifndef __MODULE_WIRELESS_H_
#define __MODULE_WIRELESS_H_
// #include "chip_include.h"
#include "stdio.h"
#ifdef __MODULE_WIRELESS_C_
#define MODULE_WIRELESS_EXT
#else
#define MODULE_WIRELESS_EXT extern
#endif

// 数据类型
typedef struct
{
    uint16_t encoding_low;  // 编码低，实际延迟时间为：encoding_low * module_wireless_delay
    uint16_t encoding_high; // 编码高，实际延迟时间为：encoding_high * module_wireless_delay
    uint16_t direction;     // 高低电平方向      0  低电平启动    1 高电平启动
} module_wireless_dat_t;

typedef struct
{
    module_wireless_dat_t module_wireless_nec_start;
    module_wireless_dat_t module_wireless_nec_zero;
    module_wireless_dat_t module_wireless_nec_one;
} module_wireless_nec_dat_t;
/*
    接收配置结构体
*/
typedef struct
{
    uint8_t module_wireless_receive_start_flag;       // 0 没有启动接收   1 启动接收
    uint8_t module_wireless_receive_start_direction;  // 触发方向 0 下降沿先触发  1 上升沿先触发
    uint16_t module_wireless_receive_encoding_offset; // 编码偏移,用于识别过程中的上下偏移，提高识别率
    uint16_t module_wireless_receive_waiting_time;    // 启动开始之后的等待时间
    uint16_t module_wireless_receive_waiting_count;   // 不需要配置，启动开始之后的等待时间计数器
    uint32_t module_wireless_receive_low_level_time;  // 不需要配置
    uint32_t module_wireless_receive_high_level_time; // 不需要配置
    uint32_t module_wireless_receive_bench_time;      // 不需要配置

    module_wireless_dat_t *module_wireless_receive_buf;
    uint16_t module_wireless_receive_buf_count;

    uint32_t (*module_wireless_receive_config_tick)(void);
} module_wireless_receive_config_t;

/*
    发送配置结构体
*/
typedef struct
{
    void (*module_wireless_send_config_delay)(void);
    void (*module_wireless_send_config_38k_delay)(void);
    void (*module_wireless_send_config_write)(uint8_t status);
} module_wireless_send_config_t;

// 接收数据结构体
typedef struct module_wireless_receive_type
{
    module_wireless_receive_config_t *module_wireless_receive_config;

    int8_t (*module_wireless_receive_falling)(struct module_wireless_receive_type *this); // 下降沿触发
    int8_t (*module_wireless_receive_rising)(struct module_wireless_receive_type *this);  // 上升沿触发
    int8_t (*module_wireless_receive_nec_analysis)(struct module_wireless_receive_type *this, module_wireless_nec_dat_t *module_wireless_nec_dat, module_wireless_dat_t *module_wireless_receive_dat, uint16_t lenght, uint8_t *buf);
    int8_t (*module_wireless_receive_read)(struct module_wireless_receive_type *this, module_wireless_dat_t *module_wireless_receive_dat, uint16_t *lenght);
} module_wireless_receive_type_t;

// 发送数据结构体
typedef struct module_wireless_send_type
{
    module_wireless_send_config_t *module_wireless_send_config;

    int8_t (*module_wireless_send_38k_low_write_start)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat);
    int8_t (*module_wireless_send_38k_low_write)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat);
    int8_t (*module_wireless_send_38k_low_write_byte)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat_zero, module_wireless_dat_t *module_wireless_send_dat_one, uint8_t dat);

    int8_t (*module_wireless_send_write_start)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat);
    int8_t (*module_wireless_send_write)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat);
    int8_t (*module_wireless_send_write_byte)(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat_zero, module_wireless_dat_t *module_wireless_send_dat_one, uint8_t dat);
} module_wireless_send_type_t;

MODULE_WIRELESS_EXT int8_t module_wireless_send_create(module_wireless_send_type_t *this, module_wireless_send_config_t *module_wireless_send_config);
MODULE_WIRELESS_EXT int8_t module_wireless_receive_create(module_wireless_receive_type_t *this, module_wireless_receive_config_t *module_wireless_receive_config);

#endif
