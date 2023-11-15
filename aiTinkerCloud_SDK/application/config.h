/**
 * @brief   This file describe the data of product dpid
 * 
 * @file    config.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note 
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-07-03          <td>1.0.0            <td>zhuolm             <td> Product dpid data
 */
#ifndef __CONFIG_H_
#define __CONFIG_H_



/* 开发平台创建产品后生成的产品ID，用户可根据创建不同产品而获得不同的产品ID，可在该处进行修改*/
#define  PRODUCT_ID                         "PKEVvgUr"

/* 产品标识,该标识是用户在创建不同产品时将自定义的产品标识 */
#define  PRODUCT_FLAG                       "kxx"


//============================ 空净物模型数据定义 ==================================================


//开关(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_POWERSTATE              1
//档位设定(可下发可上报 rw)
//备注:枚举值（Enum）
#define CMD_SPRAY_MODE              2
//灯光开关(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_LIGHT_SWITCH              4
//灯光亮度(可下发可上报 rw)
//备注:数值型（Int）
#define CMD_LIGHT_BRIGHTNESS              5
//灯光模式(可下发可上报 rw)
//备注:	枚举值（Enum）
#define CMD_LIGHT_MODE              6
//灯光内置模式(可下发可上报 rw)
//备注:	数值型（Int）
#define CMD_LIGHT_BUILTIN              7
//清洗时长(可下发可上报 rw)
//备注:	数值型（Int）
#define CMD_CLEAN_TIME              8
//清洗复位(只下发（w）)
//备注:	布尔值（Bool）
#define CMD_CLEAN_RESET              9
//清洗提示开关(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_CLEAN_TIP_SWITCH              10
//清洗提示(只上报（ro）)
//备注:	布尔值（Bool）
#define CMD_CLEAN_TIP              11
//缺油提示(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_OIL_TIP              12
//缺水提示(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_WATER_TIP              13
//息屏时长(可下发可上报 rw)
//备注:	数值型（Int）
#define CMD_REST_SCREEN_TIME              14
//息屏开关(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_REST_SCREEN_SWITCH              15
//低电量提示(可下发可上报 rw)
//备注:布尔值（Bool）
#define CMD_LOW_BATTERY_TIP              16
//剩余电量比率(只上报（ro）)
//备注:数值型（Int）
#define CMD_REMAIN_ELECTRICITY              17
//剩余水量比率(只上报（ro）)
//备注:	数值型（Int）
#define CMD_REMAIN_WATER              18
//香味选择(可下发可上报 rw)
//备注:	枚举值（Enum）
#define CMD_AROMA              19
//音乐开关(可下发可上报 rw)
//备注:枚举值（Enum）
#define CMD_PLAY_SWITCH              20
//音乐歌曲(可下发可上报 rw)
//备注:	数值型（Int）
#define CMD_PLAY_MUSIC              21
//音乐列表(只上报（ro）)
//备注:枚举值（Enum）
#define CMD_MUSIC_LIST              22
//播放列表(可下发可上报 rw)
//备注:字符型（String）
#define CMD_PLAY_LIST              23
//音乐音量(可下发可上报 rw)
//备注:数值型（Int）
#define CMD_PLAY_VOLUME              24
//音乐播放模式(可下发可上报 rw)
//备注:枚举值（Enum）
#define CMD_PLAY_MODE              25
//音乐切换(可下发可上报 rw)
//备注:枚举值（Enum）
#define CMD_PLAY_NEXT              26

#endif
