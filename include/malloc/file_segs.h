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

struct File_Seg_Stack
{
    int top;
    unsigned capacity;
    ssize_t* array;
};

typedef struct File_Seg_Stack f_stack_t;
f_stack_t f_stack;

void init_f_stack() {
  f_stack.capacity = 20;
  f_stack.top = -1;
  f_stack.array = (ssize_t*) malloc(20*sizeof(ssize_t));
}

int _f_stack_is_full()
{   return f_stack.top == f_stack.capacity - 1; }

int _f_stack_is_empty()
{   return f_stack.top == -1;  }

void _f_stack_increase_capasity()
  // TODO: increase capasity!!
{   return; }

void f_stack_push_seg(ssize_t item)
{
    if (_f_stack_is_full())
      _f_stack_increase_capasity();
    f_stack.array[++f_stack.top] = item;
}

ssize_t f_stack_pop_seg()
{
    if (_f_stack_is_empty())
        return -1;
    return f_stack.array[f_stack.top--];
}





#endif /* ifndef MALLOC_PAGE_FILE_MAP */
