#include <stddef.h>
#include <signal.h>
#include <sys/ucontext.h>

#include "my_pthread.h"


my_pthread_ptr QThreadH, QThreadM, QThreadL;
ucontext_t SCHED_CTX;

void sched_init() {
  getcontext(&SCHED_CTX);

}
