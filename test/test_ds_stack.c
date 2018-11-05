#define MOCK_MALLOC
#include "malloc/page_file_map.h"
#include "malloc/segment.h"
#include "malloc/thread_entries.h"
#include "utils/minunit.h"




MU_TEST(test_check){
sNode *s_head;
f_stack_t f_stack;
int swap_id;

}
MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}





int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}

