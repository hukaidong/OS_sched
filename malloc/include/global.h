#ifndef MY_MALLOC_GLOBAL_H
#define MY_MALLOC_GLOBAL_H

#include <unistd.h>

#define UNIT_MB 0x100000
#define UNIT_KB 0x000400

void *vm_base = NULL;
#define VM_SIZE (8*UNIT_MB)

#define PAGE_MSK (1*UNIT_MB-4*UNIT_KB)
#define PAGE_OFST 12
#define TPAGE_MSK (8*UNIT_MB-1*UNIT_MB)
#define TPAGE_OFST 20

#endif /* ifndef MY_MALLOC_GLOBAL_H */
