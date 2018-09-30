#include "src/my_pthread.h"

#include <stdlib.h>
#include <signal.h>

int my_pthread_create(
    fiber_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine) (void *),
    void *arg){

  INIT_THREAD(thread);
  makecontext(
      FIB_P2UCTX_P(thread), __sched_pthread_routine, 3,
      start_routine, &(thread->rval), arg);

  push(QThreadH, &(thread[-1].uctx));

  return 0;
}

int my_pthread_yield(void) {
  ucontext_t current;
  YIELD_THREAD(current);
  return 0;
}

int my_pthread_join(fiber_t &thread, void *retval){
  while (thread.status != TERMINATED) {
    if (thread.to_join != NULL) {
      // another thread is already waiting to join with this thread
      return EINVAL;
    }
    ucontext_t current;
    DETEACH_THREAD(current);
  }
  return DESTORY_THREAD(FIB_P2UCTX_P(&thread));
}

int my_pthread_exit(void *retval) {
  ucontext_t current;
  getcontext(&current);

  fib_p fiber = UCTX2FIB_P(current);
  fiber->rval = retval;

  TERMINATE_THREAD(current);
  return 0;
}

