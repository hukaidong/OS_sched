#ifndef MALLOC_THREAD_ENTRIES_H
#define MALLOC_THREAD_ENTRIES_H

#ifdef MOCK_MALLOC
#include <stdlib.h>
#define _lib_malloc malloc
#define _lib_free free
#else
#include "my_malloc.h"
#endif

#include "utils/utils.h"

struct pNode_s
{
  int page_idx;
  struct pNode_s *next;
};

struct tNode_s
{
  ssize_t thread_id;
  int num_page_claimed;
  struct pNode_s **pHead;
  struct tNode_s  *next;
};

typedef struct pNode_s pNode;
typedef struct tNode_s tNode;

void init_thread(ssize_t thread_id);
int search_thread(ssize_t thread_id, tNode **target);
void delete_thread(ssize_t thread_id);

#endif /* ifndef MALLOC_THREAD_ENTRIES_H */
