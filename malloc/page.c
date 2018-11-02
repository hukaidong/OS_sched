#include "include/type.h"
#include "include/page.h"
#include "include/global.h"


void _UGLY_setup_virtual_mem() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
}

void *new_page(int size_req, int thread_id) {
  //   int req_page_num = (size_req + sizeof(segment_header)) / PAGE_SIZE + 1;
  //   if thread's pagenum + req_page_num > PAGELIM
  //     return NULL;
  //  (else:)
  //   thread's pagenum += req_page_num;
  //   if find new free page by page num as index_i
  //   else find new page by swap out page owned by other thread
  //   for all index_i ... index_i + req_page_num
  //     page_assign(i, thread)
  //   maxfree = seg_init(page_id2page(inedx_i), req_page_num, size_req);
  //   page[index_i].maxfree = maxfree
  //   return page_id2page(index_i) + sizeof(seghead)
}

void release_page(int pageid) {

}

void *page_id2page(int pageid) {
  // return vm_base | pageid << PAGE_MASK_OFFSET;
}

void page_assign(int index_i, int thread_id) {
  // page[i].thread_id = thread_id
  // page[i].maxfree = 0
  // mprotect(page_buf, pagesize, PROT_READ | PROT_WRITE);
}

void page_swap_out(int index_i) {
  // threadid = page[index_i].thread_id
  // pos = file_seg.pop
  // if not pos
  //   pos = file_tail_pos
  //   file_tail_pos++;
  // swap_to_file(pos, index_i)
  // thread.file_swap.push_back (index, pos)
  // mprotect(page_buf, pagesize, PROT_NONE);
}

void page_swap_in(int index_i, int thread_id) {
  // pos = thread.file_swap.pop(index_i)
  // file_seg.push_back(pos)
  // swap_from_file(pos, index_i);
  // mprotect(page_buf, pagesize, PROT_READ | PROT_WRITE);
}
