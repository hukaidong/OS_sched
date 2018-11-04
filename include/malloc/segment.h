#ifndef MY_MALLOC_SEGMENT_H
#define MY_MALLOC_SEGMENT_H value
#include "type.h"

int seg_init(void *pte, int page_num, int size_req);
void sys_seg_init(void *pte, void *ptn_end);
int seg_insert(seg_p seg, int size);
int seg_free(seg_p seg);
int sys_seg_free(void* p);
int seg_find_preceeding_max_size(seg_p seg);

seg_p seg_find_avail(void *pte, int size);
int seg_page_free(void **p);

#endif /* ifndef MY_MALLOC_SEGMENT_H */
