#include <signal.h>


#include "include/type.h"
#include "include/page.h"
#include "include/global.h"
#include "include/my_malloc.h"
#include "include/segment.h"


void _page_setup() {
  posix_memalign(&vm_base, VM_SIZE, VM_SIZE);
  sys_vm_base = (char*)vm_base + (6*UNIT_MB);
  shared_vm_base = (char*)vm_base +(8*UNIT_MB) - (16*UNIT_KB);
  sys_seg_init(sys_vm_base, shared_vm_base);
  sys_seg_init(shared_vm_base, (char*)vm_base+VM_SIZE);


  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = page_segfault_handler;
  sigaction(SIGSEGV, &sa, NULL);

}

void *new_page(int size_req, int thread_id) {
  //   int req_page_num = (size_req + sizeof(segment_header)) / PAGE_SIZE + 1;
  int req_page_num = (size_req + sizeof(segment_header)) / PAGE_SIZE + 1;
  //   if thread's pagenum + req_page_num > PAGELIM
  if(req_page_num > page_size)
    return NULL;
  //     return NULL;
  //  (else:)
  else{
    
  }
  //   thread's pagenum += req_page_num;
  //   if find new free page by page num as index_i
  //   else find new page by swap out page owned by other thread
  //   for all index_i ... index_i + req_page_num
  //     page_assign(i, thread)
  //   maxfree = seg_init(page_id2page(inedx_i), req_page_num, size_req);
  //   page[index_i].maxfree = maxfree
  //   return page_id2page(index_i) + sizeof(seghead)
}

void release_page(int pageid, int thread_id) {
  // if pageid not belongs to thread_id
  if(page_belongs[pageid].thread_id!= thread_id){
    page_swap_out(pageid);
    page_swap_in_virtual(pageid,thread_id);
    page_belongs[pageid].thread_id = -1;
  }
  //   page_swap_out(pageid);
  //   page_swap_in_virtual(pageid, threadid);
  // page[pageid].thread_id = -1;
}

void *page_id2page(int pageid) {
  // return vm_base | pageid << PAGE_MASK_OFFSET;
  return vm_base | pageid << PAGE_OFST;
}

void page_assign(int index_i, int thread_id) {
  // page[i].thread_id = thread_id
  page_belongs[index_i].thread_id = thread_id;
  // page[i].maxfree = 0
  page_belongs[index_i].max_avail = 0;
  // mprotect(page_buf, pagesize, PROT_READ | PROT_WRITE);
  mprotect(__sys_buf,page_belongs[index_i].page_size, PROT_READ | PROT_WRITE);
}

void page_swap_out(int index_i) {
  // threadid = page[index_i].thread_id
  int threadid = page_belongs[index_i].thread_id;
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

void page_swap_in_virtual(int index_i, int thread_id) {
  // swap page but not do memcpy (faster release page)
  // pos = thread.file_swap.pop(index_i)
  // file_seg.push_back(pos)
  // mprotect(page_buf, pagesize, PROT_READ | PROT_WRITE);
}

void page_segfault_handler (int sig, siginfo_t *si, void *_) {
  UNUSED(sig);
  UNUSED(_);
  void *addr = si->si_addr;
  // thread_id = get_thread_id();
  // int page_id = (addr & PAGE_MSK) >> PAGE_OFFSET;
  // page_swap_out(pageid);
  // page_svap_in(pageid, thread_id);


}
