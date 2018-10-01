#ifndef MY_PTHREAD_INCLUDE
#define MY_PTHREAD_INCLUDE

//#define USE_MY_PTHREAD 1

#ifdef USE_MY_PTHREAD

#include "src/my_pthread.h"

#include "sched.c"
#include "mutex.c"
#include "my_pthread.c"

#endif /* ifdef USE_MY_PTHREAD */


#endif /* ifndef MY_PTHREAD_INCLUDE */


