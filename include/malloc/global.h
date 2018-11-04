#ifndef MY_MALLOC_GLOBAL_H
#define MY_MALLOC_GLOBAL_H

#include "type.h"
#include "utils/linked_map_new.h"

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

typedef long int thread_id;
typedef unsigned int page_idx, fswap_idx;

define_lmap(file_seg_t, page_idx, fswap_idx);
define_lmap(thread_entry_map, thread_id, thread_entry);
lm_head_t(file_seg_t) file_seg;
lm_head_t(thread_entry_map) thread_seg;

page_idx pointer_2_page_index(void *p)
{ return ((unsigned long int)(p) & PAGE_MSK) >> PAGE_OFFSET; }

void *page_index_2_base(page_idx idx)
{ return (char *)vm_base + ((unsigned long int)idx << PAGE_OFFSET); }

#endif /* ifndef MY_MALLOC_GLOBAL_H */
