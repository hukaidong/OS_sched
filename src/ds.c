#define NUSER
#include "gvars.h"
#include "thread_entries.h"
#include "types.h"
#include "segment.h"
#include "casts.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>




#include "pfmap.h"


void init_file_and_stack() {
  f_stack.capacity = 20;
  f_stack.top = -1;
  f_stack.array = (ssize_t*) malloc(20*sizeof(ssize_t));
  swap_fd = open("/tmp/my_swap", O_RDWR);
}

int _f_stack_is_full()
{ return f_stack.top == f_stack.capacity - 1; }

int _f_stack_is_empty()
{ return f_stack.top == -1;  }

void _f_stack_increase_capasity() {
  int temp = f_stack.capacity;
  f_stack.capacity *= 2;
  ssize_t *new_array = (ssize_t*) malloc(f_stack.capacity*sizeof(ssize_t));
  memcpy(new_array, f_stack.array, temp*sizeof(ssize_t));
  free(f_stack.array);
  f_stack.array = new_array;
}

void f_stack_push_seg(ssize_t item) {
  if (_f_stack_is_full())
    _f_stack_increase_capasity();
  f_stack.array[++f_stack.top] = item;
}

ssize_t f_stack_pop_seg() {
  if (_f_stack_is_empty())
    return -1;
  return f_stack.array[f_stack.top--];
}

void swap_to_file(ssize_t page_idx, ssize_t file_idx) {
  pwrite(swap_fd, page_index_2_base(page_idx), 4*UNIT_KB, file_idx);
}

void swap_to_mem(ssize_t page_idx, ssize_t file_idx) {
  pread(swap_fd, page_index_2_base(page_idx), 4*UNIT_KB, file_idx);
}

void insert_swap_page(ssize_t page_idx) {
  ssize_t file_idx;
  if (_f_stack_is_empty()) {
    file_idx = file_tail_pos;
    file_tail_pos += 4*UNIT_KB;
  } else {
    file_idx = f_stack_pop_seg();
  }

  sNode *new_node = (sNode*)_lib_malloc(sizeof(sNode));
  new_node->pcb = pcb[page_idx];
  new_node->page_idx = page_idx;
  new_node->file_idx = file_idx;
  new_node->next = s_head;
  s_head = new_node;
  swap_to_file(page_idx, file_idx);
  pcb[page_idx].thread_id = -1;
  pcb[page_idx].max_avail = -1;
}

int remove_swap_page(ssize_t thread_id, ssize_t page_idx) {
  ssize_t file_idx;
  sNode *temp = s_head, *prev;
  if (temp != NULL &&
      temp->pcb.thread_id == thread_id &&
      temp->page_idx == page_idx) {
    file_idx = temp->file_idx;
    _lib_free(temp);
    return 1;
  }

  if (temp != NULL && (
      temp->pcb.thread_id != thread_id ||
      temp->page_idx != page_idx)) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return -1;
  prev->next = temp->next;
  file_idx = temp->file_idx;
  f_stack_push_seg(file_idx);
  free(temp);
  return 1;
}

int pop_swap_page(ssize_t thread_id, ssize_t page_idx) {
  ssize_t file_idx;
  sNode *temp = s_head, *prev;
  if (temp != NULL &&
      temp->pcb.thread_id == thread_id &&
      temp->page_idx == page_idx) {
    file_idx = temp->file_idx;
    _lib_free(temp);
    return 1;
  }

  if (temp != NULL && (
      temp->pcb.thread_id != thread_id ||
      temp->page_idx != page_idx)) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return -1;
  prev->next = temp->next;
  file_idx = temp->file_idx;
  free(temp);
  swap_to_mem(page_idx, file_idx);
  pcb[page_idx] = temp->pcb;
  f_stack_push_seg(file_idx);
  return 1;
}

#include <sys/mman.h>
#include "pcb.h"


void _thread_purge(ssize_t thread_id) {
  int pidx;
  for(pidx=0; pidx<PCB_SIZE; pidx++) {
    if(pcb[pidx].thread_id == thread_id) {
      pcb[pidx].thread_id = -1;
      pcb[pidx].max_avail = -1;
      mprotect(page_index_2_base(pidx), PAGE_SIZE, P_N);
    }
  }
}

ssize_t pcb_next_free_page(ssize_t thread_id, int size) {
  ssize_t p = pwander, rsize=size;
  for (; p<PCB_SIZE; p++) {
    if(pcb[p].thread_id < 0) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  for (p=0; p<pwander; p++) {
    if(pcb[p].thread_id < 0) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  return -1;
}

ssize_t pcb_next_swapable_page(ssize_t thread_id, int size) {
  ssize_t p = pwander, rsize=size;
  for (; p<PCB_SIZE; p++) {
    if(pcb[p].thread_id != thread_id) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  for (p=0; p<pwander; p++) {
    if(pcb[p].thread_id != thread_id) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }
  return -1;
}



void __ds_init() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
  sys_vm_base = (char*)vm_base + (6*UNIT_MB);
  shared_vm_base = (char*)vm_base +(8*UNIT_MB) - (16*UNIT_KB);
  sys_seg_init(sys_vm_base, shared_vm_base);
  sys_seg_init(shared_vm_base, (char*)vm_base+VM_SIZE);
  memset(pcb, -1, sizeof(pcb));
  mprotect(vm_base, 6*UNIT_MB, P_N);
}
#undef NUSER
