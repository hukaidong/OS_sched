#ifndef MY_UTIL_H

#define MY_UTIL_H value
#define UNUSED(x) (void)(x)
#define NULL 0

#ifdef DEBUG
#include <stdio.h>
#define LOG(str) puts(#str)
#else
#define LOG(str) (void*)0
#endif /* ifdef DEBUG */

#endif /* ifndef MY_UTIL_H */
