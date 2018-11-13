#define NUSER

#include "pcb.h"
#include "casts.h"

#include <sys/mman.h>


void _thread_purge(ssize_t thread_id) {
  int pidx;
  for(pidx=0; pidx<PCB_SIZE; pidx++) {
    if(pcb[pidx].thread_id == thread_id) {
      pcb[pidx].thread_id = -1;
      pcb[pidx].max_avail = -1;
      mprotect(page_index_2_base(pidx), PAGE_SIZE, P_N);
    }
  }
}

ssize_t pcb_next_free_page(ssize_t thread_id, int size) {
  ssize_t p = pwander, rsize=size;
  for (; p<PCB_SIZE; p++) {
    if(pcb[p].thread_id < 0) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  for (p=0; p<pwander; p++) {
    if(pcb[p].thread_id < 0) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  return -1;
}

ssize_t pcb_next_swapable_page(ssize_t thread_id, int size) {
  ssize_t p = pwander, rsize=size;
  for (; p<PCB_SIZE; p++) {
    if(pcb[p].thread_id != thread_id) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }

  for (p=0; p<pwander; p++) {
    if(pcb[p].thread_id != thread_id) {
      if (rsize == 1) {
        pwander = p;
        return p - size;
      }
      rsize--;
    } else {
      rsize = size;
    }
  }
  return -1;
}

#undef NUSER
