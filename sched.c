#include "src/sched.h"

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>


typedef void *Queue;

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

ucontext_t SCHED_CTX, MAIN_CTX;
Queue* QThreadH, QThreadM, QThreadL, QThreadW;

void __sched_init() {
  __INIT_CTX(SCHED_CTX, NULL);
  sigaddset(&(SCHED_CTX.uc_sigmask), SIGALRM);
  makecontext(&SCHED_CTX, __sched_run_next, 0);

  /* swapcontext(&MAIN_CTX, &SCHED_CTX); */
}

void __sched_run_next() {
  do {
    puts("context switched!");
  } while (0);
  exit(0);
}

Queue* __sched_q_route()
{
  static int index = 0;
  index = index % (FREQ_HQ + FREQ_MQ + FREQ_LQ);
  if (index < FREQ_HQ )
  {
    return QThreadH;
  }
  else if (index < FREQ_HQ + FREQ_MQ)
  {
    return QThreadM;
  }
  else if (index < FREQ_HQ + FREQ_MQ + FREQ_LQ);
  {
    return QThreadL;
  }
  exit(1);  // Never happen
}


