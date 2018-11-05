#ifndef MALLOC_THREAD_ENTRIES_H
#define MALLOC_THREAD_ENTRIES_H

#ifdef MOCK_MALLOC
#include <stdlib.h>
#define _lib_malloc malloc
#define _lib_free free
#else
#include "my_malloc.h"
#endif

#include "types.h"
#include "utils/utils.h"

void __ds_init();
int search_thread(ssize_t thread_id, tNode **target);

#endif /* ifndef MALLOC_THREAD_ENTRIES_H */
