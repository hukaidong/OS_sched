#ifndef MY_MALLOC_PAGE_H
#define MY_MALLOC_PAGE_H value
#include <stdlib.h>
#include <signal.h>

typedef struct page_entry {
  int belongs_to;
  int page_size;
  int free_seg_max;
} page_entry;

void _page_setup();
void *new_page(int size_req, int thread_id);
void release(int pageid, int thread_id);
void *page_id2page(int pageid);
void page_assign(int index_i, int thread_id);
void page_swap_out(int index_i);
void page_swap_in(int index_i, int thread_id);
void page_swap_in_virtual(int index_i, int thread_id);
void page_segfault_handler (int sig, siginfo_t *si, void *_);
#endif /* ifndef MY_MALLOC_PAGE_H */
