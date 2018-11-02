#include "include/my_malloc.h"


void *myallocate(int size, char *fname,int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);
  // get current thread id
  // get current page list
  // if (size + sizeof(seg) > mem per page)
  //   if thread's pagenum + sizeof pages > PAGELIM
  //      return NULL;
  //   else
  //      thread's pagenum += sizeof pages
  //      alloc new page with desired size
  // else foreach page belongs to thread id
  //   if (size < page->free_seg_max)
  //    seg = seg_find_avail(page_base, size)
  //    seg_insert(seg, size)
  // else /* no suitable page */
  //   if thread's pagenum + 1 > PAGELIM
  //     return null
  //   else
  //     alloc new page of size 1
  //     thread's pagenum += 1
  //     alloc page with desired size
  //
  return NULL;
}

void mydeallocate(void* pointer, char *fname, int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  UNUSED(flags);

  // get segheader from pointer
  // segsize = segheader->size
  // if (segsize + sizeof(seg) > mem per page)
  //   split seg into {segpage, seglast}
  //   release (n-1 pages)
  //   thread's pagenum - (n-1)
  // freesize = seg_free(seglast)
  // if (freesize > segheaders->maxavail)
  //   segheader->maxavail = freesize
  //   if (freesize + sizeof(seg) = mem per page)
  //   release (last page)
  //   thread's pagenum - 1

}

