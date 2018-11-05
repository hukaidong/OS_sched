#ifndef MALLOC_PAGE_FILE_MAP
#define MALLOC_PAGE_FILE_MAP

#ifndef MOCK_MALLOC
#include "my_malloc.h"
#else
#include <stdlib.h>
#define _lib_malloc malloc
#define _lib_free free
#endif

#include "types.h"
#include "utils/utils.h"

void insert_swap_page(ssize_t page_idx);
int pop_swap_page(ssize_t thread_id, ssize_t page_idx);
void init_file_and_stack();
int _f_stack_is_full();
int _f_stack_is_empty();
void _f_stack_increase_capasity();
void f_stack_push_seg(ssize_t item);
ssize_t f_stack_pop_seg();
void swap_to_file(ssize_t page_idx, ssize_t file_idx);
void swap_to_mem(ssize_t page_idx, ssize_t file_idx);

#endif /* ifndef MALLOC_PAGE_FILE_MAP */
