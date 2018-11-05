#ifndef MY_PTHREAD_SCHED_H
#define MY_PTHREAD_SCHED_H

#include "types.h"
#include "thread_queue.h"
#include "my_malloc.h"

#include <stdio.h>



void __sched_init();
void __sched_alarmed(int);
void __sched_interrupt_next();
void __sched_exit_next();
void __sched_run_next(uctx_p, const uctx_p);
void __sched_pthread_routine(void *(*) (void*), fib_p, void *);
ucontext_t* __sched_q_route();


inline void _INIT_CTX(uctx_p ctx, const uctx_p link);
inline void INIT_THREAD(fib_p *fiber);
inline void YIELD_THREAD(uctx_p current);
inline void DETEACH_THREAD(uctx_p current);
inline void ATTACH_THREAD(uctx_p attach);
inline void TERMINATE_THREAD(uctx_p current);
inline void *DESTROY_THREAD(uctx_p target);
inline bool IS_MAIN_CTX(uctx_p current);

#endif /* ifndef MY_PTHREAD_SCHED_H */
