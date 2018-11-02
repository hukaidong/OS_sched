#ifndef MY_PTHREAD_SCHED_H
#define MY_PTHREAD_SCHED_H

#include "type.h"
#include "queue.h"

#include <stdio.h>

// size of each context stack
#define _STACK_SIZE sizeof(my_stack_t)
#define _THREAD_SIZE sizeof(my_thread_t)
#define _NEW_STACK() malloc(_STACK_SIZE)
#define _NEW_THREAD() malloc(_THREAD_SIZE)

void *EXIT_THD_P;
ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX, MAIN_CTX;
Quctx QThreadH, QThreadM, QThreadL;

int thread_detached = 0;
void __sched_init();
void __sched_alarmed(int);
void __sched_interrupt_next();
void __sched_exit_next();
void __sched_run_next(uctx_p, const uctx_p);
void __sched_pthread_routine(
    void *(*) (void*), fib_p, void *);
ucontext_t* __sched_q_route();


inline void
_INIT_CTX(uctx_p ctx, const uctx_p link) {
  LOG(_INIT_CTX);
  getcontext(ctx);
  ctx->uc_link = link;
  ctx->uc_stack.ss_sp = _NEW_STACK();
  ctx->uc_stack.ss_size = _STACK_SIZE;
  ctx->uc_stack.ss_flags = 0;
}

inline void
INIT_THREAD(fib_p *fiber) {
  LOG(INIT_THREAD);
  thd_p thread = _NEW_THREAD();
  fib_p fib = &(thread->fiber);
  void * stk = &(thread->stack);
  uctx_p ctx = FIB_P2UCTX_P(fib);

  fib->status = FIB_RUNNING;
  fib->rval = NULL;
  fib->to_join = NULL;

  getcontext(ctx);
  ctx->uc_link = &ENTRY_EXIT_CTX;
  ctx->uc_stack.ss_sp = stk;
  ctx->uc_stack.ss_size = _STACK_SIZE;
  ctx->uc_stack.ss_flags = 0;

  *fiber = fib;
}

inline void
YIELD_THREAD(uctx_p current) {
  LOG(YIELD_THREAD);
  _INIT_CTX(&current, &ENTRY_EXIT_CTX);
  push(&QThreadH, &current);
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

inline void
DETEACH_THREAD(uctx_p current) {
  LOG(DESTROY_THREAD);
  thread_detached++;
  swapcontext(current, &ENTRY_SCHED_CTX);
}

inline void
ATTACH_THREAD(uctx_p attach) {
  LOG(ATTACH_THREAD);
  thread_detached--;
  push(&QThreadH, attach);
}

inline void
TERMINATE_THREAD(uctx_p current) {
  LOG(TERMINATE_THREAD);
  EXIT_THD_P = UCT_P2STCK_P(current);
  fib_p fiber = UCTX_P2FIB_P(current);
  fiber->status = FIB_TERMINATED;
  setcontext(&ENTRY_EXIT_CTX);
}

inline void *
DESTROY_THREAD(uctx_p target) {
  LOG(DESTROY_THREAD);
  fib_p fiber = UCTX_P2FIB_P(target);
  void *retval = fiber->rval;
  free(UCT_P2STCK_P(target));
  return retval;
}

inline bool IS_MAIN_CTX(uctx_p current)
{ return UCT_P2STCK_P(current) == UCT_P2STCK_P(&MAIN_CTX); }

#endif /* ifndef MY_PTHREAD_SCHED_H */