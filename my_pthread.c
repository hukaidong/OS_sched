#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>

#include "my_pthread.h"


void dummy_f() {};


int main() {
  __sched_init();
  puts("never happens");
  return 0;
}
