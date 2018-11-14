#include <signal.h>
#include <sys/mman.h>

#include "my_malloc.h"
#include "types.h"
#include "gvars.h"
#include "thread_entries.h"
#include "page.h"
#include "pcb.h"
#include "pfmap.h"
#include "segment.h"
#include "casts.h"

void _page_setup() {
  __ds_init();
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = page_segfault_handler;
  sigaction(SIGSEGV, &sa, NULL);
}

void _page_protect(ssize_t pidx) {
  mprotect(
      page_index_2_base(pidx),
      PAGE_SIZE, P_N);
}

void _page_unprotect(ssize_t pidx) {
  mprotect(
      page_index_2_base(pidx),
      PAGE_SIZE, P_RW);
}

void *new_page(size_t size_req, ssize_t thread_id) {
  int i;
  int req_page_num = (size_req + sizeof(segment_header)) / PAGE_SIZE + 1;
  ssize_t pidx;
  tNode* thread_e;
  search_thread(thread_id, &thread_e);
  if(thread_e->num_page_claimed > PAGE_LIM_PER_THREAD)
    return NULL;
  else{
    if((pidx = pcb_next_free_page(thread_id, req_page_num))<0) {
      pidx = pcb_next_swapable_page(thread_id, req_page_num);
      if (pidx >= 0) {
        for (i=0; i<req_page_num; i++) {
          insert_swap_page(pidx+i);
        }
      } else {
        return NULL;
      }
    }
    thread_e->num_page_claimed += req_page_num;
  }
  for (i=0; i<req_page_num; i++) {
    page_assign(pidx+i, thread_id);
  }
  int  maxfree = seg_init(page_index_2_base(pidx), req_page_num, size_req);
  pcb[pidx].max_avail = maxfree;
  return page_index_2_base(pidx) + sizeof(segment_header);
}

void release_page(ssize_t pidx, ssize_t thread_id) {
  if(pcb[pidx].thread_id!= thread_id){
    remove_swap_page(pidx, thread_id);
  }
  pcb[pidx].thread_id = -1;
  pcb[pidx].max_avail = -1;
  tNode* thread_e;
  search_thread(thread_id, &thread_e);
  thread_e->num_page_claimed--;
}

void page_assign(ssize_t pidx, ssize_t thread_id) {
  pcb[pidx].thread_id = thread_id;
  pcb[pidx].max_avail = 0;
  _page_unprotect(pidx);
}

void page_segfault_handler (int sig, siginfo_t *si, void *_) {
  UNUSED(sig);
  UNUSED(_);
  void *addr = si->si_addr;
  ssize_t thread_id = GetCurrentThreadId();
  ssize_t page_id = pointer_2_page_index(addr);
  insert_swap_page(page_id);
  pop_swap_page(page_id, thread_id);
}

ssize_t any_page_has_free_size(ssize_t thread_id, int size) {
  int pidx;
  for(pidx=0; pidx<PCB_SIZE; pidx++) {
    if(pcb[pidx].thread_id == thread_id &&
        pcb[pidx].max_avail > size) {
      return pidx;
    }
  }
  return -1;
}
