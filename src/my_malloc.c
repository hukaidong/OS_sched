#include "my_malloc.h"
#include "malloc/global.h"
#include "malloc/segment.h"
#include "malloc/page.h"
#include "malloc/pcb.h"
#include "pthread/type.h"
#include "utils/utils.h"


void *myallocate_s(int size, const char *fname,int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  if (flags & LIBRARYREQ) {
    seg_p seg = seg_find_avail(sys_vm_base, size);
    seg_insert(seg, size);
    return seg->buf;
  } else if (flags & SHAREDREQ) {
    seg_p seg = seg_find_avail(shared_vm_base, size);
    seg_insert(seg, size);
    return seg->buf;
  } else {
    long int thread_id = GetCurrentThreadId();
    ssize_t page_idx;
    if (size + sizeof(segment_header) > PAGE_SIZE)
      return new_page(size, thread_id);
    else if ((page_idx = any_page_has_free_size(thread_id, size)) >= 0) {
      seg_p seg = seg_find_avail(page_index_2_base(page_idx), size);
      seg_insert(seg, size);
      pcb[page_idx].max_avail = seg_find_preceeding_max_size(seg);
      return seg->buf;
    } else
      return new_page(size, thread_id);
  }
}

void *myallocate(int size, const char *fname,int lnum, char flags) {
  _enter_sys_mode();
  void *p = myallocate_s(size, fname, lnum, flags);
  long int thread_id = GetCurrentThreadId();
  _enter_user_mode(thread_id);
  return p;
}

void mydeallocate_s(void* pointer, const char *fname, int lnum, char flags) {
  UNUSED(fname);
  UNUSED(lnum);
  long int thread_id = GetCurrentThreadId();

  int pagenum, pidx=pointer_2_page_index(pointer);
  void *sp_seg = pointer;
  if (flags & (LIBRARYREQ | SHAREDREQ)) {
    sys_seg_free(pointer);
  } else {
    if((pagenum=seg_page_free(&sp_seg))>0) {
      for ( ;pagenum>0; pagenum--) {
        release_page(pidx, thread_id);
        pidx++;
      }
    }
    int free_size = seg_free((seg_p) sp_seg);
    if (pcb[pidx].max_avail < free_size)
      pcb[pidx].max_avail = free_size;
  }

}

void mydeallocate(void* pointer, const char *fname, int lnum, char flags) {
  _enter_sys_mode();
  mydeallocate_s(pointer, fname, lnum, flags);
  long int thread_id = GetCurrentThreadId();
  _enter_user_mode(thread_id);
}
