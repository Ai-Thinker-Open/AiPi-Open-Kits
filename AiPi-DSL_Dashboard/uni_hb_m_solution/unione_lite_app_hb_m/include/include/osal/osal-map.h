/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Li Dapeng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_MAP_H_
#define OSAL_OSAL_MAP_H_

#include "osal/osal-rbtree.h"
#include "osal/osal-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OSAL_MAP_KEY_TYPE_DEFINE */
typedef enum {
  KEY_TYPE_INT8 = 0,
  KEY_TYPE_INT16,
  KEY_TYPE_INT32,
  KEY_TYPE_INT64,
  KEY_TYPE_POINTER
} OsalMapKeyType;

typedef union {
  uint8_t value_8;
  uint16_t value_16;
  uint32_t value_32;
  uint64_t value_64;
  intptr_t value_ptr;
  float value_float;
  double value_double;
} OsalMapValue;

typedef union {
  uint8_t key_int8;
  uint16_t key_int16;
  uint32_t key_int32;
  uint64_t key_int64;
  intptr_t key_pointer;
} OsalMapKey;

typedef struct {
  struct rb_node rb_node;
  uint32_t key_length;
  OsalMapKey key;
  OsalMapValue data;
} OsalMapNode;

typedef int32_t (*KeyCompare)(void* key1, uint32_t key1_length, void* key2,
                              uint32_t key2_length);

typedef struct {
  struct rb_root root;
  KeyCompare compare;
  uint32_t key_type;
  uint32_t node_number;
} OsalMap;

/*
 * @Description: 创建 Map
 * @Input params: key_tpye：key 的类型
 * @Output params: Map 句柄
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapCreate(OsalMap** map, OsalMapKeyType key_type);

/*
 * @Description: 插入节点
 * @Input params: map: 句柄
 *                key：节点的 key
 *                key_length：key 所占内存长度
 *                data：节点数据
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapInsert(OsalMap* map, void* key, uint32_t key_length,
                      OsalMapValue data);

/*
 * @Description: 根据 key 搜索节点
 * @Input params: map: 句柄
 *                key：搜索的 key
 *                key_length：key 所占内存长度
 * @Output params: data：节点数据
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapSearch(const OsalMap* map, void* key, uint32_t key_length,
                      OsalMapValue* data);

/*
 * @Description: 删除 key 指向的节点
 * @Input params: map: 句柄
 *                key：删除的节点key
 *                key_length：key 所占内存长度
 * @Output params: data：删除节点的数据
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapDelete(OsalMap* map, void* key, uint32_t key_length,
                      OsalMapValue* data);

/*
 * @Description: 替换 Map 中的节点
 * @Input params: map: 句柄
 *                key：替换的 key
 *                key_length：key 所占内存长度
 *                replace_data：节点数据
 * @Output params: data：替换前节点的数据
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapReplace(OsalMap* map, void* key, uint32_t key_length,
                       OsalMapValue replace_data, OsalMapValue* data);

/*
 * @Description: 释放 Map
 * @Input params: map: 句柄
 * @Output params: 无
 * @Return: 成功：OSAL_OK
 *          失败：OSAL_FAILED
 */
int32_t OsalMapRelease(OsalMap* map);

/*
 * @Description: 获取 Map 中节点个数
 * @Input params: map: 句柄
 * @Output params: 无
 * @Return: 成功：节点个数
 *          失败：OSAL_FAILED
 */
int32_t OsalMapGetSize(OsalMap* map);

/*
 * @Description: 获取 Map 中首个节点
 * @Input params: map: 句柄
 * @Output params: 无
 * @Return: 成功：首个节点
 *          失败：NULL
 */
OsalMapNode* OsalMapGetFirstNode(OsalMap* map);

/*
 * @Description: 获取当前节点的下一个节点
 * @Input params: cur_node：当前节点
 * @Output params: 无
 * @Return: 成功：下一个节点
 *          失败：NULL
 */
OsalMapNode* OsalMapGetNextNode(OsalMapNode* cur_node);

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_MAP_H_
