#include "malloc/thread_entries.h"
#include "malloc/global.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


tNode *tHead;
ssize_t file_tail_pos;

// TODO: Hook to my_pthread_create
void init_thread(ssize_t thread_id) {
  tNode* new_node = (tNode*) _lib_malloc(sizeof(tNode));
  new_node->thread_id = thread_id;
  new_node->pHead = NULL;
  new_node->next = tHead;
  tHead = new_node;
}

int search_thread(ssize_t thread_id, tNode **target) {
  tNode* current = tHead;
  while (current != NULL) {
    if (current->thread_id == thread_id) {
      *target = current;
      return 1;
    }
    current = current->next;
  }
  return -1;
}

// TODO: Hook to my_pthread_destroy
void delete_thread(ssize_t key) {
  tNode *temp = tHead, *prev;

  if (temp != NULL && temp->thread_id == key) {
    // TODO: free pNodes?
    tHead = temp->next;
    _lib_free(temp);
    return;
  }

  while (temp != NULL && temp->thread_id != key) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return;
  prev->next = temp->next;

  free(temp);
}

#include "malloc/page_file_map.h"

sNode *s_head;
f_stack_t f_stack;
int swap_fd;

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

void _f_stack_increase_capasity()
  // TODO: increase capasity!!
{ return; }

void f_stack_push_seg(ssize_t item)
{
  if (_f_stack_is_full())
    _f_stack_increase_capasity();
  f_stack.array[++f_stack.top] = item;
}

ssize_t f_stack_pop_seg()
{
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

void insert_swap_page(ssize_t thread_id, ssize_t page_idx) {
  ssize_t file_idx;
  if (_f_stack_is_empty()) {
    file_idx = file_tail_pos;
    file_tail_pos += 4*UNIT_KB;
  } else {
    file_idx = f_stack_pop_seg();
  }

  sNode *new_node = (sNode*)_lib_malloc(sizeof(sNode));
  new_node->thread_id = thread_id;
  new_node->page_idx = page_idx;
  new_node->file_idx = file_idx;
  new_node->next = s_head;
  s_head = new_node;
  swap_to_file(page_idx, file_idx);
}

int pop_swap_page(ssize_t thread_id, ssize_t page_idx) {
  ssize_t file_idx;
  sNode *temp = s_head, *prev;
  if (temp != NULL &&
      temp->thread_id == thread_id &&
      temp->page_idx == page_idx) {
    file_idx = temp->file_idx;
    _lib_free(temp);
    return 1;
  }

  if (temp != NULL && (
      temp->thread_id != thread_id ||
      temp->page_idx != page_idx)) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return -1;
  prev->next = temp->next;
  file_idx = temp->file_idx;
  free(temp);
  swap_to_mem(page_idx, file_idx);
  return 1;
}

