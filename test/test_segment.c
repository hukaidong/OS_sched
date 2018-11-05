#define MOCK_MALLOC
#define NUSER
#include "types.h"
#include "segment.h"
#include "utils/minunit.h"
#include "pfmap.h"
#include "gvars.h"


MU_TEST(test_check){
  char *ptr;
  ptr = (char*)malloc(4096 *sizeof(char));
  free(ptr);
  //char *ptr_end = ptr+4096;

  //sys_seg_init(ptr,ptr_end);
}

MU_TEST_SUITE(test_suite){
  MU_RUN_TEST(test_check);
}


int main(int argc, char **argv){
  MU_RUN_SUITE(test_suite);
  MU_REPORT();
  return minunit_status;
}
