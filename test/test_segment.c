#define MOCK_MALLOC
#define NUSER
#include "types.h"
#include "const.h"
#include "segment.h"
#include "utils/utils.h"
#include "utils/minunit.h"

#include "string.h"

char buff[8*UNIT_MB];

void reset_buffer() {
  memset(buff, 0, sizeof(buff));
}

MU_TEST(test_putils_move) {
  char *nptr = buff;
  int *iptr = (int *)buff;
  ABSOLUTE_MOVE(iptr, 40*UNIT_KB);
  mu_check(iptr == (int*)(&buff[40*UNIT_KB]));
}

MU_TEST(test_putils_offset) {
  char *nptr = buff;
  int *iptr = (int *)buff;
  ABSOLUTE_MOVE(iptr, 40*UNIT_KB);
  mu_check(
      ABSOLUTE_OFFSET(iptr, (int*)buff) == 40*UNIT_KB);
}

MU_TEST(test_flags_set) {
  char flag = 0;
  FLAG_SET(flag, 0b0001);
  FLAG_SET(flag, 0b0100);
  mu_check(flag == 0b0101);
}

MU_TEST(test_flags_transfer) {
  char flag = 0b0101,
       flag1 = 0b0010;

  FLAG_TRANSFER(flag1, flag, 0b1100);
  mu_check(flag1 == 0b0110);
}

MU_TEST(test_flags_unset) {
  char flag = 0b0111;

  FLAG_UNSET(flag, 0b0100);
  mu_check(flag == 0b0011);
}

MU_TEST(test_flags_check) {
  char flag = 0b0101;

  mu_check(FLAG_CHECK(flag, 0b0100));
  mu_check(!FLAG_CHECK(flag, 0b0010));
  mu_check(FLAG_CHECK(flag, 0b0101));
  mu_check(!FLAG_CHECK(flag, 0b0110));
}

MU_TEST_SUITE(test_macros){
  MU_RUN_TEST(test_putils_move);
  MU_RUN_TEST(test_putils_offset);
  MU_RUN_TEST(test_flags_set);
  MU_RUN_TEST(test_flags_unset);
  MU_RUN_TEST(test_flags_transfer);
  MU_RUN_TEST(test_flags_check);
}

MU_TEST(test_seg_init) {
  reset_buffer();

  char *seg_st = buff;
  char *seg_end = &buff[8*UNIT_KB];
  seg_p seg = (seg_p) buff;
  sys_seg_init(seg_st, seg_end);
  mu_check(seg->next_seg == (seg_p) seg_end);
  mu_check(FLAG_CHECK(seg->flags, SEG_AVIL_FMSK|SEG_TERM_FMSK));
}

MU_TEST(test_seg_insert) {
  reset_buffer();
  char *seg_st = buff;
  char *seg_end = &buff[8*UNIT_KB];
  sys_seg_init(seg_st, seg_end);
  seg_p seg = (seg_p) buff;
  mu_check(0);


}

MU_TEST_SUITE(test_segments){
  MU_RUN_TEST(test_seg_init);
  MU_RUN_TEST(test_seg_insert);
}

int main(int argc, char **argv){
  puts("Testing macros...");
  MU_RUN_SUITE(test_macros);
  puts("Testing segments...");
  MU_RUN_SUITE(test_segments);
  MU_REPORT();
  return minunit_status;
}
