#include "my_pthread"

int main() {
  __sched_init();
  puts("never happens");
  return 0;
}
