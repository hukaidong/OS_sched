#ifndef MY_PTHREAD_INCLUDE
#define MY_PTHREAD_INCLUDE

#define USE_MY_PTHREAD 1
// #define DEBUG

#ifdef USE_MY_PTHREAD

#ifdef DEBUG
#include <stdio.h>
#define LOG(str) puts(#str)
#else
#define LOG(str) (void*)0
#endif /* ifdef DEBUG */



#include "src/my_pthread.h"

#include "sched.c"
#include "mutex.c"
#include "my_pthread.c"

#endif /* ifdef USE_MY_PTHREAD */


#endif /* ifndef MY_PTHREAD_INCLUDE */


