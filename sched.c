#include "src/sched.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

// Frequency of queue serving
#define FREQ_HQ 20
#define FREQ_MQ 10
#define FREQ_LQ 1

// swap every 20 microsecond
#define SWAP_INTERVAL 20

// size of each context stack
#define _STACK_SIZE sizeof(char[16384])
#define _NEW_STACK() malloc(_STACK_SIZE)

#define _INIT_CTX(ctx, link) \
  do {\
  getcontext(&ctx); \
  ctx.uc_link = link; \
  ctx.uc_stack.ss_sp = _NEW_STACK(); \
  ctx.uc_stack.ss_size = _STACK_SIZE; \
  ctx.uc_stack.ss_flags = 0; \
  } while (0)

ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX,
           MAIN_CTX;
// TODO: update element from ucontext_t to costum struct for last priority info
Qucxt QThreadH, QThreadM, QThreadL;

void __sched_init() {
  QThreadH = new_list(ucontext_t);
  QThreadM = new_list(ucontext_t);
  QThreadL = new_list(ucontext_t);

  _INIT_CTX(ENTRY_SCHED_CTX, NULL);
  sigaddset(&(ENTRY_SCHED_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_SCHED_CTX, __sched_interrupt_next, 0);

  _INIT_CTX(ENTRY_EXIT_CTX, NULL);
  sigaddset(&(ENTRY_EXIT_CTX.uc_sigmask), SIGALRM);
  makecontext(&ENTRY_EXIT_CTX, __sched_exit_next, 0);

  // TODO: ENTRY_YIELD_CTX, __sched_yield_next

  /* swapcontext(&MAIN_CTX, &ENTRY_SCHED_CTX); */
}

void __sched_interrupt_next() {
  // TODO: while loop
  ucontext_t* next = __sched_q_route();
  assert(next);

}

// TODO: __sched_yield_next: release last alarm signal before next setup

void __sched_exit_next() {
  // TODO: while loop
  ucontext_t* next = __sched_q_route();
  if (next)
  {
    __sched_run_next(next);
  }
  else if (0)
  {
    puts("Dead lock detected");
    exit(1);
  }
  exit(0);
}

void __sched_run_next(const ucontext_t* next)
{
  ualarm(SWAP_INTERVAL, 0);
  setcontext(next);
}

ucontext_t* __sched_q_route()
{
  // TODO: Change to queue form & allow queue flushing (raise everything up)
  // TODO: Update last priority info before return
  static int index = 0;
  index = index % (FREQ_HQ + FREQ_MQ + FREQ_LQ);
  if (index < FREQ_HQ && QThreadH != NULL)
  {
    return QThreadH;
  }
  else if (index < FREQ_HQ + FREQ_MQ && QThreadL != NULL)
  {
    return QThreadM;
  }
  else if (index < FREQ_HQ + FREQ_MQ + FREQ_LQ && QThreadM != NULL);
  {
    return QThreadL;
  }
  exit(1);  // Never happen
}


