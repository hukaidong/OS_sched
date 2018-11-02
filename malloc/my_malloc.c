#include "include/my_malloc.h"


void *myallocate(int size, char *fname,int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);
  // get current thread id
  // get current page list
  // if (size + sizeof(seg) > mem per page)
  //   alloc new page with desired size
  // else foreach page belongs to thread id
  //   if (size < page->free_seg_max)
  //    seg = seg_find_avail(page_base, size)
  //    seg_insert(seg, size)
  return NULL;
}
void *mydeallocate(int size, char *fname, int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);

  return NULL;
}

