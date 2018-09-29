#include <stddef.h>
#include <signal.h>

#include "my_pthread.h"


my_pthread_ptr QThreadH, QThreadM, QThreadL;
ucontext_t SCHED_CTX;

void __sched_init() {
  getcontext(&SCHED_CTX);
  sigaddset(&(SCHED_CTX.uc_sigmask), SIGALARM);
  makecontext(&SCHED_CTX, __sched_run_next);

}

void __sched_run_next() {

}

int main() {

  return 0;
}
