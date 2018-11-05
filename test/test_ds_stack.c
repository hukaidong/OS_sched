#define MOCK_MALLOC
#include "malloc/page_file_map.h"
#include "malloc/segment.h"
#include "malloc/thread_entries.h"
#include "utils/minunit.h"
#include "../src/ds.c"
#include "../src/segment.c"

int foo = 0;
ssize_t id_10 = 10;
ssize_t id_11 = 11;

MU_TEST(test_check){
  sNode *s_head;
  int swap_id;
  init_file_and_stack();
  foo=_f_stack_is_empty();
  int size = f_stack.capacity;
  printf("size is %d\n",size);
  mu_assert(foo == 1, "if failed stack_is_full is wrong");
  //_f_stack_increase_capasity();
  //size = f_stack.capacity;
  //printf("after increase size becomes %d\n",size);
  
  f_stack_push_seg(id_10);
  int tem_id_10 =f_stack.array[0];
  printf("$$$$$current stack top is  %d\n",tem_id_10);
  mu_assert(tem_id_10 == 10, "iffailed stack_push_failed");
  
 // _f_stack_increase_capasity();
 // printf("after increate capacity current bottom is  %d\n",f_stack.array[0]); 
//  mu_assert(f_stack.array[0]==10,"if failed increate_capacity failed");
  ssize_t temp_pop = f_stack_pop_seg();
  printf("after pop current stack top is %d\n",f_stack.top);
  printf("pop receive is %d\n", temp_pop);

}
MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}





int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}

