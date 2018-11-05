#include "casts.h"
#include "types.h"
#include "gvars.h"
#include "utils/utils.h"
#include <sys/mman.h>

ssize_t pointer_2_page_index(void *p)
{ return ((unsigned long int)(p) & PAGE_MSK) >> PAGE_OFFSET; }

void *page_index_2_base(ssize_t idx)
{ return (char *)vm_base + ((unsigned long int)idx << PAGE_OFFSET); }

void _enter_sys_mode() {
  GML = 1;
  mprotect(sys_vm_base, sys_vm_size, P_RW);
}

void _enter_user_mode(ssize_t thread_id) {
  int pidx;
  mprotect(sys_vm_base, sys_vm_size, P_N);

  for(pidx=0; pidx<PCB_SIZE; pidx++) {
    if(pcb[pidx].thread_id == thread_id) {
      mprotect(page_index_2_base(pidx), PAGE_SIZE, P_RW);
    }
  }
  GML = 0;
}

void * UCT_P2STCK_P(uctx_p ctx)     { return ctx->uc_stack.ss_sp; }
void * FIB_P2STCK_P(fib_p fib)      { return UCT_P2STCK_P(&(fib->uctx)); }
fib_p  UCTX_P2FIB_P(uctx_p ctx)     { return (fib_p)UCT_P2STCK_P(ctx); }
uctx_p FIB_P2UCTX_P(fib_p fib)      { return &(fib->uctx); }

ssize_t GetCurrentThreadId() {
  return (long int)(UCT_P2STCK_P(current_ctx)) & ((~0ull) >> 1);
}

void init_thread(ssize_t thread_id) {
  tNode* new_node = (tNode*) _lib_malloc(sizeof(tNode));
  new_node->thread_id = thread_id;
  new_node->pHead = NULL;
  new_node->next = tHead;
  tHead = new_node;
}

int search_thread(ssize_t thread_id, tNode **target) {
  tNode* current = tHead;
  while (current != NULL) {
    if (current->thread_id == thread_id) {
      *target = current;
      return 1;
    }
    current = current->next;
  }
  return -1;
}

void delete_thread(ssize_t key) {
  tNode *temp = tHead, *prev;

  if (temp != NULL && temp->thread_id == key) {
    // TODO: free pNodes?
    tHead = temp->next;
    _lib_free(temp);
    return;
  }

  while (temp != NULL && temp->thread_id != key) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) return;
  prev->next = temp->next;

  _lib_free(temp);
}

