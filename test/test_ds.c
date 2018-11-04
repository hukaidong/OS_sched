#include "utils/minunit.h"
#include "malloc/page_file_map.h"
#include "malloc/segment.h"
#include "malloc/thread_entries.h"
ssize_t id = 1;
int foo = 0;
MU_TEST(test_check){
  tNode *target;
  init_thread(id);
  foo=search_thread(1, &target);
  mu_assert("error, foo != 1", foo == 1);
  return 0;  

}


MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}




int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}
