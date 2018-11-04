#ifndef MY_UTIL_H

#define MY_UTIL_H value
#define UNUSED(x) (void)(x)

#ifndef NULL
#define NULL 0
#endif

#ifndef ssize_t
#define ssize_t long int
#endif

#ifdef DEBUG
#include <stdio.h>
#define LOG(str) puts(#str)
#else
#define LOG(str) (void*)0
#endif /* ifdef DEBUG */

#endif /* ifndef MY_UTIL_H */
