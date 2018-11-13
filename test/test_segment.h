#define MOCK_MALLOC
#define NUSER
#include "types.h"
#include "const.h"
#include "segment.h"

#include "utils/utils.h"
#include "utils/minunit.h"

#include "test_gvar.h"

#include <string.h>

char buff[8*UNIT_KB];
char buff_bkp[8*UNIT_KB];

MU_TEST(test_putils_move) {
  int *iptr = (int *)buff;
  ABSOLUTE_MOVE(iptr, 40*UNIT_KB);
  mu_check(iptr == (int*)(&buff[40*UNIT_KB]));
}

MU_TEST(test_putils_offset) {
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

void seg_test_setup() {
  memset(buff, 0, 8*UNIT_KB);
}

void seg_test_teardown() {
}


MU_TEST(test_seg_init) {
  char *seg_st = buff;
  char *seg_end = &buff[8*UNIT_KB];
  seg_p seg = (seg_p) buff;
  sys_seg_init(seg_st, seg_end);
  mu_check(seg->next_seg == (seg_p) seg_end);
  mu_check(FLAG_CHECK(seg->flags, SEG_AVIL_FMSK|SEG_TERM_FMSK));
}

MU_TEST(test_seg_insert) {
  char *seg_st = buff;
  char *seg_end = &buff[8*UNIT_KB];
  sys_seg_init(seg_st, seg_end);
  seg_p seg = (seg_p) buff;
  mu_assert_int_eq(8*UNIT_KB, ABSOLUTE_OFFSET(seg->next_seg, seg));
  int remain = seg_insert(seg, 1*UNIT_KB-seg_s);

  mu_assert_int_eq(1*UNIT_KB, ABSOLUTE_OFFSET(seg->next_seg, seg));
  mu_assert_int_eq((7*UNIT_KB-seg_s), remain);
  mu_assert(seg==seg->next_seg->prev_seg,
      "Incorrect retriving back to head segment");

  seg_p seg1 = seg->next_seg;
  remain = seg_insert(seg1, 1*UNIT_KB-seg_s);
  mu_assert_int_eq((6*UNIT_KB-seg_s), remain);
  mu_assert_int_eq(1*UNIT_KB, ABSOLUTE_OFFSET(seg1->next_seg, seg1));
  mu_assert(seg1==seg1->next_seg->prev_seg,
      "Incorrect retriving back to head segment");

  seg_p seg2 = seg1->next_seg;
  remain = seg_insert(seg2, remain-2);
  mu_assert_int_eq(0, remain);

  mu_assert(!FLAG_CHECK(seg->flags, SEG_TERM_FMSK),
      "Incorrect non-termination flags set");
  mu_assert(FLAG_CHECK(seg2->flags, SEG_TERM_FMSK),
      "Incorrect termination flags set");
}

MU_TEST(test_seg_free) {
  char *seg_st = buff;
  char *seg_end = &buff[8*UNIT_KB];
  sys_seg_init(seg_st, seg_end);
  seg_p seg = (seg_p) buff;
  int remain = seg_insert(seg, 7*UNIT_KB-seg_s);
  seg_p seg_7k = seg->next_seg;
  seg_insert(seg_7k, remain);
  seg_free(seg);
  mu_check(seg==seg_7k->prev_seg);

  mu_assert(FLAG_CHECK(seg->flags, SEG_AVIL_FMSK),
      "seg free available flag not setted");
  mu_assert(!FLAG_CHECK(seg_7k->flags, SEG_AVIL_FMSK),
      "seg unfreed available flag check failed");

  remain = seg_insert(seg, 5*UNIT_KB-seg_s);
  seg_p seg_5k = seg->next_seg;
  mu_assert_int_eq(2*UNIT_KB-seg_s, remain);
  mu_check(seg==seg_5k->prev_seg);
  mu_check(seg_7k==seg_5k->next_seg);

  seg_insert(seg->next_seg, 1*UNIT_KB-seg_s);
  seg_p seg_6k = seg->next_seg->next_seg;

  /* Topology check */
  mu_check(seg_6k != seg_7k);
  mu_check(seg_5k==seg_6k->prev_seg);
  mu_check(seg_6k==seg_7k->prev_seg);
  mu_check(seg_6k==seg_5k->next_seg);
  mu_check(seg_7k==seg_6k->next_seg);

  seg_free(seg);

  remain = seg_insert(seg, 3*UNIT_KB-seg_s);
  seg_insert(seg->next_seg, 1*UNIT_KB-seg_s);
  seg_free(seg);
  remain = seg_insert(seg, 1*UNIT_KB-seg_s);
  seg_insert(seg->next_seg, 1*UNIT_KB-seg_s);
  seg_free(seg);
  /*
   * assume current mem struct is
   * |free|used|free|used|free|used|free|used|
   */
  seg_p cursor = seg;
  // Travel through segments
  int ticking=0;
  while (!FLAG_CHECK(cursor->flags, SEG_TERM_FMSK)) {
    ticking++;
    cursor = cursor->next_seg;
  }
  mu_assert_int_eq(8*UNIT_KB, ABSOLUTE_OFFSET(cursor->next_seg, seg));
  while (cursor->prev_seg != NULL) {
    ticking++;
    cursor = cursor->prev_seg;
  }
  mu_assert(cursor==seg, "Travel through segments failed");
  mu_assert_int_eq(14, ticking);
  memcpy(buff_bkp, buff, 8*UNIT_KB);
}

MU_TEST_SUITE(test_segments_prep){
  MU_SUITE_CONFIGURE(&seg_test_setup, &seg_test_teardown);
  MU_RUN_TEST(test_seg_init);
  MU_RUN_TEST(test_seg_insert);
  MU_RUN_TEST(test_seg_free);
}

void seg_cplx_setup(){
  memcpy(buff, buff_bkp, 8*UNIT_KB);
}

void seg_cplx_teardown(){
}

MU_TEST(cplx_seg_finds) {
  seg_p seg = (seg_p) buff;
  int remain = seg_find_preceeding_max_size(seg);
  mu_assert_int_eq(1*UNIT_KB-seg_s, remain);

  mu_assert(seg == seg_find_avail(buff, remain),
      "Taking exactly fitted segment failed");
  mu_assert(seg == seg_find_avail(buff, 10),
      "Taking minor segment failed");
  mu_assert(NULL == seg_find_avail(buff, remain+1),
      "Unexcepted unfitted segment returned");

  seg_insert(seg, remain);
  seg_p new_seg = seg_find_avail(buff, remain);
  mu_assert( (new_seg!=seg) && (new_seg!=NULL),
      "Returning new seg failed");
  seg_insert(new_seg, 10);
  seg_p new_seg2 = seg_find_avail(buff, remain);
  mu_assert( (new_seg2!=new_seg) &&
             (new_seg2!=NULL), "Returning new seg failed");
  new_seg2 = seg_find_avail(buff, 10);
  mu_assert( (new_seg2) == (new_seg->next_seg),
      "Returning new seg failed");
}

MU_TEST(cplx_seg_combine) {
  seg_p seg = (seg_p) buff,
        seg1k = seg->next_seg,
        seg2k = seg1k->next_seg,
        seg3k = seg2k->next_seg,
        seg4k = seg3k->next_seg,
        seg5k = seg4k->next_seg,
        seg6k = seg5k->next_seg,
        seg7k = seg6k->next_seg;
  /*
   * assume current mem struct is
   * |0   |1   |2   |3   |4   |5   |6   |7   |
   * |free|used|free|used|free|used|free|used|
   */

  seg_free(seg1k);
  /*
   * assume current mem struct is
   * |0             |3   |4   |5   |6   |7   |
   * |free          |used|free|used|free|used|
   */
  int remain = seg_find_preceeding_max_size(seg);
  mu_assert_int_eq(3*UNIT_KB-seg_s, remain);
  mu_check(seg->next_seg = seg3k);
  mu_check(seg3k->prev_seg = seg);

  seg_free(seg7k);
  /*
   * assume current mem struct is
   * |0             |3   |4   |5   |6        |
   * |free          |used|free|used|free     |
   */
  mu_check(FLAG_CHECK(seg6k->flags, SEG_TERM_FMSK));
  mu_assert_int_eq(8*UNIT_KB,
      ABSOLUTE_OFFSET(seg6k->next_seg, seg));
  remain = seg_find_preceeding_max_size(seg4k);
  mu_assert_int_eq(2*UNIT_KB-seg_s, remain);

  seg_free(seg5k);
  seg_free(seg3k);
  /*
   * assume current mem struct is
   * |0                                      |
   * |free                                   |
   */
  mu_check(FLAG_CHECK(seg->flags, SEG_TERM_FMSK));
  remain = seg_find_preceeding_max_size(seg);
  mu_assert_int_eq(8*UNIT_KB-seg_s, remain);

}

MU_TEST_SUITE(test_segments_cplx){
  MU_SUITE_CONFIGURE(&seg_cplx_setup, &seg_cplx_teardown);
  MU_RUN_TEST(cplx_seg_finds);
  MU_RUN_TEST(cplx_seg_combine);

}

void test_segment(){
  puts("\n\t--- Testing macros ---");
  MU_RUN_SUITE(test_macros);
  puts("\n\t--- Testing segments (basic) ---");
  MU_RUN_SUITE(test_segments_prep);
  puts("\n\t--- Testing segments (more) ---");
  MU_RUN_SUITE(test_segments_cplx);
}

#undef MOCK_MALLOC
#undef NUSER
