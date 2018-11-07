#ifndef MY_UTIL_H
#define MY_UTIL_H value

#define UNUSED(x) (void)(x)

#include <signal.h>
#ifdef __APPLE__
#define _XOPEN_SOURCE
#include <sys/ucontext.h>
#else
#define _GNU_SOURCE
#include <ucontext.h>
#endif
#include <unistd.h>

#ifdef DEBUG
#include <stdio.h>
#define LOG(str) puts(#str)
#else
#define LOG(str)
#endif /* ifdef DEBUG */

#define ABSOLUTE_MOVE(p, offset) do {\
  p = (__typeof__ (p)) ((char *) p + offset);\
} while (0)
#define ABSOLUTE_OFFSET(p1, p2) \
  ((char*) p1 - (char*) p2)

#define FLAG_SET(flg, msk) do { flg |= msk; } while(0)
#define FLAG_UNSET(flg, msk) do { flg &= (~msk); } while (0)
#define FLAG_TRANSFER(flg, src, msk) \
  do { flg = ((flg & (~msk)) | (src & (msk)) ); } while (0)
#define FLAG_CHECK(flg, msk) ((flg & (msk)) == (msk))

#define MAX(a,b) ({ __typeof__ (a) _a = (a);\
    __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

#endif /* ifndef MY_UTIL_H */
