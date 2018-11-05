#include "my_pthread.h"
#include "pthread/sched.h"
#include "pthread/type.h"
#include "pthread/queue.h"
#include "utils/utils.h"
#include "my_malloc.h"
#include "malloc/global.h"


int my_pthread_create(
    pthread_t  *thread,
    const pthread_attr_t *attr,
    void *(*start_routine) (void *),
    void *arg){

  _enter_sys_mode();
  LOG(my_pthread_create);
  INIT_THREAD(*thread);
  makecontext(
      FIB_P2UCTX_P(**thread), __sched_pthread_routine, 3,
      start_routine, (fib_p)*thread, arg);

  push(&QThreadH, &(((fib_p)*thread)->uctx));
  ssize_t thread_id = GetCurrentThreadId();
  init_thread(thread_id);
  _enter_user_mode(thread_id);
  return 0;
}

int my_pthread_yield(void) {
  _enter_sys_mode();
  LOG(my_pthread_yield);
  ucontext_t current;
  YIELD_THREAD(&current);
  _enter_user_mode(GetCurrentThreadId());
  return 0;
}

int my_pthread_join(pthread_t thread, void **retval){
  _enter_sys_mode();
  LOG(my_pthread_join);
    if ((*thread)->to_join != NULL) {
      // another thread is already waiting to join with this thread
      LOG(EINVAL);
      return EINVAL;
    }
  while ((*thread)->status != FIB_TERMINATED) {
    ucontext_t current;
    (*thread)->to_join = &current;
    DETEACH_THREAD(&current);
  }
  if (retval != NULL) {
    *retval = DESTROY_THREAD(FIB_P2UCTX_P(*thread));
  }
  _enter_user_mode(GetCurrentThreadId());
  return 0;
}

void my_pthread_exit(void *retval) {
  _enter_sys_mode();
  LOG(my_pthread_exit);
  ucontext_t current;
  getcontext(&current);

  fib_p fiber = UCTX_P2FIB_P(&current);
  fiber->rval = retval;

  TERMINATE_THREAD(&current);
  _enter_user_mode(GetCurrentThreadId());
}

