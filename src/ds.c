#define NUSER
#include "casts.h"
#include "segment.h"
#include "thread_entries.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


void __ds_init() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
  sys_vm_base = (char*)vm_base + (6*UNIT_MB);
  shared_vm_base = (char*)vm_base +(8*UNIT_MB) - (16*UNIT_KB);
  sys_seg_init(sys_vm_base, shared_vm_base);
  sys_seg_init(shared_vm_base, (char*)vm_base+VM_SIZE);
  memset(pcb, -1, sizeof(pcb));
  mprotect(vm_base, 6*UNIT_MB, P_N);
  init_thread(0);
}
#undef NUSER
