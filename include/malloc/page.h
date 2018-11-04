#ifndef MY_MALLOC_PAGE_H
#define MY_MALLOC_PAGE_H value

#include <signal.h>
#include "type.h"

void _page_setup();
void *new_page(size_t size_req, ssize_t thread_id);
void release(ssize_t pageid, ssize_t thread_id);
void *page_id2page(ssize_t pageid);
void page_assign(ssize_t index_i, ssize_t thread_id);
void page_swap_out(ssize_t index_i);
void page_swap_in(ssize_t index_i, ssize_t thread_id);
void page_swap_in_virtual(ssize_t index_i, ssize_t thread_id);
void page_segfault_handler (int sig, siginfo_t *si, void *_);

#endif /* ifndef MY_MALLOC_PAGE_H */
