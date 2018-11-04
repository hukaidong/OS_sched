#define MOCK_MALLOC
#include "utils/minunit.h"
#include "malloc/page_file_map.h"
#include "malloc/segment.h"
#include "malloc/thread_entries.h"
#include "../src/ds.c"
#include "../src/segment.c"

int foo = 0;
MU_TEST(test_check){
  sNode *s_head;
  int swap_id;
  init_file_and_stack();
  foo=_f_stack_is_empty();
  int size = f_stack.capacity;
  printf("size is %d\n",size); 
  mu_assert(foo == 1, "if failed stack_is_full is wrong");
  _f_stack_increase_capasity();
  size = f_stack.capacity;
  printf("after increase size becomes %d\n",size);
}
MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}





int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}

