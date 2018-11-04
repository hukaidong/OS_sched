#ifndef MY_MALLOC_GLOBAL_H
#define MY_MALLOC_GLOBAL_H

#include <sys/mman.h>
#include "type.h"
#include "pthread/type.h"

#define UNIT_MB 0x100000
#define UNIT_KB 0x000400
#define PAGE_LIM_PER_THREAD 1*UNIT_KB

void *static_head = NULL;

void *vm_base = NULL,
     *shared_vm_base,
     *sys_vm_base;

#define PCB_SIZE ((6*UNIT_MB) / (4*UNIT_KB))
#define sys_vm_size ((2*UNIT_MB) - (16*UNIT_KB))
#define P_RW (PROT_READ | PROT_WRITE)
#define P_N  (PROT_NONE)
page_entry pcb[PCB_SIZE];

ssize_t pwander = 0;
#define VM_SIZE (8*UNIT_MB)
#define PAGE_MSK (8*UNIT_MB-4*UNIT_KB)
#define PAGE_OFFSET 12


ssize_t pointer_2_page_index(void *p)
{ return ((unsigned long int)(p) & PAGE_MSK) >> PAGE_OFFSET; }

void *page_index_2_base(ssize_t idx)
{ return (char *)vm_base + ((unsigned long int)idx << PAGE_OFFSET); }

void _enter_sys_mode() {
  GML = 1;
  mprotect(sys_vm_base, sys_vm_size, P_RW);
}

void _enter_user_mode(ssize_t thread_id) {
  int pidx;
  mprotect(sys_vm_base, sys_vm_size, P_N);

  for(pidx=0; pidx<PCB_SIZE; pidx++) {
    if(pcb[pidx].thread_id == thread_id) {
      mprotect(page_index_2_base(pidx), PAGE_SIZE, P_RW);
    }
  }
  GML = 0;
}
void init_thread(ssize_t thread_id);
void delete_thread(ssize_t thread_id);

#endif /* ifndef MY_MALLOC_GLOBAL_H */
