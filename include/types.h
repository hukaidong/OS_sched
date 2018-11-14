#ifndef MY_TYPES_H
#define MY_TYPES_H value

#include "const.h"
#include "utils/utils.h"

typedef struct segment_header {
  struct segment_header *prev_seg;
  struct segment_header *next_seg;
  unsigned int    flags;
  char            buf[1];
} segment_header, *seg_p;

typedef struct page_entry {
  ssize_t  thread_id;
  size_t  max_avail;
} page_entry;

typedef ucontext_t* uctx_p;
typedef struct {
  uctx_p  uctx[MAX_Q_SIZE];
  int head;
  int size;
} Quctx;

typedef struct {
  char        member[4*UNIT_KB];
} my_stack_t;

// UCTX must at the first position
typedef struct {
  ucontext_t  uctx;
  int         status;
  int         thread_id;
  void        *rval;
  ucontext_t  *to_join;
} my_fiber_t, *fib_p;
typedef fib_p* my_pthread_t;

// STACK must at the first position
typedef struct {
  my_stack_t stack;
  my_fiber_t  fiber;
} my_thread_t, *thd_p;

typedef struct {
  // Memory allignment
  int     locked;
  Quctx   pending;
} my_pthread_mutex_t;

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


#endif /* ifndef MY_TYPES_H */
