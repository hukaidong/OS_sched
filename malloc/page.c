#include "include/type.h"
#include "include/page.h"
#include "include/global.h"


void _UGLY_setup_virtual_mem() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
}

void *new_page(int size_req, int thread_id) {
  int req_page_num = (size_req + sizeof(segment_header)) / PAGE_SIZE + 1;
  //   if thread's pagenum + req_page_num > PAGELIM
  //     return NULL;
  //  (else:)
  //   thread's pagenum += req_page_num;
  //   if find new free page by page num as index_i
  //     for all index_i ... index_i + req_page_num
  //       page_p = vm_base | (index_i << page_offset)
  //       page_p->thread_id = thread_id
  //       page_p->maxfree = 0
  //     for page inedx_i
  //       page_p = vm_base | (index_i << page_offset)
  //       maxfree = seg_init(page_p, req_page_num, sizq_req);
  //       page_p->thread_id = thread_id
  //       page_p->maxfree = maxfree
  //     return ((seg_p)page_p)->buf
  //   else find new page by swap out page owned by other thread
  //     for all index_i ... index_i + req_page_num
  //       swap out page i to disk
  //       page_p = vm_base | (index_i << page_offset)
  //       page_p->thread_id = thread_id
  //       page_p->maxfree = 0
  //     for page inedx_i
  //       page_p = vm_base | (index_i << page_offset)
  //       maxfree = seg_init(page_p, req_page_num, sizq_req);
  //       page_p->thread_id = thread_id
  //       page_p->maxfree = maxfree
  //     return ((seg_p)page_p)->buf
}

void release_page(int pageid) {

}

