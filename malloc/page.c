#include "include/type.h"
#include "include/page.h"
#include "include/global.h"


void _UGLY_setup_virtual_mem() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
}


