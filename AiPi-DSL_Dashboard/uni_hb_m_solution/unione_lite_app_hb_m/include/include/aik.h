/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All Rights Reserved.
 */

#ifndef INCLUDE_AIK_H_
#define INCLUDE_AIK_H_

#include "include/aik-event.h"
#include "include/aik-mode.h"

#ifndef _WIN32
#define AIK_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
extern "C" {
#endif
#else
#ifdef DLL_EXPORT
#define AIK_EXPORT extern "C" __declspec(dllexport)
#else
#define AIK_EXPORT extern "C" __declspec(dllimport)
#endif
#endif
/* AIK handle, 一个 handle 对应一个特定的 KIT */
typedef void* AikHandle;

/*
 * @Description: 事件上报回调，由用户实现，可以获取到 AIK
 * 的各种状态和数据
 * @Input params: event: 事件 id, 见 aik-event.h
 *                content: 数据，用户使用后需要释放内存
 * @Output params:
 * @Return:
 */
typedef void (*EventCallback)(AikEvent event, char* content);

/*
 * @Description: 数据上报回调，由用户实现，可以获取AIK处理后的
 *               数据（如降噪数据）
 * @Input params: event: 事件 id，见 aik-event.h
 *                data: 数据流
 *                size: data长度
 *                index: 数据块编号
 * @Output params:
 * @Return:
 */
typedef void (*DataCallback)(AikEvent event, void* data, int size, int index);

/*
 * @Description: 初始化
 * @Input params: config_json: JSON 格式配置文件，一个 JSON 生成一个 handle
 *                event_callback: 事件上报回调
 *                data_callback: 数据流上报回调（TTS）
 * @Output params:
 * @Return: 成功：AikHandle
 *          失败：NULL
 */
AIK_EXPORT AikHandle UalAikInit(const char* config_json,
                                EventCallback event_callback,
                                DataCallback data_callback);

/*
 * @Description: 设置用户输入参数（如动态修改的唤醒词语法）
 * @Input params: handle: AIK handle
 *                id: AIK_ID_XXX (aik-id.h)
 *                args: 参数
 * @Output params:
 * @Return: 成功：AIK_OK
 *          失败：AIK_FAILED
 */
AIK_EXPORT int UalAikSet(AikHandle handle, int id, void* args);

/*
 * @Description: 获取用户配置过的参数（预留接口，暂无用处）
 * @Input params: handle: aik handle
 *                id: AIK_ID_XXX (aik-id.h)
 * @Output params: args: 参数值
 * @Return: 成功：AIK_OK
 *          失败：AIK_FAILED
 */
AIK_EXPORT int UalAikGet(AikHandle handle, int id, void* args);

/*
 * @Description: 启动
 * @Input params: handle: AikHandle
 *                mode: 运行模式
 * @Output params:
 * @Return: 成功：AIK_OK
 *          失败：AIK_FAILED
 */
AIK_EXPORT int UalAikStart(AikHandle handle, AikMode mode);

/*
 * @Description: 停止，多用于打断
 * @Input params: handle: AikHandle
 *                mode: 运行模式（用来区分打断对象）
 * @Output params:
 * @Return: 成功：AIK_OK
 *          失败：AIK_FAILED
 */
AIK_EXPORT int UalAikStop(AikHandle handle, AikMode mode);

/*
 * @Description:
 * @Input params:
 * @Output params:
 * @Return:
 */
AIK_EXPORT void UalAikRelease(AikHandle handle);

/***************************************/
/* 以下接口需要用户实现                */
/***************************************/

/*
 * @Description: 获取网络通断状态
 * @Input params:
 * @Output params:
 * @Return: 网络畅通：AIK_NETWORK_OK
 *          网络不通：AIK_NETWORK_ERROR
 */
AIK_EXPORT int UalAikGetNetworkStatus();

/*
 * @Description: 获取系统当前工作模式，低功耗多级唤醒等场景使用
 * @Input params:
 * @Output params:
 * @Return: 正常模式：AIK_SYSTEM_NORMAL
 *          低功耗模式：AIK_SYSTEM_LOWPOWER
 *          失败：AIK_FAILED
 */
AIK_EXPORT int UalAikGetSystemStatus();

/*
 * @Description: 获取设备授权信息
 * @Input params:
 * @Output params: appkey: 项目申请
 *                 udid：项目授权
 *                 token: 在线服务授权 token
 * @Return:
 */
AIK_EXPORT int UalAikGetAuthInfo(char** appkey, char** udid, char** token);

/*
 * @Description: 获取设备 Mac 信息
 * @Input params:
 * @Output params: wifi：
 *                 voice：
 * @Return:
 */
AIK_EXPORT int UalAikGetDevMac(char** wifi, char** voice);

/*
 * @Description: 获取设备 IP 和端口号
 * @Input params:
 * @Output params: ipstr: IP 地址
 *                 port：端口号
 * @Return:
 */
AIK_EXPORT int UalAikGetIPAndPort(char** ipstr, int* port);

/*
 * @Description: 获取 AIK 版本号
 * @Input params:
 * @Output params:
 * @Return:
 */
AIK_EXPORT const char* UalAikGetVersion();

#ifndef _WIN32
#ifdef __cplusplus
}
#endif
#endif

#endif  // INCLUDE_AIK_H_
