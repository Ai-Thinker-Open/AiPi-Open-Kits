#ifndef UTILS_LIST_INC_LIST_HEAD_H_
#define UTILS_LIST_INC_LIST_HEAD_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uni_list_head {
  struct uni_list_head *next, *prev;
} uni_list_head;

#define UNI_LIST_HEAD_INIT(obj) { &(obj), &(obj) }

/* For the entry to be added , INIT is not required */
#define __UNI_LIST_ADD(entry,before,after) {uni_list_head *new_= (entry), *prev = (before), *next = (after); (next)->prev = (new_); (new_)->next = (next); (new_)->prev = (prev); (prev)->next = (new_);}

/* void uni_list_init(struct uni_list_head *entry) */
#define uni_list_init(entry) do {(entry)->next = (entry); (entry)->prev = (entry);} while(0)

/* void uni_list_add(struct uni_list_head *entry, struct uni_list_head *base); */
#define uni_list_add(entry,base) do { __UNI_LIST_ADD((entry),(base),(base)->next); } while(0)

/* void uni_list_add_after(struct uni_list_head *entry, struct uni_list_head *base); */
#define uni_list_add_after(entry,base) do { __UNI_LIST_ADD((entry),(base),(base)->next); } while(0)

/* void uni_list_add_before(struct uni_list_head *entry, struct uni_list_head *base); */
#define uni_list_add_before(entry,base) do { __UNI_LIST_ADD((entry),(base)->prev,(base)); } while(0)

/* void uni_list_add_head(struct uni_list_head *entry, struct uni_list_head *head); */
#define uni_list_add_head(entry,head) uni_list_add_after(entry,head)
/* void uni_list_add_tail(struct uni_list_head *entry, struct uni_list_head *head); */
#define uni_list_add_tail(entry,head) uni_list_add_before(entry,head)

/* void uni_list_del(struct uni_list_head *entry); */
#define uni_list_del(entry) do { (entry)->prev->next = (entry)->next; (entry)->next->prev = (entry)->prev; (entry)->next = (entry)->prev = (entry);} while(0)

/*  int uni_list_empty(struct uni_list_head *head) */
#define uni_list_empty(head) ((head)->next == (head))

int uni_list_count(uni_list_head *head);

/* struct uni_list_head *
   uni_list_get_head(struct uni_list_head *head); */
#define uni_list_get_head(head) (uni_list_empty(head) ? (uni_list_head*)NULL : (head)->next)
/* struct uni_list_head *
   uni_list_get_tail(struct uni_list_head *head); */
#define uni_list_get_tail(head) (uni_list_empty(head) ? (uni_list_head*)NULL : (head)->prev)

#define uni_list_is_head(entry, head) ((entry)->prev == head)
#define uni_list_is_tail(entry, head) ((entry)->next == head)

/* void uni_list_enqueue(struct uni_list_head *entry, struct uni_list_head *head) */
#define uni_list_enqueue uni_list_add_tail

uni_list_head* uni_list_dequeue(uni_list_head *head);

/* void uni_list_push(struct uni_list_head *entry, struct uni_list_head *head) */
#define uni_list_push uni_list_add_tail

/* struct uni_list_head* uni_list_pop(struct uni_list_head *head); */
uni_list_head* uni_list_pop(uni_list_head *head);

#define uni_list_entry(ptr, type, member) \
  ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define uni_list_for_each(h, head) \
  for (h = (head)->next; h != (head); h = h->next)

#define uni_list_for_each_safe(h, n, head) \
  for (h = (head)->next, n = h->next; h != (head);  h = n, n = h->next)

#define uni_list_for_each_entry(p, head, type, member) \
  for (p = uni_list_entry((head)->next, type, member); (uni_list_head *)&p->member != (head); p = uni_list_entry(p->member.next, type, member))

#define uni_list_for_each_entry_safe(p, t, head, type, member) \
  for (p = uni_list_entry((head)->next, type, member), t = uni_list_entry(p->member.next, type, member); (uni_list_head *)&p->member != (head); p = t, t = uni_list_entry(t->member.next, type, member))

#define uni_list_for_each_prev(h, head) \
  for( (h) = (head)->prev; (h) != (head); (h) = (h)->prev)

#define uni_list_for_each_prev_safe(h, t, head) \
  for( (h) = (head)->prev, (t) = (h)->prev; (h) != (head); (h) = (t), (t) = (h)->prev)

#define uni_list_get_head_entry(head, type, member) (uni_list_empty(head) ? (type*)NULL : uni_list_entry(((head)->next), type, member))
#define uni_list_get_tail_entry(head, type, member) (uni_list_empty(head) ? (type*)NULL : uni_list_entry(((head)->prev), type, member))

#ifdef __cplusplus
}
#endif
#endif
