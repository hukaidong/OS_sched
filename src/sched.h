#ifndef MY_PTHREAD_SCHED_H
#define MY_PTHREAD_SCHED_H

#include "type.h"
#include "queue.h"

// size of each context stack
#define _STACK_SIZE sizeof(my_stack_t)
#define _THREAD_SIZE sizeof(my_thread_t)
#define _NEW_STACK() malloc(_STACK_SIZE)
#define _NEW_THREAD() malloc(_THREAD_SIZE)

inline void
_INIT_CTX(ucxt_p ctx, const ucxt_p link) {
  getcontext(&ctx);
  ctx.uc_link = link;
  ctx.uc_stack.ss_sp = _NEW_STACK();
  ctx.uc_stack.ss_size = _STACK_SIZE;
  ctx.uc_stack.ss_flags = 0;
}

inline void
INIT_THREAD(my_pthread_t *fiber) {
  thd_p thread = _NEW_THREAD();
  fib = &(thread->fiber);
  uctx_p ctx = FIB2UCTX_P(fiber);
  void * stk = FIB2STCK_P(fiber);

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
  _INIT_CTX(current);
  push(QThreadH, &current);
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

inline void
DETEACH_THREAD(uctx_p current) {
  thread_detached++;
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

inline void
ATTACH_THREAD(uctx_p attach) {
  thread_detached--;
  ucontext_t current;
  _INIT_CTX(current);
  push(QThreadH, &attach);
  push(QThreadH, &current);
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

inline void
TERMINATE_THREAD(uctx_p current) {
  EXIT_THD_P = UCT2STK_P(*current);
  setcontext(&ENTRY_EXIT_CTX);
}

inline void *
DESTORY_THREAD(uctx_p target) {
  fib_p fiber = UCTX_P2FIB_P(target);
  void *retval = fiber->rval;
  free(UCTX_P2STCK_P(target));
  return retval;
}


void *EXIT_THD_P;
ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX, MAIN_CTX;
Qucxt QThreadH, QThreadM, QThreadL;

int thread_detached = 0;
void __sched_init();
void __sched_interrupt_next();
void __sched_exit_next();
void __sched_run_next(const ucontext_t*);
void __sched_pthread_routine(
    void *(*func) (void*), void **rval, void *args);
ucontext_t* __sched_q_route();

inline bool IS_MAIN_CTX(uctx_p current)
{ return UCT2STCK_P(current) == UCT2STCK_P(MAIN_CTX) }

#endif /* ifndef MY_PTHREAD_SCHED_H */
