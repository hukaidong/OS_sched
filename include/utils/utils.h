#ifndef MY_UTIL_H

#define MY_UTIL_H value
#define UNUSED(x) (void)(x)

#include <signal.h>
#include <ucontext.h>
#include <unistd.h>

#ifdef DEBUG
#include <stdio.h>
#define LOG(str) puts(#str)
#else
#define LOG(str)
#endif /* ifdef DEBUG */

#endif /* ifndef MY_UTIL_H */
