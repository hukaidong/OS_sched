#ifndef MALLOC_PAGE_FILE_MAP
#define MALLOC_PAGE_FILE_MAP

#ifndef MOCK_MALLOC
#include "my_malloc.h"
#else
#include <stdlib.h>
#define _lib_malloc malloc
#define _lib_free free
#endif

#include "utils/utils.h"


struct sNode_s {
  ssize_t thread_id;
  ssize_t page_idx;
  ssize_t file_idx;
  struct sNode_s* next;
};

typedef struct sNode_s sNode;
sNode *s_head;


void insert_swap_page(ssize_t thread_id, ssize_t page_idx, ssize_t file_idx) {
  sNode *new_node = (sNode*)_lib_malloc(sizeof(sNode));
  new_node->thread_id = thread_id;
  new_node->page_idx = page_idx;
  new_node->file_idx = file_idx;
  new_node->next = s_head;
  s_head = new_node;
}

int pop_swap_page(ssize_t thread_id, ssize_t page_idx, ssize_t *file_idx) {
  sNode *temp = s_head, *prev;

  if (temp != NULL &&
      temp->thread_id == thread_id &&
      temp->page_idx == page_idx) {
    *file_idx = temp->file_idx;
    _lib_free(temp);
    return 1;
  }

  if (temp != NULL && (
      temp->thread_id != thread_id ||
      temp->page_idx != page_idx)) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return -1;
  prev->next = temp->next;
  *file_idx = temp->file_idx;
  free(temp);
  return 1;
}







#endif /* ifndef MALLOC_PAGE_FILE_MAP */
