/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Hao Peng
 * All rights reserved.
 */

#ifndef OSAL_OSAL_LIST_H_
#define OSAL_OSAL_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 链表头 */
typedef struct OsalListHead {
  struct OsalListHead* next;
  struct OsalListHead* prev;
} OsalListHead;

/*
 * @Description: 初始化
 * @Input params: entry: OsalListHead*
 * @Output params: 无
 * @Return: 无
 */
#define OSAL_LIST_INIT(entry) list_init(entry)

/*
 * @Description: 增加节点
 * @Input params: entry: OsalListHead*
 *                base: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_ADD(entry, base) list_add(entry, base)

/*
 * @Description: 在链表头插入节点
 * @Input params: entry: OsalListHead*
 *                head: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_ADD_HEAD(entry, head) list_add_head(entry, head)

/*
 * @Description: 在链表尾插入节点
 * @Input params: entry: OsalListHead*
 *                head: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_ADD_TAIL(entry, head) list_add_tail(entry, head)

/*
 * @Description: 删除节点
 * @Input params: entry: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_DEL(entry) list_del(entry)

/*
 * @Description: 判空
 * @Input params: entry: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_EMPTY(entry) list_empty(entry)

/*
 * @Description: 获取头节点
 * @Input params: entry: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_GET_HEAD(entry) list_get_head(entry)

/*
 * @Description: 获取尾节点
 * @Input params: entry: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_GET_TAIL(entry) list_get_tail(entry)

/*
 * @Description: 判断节点是否为头
 * @Input params: entry: OsalListHead*
 *                head: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_IS_HEAD(entry, head) list_is_head(entry, head)

/*
 * @Description: 判断节点是否为尾
 * @Input params: entry: OsalListHead*
 *                head: OsalListHead*
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_IS_TAIL(entry, head) list_is_tail(entry, head)

/*
 * @Description: 获取节点结构体
 * @Input params: entry: OsalListHead*
 *                type: 结构体类型
 *                member：节点名
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_ENTRY(entry, type, member) list_entry(entry, type, member)

/*
 * @Description: 遍历链表，用于搜索查找，不含删除节点场景
 * @Input params: node: OsalListHead*
 *                head: OsalListHead*
 *                type: 结构体类型
 *                member: 节点名
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_FOR_EACH_ENTRY(node, head, type, member) \
  list_for_each_entry(node, head, type, member)

/*
 * @Description: 安全遍历，用于遍历过程中需要删除节点的场景
 * @Input params: node: OsalListHead*
 *                tmp_node: OsalListHead*
 *                head: OsalListHead*
 *                type: 结构体类型
 *                member: 节点名
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_FOR_EACH_ENTRY_SAFE(node, tmp_node, head, type, member) \
  list_for_each_entry_safe(node, tmp_node, head, type, member)

/*
 * @Description: 获取首节点结构
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_GET_HEAD_ENTRY(head, type, member) \
  list_get_head_entry(head, type, member)

/*
 * @Description: 获取尾节点结构
 * @Input params:
 * @Output params:
 * @Return:
 */
#define OSAL_LIST_GET_TAIL_ENTRY(head, type, member) \
  list_get_tail_entry(head, type, member)

/* 下面的代码拷贝自 linux 内核 list_head.h */
typedef OsalListHead list_head;
#define LIST_HEAD_INIT(obj) \
  { &(obj), &(obj) }

/* For the entry to be added , INIT is not required */
#define __LIST_ADD(entry, before, after)                          \
  {                                                               \
    list_head *new_ = (entry), *prev = (before), *next = (after); \
    (next)->prev = (new_);                                        \
    (new_)->next = (next);                                        \
    (new_)->prev = (prev);                                        \
    (prev)->next = (new_);                                        \
  }

/* void list_init(struct list_head *entry) */
#define list_init(entry)     \
  do {                       \
    (entry)->next = (entry); \
    (entry)->prev = (entry); \
  } while (0)

/* void list_add(struct list_head *entry, struct list_head *base); */
#define list_add(entry, base)                  \
  do {                                         \
    __LIST_ADD((entry), (base), (base)->next); \
  } while (0)

/* void list_add_after(struct list_head *entry, struct list_head *base); */
#define list_add_after(entry, base)            \
  do {                                         \
    __LIST_ADD((entry), (base), (base)->next); \
  } while (0)

/* void list_add_before(struct list_head *entry, struct list_head *base); */
#define list_add_before(entry, base)           \
  do {                                         \
    __LIST_ADD((entry), (base)->prev, (base)); \
  } while (0)

/* void list_add_head(struct list_head *entry, struct list_head *head); */
#define list_add_head(entry, head) list_add_after(entry, head)
/* void list_add_tail(struct list_head *entry, struct list_head *head); */
#define list_add_tail(entry, head) list_add_before(entry, head)

/* void list_del(struct list_head *entry); */
#define list_del(entry)                      \
  do {                                       \
    (entry)->prev->next = (entry)->next;     \
    (entry)->next->prev = (entry)->prev;     \
    (entry)->next = (entry)->prev = (entry); \
  } while (0)

/*  int list_empty(struct list_head *head) */
#define list_empty(head) ((head)->next == (head))

/* struct list_head *
        list_get_head(struct list_head *head); */
#define list_get_head(head) (list_empty(head) ? (list_head*)NULL : (head)->next)
/* struct list_head *
        list_get_tail(struct list_head *head); */
#define list_get_tail(head) (list_empty(head) ? (list_head*)NULL : (head)->prev)

#define list_is_head(entry, head) ((entry)->prev == head)
#define list_is_tail(entry, head) ((entry)->next == head)

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(type, member) __compiler_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t) & ((type*)0)->member)
#endif

#define list_entry(ptr, type, member) \
  ((type*)((char*)(ptr)-offsetof(type, member)))

#define list_for_each(h, head) for (h = (head)->next; h != (head); h = h->next)

#define list_for_each_safe(h, n, head) \
  for (h = (head)->next, n = h->next; h != (head); h = n, n = h->next)

#define list_for_each_entry(p, head, type, member)                       \
  for (p = list_entry((head)->next, type, member); &p->member != (head); \
       p = list_entry(p->member.next, type, member))

#define list_for_each_entry_safe(p, t, head, type, member) \
  for (p = list_entry((head)->next, type, member),         \
      t = list_entry(p->member.next, type, member);        \
       &p->member != (head);                               \
       p = t, t = list_entry(t->member.next, type, member))

#define list_for_each_prev(h, head) \
  for ((h) = (head)->prev; (h) != (head); (h) = (h)->prev)

#define list_for_each_prev_safe(h, t, head)                \
  for ((h) = (head)->prev, (t) = (h)->prev; (h) != (head); \
       (h) = (t), (t) = (h)->prev)

#define list_get_head_entry(head, type, member) \
  (list_empty(head) ? (type*)NULL : list_entry(((head)->next), type, member))

#define list_get_tail_entry(head, type, member) \
  (list_empty(head) ? (type*)NULL : list_entry(((head)->prev), type, member))

#ifdef __cplusplus
}
#endif

#endif  // OSAL_OSAL_LIST_H_
