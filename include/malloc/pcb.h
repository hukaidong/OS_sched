#ifndef MY_MALLOC_PCB_H
#define MY_MALLOC_PCB_H value
#include "utils/utils.h"

ssize_t pcb_next_free_page(ssize_t thread_id, int size);
ssize_t pcb_next_swapable_page(ssize_t thread_id, int size);
void _thread_purge(ssize_t thread_id);

#endif /* ifndef MY_MALLOC_PCB_H */
