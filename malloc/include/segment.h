#include "type.h"

int seg_init(void *pte, int page_num, int size_req);
int seg_insert(seg_p seg, int size);
int seg_free(seg_p seg);
int seg_find_preceeding_max_size(seg_p seg);

seg_p seg_find_avail(void *pte, int size);
seg_p seg_page_split(seg_p seg, int after_n_page);
