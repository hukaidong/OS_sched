#ifndef MY_GVARS_H
#define MY_GVARS_H value

#include "const.h"
#include "types.h"

extern void
*static_head, *vm_base, *shared_vm_base, *sys_vm_base,
*EXIT_THD_P;

extern page_entry pcb[PCB_SIZE];
extern ssize_t pwander, file_tail_pos;
extern int thread_detached, swap_fd, thread_idx_accu;
extern int volatile GML;
extern ucontext_t *current_ctx;

extern ucontext_t ENTRY_SCHED_CTX, ENTRY_EXIT_CTX, MAIN_CTX;
extern Quctx QThreadH, QThreadM, QThreadL;
extern sNode *s_head;
extern f_stack_t f_stack;
extern tNode *tHead;

#endif /* ifndef MY_GVARS_H */
