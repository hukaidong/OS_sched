#ifndef MY_PTHREAD_TYPE_H
#define MY_PTHREAD_TYPE_H

#include <ucontext.h>

#include "queue.h"

#define FIB_RUNNING 0
#define FIB_TERMINATED 1

#define Quctx List(uctx_p)

typedef ucontext_t* uctx_p;
define_list(uctx_p)

typedef struct {
  char        member[16384];
} my_stack_t;

// UCTX must at the first position
typedef struct {
  ucontext_t  uctx;
  int         status;
  void        *rval;
  uctx_p      to_join;
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
  Quctx   *pending;
} my_pthread_mutex_t;

inline void * UCT_P2STCK_P(uctx_p ctx)     { return ctx->uc_stack.ss_sp; }
inline void * FIB_P2STCK_P(fib_p fib)      { return UCT_P2STCK_P(&(fib->uctx)); }
inline fib_p  UCTX_P2FIB_P(uctx_p ctx)     { return (fib_p)UCT_P2STCK_P(ctx); }
inline uctx_p FIB_P2UCTX_P(fib_p fib)      { return &(fib->uctx); }

#endif /* ifndef MY_PTHREAD_TYPE_H */
