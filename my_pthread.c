#include "src/my_pthread.h"

#include <stdlib.h>
#include <signal.h>

#define fiber_t my_pthread_t

int my_pthread_create(
    fiber_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine) (void *),
    void *arg){

  thread->uc_link = malloc(sizeof(ucontext_t));
  thread->to_join = NULL;

  INIT_THREAD(*thread->uc_link);
  (void **)
    *(thread->uc_link-sizeof(void *)) = &thread->rval;

  makecontext(
      thread->uc_link, __sched_pthread_routine, 3,
      start_routine, &thread->rval, arg);
  push(QThreadH, &thread->un_link);

  return 0;
}

int my_pthread_yield(void) {
  ucontext_t current;
  YIELD_THREAD(current);
}

int my_pthread_exit(void *retval) {

}

