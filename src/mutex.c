#include "my_pthread.h"

#include "sched.h"
#include "thread_queue.h"
#include "types.h"
#include "gvars.h"
#include "casts.h"

#include <signal.h>
#include <ucontext.h>

#define true 1
#define false 0
#define mutex_t my_pthread_mutex_t

int my_pthread_mutex_init(
    mutex_t *mutex, const pthread_mutexattr_t *attr) {
  LOG(my_pthread_mutex_init);
  mutex->locked = false;
  q_init(&(mutex->pending));
  return 0;
}

int my_pthread_mutex_lock(mutex_t *mutex) {
  _enter_sys_mode();
  while (mutex->locked) {
    // LOG(my_pthread_mutex_lock);
    ucontext_t current;
    push(&(mutex->pending), &current);
    DETEACH_THREAD(&current);
    _enter_sys_mode();
  }
  mutex->locked = true;
  _enter_user_mode(GetCurrentThreadId());
  return 0;
}

int my_pthread_mutex_unlock(mutex_t *mutex) {
  // LOG(my_pthread_mutex_unlock);
  _enter_sys_mode();
  mutex->locked = false;
  if (!is_empty(&(mutex->pending))) {
    ATTACH_THREAD(pop(&(mutex->pending)));
  }
  _enter_user_mode(GetCurrentThreadId());
  return 0;
}

int my_pthread_mutex_destroy(mutex_t *mutex) {
  _enter_sys_mode();
  LOG(my_pthread_mutex_destroy);
  while (mutex->locked) {
    ucontext_t current;
    push(&(mutex->pending), &current);
    DETEACH_THREAD(&current);
    _enter_sys_mode();
  }
  _enter_user_mode(GetCurrentThreadId());
  while (!is_empty(&(mutex->pending))) {
    ATTACH_THREAD(pop(&(mutex->pending)));
  }
  return 0;
}
