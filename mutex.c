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
  mutex = (mutex_t *)malloc(sizeof(mutex_t));
  mutex.locked = false;
  mutex.pending = new_list(ucxt_p);
  return 0;
}

int my_pthread_mutex_lock(mutex_t *mutex) {
  sigrelse(SIGALRM);
  while (mutex.locked) {
    ucontext_t current;
    push(mutex.pending, &current);
    DETEACH_THREAD(current);
  }
  mutex.locked = true;
  return 0;
}

int my_pthread_mutex_unlock(mutex_t *mutex) {
  sigrelse(SIGALRM);
  mutex.locked = false;
  if (!is_empty(mutex.pending)) {
    ATTACH_THREAD(*pop(mutex.pending));
  }
  return 0;
}

int my_pthread_mutex_destory(mutex_t *mutex) {
  sigrelse(SIGALRM);
  while (mutex.locked) {
    ucontext_t current;
    push(mutex.pending, &current);
    DETEACH_THREAD(current);
  }
  while (!is_empty(mutex.pending)) {
    ATTACH_THREAD(*pop(mutex.pending));
  }
  free(mutex);
  return 0;
}
