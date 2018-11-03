#ifndef MY_PTHREAD_TYPE_H
#define MY_PTHREAD_TYPE_H

#ifdef __APPLE__
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

#include <unistd.h>

#define MAX_Q_SIZE 20
typedef ucontext_t* uctx_p;

typedef struct {
  uctx_p  uctx[MAX_Q_SIZE];
  int head;
  int size;
} Quctx;

#define FIB_RUNNING 0
#define FIB_TERMINATED 1

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
  Quctx   pending;
} my_pthread_mutex_t;

uctx_p current_ctx = NULL;
inline void * UCT_P2STCK_P(uctx_p ctx)     { return ctx->uc_stack.ss_sp; }
inline void * FIB_P2STCK_P(fib_p fib)      { return UCT_P2STCK_P(&(fib->uctx)); }
inline fib_p  UCTX_P2FIB_P(uctx_p ctx)     { return (fib_p)UCT_P2STCK_P(ctx); }
inline uctx_p FIB_P2UCTX_P(fib_p fib)      { return &(fib->uctx); }
inline long int GetCurrentThreadId() {
  return (long int)(UCT_P2STCK_P(current_ctx)) & ((~0ull) >> 1);
}

volatile int GML;

#endif /* ifndef MY_PTHREAD_TYPE_H */
