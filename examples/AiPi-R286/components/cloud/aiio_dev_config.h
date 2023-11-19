/**
 * @file aiio_dev_config.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
*/

#ifndef AIIO_DEV_CONFIG_H
#define AIIO_DEV_CONFIG_H

/* 开发平台创建产品后生成的产品ID，用户可根据创建不同产品而获得不同的产品ID，可在该处进行修改*/
#define  PRODUCT_ID                "PKWTd3mA"

/* 产品标识,该标识是用户在创建不同产品时将自定义的产品标识 */
#define  PRODUCT_FLAG              "AiPi"
/* 设备客户端ID，对应三元组的 设备ID */
#define DEVICE_CLIENT_ID            "78HW5VrlTOGn0c"
/* 设备用户名，对应三元组的用户 */
#define DEVICE_USER_NAME            "gZh2PT"
/* 设备密码，对应三元组的密码  */
#define DEVICE_PASSWD               "fs7NjJq3bCn4dp"  

//============================ 空净物模型数据定义 ==================================================
//开关(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_SWALL             1
//档位设定(可下发可上报 rw)
//备注:枚举值（Enum）
#define CMD_SW2              4
#define CMD_SW3              5
#define CMD_SW1              101

#endif