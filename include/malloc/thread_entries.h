#ifndef MALLOC_THREAD_ENTRIES_H
#define MALLOC_THREAD_ENTRIES_H

#ifdef MOCK_MALLOC
#include <stdlib.h>
#define _l
#else
#include "my_malloc.h"
#endif
#include "utils/utils.h"

#define thread_id_t int

struct pNode_s
{
  int page_idx;
  struct pNode_s *next;
};

struct tNode_s
{
  thread_id_t thread_id;
  int num_page_claimed;
  struct pNode_s **pHead;
  struct tNode_s  *next;
};

typedef struct pNode_s pNode;
typedef struct tNode_s tNode;
tNode *tHead;


void init_thread(thread_id_t thread_id) {
  tNode* new_node = (tNode*) _lib_malloc(sizeof(tNode));
  new_node->thread_id = thread_id;
  new_node->pHead = NULL;
  new_node->next = tHead;
  tHead = new_node;
}

int search_thread(thread_id_t thread_id, tNode **target) {
  tNode* current = tHead;
  while (current != NULL) {
    if (current->thread_id == thread_id) {
      *target = current;
      return 1;
    }
    current = current->next;
  }
  return -1;
}

void delete_thread(thread_id_t key) {
  tNode *temp = tHead, *prev;

  if (temp != NULL && temp->thread_id == key) {
    // TODO: free pNodes?
    tHead = temp->next;
    _lib_free(temp);
    return;
  }

  while (temp != NULL && temp->thread_id != key) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return;
  prev->next = temp->next;

  free(temp);
}




#endif /* ifndef MALLOC_THREAD_ENTRIES_H */
