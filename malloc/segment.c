#include "include/type.h"
#include "include/segment.h"

// return max size of segment available
void sys_seg_init(void *pte, void *pte_end ) {
  seg_p seg = (seg_p) pte;
  seg->prev_seg = NULL;
  seg->next_seg = (seg_p)pte_end;
  seg->flags = 0;

  FLAG_SET(seg->flags, SEG_AVIL_FMSK|SEG_TERM_FMSK);
}

// return max size of segment available
int seg_init(void *pte, int page_num, int size_req) {
  seg_p seg = (seg_p) pte;
  seg->prev_seg = NULL;
  seg->next_seg = seg;
  seg->flags = 0;

  ABSOLUTE_MOVE(seg->next_seg, page_num * PAGE_SIZE);
  FLAG_SET(seg->flags, SEG_AVIL_FMSK|SEG_TERM_FMSK);

  return seg_insert(seg, size_req);
}

// return first available segment pointer
seg_p seg_find_avail(void *pte, int size) {
  seg_p seg = (seg_p) pte;
  int size_req = size + sizeof(*seg);

  while (
      !FLAG_CHECK(seg->flags, SEG_AVIL_FMSK) ||
      ABSOLUTE_OFFSET(seg->next_seg, seg) < size_req ) {
    seg = seg->next_seg;
  }
  return seg;
}

// return remain size of segment available
int seg_insert(seg_p seg, int size) {
  int space = ABSOLUTE_OFFSET(seg->next_seg, seg),
    rest_space = space - size + 2*(int)sizeof(*seg);
  FLAG_UNSET(seg->flags, SEG_AVIL_FMSK);
  if (rest_space > 0) {
    seg_p new_seg = seg;
    ABSOLUTE_MOVE(new_seg, size + sizeof (*seg));
    seg->next_seg = new_seg;

    new_seg->prev_seg = seg;
    new_seg->next_seg = seg->next_seg;
    new_seg->flags = 0;
    FLAG_SET(new_seg->flags, SEG_AVIL_FMSK);

    if (FLAG_CHECK(seg->flags, SEG_TERM_FMSK)) {
      FLAG_UNSET(seg->flags, SEG_TERM_FMSK);
      FLAG_SET(new_seg->flags, SEG_TERM_FMSK);
    } else {
      new_seg->next_seg->prev_seg = new_seg;
    }
    return rest_space;

  } else {
    return 0;
  }
}

// return max size of segment available
int seg_find_preceeding_max_size(seg_p seg) {
  int max = 0;
  do {
    if(FLAG_CHECK(seg->flags, SEG_AVIL_FMSK)) {
      max = MAX(max, ABSOLUTE_OFFSET(seg->next_seg, seg) - (int)sizeof(*seg));
    };

    if(FLAG_CHECK(seg->flags, SEG_TERM_FMSK)) { break; }

    seg = seg->next_seg;
  } while (1);
  return max;
}

// return max size of segment available around freed seg
int seg_free(seg_p seg) {
  FLAG_SET(seg->flags, SEG_AVIL_FMSK);
  seg_p free_seg_head = seg;
  if (seg->prev_seg != NULL &&
      FLAG_CHECK(seg->prev_seg->flags, SEG_AVIL_FMSK)) {
    free_seg_head = seg->prev_seg;
    free_seg_head->next_seg = seg->next_seg;
  }
  if (!FLAG_CHECK(seg->flags, SEG_TERM_FMSK) &&
      FLAG_CHECK(seg->next_seg->flags, SEG_AVIL_FMSK)) {
    free_seg_head->next_seg = seg->next_seg->next_seg;
    FLAG_TRANSFER(free_seg_head->flags,
        seg->next_seg->flags, SEG_TERM_FMSK);
  }
  return ABSOLUTE_OFFSET(free_seg_head->next_seg, free_seg_head) - sizeof(*seg);
}

seg_p seg_page_split(seg_p seg, int after_n_page) {
    seg_p new_seg = seg;
    ABSOLUTE_MOVE(new_seg, PAGE_SIZE *after_n_page);

    new_seg->prev_seg = NULL;
    new_seg->next_seg = seg->next_seg;
    new_seg->flags = 0;
    FLAG_UNSET(new_seg->flags, SEG_AVIL_FMSK);

    return new_seg;
}
