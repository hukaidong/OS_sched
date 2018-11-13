#include "test_segment.h"
#include "test_page.h"
#include "utils/minunit.h"

int main() {
  test_segment();
  test_page();
  MU_REPORT();
  return minunit_status;
}
