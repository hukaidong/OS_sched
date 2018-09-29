#include "my_pthread.c"

int main() {
  __sched_init();
  puts("never happens");
  return 0;
}
