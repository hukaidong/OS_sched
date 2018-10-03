#include "src/mutex.h"
#include "src/sched.h"
#include "src/queue.h"
#include "src/type.h"
#include "src/my_pthread.h"

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
  mutex->pending = new_list(uctx_p);
  return 0;
}

int my_pthread_mutex_lock(mutex_t *mutex) {
  LOG(my_pthread_mutex_lock);
  sigrelse(SIGALRM);
  while (mutex->locked) {
    ucontext_t current;
    push(mutex->pending, &current);
    DETEACH_THREAD(&current);
  }
  mutex->locked = true;
  return 0;
}

int my_pthread_mutex_unlock(mutex_t *mutex) {
  LOG(my_pthread_mutex_unlock);
  sigrelse(SIGALRM);
  mutex->locked = false;
  if (!is_empty(mutex->pending)) {
    ATTACH_THREAD(pop(mutex->pending));
  }
  return 0;
}

int my_pthread_mutex_destroy(mutex_t *mutex) {
  LOG(my_pthread_mutex_destroy);
  sigrelse(SIGALRM);
  while (mutex->locked) {
    ucontext_t current;
    push(mutex->pending, &current);
    DETEACH_THREAD(&current);
  }
  while (!is_empty(mutex->pending)) {
    ATTACH_THREAD(pop(mutex->pending));
  }
  return 0;
}
