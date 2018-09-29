#ifndef SCHED_H
#define SCHED_H

#include <queue.h>

void __sched_init();
void __sched_interrupt_next();
void __sched_exit_next();
void __sched_run_next(const ucontext_t*);
ucontext_t* __sched_q_route();

#endif /* ifndef SCHED_H */
