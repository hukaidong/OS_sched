#define MOCK_MALLOC
#define NUSER
#include "gvars.h"
#include "casts.h"
#include "segment.h"
#include "thread_entries.h"

#include "utils/utils.h"
#include "utils/minunit.h"

#include "test_gvar.h"

#include <string.h>
#include <malloc.h>
#include <sys/mman.h>

char vm_bkp[VM_SIZE];

void page_test_setup() {
  _enter_user_mode(0);
}

void page_test_teardown() {
  _enter_sys_mode();
  memcpy(vm_base, vm_bkp, VM_SIZE);
}

MU_TEST(test_page_dummy_check) {
  _enter_sys_mode();
  int sys_free = seg_find_preceeding_max_size((seg_p)sys_vm_base),
      share_free = seg_find_preceeding_max_size((seg_p)shared_vm_base);

  mu_assert_int_eq(2*UNIT_MB - 16*UNIT_KB - seg_s, sys_free);
  mu_assert_int_eq(16*UNIT_KB - seg_s, share_free);
}

MU_TEST(test_page_mode_swap) {
  _enter_sys_mode();
  _enter_user_mode(0);
  mu_check(1);
}

MU_TEST_SUITE(test_page_basic) {
  MU_SUITE_CONFIGURE(&page_test_setup, &page_test_teardown);
  MU_RUN_TEST(test_page_dummy_check);
  MU_RUN_TEST(test_page_mode_swap);
}

void test_page() {
  _enter_sys_mode();
  __ds_init();
  mprotect(vm_base, VM_SIZE, P_RW);
  memcpy(vm_bkp, vm_base, VM_SIZE);

  puts("\n\t--- Testing paging (basic) ---");
  MU_RUN_SUITE(test_page_basic);
}
#undef MOCK_MALLOC
#undef NUSER
