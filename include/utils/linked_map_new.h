#ifndef MY_LINKED_MAP_NEW_H
#define MY_LINKED_MAP_NEW_H
#include "../my_malloc.h"
#include "utils.h"


#define __define_lmap(node_t, head_t, key_t, val_t, name)\
\
typedef struct node_t {\
  key_t key;\
  val_t val;\
  struct node_t *next;\
} node_t;\
\
typedef struct head_t  {\
  node_t *next;\
} head_t; \
\
void lmp_head_init(head_t head) {\
  head.next = NULL;\
}\
void lmp_insert(head_t head, key_t key, val_t val) {\
  node_t *new_node = (node_t*)_lib_malloc(sizeof(node_t));\
  new_node->key = key;\
  new_node->val = val;\
  new_node->next = head.next;\
  head.next = new_node;\
}\
\
int lmp_pop(head_t head, key_t key, val_t *val){\
  node_t **last = &head.next, \
         *current = head.next;\
  while (current != NULL) {\
    if (current->key == key) {\
      *val = current->val;\
      *last = current->next;\
      _lib_free(current);\
      return 1;\
    }\
    last = &current->next;\
    current = current->next;\
  }\
  return -1;\
}\

#define define_lmap(name, key_t, val_t)\
  __define_lmap(__node_##name, __head_##name, key_t, val_t, name)
#define lm_head_t(name) __head_##name


// Example:
// define_lmap(my_map, int, int);
// lm_head_t(my_map) map;
// lmp_insert(map, 1, 1);
// int a;
// lmp_pop(map, 1, &a);  # return 1, a == 1
//
#endif /* ifndef MY_LINKED_MAP_NEW_H */
