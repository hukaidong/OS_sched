#include "include/buffer.h"

struct _struct_SEG_T {
  _struct_SEG_T *pseg;
  _struct_SEG_T *nseg;
  _bits_FLAG_T  flags;
  char buf[0];
};

struct _struct_PAGE_T {
  _struct_PAGE_T *next;
  _struct_PAGE_T *navl;
  _bits_FLAG_T flags;
  unsigned int owner;
  _struct_SEG_T head[1];
  char buf[0];
};

inline _struct_SEG_T *_top_seg_from_page (
    _struct_PAGE_T *page) {
  return page->head;
}

inline _struct_SEG_T *_btm_seg_from_page (
    _struct_PAGE_T *page) {
  return ((_struct_SEG_T *) (page->next)) - 1;
}

void __page_init(_struct_PAGE_T *page) {
  page->next = (_struct_PAGE_T*)((char*)page + PAGE_SIZE);
  page->owner = 0;
  page->flags &= ~BUFF_OWND;

  _struct_SEG_T *initseg = _top_seg_from_page(page);
  _struct_SEG_T *termseg = _btm_seg_from_page(page);
  initseg->pseg = initseg;
  initseg->nseg = termseg;
  initseg->flags = BUFF_FREE;
  termseg->pseg = initseg;
  termseg->nseg = termseg;
  initseg->flags = BUFF_TERM;
}

void __page_slice_segment(_struct_SEG_T *seg, size_t offset) {
  _struct_SEG_T *newseg = (_struct_SEG_T*)((char*)seg + offset);
  newseg->pseg = seg;
  newseg->nseg = seg->nseg;
  newseg->pseg->nseg = newseg;
  newseg->nseg->pseg = newseg;
  newseg->flags = BUFF_FREE;
}

void __page_try_release(_struct_PAGE_T *page, _struct_PAGE_T page_bkp) {
  if(page->head->nseg == _btm_seg_from_page(page)) {
    __page_init(page);
  } else {
    *page = page_bkp;
    page->head->nseg->pseg = page->head;
  }
}

void __page_expand_malloc(_struct_PAGE_T *page, int size) {
  _struct_PAGE_T *page_iter = page;
  int req_size = size + PAGE_T_SIZE + SEG_T_SIZE;
  int expanded = PAGE_SIZE;
  while (req_size > expanded) {
    page_iter = page_iter->next;
    expanded += PAGE_SIZE;
  }
  size = (size + PAGE_SIZE > expanded) ? size: size+PAGE_T_SIZE;
  *page = *page_iter;
  __page_slice_segment(page->head, size);
}

void __page_shrink(_struct_PAGE_T *page) {
  _struct_PAGE_T page_bkp = *page;
  page->flags &= ~BUFF_TERM;
  do {
    __page_init(page);
    page = page->next;
  } while(page->next < page_bkp.next);
  __page_try_release(page, page_bkp);
}

void __buf_init () {
  _struct_PAGE_T* page = (_struct_PAGE_T*) __sys_buf;
  _struct_PAGE_T* page_end = (_struct_PAGE_T*) (__sys_buf + BUF_SIZE);
  while (1) {
    page->flags = BUFF_FREE;
    __page_init(page);

    if (page->next > page_end) { page->flags |= BUFF_TERM; break; }
    else { page = page->next; }
  }
}

void __buf_deinit() {
}

void __attribute__ ((constructor)) constructor() {
  __buf_init();
}
void __attribute__ ((destructor)) destructor() {
  __buf_deinit();
}
