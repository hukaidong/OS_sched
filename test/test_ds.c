#define MOCK_MALLOC
#include "malloc/page_file_map.h"
#include "malloc/segment.h"
#include "malloc/thread_entries.h"
#include "malloc/global.h"
#include "utils/minunit.h"

ssize_t id = 1;
ssize_t id_2 = 2;
ssize_t id_3 = 5;
int foo = 0;
int bar = 0;
int bar2 = 0;
MU_TEST(test_check){
  tNode *target;
  init_thread(id);
  init_thread(id_2);
  init_thread(id_3);

  foo = search_thread(2,&target);
  bar = search_thread(5,&target);

  mu_check(bar==1);
  mu_check(foo==1);

  delete_thread(5);
  bar2 = search_thread(5,&target);
  mu_assert(bar==5,"failed means delte_thread node success");
}

MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}



int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}

