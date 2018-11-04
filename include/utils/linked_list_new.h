#ifndef MY_LINKED_LIST_NEW_H
#define MY_LINKED_LIST_NEW_H

#include "../my_malloc.h"
#include "utils.h"

#define __define_ll(node_t, head_t, key_t, val_t, name)\
\
typedef struct node_t {\
  item_t val;\
  struct node_t *next;\
} node_t;\
\
typedef struct head_t  {\
  node_t *head;\
  node_t *tail;\
} head_t;\
\
void __ll_init(head_t** init_head) {\
  head_t *head = (head_t *)_lib_malloc(sizeof(head_t));\
  head->head = NULL;\
  head->tail = NULL;\
  *init_head = head;\
}\
\
void ll_insert_head(head_t *head, item_t item) {\
  node_t *new_node = (node_t*)_lib_malloc(sizeof(node_t));\
  head->head = new_node;\
  new_node->val = item;\
  new_node->next = head->head;\
  if (head->tail == NULL)\
    head->tail = new_node;\
}\
\
void ll_push_tail(head_t *head, item_t item) {\
  node_t *new_node = (node_t*)_lib_malloc(sizeof(node_t));\
  head->tail->next = new_node;\
  head->tail = new_node;\
  new_node->val = item;\
  new_node->next = NULL;\
}\
\
int ll_pop_head(head_t *head, item_t *item) {\
  if (head->head == NULL)\
    return -1;\
  *item = head->head->val;\
  node_t *node = head->head;\
  head->head = head->head->next;\
  _lib_free(node);\
  if (head->head == NULL)\
    head->tail = NULL;\
  return 1;\
}\

#define define_llist(name, item_t)\
  __define_ll(__ll_node_##name, __ll_head_##name, item_t, name)
#define ll_head_t(name) __ll_head_##name*
#define ll_node_t(name) __ll_node_##name
#define ll_h_init(head) __ll_init(&head)


// Example:
// define_llist(my_map, int, int);
// lm_head_t(my_map) map;
// lmp_insert(map, 1, 1);
// int a;
// lmp_pop(map, 1, &a);  # return 1, a == 1
//
#endif /* ifndef MY_LINKED_MAP_NEW_H */
