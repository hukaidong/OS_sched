#ifndef MALLOC_PAGE_FILE_MAP
#define MALLOC_PAGE_FILE_MAP

#ifndef MOCK_MALLOC
#include "my_malloc.h"
#else
#include <stdlib.h>
#define _lib_malloc malloc
#define _lib_free free
#endif

#include "malloc/type.h"
#include "utils/utils.h"


struct sNode_s {
  page_entry pcb;
  ssize_t page_idx;
  ssize_t file_idx;
  struct sNode_s* next;
};

typedef struct sNode_s sNode;

struct File_Seg_Stack
{
    int top;
    unsigned capacity;
    ssize_t* array;
};

typedef struct File_Seg_Stack f_stack_t;


void insert_swap_page(ssize_t page_idx);
int pop_swap_page(ssize_t thread_id, ssize_t page_idx);

#endif /* ifndef MALLOC_PAGE_FILE_MAP */
