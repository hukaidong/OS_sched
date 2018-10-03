#include "src/my_pthread.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int my_pthread_create(
    pthread_t  *thread,
    const pthread_attr_t *attr,
    void *(*start_routine) (void *),
    void *arg){

  LOG(my_pthread_create);
  INIT_THREAD(thread);
  makecontext(
      FIB_P2UCTX_P(*thread), __sched_pthread_routine, 3,
      start_routine, (fib_p)*thread, arg);

  push(QThreadH, &(((fib_p)*thread)->uctx));

  return 0;
}

int my_pthread_yield(void) {
  LOG(my_pthread_yield);
  ucontext_t current;
  YIELD_THREAD(&current);
  return 0;
}

int my_pthread_join(fib_p thread, void **retval){
  LOG(my_pthread_join);
  while (thread->status != FIB_TERMINATED) {
    if (thread->to_join != NULL) {
      // another thread is already waiting to join with this thread
      return EINVAL;
    }
    ucontext_t current;
    thread->to_join = &current;
    DETEACH_THREAD(&current);
  }
  if (retval != NULL) {
    *retval = DESTROY_THREAD(FIB_P2UCTX_P(thread));
  }
  return 0;
}

void my_pthread_exit(void *retval) {
  LOG(my_pthread_exit);
  ucontext_t current;
  getcontext(&current);

  fib_p fiber = UCTX_P2FIB_P(&current);
  fiber->rval = retval;

  TERMINATE_THREAD(&current);
}

