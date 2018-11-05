#ifndef MALLOC_THREAD_ENTRIES_H
#define MALLOC_THREAD_ENTRIES_H

#include "types.h"
#include "utils/utils.h"

void __ds_init();
int search_thread(ssize_t thread_id, tNode **target);

#endif /* ifndef MALLOC_THREAD_ENTRIES_H */
