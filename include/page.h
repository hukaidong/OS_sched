#ifndef MY_MALLOC_PAGE_H
#define MY_MALLOC_PAGE_H value

#include "types.h"

void _page_setup();
void _page_protect(ssize_t pidx);
void _page_unprotect(ssize_t pidx);
void *new_page(size_t size_req, ssize_t thread_id);
void release_page(ssize_t pageid, ssize_t thread_id);
void *page_id2page(ssize_t pageid);
void page_assign(ssize_t index_i, ssize_t thread_id);
void page_segfault_handler (int sig, siginfo_t *si, void *_);
int remove_swap_page(ssize_t thread_id, ssize_t page_idx);
ssize_t any_page_has_free_size(ssize_t thread_id, int size);


#endif /* ifndef MY_MALLOC_PAGE_H */
