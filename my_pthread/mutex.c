#include "include/mutex.h"
#include "include/sched.h"
#include "include/queue.h"
#include "include/type.h"
#include "include/my_pthread.h"

#include <stdlib.h>
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
  GML = 1;
  while (mutex->locked) {
    // LOG(my_pthread_mutex_lock);
    ucontext_t current;
    push(&(mutex->pending), &current);
    DETEACH_THREAD(&current);
    GML = 1;
  }
  mutex->locked = true;
  GML = 0;
  return 0;
}

int my_pthread_mutex_unlock(mutex_t *mutex) {
  // LOG(my_pthread_mutex_unlock);
  GML = 1;
  mutex->locked = false;
  if (!is_empty(&(mutex->pending))) {
    ATTACH_THREAD(pop(&(mutex->pending)));
  }
  GML = 0;
  return 0;
}

int my_pthread_mutex_destroy(mutex_t *mutex) {
  GML = 1;
  LOG(my_pthread_mutex_destroy);
  while (mutex->locked) {
    ucontext_t current;
    push(&(mutex->pending), &current);
    DETEACH_THREAD(&current);
    GML = 1;
  }
  GML = 0;
  while (!is_empty(&(mutex->pending))) {
    ATTACH_THREAD(pop(&(mutex->pending)));
  }
  return 0;
}
