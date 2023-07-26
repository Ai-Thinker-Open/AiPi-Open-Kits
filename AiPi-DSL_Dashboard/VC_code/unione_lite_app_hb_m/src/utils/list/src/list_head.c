#include "list_head.h"
#include <stdlib.h>

uni_list_head* uni_list_dequeue(uni_list_head *head) {
  uni_list_head *t;
  if (uni_list_empty(head)) {
    return (uni_list_head*) NULL;
  }
  t = head->next;
  uni_list_del(t);
  return t;
}

uni_list_head* uni_list_pop(uni_list_head *head) {
  uni_list_head *t;
  if (uni_list_empty(head)) {
    return (uni_list_head*) NULL;
  }
  t = head->prev;
  uni_list_del(t);
  return t;
}

int uni_list_count(uni_list_head *head) {
  int c = 0;
  uni_list_head *t;
  uni_list_for_each(t, head) {
    c++;
  }
  return c;
}
