#ifndef MY_BUFFER_H
#define MY_BUFFER_H

#include <unistd.h>

typedef char _bits_FLAG_T;

struct _struct_SEG_T;
struct _struct_SEG_INF_T;
#define SEG_T_SIZE sizeof(_struct_SEG_T)

struct _struct_PAGE_T;
struct _struct_PAGE_INF_T;
#define PAGE_T_SIZE sizeof(_struct_PAGE_T)



#define BUF_SIZE 8388608  // 8 MB buffer
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

#define BUFF_OWND 0x01
#define BUFF_TERM 0x02
#define BUFF_FREE ~BUFF_OWND

char __sys_buf[BUF_SIZE];

inline _struct_SEG_T *_top_seg_from_page (_struct_PAGE_T *page);
inline _struct_SEG_T *_btm_seg_from_page (_struct_PAGE_T *page);

#endif /* ifndef MY_BUFFER_H */
