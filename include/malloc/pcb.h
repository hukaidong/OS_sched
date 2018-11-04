#ifndef MY_MALLOC_PCB_H
#define MY_MALLOC_PCB_H value
#include "utils/utils.h"

ssize_t pcb_next_free_page(ssize_t thread_id, int size);
ssize_t pcb_next_swapable_page(ssize_t thread_id, int size);
void _enter_user_mode(ssize_t thread_id);
void _thread_purge(ssize_t thread_id);
void _enter_sys_mode();

#endif /* ifndef MY_MALLOC_PCB_H */
