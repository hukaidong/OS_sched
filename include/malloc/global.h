#ifndef MY_MALLOC_GLOBAL_H
#define MY_MALLOC_GLOBAL_H

#include "type.h"

#define UNIT_MB 0x100000
#define UNIT_KB 0x000400

void *static_head = NULL;

void *vm_base = NULL,
     *shared_vm_base,
     *sys_vm_base;
page_entry page_belongs[6*UNIT_KB/4];

#define VM_SIZE (8*UNIT_MB)

#define PAGE_MSK (8*UNIT_MB-4*UNIT_KB)
#define PAGE_OFFSET 12


ssize_t pointer_2_page_index(void *p)
{ return ((unsigned long int)(p) & PAGE_MSK) >> PAGE_OFFSET; }

void *page_index_2_base(ssize_t idx)
{ return (char *)vm_base + ((unsigned long int)idx << PAGE_OFFSET); }

#endif /* ifndef MY_MALLOC_GLOBAL_H */
