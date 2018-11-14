#include "gvars.h"
#include "pfmap.h"
#include "casts.h"
#include "my_malloc.h"

#include <fcntl.h>
#include <string.h>

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

