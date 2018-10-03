#include "src/sched.h"

#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

#define UNUSED(x) (void)(x)

// Frequency of queue serving
#define FREQ_FLUSH 500
#define FREQ_HQ 20
#define FREQ_MQ 10
#define FREQ_LQ 1

// swap every 20 microsecond
#define SWAP_INTERVAL_HQ 200
#define SWAP_INTERVAL_MQ 500
#define SWAP_INTERVAL_LQ 1000

#define QTOP 0
#define QMED 1
#define QLOW 2
int last_q_invoked = QTOP;
uctx_p alarmed_ctx = NULL;


void __sched_init() {
  QThreadH = new_list(uctx_p);
  QThreadM = new_list(uctx_p);
  QThreadL = new_list(uctx_p);

  _INIT_CTX(&ENTRY_SCHED_CTX, NULL);
  sigaddset(&(ENTRY_SCHED_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_SCHED_CTX, __sched_interrupt_next, 0);

  _INIT_CTX(&ENTRY_EXIT_CTX, NULL);
  sigaddset(&(ENTRY_EXIT_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_EXIT_CTX, __sched_exit_next, 0);

  signal(SIGALRM, __sched_alarmed);
  push(QThreadH, &MAIN_CTX);
  swapcontext(&MAIN_CTX, &ENTRY_SCHED_CTX);
}

void __sched_deinit() {
  free(UCT_P2STCK_P(&ENTRY_SCHED_CTX));
  free(UCT_P2STCK_P(&ENTRY_EXIT_CTX));
}

void __sched_alarmed(int signum) {
  LOG(__sched_alarmed);
  sigrelse(SIGALRM);
  ucontext_t current;
  alarmed_ctx = &current;
  swapcontext(&current, &ENTRY_SCHED_CTX);
}

void __sched_interrupt_next() {
  do {
    LOG(__sched_run_next);
    if (alarmed_ctx != NULL) {
      // malloc not save during signal handler
      switch(last_q_invoked) {
        // for future adapting more complex strategy
        case QTOP: push(QThreadM, alarmed_ctx); break;
        case QMED: push(QThreadL, alarmed_ctx); break;
        default: push(QThreadL, alarmed_ctx); break;
      }
      alarmed_ctx = NULL;
    }
    ucontext_t* next = __sched_q_route();
    __sched_run_next(&ENTRY_SCHED_CTX, next);
  } while (1);
}

void __sched_exit_next() {
  // WARNING: DO NOT free memory here, free it when joined
  while (1) {
    LOG(__sched_exit_next);
    sigrelse(SIGALRM);
    ucontext_t* next = __sched_q_route();
    if (next == NULL) { break; }
    __sched_run_next(&ENTRY_EXIT_CTX, next);
  }
  exit(0);
}

void __sched_run_next(uctx_p sched_ctx, const uctx_p next)
{
  LOG(__sched_run_next);
  switch (last_q_invoked) {
    case QTOP: ualarm(SWAP_INTERVAL_HQ, 0);
               puts("H QUEUE TRIGGERED!");
               break;
    case QMED: ualarm(SWAP_INTERVAL_MQ, 0);
               puts("M QUEUE TRIGGERED!");
               break;
    case QLOW: ualarm(SWAP_INTERVAL_LQ, 0);
               puts("L QUEUE TRIGGERED!");
               break;
  }
  swapcontext(sched_ctx, next);
}

void __sched_pthread_routine(
    void *(*func) (void*),
    fib_p fib,
    void *args){
  LOG(__sched_pthread_routine);
  fib->rval = func(args);
  if (fib->to_join != NULL) {
    ATTACH_THREAD(fib->to_join);
  }
  fib->status = FIB_TERMINATED;
}

ucontext_t* __sched_q_route()
{
  LOG(__sched_q_route);
  static unsigned int num_iter = 0;
  int index = num_iter % (FREQ_HQ + FREQ_MQ + FREQ_LQ);
  uctx_p next = NULL;
  if (index < FREQ_HQ && !is_empty(QThreadH))
  {
    last_q_invoked = QTOP;
    next = pop(QThreadH);
  }
  else if (index < FREQ_HQ + FREQ_MQ && !is_empty(QThreadM))
  {
    last_q_invoked = QMED;
    next = pop(QThreadM);
  }
  else if (index < FREQ_HQ + FREQ_MQ + FREQ_LQ && !is_empty(QThreadL))
  {
    last_q_invoked = QLOW;
    next = pop(QThreadL);
  }

  num_iter++;

  if (num_iter % FREQ_FLUSH == 0)
  {
    while(!is_empty(QThreadM)) {
      push(QThreadH, pop(QThreadM));
    }
    while(!is_empty(QThreadL)) {
      push(QThreadH, pop(QThreadL));
    }
  }

  return next;
}

void __attribute__ ((constructor)) constructor () {
  __sched_init();
}

void __attribute__ ((destructor)) destructor () {
  __sched_deinit();
}
