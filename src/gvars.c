#include "utils/utils.h"
#include "types.h"

void *static_head = NULL;

void *vm_base = NULL,
     *shared_vm_base,
     *sys_vm_base;

page_entry pcb[PCB_SIZE];

uctx_p current_ctx = NULL;
ssize_t pwander = 0;
int volatile GML = 0;

void *EXIT_THD_P;
ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX, MAIN_CTX;
Quctx QThreadH, QThreadM, QThreadL;
int thread_detached = 0;

sNode *s_head;
f_stack_t f_stack;
int swap_fd;

tNode *tHead;
ssize_t file_tail_pos;
int thread_idx_accu = 1;
