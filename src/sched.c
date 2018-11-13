#define NUSER
#include "sched.h"
#include "casts.h"
#include "gvars.h"
#include "page.h"
#include "utils/utils.h"

#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>
#include <stdlib.h>

#define _STACK_SIZE sizeof(my_stack_t)
#define _THREAD_SIZE sizeof(my_thread_t)
#define _NEW_STACK() _lib_malloc(_STACK_SIZE)

inline void* _NEW_THREAD() {
  void *p = new_page(sizeof(my_stack_t), 0);
  ssize_t thread_id = (ssize_t)p & ((~0ull) >>1);
  page_assign(pointer_2_page_index(p), thread_id);
  return p;
}

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

  fib->thread_id = thread_idx_accu++;
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
  _INIT_CTX(current, &ENTRY_EXIT_CTX);
  push(&QThreadH, current);
  swapcontext(current, &ENTRY_SCHED_CTX);
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
  _lib_free(UCT_P2STCK_P(target));
  return retval;
}

inline bool IS_MAIN_CTX(uctx_p current)
{ return UCT_P2STCK_P(current) == UCT_P2STCK_P(&MAIN_CTX); }


// Frequency of queue serving
#define FREQ_FLUSH 500
#define FREQ_HQ 20
#define FREQ_MQ 10
#define FREQ_LQ 1

// swap every 20 microsecond
#define SWAP_INTERVAL_HQ 20
#define SWAP_INTERVAL_MQ 500
#define SWAP_INTERVAL_LQ 1000

#define QTOP 0
#define QMED 1
#define QLOW 2
int last_q_invoked = QTOP;


void __sched_init() {
  _page_setup();
  q_init(&QThreadH);
  q_init(&QThreadM);
  q_init(&QThreadL);

  _INIT_CTX(&ENTRY_SCHED_CTX, NULL);
  sigaddset(&(ENTRY_SCHED_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_SCHED_CTX, __sched_interrupt_next, 0);

  _INIT_CTX(&ENTRY_EXIT_CTX, NULL);
  sigaddset(&(ENTRY_EXIT_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_EXIT_CTX, __sched_exit_next, 0);

  signal(SIGALRM, __sched_alarmed);
  push(&QThreadL, &MAIN_CTX);
  swapcontext(&MAIN_CTX, &ENTRY_SCHED_CTX);
}

void __sched_deinit() {
  _lib_free(UCT_P2STCK_P(&ENTRY_SCHED_CTX));
  _lib_free(UCT_P2STCK_P(&ENTRY_EXIT_CTX));
}

void __sched_alarmed(int signum) {
  // LOG(__sched_alarmed);
  sigrelse(SIGALRM);
  if (GML) { ualarm(SWAP_INTERVAL_HQ, 0); return; }
  ucontext_t current;
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

void __sched_interrupt_next() {
  do {
    _enter_sys_mode();
    // LOG(__sched_interrupt_next);
    if (current_ctx != NULL) {
      // malloc not save during signal handler
      switch(last_q_invoked) {
        // for future adapting more complex strategy
        case QTOP: push(&QThreadM, current_ctx); break;
        case QMED: push(&QThreadL, current_ctx); break;
        default: push(&QThreadL, current_ctx); break;
      }
    }
    ucontext_t* next = __sched_q_route();
    __sched_run_next(&ENTRY_SCHED_CTX, next);
  } while (1);
}

void __sched_exit_next() {
  // WARNING: DO NOT free memory here, free it when joined
  while (1) {
    _enter_sys_mode();
    LOG(__sched_exit_next);
    sigrelse(SIGALRM);
    ucontext_t* next = __sched_q_route();
    if (next == NULL) { break; }
    __sched_run_next(&ENTRY_EXIT_CTX, next);
  }
  LOG(ERROR_ABNORMAL_EXITED);
  exit(1);
}

void __sched_run_next(uctx_p sched_ctx, const uctx_p next) {
  // LOG(__sched_run_next);
  switch (last_q_invoked) {
    case QTOP: ualarm(SWAP_INTERVAL_HQ, 0);
               /* puts("H QUEUE TRIGGERED!"); */
               break;
    case QMED: ualarm(SWAP_INTERVAL_MQ, 0);
               /* puts("M QUEUE TRIGGERED!"); */
               break;
    case QLOW: ualarm(SWAP_INTERVAL_LQ, 0);
               /* puts("L QUEUE TRIGGERED!"); */
               break;
  }
  current_ctx = next;
  _enter_user_mode(GetCurrentThreadId());
  swapcontext(sched_ctx, next);
}

void __sched_pthread_routine(
    void *(*func) (void*),
    fib_p fib,
    void *args){

  _enter_sys_mode();
  LOG(__sched_pthread_routine);
  fib->rval = func(args);
  LOG(RET:__sched_pthread_routine);
  if (fib->to_join != NULL) {
    ATTACH_THREAD(fib->to_join);
  }
  fib->status = FIB_TERMINATED;
  ssize_t thread_id = GetCurrentThreadId();
  delete_thread(thread_id);
}

ucontext_t* __sched_q_route() {
  // LOG(__sched_q_route);
  static unsigned int num_iter = 0;
  int index = num_iter % (FREQ_HQ + FREQ_MQ + FREQ_LQ);
  int qindex = 0;
  uctx_p next = NULL;
  if (index < FREQ_HQ )
    { qindex = QTOP; }
  else if (index < FREQ_HQ + FREQ_MQ)
    { qindex = QMED; }
  else if (index < FREQ_HQ + FREQ_MQ + FREQ_LQ)
    { qindex = QLOW; }

  switch (qindex) {
    case QLOW: if (!is_empty(&QThreadL)) {
                 last_q_invoked = QLOW;
                 next = pop(&QThreadL);
                 break;
               }
    case QMED: if (!is_empty(&QThreadM)) {
                 last_q_invoked = QMED;
                 next = pop(&QThreadM);
                 break;
               }
    case QTOP: if (!is_empty(&QThreadH)) {
                 last_q_invoked = QTOP;
                 next = pop(&QThreadH);
                 break;
               }
               if (!is_empty(&QThreadL)) {
                 last_q_invoked = QLOW;
                 next = pop(&QThreadL);
                 break;
               }
               if (!is_empty(&QThreadM)) {
                 last_q_invoked = QMED;
                 next = pop(&QThreadM);
                 break;
               }
  }

  num_iter++;

  if (num_iter % FREQ_FLUSH == 0)
  {
    LOG(INFO_QUEUE_FLUSHING);
    while(!is_empty(&QThreadM)) {
      push(&QThreadH, pop(&QThreadM));
    }
    while(!is_empty(&QThreadL)) {
      push(&QThreadH, pop(&QThreadL));
    }
  }

  return next;
}

#ifndef NO_INIT
void __attribute__ ((constructor)) constructor () { __sched_init(); }
void __attribute__ ((destructor)) destructor () { __sched_deinit(); }
#endif

#undef NUSER
