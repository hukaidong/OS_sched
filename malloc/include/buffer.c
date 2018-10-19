#include "buffer.h"

struct _struct_SEG_T {
  _struct_SEG_T *prev;
  _struct_SEG_T *next;
  _bits_FLAG_T  flags;
  char buf[0];
};

struct _struct_PAGE_T {
  _struct_PAGE_T *next;
  _bits_FLAG_T flags;
  unsigned int owner;
  char buf[0];
};

inline _struct_SEG_T *_top_seg_from_page (
    _struct_PAGE_T *page) {
  return (_struct_SEG_T *) page->buf;
}

inline _struct_SEG_T *_btm_seg_from_page (
    _struct_PAGE_T *page) {
  return ((_struct_SEG_T *) (page->next)) - 1;
}


void __buf_init () {
  _struct_PAGE_T* page = (_struct_PAGE_T*) __sys_buf;
  _struct_PAGE_T* page_end = (_struct_PAGE_T*) (__sys_buf + BUF_SIZE);
  while (1) {
    page->next = (_struct_PAGE_T*)((char*)page + PAGE_SIZE);
    page->owner = 0;
    page->flags = BUFF_FREE;

    _struct_SEG_T *initseg = _top_seg_from_page(page);
    _struct_SEG_T *termseg = _btm_seg_from_page(page);

    initseg->prev = initseg;
    initseg->next = termseg;
    initseg->flags = BUFF_FREE;
    termseg->prev = initseg;
    termseg->next = termseg;
    initseg->flags = BUFF_TERM;

    if (page->next > page_end) {
      page->flags |= BUFF_TERM;
      break;
    } else {
      page = page->next;
    }
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
