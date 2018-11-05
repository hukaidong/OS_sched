#ifndef MY_CASTS_H
#define MY_CASTS_H value

#include "const.h"
#include "gvars.h"
#include "my_malloc.h"
#include "utils/utils.h"

ssize_t pointer_2_page_index(void *p);
void *page_index_2_base(ssize_t idx);

void * UCT_P2STCK_P(uctx_p ctx);
void * FIB_P2STCK_P(fib_p fib);
fib_p  UCTX_P2FIB_P(uctx_p ctx);
uctx_p FIB_P2UCTX_P(fib_p fib);

void _enter_sys_mode();
void _enter_user_mode(ssize_t thread_id);

void init_thread(ssize_t thread_id);
void delete_thread(ssize_t thread_id);

ssize_t GetCurrentThreadId();

#ifdef MOCK_MALLOC
#include <stdlib.h>
#define _lib_malloc(x) malloc(x)
#define _lib_free(x) free(x)
#else
#define _lib_malloc(x) myallocate(x, __FILE__, __LINE__, LIBRARYREQ)
#define _lib_free(x) mydeallocate(x, __FILE__, __LINE__, LIBRARYREQ)
#endif


#endif /* ifndef MY_CASTS_H */
