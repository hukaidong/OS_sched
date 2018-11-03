#include "include/type.h"
#include "include/my_malloc.h"

void insert(node_p &head_ref, int key, int val){
  node_p new_node = (node_p)_lib_malloc(sizeof(node));
  new_node->key = key;
  new_node->val = val;
  new_node->next = head_ref;
  head_ref = new_node;
}

int pop(node_p &head, int key, int &val){
  node_p *last = &head, current = head;
  while (current != NULL) {
    if (current->key == key) {
      val = current->val;
      *last = current->next;
      _lib_free(current);
      return 1;
    }
    last = &current->next;
    current = current->next;
  }
  return -1;
}
