#include "include/type.h"
#include "include/page.h"
#include "include/global.h"


void _UGLY_setup_virtual_mem() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
}

void *new_page(int size_req, int thread_id) {
  int req_page_num = size_req + sizeof(segment_header);
  //   if thread's pagenum + req_page_num > PAGELIM
  //     return NULL;
  //  (else:)
  //   thread's pagenum += req_page_num;
  //   find new free page by page num as index_i
  //   page_p = vm_base | (index_i << page_offset)
  //   maxfree = seg_init(page_p, req_page_num, sizq_req);
  //   page_p->thread_id = thread_id
  //   page_p->maxfree = maxfree
  //   return ((seg_p)page_p)->buf
}


