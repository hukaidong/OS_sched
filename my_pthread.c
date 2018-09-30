#include "src/my_pthread.h"

#include <signal.h>

int my_pthread_yield(void) {
  ucontext_t current;
  _YIELD_THREAD(current);
}
