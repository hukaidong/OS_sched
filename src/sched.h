#ifndef MY_PTHREAD_SCHED_H
#define MY_PTHREAD_SCHED_H

#include "type.h"
#include "queue.h"

// size of each context stack
#define _STACK_SHIFT sizeof(void *)
#define _STACK_SIZE sizeof(char[16384])
#define _NEW_STACK() malloc(_STACK_SIZE)

#define _INIT_CTX(ctx, link) \
  do {\
    getcontext(&ctx); \
    ctx.uc_link = link; \
    ctx.uc_stack.ss_sp = _NEW_STACK() + _STACK_SHIFT; \
    ctx.uc_stack.ss_size = _STACK_SIZE - _STACK_SHIFT; \
    ctx.uc_stack.ss_flags = 0; \
  } while (0)

#define INIT_THREAD(ctx) _INIT_CTX(ctx, &ENTRY_EXIT_CTX) \
#define NEW_THREAD(func) \
  do { \
    ucontext_t current; \
    INIT_THREAD(current); \
    makecontext(
        &current,
        __sched_pthread_routine,
        3,
        func,
        &current.rval,
        args
  } while (0)

#define YIELD_THREAD(current) \
  do {\
    INIT_THREAD(current); \
    push(QThreadH, &current); \
    swapcontext(&current, &ENTRY_SCHED_CTX); \
  } while (0)

#define DETEACH_THREAD(current) \
  do { \
    thread_detached++; \
    swapcontext(&current, &ENTRY_SCHED_CTX); \
  } while(0)

#define ATTACH_THREAD(attach) \
  do { \
    thread_detached--; \
    ucontext_t current; \
    INIT_THREAD(current); \
    push(QThreadH, &attach); \
    push(QThreadH, &current); \
    swapcontext(&current, &ENTRY_SCHED_CTX); \
  } while(0)


int thread_detached = 0;
ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX;
Qucxt QThreadH, QThreadM, QThreadL;

void __sched_init();
void __sched_interrupt_next();
void __sched_exit_next();
void __sched_run_next(const ucontext_t*);
void __sched_pthread_routine(
    void *(*func) (void*), void **rval, void *args);

ucontext_t* __sched_q_route();

#endif /* ifndef MY_PTHREAD_SCHED_H */
