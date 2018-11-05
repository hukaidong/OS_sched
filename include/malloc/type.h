#ifndef MY_MALLOC_TYPE_H
#define MY_MALLOC_TYPE_H

#include "utils/utils.h"

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define ABSOLUTE_MOVE(p, offset) do {\
  p = (__typeof__ (p)) ((char *) p + offset);\
} while (0)
#define ABSOLUTE_OFFSET(p1, p2) \
  ((char*) p1 - (char*) p2)

#define FLAG_SET(flg, msk) do { flg |= msk; } while(0)
#define FLAG_UNSET(flg, msk) do { flg &= (~msk); } while (0)
#define FLAG_TRANSFER(flg, src, msk) \
  do { flg = ((flg & (~msk)) | (src & msk) ); } while (0)
#define FLAG_CHECK(flg, msk) ((flg & msk) == 0)

#define SEG_AVIL_FMSK 0x01
#define SEG_TERM_FMSK 0x02

typedef struct segment_header {
  struct segment_header *prev_seg;
  struct segment_header *next_seg;
  unsigned int    flags;
  char            buf[0];
} segment_header, *seg_p;

typedef struct page_entry {
  ssize_t  thread_id;
  size_t  max_avail;
} page_entry;


#define MAX(a,b) ({ __typeof__ (a) _a = (a);\
    __typeof__ (b) _b = (b); _a > _b ? _a : _b; })


#endif /* ifndef MY_MALLOC_TYPE_H */
