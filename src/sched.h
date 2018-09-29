#ifndef SCHED_H
#define SCHED_H

#include <queue.h>

void __sched_init();
void __sched_run_next();
Queue* __sched_q_route();

#endif /* ifndef SCHED_H */
