#include <stdlib.h>

typedef struct page_entry {
  int belongs_to;
  int page_size;
  int free_seg_max;
} page_entry;

void page_segfault_handler (int sig, siginfo_t *si, void *_);
